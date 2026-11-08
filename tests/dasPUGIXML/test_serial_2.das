options gen2

require pugixml/PUGIXML_boost
require dastest/testing_boost public
require math

// ============ Test structs ============

struct WithTable {
    title : string
    scores : table<string; int>
}

// ============ table tests ============

[test(description = "to_XML serializes table<string;int> to XML to XML")]
def test_xml_serialize_table_string_int(t : T?) {
    t |> run("serialize table<string;int> to XML") <| @(t : T?) {
        var tab : table<string; int>
        unsafe {
            tab["alpha"] = 10
            tab["beta"] = 20
        }
        let xml_str = to_XML(tab, "scores")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let ch = doc.document_element.first_child
            t |> success(ch.ok)
            t |> equal(ch.name, "entry")
        }
    }
}

[test(description = "to_XML serializes struct containing table to XML")]
def test_xml_serialize_struct_with_table(t : T?) {
    t |> run("serialize struct containing table") <| @(t : T?) {
        var s : WithTable
        s.title = "game"
        unsafe {
            s.scores["alice"] = 100
            s.scores["bob"] = 200
        }
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let title_ch = child(root, "title")
            t |> equal(as_string(title_ch.text, ""), "game")
        }
    }
}

[test(description = "from_XML deserializes table<string;int> from XML")]
def test_from_xml_table_string_int(t : T?) {
    t |> run("deserialize table<string;int> from XML") <| @(t : T?) {
        let xml_str = "<scores><entry><_key>x</_key><_val>42</_val></entry><entry><_key>y</_key><_val>99</_val></entry></scores>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            var tab <- from_XML(doc.document_element, type<table<string; int>>)
            t |> equal(length(tab), 2)
            unsafe {
                t |> equal(tab["x"], 42)
                t |> equal(tab["y"], 99)
            }
        }
    }
}

[test(description = "from_XML deserializes table<string;string> from XML")]
def test_from_xml_table_string_string(t : T?) {
    t |> run("deserialize table<string;string> from XML") <| @(t : T?) {
        let xml_str = "<m><entry><_key>name</_key><_val>Alice</_val></entry></m>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            var tab <- from_XML(doc.document_element, type<table<string; string>>)
            t |> equal(length(tab), 1)
            unsafe(t |> equal(tab["name"], "Alice"))
        }
    }
}

[test(description = "table<string;int> survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_table_string_int(t : T?) {
    t |> run("roundtrip table<string;int>") <| @(t : T?) {
        var original : table<string; int>
        unsafe {
            original["a"] = 1
            original["b"] = 2
            original["c"] = 3
        }
        let xml_str = to_XML(original, "tab")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            var restored <- from_XML(doc.document_element, type<table<string; int>>)
            t |> equal(length(restored), 3)
            unsafe {
                t |> equal(restored["a"], 1)
                t |> equal(restored["b"], 2)
                t |> equal(restored["c"], 3)
            }
        }
    }
}

[test(description = "struct with table field survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_struct_with_table(t : T?) {
    t |> run("roundtrip struct with table field") <| @(t : T?) {
        var original : WithTable
        original.title = "test"
        unsafe {
            original.scores["p1"] = 50
            original.scores["p2"] = 75
        }
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            var restored <- from_XML(doc.document_element, type<WithTable>)
            t |> equal(restored.title, "test")
            t |> equal(length(restored.scores), 2)
            unsafe {
                t |> equal(restored.scores["p1"], 50)
                t |> equal(restored.scores["p2"], 75)
            }
        }
    }
}

[test(description = "empty table survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_table_empty(t : T?) {
    t |> run("roundtrip empty table") <| @(t : T?) {
        let original : table<string; int>
        let xml_str = to_XML(original, "tab")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored <- from_XML(doc.document_element, type<table<string; int>>)
            t |> equal(length(restored), 0)
        }
    }
}

// ============ key-only table (set) tests ============

[test(description = "to_XML serializes table<string> (set) to XML to XML")]
def test_xml_serialize_set(t : T?) {
    t |> run("serialize table<string> (set) to XML") <| @(t : T?) {
        let s : table<string> <- {"apple", "banana"}
        let xml_str = to_XML(s, "fruits")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            var count = 0
            for_each_child(root) $(ch) {
                t |> equal(ch.name, "item")
                count++
            }
            t |> equal(count, 2)
        }
    }
}

