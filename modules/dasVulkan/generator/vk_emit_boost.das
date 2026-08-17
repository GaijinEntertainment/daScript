options gen2
options _comment_hygiene = true
options indenting = 4
options persistent_heap = true
options _cyclomatic_complexity = 0   // codegen emitter: per-field-kind arms are the domain shape
options _function_length = 0

module vk_emit_boost

require vk_model
require vk_naming
require vk_emit
require daslib/fio
require strings
require daslib/strings_boost

// OB / CB / GEN_HEADER are re-exported from vk_emit

//! C scalar type -> daslang scalar type (for boost field declarations)
let DAS_SCALAR_MAP : table<string; string> <- {
    "int8_t" => "int8", "uint8_t" => "uint8", "int16_t" => "int16", "uint16_t" => "uint16",
    "int32_t" => "int", "uint32_t" => "uint", "int" => "int",
    "int64_t" => "int64", "uint64_t" => "uint64",
    "float" => "float", "double" => "double", "size_t" => "uint64", "char" => "int8"
}

// ===== ctors: sType-filling constructors over the RAW Vk structs =====
// daslang has no #if, so the boost can only reference types registered on every
// platform -- emit only for non-platform-guarded structs (protect == "").

def private emit_ctors(reg : VkRegistry; em : EmitModel; out_dir : string) {
    let stype_prefix = enum_value_prefix("VkStructureType", false, reg.vendor_tags)
    var count = 0
    fopen("{out_dir}/vulkan_ctors.das", "wb") $(f) {
        panic("can't write vulkan_ctors.das") if (f == null)
        fprint(f, GEN_HEADER)
        fprint(f, "options gen2\noptions _comment_hygiene = true\noptions indenting = 4\n\nmodule vulkan_ctors shared\n\nrequire vulkan public\n\n")
        for (s in reg.structs) {
            continue if (!empty(s.alias) || !key_exists(em.struct_emit, s.name) || !empty(s.protect))
            // find the sType member (always named "sType" with a values= attribute)
            var stype_const = ""
            for (m in s.members) {
                if (m.name == "sType" && !empty(m.stype_value)) {
                    stype_const = m.stype_value
                    break
                }
            }
            continue if (empty(stype_const))
            let short = enum_short_name(stype_prefix, stype_const)
            fprint(f, "//! construct a {s.name} with sType pre-filled\n")
            fprint(f, "def {s.name}() : {s.name} {OB}\n")
            fprint(f, "    var self : {s.name}\n")
            fprint(f, "    self.sType = VkStructureType.{short}\n")
            fprint(f, "    return self\n")
            fprint(f, "{CB}\n\n")
            count++
        }
    }
    to_log(LOG_INFO, "boost: {count} sType constructors -> vulkan_ctors.das\n")
}

//! true iff emit_ctors emits an sType-prefilling ctor for this raw struct.
//! An out struct/array a get/enumerate command fills must have its sType set
//! before the call, so where such a ctor exists the wrapper builds with it.
def private has_stype_ctor(reg : VkRegistry; em : EmitModel; name : string) : bool {
    for (s in reg.structs) {
        continue if (s.name != name)
        if (!empty(s.alias) || !key_exists(em.struct_emit, s.name) || !empty(s.protect)) {
            return false
        }
        for (m in s.members) {
            if (m.name == "sType" && !empty(m.stype_value)) {
                return true
            }
        }
        return false
    }
    return false
}

// ===== ownership: derive create/destroy pairs, emit wrapper structs =====

//! the idiomatic boost name for a handle: strip the leading "Vk"
def public boost_name(vk_handle : string) : string {
    if (vk_handle |> starts_with("Vk")) {
        return vk_handle |> slice(2)
    }
    return vk_handle
}

struct ParentField {
    field : string      //! "_device"
    vktype : string     //! "VkDevice"
}

struct OwnerInfo {
    handle : string         //! VkImage
    boost : string          //! Image
    destroyer : string      //! vkDestroyImage / vkFreeMemory  ("" = no destroyer)
    parents : array<ParentField>
    pool_owned : bool       //! freed by a pool (cmd buffers / descriptor sets) -> finalize no-op
}

def private is_boost_handle(em : EmitModel; tn : string) : bool {
    return key_exists(em.handle_names, tn)
}

//! a command is a destroyer if it is named vkDestroy*/vkFree* and takes a handle param
def private find_destroyers(reg : VkRegistry; em : EmitModel) : table<string; int> {
    var result : table<string; int>    // handle name -> command index
    for (c, idx in reg.commands, range(length(reg.commands))) {
        continue if (!empty(c.alias) || !key_exists(reg.enabled_commands, c.name) || !empty(c.protect))
        let is_destroy = c.name |> starts_with("vkDestroy")
        let is_free = c.name |> starts_with("vkFree")
        continue if (!is_destroy && !is_free)
        // the destroyed handle is the (usually last) handle-typed param
        for (p in c.params) {
            if (is_boost_handle(em, p.type_name) && !key_exists(result, p.type_name)) {
                result[p.type_name] = idx
            }
        }
    }
    return <- result
}

def public derive_owners(reg : VkRegistry; em : EmitModel) : array<OwnerInfo> {
    let destroyers = find_destroyers(reg, em)
    var owners : array<OwnerInfo>
    for (h in reg.handles) {
        continue if (!empty(h.alias) || !is_boost_handle(em, h.name) || !empty(h.protect))
        var oi = OwnerInfo(handle = h.name, boost = boost_name(h.name))
        let didx = destroyers?[h.name] ?? -1
        if (didx >= 0) {
            let dc & = unsafe(reg.commands[didx])
            oi.destroyer = dc.name
            // parents = the np==0 handle params that appear before the destroyed handle
            for (p in dc.params) {
                if (p.type_name == h.name) {
                    break
                }
                if (is_boost_handle(em, p.type_name) && p.num_ptrs == 0) {
                    oi.parents |> emplace(ParentField(field = "_{das_safe_name(p.name)}", vktype = p.type_name))
                }
            }
            // array-free (the destroyed handle param is a pointer) => pool-owned, finalize is a no-op
            for (p in dc.params) {
                if (p.type_name == h.name && p.num_ptrs >= 1) {
                    oi.pool_owned = true
                }
            }
        }
        owners |> emplace(oi)
    }
    return <- owners
}

[unused_argument(reg, em)]
def private emit_handles_boost(reg : VkRegistry; em : EmitModel; out_dir : string; owners : array<OwnerInfo>) {
    fopen("{out_dir}/vulkan_handles.das", "wb") $(f) {
        panic("can't write vulkan_handles.das") if (f == null)
        fprint(f, GEN_HEADER)
        fprint(f, "options gen2\noptions _comment_hygiene = true\noptions indenting = 4\n\nmodule vulkan_handles shared\n\n")
        fprint(f, "require vulkan public\nrequire vulkan/vulkan_runtime public\n\n")
        for (oi in owners) {
            // handles stored as uint64 (their ABI form) so wrapper copies are friction-free
            fprint(f, "struct {oi.boost} \{\n")
            fprint(f, "    _vk : uint64\n")
            fprint(f, "    _needs_delete : bool\n")
            for (pf in oi.parents) {
                fprint(f, "    {pf.field} : uint64\n")
            }
            fprint(f, "\}\n")
            // finalize
            fprint(f, "def finalize(var self : {oi.boost}) \{\n")
            if (!empty(oi.destroyer) && !oi.pool_owned) {
                fprint(f, "    if (self._needs_delete) \{\n")
                fprint(f, "        unsafe \{\n")
                fprint(f, "            {oi.destroyer}(")
                for (pf in oi.parents) {
                    fprint(f, "reinterpret<{pf.vktype}>(self.{pf.field}), ")
                }
                fprint(f, "reinterpret<{oi.handle}>(self._vk), null)\n")
                fprint(f, "        \}\n")
                fprint(f, "    \}\n")
            }
            fprint(f, "    self._needs_delete = false\n")
            fprint(f, "\}\n")
            // conversions
            fprint(f, "def boost_value_to_vk(self : {oi.boost}) : {oi.handle} \{ return unsafe(reinterpret<{oi.handle}>(self._vk)) \}\n")
            fprint(f, "def vk_value_to_boost(v : {oi.handle}) : {oi.boost} \{ var b : {oi.boost}; b._vk = unsafe(reinterpret<uint64>(v)); return b \}\n\n")
        }
    }
    to_log(LOG_INFO, "boost: {length(owners)} handle wrappers -> vulkan_handles.das\n")
}

// ===== vk_view: idiomatic boost structs with array<T> fields + transient C view =====

enum FieldKind {
    erase           //! sType / count member -- not a boost field; vk_view derives it
    next_ptr        //! pNext -> boost `next : void?`
    scalar          //! scalar / enum / flags / nested-POD / fixed-array -- direct copy
    boost_handle    //! handle by value -> boost handle type
    str             //! const char* -> string
    arr_scalar      //! count+ptr of scalar/enum -> array<T>
    arr_handle      //! count+ptr of handle -> array<BoostHandle>
    arr_struct      //! count+ptr of mappable struct -> array<BoostStruct> (recursive vk_view)
    opt_struct_ptr  //! optional single const ptr to mappable struct -> BoostStruct? (borrowed pointee, raw-viewed into scratch)
    count_independent //! count member whose arrays are ALL optional -> settable boost field
    hard            //! unmappable -> skip the whole struct
}

