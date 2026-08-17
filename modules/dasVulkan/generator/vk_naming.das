options gen2
options _comment_hygiene = true
options indenting = 4
options persistent_heap = true

module vk_naming shared

require strings
require daslib/strings_boost

//! das reserved words that can show up as struct member / bitfield bit names
let DAS_KEYWORDS : table<string> <- {
    "type", "module", "range", "class", "struct", "enum", "table", "array", "block",
    "options", "string", "float", "int", "uint", "bool", "void", "return", "break",
    "continue", "if", "else", "elif", "while", "for", "in", "let", "var", "def",
    "true", "false", "null", "new", "delete", "try", "recover", "panic", "label",
    "goto", "with", "cast", "upcast", "addr", "deref", "typeinfo", "is", "as",
    "operator", "require", "public", "private", "shared", "smart_ptr", "generator",
    "yield", "static", "abstract", "override", "sealed", "implicit", "explicit",
    "shift", "unsafe", "inscope", "aka", "assume", "finally", "pass", "expect"
}

//! suffix '_' onto das reserved words
def public das_safe_name(s : string) : string {
    return key_exists(DAS_KEYWORDS, s) ? "{s}_" : s
}

//! "VkStructureType" -> "VK_STRUCTURE_TYPE"; acronym runs stay one word (ASTCDecode -> ASTC_DECODE)
def public camel_to_screaming(s : string) : string {
    var result = ""
    peek_data(s) $(arr) {
        let n = length(arr)
        result = build_string() $(var w) {
            for (i in range(n)) {
                let c = int(arr[i])
                let prev = i > 0 ? int(arr[i - 1]) : 0
                let next = i + 1 < n ? int(arr[i + 1]) : 0
                let c_up = c >= 'A' && c <= 'Z'
                let prev_lo = prev >= 'a' && prev <= 'z'
                let prev_up = prev >= 'A' && prev <= 'Z'
                let next_lo = next >= 'a' && next <= 'z'
                if (c_up && (prev_lo || (prev_up && next_lo))) {
                    w |> write_char('_')
                } elif (is_number(c) && (prev_lo || prev_up)) {
                    w |> write_char('_')
                }
                w |> write_char((c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c)
            }
        }
    }
    return result
}

//! split a trailing vendor tag off a type name: "VkColorSpaceKHR" -> ("VkColorSpace", "KHR")
def public strip_vendor_tag(s : string; tags : array<string>) : tuple<base : string; tag : string> {
    var found_base = s
    var found_tag = ""
    peek_data(s) $(d) {
        for (t in tags) {
            // require a lowercase char right before the tag so "VkASTC..." doesn't lose a fake tag
            if (d |> ends_with(t) && length(d) > length(t)) {
                found_base = d |> slice(0, length(d) - length(t))
                found_tag = t
                break
            }
        }
    }
    return (base = found_base, tag = found_tag)
}

//! the SCREAMING_CASE_ prefix shared by an enum's value names: VkAccessFlagBits2 -> "VK_ACCESS_2_"
def public enum_value_prefix(enum_name : string; is_bitmask : bool; tags : array<string>) : string {
    let st = strip_vendor_tag(enum_name, tags)
    var prefix = camel_to_screaming(st.base)
    if (is_bitmask) {
        prefix = prefix |> replace("_FLAG_BITS", "")
    }
    return prefix + "_"
}

//! de-prefixed das-side enum entry name; falls back to the full name minus VK_
def public enum_short_name(prefix : string; value_name : string) : string {
    var short = value_name
    if (value_name |> starts_with(prefix)) {
        short = value_name |> slice(length(prefix))
    } else {
        short = value_name |> trim_prefix("VK_")
    }
    if (!empty(short)) {
        let c0 = first_character(short)
        if (is_number(c0)) {
            short = "_{short}"
        }
    }
    return short
}

//! bitfield bit name from a de-prefixed FlagBits entry: "SPARSE_BINDING_BIT" -> "sparse_binding"
def public bit_name(short : string) : string {
    var parts <- split(short, "_")
    var name = build_string() $(var w) {
        var first = true
        for (p in parts) {
            continue if (p == "BIT")
            if (!first) {
                w |> write_char('_')
            }
            w |> write(to_lower(p))
            first = false
        }
    }
    if (!empty(name)) {
        let c0 = first_character(name)
        if (is_number(c0)) {
            name = "_{name}"
        }
    }
    return das_safe_name(name)
}