[test(description = "from_XML deserializes table<int> (set) from XML")]
def test_from_xml_set(t : T?) {
    t |> run("deserialize table<int> (set) from XML") <| @(t : T?) {
        let xml_str = "<s><item>10</item><item>20</item><item>30</item></s>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let s <- from_XML(doc.document_element, type<table<int>>)
            t |> equal(length(s), 3)
            t |> success(s |> key_exists(10))
            t |> success(s |> key_exists(20))
            t |> success(s |> key_exists(30))
        }
    }
}

[test(description = "table<string> (set) survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_set(t : T?) {
    t |> run("roundtrip table<string> (set)") <| @(t : T?) {
        let original : table<string> <- {"x", "y", "z"}
        let xml_str = to_XML(original, "set")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored <- from_XML(doc.document_element, type<table<string>>)
            t |> equal(length(restored), 3)
            t |> success(restored |> key_exists("x"))
            t |> success(restored |> key_exists("y"))
            t |> success(restored |> key_exists("z"))
        }
    }
}

[test(description = "empty set survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_set_empty(t : T?) {
    t |> run("roundtrip empty set") <| @(t : T?) {
        let original : table<int>
        let xml_str = to_XML(original, "set")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored <- from_XML(doc.document_element, type<table<int>>)
            t |> equal(length(restored), 0)
        }
    }
}

// ============ Test structs ============

struct WithTuple {
    tag_name : string
    pair : tuple<int; int>
}

// ============ Tuple tests ============

[test(description = "to_XML serializes tuple to XML to XML")]
def test_xml_serialize_tuple(t : T?) {
    t |> run("serialize tuple to XML") <| @(t : T?) {
        let tup = ("hello" => 42)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "pair")
            XML(root, tup)
            t |> equal(node_text(root, "_0"), "hello")
            t |> equal(child(root, "_1").text as int, 42)
        }
    }
}

[test(description = "to_XML serializes 3-element tuple to XML to XML")]
def test_xml_serialize_tuple_3(t : T?) {
    t |> run("serialize 3-element tuple to XML") <| @(t : T?) {
        var tup : tuple<name : string; score : float; rank : int>
        tup.name = "Alice"
        tup.score = 9.5
        tup.rank = 1
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "entry")
            XML(root, tup)
            t |> equal(node_text(root, "name"), "Alice")
            t |> success(abs(child(root, "score").text as float -9.5) < 0.01)
            t |> equal(child(root, "rank").text as int, 1)
        }
    }
}

[test(description = "to_XML serializes struct containing tuple to XML")]
def test_xml_serialize_struct_with_tuple(t : T?) {
    t |> run("serialize struct containing tuple") <| @(t : T?) {
        var wt : WithTuple
        wt.tag_name = "coords"
        wt.pair._0 = 10
        wt.pair._1 = 20
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "item")
            XML(root, wt)
            t |> equal(node_text(root, "tag_name"), "coords")
            let pair_node = child(root, "pair")
            t |> success(pair_node.ok)
            t |> equal(child(pair_node, "_0").text as int, 10)
            t |> equal(child(pair_node, "_1").text as int, 20)
        }
    }
}

[test(description = "from_XML deserializes tuple from XML")]
def test_from_xml_tuple(t : T?) {
    t |> run("deserialize tuple from XML") <| @(t : T?) {
        parse_xml("<pair><_0>world</_0><_1>99</_1></pair>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let tup = from_XML(root, type<tuple<string; int>>)
            t |> equal(tup._0, "world")
            t |> equal(tup._1, 99)
        }
    }
}

[test(description = "from_XML deserializes named tuple from XML")]
def test_from_xml_named_tuple(t : T?) {
    t |> run("deserialize named tuple from XML") <| @(t : T?) {
        parse_xml("<entry><name>Bob</name><score>8.5</score></entry>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let tup = from_XML(root, type<tuple<name : string; score : float>>)
            t |> equal(tup.name, "Bob")
            t |> success(abs(tup.score - 8.5) < 0.01)
        }
    }
}

[test(description = "a tuple survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_tuple(t : T?) {
    t |> run("roundtrip tuple serialize/deserialize") <| @(t : T?) {
        let original = ("test" => 777)
        let xml_str = to_XML(original, "pair")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<tuple<string; int>>)
            t |> equal(restored._0, "test")
            t |> equal(restored._1, 777)
        }
    }
}

[test(description = "struct with tuple field survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_struct_with_tuple(t : T?) {
    t |> run("roundtrip struct with tuple field") <| @(t : T?) {
        var original : WithTuple
        original.tag_name = "pos"
        original.pair._0 = 100
        original.pair._1 = 200
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithTuple>)
            t |> equal(restored.tag_name, "pos")
            t |> equal(restored.pair._0, 100)
            t |> equal(restored.pair._1, 200)
        }
    }
}

