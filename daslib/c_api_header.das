options gen2
options indenting = 4

module c_api_header shared private

require daslib/ast_boost
require daslib/match
require daslib/rtti
require daslib/fio
require daslib/strings_boost
require strings

//! Emits the C header for `daslang -lib`, and decides which functions cross the C boundary.
//!
//! Selection reads ONE bit - `Function.flags.exports` - which `[export_c]` sets directly and
//! `policies.export_public_functions` sets for every public entry-module function. Whether a
//! signature has a C representation is decided here, after infer: `collect_c_exports` reports an
//! unrepresentable `[export_c]` signature through `macro_error`, and skips a merely-public one
//! with a warning. `build_c_header` then renders the accepted set. The LLVM side (llvm_lib.das)
//! emits one thunk per `CExport` and never re-derives any of this.

let C_KEYWORDS <- {"restrict", "_Bool", "_Complex", "_Imaginary", "_Alignas", "_Alignof",
    "_Atomic", "_Generic", "_Noreturn", "_Static_assert", "_Thread_local", "typeof"}


def public c_ident(name : string) : string {
    //! `name` reduced to a C identifier: anything outside [A-Za-z0-9_] becomes `_`, a leading digit
    //! gains a `_` prefix, and a C or C++ keyword gains a `_` suffix. Empty in, empty out.
    if (name |> empty()) {
        return ""
    }
    var out = build_string() $(writer) {
        name |> peek_data() $(bytes) {
            for (b in bytes) {
                let ch = int(b)
                let keep = (is_alpha(ch) || is_number(ch) || ch == '_')
                write_char(writer, keep ? ch : '_')
            }
        }
    }
    if (is_number(first_character(out))) {
        out = "_{out}"
    }
    if (is_cpp_keyword(out) || (C_KEYWORDS |> key_exists(out))) {
        out = "{out}_"
    }
    return out
}


def public lib_prefix_from_path(input : string) : string {
    //! The library's C symbol prefix: the input file's stem, reduced to a C identifier.
    let stem = input |> base_name() |> split(".")
    return c_ident(stem |> empty() ? "" : stem[0])
}


def public export_c_name_override(var fn : Function?) : string {
    //! The `name=` argument of this function's `[export_c]`, or "" when there is none.
    for (ann in fn.annotations) {
        if (ann.annotation.name != "export_c") {
            continue
        }
        for (arg in ann.arguments) {
            if (arg.name == "name" && arg.basicType == Type.tString) {
                return string(arg.sValue)
            }
        }
    }
    return ""
}


def public has_export_c_annotation(var fn : Function?) : bool {
    //! True when the function carries `[export_c]`.
    for (ann in fn.annotations) {
        if (ann.annotation.name == "export_c") {
            return true
        }
    }
    return false
}


def public c_export_symbol(prefix : string; var fn : Function?; name_override : string) : string {
    //! The exported C symbol for one function: `<prefix>_<name>`, where the name is the `[export_c]`
    //! override when given, else the das name (a generic instance reports its origin's name).
    if (!(name_override |> empty())) {
        return "{prefix}_{name_override}"
    }
    let das_name = string(fn.origin != null ? fn.origin.name : fn.name)
    return "{prefix}_{c_ident(das_name)}"
}


struct public CType {
    //! One das type's C spelling. `ok` false means the type has no C representation.
    //! `by_pointer` types cross the boundary as `T *`, never by value.
    ok : bool
    spelling : string
    by_pointer : bool
}

struct CVector {
    ok : bool
    name : string
    elem : string
    lanes : int
    span : bool
}

let VEC_LANE_NAMES <- ["x", "y", "z", "w"]
let SPAN_LANE_NAMES <- ["from", "to"]


