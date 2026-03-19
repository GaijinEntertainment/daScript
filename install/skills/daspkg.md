# Package Manager (daspkg)

daspkg manages daslang packages — install from GitHub or local paths, resolve dependencies, and build native modules.

## CLI Usage

```bash
bin/daslang utils/daspkg/main.das -- <command> [args]
```

### Commands

| Command | Description |
|---|---|
| `install <source>` | Install a package (GitHub URL, local path, or index name) |
| `update [name]` | Update all or a specific package |
| `remove <name>` | Remove an installed package |
| `list` | List installed packages |
| `search <query>` | Search the package index |
| `check` | Verify installed packages match lockfile |
| `doctor` | Check environment (git, cmake, gh) |
| `build <name>` | Build a native (CMake) package |

### Flags

- `--root <dir>` — project root directory (default: `.`)
- `--force` — force reinstall
- `--color` / `--no-color` — control ANSI color output

### Install sources

- **GitHub:** `daspkg install github.com/user/repo` (no `https://` prefix)
- **Local path:** `daspkg install ./path/to/package`
- **Index name:** `daspkg install my-package` (looks up in package index)

Packages install to `modules/<RepoName>/`. Lock file: `daspkg.lock` in the root directory.

## Package Structure

```
my-package/                     # package root (git repo)
  .das_package                  # daspkg metadata manifest
  .das_module                   # module registration for require
  namespace/                    # module namespace directory
    module.das                  # main module file (require namespace/module)
  test_module.das               # tests at package root
```

- **`require namespace/module`** resolves because the package root is the working directory
- **`.das_module`** registers the path for use when installed via `daspkg install`
- **No `.das_project` needed** — the root-level layout is self-contained

## `.das_package` Manifest

Every package has a `.das_package` file — a daslang script that declares metadata using `require daslib/daspkg`.

### `PackageMeta` struct (in `daslib/daspkg.das`)

```das
struct PackageMeta {
    pkg_name : string
    author : string
    description : string
    source : string       // canonical source URL (e.g. "github.com/user/repo")
    license : string      // SPDX identifier (e.g. "BSD-3-Clause", "MIT")
    tags : array<string>  // searchable tags
    min_sdk : string      // minimum daslang SDK version (e.g. "0.4")
}
```

### Metadata (`package()` function)

```das
options gen2
require daslib/daspkg

[export]
def package() {
    package_name("my-package")              // required
    package_author("username")              // recommended
    package_description("What it does")     // required
    package_source("github.com/user/repo")  // optional: canonical URL
    package_license("MIT")                  // optional: SPDX license
    package_tag("networking")               // optional: searchable tags
    package_tag("http")                     // call multiple times for multiple tags
    package_min_sdk("0.4")                  // optional: minimum SDK version
}
```

### Version resolution (`resolve()` function)

```das
[export]
def resolve(sdk_version, version : string) {
    if (empty(version) || version == "latest") {
        download_branch("main")       // track a branch
    } else {
        download_tag("v{version}")    // download a specific git tag
    }
}
```

Resolution functions: `download_tag(ref)`, `download_branch(ref)`, `download_redirect(source, ref)`.

If `resolve()` is not defined, daspkg clones the default branch.

### Dependencies (`dependencies()` function)

```das
[export]
def dependencies(version : string) {
    require_package("github.com/user/dep1")
    require_package("github.com/user/dep2", ">=1.0")  // with version constraint
}
```

Version constraints: `>=1.0`, `>1.0`, `<=2.0`, `<2.0`, `=1.5`, `>=1.0,<2.0` (range).

### Build info (`build()` function)

```das
[export]
def build() {
    cmake_build()                    // CMake-based build
    // or: custom_build("make all") // custom build command
    // or: no_build()               // pure-das, no build needed (default)
}
```

## `.das_module` for packages

Registers the module so projects can `require` it after `daspkg install`:

```das
options gen2
require daslib/fio

[export]
def initialize(project_path : string) {
    register_native_path("namespace", "module", "{project_path}/namespace/module.das")
}
```

For multiple files:

```das
[export]
def initialize(project_path : string) {
    let paths = ["module", "helper", "utils"]
    for (path in paths) {
        register_native_path("namespace", "{path}", "{project_path}/namespace/{path}.das")
    }
}
```

## Running package tests

From the package root:

```bash
bin/daslang dastest/dastest.das -- --test test_module.das
```
