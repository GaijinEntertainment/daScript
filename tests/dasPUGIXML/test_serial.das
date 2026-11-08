options gen2

require pugixml/PUGIXML_boost
require dastest/testing_boost public
require math

// ============ Test types ============

struct Point {
    x : float
    y : float
}

enum Priority {
    low
    medium
    high
}

struct RenamedFields {
    @rename = "type" _type : string
    @rename = "class" _class : int
    value : float
}

struct EnumIntFields {
    normal : Priority
    @enum_as_int as_int : Priority
}

struct MixedAnnotations {
    @rename = "display_name" name : string
    @enum_as_int pri : Priority
    score : int
}

// ============ Annotation tests ============

[test(description = "the rename annotation makes to_XML write a field under its renamed element name")]
def test_xml_rename_serialize(t : T?) {
    t |> run("rename annotation changes element name in XML") <| @(t : T?) {
        let s = RenamedFields(_type = "widget", _class = 3, value = 1.5)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "item")
            XML(root, s)
            // Fields should use renamed element names
            t |> success(child(root, "type").ok)
            t |> success(child(root, "class").ok)
            t |> success(child(root, "value").ok)
            // Original names should NOT exist
            t |> success(!child(root, "_type").ok)
            t |> success(!child(root, "_class").ok)
        }
    }
}

[test(description = "the rename annotation makes from_XML read a field from its renamed element")]
def test_xml_rename_deserialize(t : T?) {
    t |> run("rename annotation reads from renamed element") <| @(t : T?) {
        let xml_str = "<item><type>button</type><class>5</class><value>2.0</value></item>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let result = from_XML(doc.document_element, type<RenamedFields>)
            t |> equal(result._type, "button")
            t |> equal(result._class, 5)
            t |> equal(result.value, 2.0)
        }
    }
}

[test(description = "a struct with a renamed field survives a to_XML then from_XML roundtrip")]
def test_xml_rename_roundtrip(t : T?) {
    t |> run("rename annotation roundtrips correctly") <| @(t : T?) {
        let original = RenamedFields(_type = "dialog", _class = 7, value = 3.14)
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<RenamedFields>)
            t |> equal(restored._type, original._type)
            t |> equal(restored._class, original._class)
            t |> equal(restored.value, original.value)
        }
    }
}

[test(description = "the enum_as_int annotation makes to_XML write an enum field as its integer value")]
def test_xml_enum_as_int_serialize(t : T?) {
    t |> run("enum_as_int writes integer value") <| @(t : T?) {
        let s = EnumIntFields(normal = Priority.high, as_int = Priority.medium)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "item")
            XML(root, s)
            // normal enum serializes as string name
            t |> equal(as_string(child(root, "normal").text, ""), "high")
            // @enum_as_int serializes as integer
            t |> equal(as_int(child(root, "as_int").text, -1), 1)
        }
    }
}

[test(description = "the enum_as_int annotation writes the zero enum value as 0")]
def test_xml_enum_as_int_zero(t : T?) {
    t |> run("enum_as_int zero value") <| @(t : T?) {
        let s = EnumIntFields(normal = Priority.low, as_int = Priority.low)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "item")
            XML(root, s)
            t |> equal(as_string(child(root, "normal").text, ""), "low")
            t |> equal(as_int(child(root, "as_int").text, -1), 0)
        }
    }
}

[test(description = "rename and enum_as_int annotations on different fields both apply during serialization")]
def test_xml_mixed_annotations_serialize(t : T?) {
    t |> run("mixed annotations: rename + enum_as_int") <| @(t : T?) {
        let s = MixedAnnotations(name = "test", pri = Priority.high, score = 42)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "item")
            XML(root, s)
            // @rename = "display_name" on name field
            t |> success(child(root, "display_name").ok)
            t |> success(!child(root, "name").ok)
            t |> equal(as_string(child(root, "display_name").text, ""), "test")
            // @enum_as_int on pri
            t |> equal(as_int(child(root, "pri").text, -1), 2)
            // no annotation on score
            t |> equal(as_int(child(root, "score").text, -1), 42)
        }
    }
}

