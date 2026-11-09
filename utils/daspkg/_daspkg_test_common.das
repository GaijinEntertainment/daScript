options gen2
options no_global_variables = false
module _daspkg_test_common public

require dastest/testing_boost public

require daslib/fio
require daslib/clargs
require daslib/defer
require strings
require daslib/strings_boost

require daslib/json_boost


require commands
require daslib/result
require package_runner public

// Writes a .das_package with no build() — install always succeeds.
def write_passing_pkg(src_dir, pkg_name : string) {
    let no_deps : array<string>
    write_passing_pkg(src_dir, pkg_name, no_deps)
}

// Writes a .das_package with optional dependencies (each entry is a require_package source).
def write_passing_pkg(src_dir, pkg_name : string; deps : array<string>) {
    let no_lines : array<string>
    write_passing_pkg(src_dir, pkg_name, no_lines, deps)
}

// Writes a .das_package whose package() holds package_name plus `package_lines`, with optional
// dependencies (each entry is a require_package source).
def write_passing_pkg(src_dir, pkg_name : string; package_lines, deps : array<string>) {
    mkdir(src_dir)
    let manifest = build_string() $(var w) {
        w |> write("options gen2\nrequire daslib/daspkg\n[export]\ndef package() \{\n    package_name(\"")
        w |> write(pkg_name)
        w |> write("\")\n")
        for (line in package_lines) {
            w |> write("    {line}\n")
        }
        w |> write("\}\n")
        if (!empty(deps)) {
            w |> write("[export]\ndef dependencies(version : string) \{\n")
            for (d in deps) {
                w |> write("    require_package(\"")
                w |> write(d)
                w |> write("\")\n")
            }
            w |> write("\}\n")
        }
    }
    fwrite("{src_dir}/.das_package", manifest)
}

// A known platform name that is not this host's.
def other_platform() : string {
    for (p in PLATFORM_NAMES) {
        if (p != get_platform_name()) return p
    }
    return ""
}

// A fresh directory from create_temp_directory, owned by the calling run.
def scratch_dir(t : T?; prefix : string) : string {
    var error : string
    let dir_path = create_temp_directory(prefix, error)
    t |> equal("", error)
    return dir_path
}

// Writes a .das_package with no build() that declares `platform` as its only platform.
def write_platform_pkg(src_dir, pkg_name, platform : string) {
    let no_deps : array<string>
    write_passing_pkg(src_dir, pkg_name, ["package_platform(\"{platform}\")"], no_deps)
}