// ============ Test types ============

variant IntOrString {
    i_val : int
    s_val : string
}

struct WithVariant {
    tag_name : string
    data : IntOrString
}

// ============ Variant tests ============

[test(description = "to_XML serializes variant (int case) to XML to XML")]
def test_xml_serialize_variant_int(t : T?) {
    t |> run("serialize variant (int case) to XML") <| @(t : T?) {
        var v : IntOrString
        unsafe(v.i_val) = 42
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "val")
            XML(root, v)
            t |> equal(node_attr_int(root, "_variant"), 0)
            t |> equal(child(root, "i_val").text as int, 42)
        }
    }
}

[test(description = "to_XML serializes variant (string case) to XML to XML")]
def test_xml_serialize_variant_string(t : T?) {
    t |> run("serialize variant (string case) to XML") <| @(t : T?) {
        var v : IntOrString
        unsafe {
            set_variant_index(v, typeinfo variant_index<s_val>(v))
            v.s_val = "hello"
        }
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "val")
            XML(root, v)
            t |> equal(node_attr_int(root, "_variant"), 1)
            t |> equal(node_text(root, "s_val"), "hello")
        }
    }
}

[test(description = "to_XML serializes struct containing variant to XML")]
def test_xml_serialize_struct_with_variant(t : T?) {
    t |> run("serialize struct containing variant") <| @(t : T?) {
        var wv : WithVariant
        wv.tag_name = "test"
        unsafe {
            set_variant_index(wv.data, typeinfo variant_index<i_val>(wv.data))
            wv.data.i_val = 99
        }
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "item")
            XML(root, wv)
            t |> equal(node_text(root, "tag_name"), "test")
            let data_node = child(root, "data")
            t |> success(data_node.ok)
            t |> equal(node_attr_int(data_node, "_variant"), 0)
            t |> equal(child(data_node, "i_val").text as int, 99)
        }
    }
}

[test(description = "from_XML deserializes variant (int case) from XML")]
def test_from_xml_variant_int(t : T?) {
    t |> run("deserialize variant (int case) from XML") <| @(t : T?) {
        parse_xml("<val _variant=\"0\"><i_val>77</i_val></val>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let v = from_XML(root, type<IntOrString>)
            t |> success(v is i_val)
            t |> equal(v as i_val, 77)
        }
    }
}

[test(description = "from_XML deserializes variant (string case) from XML")]
def test_from_xml_variant_string(t : T?) {
    t |> run("deserialize variant (string case) from XML") <| @(t : T?) {
        parse_xml("<val _variant=\"1\"><s_val>world</s_val></val>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let v = from_XML(root, type<IntOrString>)
            t |> success(v is s_val)
            t |> equal(v as s_val, "world")
        }
    }
}

[test(description = "variant (int case) survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_variant_int(t : T?) {
    t |> run("roundtrip variant (int case)") <| @(t : T?) {
        var original : IntOrString
        unsafe(original.i_val) = 123
        let xml_str = to_XML(original, "val")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<IntOrString>)
            t |> success(restored is i_val)
            t |> equal(restored as i_val, 123)
        }
    }
}

[test(description = "variant (string case) survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_variant_string(t : T?) {
    t |> run("roundtrip variant (string case)") <| @(t : T?) {
        var original : IntOrString
        unsafe {
            set_variant_index(original, typeinfo variant_index<s_val>(original))
            original.s_val = "roundtrip"
        }
        let xml_str = to_XML(original, "val")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<IntOrString>)
            t |> success(restored is s_val)
            t |> equal(restored as s_val, "roundtrip")
        }
    }
}

[test(description = "struct with variant field survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_struct_with_variant(t : T?) {
    t |> run("roundtrip struct with variant field") <| @(t : T?) {
        var original : WithVariant
        original.tag_name = "entry"
        unsafe {
            set_variant_index(original.data, typeinfo variant_index<s_val>(original.data))
            original.data.s_val = "payload"
        }
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithVariant>)
            t |> equal(restored.tag_name, "entry")
            t |> success(restored.data is s_val)
            t |> equal(restored.data as s_val, "payload")
        }
    }
}

// ============ Test structs ============

struct WithVectors {
    pos   : float3
    size  : int2
    color : uint4
    span  : range
    span64 : range64
}

// ============ vector type tests ============

[test(description = "to_XML serializes float2 to XML")]
def test_xml_serialize_float2(t : T?) {
    t |> run("serialize float2") <| @(t : T?) {
        let v = float2(1.5, 2.5)
        let xml_str = to_XML(v, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_float(child(root, "x").text, 0.0), 1.5)
            t |> equal(as_float(child(root, "y").text, 0.0), 2.5)
        }
    }
}

