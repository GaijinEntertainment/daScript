// Tutorial 45: Debug Agents
//
// This tutorial covers:
//   - What debug agents are and why they exist
//   - Creating and installing a debug agent (fork + install pattern)
//   - Overriding DapiDebugAgent methods (onCollect, onVariable)
//   - Calling functions in the agent context with invoke_in_context
//   - The auto-start module pattern with [_macro] guards
//
// Prerequisites: Tutorial 44 (Compile and Run) for fork_debug_agent_context
//
// Key concepts:
//   - A debug agent is a daScript object that lives in its own
//     separate "agent context"  a cloned context that stays
//     resident as long as the program is running
//   - `fork_debug_agent_context` clones the current context and
//     calls a setup function inside it
//   - `install_new_debug_agent` registers a DapiDebugAgent subclass
//     under a name, making it globally accessible
//   - DapiDebugAgent has 22 overridable methods for intercepting
//     breakpoints, context lifecycle events, allocations, etc.
//   - `invoke_in_context` calls [export, pinvoke] functions in the
//     agent context from any other context
//
// Run: daslang.exe tutorials/language/45_debug_agents.das

options gen2

require debugapi
require rtti

// ============================================================
// Section 1: Creating and installing a debug agent
// ============================================================
// The fundamental pattern for debug agents is:
//   1. Define a class that extends DapiDebugAgent
//   2. Write a setup function that creates the agent and installs
//      it under a name with `install_new_debug_agent`
//   3. Call `fork_debug_agent_context(@@setup)` to clone the
//      current context and run the setup function in it
//
// The agent lives in its own "agent context"  a separate copy
// of the program that stays resident.  The name you give the
// agent (e.g. "counter") acts as a global key that any context
// can use to find and communicate with this agent.

class CounterAgent : DapiDebugAgent {
    count : int = 0
}

def install_counter(ctx : Context) {
    install_new_debug_agent(new CounterAgent(), "counter")
}

def demo_create_agent() {
    print("=== creating a debug agent ===\n")

    // Before installation
    print("  has 'counter' = {has_debug_agent_context("counter")}\n")

    // Fork + install
    fork_debug_agent_context(@@install_counter)

    // Now the agent is alive
    print("  has 'counter' = {has_debug_agent_context("counter")}\n")

    // output:
    //   has 'counter' = false
    //   has 'counter' = true
}

// ============================================================
// Section 2: Calling functions in the agent context
// ============================================================
// To communicate with the agent, use `invoke_in_context` to call
// [export, pinvoke] functions in the agent context.
//
// `get_debug_agent_context(name)` returns the agent's Context.
// Functions called via invoke_in_context run in that context,
// so they see the agent's copy of module-level variables.
//
// The [pinvoke] annotation is required  it enables the context
// mutex needed for cross-context invocation.
//
// To return values, pass a pointer to a result variable.

// Module-level variable  each context gets its own copy.
// The agent context's copy is the "shared" state.
var agent_counter : int = 0

[export, pinvoke]
def agent_increment() {
    agent_counter++
}

[export, pinvoke]
def agent_get(var result : int?) {
    unsafe {
        *result = agent_counter
    }
}

def demo_invoke_in_context() {
    print("\n=== invoke_in_context ===\n")

    unsafe {
        invoke_in_context(get_debug_agent_context("counter"), "agent_increment")
        invoke_in_context(get_debug_agent_context("counter"), "agent_increment")
        invoke_in_context(get_debug_agent_context("counter"), "agent_increment")
    }

    var result = 0
    unsafe {
        invoke_in_context(get_debug_agent_context("counter"), "agent_get", addr(result))
    }
    print("  agent_counter (in agent) = {result}\n")

    // Our local copy is untouched
    print("  agent_counter (local)    = {agent_counter}\n")

    // output:
    //   agent_counter (in agent) = 3
    //   agent_counter (local)    = 0
}

// ============================================================
// Section 3: State collection agent  onCollect & onVariable
// ============================================================
// `onCollect` is called when `collect_debug_agent_state` is
// triggered.  The agent can inspect the calling context and
// report custom variables.  `onVariable` receives each reported
// variable  this pair is how IDE debuggers show custom watch
// variables, ECS entity state, or application-specific data.
//
// `report_context_state` sends a variable (name, type, value)
// from the agent back to the debug system.

class StateAgent : DapiDebugAgent {
    collection_count : int = 0
    def override onCollect(var ctx : Context; at : LineInfo) : void {
        collection_count++
        // Report collection count as a custom variable
        unsafe {
            let tinfo = typeinfo rtti_typeinfo(collection_count)
            report_context_state(ctx, "Diagnostics", "collection_count",
                unsafe(addr(tinfo)), unsafe(addr(collection_count)))
        }
        // Report the stack depth of the calling context
        let depth = stack_depth(ctx)
        unsafe {
            let tinfo = typeinfo rtti_typeinfo(depth)
            report_context_state(ctx, "Diagnostics", "stack_depth",
                unsafe(addr(tinfo)), unsafe(addr(depth)))
        }
    }
    // onVariable is called for each variable reported above
    def override onVariable(var ctx : Context; category, name : string; info : TypeInfo; data : void?) : void {
        unsafe {
            let value = sprint_data(data, addr(info), print_flags.singleLine)
            print("  {category}: {name} = {value}\n")
        }
    }
}

def install_state_agent(ctx : Context) {
    install_new_debug_agent(new StateAgent(), "my_state")
}