struct BoostField {
    kind : FieldKind
    name : string           //! das-safe boost field name
    raw_name : string       //! original C member name (the raw struct field)
    boost_type : string     //! the boost field's das type
    elem_vktype : string    //! for arrays: the raw element type (VkSemaphore / uint32_t)
    elem_boost : string     //! for arrays: the boost element type
    count_member : string   //! for arrays: the raw count member name
    is_handle_elem : bool
    is_optional : bool      //! for arrays: optional="true" (may be empty -> null pointer)
    arr_raw_ptr : string    //! for flags-arrays: raw field ptr type (uint?/uint64?), reinterpret target; "" = none
}

//! names of members that are the count for some array member (len=thatName)
def private count_members_of(s : VkStructDef) : table<string> {
    var counts : table<string>
    for (m in s.members) {
        continue if (empty(m.len))
        // simple identifier len that names another member
        for (other in s.members) {
            if (other.name == m.len) {
                counts |> insert(m.len)
            }
        }
    }
    return <- counts
}

def private elem_is_handle(em : EmitModel; tn : string) : bool {
    return key_exists(em.handle_names, tn)
}

//! resolve a C type name to a das scalar, chasing one base_types alias (VkDeviceSize -> uint64)
def private arr_scalar_elem(reg : VkRegistry; tn : string) : string {
    let direct = DAS_SCALAR_MAP?[tn] ?? ""
    if (!empty(direct)) {
        return direct
    }
    let bt = reg.base_types?[tn] ?? ""
    return DAS_SCALAR_MAP?[bt] ?? ""
}

//! pX (const T*) and ppX (const T* const*) are a mutually-exclusive union sharing one count.
//! Bind the single-pointer form as array<T> and erase the double; the single carries the count.
//! Matched by element type across depths, the double's len naming the single's first ("X,1" vs "X").
def private has_double_ptr_alt(s : VkStructDef; m : VkMember) : bool {
    if (m.num_ptrs != 1 || !m.is_const || empty(m.len)) {
        return false
    }
    for (o in s.members) {
        if (o.num_ptrs == 2 && o.is_const && o.type_name == m.type_name &&
                !empty(o.len) && o.len |> starts_with(m.len)) {
            return true
        }
    }
    return false
}

def private is_double_ptr_alt(s : VkStructDef; m : VkMember) : bool {
    if (m.num_ptrs != 2 || !m.is_const || empty(m.len)) {
        return false
    }
    for (o in s.members) {
        if (o.num_ptrs == 1 && o.is_const && o.type_name == m.type_name &&
                !empty(o.len) && m.len |> starts_with(o.len)) {
            return true
        }
    }
    return false
}

//! map a struct member to a boost field; FieldKind.hard signals "skip the whole struct".
//! `mappable` is the set of struct names known to have a boost vk_view (for array-of-struct).
//! a len member that denotes a SIZE IN BYTES rather than a count of elements
def private len_is_bytes(len : string) : bool {
    return len == "size" || len |> ends_with("Size")
}

def private classify_field(reg : VkRegistry; em : EmitModel; s : VkStructDef; m : VkMember; counts : table<string>; mappable : table<string>) : BoostField {
    var bf = BoostField(name = das_safe_name(m.name), raw_name = m.name)
    let tn = m.type_name
    if (m.name == "sType" && !empty(m.stype_value)) {
        bf.kind = FieldKind.erase
        return <- bf
    }
    if (m.name == "pNext") {
        // only plain `const void* pNext` becomes `next`; typed self-pointers are meta-structs -> skip
        if (tn == "void") {
            bf.kind = FieldKind.next_ptr
            bf.name = "next"
        } else {
            bf.kind = FieldKind.hard
        }
        return <- bf
    }
    if (key_exists(counts, m.name)) {
        bf.kind = FieldKind.erase
        bf.boost_type = scalar_field_type(reg, em, m)   // kept in case it is promoted to a settable count field
        return <- bf
    }
    // function pointers, fixed-arrays, multi-pointers -> hard
    if (key_exists(reg.func_pointers, tn)) {
        bf.kind = FieldKind.hard
        return <- bf
    }
    if (!empty(m.arr_dims)) {
        // das will not whole-array-copy a fixed array of pointers -> defer the whole struct to raw-only
        if (elem_is_handle(em, tn)) {
            bf.kind = FieldKind.hard
            return <- bf
        }
        // inline fixed array -> scalar: the raw struct holds the same array, so `vk.x = b.x` copies it
        bf.kind = FieldKind.scalar
        bf.boost_type = build_string() $(writer) {
            writer |> write(scalar_field_type(reg, em, m))
            for (d in m.arr_dims) {
                writer |> write("[{d}]")
            }
        }
        return <- bf
    }
    // arrays: a len that names a count member
    let hasvkcount = !empty(m.len) && key_exists(counts, m.len)
    // bytes only where the len MEANS bytes: a count-shaped one counts typed values whose stride comes from a sibling field, and non-const is written by the call
    if (hasvkcount && m.num_ptrs == 1 && m.is_const && tn == "void" && len_is_bytes(m.len)) {
        bf.kind = FieldKind.arr_scalar
        bf.count_member = m.len
        bf.boost_type = "array<uint8>"
        bf.elem_vktype = "uint8"
        bf.arr_raw_ptr = "void?"
        return <- bf
    }
    if (hasvkcount && m.num_ptrs == 1 && tn != "void" && tn != "char") {
        bf.count_member = m.len
        // "optional" here = this array's length is NOT authoritative; the count member is primary
        bf.is_optional = ((m.optional_attr == "true" || m.noautovalidity == "true") &&
            !has_double_ptr_alt(s, m))
        if (elem_is_handle(em, tn)) {
            bf.kind = FieldKind.arr_handle
            bf.elem_boost = boost_name(tn)
            bf.boost_type = "array<{bf.elem_boost}>"
            bf.elem_vktype = tn               // raw handle das type (e.g. VkSemaphore)
            bf.is_handle_elem = true
        } else {
            let ds = arr_scalar_elem(reg, tn)
            if (!empty(ds)) {
                bf.kind = FieldKind.arr_scalar
                bf.boost_type = "array<{ds}>"
                bf.elem_vktype = ds           // scratch holds the same das scalar
            } elif (key_exists(mappable, tn)) {
                bf.kind = FieldKind.arr_struct
                bf.elem_vktype = tn           // scratch holds the raw Vk struct
                bf.elem_boost = boost_name(tn)
                bf.boost_type = "array<{bf.elem_boost}>"
            } elif (!key_exists(em.struct_emit, tn)) {
                // array of enum / flags / scalar-typedef -> arr_scalar (element is a value type)
                let de = scalar_field_type(reg, em, m)
                bf.kind = FieldKind.arr_scalar
                bf.boost_type = "array<{de}>"
                bf.elem_vktype = de
                // a Flags-TYPEDEF raw ptr is uint?/uint64?, a FlagBits-ENUM is the bitfield itself
                let mfi = em.flags_lookup?[tn] ?? FlagsInfo()
                if (mfi.kind == FlagsKind.flags64) {
                    bf.arr_raw_ptr = "uint64?"
                } elif (mfi.kind == FlagsKind.flags32) {
                    bf.arr_raw_ptr = "uint?"
                }
            } else {
                bf.kind = FieldKind.hard      // array of non-mappable struct -> defer
            }
        }
        return <- bf
    }
    // string
    if (tn == "char" && m.num_ptrs == 1) {
        bf.kind = FieldKind.str
        bf.boost_type = "string"
        return <- bf
    }
    // optional const ptr to a mappable struct -> `T?`; vk_view clones the pointee into scratch, so the caller's local need only outlive the view call
    if (m.num_ptrs == 1 && m.is_const && empty(m.len) && key_exists(mappable, tn)) {
        bf.kind = FieldKind.opt_struct_ptr
        bf.elem_vktype = tn
        bf.elem_boost = boost_name(tn)
        bf.boost_type = "{bf.elem_boost}?"
        bf.is_optional = true
        return <- bf
    }
    // optional remaining pointer -> omit the boost field; a null raw slot is what optional="true" permits
    if (m.num_ptrs >= 1 && m.is_const && m.optional_attr == "true") {
        bf.kind = FieldKind.erase
        return <- bf
    }
    // pX/ppX union's double-ptr alternative -> erase; the single-ptr sibling carries the data
    if (is_double_ptr_alt(s, m)) {
        bf.kind = FieldKind.erase
        return <- bf
    }
    // any remaining pointer -> hard (required void*, scalar ptr, non-mappable ptr)
    if (m.num_ptrs >= 1) {
        bf.kind = FieldKind.hard
        return <- bf
    }
    // by-value handle
    if (elem_is_handle(em, tn)) {
        bf.kind = FieldKind.boost_handle
        bf.boost_type = boost_name(tn)
        return <- bf
    }
    // by-value scalar / enum / flags / nested POD struct / base type -> direct copy (raw type kept)
    bf.kind = FieldKind.scalar
    bf.boost_type = scalar_field_type(reg, em, m)
    return <- bf
}

//! the das type of a by-value scalar/enum/flags/nested field, as the boost struct should declare it
def private scalar_field_type(reg : VkRegistry; em : EmitModel; m : VkMember) : string {
    let tn = m.type_name
    let direct = DAS_SCALAR_MAP?[tn] ?? ""
    if (!empty(direct)) {
        return direct
    }
    let fi = em.flags_lookup?[tn] ?? FlagsInfo()
    if (fi.kind != FlagsKind.not_flags) {
        return fi.canonical          // the bitfield alias
    }
    let bt = reg.base_types?[tn] ?? ""
    let via_base = DAS_SCALAR_MAP?[bt] ?? ""
    if (!empty(via_base)) {
        return via_base
    }
    return tn                          // enum / nested struct / VkBool32-style typedef
}

