options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq
require daslib/decs_boost

[test]
def test_from_decs(t : T?) {
    for (i in range(3)) {
        create_entity <| @(eid, cmp) {
            cmp.eid := eid
            cmp.index := i
            cmp.text := "{i}"
        }
    }
    commit()
    t |> run("from_decs simple") <| @(t : T?) {
        var qq = (
            from_decs($(index : int; text : string){})
                .where_($(v) => v.index < 2)
                .reverse()
                .to_array()
        )
        t |> equal(qq.length(), 2)
        var expected = [
            (1, "1"),
            (0, "0")
        ]
        for (c, e in qq, expected) {
            t |> equal(c.index, e._0)
            t |> equal(c.text, e._1)
        }
    }
}
