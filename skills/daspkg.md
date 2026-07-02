# Package Manager (daspkg)

Read this skill file before running daspkg commands or working with `.das_package` manifests.

## CLI Usage

Run daspkg via the daslang interpreter:

```bash
bin/daslang utils/daspkg/main.das -- <command> [args] --root <project_root>
```

The `--root` flag sets the project root directory (default: current directory). All commands operate relative to this root.

## Commands

| Command | Usage | Description |
|---|---|---|
| `search` | `search [query] [--json]` | Search the package index. Empty query lists all |
| `install` | `install <source> [--force] [--global]` | Install a package. Without source, installs all deps from `.das_package` |
| `remove` | `remove <name> [--global]` | Remove an installed package |
| `update` | `update [name] [--global]` | Re-install at pinned version. Without name, updates all |
| `upgrade` | `upgrade [name] [--global]` | Upgrade to latest version. Without name, upgrades all |
| `list` | `list [--json] [--global]` | List installed packages |
| `check` | `check [--json] [--global]` | Verify installed packages match lockfile |
| `build` | `build [--global]` | Build native (CMake) packages |
| `build --wasm` | `build --wasm [--wasm-lib-dir <dir>]` | Build the wasm64 runtime + module archives `release wasm` links against (see WebAssembly section) |
| `cleanup` | `cleanup [--force] [--global]` | Remove `modules/` and `daspkg.lock` to reset a project |
| `doctor` | `doctor` | Check environment (git, cmake, gh) |
| `release` | `release [--out <dir>]` | Bundle project as a redistributable standalone (exe + shared modules + assets) |
| `release wasm` | `release wasm --root <dir> [--out <dir>] [--wasm-lib-dir <dir>]` | Cross-compile the project to a standalone wasm64 web app (`<name>.{html,js,wasm}`) — see WebAssembly section |
| `introduce` | `introduce` | Register package in the public index (creates PR on daspkg-index) |
| `withdraw` | `withdraw` | Remove package from the public index |

## Install Sources

- **GitHub:** `install github.com/user/repo` — NO `https://` prefix (daspkg prepends it)
- **With version:** `install github.com/user/repo@1.0` (also accepts `@v1.0` — the `v` prefix is stripped automatically)
- **Local path:** `install ./path/to/package`
- **Index name:** `install my-package` (looks up in package index)

## Options

| Flag | Description |
|---|---|
| `--root <path>` | Project root directory (default: current directory) |
| `--force` | Force reinstall (overrides duplicate/version checks) |
| `--global`, `-g` | Operate on global modules in `{das_root}/modules/` (see below) |
| `--color` | Enable colored output |
| `--no-color` | Disable colored output (useful for capturing output) |
| `--verbose`, `-v` | Print detailed progress |
| `--json` | Machine-readable JSON output (`search`, `list`, `check`) |
| `--out <path>` | Output directory for `release` / `release wasm` (default: current directory) |
| `--wasm` | (on `build`) Build the wasm64 runtime + module archives instead of native CMake packages |
| `--wasm-lib-dir <path>` | Directory holding the wasm64 archives (`liblibDaScript_runtime.a` + `liblibDasModule*.a`); default `<das_root>/web/output64/lib` |

## Key Details

- Packages install to `{root}/modules/<RepoName>/` (e.g. `modules/dasAnthropic/`)
- Lock file: `daspkg.lock` in the `--root` directory
- Package name (in `.das_package`) can differ from repo name
- `install` and `update`/`upgrade` can take 10+ minutes for packages with native builds — use long timeouts

## Global Modules

Large packages (e.g. dasImgui) can be installed **globally** — once under `{das_root}/modules/` — and shared across all projects using that daScript SDK. This avoids redundant clones and builds.

### Usage

```bash
# Install globally (to das_root/modules/)
daspkg install --global dasImgui
daspkg install --global github.com/user/dasImgui@1.0

# List globally installed packages
daspkg list --global

# Update/upgrade globally
daspkg update --global dasImgui
daspkg upgrade --global dasImgui

# Remove globally
daspkg remove --global dasImgui

# Build all global native packages
daspkg build --global

# Check global packages
daspkg check --global
```

### Install behavior