[test(description = "a struct mixing rename and enum_as_int annotations survives a roundtrip")]
def test_xml_mixed_annotations_roundtrip(t : T?) {
    t |> run("mixed annotations roundtrip") <| @(t : T?) {
        let original = MixedAnnotations(name = "config", pri = Priority.medium, score = 99)
        let xml_str = to_XML(original, "item")
        // Verify the XML uses renamed elements
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> success(child(root, "display_name").ok)
            t |> equal(as_string(child(root, "display_name").text, ""), "config")
            // Deserialize back (from_XML also respects @rename)
            let restored = from_XML(root, type<MixedAnnotations>)
            t |> equal(restored.name, original.name)
            t |> equal(restored.pri, original.pri)
            t |> equal(restored.score, original.score)
        }
    }
}

[test(description = "fields without annotations serialize and deserialize normally next to annotated ones")]
def test_xml_annotation_defaults(t : T?) {
    t |> run("fields without annotations still work") <| @(t : T?) {
        let p = Point(x = 1.0, y = 2.0)
        let xml_str = to_XML(p, "point")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<Point>)
            t |> equal(restored.x, 1.0)
            t |> equal(restored.y, 2.0)
        }
    }
}

// ============ Test types ============

bitfield Perms {
    readable
    writable
    executable
}

bitfield Flags8 : uint8 {
    a
    b
    c
}

bitfield Flags16 : uint16 {
    x
    y
    z
}

bitfield Flags64 : uint64 {
    lo
    mid
    hi
}

struct WithBitfield {
    name : string
    perms : Perms
}

struct WithBitfield8 {
    flags : Flags8
}

struct WithBitfield16 {
    flags : Flags16
}

struct WithBitfield64 {
    flags : Flags64
}

// ============ bitfield tests ============

[test(description = "to_XML serializes struct with bitfield to XML")]
def test_xml_serialize_bitfield(t : T?) {
    t |> run("serialize struct with bitfield") <| @(t : T?) {
        let s = WithBitfield(name = "script", perms = Perms.readable | Perms.executable)
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint(child(root, "perms").text, 0u), 5u)
        }
    }
}

[test(description = "from_XML deserializes struct with bitfield from XML")]
def test_from_xml_bitfield(t : T?) {
    t |> run("deserialize struct with bitfield") <| @(t : T?) {
        let xml_str = "<item><name>doc</name><perms>3</perms></item>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let r = from_XML(doc.document_element, type<WithBitfield>)
            t |> equal(r.name, "doc")
            t |> equal(uint(r.perms), 3u)
        }
    }
}

[test(description = "struct with bitfield survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_bitfield(t : T?) {
    t |> run("roundtrip struct with bitfield") <| @(t : T?) {
        let original = WithBitfield(name = "binary", perms = Perms.readable | Perms.writable | Perms.executable)
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithBitfield>)
            t |> equal(restored.name, "binary")
            t |> equal(uint(restored.perms), 7u)
        }
    }
}

[test(description = "bitfield with no bits set survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_bitfield_empty(t : T?) {
    t |> run("roundtrip bitfield with no bits set") <| @(t : T?) {
        var original : WithBitfield
        original.name = "none"
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithBitfield>)
            t |> equal(uint(restored.perms), 0u)
        }
    }
}

[test(description = "bitfield8 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_bitfield8(t : T?) {
    t |> run("roundtrip bitfield8") <| @(t : T?) {
        var original : WithBitfield8
        original.flags = Flags8.a | Flags8.c
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint(child(root, "flags").text, 0u), 5u)
            let restored = from_XML(doc.document_element, type<WithBitfield8>)
            t |> equal(uint(restored.flags), 5u)
        }
    }
}

[test(description = "bitfield16 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_bitfield16(t : T?) {
    t |> run("roundtrip bitfield16") <| @(t : T?) {
        var original : WithBitfield16
        original.flags = Flags16.x | Flags16.y | Flags16.z
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint(child(root, "flags").text, 0u), 7u)
            let restored = from_XML(doc.document_element, type<WithBitfield16>)
            t |> equal(uint(restored.flags), 7u)
        }
    }
}

