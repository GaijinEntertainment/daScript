options persistent_heap
options gc

require daslib/live
require fio

[export]
def main
    let app_f = "test_live_app.das"
    let app_d = "{get_das_root()}/tests/decs"
    go_live(app_f, app_d)
    invoke_live("initialize")
    var t = 0lf
    while true
        live_update(t)
        sleep(200u)
        t += 0.2lf
        if watch_files() || t >= 0.4lf
            recompile()
            t = 0.0lf
    invoke_live("shutdown")

options debugger
require daslib/debug
