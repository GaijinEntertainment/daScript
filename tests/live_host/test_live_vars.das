options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require live/live_vars
require live_host
require daslib/archive

// --- Test globals tagged with @live ---

var @live test_int = 42
var @live test_float = 3.14
var @live test_string = "hello"
var @live test_bool = true
var @live test_array : array<float>

// Globals without @live — should NOT be serialized
var not_live_value = 999

[test(description = "a module with @live globals compiles with the generated serialization functions")]
def test_live_vars_compiles(t : T?) {
    t |> run("@live annotation compiles") @(t : T?) {
        // If the macro failed, this file wouldn't compile at all.
        // The macro generates __before_reload_live_vars and __after_reload_live_vars
        // which the host discovers by name prefix.
        t |> success(true, "@live globals compiled — macro generated serialization functions")
    }
}

[test(description = "int, string and float-array values round-trip through an Archive and a store, and garbage data is survived through try and recover")]
def test_live_vars_archive_round_trip(t : T?) {   // nolint:STYLE038 flat list of independent round-trip arms
    t |> run("int round-trip via Archive") @(t : T?) {
        var original = 99
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        arch |> serialize(original)
        var data <- ser->extractData()
        unsafe { delete ser; }

        var restored = 0
        var deser = new MemSerializer(data)
        var arch2 = Archive(reading = true, stream = deser)
        arch2 |> serialize(restored)
        unsafe { delete deser; }

        t |> equal(restored, 99)
    }

    t |> run("string round-trip via Archive") @(t : T?) {
        var original = "test string"
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        arch |> serialize(original)
        var data <- ser->extractData()
        unsafe { delete ser; }

        var restored = ""
        var deser = new MemSerializer(data)
        var arch2 = Archive(reading = true, stream = deser)
        arch2 |> serialize(restored)
        unsafe { delete deser; }

        t |> equal(restored, "test string")
    }

    t |> run("array<float> round-trip via Archive") @(t : T?) {
        var original <- [1.0, 2.0, 3.0]
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        arch |> serialize(original)
        var data <- ser->extractData()
        unsafe { delete ser; }

        var restored : array<float>
        var deser = new MemSerializer(data)
        var arch2 = Archive(reading = true, stream = deser)
        arch2 |> serialize(restored)
        unsafe { delete deser; }

        t |> equal(length(restored), 3)
        t |> equal(restored[0], 1.0)
        t |> equal(restored[1], 2.0)
        t |> equal(restored[2], 3.0)
    }

    t |> run("multiple vars round-trip via store") @(t : T?) {
        // Simulate what the generated __before_reload_live_vars does
        test_int = 77
        test_float = 1.414
        test_string = "preserved"
        test_bool = false
        test_array |> clear()
        test_array |> push(10.0)
        test_array |> push(20.0)

        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        arch |> serialize(test_int)
        arch |> serialize(test_float)
        arch |> serialize(test_string)
        arch |> serialize(test_bool)
        arch |> serialize(test_array)
        let data <- ser->extractData()
        live_store_bytes("__test_live_vars_manual", data)
        unsafe { delete ser; }

        // Reset
        test_int = 0
        test_float = 0.0
        test_string = ""
        test_bool = true
        test_array |> clear()

        // Simulate what the generated __after_reload_live_vars does
        var load_data : array<uint8>
        let ok = live_load_bytes("__test_live_vars_manual", load_data)
        t |> success(ok, "load_bytes returned true")

        var deser = new MemSerializer(load_data)
        var arch2 = Archive(reading = true, stream = deser)
        arch2 |> serialize(test_int)
        arch2 |> serialize(test_float)
        arch2 |> serialize(test_string)
        arch2 |> serialize(test_bool)
        arch2 |> serialize(test_array)
        unsafe { delete deser; }

        t |> equal(test_int, 77)
        t |> equal(test_float, 1.414)
        t |> equal(test_string, "preserved")
        t |> equal(test_bool, false)
        t |> equal(length(test_array), 2)
        t |> equal(test_array[0], 10.0)
        t |> equal(test_array[1], 20.0)
    }

    t |> run("garbage data does not crash with try/recover") @(t : T?) {
        let garbage <- [uint8(0xFF), uint8(0xFE)]
        live_store_bytes("__test_garbage", garbage)

        var load_data : array<uint8>
        if (live_load_bytes("__test_garbage", load_data) && !empty(load_data)) {
            try {
                var deser = new MemSerializer(load_data)
                var arch = Archive(reading = true, stream = deser)
                var v : int
                arch |> serialize(v)
                var s = ""
                arch |> serialize(s)
                unsafe { delete deser; }
            } recover {
                // Expected — garbage data causes deserialization failure
            }
        }
        t |> success(true, "survived garbage data without crash")
    }
}