[test(description = "bitfield64 survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_bitfield64(t : T?) {
    t |> run("roundtrip bitfield64") <| @(t : T?) {
        var original : WithBitfield64
        original.flags = Flags64.lo | Flags64.hi
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let v = as_uint64(child(root, "flags").text, 0ul)
            t |> equal(v, 5ul)
            let restored = from_XML(doc.document_element, type<WithBitfield64>)
            t |> equal(uint64(restored.flags), uint64(original.flags))
        }
    }
}

[test(description = "bitfield64 high bit survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_bitfield64_high_bit(t : T?) {
    t |> run("roundtrip bitfield64 high bit") <| @(t : T?) {
        var original : WithBitfield64
        original.flags = bitfield64(1ul << 63ul)
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithBitfield64>)
            t |> equal(uint64(restored.flags), 1ul << 63ul)
        }
    }
}

[test(description = "bitfield64 empty survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_bitfield64_empty(t : T?) {
    t |> run("roundtrip bitfield64 empty") <| @(t : T?) {
        var original : WithBitfield64
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithBitfield64>)
            t |> equal(uint64(restored.flags), 0ul)
        }
    }
}

// ============ Test structs ============

struct WithDim {
    tag_name : string
    values : int[3]
}

// ============ Fixed array (dim) tests ============

[test(description = "to_XML serializes fixed array to XML to XML")]
def test_xml_serialize_dim(t : T?) {
    t |> run("serialize fixed array to XML") <| @(t : T?) {
        let arr = fixed_array(10, 20, 30)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "arr")
            XML(root, arr)
            var ch = root.first_child
            t |> equal(ch.text as int, 10)
            ch = ch.next_sibling
            t |> equal(ch.text as int, 20)
            ch = ch.next_sibling
            t |> equal(ch.text as int, 30)
        }
    }
}

[test(description = "to_XML serializes struct containing fixed array to XML")]
def test_xml_serialize_struct_with_dim(t : T?) {
    t |> run("serialize struct containing fixed array") <| @(t : T?) {
        let wd = WithDim(tag_name = "data", values = fixed_array(1, 2, 3))
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "item")
            XML(root, wd)
            t |> equal(node_text(root, "tag_name"), "data")
            let vals_node = child(root, "values")
            t |> success(vals_node.ok)
            var ch = vals_node.first_child
            t |> equal(ch.text as int, 1)
            ch = ch.next_sibling
            t |> equal(ch.text as int, 2)
            ch = ch.next_sibling
            t |> equal(ch.text as int, 3)
        }
    }
}

[test(description = "from_XML deserializes fixed array from XML")]
def test_from_xml_dim(t : T?) {
    t |> run("deserialize fixed array from XML") <| @(t : T?) {
        parse_xml("<arr><item>5</item><item>10</item><item>15</item></arr>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let arr = from_XML(root, type<int[3]>)
            t |> equal(arr[0], 5)
            t |> equal(arr[1], 10)
            t |> equal(arr[2], 15)
        }
    }
}

[test(description = "from_XML deserializes fixed array of strings from XML")]
def test_from_xml_dim_strings(t : T?) {
    t |> run("deserialize fixed array of strings from XML") <| @(t : T?) {
        parse_xml("<arr><item>a</item><item>b</item></arr>") $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            let arr = from_XML(root, type<string[2]>)
            t |> equal(arr[0], "a")
            t |> equal(arr[1], "b")
        }
    }
}

[test(description = "fixed array survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_dim(t : T?) {
    t |> run("roundtrip fixed array") <| @(t : T?) {
        let original = fixed_array(100, 200, 300)
        let xml_str = to_XML(original, "arr")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<int[3]>)
            t |> equal(restored[0], 100)
            t |> equal(restored[1], 200)
            t |> equal(restored[2], 300)
        }
    }
}

