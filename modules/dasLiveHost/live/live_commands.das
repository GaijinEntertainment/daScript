options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4
options strict_smart_pointers = true

module live_commands shared public

//! Live command registration and dispatch.
//!
//! Provides the `[live_command]` annotation and a dispatch mechanism
//! for executing named commands via the REST API lockbox.
//!
//! Usage::
//!
//!     require live/live_commands
//!     [live_command]
//!     def spawn_enemy(input : JsonValue?) : JsonValue? { ... }

require daslib/json
require daslib/json_boost
require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require live_host

// --- Registry ---

var public live_command_registry : table<string; function<(input : JsonValue?) : JsonValue?>>
var public live_command_descriptions : table<string; string>

// --- Help ---

def private live_help_json() : string {
    var commands : table<string; string>
    for (name, desc in keys(live_command_descriptions), values(live_command_descriptions)) {
        commands[name] = desc
    }
    var tab : table<string; JsonValue?>
    tab |> insert("commands", JV(commands))
    return write_json(JV(tab))
}

// --- Dispatch (called from [before_update]) ---

[export]
def public live_dispatch_command(command_json : string) : string {
    //! Parses a command JSON string, looks up the command name in the registry,
    //! calls the handler, and returns the response as a JSON string.
    var parse_error = ""
    var js = read_json(command_json, parse_error)
    if (js == null) {
        return write_json(JV("parse error: {parse_error}"))
    }
    let name = js?.name ?? ""
    if (empty(name)) {
        return write_json(JV("missing command name"))
    }
    if (name == "help") {
        return live_help_json()
    }
    if (!key_exists(live_command_registry, name)) {
        return write_json(JV("unknown command: {name}"))
    }
    var args : JsonValue?
    if (js.value is _object) {
        let tab & = unsafe(js.value as _object)
        args = unsafe(reinterpret<JsonValue?>(tab?["args"] ?? null))
    }
    let handler = live_command_registry[name]
    var result = invoke(handler, args)
    if (result != null) {
        return write_json(result)
    }
    return write_json(JV(true))
}

// --- Macro ---

[function_macro(name="live_command")]
class LiveCommandMacro : AstFunctionAnnotation {
    //! Registers a function as a live command. The function must have signature
    //! `def name(input : JsonValue?) : JsonValue?`. The command is registered
    //! in `live_command_registry` at context initialization.
    def override apply(var func : FunctionPtr; var group : ModuleGroup; args : AnnotationArgumentList; var errors : das_string) : bool {
        let commandName = "{func.name}"
        var description = ""
        let p = args |> find_arg("description")
        if (p is tString) {
            description = p as tString
        }
        // Create or get the [init] function that populates the registry
        var reg <- setup_call_list("register`live`commands", func.at, true/*isInit*/, true/*isPrivate*/)
        // Append: live_command_registry |> insert("commandName", @@func_name)
        reg.list |> emplace_new <| qmacro(live_commands::live_command_registry |> insert($v(commandName), @@$c("_::{func.name}")))
        reg.list |> emplace_new <| qmacro(live_commands::live_command_descriptions |> insert($v(commandName), $v(description)))
        return true
    }
}
