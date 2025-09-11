options gen2
require daslib/linq_boost
require daslib/decs_boost

[export]
def main {
    for (i in 0..10) {
        create_entity <| @(eid, cmp) {
            cmp.eid := eid
            cmp.index := i
            cmp.text := "{i}_{i*10}"
        }
    }
    commit()
    var all = (
        from_decs($(index : int; text : string){})
            ._where(_._0 % 2 == 0)
            .reverse()
            .to_array()
    )

    for (a in all) {
        print("index {a.index} text '{a.text}'\n")
    }
}



