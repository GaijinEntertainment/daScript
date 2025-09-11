options gen2
require daslib/linq
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
            .reverse()
            .where_($(t) => t._0 % 2 == 0)
            .to_array()
    )
    for (a in all) {
        print("index {a.index} text '{a.text}'\n")
    }
}