[test(description = "struct with fixed array field survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_struct_with_dim(t : T?) {
    t |> run("roundtrip struct with fixed array field") <| @(t : T?) {
        let original = WithDim(tag_name = "xyz", values = fixed_array(7, 8, 9))
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithDim>)
            t |> equal(restored.tag_name, "xyz")
            t |> equal(restored.values[0], 7)
            t |> equal(restored.values[1], 8)
            t |> equal(restored.values[2], 9)
        }
    }
}

// ============ Test types ============

enum Color {
    red
    green
    blue
}

struct WithEnum {
    name : string
    color : Color
}

// ============ enum tests ============

[test(description = "to_XML serializes struct with enum field to XML")]
def test_xml_serialize_enum(t : T?) {
    t |> run("serialize struct with enum field") <| @(t : T?) {
        let s = WithEnum(name = "sky", color = Color.blue)
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_string(child(root, "color").text, ""), "blue")
        }
    }
}

[test(description = "from_XML deserializes struct with enum field from XML")]
def test_from_xml_enum(t : T?) {
    t |> run("deserialize struct with enum field") <| @(t : T?) {
        let xml_str = "<item><name>sky</name><color>green</color></item>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let r = from_XML(doc.document_element, type<WithEnum>)
            t |> equal(r.name, "sky")
            t |> equal(r.color, Color.green)
        }
    }
}

[test(description = "struct with enum survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_enum(t : T?) {
    t |> run("roundtrip struct with enum") <| @(t : T?) {
        let original = WithEnum(name = "grass", color = Color.green)
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithEnum>)
            t |> equal(restored.name, "grass")
            t |> equal(restored.color, Color.green)
        }
    }
}

[test(description = "an enum holding its default value survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_enum_default(t : T?) {
    t |> run("roundtrip enum with default value") <| @(t : T?) {
        var original : WithEnum
        original.name = "fire"
        // color defaults to Color.red (first value = 0)
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithEnum>)
            t |> equal(restored.color, Color.red)
        }
    }
}

// ============ Test structs ============

struct WithInts {
    i8  : int8
    u8  : uint8
    i16 : int16
    u16 : uint16
    i64 : int64
    u64 : uint64
}

// ============ integer type tests ============

[test(description = "to_XML serializes int8 value to XML")]
def test_xml_serialize_int8(t : T?) {
    t |> run("serialize int8 value") <| @(t : T?) {
        var s : WithInts
        s.i8 = int8(-42)
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_int(child(root, "i8").text, 0), -42)
        }
    }
}

[test(description = "to_XML serializes uint8 value to XML")]
def test_xml_serialize_uint8(t : T?) {
    t |> run("serialize uint8 value") <| @(t : T?) {
        var s : WithInts
        s.u8 = uint8(200)
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint(child(root, "u8").text, 0u), 200u)
        }
    }
}

[test(description = "to_XML serializes int16 value to XML")]
def test_xml_serialize_int16(t : T?) {
    t |> run("serialize int16 value") <| @(t : T?) {
        var s : WithInts
        s.i16 = int16(-1000)
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_int(child(root, "i16").text, 0), -1000)
        }
    }
}

[test(description = "to_XML serializes uint16 value to XML")]
def test_xml_serialize_uint16(t : T?) {
    t |> run("serialize uint16 value") <| @(t : T?) {
        var s : WithInts
        s.u16 = uint16(50000)
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint(child(root, "u16").text, 0u), 50000u)
        }
    }
}

[test(description = "to_XML serializes int64 value to XML")]
def test_xml_serialize_int64(t : T?) {
    t |> run("serialize int64 value") <| @(t : T?) {
        var s : WithInts
        s.i64 = 9223372036854775807l
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_int64(child(root, "i64").text, 0l), 9223372036854775807l)
        }
    }
}

[test(description = "to_XML serializes uint64 value to XML")]
def test_xml_serialize_uint64(t : T?) {
    t |> run("serialize uint64 value") <| @(t : T?) {
        var s : WithInts
        s.u64 = 18446744073709551615ul
        let xml_str = to_XML(s, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(as_uint64(child(root, "u64").text, 0ul), 18446744073709551615ul)
        }
    }
}