struct ViewStruct {
    s_index : int
    boost : string
    fields : array<BoostField>
}

//! is every member of `s` mappable, given the current candidate set? (array-of-struct
//! members are mappable iff their element struct is itself in `mappable`)
def private all_fields_mappable(reg : VkRegistry; em : EmitModel; s : VkStructDef; mappable : table<string>) : bool {
    let counts = count_members_of(s)
    for (m in s.members) {
        let bf <- classify_field(reg, em, s, m, counts, mappable)
        return false if (bf.kind == FieldKind.hard)
    }
    return true
}

def private first_hard_reason(reg : VkRegistry; em : EmitModel; s : VkStructDef; mappable : table<string>) : string {
    let counts = count_members_of(s)
    for (m in s.members) {
        let bf <- classify_field(reg, em, s, m, counts, mappable)
        if (bf.kind == FieldKind.hard) {
            if (m.num_ptrs == 1 && m.is_const && m.type_name == "void" && !empty(m.len)) {
                return "{s.name}: member {m.name} len {m.len} counts typed values, stride from a sibling field"
            }
            return "{s.name}: member {m.name} ({m.type_name} x{m.num_ptrs}) not mappable"
        }
    }
    return "{s.name}: not mappable"
}

def public derive_views(reg : VkRegistry; em : EmitModel; var skip_log : array<string>) : array<ViewStruct> {
    // Phase 1: fixed-point on the mappable set. Seed with every candidate struct, then
    // drop any whose fields aren't all mappable (array-of-struct elements must themselves
    // be mappable) until stable -- handles nested views (e.g. RenderPassCreateInfo).
    var mappable : table<string>
    for (s in reg.structs) {
        continue if (!empty(s.alias) || !key_exists(em.struct_emit, s.name) || !empty(s.protect) || s.is_union)
        mappable |> insert(s.name)
    }
    var changed = true
    while (changed) {
        changed = false
        for (s in reg.structs) {
            continue if (!key_exists(mappable, s.name))
            if (!all_fields_mappable(reg, em, s, mappable)) {
                mappable |> erase(s.name)
                changed = true
            }
        }
    }
    // Phase 2: build the views; classification is now stable so no field is hard.
    var views : array<ViewStruct>
    for (s, idx in reg.structs, range(length(reg.structs))) {
        continue if (!key_exists(mappable, s.name))
        let counts = count_members_of(s)
        var vs = ViewStruct(s_index = idx, boost = boost_name(s.name))
        vs.fields |> reserve(length(s.members))
        for (m in s.members) {
            var bf <- classify_field(reg, em, s, m, counts, mappable)
            vs.fields |> emplace(bf)
        }
        promote_independent_counts(vs)
        views |> emplace(vs)
    }
    // log the structs that didn't make it (with the first offending field)
    for (s in reg.structs) {
        continue if (!empty(s.alias) || !key_exists(em.struct_emit, s.name) || !empty(s.protect) || s.is_union || key_exists(mappable, s.name))
        skip_log |> push(first_hard_reason(reg, em, s, mappable))
    }
    return <- views
}

def private is_array_kind(k : FieldKind) : bool {
    return k == FieldKind.arr_scalar || k == FieldKind.arr_handle || k == FieldKind.arr_struct
}

//! vk_view_create_unsafe writes `b` (its scratch fields) only for array / opt-struct-ptr fields;
//! a struct with none is read-only, so the create param drops `var` and the destroy is a no-op.
def private view_has_scratch(vs : ViewStruct) : bool {
    for (bf in vs.fields) {
        return true if (is_array_kind(bf.kind) || bf.kind == FieldKind.opt_struct_ptr)
    }
    return false
}

//! Promote count members whose referencing arrays are ALL optional to settable boost fields:
//! such a count has independent meaning when its array is empty (descriptorCount of a
//! non-sampler binding). Counts with a non-optional array stay erased (derived from its length).
def private promote_independent_counts(var vs : ViewStruct) {
    for (i in range(length(vs.fields))) {
        continue if (vs.fields[i].kind != FieldKind.erase)
        let cname = vs.fields[i].raw_name
        var refs = 0
        var all_opt = true
        for (af in vs.fields) {
            if (is_array_kind(af.kind) && af.count_member == cname) {
                refs ++
                all_opt = all_opt && af.is_optional
            }
        }
        if (refs > 0 && all_opt) {
            vs.fields[i].kind = FieldKind.count_independent
        }
    }
}

//! das-safe boost field names of the array fields counted by `raw_count`
def private count_referencing_arrays(vs : ViewStruct; raw_count : string) : array<string> {
    var out <- [for (af in vs.fields); af.name; where is_array_kind(af.kind) && af.count_member == raw_count]
    return <- out
}

//! the boost field name of the array that authoritatively sets a DERIVED count: the
//! first non-optional referencing array, else the first referencing array ("" if none)
def private count_authoritative_array(vs : ViewStruct; raw_count : string) : string {
    var first = ""
    for (af in vs.fields) {
        if (is_array_kind(af.kind) && af.count_member == raw_count) {
            if (empty(first)) {
                first = af.name
            }
            return af.name if (!af.is_optional)
        }
    }
    return first
}

//! one array field's length as `count_type`: the cast is the count member's own
//! type, since das promotes nothing.  A 64-bit count takes long_length -- widening
//! length's own 32-bit result gets past no limit it already hit (LINT017).
def private count_len_expr(count_type, arr : string) : string {
    let bits64 = count_type == "uint64" || count_type == "int64"
    return "{count_type}({bits64 ? "long_length" : "length"}(b.{arr}))"
}

//! `count_type` expression for the largest length among `arrs` (fallback for an
//! independent count)
def private len_max_expr(arrs : array<string>; count_type : string) : string {
    return "{count_type}(0)" if (empty(arrs))
    let extra = length(arrs) - 1
    return build_string() $(var w) {
        for (_i in range(extra)) {
            w |> write("max(")
        }
        w |> write(count_len_expr(count_type, arrs[0]))
        for (i in range(1, length(arrs))) {
            w |> write(", {count_len_expr(count_type, arrs[i])})")
        }
    }
}

// A pointer-of-pointers argument views the same way in every command that takes one, so
// the loop is a pair of generics here rather than a copy in each wrapper.
def private emit_jagged_view_helpers(f : file) {
    fprint(f, "// One view per element of a jagged argument, into raw storage the caller owns.  The\n")
    fprint(f, "// interior pointers are void?, never T?: a T? array has das free each one on delete,\n")
    fprint(f, "// and these point into vkraw2's rows rather than at anything of their own.\n")
    fprint(f, "def vk_views2_create(var boost : array<array<auto(BT)>>; var vkraw2 : array<array<auto(VT)>>; var vkptr : array<void?>) \{\n")
    fprint(f, "    vkraw2 |> resize(length(boost))\n")
    fprint(f, "    vkptr |> resize(length(boost))\n")
    fprint(f, "    for (vkr2, vkp, vke in vkraw2, vkptr, boost) \{\n")
    fprint(f, "        vkr2 |> resize(length(vke))\n")
    fprint(f, "        for (vkw, vkf in vkr2, vke) \{\n")
    fprint(f, "            vkw = vk_view_create_unsafe(vkf)\n")
    fprint(f, "        \}\n")
    fprint(f, "        vkp = unsafe(reinterpret<void?>(array_addr(vkr2)))\n")
    fprint(f, "    \}\n")
    fprint(f, "\}\n\n")
    fprint(f, "def vk_views2_destroy(var boost : array<array<auto(BT)>>; var vkraw2 : array<array<auto(VT)>>; var vkptr : array<void?>) \{\n")
    fprint(f, "    for (vkrow in boost) \{\n")
    fprint(f, "        for (vkv in vkrow) \{ vk_view_destroy(vkv) \}\n")
    fprint(f, "    \}\n")
    fprint(f, "    for (vkrow2 in vkraw2) \{ delete vkrow2 \}\n")
    fprint(f, "    delete vkraw2\n")
    fprint(f, "    delete vkptr\n")
    fprint(f, "\}\n\n")
}

