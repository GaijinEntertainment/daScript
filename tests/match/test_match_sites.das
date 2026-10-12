options gen2

require dastest/testing_boost public
require daslib/match

struct Point {
    x : int
    y : int
}

def private point_for(x : int) : Point? {
    var log = ""
    match (x) {
        0 => $ { log = "zero" }
        _ => $ { log = "other" }
    }
    var p : Point? = match (x) {
        0 => null
        _ => new Point(x = x, y = length(log))
    }
    return match (x) {
        0 => p
        n => match (n) {
            1 => new Point(x = 1, y = 1)
            _ => p
        }
    }
}

[test]
def test_every_match_of_a_function_keeps_its_place(t : T?) {
    t |> run("a typed let and a typed return type their null arms") @(t : T?) {
        t |> success(point_for(0) == null)
    }
    t |> run("a match nested in an arm keeps its own place") @(t : T?) {
        t |> equal(point_for(1).y, 1)
    }
    t |> run("the statement match ran before the value matches") @(t : T?) {
        let p = point_for(7)
        t |> success(p != null)
        t |> equal(p.x, 7)
        t |> equal(p.y, 5)
    }
}