[test(description = "from_XML deserializes int8 value from XML")]
def test_from_xml_int8(t : T?) {
    t |> run("deserialize int8 value") <| @(t : T?) {
        let xml_str = "<item><i8>-128</i8><u8>0</u8><i16>0</i16><u16>0</u16><i64>0</i64><u64>0</u64></item>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let r = from_XML(doc.document_element, type<WithInts>)
            t |> equal(int(r.i8), -128)
        }
    }
}

[test(description = "from_XML deserializes uint8 value from XML")]
def test_from_xml_uint8(t : T?) {
    t |> run("deserialize uint8 value") <| @(t : T?) {
        let xml_str = "<item><i8>0</i8><u8>255</u8><i16>0</i16><u16>0</u16><i64>0</i64><u64>0</u64></item>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let r = from_XML(doc.document_element, type<WithInts>)
            t |> equal(int(r.u8), 255)
        }
    }
}

[test(description = "from_XML deserializes int64 value from XML")]
def test_from_xml_int64(t : T?) {
    t |> run("deserialize int64 value") <| @(t : T?) {
        let xml_str = "<item><i8>0</i8><u8>0</u8><i16>0</i16><u16>0</u16><i64>-9223372036854775807</i64><u64>0</u64></item>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let r = from_XML(doc.document_element, type<WithInts>)
            t |> equal(r.i64, -9223372036854775807l)
        }
    }
}

[test(description = "all integer types survive a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_all_int_types(t : T?) {
    t |> run("roundtrip all integer types") <| @(t : T?) {
        let original = WithInts(i8 = int8(-100), u8 = uint8(200),
            i16 = int16(-30000), u16 = uint16(60000),
            i64 = -1234567890123456789l, u64 = 12345678901234567890ul)
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithInts>)
            t |> equal(int(restored.i8), -100)
            t |> equal(int(restored.u8), 200)
            t |> equal(int(restored.i16), -30000)
            t |> equal(int(restored.u16), 60000)
            t |> equal(restored.i64, -1234567890123456789l)
            t |> equal(restored.u64, 12345678901234567890ul)
        }
    }
}

[test(description = "integer default values survive a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_int_defaults(t : T?) {
    t |> run("roundtrip integer default values") <| @(t : T?) {
        var original : WithInts
        let xml_str = to_XML(original, "item")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<WithInts>)
            t |> equal(int(restored.i8), 0)
            t |> equal(int(restored.u8), 0)
            t |> equal(int(restored.i16), 0)
            t |> equal(int(restored.u16), 0)
            t |> equal(restored.i64, 0l)
            t |> equal(restored.u64, 0ul)
        }
    }
}

[test(description = "from_XML deserializes a standalone int8 value")]
def test_from_xml_standalone_int8(t : T?) {
    t |> run("from_XML standalone int8") <| @(t : T?) {
        let xml_str = "<val>-42</val>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let v = from_XML(doc.document_element, type<int8>)
            t |> equal(int(v), -42)
        }
    }
}

[test(description = "from_XML deserializes a standalone uint64 value")]
def test_from_xml_standalone_uint64(t : T?) {
    t |> run("from_XML standalone uint64") <| @(t : T?) {
        let xml_str = "<val>18446744073709551615</val>"
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let v = from_XML(doc.document_element, type<uint64>)
            t |> equal(v, 18446744073709551615ul)
        }
    }
}

[test(description = "an array of int16 survives a roundtrip")]
def test_xml_array_of_int16(t : T?) {
    t |> run("roundtrip array of int16") <| @(t : T?) {
        let arr <- [int16(-1), int16(0), int16(32767)]
        let xml_str = to_XML(arr, "list")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored <- from_XML(doc.document_element, type<array<int16>>)
            t |> equal(length(restored), 3)
            t |> equal(int(restored[0]), -1)
            t |> equal(int(restored[1]), 0)
            t |> equal(int(restored[2]), 32767)
        }
    }
}

