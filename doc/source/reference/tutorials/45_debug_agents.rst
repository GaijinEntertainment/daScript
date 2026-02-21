.. _tutorial_debug_agents:

=============
Debug Agents
=============

.. index::
    single: Tutorial; Debug Agents
    single: Tutorial; DapiDebugAgent
    single: Tutorial; fork_debug_agent_context
    single: Tutorial; invoke_in_context
    single: Tutorial; Named Context

This tutorial covers **debug agents** — persistent objects that live
in their own separate context and can intercept runtime events,
collect state, and host shared data accessible from any context.

Prerequisites: :ref:`tutorial_compile_and_run` (Tutorial 44) for
``invoke_in_context`` basics.

.. code-block:: das

    options gen2

    require debugapi
    require rtti


Creating and installing a debug agent
======================================

A debug agent is a class that extends ``DapiDebugAgent``.  The
fundamental pattern is:

1. Define a class that extends ``DapiDebugAgent``
2. Write a setup function that creates the agent and installs
   it under a name with ``install_new_debug_agent``
3. Call ``fork_debug_agent_context(@@setup)`` to clone the
   current context and run the setup function in it

The agent lives in its own "agent context" — a separate copy
of the program that stays resident:

.. code-block:: das

    class CounterAgent : DapiDebugAgent {
        count : int = 0
    }

    def install_counter(ctx : Context) {
        install_new_debug_agent(new CounterAgent(), "counter")
    }

    def demo_create_agent() {
        print("  has 'counter' = {has_debug_agent_context("counter")}\n")
        fork_debug_agent_context(@@install_counter)
        print("  has 'counter' = {has_debug_agent_context("counter")}\n")
    // output:
    //   has 'counter' = false
    //   has 'counter' = true


Calling functions in the agent context
=======================================

Use ``invoke_in_context`` to call ``[export, pinvoke]`` functions
in the agent context.  ``get_debug_agent_context(name)`` returns the
agent's ``Context``.  Functions run in that context, so they see the
agent's copy of module-level variables — not the caller's.

The ``[pinvoke]`` annotation is required — it enables the context
mutex needed for cross-context invocation.

To return values, pass a pointer to a result variable:

.. code-block:: das

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
        print("  agent_counter (local)    = {agent_counter}\n")
    // output:
    //   agent_counter (in agent) = 3
    //   agent_counter (local)    = 0


State collection — onCollect and onVariable
============================================

``onCollect`` is called when ``collect_debug_agent_state`` is triggered.
The agent can report custom variables via ``report_context_state``.
``onVariable`` receives each reported variable — this is how IDE
debuggers show custom watch variables and application diagnostics:

.. code-block:: das

    class StateAgent : DapiDebugAgent {
        collection_count : int = 0
        def override onCollect(var ctx : Context; at : LineInfo) : void {
            collection_count++
            unsafe {
                let tinfo = typeinfo rtti_typeinfo(collection_count)
                report_context_state(ctx, "Diagnostics", "collection_count",
                    unsafe(addr(tinfo)), unsafe(addr(collection_count)))
            }
        def override onVariable(var ctx : Context; category, name : string;
                                info : TypeInfo; data : void?) : void {
            unsafe {
                let value = sprint_data(data, addr(info), print_flags.singleLine)
                print("  {category}: {name} = {value}\n")
            }
    }

    // Trigger collection
    collect_debug_agent_state(this_context(), get_line_info(1))
    // output:
    //   Diagnostics: collection_count = 1


Agent existence checks
=======================

``has_debug_agent_context(name)`` checks if a named agent exists.
Always check before accessing the context to avoid panics:

.. code-block:: das

    print("  has 'counter'  = {has_debug_agent_context("counter")}\n")
    print("  has 'missing'  = {has_debug_agent_context("missing")}\n")
    // output:
    //   has 'counter'  = true
    //   has 'missing'  = false


Auto-start module pattern
==========================

In modules, agents are installed automatically via a ``[_macro]``
function.  Four guards ensure safe, single installation:

.. code-block:: das

    [_macro]
    def private auto_start() {
        if (is_compiling_macros_in_module("my_module") && !is_in_completion()) {
            if (!is_in_debug_agent_creation()) {
                if (!has_debug_agent_context("my_agent")) {
                    fork_debug_agent_context(@@my_agent_setup)
                }
            }
        }
    }

The guards prevent:

- Running outside the module's own compilation
- Running during IDE code completion
- Recursive agent creation
- Duplicate installation


Plain agent as named context host
==================================

A common pattern is to create a plain ``DapiDebugAgent`` (no
overrides) just to own a named context.  Module-level variables
in that context become shared state accessible via
``invoke_in_context``.  This is the foundation of the
``[apply_in_context]`` pattern (Tutorial 46):

.. code-block:: das

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

    // Multiple calls accumulate in the agent's copy
    unsafe {
        invoke_in_context(get_debug_agent_context("data_host"), "add_data", 10)
        invoke_in_context(get_debug_agent_context("data_host"), "add_data", 20)
    }
    // output:
    //   shared_data (in agent) = 30
    //   shared_data (local)    = 0


Quick reference
===============

==============================================  ==================================================
``fork_debug_agent_context(@@fn)``              Clone context and call setup function in clone
``install_new_debug_agent(agent, name)``        Register agent under a global name
``has_debug_agent_context(name)``               Check if named agent exists
``get_debug_agent_context(name)``               Get agent's Context for invoke_in_context
``invoke_in_context(ctx, fn_name, ...)``        Call [export, pinvoke] function in agent context
``collect_debug_agent_state(ctx, line)``        Trigger onCollect on all agents
``report_context_state(ctx, cat, name, ...)``   Report variable from onCollect to onVariable
``is_in_debug_agent_creation()``                True during fork_debug_agent_context
``[pinvoke]``                                   Annotation enabling context mutex
==============================================  ==================================================


.. seealso::

   Full source: :download:`tutorials/language/45_debug_agents.das <../../../../tutorials/language/45_debug_agents.das>`

   :ref:`tutorial_apply_in_context` — cross-context services via
   ``[apply_in_context]`` annotation (Tutorial 46).

   :ref:`tutorial_compile_and_run` — compiling and running programs
   at runtime (Tutorial 44).

   :ref:`Contexts <contexts>` — language reference for context semantics.

   Previous tutorial: :ref:`tutorial_compile_and_run`

   Next tutorial: :ref:`tutorial_apply_in_context`
