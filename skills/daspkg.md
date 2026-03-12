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
| `search` | `search [query] --json` | Search the package index. Empty query lists all. `--json` for structured output |
| `install` | `install <source> [--force]` | Install a package. Without source, installs all deps from `.das_package` |
| `remove` | `remove <name>` | Remove an installed package |
| `update` | `update [name]` | Re-install at pinned version. Without name, updates all |
| `upgrade` | `upgrade [name]` | Upgrade to latest version. Without name, upgrades all |
| `list` | `list [--json]` | List installed packages |
| `check` | `check [--json]` | Verify installed packages match lockfile |
| `build` | `build` | Build native (CMake) packages |
| `doctor` | `doctor` | Check environment (git, cmake, gh) |
| `introduce` | `introduce` | Register package in the public index (creates PR on daspkg-index) |
| `withdraw` | `withdraw` | Remove package from the public index |

## Install Sources

- **GitHub:** `install github.com/user/repo` — NO `https://` prefix (daspkg prepends it)
- **With version:** `install github.com/user/repo@1.0`
- **Local path:** `install ./path/to/package`
- **Index name:** `install my-package` (looks up in package index)

## Key Details

- Packages install to `modules/<RepoName>/` (e.g. `modules/dasAnthropic/`)
- Lock file: `daspkg.lock` in the `--root` directory
- Package name (in `.das_package`) can differ from repo name
- `install` and `update`/`upgrade` can take 10+ minutes for packages with native builds — use long timeouts
- `--no-color` flag disables ANSI color output (useful for capturing output)
- `--json` flag on `search`, `list`, `check` returns structured JSON

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
require fio

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
