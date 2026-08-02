options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_language shared

require tree_sitter/tree_sitter public

//! Per-language editor settings — THE one home for language-specific data:
//! comment/string lexing hints, bracket pairs, indent behavior, foldable
//! node kinds, completion keywords. Profiles are hardcoded (das/C/C++/md/
//! json + plain fallback) and keyed by `syntax_canonical_language` names.

struct public TextLanguageProfile {
    //! Canonical language name ("daslang", "cpp", ...); "plain" = fallback.
    name : string
    //! Lexing hints for the immediate per-line restyle while editing.
    line_comment : string
    block_comment_open : string
    block_comment_close : string
    //! Bytes that open/close string literals (each byte is one delimiter).
    string_delimiters : string
    //! (open, close) byte pairs highlighted as matching brackets.
    bracket_pairs : array<int2>
    //! (open, close) byte pairs that drive auto-indent; empty = no indent.
    indent_pairs : array<int2>
    //! Tree-sitter node types that fold (multi-line nodes only).
    fold_kinds : table<string>
    //! Completion candidates always offered alongside document words; also
    //! THE keyword set for the per-line restyle and the fallback lexer.
    keywords : array<string>
    //! Ghost-text completion default (the per-language kill switch).
    ghost_completion : bool = true
}

def private standard_brackets : array<int2> {
    return <- [int2('(', ')'), int2('{', '}'), int2('[', ']')]
}

def private profile_daslang : TextLanguageProfile {
    return <- TextLanguageProfile(name = "daslang",
        line_comment = "//", block_comment_open = "/*", block_comment_close = "*/",
        string_delimiters = "\"'",
        bracket_pairs <- standard_brackets(),
        indent_pairs <- [int2('{', '}')],
        fold_kinds <- {
            "function_declaration", "structure_declaration", "enum_declaration",
            "variant_alias_declaration", "bitfield_alias_declaration",
            "typedef_declaration", "structure_method", "block_comment"
        },
        keywords <- [
            "abstract", "addr", "aka", "array", "as", "assert", "assume",
            "auto", "bitfield", "block", "bool", "break", "capture", "cast",
            "class", "const", "continue", "def", "default", "defer", "delete",
            "deref", "distinct", "double", "elif", "else", "enum", "expect",
            "explicit", "false", "finally", "float", "float2", "float3",
            "float4", "for", "function", "generator", "goto", "if",
            "implicit", "in", "include", "inscope", "int", "int2", "int3",
            "int4", "int8", "int16", "int64", "is", "iterator", "label",
            "lambda", "let", "module", "new", "null", "operator", "options",
            "override", "panic", "pass", "private", "public", "range",
            "recover", "reinterpret", "require", "return", "safe_addr",
            "sealed", "shared", "smart_ptr", "static", "static_if",
            "static_elif", "string", "struct", "table", "template", "true",
            "try", "tuple", "type", "typedecl", "typedef", "typeinfo",
            "uint", "uint2", "uint3", "uint4", "uint8", "uint16", "uint64",
            "uninitialized", "unsafe", "upcast", "urange", "var", "variant",
            "verify", "void", "where", "while", "with", "yield"
        ])
}

def private profile_c : TextLanguageProfile {
    return <- TextLanguageProfile(name = "c",
        line_comment = "//", block_comment_open = "/*", block_comment_close = "*/",
        string_delimiters = "\"'",
        bracket_pairs <- standard_brackets(),
        indent_pairs <- [int2('{', '}')],
        fold_kinds <- {
            "function_definition", "struct_specifier", "enum_specifier",
            "union_specifier", "comment"
        },
        keywords <- [
            "auto", "break", "case", "char", "const", "continue", "default",
            "do", "double", "else", "enum", "extern", "false", "float",
            "for", "goto", "if", "inline", "int", "long", "register",
            "restrict", "return", "short", "signed", "sizeof", "static",
            "struct", "switch", "true", "typedef", "union", "unsigned",
            "void", "volatile", "while", "NULL"
        ])
}

def private profile_cpp : TextLanguageProfile {
    return <- TextLanguageProfile(name = "cpp",
        line_comment = "//", block_comment_open = "/*", block_comment_close = "*/",
        string_delimiters = "\"'",
        bracket_pairs <- standard_brackets(),
        indent_pairs <- [int2('{', '}')],
        fold_kinds <- {
            "function_definition", "struct_specifier", "enum_specifier",
            "union_specifier", "class_specifier", "namespace_definition",
            "comment"
        },
        keywords <- [
            "alignas", "alignof", "auto", "bool", "break", "case", "catch",
            "char", "class", "const", "constexpr", "const_cast", "continue",
            "decltype", "default", "delete", "do", "double", "dynamic_cast",
            "else", "enum", "explicit", "extern", "false", "final", "float",
            "for", "friend", "goto", "if", "inline", "int", "long", "mutable",
            "namespace", "new", "noexcept", "nullptr", "operator", "override",
            "private", "protected", "public", "reinterpret_cast", "return",
            "short", "signed", "sizeof", "static", "static_assert",
            "static_cast", "struct", "switch", "template", "this",
            "thread_local", "throw", "true", "try", "typedef", "typeid",
            "typename", "union", "unsigned", "using", "virtual", "void",
            "volatile", "while"
        ])
}

def private profile_markdown : TextLanguageProfile {
    // no auto-indent, no keywords; brackets still pair for [](...) links
    return <- TextLanguageProfile(name = "markdown",
        bracket_pairs <- standard_brackets(),
        fold_kinds <- {
            "fenced_code_block", "section"
        })
}

def private profile_json : TextLanguageProfile {
    return <- TextLanguageProfile(name = "json",
        string_delimiters = "\"",
        bracket_pairs <- [int2('{', '}'), int2('[', ']')],
        indent_pairs <- [int2('{', '}'), int2('[', ']')],
        keywords <- ["true", "false", "null"],
        ghost_completion = false)
}

def private profile_plain : TextLanguageProfile {
    return <- TextLanguageProfile(name = "plain", ghost_completion = false)
}

var private g_profiles : table<string; TextLanguageProfile>

def private ensure_profiles {
    return if (!empty(g_profiles))
    g_profiles["daslang"] <- profile_daslang()
    g_profiles["c"] <- profile_c()
    g_profiles["cpp"] <- profile_cpp()
    g_profiles["markdown"] <- profile_markdown()
    g_profiles["json"] <- profile_json()
    g_profiles["plain"] <- profile_plain()
}

def public text_language_profile(language : string;
                                 blk : block<(profile : TextLanguageProfile const) : void>) {
    //! Look up the profile for a language name or file extension (canonical
    //! per `syntax_canonical_language`); unknown languages get the "plain"
    //! profile. The profile is passed by reference — read it, don't store it.
    ensure_profiles()
    let canonical = syntax_canonical_language(language)
    let key = key_exists(g_profiles, canonical) ? canonical : "plain"
    blk |> invoke(g_profiles[key])
}

def public text_language_known(language : string) : bool {
    //! True when the language has a first-class profile (not the fallback).
    ensure_profiles()
    return key_exists(g_profiles, syntax_canonical_language(language))
}
