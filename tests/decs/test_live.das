options persistent_heap
options gc

require dastest/testing_boost public
require daslib/live
require fio

[test]
def test_create_object( var t: T? )
    let app_f = "test_live_app.das"
    let app_d = "{get_das_root()}/tests/decs"
    t |> run("go live") <| @ ( t : T? )
        go_live(app_f, app_d)
        t |> success(is_app_live())
    t |> run("initialize") <| @ ( t : T? )
        invoke_live("initialize")
    t |> run("survive 20 recompiles and reloads with restart") <| @ ( t : T? )
        for q in range(20)
            live_update(double(q))
            recompile()
            t |> success(is_app_live())
    t |> run("go live") <| @ ( t : T? )
        invoke_live("shutdown")
