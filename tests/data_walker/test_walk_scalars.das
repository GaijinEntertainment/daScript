options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require _dw_common

// Tests for DapiDataWalker — scalar types
//
// Covers: int, uint, int64, uint64, int8, uint8, int16, uint16,
//         float, double, bool, string, enum, bitfield




[test(description = "the data walker delivers int, uint, int64, uint64, int8, uint8, int16 and uint16 values")]
def test_walk_int(t : T?) {
    t |> run("int") @(t : T?) {
        var walker = new LogWalker()
        var x = 42
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Int:42\n")
        unsafe { delete walker; }
    }
    t |> run("uint") @(t : T?) {
        var walker = new LogWalker()
        var x = 0xFF_u
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "UInt:0xff\n")
        unsafe { delete walker; }
    }
    t |> run("int64") @(t : T?) {
        var walker = new LogWalker()
        var x = 100_000_000_000l
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Int64:100000000000\n")
        unsafe { delete walker; }
    }
    t |> run("uint64") @(t : T?) {
        var walker = new LogWalker()
        var x = 99ul
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "UInt64:0x63\n")
        unsafe { delete walker; }
    }
    t |> run("int8") @(t : T?) {
        var walker = new LogWalker()
        var x = int8(127)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Int8:127\n")
        unsafe { delete walker; }
    }
    t |> run("uint8") @(t : T?) {
        var walker = new LogWalker()
        var x = uint8(200)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "UInt8:200\n")
        unsafe { delete walker; }
    }
    t |> run("int16") @(t : T?) {
        var walker = new LogWalker()
        var x = int16(-1000)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Int16:-1000\n")
        unsafe { delete walker; }
    }
    t |> run("uint16") @(t : T?) {
        var walker = new LogWalker()
        var x = uint16(60000)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "UInt16:60000\n")
        unsafe { delete walker; }
    }
}


[test(description = "the data walker delivers float and double values")]
def test_walk_float_double(t : T?) {
    t |> run("float") @(t : T?) {
        var walker = new LogWalker()
        var x = 3.14
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Float:3.14\n")
        unsafe { delete walker; }
    }
    t |> run("double") @(t : T?) {
        var walker = new LogWalker()
        var x = 2.718281828lf
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Double:2.718281828\n")
        unsafe { delete walker; }
    }
}


[test(description = "the data walker delivers true, false, a string and an empty string")]
def test_walk_bool_string(t : T?) {
    t |> run("bool true") @(t : T?) {
        var walker = new LogWalker()
        var x = true
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Bool:true\n")
        unsafe { delete walker; }
    }
    t |> run("bool false") @(t : T?) {
        var walker = new LogWalker()
        var x = false
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "Bool:false\n")
        unsafe { delete walker; }
    }
    t |> run("string") @(t : T?) {
        var walker = new LogWalker()
        var x = "hello world"
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "String:hello world\n")
        unsafe { delete walker; }
    }
    t |> run("empty string") @(t : T?) {
        var walker = new LogWalker()
        var x = ""
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(x)), typeinfo rtti_typeinfo(x))
        }
        t |> equal(walker.log, "String:\n")
        unsafe { delete walker; }
    }
}


[test(description = "the data walker delivers enum values including the first one")]
def test_walk_enum(t : T?) {
    t |> run("enum value") @(t : T?) {
        var walker = new LogWalker()
        var c : Color = Color.Green
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(c)), typeinfo rtti_typeinfo(c))
        }
        t |> equal(walker.log, "Enum:Color:1\n")
        unsafe { delete walker; }
    }
    t |> run("first enum value") @(t : T?) {
        var walker = new LogWalker()
        var c : Color = Color.Red
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(c)), typeinfo rtti_typeinfo(c))
        }
        t |> equal(walker.log, "Enum:Color:0\n")
        unsafe { delete walker; }
    }
}


[test(description = "the data walker delivers a bitfield with flags set and an empty one")]
def test_walk_bitfield(t : T?) {
    t |> run("bitfield with flags set") @(t : T?) {
        var walker = new LogWalker()
        var p : Permissions = Permissions.read | Permissions.execute
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(p)), typeinfo rtti_typeinfo(p))
        }
        // read=bit0 (1), execute=bit2 (4) → 5 = 0x5
        t |> equal(walker.log, "Bitfield:0x5\n")
        unsafe { delete walker; }
    }
    t |> run("empty bitfield") @(t : T?) {
        var walker = new LogWalker()
        var p : Permissions
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(p)), typeinfo rtti_typeinfo(p))
        }
        t |> equal(walker.log, "Bitfield:0x0\n")
        unsafe { delete walker; }
    }
}

// Tests for DapiDataWalker — vector and range types
//
// Covers: int2/3/4, uint2/3/4, float2/3/4, range, urange, range64, urange64




[test(description = "the data walker delivers int, float and uint vectors of two, three and four lanes")]
def test_walk_vectors(t : T?) { // nolint:STYLE038 - flat list of independent walker arms
    t |> run("int2") @(t : T?) {
        var walker = new LogWalker()
        var v = int2(10, 20)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "Int2:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("int3") @(t : T?) {
        var walker = new LogWalker()
        var v = int3(1, 2, 3)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "Int3:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("int4") @(t : T?) {
        var walker = new LogWalker()
        var v = int4(1, 2, 3, 4)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "Int4:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("float2") @(t : T?) {
        var walker = new LogWalker()
        var v = float2(1.5, 2.5)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "Float2:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("float3") @(t : T?) {
        var walker = new LogWalker()
        var v = float3(1.0, 2.0, 3.0)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "Float3:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("float4") @(t : T?) {
        var walker = new LogWalker()
        var v = float4(1.0, 2.0, 3.0, 4.0)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "Float4:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("uint2") @(t : T?) {
        var walker = new LogWalker()
        var v = uint2(1u, 2u)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "UInt2:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("uint3") @(t : T?) {
        var walker = new LogWalker()
        var v = uint3(1u, 2u, 3u)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "UInt3:{v}\n")
        unsafe { delete walker; }
    }
    t |> run("uint4") @(t : T?) {
        var walker = new LogWalker()
        var v = uint4(1u, 2u, 3u, 4u)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(v)), typeinfo rtti_typeinfo(v))
        }
        t |> equal(walker.log, "UInt4:{v}\n")
        unsafe { delete walker; }
    }
}


[test(description = "the data walker delivers range, urange, range64 and urange64 values")]
def test_walk_ranges(t : T?) {
    t |> run("range") @(t : T?) {
        var walker = new LogWalker()
        var r = range(0, 10)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(r)), typeinfo rtti_typeinfo(r))
        }
        t |> equal(walker.log, "Range:{r}\n")
        unsafe { delete walker; }
    }
    t |> run("urange") @(t : T?) {
        var walker = new LogWalker()
        var r = urange(5u, 15u)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(r)), typeinfo rtti_typeinfo(r))
        }
        t |> equal(walker.log, "URange:{r}\n")
        unsafe { delete walker; }
    }
    t |> run("range64") @(t : T?) {
        var walker = new LogWalker()
        var r = range64(0l, 100l)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(r)), typeinfo rtti_typeinfo(r))
        }
        t |> equal(walker.log, "Range64:{r}\n")
        unsafe { delete walker; }
    }
    t |> run("urange64") @(t : T?) {
        var walker = new LogWalker()
        var r = urange64(10ul, 20ul)
        make_data_walker(walker) $(adapter) {
            adapter |> walk_data(unsafe(addr(r)), typeinfo rtti_typeinfo(r))
        }
        t |> equal(walker.log, "URange64:{r}\n")
        unsafe { delete walker; }
    }
}
