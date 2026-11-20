options gen2

require dastest/testing_boost public
require daslib/fio

//! a fixture for uncategorized.das: under dastest's --force-tune the
//! forced file is the manifest the run was started with

[test]
def test_forced_file_is_the_manifest(t : T?) {
    let manifest = get_env_variable("DAS_TUNE_MANIFEST")
    t |> success(!empty(manifest), "the run names a manifest")
    t |> equal(get_env_variable("DAS_TUNE_FORCE"), manifest,
        "the forced file is the manifest")
}