def private vector_of(t : Type) : CVector {
    //! The C shape of a das vector or range type. `span` picks the from/to field names over x/y/z/w.
    match (t) {
        if (Type.tFloat2) { return CVector(ok = true, name = "float2", elem = "float", lanes = 2) }
        if (Type.tFloat3) { return CVector(ok = true, name = "float3", elem = "float", lanes = 3) }
        if (Type.tFloat4) { return CVector(ok = true, name = "float4", elem = "float", lanes = 4) }
        if (Type.tInt2) { return CVector(ok = true, name = "int2", elem = "int32_t", lanes = 2) }
        if (Type.tInt3) { return CVector(ok = true, name = "int3", elem = "int32_t", lanes = 3) }
        if (Type.tInt4) { return CVector(ok = true, name = "int4", elem = "int32_t", lanes = 4) }
        if (Type.tUInt2) { return CVector(ok = true, name = "uint2", elem = "uint32_t", lanes = 2) }
        if (Type.tUInt3) { return CVector(ok = true, name = "uint3", elem = "uint32_t", lanes = 3) }
        if (Type.tUInt4) { return CVector(ok = true, name = "uint4", elem = "uint32_t", lanes = 4) }
        if (Type.tRange) { return CVector(ok = true, name = "range", elem = "int32_t", lanes = 2, span = true) }
        if (Type.tURange) { return CVector(ok = true, name = "urange", elem = "uint32_t", lanes = 2, span = true) }
        if (Type.tRange64) { return CVector(ok = true, name = "range64", elem = "int64_t", lanes = 2, span = true) }
        if (Type.tURange64) { return CVector(ok = true, name = "urange64", elem = "uint64_t", lanes = 2, span = true) }
    }
    return CVector(ok = false)
}


def private vector_field_names(v : CVector) : array<string> {
    return [for (i in range(v.lanes)); v.span ? SPAN_LANE_NAMES[i] : VEC_LANE_NAMES[i]]
}


def private scalar_of(t : Type) : string {
    //! The C spelling of a das scalar that crosses the boundary by value, or "" for anything else.
    match (t) {
        if (Type.tBool) { return "bool" }
        if (Type.tInt8) { return "int8_t" }
        if (Type.tUInt8) { return "uint8_t" }
        if (Type.tInt16) { return "int16_t" }
        if (Type.tUInt16) { return "uint16_t" }
        if (Type.tInt) { return "int32_t" }
        if (Type.tUInt) { return "uint32_t" }
        if (Type.tInt64) { return "int64_t" }
        if (Type.tUInt64) { return "uint64_t" }
        if (Type.tFloat) { return "float" }
        if (Type.tDouble) { return "double" }
        if (Type.tBitfield) { return "uint32_t" }
        if (Type.tBitfield8) { return "uint8_t" }
        if (Type.tBitfield16) { return "uint16_t" }
        if (Type.tBitfield64) { return "uint64_t" }
    }
    return ""
}


def private enum_base_of(t : Type) : string {
    //! The C integer a das enumeration's values are spelled as.
    let s = scalar_of(t)
    return s |> empty() ? "int32_t" : s
}


struct public CNames {
    //! What the C spellings are built from: the library prefix and the entry module (whose types
    //! are named `<prefix>_<Type>`; a required module's are `<prefix>_<module>_<Type>`).
    prefix : string
    @do_not_delete this_module : Module?
}


def private c_type_name(names : CNames; var owner : Module?; name : das_string) : string {
    if (owner == null || owner == names.this_module || owner.name |> empty()) {
        return "{names.prefix}_{c_ident(string(name))}"
    }
    return "{names.prefix}_{c_ident(string(owner.name))}_{c_ident(string(name))}"
}


def private peel(var t : TypeDeclPtr) : TypeDeclPtr {
    //! A distinct type is ABI-identical to what it wraps - the C boundary sees through it, the same
    //! way `type_to_llvm_abi_type` does.
    if (t.baseType == Type.tDistinct && t.firstType != null) {
        return peel(t.firstType)
    }
    return t
}


def private is_pod_struct(var st : Structure?) : bool {
    return st != null && !st.flags.isClass && !(st.fields |> empty())
}


def private pointee_ok(var t : TypeDeclPtr) : bool {
    //! A pointer's target only has to be NAMEABLE in C - its fields are not walked here, so a
    //! self-referential struct terminates and still gets its forward declaration.
    if (t == null) {
        return true
    }
    var pt = peel(t)
    if (pt.isStructure) {
        return is_pod_struct(pt.structType)
    }
    if (pt.isEnumT) {
        return pt.enumType != null && !pt.enumType.external
    }
    return !(scalar_of(pt.baseType) |> empty()) || vector_of(pt.baseType).ok || pt.isVoid
}


