# daspkg — daslang Package Manager

## Goals

Package manager for the daslang ecosystem. Automates discovery, installation, dependency resolution, and building of daslang packages (both pure-das and C++ modules).

## Package Types

### Pure-das packages
- Just `.das` files + `.das_module` (provided by author), no build step
- Package manager downloads and places in `modules/`
- Works for everyone — no toolchain required

### C++ packages
- Contains C++ source + `CMakeLists.txt` using `find_package(DAS)`
- Package manager runs `cmake --configure` + `cmake --build` automatically
- Requires C++ toolchain (MSVC on Windows, GCC/Clang on Linux/Mac)
- Fails gracefully if toolchain is missing
- No prebuilt binary hosting — always build from source

## Version Model

Three version axes:
- **Package version** — semver of the package itself
- **daslang version** — which SDK release the package is compatible with
- **Dependencies** — other packages (with their own version constraints)

The manifest can use `sdk_version` in `resolve()` to return different tags for different SDK versions.

C++ packages are sensitive to the exact daslang version (ABI compatibility). Pure-das packages are more tolerant (only language-level compatibility).

## Key Design Decisions

### Registry: Git-based (like Go modules) + curated index
- Packages are git repositories (GitHub, GitLab, etc.)
- No centralized registry server to maintain
- Install by URL: `daspkg install github.com/user/repo`
- Version = git tag (e.g., `v1.0.0`)

