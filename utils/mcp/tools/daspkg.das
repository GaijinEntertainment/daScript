options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require common public

// Run a daspkg command, return (output, exit_code).
// root defaults to get_das_root() (the daScript repo root).
def private run_daspkg_raw(command : string; var output : string&; root : string = ""; timeout_sec : float = 120.0) : int {
    let exe = get_daslang_exe()
    if (empty(exe)) {
        output = "Cannot determine daslang executable path"
        return -1
    }
    let daspkg_script = "{get_das_root()}/utils/daspkg/main.das"
    let pkg_root = empty(root) ? get_das_root() : resolve_path(root)
    return run_and_capture("{exe} {daspkg_script} -- {command} --root {pkg_root} --no-color", output, timeout_sec)
}

def private run_daspkg(command : string; root : string = ""; timeout_sec : float = 120.0) : string {
    var output : string
    let exit_code = run_daspkg_raw(command, output, root, timeout_sec)
    if (exit_code == popen_timed_out) {
        return make_tool_result("daspkg timed out after {int(timeout_sec)}s:\n{output}", true)
    }
    if (exit_code != 0) {
        return make_tool_result("daspkg failed (exit code {exit_code}):\n{output}", true)
    }
    return make_tool_result(output)
}

def do_daspkg_search(query, root : string) : string {
    if (empty(query)) {
        return run_daspkg("search --json", root)
    }
    return run_daspkg("search {query} --json", root)
}

def do_daspkg_info(name, root : string) : string {
    if (empty(name)) {
        return make_tool_result("missing 'name' argument", true)
    }
    // search returns JSON array filtered by name — exact match scores highest
    return run_daspkg("search {name} --json", root)
}

def do_daspkg_list(root : string) : string {
    return run_daspkg("list --json", root)
}

def do_daspkg_check(root : string) : string {
    return run_daspkg("check --json", root)
}

def do_daspkg_install(spec, root : string; force : bool) : string {
    if (empty(spec)) {
        return run_daspkg("install", root, 600.0)
    }
    let force_flag = force ? " --force" : ""
    return run_daspkg("install {spec}{force_flag}", root, 600.0)
}

def do_daspkg_remove(name, root : string) : string {
    return run_daspkg("remove {name}", root)
}

def do_daspkg_update(name, root : string) : string {
    if (empty(name)) {
        return run_daspkg("update", root, 600.0)
    }
    return run_daspkg("update {name}", root, 600.0)
}

def do_daspkg_upgrade(name, root : string) : string {
    if (empty(name)) {
        return run_daspkg("upgrade", root, 600.0)
    }
    return run_daspkg("upgrade {name}", root, 600.0)
}

def do_daspkg_build(root : string) : string {
    return run_daspkg("build", root, 600.0)
}
