options gen2

require dastest/testing_boost public
require daslib/fio
require package_runner
require release_workspace

[test(description = "a throwing package hook fails the run instead of leaking partial release or build metadata")]
def test_package_hook_failures(t : T?) {
    var error : string
    let root = create_temp_directory("daspkg-hook-error-", error)
    t |> equal(error, "")
    let manifest = path_join(root, ".das_package")
    fwrite(manifest, "options gen2\nrequire daslib/daspkg\n[export]\n" +
        "def package() \{ package_name(\"hook_fixture\") \}\n[export]\n" +
        "def release() \{ release_main(\"main.das\"); panic(\"release hook sentinel\") \}\n" +
        "[export]\ndef build() \{ cmake_build(); panic(\"build hook sentinel\") \}\n")
    var meta : PackageMeta
    t |> success(run_das_package_meta(manifest, meta), "fixture compiles and runs its ordinary metadata hook")
    t |> equal(meta.pkg_name, "hook_fixture")
    var release : PackageReleaseInfo
    t |> success(!run_das_package_release(manifest, release), "throwing release must fail rather than return partial metadata")
    t |> equal(release.main_script, "", "partial main script must not escape")
    var build : PackageBuildInfo
    t |> success(!run_das_package_build(manifest, build), "throwing build must fail")
    t |> success(!build.is_cmake, "partial build declaration must not escape")
    remove(manifest)
    rmdir(root)
}

[test(description = "overlapping releases get separate dependency workspaces, and each is removed when its release ends")]
def release_workspaces_are_isolated_and_removed(t : T?) {
    var outer_path, inner_path : string
    let rc = with_release_workspace() $(outer) : int {
        outer_path = outer
        fwrite(path_join(outer, "deps.json"), "outer")
        let inner_rc = with_release_workspace() $(inner) : int {
            inner_path = inner
            t |> success(inner != outer, "overlapping releases own different scratch directories")
            fwrite(path_join(inner, "deps.json"), "inner")
            t |> equal(fread(path_join(outer, "deps.json")), "outer", "another release cannot overwrite dependencies")
            return 7
        }
        t |> equal(inner_rc, 7, "a build failure retains its result")
        t |> success(!stat(inner_path).is_valid, "failure removes its dependency workspace")
        t |> success(stat(outer_path).is_valid, "another release's workspace survives")
        return 0
    }
    t |> equal(rc, 0)
    t |> success(!stat(outer_path).is_valid, "success removes its dependency workspace")
}
