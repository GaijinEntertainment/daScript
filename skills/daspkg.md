# Package Manager (daspkg)

Read this skill file before running daspkg commands or working with `.das_package` manifests.

## CLI Usage

Run daspkg via the daslang interpreter:

```bash
# From repo root (development)
bin/Release/daslang.exe utils/daspkg/main.das -- <command> [args] --root <project_root>

# From installed SDK
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
| `doctor` | `doctor` | Check environment (git, cmake, gh) |
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

## Package Index

- Public index: `borisbat/daspkg-index` on GitHub
- `introduce` creates PRs on the index repo (NOT on `GaijinEntertainment/daScript`)
- `search` queries the index for matching packages

## Running Package Tests

From the package root:

```bash
bin/Release/daslang.exe dastest/dastest.das -- --test test_module.das
```