[test(description = "a live variable saves and loads under its own key when the init hash matches, discards stale data on a hash mismatch and uses hash 0 without an initializer")]
def test_live_vars_per_key_round_trip(t : T?) {   // nolint:STYLE038 flat list of independent round-trip arms
    t |> run("per-variable save/load with matching hash") <| @(t : T?) {
        // Simulate what the generated code does: hash + value
        let init_hash = int64(hash("42"))  // hash of describe(test_int.init) = "42"
        let key = "__test_per_key_int"
        // Save
        var value = 999
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var h = init_hash
        arch |> serialize(h)
        arch |> serialize(value)
        let data <- ser->extractData()
        live_store_bytes(key, data)
        unsafe { delete ser; }
        // Load with same hash — should restore
        var restored = 0
        var load_data : array<uint8>
        if (live_load_bytes(key, load_data) && !empty(load_data)) {
            var deser = new MemSerializer(load_data)
            var arch2 = Archive(reading = true, stream = deser)
            var stored_hash : int64
            arch2 |> serialize(stored_hash)
            if (stored_hash == init_hash) {
                arch2 |> serialize(restored)
            }
            unsafe { delete deser; }
        }
        t |> equal(restored, 999)
    }
    t |> run("hash mismatch discards stale data") <| @(t : T?) {
        let key = "__test_per_key_mismatch"
        let old_hash = int64(hash("42"))
        let new_hash = int64(hash("99"))  // init changed from 42 to 99
        // Save with old hash
        var value = 777
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var h = old_hash
        arch |> serialize(h)
        arch |> serialize(value)
        let data <- ser->extractData()
        live_store_bytes(key, data)
        unsafe { delete ser; }
        // Load expecting new hash — should NOT restore
        var restored = 0
        var load_data : array<uint8>
        if (live_load_bytes(key, load_data) && !empty(load_data)) {
            var deser = new MemSerializer(load_data)
            var arch2 = Archive(reading = true, stream = deser)
            var stored_hash : int64
            arch2 |> serialize(stored_hash)
            if (stored_hash == new_hash) {
                arch2 |> serialize(restored)
            }
            unsafe { delete deser; }
        }
        t |> equal(restored, 0)
    }
    t |> run("no-init var uses hash 0") <| @(t : T?) {
        let key = "__test_per_key_no_init"
        let no_init_hash = int64(0)
        // Save with hash 0
        var value = 55
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var h = no_init_hash
        arch |> serialize(h)
        arch |> serialize(value)
        let data <- ser->extractData()
        live_store_bytes(key, data)
        unsafe { delete ser; }
        // Load with hash 0 — should restore
        var restored = 0
        var load_data : array<uint8>
        if (live_load_bytes(key, load_data) && !empty(load_data)) {
            var deser = new MemSerializer(load_data)
            var arch2 = Archive(reading = true, stream = deser)
            var stored_hash : int64
            arch2 |> serialize(stored_hash)
            if (stored_hash == no_init_hash) {
                arch2 |> serialize(restored)
            }
            unsafe { delete deser; }
        }
        t |> equal(restored, 55)
    }
}

// --- Compilation smoke ---
//
// If the field-walking pass doesn't emit clean code, this file fails to
// compile. The generated `__before_reload_live_vars` / `__after_reload_live_vars`
// are called by the host (C++ findFunction lookup), not from daslang user code,
// so we can't invoke them directly here.

struct MixedFields {
    @live preserved_int : int
    @live preserved_str : string
    transient_int : int
    transient_str : string
}

var mixed_global : MixedFields  // non-@live; per-field preservation only

struct InnerState {
    @live keeper : int
    extra : int
}

var @live whole_global : InnerState  // @live on the global wins; whole struct preserved

[test(description = "a struct whose fields are marked @live compiles through the field-walking pass")]
def test_macro_emits_for_field_targets(t : T?) {
    t |> run("file with @live struct fields compiles") <| @(t : T?) {
        // Reaching this assertion proves the macro accepted both shapes:
        //   (1) non-@live global with @live fields  → per-field save/load
        //   (2) @live global with @live + non-@live fields → whole-struct save/load
        t |> success(true, "field-walking pass produced compilable code")
    }
}

// --- Per-field Archive round-trip ---
//
// Mirrors the byte sequence the generated per-field save/load blocks emit:
// `__live_vars_<mod>_<var>__<field>` keyed entries, each carrying an init-hash
// and one field value.