def private pointer_c_type(var t : TypeDeclPtr; names : CNames) : CType {
    var target = t.firstType
    if (target == null || target.isVoid || !pointee_ok(target)) {
        return CType(ok = true, spelling = "void *")
    }
    var pt = peel(target)
    var inner = ""
    if (pt.isStructure) {
        inner = c_type_name(names, pt.structType._module, pt.structType.name)
    } elif (pt.isEnumT) {
        inner = c_type_name(names, pt.enumType._module, pt.enumType.name)
    } else {
        let v = vector_of(pt.baseType)
        inner = v.ok ? "{names.prefix}_{v.name}" : scalar_of(pt.baseType)
    }
    let qual = pt.flags.constant ? "const " : ""
    return CType(ok = true, spelling = "{qual}{inner} *")
}


def private field_c_type(var t : TypeDeclPtr; names : CNames; var visiting : table<string>) : CType {
    //! A struct field's C type. A fixed_array field stays an array (`T name[N]`), so its dimension
    //! is spelled by the caller; everything else answers as it would at the boundary.
    var ft = peel(t)
    if (ft.baseType == Type.tFixedArray) {
        let inner = ft.firstType == null ? CType(ok = false) : field_c_type(ft.firstType, names, visiting)
        return inner.ok && !inner.by_pointer ? CType(ok = true, spelling = inner.spelling) : CType(ok = false)
    }
    if (ft.isStructure) {
        if (!struct_c_ok(ft.structType, names, visiting)) {
            return CType(ok = false)
        }
        return CType(ok = true, spelling = c_type_name(names, ft.structType._module, ft.structType.name))
    }
    return value_c_type(ft, names)
}


def private struct_c_ok(var st : Structure?; names : CNames; var visiting : table<string>) : bool {
    //! Whether this structure crosses the C boundary: a plain struct, non-empty, every field
    //! representable. A pointer field only needs a nameable target, which is what breaks cycles.
    if (!is_pod_struct(st)) {
        return false
    }
    let key = "{st._module.name}::{st.name}"
    if (visiting |> key_exists(key)) {
        return true
    }
    visiting |> insert(key)
    var ok = true
    for (fld in st.fields) {
        if (!field_c_type(fld._type, names, visiting).ok) {
            ok = false
            break
        }
    }
    visiting |> erase(key)
    return ok
}


def private value_c_type(var t : TypeDeclPtr; names : CNames) : CType {
    //! The C type of a das type that is NOT a structure and NOT a fixed array.
    if (t.isPointer) {
        return pointer_c_type(t, names)
    }
    if (t.isString) {
        return CType(ok = true, spelling = "const char *")
    }
    if (t.isEnumT) {
        if (t.enumType == null || t.enumType.external) {
            return CType(ok = false)
        }
        return CType(ok = true, spelling = c_type_name(names, t.enumType._module, t.enumType.name))
    }
    let v = vector_of(t.baseType)
    if (v.ok) {
        return CType(ok = true, spelling = "{names.prefix}_{v.name}", by_pointer = true)
    }
    let sc = scalar_of(t.baseType)
    return sc |> empty() ? CType(ok = false) : CType(ok = true, spelling = sc)
}


def public c_type_of(var t : TypeDeclPtr; names : CNames) : CType {
    //! The C type one das type takes at the boundary. Structures and the vector/range families come
    //! back `by_pointer`; scalars, `string`, pointers and enums come back by value.
    if (t == null) {
        return CType(ok = false)
    }
    var pt = peel(t)
    if (pt.isVoid) {
        return CType(ok = true, spelling = "void")
    }
    if (pt.isStructure) {
        var visiting : table<string>
        if (!struct_c_ok(pt.structType, names, visiting)) {
            return CType(ok = false)
        }
        return CType(ok = true, spelling = c_type_name(names, pt.structType._module, pt.structType.name), by_pointer = true)
    }
    if (pt.baseType == Type.tFixedArray) {
        var visiting : table<string>
        let inner = pt.firstType == null ? CType(ok = false) : field_c_type(pt.firstType, names, visiting)
        return inner.ok ? CType(ok = true, spelling = inner.spelling, by_pointer = true) : CType(ok = false)
    }
    return value_c_type(pt, names)
}


struct public CParam {
    //! One C parameter. `by_pointer` params are spelled `[const ]c_type *`; the rest by value.
    //! `pointer_in_impl` is true when the das ABI already passes a pointer, so the thunk forwards
    //! the C pointer unchanged instead of loading through it.
    name : string
    c_type : string
    by_pointer : bool
    is_const : bool
    pointer_in_impl : bool
}