// ============ Test structs ============

struct Person {
    name : string
    age : int
    active : bool
}

struct Book {
    title : string
    year : int
    price : float
}

struct Library {
    name : string
    books : array<Book>
}

struct Address {
    city : string
    zip : int
}

struct Employee {
    name : string
    address : Address
}

// ============ XML() serialize tests ============

[test(description = "to_XML serializes simple struct fields to XML to XML")]
def test_xml_serialize_simple(t : T?) {
    t |> run("serialize simple struct fields to XML") <| @(t : T?) {
        let p = Person(name = "Alice", age = 30, active = true)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "person")
            XML(root, p)
            t |> equal(node_text(root, "name"), "Alice")
            t |> equal(child(root, "age").text as int, 30)
            t |> equal(child(root, "active").text as bool, true)
        }
    }
}

[test(description = "to_XML serializes float fields to XML")]
def test_xml_serialize_floats(t : T?) {
    t |> run("serialize float fields") <| @(t : T?) {
        let pt = Point(x = 3.14, y = 2.72)
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "point")
            XML(root, pt)
            t |> success(abs(child(root, "x").text as float -3.14) < 0.01)
            t |> success(abs(child(root, "y").text as float -2.72) < 0.01)
        }
    }
}

[test(description = "to_XML serializes nested struct to XML")]
def test_xml_serialize_nested(t : T?) {
    t |> run("serialize nested struct") <| @(t : T?) {
        let emp = Employee(name = "Bob", address = Address(city = "NYC", zip = 10001))
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "employee")
            XML(root, emp)
            t |> equal(node_text(root, "name"), "Bob")
            let address_node = child(root, "address")
            t |> success(address_node.ok)
            t |> equal(node_text(address_node, "city"), "NYC")
            t |> equal(child(address_node, "zip").text as int, 10001)
        }
    }
}

[test(description = "to_XML serializes struct with array field to XML")]
def test_xml_serialize_array(t : T?) {
    t |> run("serialize struct with array field") <| @(t : T?) {
        var lib : Library
        lib.name = "City Library"
        lib.books |> push_clone(Book(title = "Book A", year = 2000, price = 19.99))
        lib.books |> push_clone(Book(title = "Book B", year = 2010, price = 29.99))
        with_doc() $(doc) {
            var dnode = doc as xml_node
            var root = append_child(dnode, "library")
            XML(root, lib)
            t |> equal(node_text(root, "name"), "City Library")
            let books_node = child(root, "books")
            t |> success(books_node.ok)
            // Array items are children named "item"
            let b1 = books_node.first_child
            t |> success(b1.ok)
            t |> equal(node_text(b1, "title"), "Book A")
            t |> equal(child(b1, "year").text as int, 2000)
            let b2 = b1.next_sibling
            t |> success(b2.ok)
            t |> equal(node_text(b2, "title"), "Book B")
            t |> equal(child(b2, "year").text as int, 2010)
        }
    }
}

[test(description = "to_XML with a root name returns the XML text as a string")]
def test_xml_convenience_string(t : T?) {
    t |> run("to_XML(value, root) returns XML string") <| @(t : T?) {
        let p = Point(x = 1.0, y = 2.0)
        let xml_str = to_XML(p, "point")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let root = doc.document_element
            t |> equal(root.name, "point")
            t |> success(abs(child(root, "x").text as float -1.0) < 0.01)
            t |> success(abs(child(root, "y").text as float -2.0) < 0.01)
        }
    }
}

[test(description = "to_XML without a root name uses root as the element name")]
def test_xml_convenience_default_root(t : T?) {
    t |> run("to_XML(value) uses 'root' as default element name") <| @(t : T?) {
        let p = Person(name = "Test", age = 1, active = false)
        let xml_str = to_XML(p)
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            t |> equal(doc.document_element.name, "root")
        }
    }
}

// ============ from_XML() deserialize tests ============

