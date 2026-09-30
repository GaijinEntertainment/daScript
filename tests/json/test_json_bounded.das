options gen2
require dastest/testing_boost public
require daslib/json_boost

[test]
def bounded_json_preserves_unicode_and_rejects_invalid_tokens(t : T?) {
    let limits = JsonReadLimits(max_bytes = 1024, max_depth = 8, max_tokens = 128, max_string_bytes = 64)
    var error : string
    var value = read_json_bounded("[\"Привет\",\"\\uD83D\\uDE00\"]", limits, error)
    t |> equal(error, "")
    t |> success(value != null)
    if (value != null) {
        t |> equal(value?[0].value as _string, "Привет")
        t |> equal(value?[1].value as _string, "😀")
        delete_json(value)
    }
    for (text in ["[1] trailing", "[01]", "[1.]", "[1e]", "[1e9999]", "[\"\\q\"]", "[\"\\uD800\"]", "[\"\\uDC00\"]", "[\"\\uZZZZ\"]", "[\"\\u0000\"]", "[\"raw\nnewline\"]", "\{\"x\":1,\"x\":2\}"]) {
        value = read_json_bounded(text, limits, error)
        t |> success(value == null, "reject malformed input")
        t |> success(!empty(error))
        delete_json(value)
    }
    let invalid_utf8 = [34u8, 192u8, 128u8, 34u8]
    value = read_json_bounded(invalid_utf8, limits, error)
    t |> success(value == null)
    let nul = [49u8, 0u8, 50u8]
    value = read_json_bounded(nul, limits, error)
    t |> success(value == null, "raw NUL cannot hide a suffix")
}

[test]
def bounded_json_limits_are_inclusive_and_fail_without_a_tree(t : T?) {
    var error : string
    let limits = JsonReadLimits(max_bytes = 32, max_depth = 2, max_tokens = 5, max_string_bytes = 2)
    var value = read_json_bounded("[[0]]", limits, error)
    t |> success(value != null)
    delete_json(value)
    value = read_json_bounded("[[[0]]]", limits, error)
    t |> success(value == null)
    value = read_json_bounded("[0,1,2]", limits, error)
    t |> success(value == null)
    value = read_json_bounded("\"é\"", limits, error)
    t |> success(value != null, "two-byte string fits exactly")
    delete_json(value)
    value = read_json_bounded("\"éx\"", limits, error)
    t |> success(value == null)
    value = read_json_bounded("[0]", JsonReadLimits(max_bytes = 2), error)
    t |> success(value == null)
    value = read_json_bounded("[0]", JsonReadLimits(max_depth = 0), error)
    t |> success(value == null)
}

[test]
def bounded_json_does_not_inherit_permissive_duplicate_setting(t : T?) {
    let prior = set_allow_duplicate_keys(true)
    var error : string
    var value = read_json_bounded("\{\"x\":1,\"x\":2\}", JsonReadLimits(), error)
    t |> success(value == null)
    value = read_json("\{\"x\":1,\"x\":2\}", error)
    t |> success(value != null, "ordinary reader's configured behavior is restored")
    delete_json(value)
    set_allow_duplicate_keys(prior)
}
