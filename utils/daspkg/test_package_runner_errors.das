options gen2
require dastest/testing_boost
require daslib/fio
require package_runner
[test]
def failing_package_hooks_do_not_publish_partial_metadata(t : T?){
 var error : string; let root = create_temp_directory("daspkg-hook-error-", error); t |> equal(error, "")
 let manifest = path_join(root, ".das_package")
 fwrite(manifest, "options gen2\nrequire daslib/daspkg\n[export]\ndef package()\{package_name(\"hook_fixture\")}\n[export]\ndef release()\{release_main(\"main.das\");panic(\"release hook sentinel\")}\n[export]\ndef build()\{cmake_build();panic(\"build hook sentinel\")}\n")
 var meta : PackageMeta
 t |> success(run_das_package_meta(manifest, meta), "fixture compiles and runs its ordinary metadata hook")
 t |> equal(meta.pkg_name, "hook_fixture")
 var release : PackageReleaseInfo
 t |> success(!run_das_package_release(manifest, release), "throwing release must fail rather than return partial metadata")
 t |> equal(release.main_script, "", "partial main script must not escape")
 var build : PackageBuildInfo
 t |> success(!run_das_package_build(manifest, build), "throwing build must fail")
 t |> success(!build.is_cmake, "partial build declaration must not escape")
 remove(manifest); rmdir(root)
}
