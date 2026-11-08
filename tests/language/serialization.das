options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
options persistent_heap

// todo: resize(array<Color>) is becoming
// unsafe under coverage.
options no_coverage

require dastest/testing_boost public
require daslib/archive
require math

struct Player {
    name : string
    score : int
    alive : bool
}

// Custom serialize: Color stores floats but serializes as 3 bytes
struct Color {
    r : float = 0.0
    g : float = 0.0
    b : float = 0.0
}

def serialize(var arch : Archive; var c : Color&) {
    if (arch.reading) {
        var rb, gb, bb : uint8
        arch |> serialize_raw(rb)
        arch |> serialize_raw(gb)
        arch |> serialize_raw(bb)
        c.r = float(rb) / 255.0
        c.g = float(gb) / 255.0
        c.b = float(bb) / 255.0
    } else {
        var rb = uint8(clamp(c.r * 255.0, 0.0, 255.0))
        var gb = uint8(clamp(c.g * 255.0, 0.0, 255.0))
        var bb = uint8(clamp(c.b * 255.0, 0.0, 255.0))
        arch |> serialize_raw(rb)
        arch |> serialize_raw(gb)
        arch |> serialize_raw(bb)
    }
}

[test(description = "an int, a float and a string each survive a mem_archive_save / mem_archive_load round trip")]
def test_primitive_round_trip(t : T?) {
    t |> run("int round trip") @(t : T?) {
        var val = 42
        var buf <- mem_archive_save(val)
        var loaded = 0
        mem_archive_load(buf, loaded)
        t |> equal(loaded, 42)
    }
    t |> run("float round trip") @(t : T?) {
        var val = 3.14
        var buf <- mem_archive_save(val)
        var loaded = 0.0
        mem_archive_load(buf, loaded)
        t |> equal(loaded, 3.14)
    }
    t |> run("string round trip") @(t : T?) {
        var val = "hello"
        var buf <- mem_archive_save(val)
        var loaded = ""
        mem_archive_load(buf, loaded)
        t |> equal(loaded, "hello")
    }
}

[test(description = "every field of a struct survives a mem archive round trip")]
def test_struct_round_trip(t : T?) {
    t |> run("Player struct") @(t : T?) {
        var p = Player(name = "hero", score = 100, alive = true)
        var buf <- mem_archive_save(p)
        var loaded : Player
        mem_archive_load(buf, loaded)
        t |> equal(loaded.name, "hero")
        t |> equal(loaded.score, 100)
        t |> equal(loaded.alive, true)
    }
}

[test(description = "an int array keeps its length and elements through a mem archive round trip")]
def test_array_round_trip(t : T?) {
    t |> run("int array") @(t : T?) {
        var arr <- [10, 20, 30]
        var buf <- mem_archive_save(arr)
        var loaded : array<int>
        mem_archive_load(buf, loaded)
        t |> equal(length(loaded), 3)
        t |> equal(loaded[0], 10)
        t |> equal(loaded[1], 20)
        t |> equal(loaded[2], 30)
    }
}

[test(description = "a string-to-int table keeps its keys and values through a mem archive round trip")]
def test_table_round_trip(t : T?) {
    t |> run("string-int table") @(t : T?) {
        var tab <- {"a" => 1, "b" => 2}
        var buf <- mem_archive_save(tab)
        var loaded : table<string; int>
        mem_archive_load(buf, loaded)
        t |> equal(key_exists(loaded, "a"), true)
        t |> equal(key_exists(loaded, "b"), true)
        unsafe {
            t |> equal(loaded["a"], 1)
            t |> equal(loaded["b"], 2)
        }
    }
}

[test(description = "both elements of an int-string tuple survive a mem archive round trip")]
def test_tuple_round_trip(t : T?) {
    t |> run("tuple int,string") @(t : T?) {
        var tup = (42, "answer")
        var buf <- mem_archive_save(tup)
        var loaded : tuple<int; string>
        mem_archive_load(buf, loaded)
        t |> equal(loaded._0, 42)
        t |> equal(loaded._1, "answer")
    }
}