[test(description = "to_XML serializes float3 to XML")]
def test_xml_serialize_float3(t : T?) {
    t |> run("serialize float3") <| @(t : T?) {
        let v = float3(1.0, 2.0, 3.0)
        let xml_str = to_XML(v, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_float(child(root, "x").text, 0.0), 1.0)
            t |> equal(as_float(child(root, "y").text, 0.0), 2.0)
            t |> equal(as_float(child(root, "z").text, 0.0), 3.0)
        }
    }
}

[test(description = "to_XML serializes float4 to XML")]
def test_xml_serialize_float4(t : T?) {
    t |> run("serialize float4") <| @(t : T?) {
        let v = float4(1.0, 2.0, 3.0, 4.0)
        let xml_str = to_XML(v, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_float(child(root, "x").text, 0.0), 1.0)
            t |> equal(as_float(child(root, "y").text, 0.0), 2.0)
            t |> equal(as_float(child(root, "z").text, 0.0), 3.0)
            t |> equal(as_float(child(root, "w").text, 0.0), 4.0)
        }
    }
}

[test(description = "to_XML serializes int2 to XML")]
def test_xml_serialize_int2(t : T?) {
    t |> run("serialize int2") <| @(t : T?) {
        let v = int2(10, 20)
        let xml_str = to_XML(v, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_int(child(root, "x").text, 0), 10)
            t |> equal(as_int(child(root, "y").text, 0), 20)
        }
    }
}

[test(description = "to_XML serializes int4 to XML")]
def test_xml_serialize_int4(t : T?) {
    t |> run("serialize int4") <| @(t : T?) {
        let v = int4(1, 2, 3, 4)
        let xml_str = to_XML(v, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_int(child(root, "x").text, 0), 1)
            t |> equal(as_int(child(root, "y").text, 0), 2)
            t |> equal(as_int(child(root, "z").text, 0), 3)
            t |> equal(as_int(child(root, "w").text, 0), 4)
        }
    }
}

[test(description = "to_XML serializes range to XML")]
def test_xml_serialize_range(t : T?) {
    t |> run("serialize range") <| @(t : T?) {
        let v = range(5, 15)
        let xml_str = to_XML(v, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_int(child(root, "x").text, 0), 5)
            t |> equal(as_int(child(root, "y").text, 0), 15)
        }
    }
}

[test(description = "to_XML serializes urange to XML")]
def test_xml_serialize_urange(t : T?) {
    t |> run("serialize urange") <| @(t : T?) {
        let v = urange(0u, 100u)
        let xml_str = to_XML(v, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint(child(root, "x").text, 0u), 0u)
            t |> equal(as_uint(child(root, "y").text, 0u), 100u)
        }
    }
}

[test(description = "to_XML serializes range64 to XML")]
def test_xml_serialize_range64(t : T?) {
    t |> run("serialize range64") <| @(t : T?) {
        let v = range64(100l, 9999999999l)
        let xml_str = to_XML(v, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_int64(child(root, "x").text, 0l), 100l)
            t |> equal(as_int64(child(root, "y").text, 0l), 9999999999l)
        }
    }
}

[test(description = "to_XML serializes urange64 to XML")]
def test_xml_serialize_urange64(t : T?) {
    t |> run("serialize urange64") <| @(t : T?) {
        let v = urange64(0ul, 18446744073709551615ul)
        let xml_str = to_XML(v, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint64(child(root, "x").text, 0ul), 0ul)
            t |> equal(as_uint64(child(root, "y").text, 0ul), 18446744073709551615ul)
        }
    }
}

[test(description = "float2 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_float2(t : T?) {
    t |> run("roundtrip float2") <| @(t : T?) {
        let original = float2(3.14, -2.71)
        let xml_str = to_XML(original, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<float2>)
            t |> success(abs(restored.x - 3.14) < 0.01)
            t |> success(abs(restored.y + 2.71) < 0.01)
        }
    }
}

[test(description = "float3 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_float3(t : T?) {
    t |> run("roundtrip float3") <| @(t : T?) {
        let original = float3(1.0, 2.0, 3.0)
        let xml_str = to_XML(original, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<float3>)
            t |> equal(restored, float3(1.0, 2.0, 3.0))
        }
    }
}

[test(description = "float4 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_float4(t : T?) {
    t |> run("roundtrip float4") <| @(t : T?) {
        let original = float4(1.0, 2.0, 3.0, 4.0)
        let xml_str = to_XML(original, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<float4>)
            t |> equal(restored, float4(1.0, 2.0, 3.0, 4.0))
        }
    }
}

