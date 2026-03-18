# daslang-live (Live-Reloading Host)

## Philosophy

**The live app never restarts.** The entire point of `daslang-live.exe` is continuous execution with hot-reload. You edit the `.das` file, save it, and the running application reloads instantly — preserving windows, audio, game state, and everything else stored in the persistent byte store.

**NEVER kill the `daslang-live.exe` process** unless:
- The C++ host binary itself was recompiled (requires restart)
- An unrecoverable crash/hang (rare)

Instead of restarting, use:
- **Save the file** → auto-reload via file watcher
- `POST /reload` → trigger reload via REST API
- `POST /reload/full` → deeper recompilation if needed

**Always persist important state** via `[before_reload]`/`[after_reload]` so reloads are seamless. The goal is that a reload feels invisible — the user sees their code changes take effect without losing any runtime state.

## Overview

`daslang-live.exe` is a live-reloading application host. It compiles and runs a daScript file, then watches for file changes and hot-reloads the script without restarting the process. It also exposes a REST API for remote control.

**Binary:** `bin/Release/daslang-live.exe`

## Running

```bash
# Live-reload mode (recommended for development)
bin/Release/daslang-live.exe path/to/main.das

# With a project file
bin/Release/daslang-live.exe -project path/to/project.das_project path/to/main.das

# Standalone mode (same as daslang.exe — if script only has main())
bin/Release/daslang-live.exe path/to/main.das
```

Scripts can also run standalone with `bin/Release/daslang.exe path/to/main.das` — the same `main.das` works in both hosts. Under `daslang.exe`, `is_live_mode()` returns `false`, the REST API does not start, and `[before_reload]`/`[after_reload]` annotations are ignored.

## Mode Detection

- If script exports `init()` → **lifecycle mode**: host calls `init()`, then loops `update()`, calls `shutdown()` on exit
- If script only exports `main()` → **simple mode**: host calls `main()` directly (same as `daslang.exe`)

## Lifecycle Functions

Scripts in lifecycle mode must export:

```das
[export] def init() { ... }      // Called once on start and after each reload
[export] def update() { ... }    // Called every frame
[export] def shutdown() { ... }  // Called on exit and before each reload
```

## Core Module: `live_host`

`require live_host` — provides the lifecycle API:

| Function | Description |
|---|---|
| `is_live_mode() : bool` | `true` under daslang-live, `false` under daslang.exe |
| `request_exit()` | Signal the host to exit |
| `exit_requested() : bool` | Check if exit was requested |
| `request_reload(full : bool)` | Trigger a reload (`full` = full recompile) |
| `is_reload() : bool` | `true` during init() after a reload (not first run) |
| `get_dt() : float` | Frame delta time in seconds |
| `get_uptime() : float` | Time since start in seconds |
| `get_fps() : float` | Current FPS |
| `is_paused() : bool` | Whether the host is paused |
| `set_paused(v : bool)` | Pause/unpause the host |
| `get_last_error() : string` | Last compilation error (after failed reload) |
| `live_store_bytes(key, data)` | Store bytes in persistent store (survives reloads) |
| `live_load_bytes(key, data) : bool` | Load bytes from persistent store |

## Reload Annotations

```das
[before_reload]
def save_state() { ... }   // Called before shutdown during reload — save state here

[after_reload]
def restore_state() { ... } // Called after restart, BEFORE init() — restore state here

[before_update]
def pre_tick() { ... }      // Called every frame before update() — used by live_api agent
```

## Helper Modules

### `live/glfw_live` — GLFW Window Lifecycle

Manages a GLFW window that persists across reloads. Provides:

- `live_window : GLFWwindow?` — the current window pointer
- `live_create_window(title, w, h) : GLFWwindow?` — create or reuse window
- `live_destroy_window()` — destroy (skipped during reload)
- `live_begin_frame() : bool` — poll events, return false if window closed
- `live_end_frame()` — swap buffers
- `live_get_framebuffer_size(var w, h : int&)` — current framebuffer size

### `live/opengl_live` — OpenGL Utilities

Provides the `screenshot` live command (captures framebuffer to PNG).

### `live/decs_live` — DECS Persistence

`require live/decs_live` — automatically serializes/restores DECS entity state across reloads via `mem_archive_save`/`mem_archive_load` on `decsState`. Just require it — no boilerplate needed. Works with POD templates (float3, int, string, etc.) — only fails if templates contain pointers, lambdas, or other non-serializable types.