[unused_argument(em)]
def private emit_structs_boost(reg : VkRegistry; em : EmitModel; out_dir : string; views : array<ViewStruct>) {
    fopen("{out_dir}/vulkan_structs.das", "wb") $(f) {
        panic("can't write vulkan_structs.das") if (f == null)
        fprint(f, GEN_HEADER)
        fprint(f, "options gen2\noptions _comment_hygiene = true\noptions indenting = 4\noptions _function_length = 0   // generated flat field-copy lists\n\nmodule vulkan_structs shared\n\n")
        fprint(f, "require vulkan public\nrequire vulkan/vulkan_ctors public\nrequire vulkan/vulkan_handles public\nrequire vulkan/vulkan_runtime public\nrequire math\n\n")
        for (vs in views) {
            let s & = unsafe(reg.structs[vs.s_index])
            // boost struct
            fprint(f, "struct {vs.boost} \{\n")
            for (bf in vs.fields) {
                if (bf.kind == FieldKind.erase) {
                    continue
                }
                if (bf.kind == FieldKind.next_ptr) {
                    fprint(f, "    next : void?\n")
                } elif (bf.kind == FieldKind.opt_struct_ptr) {
                    // borrowed, never owned: @do_not_delete stops the finalize that would free the caller's local
                    fprint(f, "    @do_not_delete {bf.name} : {bf.boost_type}\n")
                } else {
                    fprint(f, "    {bf.name} : {bf.boost_type}\n")
                }
            }
            // scratch arrays for the array fields
            for (bf in vs.fields) {
                if (bf.kind == FieldKind.arr_scalar || bf.kind == FieldKind.arr_handle || bf.kind == FieldKind.arr_struct) {
                    fprint(f, "    _vk_view_{bf.name} : array<{bf.elem_vktype}>\n")
                }
            }
            // opt_struct_ptr scratch: pointee clone + raw view + `_present` so vk_view_destroy only tears down what was populated
            for (bf in vs.fields) {
                if (bf.kind == FieldKind.opt_struct_ptr) {
                    fprint(f, "    _vk_view_{bf.name}_vk : {bf.elem_vktype}\n")
                    fprint(f, "    _vk_view_{bf.name}_present : bool\n")
                }
            }
            fprint(f, "\}\n")
            // counts are set after the arrays so a shared count sees every length; `b` drops `var` when the view has no scratch (LINT014-clean)
            let b_create = view_has_scratch(vs) ? "var b" : "b"
            fprint(f, "def vk_view_create_unsafe({b_create} : {vs.boost}) : {s.name} \{\n")
            fprint(f, "    var vk = {s.name}()\n")
            for (bf in vs.fields) {
                if (bf.kind == FieldKind.scalar || bf.kind == FieldKind.str) {
                    fprint(f, "    vk.{bf.name} = b.{bf.name}\n")
                } elif (bf.kind == FieldKind.boost_handle) {
                    fprint(f, "    vk.{bf.name} = boost_value_to_vk(b.{bf.name})\n")
                } elif (bf.kind == FieldKind.next_ptr) {
                    fprint(f, "    vk.pNext = b.next\n")
                } elif (bf.kind == FieldKind.arr_scalar) {
                    // array_addr returns null for an empty array -> an optional empty array becomes a null pointer
                    fprint(f, "    b._vk_view_{bf.name} := b.{bf.name}\n")
                    // a Flags-typedef array's raw ptr is uint?/uint64? but the scratch holds the bitfield alias -> reinterpret
                    if (!empty(bf.arr_raw_ptr)) {
                        fprint(f, "    vk.{bf.name} = unsafe(reinterpret<{bf.arr_raw_ptr}>(array_addr(b._vk_view_{bf.name})))\n")
                    } else {
                        fprint(f, "    vk.{bf.name} = array_addr(b._vk_view_{bf.name})\n")
                    }
                } elif (bf.kind == FieldKind.arr_handle) {
                    fprint(f, "    b._vk_view_{bf.name} <- [for (h in b.{bf.name}); boost_value_to_vk(h)]\n")
                    fprint(f, "    vk.{bf.name} = array_addr(b._vk_view_{bf.name})\n")
                } elif (bf.kind == FieldKind.arr_struct) {
                    // recursively view each element in place (its sub-scratch lives in b.{bf.name}[i])
                    fprint(f, "    b._vk_view_{bf.name} |> resize(length(b.{bf.name}))\n")
                    fprint(f, "    for (vkv, vke in b._vk_view_{bf.name}, b.{bf.name}) {OB}\n")
                    fprint(f, "        vkv = vk_view_create_unsafe(vke)\n")
                    fprint(f, "    {CB}\n")
                    fprint(f, "    vk.{bf.name} = array_addr(b._vk_view_{bf.name})\n")
                } elif (bf.kind == FieldKind.opt_struct_ptr) {
                    // viewed where the caller keeps it, so this struct embeds only the raw view and stays copyable
                    fprint(f, "    unsafe {OB}\n")
                    fprint(f, "        if (b.{bf.name} != null) {OB}\n")
                    fprint(f, "            b._vk_view_{bf.name}_vk = vk_view_create_unsafe(*b.{bf.name})\n")
                    fprint(f, "            b._vk_view_{bf.name}_present = true\n")
                    fprint(f, "            vk.{bf.name} = addr(b._vk_view_{bf.name}_vk)\n")
                    fprint(f, "            b.{bf.name} = null\n")
                    fprint(f, "        {CB}\n")
                    fprint(f, "    {CB}\n")
                }
            }
            // counts: derived (authoritative array length), or independent (caller's value, 0 -> largest referencing length)
            for (bf in vs.fields) {
                let cm = das_safe_name(bf.raw_name)
                if (bf.kind == FieldKind.count_independent) {
                    let arrs <- count_referencing_arrays(vs, bf.raw_name)
                    fprint(f, "    vk.{cm} = b.{bf.name} != 0u ? b.{bf.name} : {len_max_expr(arrs, bf.boost_type)}\n")
                } elif (bf.kind == FieldKind.erase) {
                    let auth = count_authoritative_array(vs, bf.raw_name)
                    if (!empty(auth)) {
                        fprint(f, "    vk.{cm} = {count_len_expr(bf.boost_type, auth)}\n")
                    }
                }
            }
            fprint(f, "    return <- vk\n")
            fprint(f, "\}\n")
            // a scratch-free view has nothing to tear down -> `_b` (LINT012-clean) and a bare `pass` body
            let b_destroy = view_has_scratch(vs) ? "var b" : "_b"
            fprint(f, "def vk_view_destroy({b_destroy} : {vs.boost}) \{\n")
            var any_scratch = false
            for (bf in vs.fields) {
                if (bf.kind == FieldKind.arr_handle) {
                    fprint(f, "    unsafe \{ delete b._vk_view_{bf.name} \}\n")    // array of handle pointers
                    any_scratch = true
                } elif (bf.kind == FieldKind.arr_scalar) {
                    fprint(f, "    delete b._vk_view_{bf.name}\n")
                    any_scratch = true
                } elif (bf.kind == FieldKind.arr_struct) {
                    // tear down each element's sub-view first, then free the raw scratch
                    fprint(f, "    for (vki in range(length(b.{bf.name}))) {OB}\n")
                    fprint(f, "        vk_view_destroy(b.{bf.name}[vki])\n")
                    fprint(f, "    {CB}\n")
                    fprint(f, "    delete b._vk_view_{bf.name}\n")
                    any_scratch = true
                } elif (bf.kind == FieldKind.opt_struct_ptr) {
                    // the pointee was the caller's, so releasing is just dropping the claim one was taken
                    fprint(f, "    if (b._vk_view_{bf.name}_present) {OB}\n")
                    fprint(f, "        b._vk_view_{bf.name}_present = false\n")
                    fprint(f, "    {CB}\n")
                    any_scratch = true
                }
            }
            if (!any_scratch) {
                fprint(f, "    pass\n")
            }
            fprint(f, "\}\n\n")
        }
        emit_jagged_view_helpers(f)
    }
    to_log(LOG_INFO, "boost: {length(views)} vk_view structs -> vulkan_structs.das\n")
}

// ===== commands: generated creators (RAII create_X returning an owning wrapper) =====

//! One raw-call argument of a batch creator, in command param order.
enum BatchArgKind {
    parent          //! np==0 handle -> boost_value_to_vk(name)
    count           //! shape A: the createInfoCount scalar -> uint(n)
    create_infos    //! shape A: array of CreateInfo -> addr(vkraw[0])
    create_info     //! shape B: single AllocateInfo -> vci
    allocator       //! VkAllocationCallbacks -> null
    out_handles     //! the array out-handle -> addr(vkout[0])
}

struct BatchArg {
    kind : BatchArgKind
    pname : string      //! parent: das-safe handle param name
}

struct CreatorInfo {
    command : string            //! vkCreateImage
    fn_name : string            //! create_image
    handle : string             //! VkImage
    boost : string              //! Image
    create_info_boost : string  //! ImageCreateInfo
    create_info_vk : string     //! VkImageCreateInfo (batch shape A array element type)
    creator_params : array<ParentField>  //! the creator's leading handle params (call + signature)
    store_parents : array<ParentField>   //! the destroyer's parents, stored in the wrapper for finalize
    // batch creators (array-producing Create/Allocate) -- see derive_creators / emit_batch_creator
    is_batch : bool
    shape_a : bool              //! true = array of CreateInfos (pipelines); false = one AllocateInfo
    count_member : string       //! shape B: CI member holding the element count (descriptorSetCount)
    batch_call : array<BatchArg>  //! raw-call args in command param order
}

//! snake_case a vkCreateImage / vkAllocateMemory name -> create_image / allocate_memory
def private command_fn_name(vkname : string) : string {
    var body = vkname
    if (body |> starts_with("vk")) {
        body = body |> slice(2)
    }
    return camel_to_snake(body)
}

def private camel_to_snake(s : string) : string {
    return build_string() $(var w) {
        peek_data(s) $(arr) {
            for (i in range(length(arr))) {
                let c = int(arr[i])
                let up = c >= 'A' && c <= 'Z'
                if (up && i > 0) {
                    w |> write_char('_')
                }
                w |> write_char(up ? c - 'A' + 'a' : c)
            }
        }
    }
}

