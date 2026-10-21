options gen2
require dastest/testing_boost public
require daslib/rtti

struct Layer {
    gain : float = 1.0
    enabled : bool = true
    name : string = "layer"
}

struct Doc {
    layers : array<Layer>
    byName : table<string; Layer>
    solo : Layer?
}

variant Slot {
    bits : uint4
    layer : Layer
}

variant Pick {
    layer : Layer
    bits : uint4
}

struct Picks {
    items : array<Pick>
}

struct Cell {
    v : int = 7
}

struct Alt {
    w : int = 9
}

variant Choice {
    none : int
    alt : Alt
}

struct Holder {
    cells : table<string; Cell>
    choice : Choice = Choice(none = 0)
    pairs : array<tuple<Layer; int>>
}

struct Tree {
    value : int = 3
    kids : array<Tree>
}

class Node {
    value : int = 5
    def get() : int => value
}

struct Graph {
    nodes : array<Node?>
}

[test]
def test_created_values_run_the_initializer(t : T?) {
    t |> run("array element") @(t : T?) {
        var doc = Doc()
        t |> success(sscan_json("\{\"layers\":[\{\"gain\":0.5\},\{\},null]\}", doc))
        t |> equal(length(doc.layers), 3)
        t |> equal(doc.layers[0].gain, 0.5)
        t |> equal(doc.layers[0].enabled, true)
        t |> equal(doc.layers[0].name, "layer")
        t |> equal(doc.layers[1].gain, 1.0)
        t |> equal(doc.layers[2].enabled, true)
    }
    t |> run("table value") @(t : T?) {
        var doc = Doc()
        t |> success(sscan_json("\{\"byName\":\{\"a\":\{\"enabled\":false\}\}\}", doc))
        t |> equal(doc.byName?["a"]?.gain ?? 0.0, 1.0)
        t |> equal(doc.byName?["a"]?.enabled ?? true, false)
    }
    t |> run("pointee") @(t : T?) {
        var doc = Doc()
        t |> success(sscan_json("\{\"solo\":\{\"name\":\"s\"\}\}", doc))
        t |> success(doc.solo != null)
        t |> equal(doc.solo.gain, 1.0)
        t |> equal(doc.solo.enabled, true)
        t |> equal(doc.solo.name, "s")
    }
    t |> run("class pointee gets its methods") @(t : T?) {
        var g = Graph()
        t |> success(sscan_json("\{\"nodes\":[\{\"value\":9\},\{\}]\}", g))
        t |> equal(length(g.nodes), 2)
        t |> equal(g.nodes[0]->get(), 9)
        t |> equal(g.nodes[1]->get(), 5)
    }
    t |> run("types reached only as a table value or a variant alternative") @(t : T?) {
        var h = Holder()
        t |> success(sscan_json("\{\"cells\":\{\"a\":\{\}\},\"choice\":\{\"alt\":\{\}\}\}", h))
        t |> equal(h.cells?["a"]?.v ?? 0, 7)
        t |> equal((h.choice as alt).w, 9)
    }
    t |> run("tuple members") @(t : T?) {
        var h = Holder()
        t |> success(sscan_json("\{\"pairs\":[\{\"_0\":\{\"gain\":2.0\},\"_1\":5\}]\}", h))
        t |> equal(h.pairs[0]._0.gain, 2.0)
        t |> equal(h.pairs[0]._0.enabled, true)
        t |> equal(h.pairs[0]._1, 5)
    }
    t |> run("a structure that holds itself") @(t : T?) {
        var tree = Tree()
        t |> success(sscan_json("\{\"kids\":[\{\"kids\":[\{\}]\}]\}", tree))
        t |> equal(tree.kids[0].kids[0].value, 3)
    }
}

[test]
def test_sscan_json_at_runs_a_kept_initializer(t : T?) {
    var layers : array<Layer>
    t |> success(sscan_json_at("[\{\}]", unsafe(addr(layers)), typeinfo rtti_typeinfo(layers)))
    t |> equal(length(layers), 1)
    t |> equal(layers[0].gain, Layer().gain)
}

[test]
def test_variant_alternative(t : T?) {
    t |> run("a new alternative does not read the old one") @(t : T?) {
        var slot = Slot(bits = uint4(0x12345678u, 0u, 0u, 0u))
        t |> success(sscan_json("\{\"layer\":\{\"enabled\":false\}\}", slot))
        t |> success(slot is layer)
        t |> equal((slot as layer).gain, 1.0)
        t |> equal((slot as layer).enabled, false)
        t |> equal((slot as layer).name, "layer")
    }
    t |> run("the same alternative is updated in place") @(t : T?) {
        var slot = Slot(bits = uint4(0u))
        t |> success(sscan_json("\{\"layer\":\{\"gain\":3.0\}\}", slot))
        t |> success(sscan_json("\{\"layer\":\{\"enabled\":false\}\}", slot))
        t |> equal((slot as layer).gain, 3.0)
        t |> equal((slot as layer).enabled, false)
    }
    t |> run("a switch clears the bytes of the old alternative") @(t : T?) {
        var slot = Slot(bits = uint4(0u))
        t |> success(sscan_json("\{\"layer\":\{\"gain\":3.0\}\}", slot))
        t |> success(sscan_json("\{\"bits\":null\}", slot))
        t |> equal(slot as bits, uint4(0u))
    }
    t |> run("a new variant element starts as its first alternative") @(t : T?) {
        var picks = Picks()
        t |> success(sscan_json("\{\"items\":[\{\"layer\":\{\"enabled\":false\}\}]\}", picks))
        t |> equal((picks.items[0] as layer).gain, 1.0)
        t |> equal((picks.items[0] as layer).enabled, false)
    }
}