[test(description = "values serialized one by one through a writing Archive over MemSerializer read back in order through a reading Archive")]
def test_manual_archive(t : T?) {
    t |> run("MemSerializer write then read") @(t : T?) {
        // Write phase
        var writer = new MemSerializer()
        var warch = Archive(reading = false, stream = writer)
        var name = "test"
        var score = 42
        warch |> serialize(name)
        warch |> serialize(score)

        // Read phase
        var data <- writer->extractData()
        var reader = new MemSerializer(data)
        var rarch = Archive(reading = true, stream = reader)
        var rname = ""
        var rscore = 0
        rarch |> serialize(rname)
        rarch |> serialize(rscore)
        t |> equal(rname, "test")
        t |> equal(rscore, 42)
    }
}

[test(description = "a user serialize overload for a struct replaces the default layout, for a single value and for each element of an array")]
def test_custom_serialize(t : T?) {
    t |> run("Color packs to 3 bytes") @(t : T?) {
        var c = Color(r = 1.0, g = 0.5, b = 0.0)
        var writer = new MemSerializer()
        var warch = Archive(reading = false, stream = writer)
        warch |> serialize(c)
        let data <- writer->extractData()
        // 3 bytes, not 12 (3 floats × 4)
        t |> equal(length(data), 3)
        // r=1.0 -> 255, g=0.5 -> 127, b=0.0 -> 0
        t |> equal(data[0], uint8(0xff))
        t |> equal(data[1], uint8(0x7f))
        t |> equal(data[2], uint8(0x0))
    }
    t |> run("Color round trip") @(t : T?) {
        var c = Color(r = 1.0, g = 0.0, b = 1.0)
        var writer = new MemSerializer()
        var warch = Archive(reading = false, stream = writer)
        warch |> serialize(c)
        var data <- writer->extractData()
        var reader = new MemSerializer(data)
        var rarch = Archive(reading = true, stream = reader)
        var loaded = Color()
        rarch |> serialize(loaded)
        t |> equal(loaded.r, 1.0)
        t |> equal(loaded.g, 0.0)
        t |> equal(loaded.b, 1.0)
    }
    t |> run("Color array uses custom serialize") @(t : T?) {
        var colors <- [Color(r = 1.0, g = 0.0, b = 0.0), Color(r = 0.0, g = 1.0, b = 0.0)]
        var writer = new MemSerializer()
        var warch = Archive(reading = false, stream = writer)
        warch |> serialize(colors)
        var data <- writer->extractData()
        // 4 bytes array length + 2 × 3 bytes = 10
        t |> equal(length(data), 10)
        var reader = new MemSerializer(data)
        var rarch = Archive(reading = true, stream = reader)
        var loaded : array<Color>
        rarch |> serialize(loaded)
        t |> equal(length(loaded), 2)
        t |> equal(loaded[0].r, 1.0)
        t |> equal(loaded[0].g, 0.0)
        t |> equal(loaded[1].g, 1.0)
        t |> equal(loaded[1].b, 0.0)
    }
}

[safe_when_uninitialized]
struct Bar {
    t : string = "blah"
    ta : array<float> <- make_10()
}

def make_10 => <- [for (i in range(10)); float(i)]

[safe_when_uninitialized]
struct Foo {
    data_bool : bool = true
    data_int : int = 1
    data_float : float = 3.14
    data_bar : Bar <- Bar()
    data_uint_3 : uint[3] = fixed_array<uint>(1u, 2u, 3u)
}

[test(description = "binary_save then binary_load restores scalar, string, nested-array and fixed-array fields of a struct with field initializers")]
def test_bin_serializer(t : T?) {
    let f0 <- Foo()
    var f1 : Foo
    binary_save(f0) $(data) {
        binary_load(f1, data)
        t |> equal(f1.data_bool, true)
        t |> equal(f1.data_int, 1)
        t |> equal(f1.data_float, 3.14)
        t |> equal(f1.data_bar.t, "blah")
        t |> success(f1.data_uint_3[0] == 1u && f1.data_uint_3[1] == 2u && f1.data_uint_3[2] == 3u)
        for (i in range(10)) {
            t |> equal(f1.data_bar.ta[i], float(i))
        }
    }
}