def private derive_creators(reg : VkRegistry; em : EmitModel; owners : array<OwnerInfo>; views : array<ViewStruct>) : array<CreatorInfo> {
    // index owners by handle name, and the set of emitted view struct (vk) names
    let owner_of <- {for (oi, i in owners, range(length(owners))); oi.handle => i}
    var view_set : table<string>
    for (vs in views) {
        view_set |> insert(reg.structs[vs.s_index].name)
    }
    var creators : array<CreatorInfo>
    for (c in reg.commands) {
        continue if (!empty(c.alias) || !key_exists(reg.enabled_commands, c.name) || !empty(c.protect) ||
            (!(c.name |> starts_with("vkCreate")) && !(c.name |> starts_with("vkAllocate"))))
        // out-handle `len` -> batch; the create-info's OWN len picks array-of-CreateInfos vs single AllocateInfo
        var out_handle = ""
        var out_len = ""
        var ci_vk = ""
        var ci_has_len = false
        var creator_params : array<ParentField>
        for (p in c.params) {
            if (key_exists(em.handle_names, p.type_name) && p.num_ptrs >= 1) {
                out_handle = p.type_name
                out_len = p.len
            } elif (key_exists(em.handle_names, p.type_name) && p.num_ptrs == 0) {
                creator_params |> emplace(ParentField(field = "_{das_safe_name(p.name)}", vktype = p.type_name))
            } elif (key_exists(view_set, p.type_name) && p.num_ptrs == 1) {
                ci_vk = p.type_name
                ci_has_len = !empty(p.len)
            }
        }
        continue if (empty(out_handle) || empty(ci_vk))
        let oidx = owner_of?[out_handle] ?? -1
        continue if (oidx < 0)
        let oi & = unsafe(owners[oidx])
        var info = CreatorInfo(
            command = c.name,
            fn_name = command_fn_name(c.name),
            handle = out_handle,
            boost = oi.boost,
            create_info_boost = boost_name(ci_vk),
            create_info_vk = ci_vk
        )
        info.creator_params <- creator_params
        info.store_parents := oi.parents
        if (!empty(out_len)) {
            info.is_batch = true
            info.shape_a = ci_has_len
            info.count_member = batch_count_member(out_len)
            info.batch_call <- derive_batch_call(em, c, view_set, out_len, ci_has_len)
        }
        creators |> emplace(info)
    }
    return <- creators
}

//! shape B len: "pAllocateInfo->descriptorSetCount" -> "descriptorSetCount"
def private batch_count_member(len_attr : string) : string {
    let arrow = len_attr |> find("->")
    return arrow >= 0 ? len_attr |> slice(arrow + 2) : len_attr
}

def private batch_arg(k : BatchArgKind; pname : string) : BatchArg => BatchArg(kind = k, pname = pname)

//! the raw-call args of a batch creator, in command param order: parents, the count
//! scalar (shape A), the create-info, the allocator, the array out-handle.
def private derive_batch_call(em : EmitModel; c : VkCommandDef; view_set : table<string>; out_len : string; shape_a : bool) : array<BatchArg> {
    var call : array<BatchArg>
    for (p in c.params) {
        if (key_exists(em.handle_names, p.type_name) && p.num_ptrs >= 1) {
            call |> emplace(batch_arg(BatchArgKind.out_handles, ""))
        } elif (key_exists(em.handle_names, p.type_name) && p.num_ptrs == 0) {
            call |> emplace(batch_arg(BatchArgKind.parent, das_safe_name(p.name)))
        } elif (key_exists(view_set, p.type_name) && p.num_ptrs == 1) {
            call |> emplace(batch_arg(shape_a ? BatchArgKind.create_infos : BatchArgKind.create_info, ""))
        } elif (p.type_name == "VkAllocationCallbacks") {
            call |> emplace(batch_arg(BatchArgKind.allocator, ""))
        } elif (shape_a && p.name == out_len) {
            call |> emplace(batch_arg(BatchArgKind.count, ""))
        }
    }
    return <- call
}

def private param_name_of(field : string) : string {
    // wrapper field "_device" -> param "device"
    if (field |> starts_with("_")) {
        return field |> slice(1)
    }
    return field
}

def private batch_call_arg(a : BatchArg) : string {
    if (a.kind == BatchArgKind.parent) {
        return "boost_value_to_vk({a.pname})"
    } elif (a.kind == BatchArgKind.count) {
        return "uint(n)"
    } elif (a.kind == BatchArgKind.create_infos) {
        return "addr(vkraw[0])"
    } elif (a.kind == BatchArgKind.create_info) {
        return "vci"
    } elif (a.kind == BatchArgKind.out_handles) {
        return "addr(vkout[0])"
    }
    return "null"   // allocator
}

//! Emit a batch creator (array-producing Create/Allocate) -> array<Boost>. Shape A is an array
//! of CreateInfos sized by createInfoCount; shape B is a single AllocateInfo whose count member
//! sizes the result. Mirrors the single-creator and in_array_struct/out_array idioms.
def private emit_batch_creator(f : file; ci : CreatorInfo; scratch_types : table<string>) {
    let ci_var = key_exists(scratch_types, ci.create_info_boost) ? "var " : ""
    fprint(f, "def {ci.fn_name}(")
    for (pf in ci.creator_params) {
        fprint(f, "{param_name_of(pf.field)} : {boost_name(pf.vktype)}; ")
    }
    if (ci.shape_a) {
        fprint(f, "{ci_var}create_infos : array<{ci.create_info_boost}>; ")
    } else {
        fprint(f, "{ci_var}create_info : {ci.create_info_boost}; ")
    }
    fprint(f, "var result : VkResult? = null) : array<{ci.boost}> \{\n")
    if (ci.shape_a) {
        fprint(f, "    let n = length(create_infos)\n")
        fprint(f, "    var vkraw : array<{ci.create_info_vk}>\n")
        fprint(f, "    vkraw |> resize(n)\n")
        fprint(f, "    for (vkv, vke in vkraw, create_infos) \{\n")
        fprint(f, "        vkv = vk_view_create_unsafe(vke)\n")
        fprint(f, "    \}\n")
    } else {
        fprint(f, "    let vci = vk_view_create_unsafe(create_info)\n")
        fprint(f, "    defer() \{ vk_view_destroy(create_info) \}\n")
        fprint(f, "    let n = int(vci.{ci.count_member})\n")
    }
    fprint(f, "    var vkout : array<{ci.handle}>\n")
    fprint(f, "    vkout |> resize(n)\n")
    fprint(f, "    if (n > 0) \{\n")
    fprint(f, "        unsafe \{\n")
    fprint(f, "            vk_check({ci.command}(")
    var first = true
    for (a in ci.batch_call) {
        fprint(f, ", ") if (!first)
        first = false
        fprint(f, batch_call_arg(a))
    }
    fprint(f, "), result)\n")
    fprint(f, "        \}\n")
    fprint(f, "    \}\n")
    if (ci.shape_a) {
        fprint(f, "    for (vkv in create_infos) \{ vk_view_destroy(vkv) \}\n")
        fprint(f, "    delete vkraw\n")
    }
    fprint(f, "    var blist : array<{ci.boost}>\n")
    fprint(f, "    blist |> resize(n)\n")
    fprint(f, "    for (i in range(n)) \{\n")
    fprint(f, "        var b = vk_value_to_boost(vkout[i])\n")
    fprint(f, "        b._needs_delete = true\n")
    for (sp in ci.store_parents) {
        for (cp in ci.creator_params) {
            if (cp.vktype == sp.vktype) {
                fprint(f, "        b.{sp.field} = unsafe(reinterpret<uint64>(boost_value_to_vk({param_name_of(cp.field)})))\n")
                break
            }
        }
    }
    fprint(f, "        blist[i] = b\n")
    fprint(f, "    \}\n")
    fprint(f, "    return <- blist\n")
    fprint(f, "\}\n\n")
}

[unused_argument(reg, em)]
def private emit_commands_boost(reg : VkRegistry; em : EmitModel; out_dir : string; creators : array<CreatorInfo>; scratch_types : table<string>) {
    fopen("{out_dir}/vulkan_commands.das", "wb") $(f) {
        panic("can't write vulkan_commands.das") if (f == null)
        fprint(f, GEN_HEADER)
        fprint(f, "options gen2\noptions _comment_hygiene = true\noptions indenting = 4\n\nmodule vulkan_commands shared\n\n")
        fprint(f, "require vulkan public\nrequire vulkan/vulkan_handles public\nrequire vulkan/vulkan_structs public\nrequire vulkan/vulkan_runtime public\nrequire daslib/defer\n\n")
        for (ci in creators) {
            if (ci.is_batch) {
                emit_batch_creator(f, ci, scratch_types)
                continue
            }
            // signature: creator's leading handle params (boost) + create_info + optional result
            fprint(f, "def {ci.fn_name}(")
            for (pf in ci.creator_params) {
                fprint(f, "{param_name_of(pf.field)} : {boost_name(pf.vktype)}; ")
            }
            let ci_var = key_exists(scratch_types, ci.create_info_boost) ? "var " : ""
            fprint(f, "{ci_var}create_info : {ci.create_info_boost}; var result : VkResult? = null) : {ci.boost} \{\n")
            fprint(f, "    let vci = vk_view_create_unsafe(create_info)\n")
            fprint(f, "    defer() \{ vk_view_destroy(create_info) \}\n")
            fprint(f, "    var h : {ci.handle}\n")
            fprint(f, "    vk_check({ci.command}(")
            for (pf in ci.creator_params) {
                fprint(f, "boost_value_to_vk({param_name_of(pf.field)}), ")
            }
            fprint(f, "vci, null, h), result)\n")
            fprint(f, "    var b = vk_value_to_boost(h)\n")
            fprint(f, "    b._needs_delete = true\n")
            // store the destroyer's parents (matched to a creator param of the same type)
            for (sp in ci.store_parents) {
                for (cp in ci.creator_params) {
                    if (cp.vktype == sp.vktype) {
                        fprint(f, "    b.{sp.field} = unsafe(reinterpret<uint64>(boost_value_to_vk({param_name_of(cp.field)})))\n")
                        break
                    }
                }
            }
            fprint(f, "    return <- b\n")
            fprint(f, "\}\n\n")
        }
    }
    to_log(LOG_INFO, "boost: {length(creators)} creators -> vulkan_commands.das\n")
}

