options gen2
options indenting = 4
require dastest/testing_boost public
require daslib/fio
require release_workspace

[test]
def release_workspaces_are_isolated_and_removed(t : T?) {
    var outer_path : string
    var inner_path : string
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