- **Global install** (`--global`): clones and builds in `{das_root}/modules/`, records in `{das_root}/modules/.daspkg_global.lock`
- **Local install auto-uses global:** `daspkg install foo` checks the global lock file first. If a compatible global version exists, it records a reference (`"global": true` in project lock file) instead of cloning — zero network, zero build time
- **Version mismatch:** if the global version doesn't satisfy the project's requested version, daspkg errors with a suggestion. Use `--force` to install locally, or `--global` to update the global copy
- **Dependencies:** global packages' dependencies also install globally. Built-in SDK modules (already in `das_root/modules/`) are automatically skipped

### Coexistence (local + global)

A package can exist both locally and globally. The C++ runtime (`require_dynamic_modules`) handles this via **shadow detection**:

- If the same module directory exists in both `{das_root}/modules/` and `{project_root}/modules/`, the **local version wins**
- A warning is printed: `"Warning: local 'dasImgui' shadows global — using local"`
- This is safe — removing the local copy seamlessly falls back to the global one

### Remove behavior

- `daspkg remove --global foo` — deletes `{das_root}/modules/foo/` and removes from global lock file
- `daspkg remove foo` (where project entry has `"global": true`) — only removes the project lock file reference; the global directory is not deleted

### CMake integration

Global packages that use `cmake_build()` or `custom_build()` get a `.daspkg_standalone` marker file. The main daScript `CMakeLists.txt` skips directories with this marker during auto-discovery, preventing `FATAL_ERROR` from standalone CMakeLists.txt files (e.g. dasImgui requires `DASLANG_DIR` to be set explicitly).

## `.das_package` Manifest

Every package has a `.das_package` file — a daslang script declaring metadata:

```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("my-package")
    package_author("username")
    package_description("What it does")
    package_source("github.com/user/repo")
    package_license("MIT")
    package_tag("networking")
    package_min_sdk("0.4")
}

[export]
def resolve(sdk_version, version : string) {
    if (empty(version) || version == "latest") {
        download_branch("main")
    } else {
        download_tag("v{version}")
    }
}

[export]
def dependencies(version : string) {
    require_package("github.com/user/dep1")
    require_package("github.com/user/dep2", ">=1.0")
}

[export]
def build() {
    cmake_build()       // or: custom_build("make all"), or: no_build() (default)
}
```

## `.das_module` for Packages

Registers the module so projects can `require` it after `daspkg install`:

```das
options gen2
require daslib/fio

[export]
def initialize(project_path : string) {
    register_native_path("namespace", "module", "{project_path}/namespace/module.das")
}
```

## Release — bundle project as a standalone

`daspkg release` produces a redistributable folder under `<--out>/<bundle_name>/` containing:

- The standalone exe (`<bundle_name>.exe` — produced by `daslang -exe`).
- Every `.shared_module` dylib the program transitively requires, at the path the exe expects (`modules/<X>/<X>.shared_module`).
- Every asset matching the project's `release_include` globs.

The recipient can run the bundled exe **without daslang installed**. PR #1 (exe-relative shared modules, merged 2026-05-05) is the prerequisite — the runtime resolves dylibs against the exe's own directory first.

### `release()` hook in `.das_package`

A package's `.das_package` declares what `daspkg release` should ship:

```das
[export]
def release() {
    release_main("main.das")            // entry script (required for the project being released)
    release_name("MyApp")               // optional; defaults to package_name() / root dir
    release_include("data/**")          // ship matching files (glob; multiple calls accumulate)
    release_include("*.png")
    release_exclude("data/secret/**")
    release_shared_module("dasSQLITE")  // force-include a dylib not auto-detected
}
```

For a **dep package** (e.g. `dasCards`), only the asset-glob calls matter — its `release_main` is ignored when releasing a parent project that depends on it.

### Auto-detection — shared modules + transitive dep traversal

`daslang -exe --list-shared-modules <path>` writes a JSON file describing the program's deps. `cmd_release` then:

1. **Ships dylibs**: every `.shared_module` registered for a daslang module that the compiled program references. Detection is by program-module membership (not used-function set) — a `require`d module whose .das functions aren't called still gets shipped, because its native data may be loaded at runtime.
2. **Walks transitive deps**: for every program module whose .das source file lives under a `<root>/modules/<DepName>/` package (i.e. has a `.das_package` ancestor distinct from the project's own root), runs that dep's `release()` and copies its `release_include` files to `<bundle>/modules/<DepName>/<rel>`.

If a module is loaded only at runtime (e.g. data files read while the .das is never `require`d), use `release_shared_module(name)` to force-include it. Argument can be the daslang-side module name (`"sqlite"`) OR the package directory name (`"dasSQLITE"`).

### Output layout

```
<out>/
└── <bundle_name>/
    ├── <bundle_name>.exe                       # the standalone binary
    ├── modules/                                # auto-detected dylibs + transitive dep assets
    │   ├── dasGlfw/dasModuleGlfw.shared_module
    │   ├── dasStbImage/dasModuleStbImage.shared_module
    │   ├── dasCards/cards/svg-cards.svg        # from dasCards's release_include
    │   └── ...
    ├── <asset paths from project's release_include>  # preserved relative to project root
    └── ...
```

The bundle is the host platform only. Cross-compilation is deferred until daslang itself supports it; v1 has no platform-tag suffix or auto-archive (`--zip` etc.). Recipients can tar/zip the directory themselves.

### Runtime asset paths — `get_this_module_dir()`

Dep code that loads data files via `dir_name(get_module_file_name("X"))` gets the **build-machine path**, not the bundle path — `get_module_file_name` returns the `.das` file path baked at compile time. Use `get_this_module_dir()` from `daslib/module_path` instead:

```das
require daslib/module_path

def load_assets() {
    let dir = get_this_module_dir()
    let svg = "{dir}/cards.svg"
    // ...
}
```

The macro captures the call-site source file path at expansion, then walks the same 3-tier resolution PR #2579 introduced for `.shared_module`:

1. `<exe_dir>/<rel>` — daspkg release standalone bundle
2. `<das_root>/<rel>` — SDK / cmake install
3. `dir_name(<call-site-baked>)` — dev (interpreted from source)

`<rel>` is the suffix starting at the last `/modules/` segment.

**Project-local code (no `/modules/` in path)** skips tiers 1+2 and goes straight to tier 3, which returns the baked dir if it still exists (dev) or `<exe_dir>` if it doesn't (relocated bundle). So `path_join(get_this_module_dir(), "asset.png")` from a project-local `main.das` works in both dev and shipped bundles, as long as the asset is shipped sitting next to the exe.

**Call from inside a function — not a top-level `let` initializer.** daslang's `-exe` JIT path has a known limitation: top-level `let`s that call Context-allocating builtins (like `get_this_module_dir()`, `get_das_root()`, `dir_name(get_module_file_name(...))`) emit JIT-process-baked function pointers that are wrong under ASLR in the standalone exe. The same call works fine inside any function body. Tracked separately; once the daslang fix lands, the top-level form will Just Work.

### What does **not** ship

- Source `.das` files compiled into the exe (daslib stdlib, project's own helpers, dep `.das` companions). The runtime exe doesn't need them — they're already baked in.
- Source files alongside `.shared_module` dylibs (CMakeLists, README, etc.).
- The `.das_package` and `daspkg.lock` files.
- Anything not matched by a `release_include` glob.

## WebAssembly (wasm64) — `build --wasm` / `release wasm`

`daspkg release wasm` cross-compiles a project to a **standalone wasm64 web app** (`<name>.{html,js,wasm}`) that runs compiled (not interpreted) in the browser. It is the web analogue of native `release`: one `.das_package` per example, driven by the same `release()` hook.

**wasm64 ONLY.** Every compiled web artifact is memory64 (8-byte pointers). The wasm32 *cross-compile* target is unsupported — on a 64-bit host it bakes 8-byte-pointer layouts the wasm32 runtime reads as 4 (garbage); pure scripts only ever "worked" at wasm32 by dodging that bug class. wasm64 makes host-ptr-size == target-ptr-size, so the bug class vanishes with zero codegen changes. The browser needs memory64 (Chrome/Edge/Firefox 133+); on Safari/iOS the site falls back to the wasm32 *interpreter* (`daslang_static.wasm`, an ordinary C++→wasm build — not the cross-compiler, so unaffected).

### Two-step workflow

```bash
# 1. Build the wasm64 runtime + module archives (once; emsdk must be active).
daspkg build --wasm
#    -> configures web/build64 (-DDAS_WASM_MEMORY64=ON), builds the runtime +
#       OpenGL/Glfw/StbImage/Audio/Minfft archives, consolidates into web/output64/lib

# 2. Release each example to a wasm64 web app.
daspkg release wasm --root examples/games/arcanoid --out _release
#    -> arcanoid.{html,js,wasm}
```

`release wasm` per example: cross-compiles the entry script to a wasm64 object, reads `--list-shared-modules` to know which module archives to link, runs `emcc` (memory64 + WebGL2 flags + each module's `release_emcc_arg`/`release_embed_file` + the shell file), and emits `<name>.{html,js,wasm}`.

### Prerequisites

- **emsdk active** — `emcc` + `cmake` on PATH (CI activates `emsdk_env` first).
- **wasm64 archives** — from `daspkg build --wasm` (in `web/output64/lib`, or pass `--wasm-lib-dir`).
- **Host shared modules** (graphics examples only) — the host daslang must have `modules/dasGlfw/dasModuleGlfw.shared_module` + `modules/dasOpenGL/dasModuleOpenGL.shared_module` present, or GL silently falls back to the broken dasbind path (black canvas + runtime "Failed to find @dasbind::glCreateShader"). Guard before release: the cross-compile log must show `NEED_MODULE(Module_dasOpenGL) for opengl` (native), not a `@dasbind` binding. Pure compute scripts need no shared modules.

### `release()` hooks for web

```das
[export]
def release() {
    release_main("main.das")            // native bundle: desktop entry (e.g. live-reload stack)
    release_wasm_main("main_web.das")   // wasm entry; falls back to release_main if unset
}
```

`release_wasm_main` exists because the desktop entry often `require`s host-only stacks (live-reload, libhv HTTP, threaded jobque) that don't cross-compile. The web variant swaps those for a `live_stub` shim (plain GLFW window + frame clock + no-op `[live_command]`) and guards music off (threaded strudel needs the jobque worker; SFX is single-threaded and stays on). One `.das_package` drives both targets.

**Module-side hooks** (in a module's own `.das_package`, accumulated across all linked modules):

| Hook | Effect |
|---|---|
| `release_emcc_arg("-sUSE_GLFW=3")` | Append an emcc link flag (dasGlfw declares this) |
| `release_embed_file(src, dst)` | `--embed-file src@dst` into MEMFS (dasStbImage embeds its HUD font) |
| `release_web_shell("path.html")` | Override the default canvas shell (`web/templates/wasm_canvas_shell.html`) |

### Game source contract (cross-compiles UNCHANGED)

A game keeps its plain desktop `main` — on wasm the browser lifecycle auto-drives `init`/`update`/`shutdown` per `requestAnimationFrame` and **bypasses `main`**. No `eval_main_loop` / per-platform edits. (Graphics examples may need `options stack = 8_388_608` — decs nested queries overflow the 16 KB default context stack in the cross-compiled exe.)

## Resetting a Project (cleanup)

Use `cleanup` to wipe a consumer project's daspkg state so the next `install` starts from scratch — useful when auditing packages or recovering from a bad partial install.

```bash
# dry-run: show what would be removed
daspkg cleanup --root examples/graphics

# actually remove
daspkg cleanup --root examples/graphics --force
```

What `cleanup` removes:
- `{root}/modules/` (recursively)
- `{root}/daspkg.lock`
- `.gitignore` entries daspkg had added (one line per non-global package in the lockfile)

Safety rails:
- Refuses to run in a directory that has neither `.das_package` nor `daspkg.lock` — prevents nuking arbitrary `modules/` dirs
- `--force` is required to actually delete; without it, `cleanup` prints a plan and exits
- `--global --force` removes every globally-installed daspkg package from `{das_root}/modules/` and the global lockfile (in-tree SDK modules are left alone since they're never listed in the global lockfile)

## Running Package Tests

From the package root:

```bash
bin/daslang dastest/dastest.das -- --test test_module.das
```
