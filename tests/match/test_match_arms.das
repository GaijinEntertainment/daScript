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

[test(description = "An alternative written as a call matches its value against the argument, wins over a same-named function, and match_expr calls the function")]
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

[sideeffects]
def legacy_arms(n : int) {
    match (n) {   // nolint:STYLE044 - covers the deprecated arm form while it still compiles
        if (0) {
            return "zero"
        }
        if ($v(k) && k > 0) {
            return "positive {k}"
        }
        if (_) {
            return "negative"
        }
    }
    return "unreachable"
}

[test(description = "Legacy if-style match arms still compile and select the right arm")]
def test_legacy_if_arms_still_compile(t : T?) {
    t |> equal(legacy_arms(0), "zero")
    t |> equal(legacy_arms(3), "positive 3")
    t |> equal(legacy_arms(-3), "negative")
}

def private bucket(n : int) : string {
    return match (n) {
        (0..10) => "low"
        10 => "ten"
        (11..20) | (30..40) => "teen or thirties"
        (20..30) => "twenties"
        _ => "other"
    }
}

[test(description = "A range and constants that touch without overlapping pick the right arm at every boundary")]
def test_ranges_and_constants_that_touch_but_do_not_overlap(t : T?) {
    t |> equal(bucket(0), "low")
    t |> equal(bucket(9), "low")
    t |> equal(bucket(10), "ten")
    t |> equal(bucket(11), "teen or thirties")
    t |> equal(bucket(35), "teen or thirties")
    t |> equal(bucket(25), "twenties")
    t |> equal(bucket(40), "other")
    t |> equal(bucket(-1), "other")
}

variant Reading {
    value : float
    error : string
}

def private reading_or_zero(r : Reading) : float {
    let f = match (r) {
        value(v) => v
        error(e) => $ {
            feint(e)
            return 0.0
        }
    }
    return f + 1.0
}

def private sign_name(n : int) : string {
    return match (n) {
        0 => "zero"
        _ => $ {
            if (n > 0) {
                return "positive"
            }
            return "negative"
        }
    }
}

def private bucket_sum(xs : array<int>) : int {
    var total = 0
    for (x in xs) {
        let w = match (x) {
            (0..10) => 1
            _ => $ {
                let scaled = x / 10
                return scaled * scaled
            }
        }
        total += w
    }
    return total
}

[test(description = "A return in an arm's block body is the arm's value and leaves only that arm")]
def test_match_value_block_body(t : T?) {
    t |> run("a return in a block body is the arm's value") @(t : T?) {
        t |> equal(reading_or_zero(Reading(value = 2.0)), 3.0)
        t |> equal(reading_or_zero(Reading(error = "no sensor")), 1.0)
    }
    t |> run("a block body returns on every path it takes") @(t : T?) {
        t |> equal(sign_name(0), "zero")
        t |> equal(sign_name(5), "positive")
        t |> equal(sign_name(-5), "negative")
    }
    t |> run("a return in a block body leaves the arm, not the loop or the function") @(t : T?) {
        t |> equal(bucket_sum([3, 25, 7, 40]), 1 + 4 + 1 + 16)
    }
}

def private kind_of(t) : string {
    static_match (t) {
        true => $ { return "true" }
        false => $ { return "false" }
        _ => $ { return "other" }
    }
    return ""
}

[test(description = "A static match instance keeps an arm unreachable for it but needed by another instance")]
def test_static_match_keeps_arms_other_instances_need(t : T?) {
    t |> run("an arm unreachable for one instance is not an error") @(t : T?) {
        t |> equal(kind_of(true), "true")
        t |> equal(kind_of(false), "false")
    }
    t |> run("the same arm matches in another instance") @(t : T?) {
        t |> equal(kind_of(5), "other")
    }
}