struct public CResult {
    //! The C result. `via_out` means the C function returns void and writes through a trailing
    //! `c_type * out`; `cmres` means the das impl fills that pointer itself.
    c_type : string
    via_out : bool
    cmres : bool
}


struct public CExport {
    //! One function crossing the C boundary. `c_name` is the full exported symbol - the LLVM side
    //! names its thunk exactly this.
    @do_not_delete fn : Function?
    c_name : string
    das_signature : string
    params : array<CParam>
    result : CResult
}


struct CReject {
    //! Why one function cannot cross: `what` names the parameter or the result, `why` its das type.
    ok : bool
    what : string
    why : string
}


def private param_name_of(raw : string; index : int) : string {
    let n = c_ident(raw)
    if (n |> empty()) {
        return "a{index}"
    }
    return n == "ctx" || n == "out" ? "{n}_" : n
}


def private describe_params(var fn : Function?; names : CNames; var out : array<CParam>) : CReject {
    for (arg, i in fn.arguments, count()) {
        let ct = c_type_of(arg._type, names)
        if (!ct.ok || arg._type.isVoid) {
            return CReject(what = "parameter `{arg.name}`", why = arg._type.describe())
        }
        let by_ptr = ct.by_pointer || arg._type.flags.ref
        out |> emplace(CParam(name = param_name_of(string(arg.name), i),
            c_type = ct.spelling,
            by_pointer = by_ptr,
            is_const = by_ptr && arg._type.flags.constant,
            pointer_in_impl = arg._type.isRef))
    }
    return CReject(ok = true)
}


def public describe_c_signature(var fn : Function?; names : CNames; c_name : string) : tuple<exp : CExport; reject : CReject> {
    //! Describes one function at the C boundary, or reports the first type that has no C spelling.
    var params : array<CParam>
    let bad = describe_params(fn, names, params)
    if (!bad.ok) {
        return (exp = CExport(), reject = bad)
    }
    let rt = c_type_of(fn.result, names)
    // A fixed_array result would be a CMRES of an array; C takes it as a pointer nothing sizes.
    if (!rt.ok || peel(fn.result).baseType == Type.tFixedArray) {
        return (exp = CExport(), reject = CReject(what = "result", why = fn.result.describe()))
    }
    let res = CResult(c_type = rt.spelling, via_out = rt.by_pointer, cmres = isCMRES_result(fn))
    return (exp = CExport(fn = fn, c_name = c_name, das_signature = das_signature_of(fn),
        params <- params, result = res), reject = CReject(ok = true))
}


def private isCMRES_result(var fn : Function?) : bool {
    return fn.flags.copyOnReturn || fn.flags.moveOnReturn
}


def private das_signature_of(var fn : Function?) : string {
    //! The das declaration, for the comment above each C prototype. The implicit ` const` every
    //! by-value argument carries is dropped; a `var` argument keeps its `-const` marker.
    let args = [for (a in fn.arguments); "{a.name} : {a._type.describe() |> replace(" const", "")}"]
    return "def {fn.name}({args |> join("; ")}) : {fn.result.describe() |> replace(" const", "")}"
}


struct CTypeSet {
    //! The types the accepted signatures reach, in emission order: enums, then vectors, then
    //! structures post-order (a by-value field's structure lands before the structure holding it).
    enums : array<Enumeration?>
    vectors : array<Type>
    structs : array<Structure?>
    seen : table<string>
}


def private collect_type(var t : TypeDeclPtr; names : CNames; var set : CTypeSet) {
    if (t == null) {
        return
    }
    var pt = peel(t)
    if (pt.baseType == Type.tFixedArray || pt.isPointer) {
        collect_type(pt.firstType, names, set)
        return
    }
    if (pt.isEnumT && pt.enumType != null && !pt.enumType.external) {
        let key = "e:{pt.enumType._module.name}::{pt.enumType.name}"
        if (!(set.seen |> key_exists(key))) {
            set.seen |> insert(key)
            set.enums |> push(pt.enumType)
        }
        return
    }
    let v = vector_of(pt.baseType)
    if (v.ok) {
        let key = "v:{v.name}"
        if (!(set.seen |> key_exists(key))) {
            set.seen |> insert(key)
            set.vectors |> push(pt.baseType)
        }
        return
    }
    if (pt.isStructure) {
        collect_struct(pt.structType, names, set)
    }
}