[test(description = "from_XML deserializes simple struct from XML")]
def test_from_xml_simple(t : T?) {
    t |> run("deserialize simple struct from XML") <| @(t : T?) {
        parse_xml("<person><name>Bob</name><age>25</age><active>false</active></person>") $(doc, ok) {
            t |> success(ok)
            let p = from_XML(doc.document_element, type<Person>)
            t |> equal(p.name, "Bob")
            t |> equal(p.age, 25)
            t |> equal(p.active, false)
        }
    }
}

[test(description = "from_XML deserializes float fields from XML")]
def test_from_xml_floats(t : T?) {
    t |> run("deserialize float fields") <| @(t : T?) {
        parse_xml("<pt><x>3.14</x><y>2.72</y></pt>") $(doc, ok) {
            t |> success(ok)
            let pt = from_XML(doc.document_element, type<Point>)
            t |> success(abs(pt.x - 3.14) < 0.01)
            t |> success(abs(pt.y - 2.72) < 0.01)
        }
    }
}

[test(description = "from_XML deserializes nested struct from XML")]
def test_from_xml_nested(t : T?) {
    t |> run("deserialize nested struct") <| @(t : T?) {
        let xml = "<emp><name>Carol</name><address><city>LA</city><zip>90001</zip></address></emp>"
        parse_xml(xml) $(doc, ok) {
            t |> success(ok)
            let emp = from_XML(doc.document_element, type<Employee>)
            t |> equal(emp.name, "Carol")
            t |> equal(emp.address.city, "LA")
            t |> equal(emp.address.zip, 90001)
        }
    }
}

[test(description = "from_XML deserializes struct with array from XML")]
def test_from_xml_array(t : T?) {
    t |> run("deserialize struct with array") <| @(t : T?) {
        let xml = "<lib><name>Town Lib</name><books><item><title>Alpha</title><year>2001</year><price>9.99</price></item><item><title>Beta</title><year>2002</year><price>19.99</price></item></books></lib>"
        parse_xml(xml) $(doc, ok) {
            t |> success(ok)
            let lib = from_XML(doc.document_element, type<Library>)
            t |> equal(lib.name, "Town Lib")
            t |> equal(length(lib.books), 2)
            t |> equal(lib.books[0].title, "Alpha")
            t |> equal(lib.books[0].year, 2001)
            t |> equal(lib.books[1].title, "Beta")
            t |> equal(lib.books[1].year, 2002)
        }
    }
}

[test(description = "from_XML leaves fields at their defaults when their elements are missing from the XML")]
def test_from_xml_missing_fields(t : T?) {
    t |> run("missing XML elements yield default values") <| @(t : T?) {
        parse_xml("<person><name>Partial</name></person>") $(doc, ok) {
            t |> success(ok)
            let p = from_XML(doc.document_element, type<Person>)
            t |> equal(p.name, "Partial")
            t |> equal(p.age, 0)
            t |> equal(p.active, false)
        }
    }
}

[test(description = "from_XML turns an empty container element into an empty array")]
def test_from_xml_empty_array(t : T?) {
    t |> run("empty container element yields empty array") <| @(t : T?) {
        parse_xml("<lib><name>Empty</name><books></books></lib>") $(doc, ok) {
            t |> success(ok)
            let lib = from_XML(doc.document_element, type<Library>)
            t |> equal(lib.name, "Empty")
            t |> equal(length(lib.books), 0)
        }
    }
}

// ============ Roundtrip tests ============

[test(description = "a simple struct survives a to_XML then from_XML roundtrip")]
def test_roundtrip_simple(t : T?) {
    t |> run("roundtrip struct -> XML -> struct") <| @(t : T?) {
        let original = Person(name = "Diana", age = 42, active = true)
        let xml_str = to_XML(original, "person")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<Person>)
            t |> equal(restored.name, original.name)
            t |> equal(restored.age, original.age)
            t |> equal(restored.active, original.active)
        }
    }
}