// ===== plain commands (vkCmd* / vkGet* / vkQueue* / vkWait* / ...) =====

// Every non-create, non-destroy command: boost handles, vk_view structs, array<T> for count+ptr
// pairs, a single out-param -> return value, two-call enumerate -> array<T>. Conservative: any
// unmappable param skips the whole command (logged); the raw vk* call always stays available.

enum CmdArgKind {
    in_handle       //! boost handle by value -> boost_value_to_vk(x)
    in_view         //! single const struct ptr -> boost vk_view struct
    in_array_scalar //! count+const scalar/enum ptr -> array<scalar>
    in_array_handle //! count+const handle ptr -> array<BoostHandle>
    in_array_struct //! count+const mappable-struct ptr -> array<BoostStruct> (recursive vk_view)
    in_array2_struct //! count+const T* const* mappable struct -> array<array<BoostStruct>> (jagged)
    in_scalar       //! by-value scalar / enum / flags
    in_string       //! const char*
    erasevkcount     //! input count member named by an input array's len -> uint(length(arr))
    out_value       //! single non-const ptr (by-ref) -> returned
    out_array       //! enumerate array out -> array<elem>, addr() pointer
    count_query     //! the uint32* count in a two-call enumerate (by-ref)
    hard            //! unmappable -> skip the whole command
}

struct CmdArg {
    kind : CmdArgKind
    pname : string          //! das-safe param name
    boost_type : string     //! signature type (for inputs)
    vktype : string         //! raw type name
    elem_vktype : string    //! arrays: raw element type
    elem_boost : string     //! arrays: boost element type
    count_for : string      //! erasevkcount: das-safe name of the array param it counts
}

struct CmdPlan {
    emittable : bool
    skip : string
    fn_name : string
    command : string
    args : array<CmdArg>            //! all params, in declaration order
    is_result : bool               //! returns VkResult -> thread var result + vk_check
    has_out_value : bool
    out_value_pname : string
    out_value_vktype : string
    out_value_ret : string         //! returned das type
    out_value_is_handle : bool
    has_out_array : bool
    out_array_pname : string
    out_array_elem : string        //! raw elem type (array<elem>)
    out_array_is_bytes : bool      //! enumerate writes a void payload -> array<uint8>
    count_query_pname : string
    count_is_64 : bool             //! enumerate count is size_t (uint64) not uint32
    direct_ret : string            //! non-void non-VkResult scalar return ("" if none)
}

def private boost_is_flags(em : EmitModel; tn : string) : bool {
    let fi = em.flags_lookup?[tn] ?? FlagsInfo()
    return fi.kind != FlagsKind.not_flags
}

//! can we declare `var x : <mapped>` and return/pass it by value? scalar / handle /
//! emitted-struct / plain-enum. Excludes flags (raw uint* pointer), void, foreign, PFN.
def private mappable_value_type(reg : VkRegistry; em : EmitModel; tn : string) : bool {
    let bt = reg.base_types?[tn] ?? ""
    return (!empty(DAS_SCALAR_MAP?[tn] ?? "") || !empty(DAS_SCALAR_MAP?[bt] ?? "") ||
        key_exists(em.handle_names, tn) || key_exists(em.struct_emit, tn) || key_exists(em.plain_enums, tn))
}

//! the das type for a by-value/out scalar/enum/flags/handle/struct type name
def private cmd_value_type(reg : VkRegistry; em : EmitModel; vktype : string) : string {
    let ds = DAS_SCALAR_MAP?[vktype] ?? ""
    if (!empty(ds)) {
        return ds
    }
    let fi = em.flags_lookup?[vktype] ?? FlagsInfo()
    if (fi.kind != FlagsKind.not_flags) {
        return fi.canonical
    }
    let bt = reg.base_types?[vktype] ?? ""
    let vb = DAS_SCALAR_MAP?[bt] ?? ""
    if (!empty(vb)) {
        return vb
    }
    return vktype                  // enum / nested struct / handle raw type
}

def private plan_command(reg : VkRegistry; em : EmitModel; c : VkCommandDef; view_set : table<string>) : CmdPlan {
    var plan = CmdPlan(emittable = true, fn_name = command_fn_name(c.name), command = c.name)
    let n = length(c.params)
    plan.is_result = c.ret_type == "VkResult"
    if (c.ret_type != "void" && !plan.is_result) {
        if (key_exists(reg.func_pointers, c.ret_type) || !mappable_value_type(reg, em, c.ret_type)) {
            plan.emittable = false
            plan.skip = "{c.name}: return type {c.ret_type} not mappable"
            return <- plan
        }
        plan.direct_ret = cmd_value_type(reg, em, c.ret_type)
    }
    var param_names : table<string>
    for (p in c.params) {
        param_names |> insert(p.name)
    }
    // two-call enumerate: a non-const count param whose name is the len= of a non-const out-array param
    var enum_count = -1
    var enum_array = -1
    for (p, i in c.params, range(n)) {
        if (p.num_ptrs == 1 && !p.is_const && (p.type_name == "uint32_t" || p.type_name == "size_t")) {
            for (q, j in c.params, range(n)) {
                let bytes = q.type_name == "void" && q.num_ptrs == 1
                if (j != i && !empty(q.len) && q.len == p.name && q.num_ptrs >= 1 && !q.is_const &&
                        (bytes || (q.type_name != "void" && mappable_value_type(reg, em, q.type_name)))) {
                    enum_count = i
                    enum_array = j
                    plan.out_array_is_bytes = bytes
                    plan.count_is_64 = p.type_name == "size_t"
                }
            }
        }
    }
    // names of params that are the count (len=) of an INPUT array param
    var in_counts : table<string>
    for (p, i in c.params, range(n)) {
        continue if (i == enum_array)
        if (!empty(p.len) && p.is_const && p.num_ptrs >= 1 && key_exists(param_names, p.len)) {
            in_counts |> insert(p.len)
        }
    }
    var out_value_count = 0
    for (p, i in c.params, range(n)) {
        var a = CmdArg(pname = das_safe_name(p.name), vktype = p.type_name)
        let is_handle = key_exists(em.handle_names, p.type_name)
        if (i == enum_count) {
            a.kind = CmdArgKind.count_query
            plan.count_query_pname = a.pname
            plan.args |> emplace(a)
            continue
        }
        if (i == enum_array) {
            a.kind = CmdArgKind.out_array
            plan.has_out_array = true
            plan.out_array_pname = a.pname
            plan.out_array_elem = plan.out_array_is_bytes ? "uint8" : p.type_name
            plan.args |> emplace(a)
            continue
        }
        // jagged const T* const* -> array<array<Boost>>; inner length is per-element, no count param to erase
        if (p.num_ptrs == 2 && p.is_const && !empty(p.len) && key_exists(view_set, p.type_name)) {
            a.kind = CmdArgKind.in_array2_struct
            a.elem_boost = boost_name(p.type_name)
            a.elem_vktype = p.type_name
            a.boost_type = "array<array<{a.elem_boost}>>"
            plan.args |> emplace(a)
            continue
        }
        // function pointer / fixed-array / multi-ptr -> hard
        if (key_exists(reg.func_pointers, p.type_name) || !empty(p.arr_dims) || p.num_ptrs >= 2) {
            plan.emittable = false
            plan.skip = "{c.name}: param {p.name} ({p.type_name} x{p.num_ptrs}) not mappable"
            return <- plan
        }
        // input count member of an input array -> derived
        if (key_exists(in_counts, p.name)) {
            a.kind = CmdArgKind.erasevkcount
            plan.args |> emplace(a)
            continue
        }
        // by-ref out value: the raw binding ref-promotes only when !optional, and only for mappable value types
        if (p.num_ptrs == 1 && !p.is_const && empty(p.len) && p.type_name != "void") {
            if (p.optional_attr == "true" || boost_is_flags(em, p.type_name) || !mappable_value_type(reg, em, p.type_name)) {
                plan.emittable = false
                plan.skip = "{c.name}: out param {p.name} ({p.type_name}) not a ref-mappable value"
                return <- plan
            }
            a.kind = CmdArgKind.out_value
            plan.has_out_value = true
            plan.out_value_pname = a.pname
            plan.out_value_vktype = p.type_name
            plan.out_value_is_handle = is_handle
            plan.out_value_ret = is_handle ? boost_name(p.type_name) : cmd_value_type(reg, em, p.type_name)
            out_value_count ++
            plan.args |> emplace(a)
            continue
        }
        // const array: len names a count param (erased, derived from length) or another struct's member ("pBuildInfo::geometryCount") -- the latter has no count param to erase
        let len_is_param = !empty(p.len) && key_exists(param_names, p.len)
        let hasvkcount = len_is_param && p.is_const && p.num_ptrs == 1
        let member_len = !empty(p.len) && !len_is_param && p.is_const && p.num_ptrs == 1
        let arrayvkcount = len_is_param ? das_safe_name(p.len) : ""
        if ((hasvkcount || member_len) && p.type_name != "void" && p.type_name != "char") {
            if (is_handle) {
                a.kind = CmdArgKind.in_array_handle
                a.elem_boost = boost_name(p.type_name)
                a.elem_vktype = p.type_name
                a.boost_type = "array<{a.elem_boost}>"
                a.count_for = arrayvkcount
                plan.args |> emplace(a)
                continue
            }
            let ds = arr_scalar_elem(reg, p.type_name)
            if (!empty(ds)) {
                a.kind = CmdArgKind.in_array_scalar
                a.elem_vktype = ds
                a.boost_type = "array<{ds}>"
                a.count_for = arrayvkcount
                plan.args |> emplace(a)
                continue
            }
            if (key_exists(view_set, p.type_name)) {
                a.kind = CmdArgKind.in_array_struct
                a.elem_boost = boost_name(p.type_name)
                a.elem_vktype = p.type_name
                a.boost_type = "array<{a.elem_boost}>"
                a.count_for = arrayvkcount
                plan.args |> emplace(a)
                continue
            }
            plan.emittable = false      // array of enum/flags / non-mappable struct -> defer
            plan.skip = "{c.name}: array param {p.name} ({p.type_name}) of non-scalar/handle"
            return <- plan
        }
        // single const struct ptr (ref-promoted: no len, not optional) -> boost vk_view
        if (p.num_ptrs == 1 && p.is_const && empty(p.len) && p.optional_attr != "true" && key_exists(view_set, p.type_name)) {
            a.kind = CmdArgKind.in_view
            a.boost_type = boost_name(p.type_name)
            plan.args |> emplace(a)
            continue
        }
        // const char*
        if (p.type_name == "char" && p.num_ptrs == 1) {
            a.kind = CmdArgKind.in_string
            a.boost_type = "string"
            plan.args |> emplace(a)
            continue
        }
        // any remaining pointer -> hard
        if (p.num_ptrs >= 1) {
            plan.emittable = false
            plan.skip = "{c.name}: pointer param {p.name} ({p.type_name}) not mappable"
            return <- plan
        }
        // by-value handle
        if (is_handle) {
            a.kind = CmdArgKind.in_handle
            a.boost_type = boost_name(p.type_name)
            plan.args |> emplace(a)
            continue
        }
        // by-value scalar / enum / flags / nested POD struct
        if (!mappable_value_type(reg, em, p.type_name) && !boost_is_flags(em, p.type_name)) {
            plan.emittable = false
            plan.skip = "{c.name}: by-value param {p.name} ({p.type_name}) not mappable"
            return <- plan
        }
        a.kind = CmdArgKind.in_scalar
        a.boost_type = cmd_value_type(reg, em, p.type_name)
        plan.args |> emplace(a)
    }
    if (out_value_count > 1) {
        plan.emittable = false
        plan.skip = "{c.name}: {out_value_count} out params (only single out supported)"
        return <- plan
    }
    // a two-call enumerate returns its array, leaving nowhere for a second out param
    if (plan.has_out_array && plan.has_out_value) {
        plan.emittable = false
        plan.skip = "{c.name}: out param {plan.out_value_pname} beside an enumerated array"
        return <- plan
    }
    // link each erasevkcount to the array param it counts (-> uint(length(arr)))
    for (i in range(length(plan.args))) {
        let ak = plan.args[i].kind
        if (ak == CmdArgKind.in_array_scalar || ak == CmdArgKind.in_array_handle || ak == CmdArgKind.in_array_struct) {
            let target = plan.args[i].count_for
            let arr_pname = plan.args[i].pname
            for (j in range(length(plan.args))) {
                if (plan.args[j].kind == CmdArgKind.erasevkcount && plan.args[j].pname == target) {
                    plan.args[j].count_for = arr_pname
                }
            }
        }
    }
    return <- plan
}