**IMPORTANT:** When using `decs_live`, guard `decs::restart()` and initial entity spawning with `is_reload()`:

```das
def init() {
    if (!is_reload()) {
        decs::restart()
        spawn_initial_entities()
        commit()
    }
}
```

Without this guard, `decs::restart()` wipes all entities that `decs_live` just restored in `[after_reload]` (which runs before `init()`).

### `live/live_commands` — Command Registration

Provides the `[live_command]` annotation for registering commands dispatchable via REST API.

### `live/live_watch` — File Watcher

Auto-installs as a debug agent. Polls `stat()` on watched files and triggers reload when mtime or size changes. Checks every 0.5 seconds using `get_uptime()` for throttling (high-resolution timer). Uses both mtime and file size to catch rapid edits that land within the same second.

**Important:** `live_watch` only watches files the host registers (currently just the main script file, not its dependencies). The `[_macro]` in `live_watch` auto-installs the debug agent on first compilation under `daslang-live.exe`.

### `live/live_watch_boost` — Watcher Diagnostics

`require live/live_watch_boost` — adds diagnostic `[live_command]` functions AND pulls in `live_watch` (so you get the watcher automatically). Commands:

| Command | Description |
|---|---|
| `cmd_watch_status` | Show watched files with stat info (mtime, size), agent status |
| `cmd_watch_touch` | Touch first watched file to trigger watcher reload |
| `cmd_watch_signal` | Directly call `signal_files_changed()` to trigger reload |

**Prefer `require live/live_watch_boost`** over `require live/live_watch` — it gives you both the watcher and the diagnostic commands.

## REST API (`live/live_api`)

When `require live/live_api` is present, an HTTP server starts on port 9090 (configurable via `live_api_set_port()`). Auto-installs as a debug agent.

### Built-in Endpoints

| Method | Path | Description |
|---|---|---|
| GET | `/status` | JSON: fps, uptime, paused, dt, has_error |
| GET | `/error` | Plain text: last compilation error |
| POST | `/reload` | Trigger reload |
| POST | `/reload/full` | Trigger full reload |
| POST | `/pause` | Pause the host |
| POST | `/unpause` | Unpause the host |
| POST | `/command` | Dispatch a live command (JSON body) |

### Command Dispatch

POST to `/command` with JSON body `{"name": "command_name", "args": {...}}`.

The special command `help` returns all registered commands and their descriptions.

### Using curl

```bash
# Check status
curl http://localhost:9090/status

# List all commands
curl -X POST http://localhost:9090/command -d '{"name":"help"}'

# Execute a command with args
curl -X POST http://localhost:9090/command -d '{"name":"cmd_spawn_ball","args":{"x":0,"vx":0.5}}'

# Screenshot
curl -X POST http://localhost:9090/command -d '{"name":"screenshot","args":{"file":"capture.png"}}'

# Trigger reload
curl -X POST http://localhost:9090/reload
```

## Writing Live Commands

Use the `[live_command]` annotation on functions with signature `(input : JsonValue?) : JsonValue?`. Use structs with `from_JV`/`JV` for clean arg parsing and response building — **never parse JSON manually**.

```das
require live/live_commands
require daslib/json
require daslib/json_boost

// Args struct — field defaults are used when the field is missing from JSON
struct CmdDoThingArgs {
    x : float = 0.0
    y : float = 0.0
}

// Response struct
struct CmdDoThingResult {
    result : float
    ok : bool
}

[live_command(description="Do something. Args: x, y")]
def cmd_do_thing(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<CmdDoThingArgs>)
    // ... do the thing ...
    return JV(CmdDoThingResult(result = args.x + args.y, ok = true))
}
```

**Key patterns:**
- **Parse args:** `from_JV(input, type<MyArgs>)` — returns a struct, uses defaults for missing fields
- **Return response:** `JV(my_struct)` — serializes struct to JSON
- **Simple responses:** `JV(true)`, `JV("error message")`, `JV(42)`
- **Enum args:** `from_JV` converts `"fireball"` → `BonusType.fireball` automatically
- **Reserved words:** Use `@rename = "type" _type : BonusType` to map JSON key `"type"` to field `_type`
- **IMPORTANT:** `from_JV(input, type<T>)` **returns** the struct — it does NOT modify the second argument in-place. The second arg is a type template.

