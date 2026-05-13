// Single source of truth for daslang keyword/type/atom token sets.
// Derived from src/parser/ds2_lexer.lpp — 104 reserved words total.
// Consumers: daslang-mode.js (CodeMirror), highlight.js (blog tokenizer).

(function (global) {
    const TYPES = new Set([
        // primitives
        'bool', 'void', 'string', 'auto',
        // signed ints
        'int', 'int8', 'int16', 'int64', 'int2', 'int3', 'int4',
        // unsigned ints
        'uint', 'uint8', 'uint16', 'uint64', 'uint2', 'uint3', 'uint4',
        // floats
        'float', 'float2', 'float3', 'float4', 'double',
        // ranges
        'range', 'range64', 'urange', 'urange64',
        // type constructors (used like types)
        'array', 'table', 'tuple', 'variant', 'bitfield', 'smart_ptr',
        'block', 'function', 'lambda', 'generator', 'iterator', 'fixed_array',
    ]);

    const ATOMS = new Set(['true', 'false', 'null']);

    const KEYWORDS = new Set([
        'abstract', 'addr', 'aka', 'as', 'assume',
        'break',
        'capture', 'cast', 'class', 'const', 'continue',
        'def', 'default', 'delete', 'deref',
        'elif', 'else', 'enum', 'expect', 'explicit',
        'finally', 'for',
        'goto',
        'if', 'implicit', 'in', 'include', 'inscope', 'is',
        'label', 'let',
        'module',
        'new',
        'operator', 'options', 'override',
        'pass', 'private', 'public',
        'recover', 'reinterpret', 'require', 'return',
        'sealed', 'shared', 'static', 'static_elif', 'static_if', 'struct',
        'template', 'try', 'type', 'typedecl', 'typedef', 'typeinfo',
        'uninitialized', 'unsafe', 'upcast',
        'var',
        'where', 'while', 'with',
        'yield',
    ]);

    global.daslangKeywords = { KEYWORDS, TYPES, ATOMS };
})(typeof window !== 'undefined' ? window : globalThis);