[test(description = "nested struct survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_nested(t : T?) {
    t |> run("roundtrip nested struct") <| @(t : T?) {
        let original = Employee(name = "Eve", address = Address(city = "SF", zip = 94102))
        let xml_str = to_XML(original, "employee")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<Employee>)
            t |> equal(restored.name, original.name)
            t |> equal(restored.address.city, original.address.city)
            t |> equal(restored.address.zip, original.address.zip)
        }
    }
}

[test(description = "struct with array field survives a to_XML then from_XML roundtrip unchanged")]
def test_roundtrip_with_array(t : T?) {
    t |> run("roundtrip struct with array field") <| @(t : T?) {
        var original : Library
        original.name = "Round Lib"
        original.books |> push_clone(Book(title = "Gamma", year = 2020, price = 39.99))
        original.books |> push_clone(Book(title = "Delta", year = 2021, price = 49.99))
        let xml_str = to_XML(original, "library")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<Library>)
            t |> equal(restored.name, original.name)
            t |> equal(length(restored.books), 2)
            t |> equal(restored.books[0].title, "Gamma")
            t |> equal(restored.books[0].year, 2020)
            t |> success(abs(restored.books[0].price - 39.99) < 0.01)
            t |> equal(restored.books[1].title, "Delta")
            t |> equal(restored.books[1].year, 2021)
        }
    }
}

[test(description = "float fields keep their exact value through a roundtrip")]
def test_roundtrip_floats(t : T?) {
    t |> run("roundtrip preserves float precision") <| @(t : T?) {
        let original = Point(x = 1.5, y = -3.25)
        let xml_str = to_XML(original, "point")
        parse_xml(xml_str) $(doc, ok) {
            t |> success(ok)
            let restored = from_XML(doc.document_element, type<Point>)
            t |> success(abs(restored.x - original.x) < 0.001)
            t |> success(abs(restored.y - original.y) < 0.001)
        }
    }
}

// ============ from_XML(string) convenience tests ============

[test(description = "from_XML given an XML string deserializes in one step")]
def test_from_xml_string_simple(t : T?) {
    t |> run("from_XML(string) one-step deserialize") <| @(t : T?) {
        let xml_str = "<person><name>Alice</name><age>30</age><active>true</active></person>"
        let p = from_XML(xml_str, type<Person>)
        t |> equal(p.name, "Alice")
        t |> equal(p.age, 30)
        t |> equal(p.active, true)
    }
}

[test(description = "from_XML given an XML string deserializes a nested struct")]
def test_from_xml_string_nested(t : T?) {
    t |> run("from_XML(string) nested struct") <| @(t : T?) {
        let xml_str = "<e><name>Bob</name><address><city>NYC</city><zip>10001</zip></address></e>"
        let emp = from_XML(xml_str, type<Employee>)
        t |> equal(emp.name, "Bob")
        t |> equal(emp.address.city, "NYC")
        t |> equal(emp.address.zip, 10001)
    }
}

[test(description = "to_XML followed by from_XML on the string roundtrips a value")]
def test_from_xml_string_roundtrip(t : T?) {
    t |> run("to_XML then from_XML(string) roundtrip") <| @(t : T?) {
        let original = Person(name = "Diana", age = 42, active = true)
        let xml_str = to_XML(original, "person")
        let restored = from_XML(xml_str, type<Person>)
        t |> equal(restored.name, original.name)
        t |> equal(restored.age, original.age)
        t |> equal(restored.active, original.active)
    }
}

[test(description = "from_XML given an XML string deserializes a struct with an array field")]
def test_from_xml_string_with_array(t : T?) {
    t |> run("from_XML(string) struct with array") <| @(t : T?) {
        var original : Library
        original.name = "Test Lib"
        original.books |> push_clone(Book(title = "A", year = 2020, price = 10.0))
        original.books |> push_clone(Book(title = "B", year = 2021, price = 20.0))
        let xml_str = to_XML(original, "lib")
        let restored = from_XML(xml_str, type<Library>)
        t |> equal(restored.name, "Test Lib")
        t |> equal(length(restored.books), 2)
        t |> equal(restored.books[0].title, "A")
        t |> equal(restored.books[1].title, "B")
    }
}
