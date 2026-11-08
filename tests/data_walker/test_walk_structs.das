options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require _dw_common

// Tests for DapiDataWalker — structures and classes
//
// Covers: simple struct, nested struct, field last flags, class/derived class,
//         struct with all scalar types, struct with vectors and ranges




[test(description = "the data walker visits simple and nested structs and flags the last field correctly")]
def test_walk_struct(t : T?) {
    t |> run("simple struct") @(t : T?) {
        var walker = new LogWalker()
        var s = SimpleStruct(x = 10, y = 2.5)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(s)), typeinfo rtti_typeinfo(s))
        }
        var expected = "beforeStruct:SimpleStruct\n"
        expected += "beforeField:x:false\n"
        expected += "Int:10\n"
        expected += "afterField:x:false\n"
        expected += "beforeField:y:true\n"
        expected += "Float:2.5\n"
        expected += "afterField:y:true\n"
        expected += "afterStruct:SimpleStruct\n"
        t |> equal(walker.log, expected)
        unsafe { delete walker; }
    }
    t |> run("nested struct") @(t : T?) {
        var walker = new LogWalker()
        var s = NestedStruct(inner = SimpleStruct(x = 1, y = 2.0), tag = "test")
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(s)), typeinfo rtti_typeinfo(s))
        }
        // Should see: NestedStruct → inner(SimpleStruct → x, y) → tag
        t |> equal(find(walker.log, "beforeStruct:NestedStruct") != -1, true)
        t |> equal(find(walker.log, "beforeStruct:SimpleStruct") != -1, true)
        t |> equal(find(walker.log, "Int:1") != -1, true)
        t |> equal(find(walker.log, "Float:2") != -1, true)
        t |> equal(find(walker.log, "String:test") != -1, true)
        unsafe { delete walker; }
    }
    t |> run("field last flags") @(t : T?) {
        // ManyFields has 4 fields — only the last should have last=true
        var walker = new LogWalker()
        var s = ManyFields(a = 1, b = 2, c = 3, d = 4)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(s)), typeinfo rtti_typeinfo(s))
        }
        t |> equal(find(walker.log, "beforeField:a:false") != -1, true)
        t |> equal(find(walker.log, "beforeField:b:false") != -1, true)
        t |> equal(find(walker.log, "beforeField:c:false") != -1, true)
        t |> equal(find(walker.log, "beforeField:d:true") != -1, true)
        unsafe { delete walker; }
    }
}


[test(description = "the data walker visits a base class and a derived class with the inherited fields")]
def test_walk_class(t : T?) {
    t |> run("base class") @(t : T?) {
        var walker = new LogWalker()
        var a = new Animal(name = "cat")
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(a)), typeinfo rtti_typeinfo(a))
        }
        // Walking a class pointer: beforePtr → beforeStruct:Animal → field:name → afterStruct → afterPtr
        t |> equal(find(walker.log, "beforeStruct:Animal") != -1, true)
        t |> equal(find(walker.log, "String:cat") != -1, true)
        unsafe { delete a; delete walker; }
    }
    t |> run("derived class") @(t : T?) {
        var walker = new LogWalker()
        var d = new Dog(name = "Rex", breed = "Shepherd")
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(d)), typeinfo rtti_typeinfo(d))
        }
        // Derived class should walk Dog fields including breed
        t |> equal(find(walker.log, "beforeStruct:Dog") != -1, true)
        t |> equal(find(walker.log, "String:Rex") != -1, true)
        t |> equal(find(walker.log, "String:Shepherd") != -1, true)
        unsafe { delete d; delete walker; }
    }
}


[test(description = "a struct holding every scalar type is walked field by field")]
def test_walk_all_scalars_in_struct(t : T?) {
    t |> run("struct with all scalar types") @(t : T?) {
        var walker = new LogWalker()
        var s = AllScalars(
            b = true,
            i = 42,
            u = 0xFFu,
            f = 3.14,
            d = 2.718lf,
            s = "test",
            i8 = int8(8),
            u8 = uint8(16),
            i16 = int16(256),
            u16 = uint16(512),
            i64 = 100l,
            u64 = 200ul
        )
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(s)), typeinfo rtti_typeinfo(s))
        }
        // Verify all scalar callbacks fired
        t |> equal(find(walker.log, "Bool:true") != -1, true)
        t |> equal(find(walker.log, "Int:42") != -1, true)
        t |> equal(find(walker.log, "UInt:0xff") != -1, true)
        t |> equal(find(walker.log, "Float:3.14") != -1, true)
        t |> equal(find(walker.log, "Double:2.718") != -1, true)
        t |> equal(find(walker.log, "String:test") != -1, true)
        t |> equal(find(walker.log, "Int8:8") != -1, true)
        t |> equal(find(walker.log, "UInt8:16") != -1, true)
        t |> equal(find(walker.log, "Int16:256") != -1, true)
        t |> equal(find(walker.log, "UInt16:512") != -1, true)
        t |> equal(find(walker.log, "Int64:100") != -1, true)
        t |> equal(find(walker.log, "UInt64:") != -1, true)
        unsafe { delete walker; }
    }
}