def private collect_struct(var st : Structure?; names : CNames; var set : CTypeSet) {
    if (st == null) {
        return
    }
    let key = "s:{st._module.name}::{st.name}"
    if (set.seen |> key_exists(key)) {
        return
    }
    set.seen |> insert(key)
    for (fld in st.fields) {
        collect_type(fld._type, names, set)
    }
    set.structs |> push(st)
}


def public collect_c_types(var exports : array<CExport>; names : CNames) : CTypeSet {
    //! Every type the exported signatures reach, transitively, each once.
    var set : CTypeSet
    for (e in exports) {
        for (arg in e.fn.arguments) {
            collect_type(arg._type, names, set)
        }
        collect_type(e.fn.result, names, set)
    }
    return <- set
}


def private enum_value_of(var value : ExpressionPtr) : string {
    //! One enumeration entry's value, read off the folded constant the entry carries.
    if (value == null) {
        return "0"
    }
    if (value is ExprConstInt8) { return "{(value as ExprConstInt8).value}" }
    if (value is ExprConstUInt8) { return "{(value as ExprConstUInt8).value}u" }
    if (value is ExprConstInt16) { return "{(value as ExprConstInt16).value}" }
    if (value is ExprConstUInt16) { return "{(value as ExprConstUInt16).value}u" }
    if (value is ExprConstInt) { return "{(value as ExprConstInt).value}" }
    if (value is ExprConstUInt) { return "{(value as ExprConstUInt).value}u" }
    if (value is ExprConstInt64) { return "INT64_C({(value as ExprConstInt64).value})" }
    if (value is ExprConstUInt64) { return "UINT64_C({(value as ExprConstUInt64).value})" }
    return "0"
}


def private write_enum(var writer : StringBuilderWriter; var en : Enumeration?; names : CNames) {
    let cname = c_type_name(names, en._module, en.name)
    write(writer, "\n// das: enum {en.name}\ntypedef {enum_base_of(en.baseType)} {cname};\n")
    write(writer, "enum \{\n")
    let entries = [for (ee in en.list); "    {cname}_{c_ident(string(ee.name))} = {enum_value_of(ee.value)}"]
    write(writer, "{entries |> join(",\n")}\n\};\n")
}


def private write_vector(var writer : StringBuilderWriter; t : Type; names : CNames) {
    let v = vector_of(t)
    let cname = "{names.prefix}_{v.name}"
    let fields = vector_field_names(v) |> join(", ")
    write(writer, "\n// das: {v.name}\ntypedef struct \{ {v.elem} {fields}; \} {cname};\n")
    write(writer, "{names.prefix |> to_upper()}_STATIC_ASSERT(sizeof({cname}) == {v.elem == "int64_t" || v.elem == "uint64_t" ? v.lanes * 8 : v.lanes * 4}, \"{cname}: size differs from the daslang layout\");\n")
}


def private field_decl(var fld : FieldDeclaration; names : CNames) : string {
    var visiting : table<string>
    var ft = peel(fld._type)
    var dims : array<string>
    while (ft.baseType == Type.tFixedArray && ft.firstType != null) {
        dims |> push("[{ft.fixedDim}]")
        ft = peel(ft.firstType)
    }
    let ct = field_c_type(fld._type, names, visiting)
    return "    {ct.spelling} {c_ident(string(fld.name))}{dims |> join("")};"
}


def private write_struct(var writer : StringBuilderWriter; var st : Structure?; names : CNames) {
    let cname = c_type_name(names, st._module, st.name)
    let guard = names.prefix |> to_upper()
    write(writer, "\n// das: struct {st.name}\nstruct {cname} \{\n")
    for (fld in st.fields) {
        write(writer, "{field_decl(fld, names)}\n")
    }
    write(writer, "\};\n")
    write(writer, "{guard}_STATIC_ASSERT(sizeof({cname}) == {st.sizeOf}, \"{cname}: size differs from the daslang layout\");\n")
    for (fld in st.fields) {
        write(writer, "{guard}_STATIC_ASSERT(offsetof({cname}, {c_ident(string(fld.name))}) == {fld.offset}, \"{cname}.{fld.name}: offset differs from the daslang layout\");\n")
    }
}


def private param_decl(p : CParam) : string {
    if (!p.by_pointer) {
        return "{p.c_type} {p.name}"
    }
    return "{p.is_const ? "const " : ""}{p.c_type} * {p.name}"
}


