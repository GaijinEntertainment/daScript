# daspkg — daslang package manager

Package manager for [daslang](https://dascript.org/). Installs, updates, builds, and manages daslang modules from git repositories or a central package index.

## Quick start

```bash
# run daspkg
daslang utils/daspkg/main.das -- <command> [args...]

# install a package by name (from the index)
daslang utils/daspkg/main.das -- install daspkg-test-pure

# install a package by URL
daslang utils/daspkg/main.das -- install github.com/borisbat/daspkg-test-pure

# install a specific version
daslang utils/daspkg/main.das -- install github.com/borisbat/daspkg-test-versions@v1.0

# install all dependencies listed in .das_package
daslang utils/daspkg/main.das -- install

# update all packages
daslang utils/daspkg/main.das -- update
```

## Commands

| Command | Description |
|---------|-------------|
| `install <url\|name\|path>[@version]` | Install a package from git, index, or local path |
| `install` (no args) | Install all dependencies from `.das_package` |
| `remove <name>` | Remove an installed package |
| `update [name]` | Update one or all packages |
| `list` | List installed packages |
| `search <query>` | Search the package index |
| `build` | Build all C/C++ packages (cmake) |
| `check` | Verify installed packages are present |
| `doctor` | Check environment (git, cmake, gh) |
| `introduce [url]` | Submit a package to the index via PR |
| `withdraw <name>` | Remove a package from the index via PR |

Options: `--root <path>` (project root, default `.`), `--force` (force reinstall).

## .das_package manifest

Every package is described by a `.das_package` file — an executable daslang script that registers metadata, version resolution, dependencies, and build info. This replaces static JSON manifests with programmable logic.

```das
options gen2

require daslib/daspkg

// Required: package identity
[export]
def package() {
    package_name("mymodule")
    package_author("username")
    package_description("What this module does")
    package_source("github.com/user/mymodule")
}

// Optional: how to resolve a version request into a git ref
[export]
def resolve(sdk_version, version : string) {
    if (version == "" || version == "latest") {
        download_tag("v2.0")          // checkout a tag
    } elif (version == "1.x") {
        download_tag("v1.5")
    } else {
        download_tag("v{version}")    // e.g. "1.2.3" -> tag "v1.2.3"
    }
    // alternatives:
    //   download_branch("main")
    //   download_redirect("github.com/other/repo", "v3.0")
}

// Optional: dependencies on other packages
[export]
def dependencies(version : string) {
    require_package("github.com/user/dep-a", ">=1.0")
    require_package("github.com/user/dep-b")
}

// Optional: build step (default: no build)
[export]
def build() {
    cmake_build()                     // cmake configure + build
    // alternatives:
    //   custom_build("make all")
    //   no_build()
}
```

### Version resolution

The `resolve()` function decides what git ref to check out. It receives the daslang SDK version and the user-requested version, and calls one of:

| Function | Effect |
|----------|--------|
| `download_tag("v1.0")` | Fetch and checkout git tag `v1.0` |
| `download_branch("main")` | Fetch and checkout branch `main` |
| `download_redirect("github.com/org/new-repo", "v2.0")` | Re-clone from a different repository and checkout tag `v2.0` |

If `.das_package` has no `resolve()` function, the version string (from `install foo@v1.0`) is used as a tag directly. If no version is specified, the default branch is used.

### Install flow

1. Shallow-clone the package from the default branch
2. If `.das_package` exists and has `resolve()` — call it, checkout the resolved tag/branch (or re-clone on redirect)
3. If resolve returned nothing and a version was requested — checkout version as tag
4. Move to `modules/<name>/`
5. Record in `daspkg.lock`
6. Install transitive dependencies (from `.das_package` `dependencies()`)
7. Auto-build if `.das_package` has `build()` (cmake or custom)

## Project layout

### Source modules (`utils/daspkg/`)

| File | Description |
|------|-------------|
| `main.das` | CLI entry point — parses args, dispatches to commands |
| `commands.das` | Command implementations: install, remove, update, build, check, doctor |
| `index.das` | Package index: fetch, search, introduce, withdraw |
| `lockfile.das` | `daspkg.lock` read/write (JSON serialization of installed packages) |
| `package_runner.das` | In-process `.das_package` compiler — compiles, simulates, and extracts metadata |
| `utils.das` | Shared utilities: `run_cmd`, `package_name_from_source`, `copy_dir_rec` |
| `DESIGN.md` | Original design document |

### API module (`daslib/daspkg.das`)

The module that `.das_package` scripts `require`. Provides the registration functions:

- **Metadata:** `package_name()`, `package_author()`, `package_description()`, `package_source()`
- **Resolution:** `download_tag()`, `download_branch()`, `download_redirect()`
- **Dependencies:** `require_package(source, version_constraint)`
- **Build:** `cmake_build()`, `custom_build(command)`, `no_build()`

State is stored in module globals (`_pkg_name`, `_download_ref`, etc.) and read back by `package_runner.das` via `get_context_global_variable()` after execution.

### Example projects (`examples/daspkg/`)

| Directory | Description |
|-----------|-------------|
| `examples/daspkg/daspkg-example/` | Pure daslang install from git (uses `daspkg-test-pure` + `daspkg-test-deps`) |
| `examples/daspkg/daspkg-build-example/` | C/C++ native module install + cmake build workflow |
| `examples/daspkg/daspkg-version-1/` | Install specific version (v1.0) — only `get_version()` available |
| `examples/daspkg/daspkg-version-2/` | Install specific version (v2.0) — adds `new_in_v2()` API |
| `examples/daspkg/packages/` | Example package sources (below) |

### Example packages (`examples/daspkg/packages/`)

| Package | Type | `.das_package` features |
|---------|------|------------------------|
| `daspkg-example-c` | C module (daScriptC.h) | `package()` + `cmake_build()` |
| `daspkg-example-cpp` | C++ module (daScript.h) | `package()` + `cmake_build()` |
| `daspkg-example-mixed` | Mixed das + C dependency | `package()` + `dependencies()` |

### Test repositories (GitHub)

| Repository | Purpose | `.das_package` |
|------------|---------|---------------|
| [`borisbat/daspkg-test-pure`](https://github.com/borisbat/daspkg-test-pure) | Pure daslang module | `package()` only |
| [`borisbat/daspkg-test-versions`](https://github.com/borisbat/daspkg-test-versions) | Module with version tags (v1.0, v2.0) | `package()` + `resolve()` |
| [`borisbat/daspkg-test-deps`](https://github.com/borisbat/daspkg-test-deps) | Module with transitive dependency | `package()` + `dependencies()` |
| [`borisbat/daspkg-index`](https://github.com/borisbat/daspkg-index) | Central package index (`packages.json`) | N/A |

### Test fixtures (`utils/daspkg/fixtures/`)

| Fixture | Tests |
|---------|-------|
| `test.das_package` | All four functions: package, resolve (tag), dependencies, build (cmake) |
| `test_branch.das_package` | Branch-based resolution |
| `test_redirect.das_package` | Redirect to a different repository |
| `test_sdk_aware.das_package` | Version-dependent resolution (latest/1.x/explicit) |

### Tests

| File | Count | Type |
|------|-------|------|
| `test_daspkg.das` | 80 | Unit tests (local operations, parsing, package_runner) |
| `test_daspkg_git.das` | 34 | Integration tests (git clone, version resolve, index fetch, full workflows) |

Run tests:
```bash
daslang dastest/dastest.das -- --test utils/daspkg/test_daspkg.das
daslang dastest/dastest.das -- --test utils/daspkg/test_daspkg_git.das
```

## Use-case workflows

### 1. Pure daslang package (no build step)

A module that's just `.das` files — the most common case.

**Package structure:**
```
mymodule/
  .das_package
  .das_module
  daslib/
    mymodule.das
```

**`.das_package`:**
```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("mymodule")
    package_description("My pure daslang module")
}
```

**User installs:**
```bash
daspkg install github.com/user/mymodule
```

### 2. C/C++ package with cmake build

A module that wraps native code. daspkg auto-builds it after install.

**`.das_package`:**
```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("fastmath")
    package_description("Fast math via native C code")
}

[export]
def build() {
    cmake_build()
}
```

**What happens on install:**
1. Clone repo
2. `cmake -B _build -S . -DDASLANG_DIR=<das_root>`
3. `cmake --build _build --config Release`
4. The built shared library is available in `modules/fastmath/_build/`

### 3. Package with dependencies

A module that depends on other packages.

**`.das_package`:**
```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("game-utils")
    package_description("Game utility collection")
}

[export]
def dependencies(version : string) {
    require_package("github.com/user/math-lib")
    require_package("github.com/user/ecs-helpers", ">=2.0")
}
```

**What happens on install:**
1. Clone `game-utils`
2. Read `.das_package`, call `dependencies()`
3. For each dependency not already installed, recursively `install_git`
4. All packages end up in `modules/`

### 4. Version-pinned install

Install a specific version of a package.

```bash
daspkg install github.com/user/mymodule@v1.0
```

**If the package has `resolve()`**, it receives `version="v1.0"` and can transform it (e.g., map `"1.0"` to tag `"v1.0"`).

**If no `resolve()`**, the version string is used as a git tag directly.

### 5. SDK-aware version resolution

A package that ships different versions for different SDK releases.

**`.das_package`:**
```das
options gen2
require daslib/daspkg
require strings

[export]
def package() {
    package_name("compat-lib")
    package_description("SDK-aware compatibility library")
}

[export]
def resolve(sdk_version, version : string) {
    if (version == "" || version == "latest") {
        download_tag("v2.0")
    } elif (version == "1.x") {
        download_tag("v1.5")
    } else {
        download_tag("v{version}")
    }
}
```

### 6. Repository redirect (package moved)

A package that has moved to a new repository. The old repo's `.das_package` redirects.

**`.das_package` (in the old repo):**
```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("old-name")
    package_description("This package has moved")
}

[export]
def resolve(sdk_version, version : string) {
    download_redirect("github.com/neworg/new-repo", "v3.0")
}
```

**What happens:** daspkg clones the old repo, reads `.das_package`, sees the redirect, discards the clone, re-clones from the new repo, and checks out `v3.0`.

### 7. Branch tracking (development/nightly)

A package that tracks a branch instead of tagged releases.

**`.das_package`:**
```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("bleeding-edge")
    package_description("Always latest from main branch")
}

[export]
def resolve(sdk_version, version : string) {
    download_branch("main")
}
```

### 8. Install from package index by name

Packages registered in the [daspkg-index](https://github.com/borisbat/daspkg-index) can be installed by bare name:

```bash
daspkg search math          # find packages
daspkg install mymodule     # resolves name -> URL via index
```

### 9. Local package (development)

Install a package from a local directory (copies files, no git):

```bash
daspkg install ./path/to/mymodule
```

Useful during development — edit the source, then `daspkg update mymodule` to re-copy.

### 10. Project with .das_package at root

A project can list its own dependencies in a root `.das_package`:

**`.das_package` (project root):**
```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("my-game")
    package_description("My game project")
}

[export]
def dependencies(version : string) {
    require_package("github.com/user/ecs-lib")
    require_package("github.com/user/render-lib")
}
```

```bash
cd my-game/
daspkg install        # installs all dependencies
daspkg update         # updates all
daspkg check          # verifies everything is present
```

### 11. Publishing a package

Register your package in the central index:

```bash
# from your package directory (reads .das_package for name/description)
daspkg introduce

# or by URL
daspkg introduce github.com/user/mymodule
```

This clones the index repo, adds your package to `packages.json`, and creates a PR on GitHub (requires `gh` CLI).

To remove:
```bash
daspkg withdraw mymodule
```

## Lock file (`daspkg.lock`)

Tracks installed packages with their source, version, resolved tag/branch, and whether they're root or transitive dependencies:

```json
{
    "sdk_version": "",
    "packages": {
        "mymodule": {
            "source": "github.com/user/mymodule",
            "version": "v1.0",
            "tag": "v1.0",
            "branch": "",
            "root": true,
            "local": false
        }
    }
}
```

## Requirements

- **git** — required for all remote operations
- **cmake** — required for building C/C++ packages
- **gh** (GitHub CLI) — optional, only needed for `introduce`/`withdraw`

Run `daspkg doctor` to check your environment.
