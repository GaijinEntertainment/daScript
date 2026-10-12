options gen2
options indenting = 4
module unicode_text shared public
require daslib/utf8_utils
require daslib/unicode_text_data
require strings

let public UNICODE_TEXT_VERSION = "17.0.0" //! Bundled Unicode Character Database version.
let public UNICODE_CASELESS_KEY_VERSION = "unicode-17.0.0-d145-1" //! Persisted comparison keys must record this version and be reindexed when it changes.

struct private UnicodeMark {
    codepoint, combining_class : uint
}

def private append_decomposition(codepoint : uint; var result : array<uint>) {
    if (codepoint >= 0xac00u && codepoint < 0xd7a4u) {
        let syllable = codepoint - 0xac00u
        result |> push(0x1100u + syllable / 588u)
        result |> push(0x1161u + (syllable % 588u) / 28u)
        if (syllable % 28u != 0u) { result |> push(0x11a7u + syllable % 28u) }
        return
    }
    let mapping = unicode_data_decomposition(codepoint)
    if (mapping.z == 0u) {
        result |> push(codepoint)
    } else {
        append_decomposition(mapping.x, result)
        if (mapping.z == 2u) { append_decomposition(mapping.y, result) }
    }
}

def private order_marks(var result : array<uint>; start : int; var marks : array<UnicodeMark>) {
    if (length(marks) > 1) {
        stable_sort(marks) $(a, b : UnicodeMark) : bool { return a.combining_class < b.combining_class }
        for (mark, index in marks, count()) { result[start + index] = mark.codepoint }
    }
    clear(marks)
}

def private normalize_scalars(source : array<uint>; var result : array<uint>) {
    clear(result)
    reserve(result, length(source))
    for (codepoint in source) { append_decomposition(codepoint, result) }
    var inscope marks : array<UnicodeMark>
    reserve(marks, length(result))
    var start = 0
    for (index in range(length(result))) {
        let codepoint = result[index]
        let combining_class = unicode_data_combining_class(codepoint)
        if (combining_class == 0u) {
            order_marks(result, start, marks)
            start = index + 1
        } else {
            marks |> push(UnicodeMark(codepoint = codepoint, combining_class = combining_class))
        }
    }
    order_marks(result, start, marks)
}

def private fold_scalars(source : array<uint>; var result : array<uint>) {
    clear(result)
    reserve(result, length(source))
    for (codepoint in source) {
        let mapping = unicode_data_casefold(codepoint)
        if (mapping.w == 0u) {
            result |> push(codepoint)
        } else {
            result |> push(mapping.x)
            if (mapping.w > 1u) { result |> push(mapping.y) }
            if (mapping.w > 2u) { result |> push(mapping.z) }
        }
    }
}

def private encode_scalars(source : array<uint>) : string {
    var inscope bytes <- utf8_encode(source)
    return string(bytes)
}

def public try_unicode_nfd(value : string; var result : string&) : bool {
    //! Canonically decompose valid UTF-8 using Unicode17.0.0. Invalid UTF-8 leaves result unchanged.
    if (!is_utf8_string_valid(value)) return false
    var inscope source <- utf8_decode(value, false)
    var inscope normalized : array<uint>
    normalize_scalars(source, normalized)
    result = encode_scalars(normalized)
    return true
}
def public try_unicode_casefold(value : string; var result : string&) : bool {
    //! Full default, locale-independent Unicode case folding. Invalid UTF-8 leaves result unchanged.
    if (!is_utf8_string_valid(value)) return false
    var inscope source <- utf8_decode(value, false)
    var inscope folded : array<uint>
    fold_scalars(source, folded)
    result = encode_scalars(folded)
    return true
}
def public try_unicode_caseless_key(value : string; var result : string&) : bool {
    //! Unicode D145 canonical caseless key: NFD(casefold(NFD(value))). Invalid UTF-8 leaves result unchanged.
    //! Input and output may alias. Preserves accents; does not filter visual confusables or perform locale-specific casing.
    //! Record UNICODE_CASELESS_KEY_VERSION with persisted keys; future changes require explicit reindexing and collision handling.
    if (!is_utf8_string_valid(value)) return false
    var inscope source <- utf8_decode(value, false)
    var inscope normalized, folded, final_form : array<uint>
    normalize_scalars(source, normalized)
    fold_scalars(normalized, folded)
    normalize_scalars(folded, final_form)
    result = encode_scalars(final_form)
    return true
}