[test(description = "@live fields round-trip under per-field keys, transient fields do not, a field hash mismatch discards stale data, and the hash combines field and global initializers")]
def test_per_field_archive_round_trip(t : T?) {   // nolint:STYLE038 flat list of independent per-field round-trip arms
    t |> run("preserved_int round-trips via per-field key") <| @(t : T?) {
        let key = "__live_vars__mixed_global__preserved_int"
        let init_hash = int64(0)  // no init expr → hash 0

        mixed_global.preserved_int = 12345

        // Save (mirrors generated save block)
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var save_hash = init_hash
        arch |> serialize(save_hash)
        arch |> serialize(mixed_global.preserved_int)
        let data <- ser->extractData()
        live_store_bytes(key, data)
        unsafe { delete ser; }

        // Reset
        mixed_global.preserved_int = 0

        // Load (mirrors generated load block)
        var load_data : array<uint8>
        if (live_load_bytes(key, load_data) && !empty(load_data)) {
            var deser = new MemSerializer(load_data)
            var arch2 = Archive(reading = true, stream = deser)
            var stored_hash : int64
            arch2 |> serialize(stored_hash)
            if (stored_hash == init_hash) {
                arch2 |> serialize(mixed_global.preserved_int)
            }
            unsafe { delete deser; }
        }
        t |> equal(mixed_global.preserved_int, 12345)
    }

    t |> run("preserved_str round-trips via per-field key") <| @(t : T?) {
        let key = "__live_vars__mixed_global__preserved_str"
        let init_hash = int64(0)

        mixed_global.preserved_str := "kept across reload"

        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var save_hash = init_hash
        arch |> serialize(save_hash)
        arch |> serialize(mixed_global.preserved_str)
        let data <- ser->extractData()
        live_store_bytes(key, data)
        unsafe { delete ser; }

        mixed_global.preserved_str := ""

        var load_data : array<uint8>
        if (live_load_bytes(key, load_data) && !empty(load_data)) {
            var deser = new MemSerializer(load_data)
            var arch2 = Archive(reading = true, stream = deser)
            var stored_hash : int64
            arch2 |> serialize(stored_hash)
            if (stored_hash == init_hash) {
                arch2 |> serialize(mixed_global.preserved_str)
            }
            unsafe { delete deser; }
        }
        t |> equal(mixed_global.preserved_str, "kept across reload")
    }

    t |> run("transient field is NOT under any per-field key") <| @(t : T?) {
        // Confirm the conventional key for a transient field is empty —
        // the macro deliberately did not emit a save block for it.
        let key = "__live_vars__mixed_global__transient_int"
        var load_data : array<uint8>
        let ok = live_load_bytes(key, load_data)
        // The host returns false (or empty) for never-stored keys.
        if (ok) {
            t |> equal(length(load_data), 0)
        } else {
            t |> success(true, "no entry for transient field, as expected")
        }
    }

    t |> run("hash mismatch on a field discards stale data") <| @(t : T?) {
        let key = "__live_vars__mixed_global__preserved_int_mismatch"
        let old_hash = int64(hash("123"))
        let new_hash = int64(hash("999"))  // simulates source default change

        var v = 5555
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var h = old_hash
        arch |> serialize(h)
        arch |> serialize(v)
        let data <- ser->extractData()
        live_store_bytes(key, data)
        unsafe { delete ser; }

        var restored = 0
        var load_data : array<uint8>
        if (live_load_bytes(key, load_data) && !empty(load_data)) {
            var deser = new MemSerializer(load_data)
            var arch2 = Archive(reading = true, stream = deser)
            var stored_hash : int64
            arch2 |> serialize(stored_hash)
            if (stored_hash == new_hash) {
                arch2 |> serialize(restored)
            }
            unsafe { delete deser; }
        }
        t |> equal(restored, 0)
    }

    t |> run("per-field hash combines field init AND global init (XOR)") <| @(t : T?) {
        // Mirrors the macro's hash composition. Source-level edits to either
        // the struct field's init or the global's initializer must invalidate
        // the cache. Both parts together = single per-field key.
        let key = "__live_vars__synthetic_combined_hash"

        let g_init_old = int64(hash("S(x = 100)"))
        let g_init_new = int64(hash("S(x = 999)"))
        let f_init = int64(hash("5"))

        let combined_old = g_init_old ^ f_init
        let combined_new = g_init_new ^ f_init  // same field, different global init

        var v = 7777
        var ser = new MemSerializer()
        var arch = Archive(reading = false, stream = ser)
        var h = combined_old
        arch |> serialize(h)
        arch |> serialize(v)
        let data <- ser->extractData()
        live_store_bytes(key, data)
        unsafe { delete ser; }

        // Reload as if source g.init changed: we expect combined_new.
        var restored = 0
        var load_data : array<uint8>
        if (live_load_bytes(key, load_data) && !empty(load_data)) {
            var deser = new MemSerializer(load_data)
            var arch2 = Archive(reading = true, stream = deser)
            var stored_hash : int64
            arch2 |> serialize(stored_hash)
            if (stored_hash == combined_new) {
                arch2 |> serialize(restored)
            }
            unsafe { delete deser; }
        }
        t |> equal(restored, 0)  // editing g.init alone busts the per-field cache
    }
}