def public c_declaration(e : CExport; names : CNames) : string {
    //! The C prototype for one export, without the trailing semicolon.
    var args = ["{names.prefix}_ctx * ctx"]
    args |> reserve(length(e.params) + 2)
    for (p in e.params) {
        args |> push(param_decl(p))
    }
    if (e.result.via_out) {
        args |> push("{e.result.c_type} * out")
    }
    let ret = e.result.via_out ? "void" : e.result.c_type
    return "{ret} {e.c_name}({args |> join(", ")})"
}


def private write_prologue(var writer : StringBuilderWriter; names : CNames; input_path, output_path : string) {
    let guard = names.prefix |> to_upper()
    write(writer, "// Code generated by `daslang -lib {input_path} -output {output_path}`. DO NOT EDIT.\n")
    write(writer, "// Layouts are asserted for the daslang that generated this header; regenerate it for another target.\n")
    write(writer, "#pragma once\n\n#include <stdint.h>\n#include <stdbool.h>\n#include <stddef.h>\n\n")
    write(writer, "#if defined(__cplusplus)\n#define {guard}_STATIC_ASSERT(cond, msg) static_assert(cond, msg)\n")
    write(writer, "#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L\n")
    write(writer, "#define {guard}_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)\n")
    write(writer, "#else\n#define {guard}_STATIC_ASSERT(cond, msg)\n#endif\n\n")
    write(writer, "#ifdef __cplusplus\nextern \"C\" \{\n#endif\n")
}


def private write_fixed_api(var writer : StringBuilderWriter; names : CNames) {
    let p = names.prefix
    write(writer, "\n// One library instance and everything it owns: globals, heap, string heap. Drive one\n")
    write(writer, "// instance from one thread at a time, or make one per thread.\n")
    write(writer, "typedef struct {p}_ctx {p}_ctx;\n")
    write(writer, "\n// Creates an instance, then runs the script's global initializers and its [init] functions.\n")
    write(writer, "// Callable from any thread. Returns NULL when the initializers raised, or when this\n")
    write(writer, "// process already carries another daslang runtime - only one daslang library per process,\n")
    write(writer, "// and a host that registers the daslang modules itself counts as one. {p}_last_error(NULL)\n")
    write(writer, "// reports which.\n")
    write(writer, "{p}_ctx * {p}_create(void);\n")
    write(writer, "\n// Runs the script's [finalize] functions, then frees the instance and everything it\n")
    write(writer, "// allocated - every string and every pointer it returned included.\n")
    write(writer, "void {p}_destroy({p}_ctx * ctx);\n")
    write(writer, "\n// NULL when the last call on `ctx` completed normally; otherwise that call's exception text,\n")
    write(writer, "// owned by the instance and valid until the next call on it. Pass NULL to read a failed\n")
    write(writer, "// {p}_create. On a failed call the returned value is 0/false/NULL and `out` is left untouched.\n")
    write(writer, "const char * {p}_last_error({p}_ctx * ctx);\n")
    write(writer, "\n// Drains the daslang runtime for the whole process. Optional, and final: no instance of any\n")
    write(writer, "// daslang library may be created or called afterwards.\n")
    write(writer, "void {p}_shutdown_runtime(void);\n")
    write(writer, "\n// A `const char *` an exported function returns points into the instance's string heap, and a\n")
    write(writer, "// returned `T *` into its object heap: copy what you need past the next call on that instance,\n")
    write(writer, "// because a later call may collect. The empty daslang string comes back NULL. A pointer you\n")
    write(writer, "// pass in, `const char *` included, is borrowed for the duration of the call.\n")
}