def demo_state_collection() {
    print("\n=== state collection (onCollect + onVariable) ===\n")
    fork_debug_agent_context(@@install_state_agent)

    // Trigger collection  the agent sees the calling context
    collect_debug_agent_state(this_context(), get_line_info(1))
    print("  (second collection)\n")
    collect_debug_agent_state(this_context(), get_line_info(1))

    // output:
    //   Diagnostics: collection_count = 1
    //   Diagnostics: stack_depth = ...
    //   (second collection)
    //   Diagnostics: collection_count = 2
    //   Diagnostics: stack_depth = ...
}

// ============================================================
// Section 4: Agent existence and context access
// ============================================================
// `has_debug_agent_context(name)` checks if a named agent exists.
// `get_debug_agent_context(name)` returns the agent's Context.
//
// Always check existence before accessing the context to avoid
// panics.  Agent contexts stay alive for the program's lifetime
// unless explicitly shut down.

def demo_existence_checks() {
    print("\n=== agent existence checks ===\n")

    // Agents installed in previous sections are still alive
    print("  has 'counter'  = {has_debug_agent_context("counter")}\n")
    print("  has 'my_state' = {has_debug_agent_context("my_state")}\n")
    print("  has 'missing'  = {has_debug_agent_context("missing")}\n")

    // output:
    //   has 'counter'  = true
    //   has 'my_state' = true
    //   has 'missing'  = false
}

// ============================================================
// Section 5: Auto-start module pattern
// ============================================================
// In modules (shared libraries), you often want the agent to be
// installed automatically when the module is loaded  without
// requiring the user to call an init function.
//
// The pattern uses a [_macro] function that runs at compile time.
// Four guards ensure safe, single installation:
//
//   1. is_compiling_macros_in_module("module_name")
//       only runs when THIS module is being compiled
//
//   2. !is_in_completion()
//       skips when the IDE is doing code completion
//      (avoids side effects during auto-complete)
//
//   3. !is_in_debug_agent_creation()
//       prevents recursive agent creation if the agent
//      module itself requires another agent module
//
//   4. !has_debug_agent_context("agent_name")
//       avoids duplicate installation if the module is
//      compiled more than once
//
// Here is the canonical pattern (as used in production modules
// like tbotlog.das and profiler.das):
//
//   [_macro]
//   def private auto_start() {
//       if (is_compiling_macros_in_module("my_module") && !is_in_completion()) {
//           if (!is_in_debug_agent_creation()) {
//               if (!has_debug_agent_context("my_agent")) {
//                   fork_debug_agent_context(@@my_agent_setup)
//               }
//           }
//       }
//   }
//
// We cannot demonstrate [_macro] in a standalone tutorial script
// (it requires a named `module`), but here is the equivalent
// runtime pattern that achieves the same idempotent installation:

def ensure_agent(name : string; setup : function<(ctx : Context) : void>) {
    if (!is_in_debug_agent_creation()) {
        if (!has_debug_agent_context(name)) {
            fork_debug_agent_context(setup)
        }
    }
}

class ServiceAgent : DapiDebugAgent {}

def install_service(ctx : Context) {
    install_new_debug_agent(new ServiceAgent(), "service")
}

def demo_auto_start() {
    print("\n=== auto-start pattern (idempotent) ===\n")

    // First call installs the agent
    ensure_agent("service", @@install_service)
    print("  after first:  has 'service' = {has_debug_agent_context("service")}\n")

    // Second call is a no-op (agent already exists)
    ensure_agent("service", @@install_service)
    print("  after second: has 'service' = {has_debug_agent_context("service")}\n")

    // output:
    //   after first:  has 'service' = true
    //   after second: has 'service' = true
}

// ============================================================
// Section 6: Plain agent as named context host
// ============================================================
// A common pattern is to create a plain DapiDebugAgent (no
// overrides) just to own a named context.  Module-level variables
// in that context become shared state accessible via
// invoke_in_context.  This is the foundation of the
// [apply_in_context] pattern covered in Tutorial 46.

var shared_data : int = 0

[export, pinvoke]
def add_data(amount : int) {
    shared_data += amount
}

[export, pinvoke]
def get_data(var result : int?) {
    unsafe {
        *result = shared_data
    }
}

def install_data_host(ctx : Context) {
    install_new_debug_agent(new DapiDebugAgent(), "data_host")
}

def demo_named_context() {
    print("\n=== plain agent as named context host ===\n")
    fork_debug_agent_context(@@install_data_host)

    // Multiple calls accumulate in the agent's copy
    unsafe {
        invoke_in_context(get_debug_agent_context("data_host"), "add_data", 10)
        invoke_in_context(get_debug_agent_context("data_host"), "add_data", 20)
        invoke_in_context(get_debug_agent_context("data_host"), "add_data", 30)
    }

    var result = 0
    unsafe {
        invoke_in_context(get_debug_agent_context("data_host"), "get_data", addr(result))
    }
    print("  shared_data (in agent) = {result}\n")
    print("  shared_data (local)    = {shared_data}\n")

    // output:
    //   shared_data (in agent) = 60
    //   shared_data (local)    = 0
}

// ============================================================
// Main
// ============================================================

[export]
def main() {
    demo_create_agent()
    demo_invoke_in_context()
    demo_state_collection()
    demo_existence_checks()
    demo_auto_start()
    demo_named_context()
}
