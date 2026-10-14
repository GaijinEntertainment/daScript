options gen2
require dastest/testing_boost public
require daslib/match
require daslib/apply

//! A variant case written without a type is an index-only case: it
//! has an index and no value.

variant Tags {
    nada
    nothing
    zilch
    something
    anything : int
}

typedef Anon = variant<nada; nothing; zilch; something; anything : int>

variant TailTag {
    value : int
    done
}

variant Owner {
    none
    items : array<int>
}

struct Holder {
    tag : Tags
    value : int
}

variant Light {
    red
    green
}

struct Signal {
    light : Light
    count : int
}

def apply_visits(v : Tags) : string {
    var seen = ""
    apply(v) $(name, value) {
        seen += "{name}={value};"
    }
    return seen
}

def apply_visits_with_return(v : Tags) : string {
    var seen = ""
    apply(v) $(name, value) {
        seen += "{name}={value};"
        return
    }
    return seen
}

def Tags(doubled : int) : Tags => Tags(anything = doubled * 2)

def case_label(v : Tags) : string {
    return match (v) {
        Tags(nada) => "nada"
        Tags(nothing) => "nothing"
        Tags(zilch) => "zilch"
        Tags(something) => "something"
        Tags(anything = x) => "anything {x}"
    }
}

[test]
def test_bare_case_layout(t : T?) {
    t |> run("all-bare variant is one int") @(t : T?) {
        t |> equal(typeinfo sizeof(type<variant<nada; nothing; zilch>>), 4)
        t |> equal(typeinfo alignof(type<variant<nada; nothing; zilch>>), 4)
    }
    t |> run("bare case adds nothing to size or alignment") @(t : T?) {
        t |> equal(typeinfo sizeof(type<Tags>), 8)
        t |> equal(typeinfo alignof(type<Tags>), 4)
    }
    t |> run("all-bare variant held by value") @(t : T?) {
        var s = Signal(light = Light(green), count = 1)
        t |> equal(typeinfo sizeof(s), 8)
        t |> success(s.light is green)
        var lights <- [Light(red), Light(green), Light(red)]
        var greens = 0
        for (l in lights) {
            greens += l is green ? 1 : 0
        }
        t |> equal(greens, 1)
        s.light = lights[1]
        t |> success(s.light is green)
    }
    t |> run("alias and anonymous spelling are one type") @(t : T?) {
        let v : Tags
        let w : Anon = v
        t |> success(w is nada)
    }
}

[test]
def test_bare_case_indices(t : T?) {
    t |> run("zero memory is case 0") @(t : T?) {
        let v : Tags
        t |> equal(variant_index(v), 0)
        t |> success(v is nada)
        t |> success(!(v is nothing))
        let h : Holder
        t |> success(h.tag is nada)
        t |> success(default<Tags> is nada)
    }
    t |> run("each bare case is its own index") @(t : T?) {
        t |> equal(typeinfo variant_index<nada>(type<Tags>), 0)
        t |> equal(typeinfo variant_index<nothing>(type<Tags>), 1)
        t |> equal(typeinfo variant_index<zilch>(type<Tags>), 2)
        t |> equal(typeinfo variant_index<something>(type<Tags>), 3)
        t |> equal(typeinfo variant_index<anything>(type<Tags>), 4)
        t |> equal(typeinfo safe_variant_index < nope > (type<Tags>), -1)
    }
    t |> run("a bare case after a payload case") @(t : T?) {
        var v : TailTag
        t |> success(v is value)
        v = TailTag(done)
        t |> success(v is done)
        t |> equal(variant_index(v), 1)
    }
}

[test]
def test_bare_case_construct(t : T?) {
    t |> run("construct by name, switch by assignment") @(t : T?) {
        var v = Tags(nothing)
        t |> success(v is nothing)
        t |> equal(variant_index(v), 1)
        v = Tags(zilch)
        t |> success(v is zilch)
        v = Tags(anything = 7)
        t |> success(v is anything)
        t |> equal(v as anything, 7)
        v = Tags(something)
        t |> success(v is something)
        t |> success(!(v is anything))
    }
    t |> run("safe access on a bare case is null") @(t : T?) {
        var v = Tags(zilch)
        t |> equal(v ?as anything ?? -1, -1)
        v = Tags(anything = 3)
        t |> equal(v ?as anything ?? -1, 3)
    }
    t |> run("copy and clone keep the case") @(t : T?) {
        let v = Tags(something)
        let w = v
        t |> success(w is something)
        let c := v
        t |> success(c is something)
        let none_owner : Owner
        let none_clone := none_owner
        t |> success(none_clone is none)
        let items_owner = Owner(items <- [1, 2, 3])
        let items_clone := items_owner
        t |> success(items_clone is items)
        t |> equal(length(items_clone as items), 3)
    }
    t |> run("array of mixed cases") @(t : T?) {
        let arr <- [Tags(nada), Tags(anything = 1), Tags(zilch), Tags(anything = 2)]
        var bare = 0
        var sum = 0
        for (v in arr) {
            if (v is anything) {
                sum += v as anything
            } else {
                bare++
            }
        }
        t |> equal(bare, 2)
        t |> equal(sum, 3)
    }
}

[test]
def test_bare_case_match_and_print(t : T?) {
    t |> run("match covers bare cases without a wildcard") @(t : T?) {
        t |> equal(case_label(Tags(nada)), "nada")
        t |> equal(case_label(Tags(nothing)), "nothing")
        t |> equal(case_label(Tags(zilch)), "zilch")
        t |> equal(case_label(Tags(something)), "something")
        t |> equal(case_label(Tags(anything = 42)), "anything 42")
    }
    t |> run("print spells the case name") @(t : T?) {
        t |> equal("{Tags(zilch)}", "zilch")
        t |> equal("{Tags(anything = 5)}", "5")
    }
}

[test]
def test_bare_case_apply(t : T?) {
    t |> run("apply visits nothing for a bare case, inline path") @(t : T?) {
        t |> equal(apply_visits(Tags(zilch)), "")
        t |> equal(apply_visits(Tags(anything = 4)), "anything=4;")
    }
    t |> run("apply visits nothing for a bare case, escaping-return path") @(t : T?) {
        t |> equal(apply_visits_with_return(Tags(nothing)), "")
        t |> equal(apply_visits_with_return(Tags(anything = 6)), "anything=6;")
    }
}

[test]
def test_bare_case_names(t : T?) {
    t |> run("a function may share the typedef's name") @(t : T?) {
        let v = Tags(21)
        t |> success(v is anything)
        t |> equal(v as anything, 42)
        t |> success(Tags(nada) is nada)
        let anything = 5
        t |> equal(Tags(anything) as anything, 10)
    }
    t |> run("type names spell a bare case bare") @(t : T?) {
        t |> equal(typeinfo typename(type<Tags>),
            "variant<nada;nothing;zilch;something;anything:int>")
        t |> equal(sprint(Tags(nothing), print_flags.namesAndDimensions),
            "variant<nada; nothing; zilch; something; anything:int> = variant(nothing)")
    }
    t |> run("JSON writes null for a bare case and reads it back") @(t : T?) {
        t |> equal(sprint_json(Tags(nada), false), "\{\"nada\":null\}")
        t |> equal(sprint_json(Tags(anything = 3), false), "\{\"anything\":3\}")
        var scanned : Tags
        t |> success(sscan_json("\{\"nothing\":null\}", scanned))
        t |> success(scanned is nothing)
        t |> success(sscan_json("\{\"anything\":4\}", scanned))
        t |> equal(scanned ?as anything ?? -1, 4)
    }
}
