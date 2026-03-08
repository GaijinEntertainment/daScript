options gen2
options rtti
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false
options strict_smart_pointers = true

module package_runner shared public

require debugapi
require fio
require daslib/daspkg

// Results from running a .das_package script

struct PackageMeta {
    pkg_name : string
    author : string
    description : string
    source : string
}

struct ResolveResult {
    tag : string
    branch : string
}

struct PackageDependency {
    source : string
    version_constraint : string
}

struct PackageBuildInfo {
    command : string
    is_cmake : bool
    is_custom : bool
}


// Run the `package()` function of a .das_package and extract metadata
def run_das_package_meta(das_package_path : string; var meta : PackageMeta) : bool {
    var args : array<string>
    var ok = run_das_package(das_package_path, "package", args) <| $(ctx) {
        var p = unsafe(get_context_global_variable(ctx, "_pkg_name"))
        if (p != null) {
            unsafe {
                meta.pkg_name = clone_string(*reinterpret<string?>(p))
            }
        }
        p = unsafe(get_context_global_variable(ctx, "_pkg_author"))
        if (p != null) {
            unsafe {
                meta.author = clone_string(*reinterpret<string?>(p))
            }
        }
        p = unsafe(get_context_global_variable(ctx, "_pkg_description"))
        if (p != null) {
            unsafe {
                meta.description = clone_string(*reinterpret<string?>(p))
            }
        }
        p = unsafe(get_context_global_variable(ctx, "_pkg_source"))
        if (p != null) {
            unsafe {
                meta.source = clone_string(*reinterpret<string?>(p))
            }
        }
    }
    return ok
}

// Run the `resolve()` function of a .das_package and extract download info
def run_das_package_resolve(das_package_path : string; sdk_version, version : string; var result : ResolveResult) : bool {
    var args <- [sdk_version, version]
    var ok = run_das_package(das_package_path, "resolve", args) <| $(ctx) {
        var p = unsafe(get_context_global_variable(ctx, "_download_ref"))
        if (p != null) {
            unsafe {
                result.tag = clone_string(*reinterpret<string?>(p))
            }
        }
        p = unsafe(get_context_global_variable(ctx, "_download_kind"))
        if (p != null) {
            unsafe {
                let kind = *reinterpret<DownloadKind?>(p)
                if (kind == DownloadKind.branch) {
                    result.branch = result.tag
                    result.tag = ""
                }
            }
        }
    }
    return ok
}

// Run the `dependencies()` function of a .das_package and extract deps
def run_das_package_deps(das_package_path : string; version : string; var deps : array<PackageDependency>) : bool {
    var args <- [version]
    var ok = run_das_package(das_package_path, "dependencies", args) <| $(ctx) {
        var p = unsafe(get_context_global_variable(ctx, "_dependencies"))
        if (p != null) {
            unsafe {
                let arr = reinterpret<array<Dependency>?>(p)
                for (d in *arr) {
                    deps |> emplace(PackageDependency(source = clone_string(d.source), version_constraint = clone_string(d.version_constraint)))
                }
            }
        }
    }
    return ok
}

// Run the `build()` function of a .das_package and extract build info
def run_das_package_build(das_package_path : string; var info : PackageBuildInfo) : bool {
    var args : array<string>
    var ok = run_das_package(das_package_path, "build", args) <| $(ctx) {
        var p = unsafe(get_context_global_variable(ctx, "_build_kind"))
        if (p != null) {
            unsafe {
                let kind = *reinterpret<BuildKind?>(p)
                info.is_cmake = kind == BuildKind.cmake
                info.is_custom = kind == BuildKind.custom
            }
        }
        p = unsafe(get_context_global_variable(ctx, "_build_command"))
        if (p != null) {
            unsafe {
                info.command = clone_string(*reinterpret<string?>(p))
            }
        }
    }
    return ok
}

// Internal: compile, simulate, call function, read state
def private run_das_package(das_package_path : string; func_name : string; args : array<string>; blk : block<(ctx : smart_ptr<Context>) : void>) : bool {
    if (!fexist(das_package_path)) {
        print("Error: {das_package_path} not found\n")
        return false
    }
    var ok = false
    var inscope access <- make_file_access("")
    using() <| $(var mg : ModuleGroup) {
        using() <| $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.export_all = true
            compile_file(das_package_path, access, unsafe(addr(mg)), cop) <| $(compiled; program; issues) {
                if (!compiled) {
                    print("Error compiling {das_package_path}:\n{issues}\n")
                    return
                }
                simulate(program) <| $(simulated; ctx; serrors) {
                    if (!simulated) {
                        print("Error simulating {das_package_path}:\n{serrors}\n")
                        return
                    }
                    try {
                        unsafe {
                            let n = length(args)
                            if (n == 0) {
                                invoke_in_context(ctx, func_name)
                            } elif (n == 1) {
                                invoke_in_context(ctx, func_name, args[0])
                            } elif (n == 2) {
                                invoke_in_context(ctx, func_name, args[0], args[1])
                            }
                        }
                    } recover {
                        pass
                    }
                    invoke(blk, ctx)
                    ok = true
                }
            }
        }
    }
    return ok
}