// hand-written in vulkan_boost.das as ergonomic conveniences (defaulted args /
// raw-queue return) -> the generator skips these names to avoid a redefinition.
let HAND_WRITTEN_COMMANDS : table<string> <- { "cmd_bind_pipeline", "cmd_draw", "get_device_queue" }

def public derive_commands(reg : VkRegistry; em : EmitModel; views : array<ViewStruct>; var skip_log : array<string>) : array<CmdPlan> {
    var view_set : table<string>
    for (vs in views) {
        view_set |> insert(reg.structs[vs.s_index].name)
    }
    var plans : array<CmdPlan>
    for (c in reg.commands) {
        // creators (vkCreate*/vkAllocate*) and destroyers (vkDestroy*/vkFree*) are
        // handled elsewhere (RAII creators / finalize)
        continue if (!empty(c.alias) || !key_exists(reg.enabled_commands, c.name) || !empty(c.protect) ||
            c.name |> starts_with("vkCreate") || c.name |> starts_with("vkAllocate") ||
            c.name |> starts_with("vkDestroy") || c.name |> starts_with("vkFree"))
        var plan <- plan_command(reg, em, c, view_set)
        continue if (key_exists(HAND_WRITTEN_COMMANDS, plan.fn_name))
        if (!plan.emittable) {
            skip_log |> push(plan.skip)
            continue
        }
        plans |> emplace(plan)
    }
    return <- plans
}

//! the raw-call argument expression for one param
def private call_arg(a : CmdArg) : string {
    if (a.kind == CmdArgKind.in_handle) {
        return "boost_value_to_vk({a.pname})"
    } elif (a.kind == CmdArgKind.in_view) {
        return "vkview_{a.pname}"
    } elif (a.kind == CmdArgKind.in_scalar || a.kind == CmdArgKind.in_string) {
        return a.pname
    } elif (a.kind == CmdArgKind.in_array_scalar) {
        return "array_addr({a.pname})"
    } elif (a.kind == CmdArgKind.in_array_handle || a.kind == CmdArgKind.in_array_struct) {
        return "array_addr(vkraw_{a.pname})"
    } elif (a.kind == CmdArgKind.in_array2_struct) {
        return "unsafe(reinterpret<{a.elem_vktype}??>(array_addr(vkptr_{a.pname})))"
    } elif (a.kind == CmdArgKind.erasevkcount) {
        return "uint(length({a.count_for}))"
    } elif (a.kind == CmdArgKind.count_query) {
        return "vkcount"
    } elif (a.kind == CmdArgKind.out_value) {
        return "vkout"
    }
    return "null"   // out_array first-call placeholder is handled specially
}

def private emit_plain_commands_boost(reg : VkRegistry; em : EmitModel; out_dir : string; plans : array<CmdPlan>; scratch_types : table<string>) {
    fopen("{out_dir}/vulkan_cmds.das", "wb") $(f) {
        panic("can't write vulkan_cmds.das") if (f == null)
        fprint(f, GEN_HEADER)
        fprint(f, "options gen2\noptions _comment_hygiene = true\noptions indenting = 4\n\nmodule vulkan_cmds shared\n\n")
        fprint(f, "require vulkan public\nrequire vulkan/vulkan_handles public\nrequire vulkan/vulkan_structs public\nrequire vulkan/vulkan_runtime public\nrequire daslib/defer\n\n")
        for (plan in plans) {
            // --- signature ---
            fprint(f, "def {plan.fn_name}(")
            var first = true
            for (a in plan.args) {
                let in_sig = (a.kind == CmdArgKind.in_handle || a.kind == CmdArgKind.in_view ||
                    a.kind == CmdArgKind.in_array_scalar || a.kind == CmdArgKind.in_array_handle ||
                    a.kind == CmdArgKind.in_array_struct || a.kind == CmdArgKind.in_array2_struct ||
                    a.kind == CmdArgKind.in_scalar || a.kind == CmdArgKind.in_string)
                continue if (!in_sig)
                if (!first) {
                    fprint(f, "; ")
                }
                first = false
                // in_array_scalar always needs var (array_addr); views need it only when their vk_view has scratch
                var needs_var = (a.kind == CmdArgKind.in_array_scalar ||
                    a.kind == CmdArgKind.in_array2_struct)
                if (a.kind == CmdArgKind.in_view) {
                    needs_var = key_exists(scratch_types, a.boost_type)
                } elif (a.kind == CmdArgKind.in_array_struct) {
                    needs_var = key_exists(scratch_types, a.elem_boost)
                }
                fprint(f, "{needs_var ? "var " : ""}{a.pname} : {a.boost_type}")
            }
            if (plan.is_result) {
                fprint(f, "{first ? "" : "; "}var result : VkResult? = null")
                first = false
            }
            fprint(f, ")")
            // --- return type ---
            if (plan.has_out_array) {
                fprint(f, " : array<{plan.out_array_elem}>")
            } elif (plan.has_out_value) {
                fprint(f, " : {plan.out_value_ret}")
            } elif (!empty(plan.direct_ret)) {
                fprint(f, " : {plan.direct_ret}")
            }
            fprint(f, " {OB}\n")
            // --- view prep ---
            for (a in plan.args) {
                if (a.kind == CmdArgKind.in_view) {
                    fprint(f, "    let vkview_{a.pname} = vk_view_create_unsafe({a.pname})\n")
                    fprint(f, "    defer() {OB} vk_view_destroy({a.pname}) {CB}\n")
                } elif (a.kind == CmdArgKind.in_array_handle) {
                    fprint(f, "    var vkraw_{a.pname} <- [for (h in {a.pname}); boost_value_to_vk(h)]\n")
                } elif (a.kind == CmdArgKind.in_array_struct) {
                    fprint(f, "    var vkraw_{a.pname} : array<{a.elem_vktype}>\n")
                    fprint(f, "    vkraw_{a.pname} |> resize(length({a.pname}))\n")
                    fprint(f, "    for (vkv, vke in vkraw_{a.pname}, {a.pname}) {OB}\n")
                    fprint(f, "        vkv = vk_view_create_unsafe(vke)\n")
                    fprint(f, "    {CB}\n")
                } elif (a.kind == CmdArgKind.in_array2_struct) {
                    fprint(f, "    var vkraw2_{a.pname} : array<array<{a.elem_vktype}>>\n")
                    fprint(f, "    var vkptr_{a.pname} : array<void?>\n")
                    fprint(f, "    vk_views2_create({a.pname}, vkraw2_{a.pname}, vkptr_{a.pname})\n")
                }
            }
            // --- the call(s) ---
            if (plan.has_out_array) {
                // both enumerate calls go through vk_check so OOM / VK_INCOMPLETE surfaces instead of truncating the array
                fprint(f, "    var vkcount = {plan.count_is_64 ? "0ul" : "0u"}\n")
                let result_arg = plan.is_result ? ", result" : ""
                let lhs_open = plan.is_result ? "vk_check(" : ""
                let lhs_close = plan.is_result ? "{result_arg})" : ""
                fprint(f, "    {lhs_open}{plan.command}(")
                emit_call_args(f, plan, "null")
                fprint(f, "){lhs_close}\n")
                fprint(f, "    var vklist : array<{plan.out_array_elem}>\n")
                fprint(f, "    vklist |> resize({plan.count_is_64 ? "int64" : "int"}(vkcount))\n")
                if (has_stype_ctor(reg, em, plan.out_array_elem)) {
                    fprint(f, "    for (vki in range(int(vkcount))) {OB}\n")
                    fprint(f, "        vklist[vki] = {plan.out_array_elem}()\n")
                    fprint(f, "    {CB}\n")
                }
                fprint(f, "    if (vkcount > {plan.count_is_64 ? "0ul" : "0u"}) {OB}\n")
                fprint(f, "        unsafe {OB}\n")
                fprint(f, "            {lhs_open}{plan.command}(")
                // already inside `unsafe`, so the byte payload cast takes no wrap of its own
                emit_call_args(f, plan, plan.out_array_is_bytes
                    ? "addr<void?>(vklist[0])" : "addr(vklist[0])")
                fprint(f, "){lhs_close}\n")
                fprint(f, "        {CB}\n")
                fprint(f, "    {CB}\n")
                emit_scratch_cleanup(f, plan)
                fprint(f, "    return <- vklist\n")
            } elif (plan.has_out_value) {
                let out_vt = cmd_value_type(reg, em, plan.out_value_vktype)
                if (has_stype_ctor(reg, em, out_vt)) {
                    fprint(f, "    var vkout = {out_vt}()\n")
                } else {
                    fprint(f, "    var vkout : {out_vt}\n")
                }
                if (plan.is_result) {
                    // vk_check so failures surface via `result` instead of silently yielding an uninitialized vkout
                    fprint(f, "    vk_check({plan.command}(")
                    emit_call_args(f, plan, "")
                    fprint(f, "), result)\n")
                } else {
                    fprint(f, "    {plan.command}(")
                    emit_call_args(f, plan, "")
                    fprint(f, ")\n")
                }
                emit_scratch_cleanup(f, plan)
                if (plan.out_value_is_handle) {
                    fprint(f, "    return vk_value_to_boost(vkout)\n")
                } else {
                    fprint(f, "    return vkout\n")
                }
            } elif (plan.is_result) {
                fprint(f, "    vk_check({plan.command}(")
                emit_call_args(f, plan, "")
                fprint(f, "), result)\n")
                emit_scratch_cleanup(f, plan)
            } elif (!empty(plan.direct_ret)) {
                fprint(f, "    let vkret = {plan.command}(")
                emit_call_args(f, plan, "")
                fprint(f, ")\n")
                emit_scratch_cleanup(f, plan)
                fprint(f, "    return vkret\n")
            } else {
                fprint(f, "    {plan.command}(")
                emit_call_args(f, plan, "")
                fprint(f, ")\n")
                emit_scratch_cleanup(f, plan)
            }
            fprint(f, "{CB}\n\n")
        }
    }
    to_log(LOG_INFO, "boost: {length(plans)} plain command wrappers -> vulkan_cmds.das\n")
}