### Package index
- JSON array in a dedicated index repo ([borisbat/daspkg-index](https://github.com/borisbat/daspkg-index))
- Each entry: `{ "name": "...", "url": "...", "description": "..." }`
- Authors add packages via `daspkg introduce`, which creates a PR
- `daspkg search <query>` clones/fetches the index repo, matches against it. Cached in `modules/.daspkg_cache/index/`
- Install by name (from index): `daspkg install imgui`
- Install by URL (direct): `daspkg install github.com/user/dasImgui`

### Manifest: `.das_package` (executable)

The manifest is a daslang script — same pattern as `.das_module`. daspkg compiles it in-process (`package_runner.das`), calls exported functions, and reads accumulated state from `daslib/daspkg` module globals.

Each function answers a specific question. All are optional — a repo without `.das_package` gets a "dumb clone" (no version resolution, no deps, no build).

```das
options gen2
require daslib/daspkg

// Package metadata
[export]
def package() {
    package_name("mymodule")
    package_author("aleksisch")
    package_description("Cool module for daslang")
    package_source("https://github.com/aleksisch/mymodule")
}

// What to download for a given version + SDK
[export]
def resolve(sdk_version : string; version : string) {
    if (version == "" || version == "latest")
        download_tag("v2.1.0")
    elif (version == "1.x")
        download_tag("v1.5.3")
    else
        download_tag("v{version}")
    // alternatives: download_branch("main"), download_redirect("github.com/other/repo", "v1.0")
}

// What other packages are needed
[export]
def dependencies(version : string) {
    require_package("github.com/user/some_lib", ">=1.0")
    require_package("github.com/user/other_lib")
}

// How to build (if missing, assumes pure-das — no build step)
[export]
def build(sdk_path : string; version : string) {
    cmake_build()  // default: cmake configure + build with -DDASLANG_DIR=<sdk>
    // alternatives: no_build(), custom_build("make all")
}
```

**Why executable, not JSON?**
- Authors can put version-conditional logic in `resolve` and `dependencies`
  (e.g., "for SDK >= 0.6 use this tag, for older use that one")
- Same pattern as `.das_module` — familiar to daslang authors
- daspkg provides the registration functions (`package_name`, `download_tag`, etc.) via `daslib/daspkg`

Module registration (namespaces, DLLs, require paths) is handled entirely by `.das_module` — not duplicated in `.das_package`.

### Local layout

daspkg installs into `modules/` relative to the project root. The daslang runtime already scans `modules/` for `.das_module` files — first in the SDK, then in the app's directory. No new discovery mechanism needed.

```
my_project/
  main.das
  .das_package                   # project manifest (optional — lists dependencies)
  modules/                       # standard daslang modules directory
    <package_name>/
      .das_module                # provided by package author
      .das_package               # package manifest
      _build/                    # CMake build directory (if C++ package)
      *.shared_module            # built C++ output (if applicable)
      *.das                      # source files
    .daspkg_cache/               # index cache (gitignored)
    .daspkg_tmp/                 # temp dir during install (gitignored)
  daspkg.lock                    # JSON — installed packages, versions, sources
```

Packages are **per-project** (like `node_modules`), which is the right default for game projects that need reproducible builds. The SDK's own `modules/` has the built-in modules; the project's `modules/` has third-party packages.

### Package naming
- Package name = what `package_name()` declares in `.das_package`. Must be unique.
- Directory in `modules/` uses the package name (not the git repo name)
- The curated index enforces uniqueness — PR review catches collisions
- **Module namespace conflicts** (two packages registering the same `require` path) are a runtime issue, not a daspkg issue. Authors must pick unique namespaces.

### Repos without `.das_package`
If a repo has no `.das_package`, daspkg does a "dumb clone" — clones the repo into `modules/<name>/` at the requested tag (or HEAD). No version resolution, no dependency tracking, no build. The repo must already have a `.das_module` and be self-contained. Recorded in the lock file with minimal info.

### `.das_module` handling
- `.das_module` is always provided by the package author — not generated by daspkg
- The author knows their DLL names, `require` namespaces, load dependencies
- daspkg just places the package in `modules/<name>/` and the existing `.das_module` works as-is

### Package sources

Packages can come from three sources:

1. **Git URL**: `daspkg install github.com/user/repo[@version]` — clones the repo
2. **Index name**: `daspkg install imgui` — looks up URL in the curated index
3. **Local path**: `daspkg install /path/to/my/module` — copies from a local directory

Local paths are essential for development — you're working on a package and want to test it without pushing to git. Currently always copies; symlink support planned.

## Commands

```
daspkg install <url|name|path>[@version]  Install from git, index, or local path (--force to reinstall)
daspkg install                            Install all deps from project .das_package
daspkg remove <name>                      Uninstall a package
daspkg update [name]                      Re-install at pinned version (re-clone, rebuild)
daspkg upgrade [name]                     Upgrade to latest version (re-resolve, compare, re-install)
daspkg list                               List installed packages
daspkg search <query>                     Search the package index
daspkg introduce [url]                    Add a package to the index via PR (requires gh)
daspkg withdraw <name>                    Remove a package from the index via PR (requires gh)
daspkg build                              Build all C++ packages in modules/
daspkg check                              Verify all packages are present and have .das_module
daspkg doctor                             Check environment (git, cmake, gh)
```

**Options:**
- `--root <path>` — project root (default: current directory)
- `--force` — force reinstall even if already installed

## Transport

Only external dependency: **`git` CLI**. No HTTP library, no curl, no dasHV.

### Git clone flow (current)

Full shallow clone, then checkout tag/branch if needed.

**Install:**
1. `git clone --depth 1 <url> modules/.daspkg_tmp/<name>`
2. Run `.das_package` → `resolve(sdk_version, version)` → get tag/branch, `dependencies()` → get deps
3. If tag/branch specified: `git fetch --depth 1 origin tag <tag>` then `git checkout <tag>`
4. If redirect: re-clone from the redirect URL
5. Move from `.daspkg_tmp/<name>` to `modules/<name>/`
6. Record in lock file
7. Install dependencies (recursive)
8. Build if C++ (auto-detected from `.das_package` or `CMakeLists.txt`)

**Update:** re-install at the pinned version from the lock file. Deletes existing, re-clones.

### Local paths
- Filesystem copy, no git involved
- `.das_package` read directly from the local directory

## Architecture

### Modules

- `main.das` — CLI entry point, argument parsing, command dispatch
- `commands.das` — all command implementations (install, remove, update, build, check, doctor)
- `lockfile.das` — `LockFile` / `PackageEntry` structs, JSON serialization
- `index.das` — package index fetch/search, introduce/withdraw PR workflow
- `utils.das` — `run_cmd`, `copy_dir_rec`, path utilities
- `package_runner.das` — in-process `.das_package` compiler and runner
- `daslib/daspkg.das` — API module that `.das_package` scripts `require`

### Package runner

`package_runner.das` compiles `.das_package` scripts in-process using `compile_file` + `simulate` + `invoke_in_context`. It calls specific exported functions (`package`, `resolve`, `dependencies`, `build`) and reads accumulated state from `daslib/daspkg` module globals via `get_context_global_variable`.

This avoids spawning a subprocess — the `.das_package` runs in the same daslang process, just in a separate context.

## Version Resolution

### v1: Simple, no constraint solver

**Install with version**: `daspkg install foo@1.2.0` → runs `resolve(sdk_version, "1.2.0")` → gets a git tag → clones it.

**Install without version**: `daspkg install foo` → runs `resolve(sdk_version, "")` → `.das_package` decides what "latest" means.

**Update**: `daspkg update foo` → re-clones at the same pinned version from the lock file. Re-runs `.das_package` resolve, rebuilds. Useful for picking up `.das_package` changes or rebuilding after SDK upgrade.

### Lock file (`daspkg.lock`)

Ordered JSON array — preserves installation order for sequential upgrades.

```json
{
  "sdk_version": "",
  "packages": [
    {
      "name": "dasVulkan",
      "source": "github.com/user/dasVulkan",
      "version": "1.2.0",
      "tag": "v1.2.0",
      "branch": "",
      "root": true,
      "local": false
    },
    {
      "name": "dasUtils",
      "source": "github.com/user/dasUtils",
      "version": "",
      "tag": "v0.3.1",
      "branch": "",
      "root": false,
      "local": false
    },
    {
      "name": "myLocalMod",
      "source": "/home/me/dev/myLocalMod",
      "version": "",
      "tag": "",
      "branch": "",
      "root": true,
      "local": true
    }
  ]
}
```

- **`root: true`** — user explicitly installed this package
- **`root: false`** — pulled in as a transitive dependency
- **`version`** — what the user requested (e.g., `1.2.0` from `install foo@1.2.0`)
- **`tag` / `branch`** — what `.das_package` resolved to (the actual git ref)

### Diamond dependencies
Only one version of a package can be installed — daslang has a single `require` namespace, no way to load two versions simultaneously (unlike npm's nested `node_modules`).

**v1 rule:** first installed wins. If C@1.0 is already installed and package B wants C>=2.0, daspkg skips it (already installed). User can force-reinstall a specific version.

Similar to Go modules — one version per module, manual upgrade. Works fine for small ecosystems.

## C++ Build

Build directory: `modules/<name>/_build/` — inside the package directory. Can be safely deleted and rebuilt with `daspkg build`.

```
cmake -B modules/<name>/_build -S modules/<name> -DDASLANG_DIR=<das_root>
cmake --build modules/<name>/_build --config Release
```

The package's `CMakeLists.txt` is responsible for placing output (`.shared_module`) into its own source directory. daspkg just runs cmake.

`_build/` and `.daspkg_cache/` should be in `.gitignore`.

## Upgrade

**`upgrade [name]`** — bump to latest available version.

- Re-resolves with empty version string (`.das_package` decides "latest")
- Compares resolved tag/branch to installed — skips if already at latest
- Walks dependency graph downward: checks constraints, upgrades deps that don't satisfy their parent's `require_package` constraint
- `upgrade` (no args) — upgrades all root, non-local packages

**Version constraints** in `require_package(url, constraint)`:
- Operators: `>=`, `>`, `<=`, `<`, `=` (or bare version for exact match)
- Comma-separated AND: `">=1.0,<2.0"`
- Semver parsing: strips `v` prefix, handles `major`, `major.minor`, `major.minor.patch`
- Implementation: `satisfies_constraint()` in `utils.das`

## Not Yet Implemented

- **Sparse checkout** — fetch only `.das_package` before downloading the full repo. Would reduce bandwidth for version resolution.
- **Rollback on failure** — backup old `modules/<name>/` before update, restore on failure. Currently update deletes then re-installs with no rollback.
- **Symlinks for local paths** — `--link` flag for local installs. Currently always copies.
- **Orphan cleanup** — detect unused transitive deps after `remove`.
- **SDK version tracking** — record SDK version in lock file, warn on mismatch, prompt `daspkg build` after SDK upgrade.
- **`.das_project` sandbox** — restrict what `.das_package` scripts can `require` (no `fio`, no `unsafe`). Currently `.das_package` has full access.
- **Full constraint solver** — for when the ecosystem is large enough to need it (currently first-installed wins for diamonds).

## Open Questions

- [x] ~~Should packages live inside the SDK or per-project?~~ Per-project `modules/` — runtime already scans it
- [x] ~~Package tests~~ — not a daspkg concern. Authors put tests in `modules/<name>/tests/`. dastest can learn to glob `modules/*/tests/*.das`.
- [x] ~~Index/discovery~~ — single curated file in index repo, authors add via `daspkg introduce`
- [x] ~~Should `upgrade` be a separate command?~~ Yes — `update` re-installs at pinned version, `upgrade` bumps to latest
- [x] ~~Version constraint syntax~~ — semver ranges with operators (`>=1.0,<2.0`), implemented in `utils.das`
- [ ] Should local installs default to symlink or copy?
