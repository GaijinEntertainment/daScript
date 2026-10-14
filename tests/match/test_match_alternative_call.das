options gen2
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/match
require daslib/option

struct Point {
    x : int
    y : int
}

struct Nothing {}

variant Event {
    click : Point
    key : int
    idle : Nothing
}

variant Maybe {
    some : int
    none : Nothing
}

def private describe_event(e : Event) : string {
    return match (e) {
        click(Point(x = 0, y = y)) => "edge click {y}"
        click(p) => "click {p.x}"
        key(13) | key(10) => "enter"
        key(k) && k > 100 => "high {k}"
        key(_) => "key"
        idle(_) => "idle"
    }
}

def private maybe_kind(m : Maybe) : string {
    return match (m) {
        some(0) => "zero"
        some(x) => "some {x}"
        none(_) => "none"
    }
}

def private key(x : int) : int => x + 100

def private keyed(e : Event) : string {
    return match (e) {
        key(match_expr(key(1))) => "key 101"
        key(k) => "key {k}"
        _ => "other"
    }
}

[test]
def test_alternative_call_patterns(t : T?) {
    t |> run("an alternative matches its value against the call's argument") @(t : T?) {
        t |> equal(describe_event(Event(click = Point(x = 0, y = 4))), "edge click 4")
        t |> equal(describe_event(Event(click = Point(x = 2))), "click 2")
        t |> equal(describe_event(Event(key = 10)), "enter")
        t |> equal(describe_event(Event(key = 200)), "high 200")
        t |> equal(describe_event(Event(key = 1)), "key")
        t |> equal(describe_event(Event(idle = Nothing())), "idle")
    }
    t |> run("an alternative wins over a function of the same name") @(t : T?) {
        t |> equal(maybe_kind(Maybe(some = 0)), "zero")
        t |> equal(maybe_kind(Maybe(some = 5)), "some 5")
        t |> equal(maybe_kind(Maybe(none = Nothing())), "none")
    }
    t |> run("match_expr calls the function the alternative shares its name with") @(t : T?) {
        t |> equal(keyed(Event(key = 101)), "key 101")
        t |> equal(keyed(Event(key = 1)), "key 1")
        t |> equal(keyed(Event(idle = Nothing())), "other")
    }
}