**Built-in commands** (from helper modules):
- `screenshot` — from `live/opengl_live`, captures framebuffer to PNG
- `help` — built-in, lists all registered commands

**Convention:** Prefix game-specific commands with `cmd_` to distinguish them from built-in commands.

## State Preservation Across Reloads

The persistent byte store (`live_store_bytes` / `live_load_bytes`) survives reloads. Use `Archive` for serialization:

```das
[before_reload]
def save_my_state() {
    var ser = new MemSerializer()
    var arch = Archive(reading = false, stream = ser)
    arch |> serialize(my_variable)
    var data <- ser->extractData()
    live_store_bytes("__my_key", data)
    unsafe { delete ser; }
}

[after_reload]
def restore_my_state() {
    var data : array<uint8>
    if (live_load_bytes("__my_key", data) && length(data) > 0) {
        var ser = new MemSerializer(data)
        var arch = Archive(reading = true, stream = ser)
        arch |> serialize(my_variable)
        unsafe { delete ser; }
    }
}
```

**Pointer preservation** (GLFW window, audio handles): serialize as `uint64` via `reinterpret`, restore and cast back. The underlying C/C++ objects live in the DLL and survive reload.

### `live/live_vars` — `@live` Variable Macro

`require live/live_vars` — annotate globals with `@live` for automatic serialization across reloads. Replaces manual `[before_reload]`/`[after_reload]` boilerplate entirely.

```das
require live/live_vars

var @live score = 0
var @live lives = 3
var @live game_state = GameState.menu
var @live paddle_x = 0.0
```

The macro generates `__before_reload_live_vars` and `__after_reload_live_vars` functions that serialize all `@live`-tagged globals via `Archive`. The host discovers these by name prefix.

**What works:** All serializable types — `int`, `float`, `string`, `float3`, enums, structs (if all fields are serializable), `array<T>`, `table<K;V>`.

**What doesn't work:** Pointers, lambdas, handles (GLFW windows, audio handles). For these, use manual `[before_reload]`/`[after_reload]` with `reinterpret<uint64>`.

**Format mismatch safety:** Adding/removing/reordering `@live` variables between reloads changes the binary format. Deserialization is wrapped in `try/recover` — mismatches degrade to defaults instead of crashing.

**Store key:** `"__live_vars_{module_name}"` — module-specific to avoid collisions.

## Typical Requires for a Live App

```das
require live/glfw_live       // GLFW window lifecycle
require live/opengl_live     // screenshot command
require live/live_commands   // [live_command] annotation
require live/live_api        // REST API server
require live_host            // core lifecycle API
// Optional:
require live/live_vars        // @live variable auto-persistence
require live/decs_live       // DECS persistence (if templates serialize)
require live/live_watch_boost // file watcher + diagnostic commands
require audio/audio_live     // audio sample persistence across reloads
```

## Debugging

- **Failed reload**: host pauses, old context re-initializes. Check `GET /error` for the compilation error.
- **Exception in update()**: host pauses. Fix the script, save → auto-reload.
- **GC**: host runs `collectHeap` automatically between frames when heap fragmentation is high.
- **Persistent heap**: `policies.persistent_heap = true` is set by the host — string builders work without `unsafe`.

## Gotchas

- **`.das_module` changes require restart.** Path registration (`register_native_path`) only runs at process startup. Adding a new file to `.das_module` won't take effect until `daslang-live.exe` is restarted.
- **`get_uptime()` does NOT reset on reload.** It's a host-level timer (time since process start). Don't use it to detect reloads — use `is_reload()` in `init()` instead.
- **Debug agent globals are per-context.** Shared module globals in the debug agent context are separate copies from the main context. You can't read agent state from main-context commands via shared globals. Use C++ `g_state` (host-level) for cross-context communication.
- **`[live_command]` can't be used in the module that defines it.** The `LiveCommandMacro` in `live_commands.das` is a `[function_macro]` — it's not available during compilation of `live_commands.das` itself. Put commands in a separate module that `require live/live_commands`. This is why `live_watch_boost.das` exists as a separate file from `live_watch.das`.
- **Debug agents persist across reloads.** Once `fork_debug_agent_context` creates an agent, the agent's code is NOT updated on reload — it keeps running the version from its original compilation. Changing agent code (e.g. `live_watch.das`) requires restarting the process.
