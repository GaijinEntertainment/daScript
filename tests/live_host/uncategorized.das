options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require live_host

[test(description = "live_store_bytes and live_load_bytes round-trip byte arrays including empty and large ones, miss on unknown keys and overwrite existing ones")]
def test_store_and_load(t : T?) {
    t |> run("store and load bytes") @(t : T?) {
        let data <- [uint8(1), uint8(2), uint8(3)]
        live_store_bytes("test_key", data)
        var loaded : array<uint8>
        let ok = live_load_bytes("test_key", loaded)
        t |> success(ok, "load_bytes should return true")
        t |> equal(length(loaded), 3)
        t |> equal(int(loaded[0]), 1)
        t |> equal(int(loaded[1]), 2)
        t |> equal(int(loaded[2]), 3)
    }
    t |> run("load missing key returns false") @(t : T?) {
        var loaded : array<uint8>
        let ok = live_load_bytes("nonexistent_key_xyz", loaded)
        t |> success(!ok, "load_bytes for missing key should return false")
        t |> equal(length(loaded), 0)
    }
    t |> run("overwrite existing key") @(t : T?) {
        var data1 : array<uint8>
        data1 |> push(uint8(10))
        live_store_bytes("overwrite_key", data1)
        let data2 <- [uint8(20), uint8(30)]
        live_store_bytes("overwrite_key", data2)
        var loaded : array<uint8>
        let ok = live_load_bytes("overwrite_key", loaded)
        t |> success(ok)
        t |> equal(length(loaded), 2)
        t |> equal(int(loaded[0]), 20)
        t |> equal(int(loaded[1]), 30)
    }
    t |> run("store empty array") @(t : T?) {
        let data : array<uint8>
        live_store_bytes("empty_key", data)
        var loaded : array<uint8>
        let ok = live_load_bytes("empty_key", loaded)
        t |> success(ok, "load_bytes for empty data should return true")
        t |> equal(length(loaded), 0)
    }
    t |> run("store large data") @(t : T?) {
        let data <- [for (i in range(10000)); uint8(i % 256)]
        live_store_bytes("large_key", data)
        var loaded : array<uint8>
        let ok = live_load_bytes("large_key", loaded)
        t |> success(ok)
        t |> equal(length(loaded), 10000)
        var all_match = true
        for (i in range(10000)) {
            if (int(loaded[i]) != i % 256) {
                all_match = false
                break
            }
        }
        t |> success(all_match, "large data should round-trip correctly")
    }
    t |> run("multiple independent keys") @(t : T?) {
        var a : array<uint8>
        a |> push(uint8(11))
        live_store_bytes("multi_a", a)
        var b : array<uint8>
        b |> push(uint8(22))
        live_store_bytes("multi_b", b)
        var la, lb : array<uint8>
        live_load_bytes("multi_a", la)
        live_load_bytes("multi_b", lb)
        t |> equal(int(la[0]), 11)
        t |> equal(int(lb[0]), 22)
    }
}

// ============================================================
//  live_store_string / live_load_string — string sibling of the
//  bytes-typed persistent store. Used by the LiveVarsPassMacro
//  JSON-text pivot (PR3) and the dasImgui widget serializer rail.
// ============================================================

[test(description = "live_store_string and live_load_string round-trip strings including empty and JSON text, miss on unknown keys, overwrite existing ones and keep their own store apart from bytes")]
def test_store_and_load_string(t : T?) {
    t |> run("store and load short string") @(t : T?) {
        live_store_string("string_key", "hello world")
        var loaded : string
        let ok = live_load_string("string_key", loaded)
        t |> success(ok, "load_string should return true")
        t |> equal(loaded, "hello world")
    }
    t |> run("load missing key returns false") @(t : T?) {
        var loaded : string
        let ok = live_load_string("nonexistent_string_xyz", loaded)
        t |> success(!ok, "load_string for missing key should return false")
    }
    t |> run("overwrite existing key") @(t : T?) {
        live_store_string("string_overwrite", "first")
        live_store_string("string_overwrite", "second")
        var loaded : string
        let ok = live_load_string("string_overwrite", loaded)
        t |> success(ok)
        t |> equal(loaded, "second")
    }
    t |> run("store empty string") @(t : T?) {
        live_store_string("string_empty", "")
        var loaded : string
        let ok = live_load_string("string_empty", loaded)
        t |> success(ok, "load_string for empty string should return true")
        t |> equal(loaded, "")
    }
    t |> run("store JSON-like content") @(t : T?) {
        let json = "\{\"a\":42,\"b\":\"hello\"\}"
        live_store_string("string_json", json)
        var loaded : string
        let ok = live_load_string("string_json", loaded)
        t |> success(ok)
        t |> equal(loaded, json)
    }
    t |> run("multiple independent string keys") @(t : T?) {
        live_store_string("string_multi_a", "alpha")
        live_store_string("string_multi_b", "beta")
        var la, lb : string
        live_load_string("string_multi_a", la)
        live_load_string("string_multi_b", lb)
        t |> equal(la, "alpha")
        t |> equal(lb, "beta")
    }
    t |> run("bytes and strings live in separate stores") @(t : T?) {
        // Same key in both stores — must not collide.
        var data : array<uint8>
        data |> push(uint8(42))
        live_store_bytes("shared_key", data)
        live_store_string("shared_key", "string_value")
        var loaded_bytes : array<uint8>
        var loaded_string : string
        let ok1 = live_load_bytes("shared_key", loaded_bytes)
        let ok2 = live_load_string("shared_key", loaded_string)
        t |> success(ok1, "bytes side intact")
        t |> success(ok2, "string side intact")
        t |> equal(length(loaded_bytes), 1)
        t |> equal(int(loaded_bytes[0]), 42)
        t |> equal(loaded_string, "string_value")
    }
}