//! Keep the embedded fixed array so this fixture exercises differing array allocation strides.
struct BinaryStrideRecord { bytes : array<uint8>; values : float4[3] }
struct BinaryStrideEnvelope { records : array<BinaryStrideRecord> }
struct BinarySaveBuffer { bytes : array<uint8> }
[test(description = "binary_load decodes an array of records holding an array and a fixed array with the right element stride, so the decoded hash equals the source")]
def test_binary_array_element_stride(t : T?){
    var source : BinaryStrideEnvelope; source.records |> resize(3)
    for (record, i in source.records, count()){
        record.bytes |> resize(7 + i)
        for (value, k in record.bytes, count()){value = uint8(k + i)}
        record.values[2] = float4(float(i), 2.0, 3.0, 4.0)
    }
    binary_save(source) $(data){
        for (_repeat in range(8)){
            var decoded : BinaryStrideEnvelope
            binary_load(decoded, data)
            t |> equal(hash(decoded), hash(source), "nested arrays deserialize with the element stride")
            delete decoded
        }
    }
    delete source
}

[test(description = "repeated binary_save calls free each temporary serialization buffer, so heap usage does not grow")]
def test_binary_save_reclaims_temporary_buffer(t : T?){
    var source : BinarySaveBuffer; source.bytes |> resize(64 * 1024)
    for (value, i in source.bytes, count()){value = uint8(i)}
    binary_save(source) $(data){let _size = length(data)}
    let before = heap_bytes_allocated()
    for (_repeat in range(16)){
        binary_save(source) $(data){let _size = length(data)}
    }
    let after = heap_bytes_allocated()
    t |> equal(after, before, "binary_save releases each temporary serialization buffer")
    delete source
}

[test(description = "a panic in the binary_save callback propagates to the caller and still frees the temporary buffer")]
def test_binary_save_reclaims_buffer_when_callback_panics(t : T?) {
    var source : BinarySaveBuffer; source.bytes |> resize(64 * 1024)
    try {
        binary_save(source) $(data) {let _size = length(data); panic("callback failure")}
    } recover {}
    let before = heap_bytes_allocated()
    var caught = 0
    for (_repeat in range(16)) {
        try {
            binary_save(source) $(data) {let _size = length(data); panic("callback failure")}
        } recover {caught++}
    }
    let after = heap_bytes_allocated()
    t |> equal(caught, 16, "callback exceptions propagate to the caller")
    t |> equal(after, before, "callback exceptions release the temporary serialization buffer")
    delete source
}

struct WithLambda {
    fn : lambda<(x : int) : int>
    v : int
}

[test(description = "a struct with a null lambda field binary round-trips, keeping its other fields and loading the lambda as null")]
def test_bin_serializer_null_lambda(t : T?) {
    var w : WithLambda
    w.v = 13
    var loaded : WithLambda
    binary_save(w) $(data) {
        binary_load(loaded, data)
    }
    t |> equal(loaded.v, 13)
    t |> equal(loaded.fn == null, true)
}

struct BinaryEmptyStrings {
    null_text : string
    empty_text : string
    following_text : string
}

[test(description = "a null string and an empty string binary round-trip as themselves without corrupting the string after them")]
def test_binary_null_and_empty_strings(t : T?) {
    let source = BinaryEmptyStrings(empty_text = "", following_text = "after empty")
    var decoded : BinaryEmptyStrings
    binary_save(source) $(data) {
        binary_load(decoded, data)
    }
    t |> equal(decoded.null_text, source.null_text)
    t |> equal(decoded.empty_text, source.empty_text)
    t |> equal(decoded.following_text, source.following_text, "empty payloads preserve the following value")
}