[test(description = "int2 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_int2(t : T?) {
    t |> run("roundtrip int2") <| @(t : T?) {
        let original = int2(-5, 10)
        let xml_str = to_XML(original, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<int2>)
            t |> equal(restored, int2(-5, 10))
        }
    }
}

[test(description = "int3 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_int3(t : T?) {
    t |> run("roundtrip int3") <| @(t : T?) {
        let original = int3(1, -2, 3)
        let xml_str = to_XML(original, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<int3>)
            t |> equal(restored, int3(1, -2, 3))
        }
    }
}

[test(description = "int4 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_int4(t : T?) {
    t |> run("roundtrip int4") <| @(t : T?) {
        let original = int4(10, 20, 30, 40)
        let xml_str = to_XML(original, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<int4>)
            t |> equal(restored, int4(10, 20, 30, 40))
        }
    }
}

[test(description = "uint4 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_uint4(t : T?) {
    t |> run("roundtrip uint4") <| @(t : T?) {
        let original = uint4(1u, 2u, 3u, 4u)
        let xml_str = to_XML(original, "vec")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<uint4>)
            t |> equal(restored, uint4(1u, 2u, 3u, 4u))
        }
    }
}

[test(description = "range survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_range(t : T?) {
    t |> run("roundtrip range") <| @(t : T?) {
        let original = range(5, 15)
        let xml_str = to_XML(original, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<range>)
            t |> equal(restored.x, 5)
            t |> equal(restored.y, 15)
        }
    }
}

[test(description = "urange survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_urange(t : T?) {
    t |> run("roundtrip urange") <| @(t : T?) {
        let original = urange(10u, 50u)
        let xml_str = to_XML(original, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<urange>)
            t |> equal(restored.x, 10u)
            t |> equal(restored.y, 50u)
        }
    }
}

[test(description = "range64 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_range64(t : T?) {
    t |> run("roundtrip range64") <| @(t : T?) {
        let original = range64(-1000000000l, 1000000000l)
        let xml_str = to_XML(original, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<range64>)
            t |> equal(restored.x, -1000000000l)
            t |> equal(restored.y, 1000000000l)
        }
    }
}

[test(description = "urange64 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_urange64(t : T?) {
    t |> run("roundtrip urange64") <| @(t : T?) {
        let original = urange64(0ul, 18446744073709551615ul)
        let xml_str = to_XML(original, "rng")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<urange64>)
            t |> equal(restored.x, 0ul)
            t |> equal(restored.y, 18446744073709551615ul)
        }
    }
}

[test(description = "struct with vector fields survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_struct_with_vectors(t : T?) {
    t |> run("roundtrip struct with vector fields") <| @(t : T?) {
        let original = WithVectors(pos = float3(1.0, 2.0, 3.0), size = int2(800, 600),
            color = uint4(255u, 128u, 0u, 255u),
            span = range(10, 20), span64 = range64(-100l, 100l))
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithVectors>)
            t |> equal(restored.pos, float3(1.0, 2.0, 3.0))
            t |> equal(restored.size, int2(800, 600))
            t |> equal(restored.color, uint4(255u, 128u, 0u, 255u))
            t |> equal(restored.span.x, 10)
            t |> equal(restored.span.y, 20)
            t |> equal(restored.span64.x, -100l)
            t |> equal(restored.span64.y, 100l)
        }
    }
}

[test(description = "an array of float2 survives a roundtrip")]
def test_xml_array_of_float2(t : T?) {
    t |> run("roundtrip array of float2") <| @(t : T?) {
        let arr <- [float2(1.0, 2.0), float2(3.0, 4.0)]
        let xml_str = to_XML(arr, "list")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored <- from_XML(doc.document_element, type<array<float2>>)
            t |> equal(length(restored), 2)
            t |> equal(restored[0], float2(1.0, 2.0))
            t |> equal(restored[1], float2(3.0, 4.0))
        }
    }
}

[test(description = "vector default values survive a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_vector_defaults(t : T?) {
    t |> run("roundtrip vector default values") <| @(t : T?) {
        var original : WithVectors
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithVectors>)
            t |> equal(restored.pos, float3(0.0, 0.0, 0.0))
            t |> equal(restored.size, int2(0, 0))
            t |> equal(restored.color, uint4(0u, 0u, 0u, 0u))
            t |> equal(restored.span.x, 0)
            t |> equal(restored.span.y, 0)
        }
    }
}