[test(description = "vector and range fields inside a struct are walked")]
def test_walk_vectors_in_struct(t : T?) {
    t |> run("struct with vectors and range") @(t : T?) {
        var walker = new LogWalker()
        var s = VectorRangeStruct(
            v2 = float2(1.0, 2.0),
            v3 = float3(3.0, 4.0, 5.0),
            v4 = float4(6.0, 7.0, 8.0, 9.0),
            r = range(10, 20)
        )
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(s)), typeinfo rtti_typeinfo(s))
        }
        t |> equal(find(walker.log, "Float2:") != -1, true)
        t |> equal(find(walker.log, "Float3:") != -1, true)
        t |> equal(find(walker.log, "Float4:") != -1, true)
        t |> equal(find(walker.log, "Range:") != -1, true)
        unsafe { delete walker; }
    }
}

// Tests for DapiDataWalker — tuples and variants
//
// Covers: tuple (2-element, 3-element), variant (int/string/float alternatives)




[test(description = "the data walker visits two- and three-element tuples")]
def test_walk_tuple(t : T?) {
    t |> run("int-string tuple") @(t : T?) {
        var walker = new LogWalker()
        var tup : tuple<int; string> = (42, "hello")
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(tup)), typeinfo rtti_typeinfo(tup))
        }
        var expected = "beforeTuple\n"
        expected += "beforeTupleEntry:0:false\n"
        expected += "Int:42\n"
        expected += "afterTupleEntry:0:false\n"
        expected += "beforeTupleEntry:1:true\n"
        expected += "String:hello\n"
        expected += "afterTupleEntry:1:true\n"
        expected += "afterTuple\n"
        t |> equal(walker.log, expected)
        unsafe { delete walker; }
    }
    t |> run("3-element tuple") @(t : T?) {
        var walker = new LogWalker()
        var tup : tuple<int; float; bool> = (1, 2.0, true)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(tup)), typeinfo rtti_typeinfo(tup))
        }
        // Middle element should not be last
        t |> equal(find(walker.log, "beforeTupleEntry:1:false") != -1, true)
        // Last element should be last
        t |> equal(find(walker.log, "beforeTupleEntry:2:true") != -1, true)
        unsafe { delete walker; }
    }
}


[test(description = "the data walker visits the int, string and float alternatives of a variant")]
def test_walk_variant(t : T?) {
    t |> run("variant int alternative") @(t : T?) {
        var walker = new LogWalker()
        var v : MyVariant = MyVariant(i = 42)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(find(walker.log, "beforeVariant") != -1, true)
        t |> equal(find(walker.log, "Int:42") != -1, true)
        t |> equal(find(walker.log, "afterVariant") != -1, true)
        // Should NOT contain String or Float callbacks
        t |> equal(find(walker.log, "String:") == -1, true)
        t |> equal(find(walker.log, "Float:") == -1, true)
        unsafe { delete walker; }
    }
    t |> run("variant string alternative") @(t : T?) {
        var walker = new LogWalker()
        var v : MyVariant = MyVariant(s = "hello")
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(find(walker.log, "beforeVariant") != -1, true)
        t |> equal(find(walker.log, "String:hello") != -1, true)
        t |> equal(find(walker.log, "afterVariant") != -1, true)
        t |> equal(find(walker.log, "Int:") == -1, true)
        unsafe { delete walker; }
    }
    t |> run("variant float alternative") @(t : T?) {
        var walker = new LogWalker()
        var v : MyVariant = MyVariant(f = 9.5)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(find(walker.log, "Float:9.5") != -1, true)
        unsafe { delete walker; }
    }
}