def public build_c_header(var exports : array<CExport>; names : CNames; input_path, output_path, link_note : string) : string {
    //! The whole header text. Pure - nothing is written and nothing is validated here; the exports
    //! reaching this point have already been accepted by `collect_c_exports`.
    var types <- collect_c_types(exports, names)
    return build_string() $(var writer) {
        write_prologue(writer, names, input_path, output_path)
        write_fixed_api(writer, names)
        for (en in types.enums) {
            write_enum(writer, en, names)
        }
        for (vt in types.vectors) {
            write_vector(writer, vt, names)
        }
        if (!(types.structs |> empty())) {
            write(writer, "\n")
            for (st in types.structs) {
                let cname = c_type_name(names, st._module, st.name)
                write(writer, "typedef struct {cname} {cname};\n")
            }
        }
        for (st in types.structs) {
            write_struct(writer, st, names)
        }
        write(writer, "\n")
        for (e in exports) {
            write(writer, "\n// {e.das_signature}\n{c_declaration(e, names)};\n")
        }
        if (!(link_note |> empty())) {
            write(writer, "\n// Linking:\n")
            for (line in link_note |> split("\n")) {
                write(writer, "// {line}\n")
            }
        }
        write(writer, "\n#ifdef __cplusplus\n\} // extern \"C\"\n#endif\n")
    }
}


def public emit_c_header(var exports : array<CExport>; names : CNames; input_path, output_path, link_note : string) : bool {
    //! Writes the header beside the library, as `<output_path>.h`.
    let text = build_c_header(exports, names, input_path, output_path, link_note)
    let path = "{output_path}.h"
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            f |> fwrite(text)
            ok = true
        }
    }
    if (!ok) {
        to_log(LOG_ERROR, "daslang -lib: can't write the C header {path}\n")
    }
    return ok
}


let REPRESENTABLE_HINT = "allowed: bool, int8..uint64, int, uint, float, double, string, pointers, enums, POD structs, float2..uint4, range/urange/range64/urange64, fixed_array arguments"


struct public CRejection {
    //! One function that carries `[export_c]` and cannot cross the boundary. `message` already
    //! names the site, so a caller that has no LineInfo to hand can report it as it stands.
    fn_name : string
    message : string
}


def public is_c_export_candidate(var fn : Function?; prog : Program?; export_all : bool) : bool {
    //! Whether this function is offered to the C boundary at all. `exports` is the one selection
    //! bit: `[export_c]` sets it, and `policies.export_public_functions` sets it for every public
    //! entry-module function.
    if (fn._module != prog.getThisModule || !fn.flags.exports
            || fn.flags.builtIn || fn.flags.generated || fn.moreFlags.isTemplate || fn.fromGeneric != null
            || fn.flags.init || fn.flags.shutdown || fn.flags.macroInit || fn.moreFlags.macroFunction) {
        return false
    }
    return export_all || has_export_c_annotation(fn)
}


def private reserved_c_names(names : CNames) : array<string> {
    let p = names.prefix
    return ["{p}_create", "{p}_destroy", "{p}_last_error", "{p}_shutdown_runtime", "{p}_ctx"]
}


def public collect_c_exports(prog : Program?; names : CNames; export_all : bool) : tuple<exports : array<CExport>; errors : array<CRejection>> {
    //! Selects and describes every function crossing the C boundary. An unrepresentable or clashing
    //! `[export_c]` comes back in `errors` for the caller to report; a merely-public one is skipped
    //! here with a warning naming the function and the offending type.
    var accepted : array<CExport>
    var errors : array<CRejection>
    var claimed <- {for (r in reserved_c_names(names)); r => "the fixed library API"}
    var this_mod = prog.getThisModule
    this_mod |> for_each_function("") $(var fn) {
        if (!is_c_export_candidate(fn, prog, export_all)) {
            return
        }
        let is_explicit = has_export_c_annotation(fn)
        let c_name = c_export_symbol(names.prefix, fn, export_c_name_override(fn))
        var why = ""
        let owner = claimed?[c_name] ?? ""
        var sig <- describe_c_signature(fn, names, c_name)
        if (!(owner |> empty())) {
            why = "C symbol `{c_name}` is already taken by {owner}; C has no overloading - give one of them [export_c(name = \"...\")]"
        } elif (!sig.reject.ok) {
            why = "{sig.reject.what} has type {sig.reject.why}, which has no C representation ({REPRESENTABLE_HINT})"
        }
        if (!(why |> empty())) {
            if (is_explicit) {
                errors |> emplace(CRejection(fn_name = string(fn.name),
                message = "{describe(fn.at)}: [export_c] {fn.name}: {why}"))
            } else {
                to_log(LOG_WARNING, "{describe(fn.at)}: daslang -lib: skipping {fn.name}: {why}\n")
            }
            return
        }
        claimed[c_name] = "{fn.name} at {describe(fn.at)}"
        accepted |> emplace(sig.exp)
    }
    return (exports <- accepted, errors <- errors)
}