//! emit the comma-separated raw-call args; `out_array_expr` replaces the out_array slot
def private emit_call_args(f : file; plan : CmdPlan; out_array_expr : string) {
    var first = true
    for (a in plan.args) {
        if (!first) {
            fprint(f, ", ")
        }
        first = false
        if (a.kind == CmdArgKind.out_array) {
            fprint(f, out_array_expr)
        } else {
            fprint(f, call_arg(a))
        }
    }
}

def private emit_scratch_cleanup(f : file; plan : CmdPlan) {
    for (a in plan.args) {
        if (a.kind == CmdArgKind.in_array_handle) {
            fprint(f, "    unsafe {OB} delete vkraw_{a.pname} {CB}\n")
        } elif (a.kind == CmdArgKind.in_array_struct) {
            fprint(f, "    for (vkv in {a.pname}) {OB} vk_view_destroy(vkv) {CB}\n")
            fprint(f, "    delete vkraw_{a.pname}\n")
        } elif (a.kind == CmdArgKind.in_array2_struct) {
            fprint(f, "    vk_views2_destroy({a.pname}, vkraw2_{a.pname}, vkptr_{a.pname})\n")
        }
    }
}

// ===== orchestrator =====

//! Emit daslib/vulkan_lint_data.das: raw Vulkan call -> boost wrapper name, for
//! every emitted creator/command plus the hand-written wrappers the generator
//! skips. Consumed by vulkan_lint (VK001).
def private emit_lint_data(creators : array<CreatorInfo>; plans : array<CmdPlan>; out_dir : string) {
    var seen : table<string>
    var pairs : array<tuple<raw : string; boost : string>>
    for (c in creators) {
        if (!key_exists(seen, c.command)) {
            seen |> insert(c.command)
            pairs |> push((raw = c.command, boost = c.fn_name))
        }
    }
    for (p in plans) {
        if (p.emittable && !key_exists(seen, p.command)) {
            seen |> insert(p.command)
            pairs |> push((raw = p.command, boost = p.fn_name))
        }
    }
    // hand-written boost wrappers (vulkan_boost.das) the generator deliberately skips
    let hand <- [(raw = "vkCmdBindPipeline", boost = "cmd_bind_pipeline"),
                 (raw = "vkCmdDraw", boost = "cmd_draw"),
                 (raw = "vkGetDeviceQueue", boost = "get_device_queue")]
    for (h in hand) {
        if (!key_exists(seen, h.raw)) {
            seen |> insert(h.raw)
            pairs |> push(h)
        }
    }
    fopen("{out_dir}/vulkan_lint_data.das", "wb") $(f) {
        if (f == null) {
            panic("cannot write vulkan_lint_data.das")
        }
        fprint(f, "options gen2\noptions _comment_hygiene = true\noptions indenting = 4\n\n")
        fprint(f, "// DO NOT EDIT - generated by generator/vk_emit_boost.das\n")
        fprint(f, "// raw Vulkan call -> boost wrapper name; consumed by vulkan_lint (VK001).\n")
        fprint(f, "module vulkan_lint_data shared\n\n")
        fprint(f, "let public VK_WRAPPED : table<string; string> <- {OB}\n")
        var first = true
        for (pr in pairs) {
            if (first) {
                first = false
            } else {
                fprint(f, ",\n")
            }
            fprint(f, "    \"{pr.raw}\" => \"{pr.boost}\"")
        }
        fprint(f, "\n{CB}\n")
    }
    to_log(LOG_INFO, "boost: vulkan_lint_data with {length(pairs)} wrapped calls\n")
}

//! `struct_skips_out` / `cmd_skips_out` receive the boost-layer skip logs so generate.das can
//! fold them into the skip-ratchet report
def private is_array_field(bf : BoostField) : bool {
    return (bf.kind == FieldKind.arr_scalar || bf.kind == FieldKind.arr_handle
        || bf.kind == FieldKind.arr_struct)
}

// an array field is what makes a boost struct non-copyable, and das MOVES a non-copyable element out of an array literal
def private collect_non_copyable(views : array<ViewStruct>; var out : array<string>) {
    for (vs in views) {
        var reasons <- [for (bf in vs.fields); "{bf.name} : {bf.boost_type}"; where is_array_field(bf)]
        if (!empty(reasons)) {
            out |> push("{vs.boost}: {join(reasons, ", ")}")
        }
        delete reasons
    }
}

def public emit_boost(reg : VkRegistry; em : EmitModel; out_dir : string;
                      var struct_skips_out : array<string>; var cmd_skips_out : array<string>;
                      var non_copyable_out : array<string>) {
    var skip_log : array<string>
    emit_ctors(reg, em, out_dir)
    let owners <- derive_owners(reg, em)
    emit_handles_boost(reg, em, out_dir, owners)
    let views <- derive_views(reg, em, skip_log)
    emit_structs_boost(reg, em, out_dir, views)
    // boost type names whose vk_view_create_unsafe/destroy mutate `b` (scratch fields). Callers
    // that merely forward such a view keep `var`; forwarders of scratch-free views drop it (LINT014).
    var scratch_types : table<string>
    for (vs in views) {
        scratch_types |> insert(vs.boost) if (view_has_scratch(vs))
    }
    let creators <- derive_creators(reg, em, owners, views)
    emit_commands_boost(reg, em, out_dir, creators, scratch_types)
    var cmd_skip : array<string>
    let plans <- derive_commands(reg, em, views, cmd_skip)
    emit_plain_commands_boost(reg, em, out_dir, plans, scratch_types)
    emit_lint_data(creators, plans, out_dir)
    to_log(LOG_INFO, "boost: {length(skip_log)} structs + {length(cmd_skip)} commands skipped (unmappable)\n")
    delete struct_skips_out
    struct_skips_out <- skip_log
    delete cmd_skips_out
    cmd_skips_out <- cmd_skip
    var non_copyable : array<string>
    collect_non_copyable(views, non_copyable)
    delete non_copyable_out
    non_copyable_out <- non_copyable
}
