options gen2

options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options relaxed_pointer_const
options unsafe_table_lookup = false
options no_global_variables = false
options stack = 4_194_304 // On huge files we need huge stack for jit

module llvm_jit_common shared private

require llvm/daslib/llvm_boost
require llvm/daslib/llvm_dll_utils
require llvm/daslib/llvm_jit_plan public
require llvm/daslib/llvm_jit_cli
require llvm/daslib/llvm_targets

require llvm/daslib/llvm_dsl
require llvm/daslib/llvm_env
require llvm/daslib/llvm_cpu_class
require daslib/ast_boost
require daslib/strings
require daslib/strings_boost
require daslib/defer
require daslib/enum_trait
require daslib/fio
require daslib/option
require jit

let public LLVM_DEBUG_RESULT = false
let public LLVM_DEBUG_EVERYTHING = false

let public LLVM_DEBUG_TRACES = false
let public LLVM_DEBUG_LINE_TRACES = false

let public LLVM_JIT_LOG = false || LLVM_DEBUG_EVERYTHING

let public LLVM_ENABLE_OPT_PASS = LLVM_DEBUG_EVERYTHING ? false : true
let public LLVM_LOG_RESULT = LLVM_DEBUG_EVERYTHING || LLVM_DEBUG_RESULT
let public LLVM_INLINE_THRESHOLD = 1024u // 225u

// This functions should be extern "C" and their name should match with c-name.
let public FN_JIT_EXCEPTION         = "jit_exception"
let public FN_JIT_CALL_OR_FASTCALL  = "jit_call_or_fastcall"
let public FN_JIT_CALL_WITH_CMRES   = "jit_call_with_cmres"
let public FN_JIT_INVOKE_BLOCK      = "jit_invoke_block"
let public FN_JIT_INVOKE_BLOCK_WITH_CMRES = "jit_invoke_block_with_cmres"
let public FN_JIT_STRING_BUILDER    = "jit_string_builder"
let public FN_JIT_STRING_BUILDER_TEMP    = "jit_string_builder_temp"
let public FN_JIT_GET_GLOBAL_MNH    = "jit_get_global_mnh"
let public FN_JIT_GET_SHARED_MNH    = "jit_get_shared_mnh"
// Wasm cross-compile only — handled-type field offset resolved against the TARGET
// runtime's annotation registry (the host's offsetof is wrong when the target C++
// ABI lays the struct out differently; see handled_field_offset_value in llvm_jit.das).
let public FN_JIT_GET_HANDLED_FIELD_OFFSET = "jit_get_handled_field_offset"
let public FN_JIT_CHECK_HANDLED_TYPE_SIZE = "jit_check_handled_type_size"
let public FN_JIT_CHECK_HANDLED_FIELD_OFFSET = "jit_check_handled_field_offset"
let public FN_JIT_HANDLED_ABI_CHECK_REPORT = "jit_handled_abi_check_report"
let public FN_JIT_ALLOC_HEAP        = "jit_alloc_heap"
let public FN_JIT_ALLOC_PERSISTENT  = "jit_alloc_persistent"
let public FN_JIT_FREE_HEAP         = "jit_free_heap"
let public FN_JIT_FREE_PERSISTENT   = "jit_free_persistent"
let public FN_JIT_ARRAY_RESIZE      = "jit_array_resize"
let public FN_JIT_STR_CAT           = "jit_str_cat"
let public FN_JIT_MAKE_BLOCK        = "jit_make_block"
let public FN_JIT_TRY_RECOVER       = "jit_try_recover"
let public FN_JIT_AD_BY_SID         = "jit_ad_by_sid"
let public FN_JIT_DEBUG             = "jit_debug"
let public FN_JIT_ITERATOR_ITERATE  = "jit_iterator_iterate"
let public FN_JIT_ITERATOR_DELETE   = "jit_iterator_delete"
let public FN_JIT_ITERATOR_CLOSE    = "jit_iterator_close"
let public FN_JIT_ITERATOR_FIRST    = "jit_iterator_first"
let public FN_JIT_ITERATOR_NEXT     = "jit_iterator_next"
let public FN_JIT_DEBUG_ENTER       = "jit_debug_enter"
let public FN_JIT_DEBUG_EXIT        = "jit_debug_exit"
let public FN_JIT_DEBUG_LINE        = "jit_debug_line"
let public FN_JIT_AST_TYPEDECL      = "jit_ast_typedecl"
let public FN_JIT_INIT_EXTERN       = "jit_init_extern_function"

var public g_ctx : LLVMContextRef
var public g_prim_t : PrimitiveTypes?
var public g_mod : LLVMOpaqueModule?
var public g_engine : LLVMExecutionEngineRef
var public g_failed : bool = false
//! the module in flight addresses a solid-context global
var public g_solid_globals : bool = false

def public take_solid_globals() : bool {
    let solid = g_solid_globals
    g_solid_globals = false
    return solid
}

//! every instruction of `fn`, block by block
def public for_each_instruction(fn : LLVMOpaqueValue?; blk : block<(inst : LLVMOpaqueValue?)>) {
    var bb = LLVMGetFirstBasicBlock(fn)
    while (bb != null) {
        var inst = LLVMGetFirstInstruction(bb)
        while (inst != null) {
            invoke(blk, inst)
            inst = LLVMGetNextInstruction(inst)
        }
        bb = LLVMGetNextBasicBlock(bb)
    }
}
var public g_errors : array<string>

var public g_t_simFunction : LLVMOpaqueType?
var public g_t_function : LLVMOpaqueType?
var public g_t_lambda : LLVMOpaqueType?
var public g_t_block : LLVMOpaqueType?
var public g_t_jit_function : LLVMOpaqueType?
var public g_t_stack_state : LLVMOpaqueType?
var public g_t_iterator : LLVMOpaqueType?
var public g_t_sequence : LLVMOpaqueType?

var public g_str2v : table<string; LLVMOpaqueValue?>

let private MOD_REF_READ = 1ul
let private MOD_REF_READ_WRITE = 3ul

def private memory_effects(arg_mem, inaccessible_mem, errno_mem, other : uint64) : uint64 => (
    arg_mem | (inaccessible_mem << 2ul) | (errno_mem << 4ul) | (other << 6ul))

class public Attributes {
    noreturn : LLVMOpaqueAttributeRef?
    readonly : LLVMOpaqueAttributeRef?
    optnone : LLVMOpaqueAttributeRef?
    nounwind : LLVMOpaqueAttributeRef?
    willreturn : LLVMOpaqueAttributeRef?
    nocapture : LLVMOpaqueAttributeRef?
    alwaysinline : LLVMOpaqueAttributeRef?
    noinline : LLVMOpaqueAttributeRef?
    hot : LLVMOpaqueAttributeRef?
    cold : LLVMOpaqueAttributeRef?
    mustprogress : LLVMOpaqueAttributeRef?
    minsize : LLVMOpaqueAttributeRef?
    nofree : LLVMOpaqueAttributeRef?
    norecurse : LLVMOpaqueAttributeRef?
    noalias : LLVMOpaqueAttributeRef?
    nonnull : LLVMOpaqueAttributeRef?
    speculatable : LLVMOpaqueAttributeRef?
    sext : LLVMOpaqueAttributeRef?
    zext : LLVMOpaqueAttributeRef?
    // memory(inaccessiblemem: readwrite) — for allocator-style runtime calls
    // LLVM 22 IRMemLocation layout: ArgMem=0(bits 0-1), InaccessibleMem=1(bits 2-3), each ModRef=3 → 3<<2=12
    memory_inaccessible_rw : LLVMOpaqueAttributeRef?
    // memory(argmem: readwrite, inaccessiblemem: readwrite) — for deallocator-style runtime calls
    // 3 | (3 << 2) = 15
    memory_arg_inaccessible_rw : LLVMOpaqueAttributeRef?
    // memory(none) — a pure function of its arguments; the global-offset lookups
    memory_none : LLVMOpaqueAttributeRef?
    memory_read : LLVMOpaqueAttributeRef?
    memory_pure_ctx : LLVMOpaqueAttributeRef?

    def Attributes(ctx : LLVMContextRef) {
        noreturn = ctx |> LLVMGetEnumAttribute("noreturn")
        readonly = ctx |> LLVMGetEnumAttribute("readonly")
        optnone = ctx |> LLVMGetEnumAttribute("optnone")
        nounwind = ctx |> LLVMGetEnumAttribute("nounwind")
        willreturn = ctx |> LLVMGetEnumAttribute("willreturn")
        nocapture = ctx |> LLVMGetEnumAttribute("nocapture")
        alwaysinline = ctx |> LLVMGetEnumAttribute("alwaysinline")
        noinline = ctx |> LLVMGetEnumAttribute("noinline")
        hot = ctx |> LLVMGetEnumAttribute("hot")
        cold = ctx |> LLVMGetEnumAttribute("cold")
        mustprogress = ctx |> LLVMGetEnumAttribute("mustprogress")
        minsize = ctx |> LLVMGetEnumAttribute("minsize")
        nofree = ctx |> LLVMGetEnumAttribute("nofree")
        norecurse = ctx |> LLVMGetEnumAttribute("norecurse")
        noalias = ctx |> LLVMGetEnumAttribute("noalias")
        nonnull = ctx |> LLVMGetEnumAttribute("nonnull")
        speculatable = ctx |> LLVMGetEnumAttribute("speculatable")
        sext = ctx |> LLVMGetEnumAttribute("signext")
        zext = ctx |> LLVMGetEnumAttribute("zeroext")
        let memory_kind = LLVMGetEnumAttributeKindForName("memory")
        memory_inaccessible_rw = LLVMCreateEnumAttribute(ctx, memory_kind, 12ul)
        memory_arg_inaccessible_rw = LLVMCreateEnumAttribute(ctx, memory_kind, 15ul)
        memory_none = LLVMCreateEnumAttribute(ctx, memory_kind, 0ul)
        memory_read = LLVMCreateEnumAttribute(ctx, memory_kind,
            memory_effects(MOD_REF_READ, MOD_REF_READ, MOD_REF_READ, MOD_REF_READ))
        memory_pure_ctx = LLVMCreateEnumAttribute(ctx, memory_kind,
            memory_effects(MOD_REF_READ_WRITE, MOD_REF_READ_WRITE,
                MOD_REF_READ_WRITE, MOD_REF_READ))
    }

}

// Indices MUST match the field order produced by `type_to_llvm_type` for `Type.tArray`
// below, which itself mirrors the C++ `das::Array` layout (data, size, capacity, magic,
// lock, flags, _pad_after_flags).
enum public JIT_ARRAY {
    DATA
    SIZE
    CAPACITY
    MAGIC
    LOCK
    FLAGS
    PAD_AFTER_FLAGS  // mirror Array's trailing alignment pad (see arraytype.h)
}

// Indices MUST match the field order produced by `type_to_llvm_type` for `Type.tTable`
// below — Array base fields, then keys/hashes/tombstones. Keep in sync with the C++
// `das::Table` layout (Table : Array).
enum public JIT_TABLE {
    DATA
    SIZE
    CAPACITY
    MAGIC
    LOCK
    FLAGS
    PAD_AFTER_FLAGS  // matches C++ sizeof(Array)==40 on 32-bit; Table::keys lands at 40
    KEYS
    HASHES
    TOMBSTONES
}

enum public JIT_SIMFUNCTION {
    NAME
    MANGLED_NAME
    CODE
    DEBUG_INFO
    MANGLED_NAME_HASH
    AOT_FUNCTION
    STACK_SIZE
    FLAGS
}

// Must match TABLE_MAX_LINEAR_CAPACITY in runtime_table.h. A packed table has exactly this
// many slots, so the inlined packed find scans a fixed count (and can vectorize it).
let public JIT_TABLE_MAX_LINEAR_CAPACITY = 8

enum public JIT_FUNCTION {
    SIM_FUNCTION
}

enum public JIT_LAMBDA {
    EVAL
    FINALIZER
}

enum public JIT_BLOCK {
    STACK_OFFSET
    ARGUMENTS_OFFSET
    BODY
    AOT_FUNCTION
    JIT_FUNCTION
    FUNCTION_ARGUMENTS
    INFO
}

enum public JIT_SEQUENCE {
    ITERATOR
}


def public set_private_linkage(value : LLVMOpaqueValue?) {
    LLVMSetLinkage(value, LLVMLinkage.LLVMPrivateLinkage)
    LLVMSetUnnamedAddress(value, LLVMUnnamedAddr.LLVMGlobalUnnamedAddr);
}

// nolint:STYLE014
// Address-carrying globals whose NAME is shared across split partitions (interp-call / interop
// fn-ptr globs keyed by mangled name, the jit table-method globs keyed by key type): under
// --jit-split-modules several partition objects define the same symbol, so it takes linkonce_odr
// + an any-comdat and the linker keeps ONE copy. DLLExport storage stays in BOTH modes so
// set_glob_address (GetProcAddress on Windows) finds the slot on cache-hit reload — else #2802
// stale-addr. NEVER SetGlobalConstant on these — DLLExport + non-constant is what stops LLVM
// folding the baked codegen-time address into call sites (fold = ASLR death on cached DLLs).
[arch(at="../ARCHITECTURE_EXE.md#split-exe-slots")]
def public set_shared_export_linkage(val : LLVMOpaqueValue?; name : string) {
    if (g_jit_split_active) {
        LLVMSetLinkage(val, g_jit_split_exe ? LLVMLinkage.LLVMWeakODRLinkage : LLVMLinkage.LLVMLinkOnceODRLinkage)
        if (g_jit_split_exe) {
            g_split_glob_names |> insert(name)
        }
        // Mach-O has NO comdat support - LLVM's checkMachOComdat is a fatal error at emission;
        // linkonce_odr alone dedups there (weak defs). Split is host-only, so host truth suffices.
        if (get_platform_name() != "darwin") {
            var comdat = LLVMGetOrInsertComdat(g_mod, name)
            LLVMSetComdatSelectionKind(comdat, LLVMComdatSelectionKind.LLVMAnyComdatSelectionKind)
            LLVMSetComdat(val, comdat)
        }
    } else {
        LLVMSetLinkage(val, LLVMLinkage.LLVMDLLExportLinkage)
    }
    if (!g_jit_split_exe) {   //! a split exe's entry stores into the slot by name at codegen; nothing looks it up through the export table
        LLVMSetDLLStorageClass(val, LLVMDLLStorageClass.LLVMDLLExportStorageClass)
    }
}

[arch(at="../ARCHITECTURE_EXE.md#split-exe-slots")]
def private get_or_create_table_fn(llvm_module : LLVMOpaqueModule?; types : PrimitiveTypes?; fn_name : DllName; at_type : LLVMOpaqueType?; fn_ptr : void?) {
    var tab_fun = LLVMGetNamedGlobal(llvm_module, fn_name.glob())   //! the entry partition declares a collected slot ahead of its own first use; adding the name again would mint a twin the startup walk never fills
    if (!g_fn_types |> key_exists(fn_name.publ())) {
        g_fn_types[fn_name.publ()] = at_type
    }
    if (tab_fun == null) {
        let at_ptr_type = LLVMPointerType(at_type, 0u)
        tab_fun = LLVMAddGlobal(llvm_module, LLVMPointerType(at_ptr_type, 0u), fn_name.glob())
        if (g_emit_aot_object_globals) {
            // Per-object internal + null; the globinit ctor re-resolves it at load.
            LLVMSetLinkage(tab_fun, LLVMLinkage.LLVMInternalLinkage)
            LLVMSetInitializer(tab_fun, LLVMConstPointerNull(LLVMPointerType(at_ptr_type, 0u)))
        } elif (g_jit_exe_mode && !g_jit_split_exe) {
            set_private_linkage(tab_fun)
            LLVMSetInitializer(tab_fun, LLVMConstPointerNull(LLVMPointerType(at_ptr_type, 0u)))
        } else {
            set_shared_export_linkage(tab_fun, fn_name.glob())
            LLVMSetInitializer(tab_fun, g_jit_split_exe ? LLVMConstPointerNull(LLVMPointerType(at_ptr_type, 0u))
                                                        : types.ConstPtr(fn_ptr |> intptr(), at_ptr_type))
        }
    }
    return tab_fun
}

def public get_table_at(g_builder : LLVMOpaqueBuilder?; llvm_module : LLVMOpaqueModule?; t : Type; var types : PrimitiveTypes?) {
    assume fn_ptr = get_jit_table_at(unsafe(reinterpret<int>(t)))
    // int32_t jit_table_at ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context )
    assume at_type = LLVMFunctionType(types.t_int32,   // int32_t result
        fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(), // table ptr
        base_type_to_llvm_type(t),                           // key
        types.t_int32,                                    // valueTypeSize
        types.LLVMVoidPtrType(),                          // context
        types.LLVMVoidPtrType(),                          // at
    ))
    let fn_name = DllName(FN_JIT_TABLE_AT(t))
    var tab_fun : LLVMOpaqueValue? = get_or_create_table_fn(llvm_module, types, fn_name, at_type, fn_ptr)
    assert(tab_fun != null)
    tab_fun = LLVMBuildLoad2(g_builder, LLVMPointerType(at_type, 0u), tab_fun, "func_at")
    return (tab_fun, at_type)
}

def public get_table_find(g_builder : LLVMOpaqueBuilder?; llvm_module : LLVMOpaqueModule?; t : Type; var types : PrimitiveTypes?) {
    assume fn_ptr = get_jit_table_find(unsafe(reinterpret<int>(t)))
    // int32_t jit_table_find ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context )
    let find_type = LLVMFunctionType(types.t_int32,   // int32_t result
            fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(),   // table ptr
            base_type_to_llvm_type(t),                          // key
            types.t_int32,                                    // valueTypeSize
            types.LLVMVoidPtrType()                 // context
            ))
    let fn_name = DllName(FN_JIT_TABLE_FIND(t))
    var tab_fun : LLVMOpaqueValue? = get_or_create_table_fn(llvm_module, types, fn_name, find_type, fn_ptr)
    assert(tab_fun != null)
    tab_fun = LLVMBuildLoad2(g_builder, LLVMPointerType(find_type, 0u), tab_fun, "func_find")
    return (tab_fun, find_type)
}

def public get_string_table_at_with_hash(g_builder : LLVMOpaqueBuilder?; llvm_module : LLVMOpaqueModule?; var types : PrimitiveTypes?) {
    assume fn_ptr = get_jit_string_table_at_with_hash()
    // int32_t jit_string_table_at_with_hash ( Table * tab, char * key, uint64_t hfn, int32_t valueTypeSize, Context * context, LineInfoArg * at )
    assume at_type = LLVMFunctionType(types.t_int32,   // int32_t result
        fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(), // table ptr
        base_type_to_llvm_type(Type.tString),                // key
        types.t_int64,                                    // precomputed hash
        types.t_int32,                                    // valueTypeSize
        types.LLVMVoidPtrType(),                          // context
        types.LLVMVoidPtrType(),                          // at
    ))
    let fn_name = DllName(FN_JIT_STRING_TABLE_AT_WITH_HASH())
    var tab_fun : LLVMOpaqueValue? = get_or_create_table_fn(llvm_module, types, fn_name, at_type, fn_ptr)
    assert(tab_fun != null)
    tab_fun = LLVMBuildLoad2(g_builder, LLVMPointerType(at_type, 0u), tab_fun, "func_at_wh")
    return (tab_fun, at_type)
}

def public get_string_table_at_after_packed_miss(g_builder : LLVMOpaqueBuilder?; llvm_module : LLVMOpaqueModule?; var types : PrimitiveTypes?) {
    assume fn_ptr = get_jit_string_table_at_after_packed_miss()
    // int32_t jit_string_table_at_after_packed_miss ( Table*, char* key, uint64_t hfn, int32_t valueTypeSize, Context*, LineInfoArg* )
    assume at_type = LLVMFunctionType(types.t_int32,
        fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(), // table ptr
        base_type_to_llvm_type(Type.tString),                // key
        types.t_int64,                                    // precomputed hash
        types.t_int32,                                    // valueTypeSize
        types.LLVMVoidPtrType(),                          // context
        types.LLVMVoidPtrType(),                          // at
    ))
    let fn_name = DllName(FN_JIT_STRING_TABLE_AT_AFTER_PACKED_MISS())
    var tab_fun : LLVMOpaqueValue? = get_or_create_table_fn(llvm_module, types, fn_name, at_type, fn_ptr)
    assert(tab_fun != null)
    tab_fun = LLVMBuildLoad2(g_builder, LLVMPointerType(at_type, 0u), tab_fun, "func_at_apm")
    return (tab_fun, at_type)
}

// Generic Table header (Array base + keys/hashes/tombstones) with pointer fields as void*,
// so it is independent of key/value type. Field order matches JIT_TABLE; used only to read
// capacity / hashes for the inlined packed find.
def public table_header_llvm_type(var _types : PrimitiveTypes?) : LLVMOpaqueType? {
    // padAfterFlags is an explicit slot (8-aligns keys on 32-bit ABIs); hashes is TableHashKey* = uint32*
    return jit_struct_type(type<tuple<data : void?; size : int64; capacity : int64; magic : int; lock : int; flags : int; padAfterFlags : int; keys : void?; hashes : void?; tombstones : int64>>)
}

def public get_table_erase(g_builder : LLVMOpaqueBuilder?; llvm_module : LLVMOpaqueModule?; t : Type; var types : PrimitiveTypes?) {
    let fn_name = DllName(FN_JIT_TABLE_ERASE(t))
    assume fn_ptr = get_jit_table_erase(unsafe(reinterpret<int>(t)))
    // bool jit_table_erase ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context, LineInfoArg * at )
    let erase_type = LLVMFunctionType(types.t_int1,                        // bool result
            fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(),   // table ptr
            base_type_to_llvm_type(t),                          // key
            types.t_int32,                                    // valueTypeSize
            types.LLVMVoidPtrType(),                          // context
            types.LLVMVoidPtrType()                           // at
            ))
    var tab_fun : LLVMOpaqueValue? = get_or_create_table_fn(llvm_module, types, fn_name, erase_type, fn_ptr)
    assert(tab_fun != null)
    tab_fun = LLVMBuildLoad2(g_builder, LLVMPointerType(erase_type, 0u), tab_fun, "func_erase")
    return (tab_fun, erase_type)
}

var public g_fn_types : table<string; LLVMTypeRef>

// Wasm cross-compile only: per (handled-type, field) i32 globals holding the TARGET
// field offset, filled once after initialize_modules() via jit_get_handled_field_offset.
// Keyed by the global name; value carries the (module, type, field) for the init call.
var public g_handled_field_offset_globals : table<string; tuple<modName : string; typeName : string; fieldName : string>>

//! what the JIT emits for: arch, OS, and the features the intrinsic
//! tables and generators may use: a named class's set, else cpuid on
//! a host target, plus DAS_JIT_{X64,ARM64}_FORCE_FEATURES (emit-only)
struct public JitTarget {
    //! the class it builds for - a named one, else its arch's generic
    //! class; the arch picks the platform-intrinsic table, else the
    //! daslang fallback body
    klass : CpuClass
    //! a cross triple: the link brings only that target's runtime
    //! archive - every module is static, none loads dynamically
    cross : bool
    //! the cross triple, "" on a native run
    triple : string
    //! the tier gates came from the running box's cpuid (false: the
    //! generic or forced-only rail)
    host_features : bool
    //! the host platform on a native run, the triple's OS field on a
    //! cross one; gates Linux-only externs
    os_linux : bool
    //! a wasm object for a -pthread runtime: +atomics,+bulk-memory,
    //! which must match the archives
    wasm_threads : bool
    //! the features the intrinsic tables are gated on - the target
    //! arch's alone
    features : CpuFeatures
}

def public target_x64(target : JitTarget) : bool =>
    cpu_class_arch(target.klass) == CpuClass.x86_generic
def public target_aarch64(target : JitTarget) : bool =>
    cpu_class_arch(target.klass) == CpuClass.arm_generic
def public target_wasm(target : JitTarget) : bool =>
    cpu_class_arch(target.klass) == CpuClass.wasm_generic

//! the target converts half <-> float in hardware: any aarch64, x86-64 with F16C
def public target_has_half_converts(target : JitTarget) =>
    target_aarch64(target) || target.features.f16c

def private force_env(target : JitTarget) : string =>
    (target_x64(target) ? g_env_jit.jit_x64_force_features
        : (target_aarch64(target) ? g_env_jit.jit_arm64_force_features : ""))

// the baseline class's append first, the env's forced names after it (the last spelling of a name wins in LLVM)
def private join_plus_features(base, forced : string) : string =>
    empty(base) ? forced : (empty(forced) ? base : "{base},{forced}")

def private host_llvm_features() : CpuFeatures {
    let feats = LLVMGetHostCPUFeatures()
    let on <- [for (part in split(feats, ",")); slice(part, 1); where starts_with(part, "+")]
    LLVMDisposeMessage(feats)
    return cpu_features_of(join(on, ";"))
}

[arch(at = "../ARCHITECTURE_CPU_CLASSES.md#cpu-classes")]
def public forced_plus_features(target : JitTarget) : string {
    let plus <- [for (part in split(force_env(target), ",")); "+{part}"; where !empty(part)]
    return join_plus_features(cpu_features_text(jit_cpu_class_row(target.klass).features, ",", "+"),
        join(plus, ","))
}

// --jit-check-abi: startup sweep comparing every used handled-type's HOST-baked size + field
// offsets against the TARGET runtime annotation, aborting (reporting ALL divergences) on any
// mismatch. Diagnoses the MSVC-host vs wasm/clang-target C++ ABI layout class. Opt-in, off by default.
var public g_jit_check_handled_abi = false

[arch(at = "../ARCHITECTURE_TARGET_FEATURES.md#wasm-feature-string")]
def private wasm_target_features(target : JitTarget) : string =>
    (target.wasm_threads
        ? "+simd128,+nontrapping-fptoint,+atomics,+bulk-memory"
        : "+simd128,+nontrapping-fptoint")

//! the features every target machine of the arch carries: the wasm
//! runtime archive's set, or the arch's forced features - the ones
//! make_jit_target gates the intrinsic tables on
def public target_plus_features(target : JitTarget) : string =>
    target_wasm(target) ? wasm_target_features(target) : forced_plus_features(target)

def public LLVMAddFunctionWithType(mod : LLVMModuleRef; name : string; typ : LLVMTypeRef) {
    var rv = LLVMAddFunction(mod, name, typ)
    g_fn_types[name] = typ
    // uwtable(async): -O3 function-attrs infers `nounwind` through the runtime-helper
    // attributes and Win64 codegen then omits .pdata, so a C++-EH panic crossing such a frame kills
    // the process. Forced regardless (ignored on declarations); skipped on longjmp hosts.
    if (das_is_exceptions_enabled()) {
        rv |> LLVMAddAttributeToFunction(LLVMCreateEnumAttribute(g_ctx, LLVMGetEnumAttributeKindForName("uwtable"), 2ul))
    }
    return rv
}

//! the optimizer reads the MODULE triple and data layout - its library-call table, type sizes and alignments, an
//! instrumentation pass's shadow mapping - while codegen reads the target machine's; a module built for the host
//! carries neither, so the target machine's go on it before the passes. A cross target's module has its own already
def public stamp_module_target(mod : LLVMOpaqueModule?; tm : LLVMTargetMachineRef) {
    return if (!empty(LLVMGetTarget(mod)))
    let triple = LLVMGetTargetMachineTriple(tm)
    LLVMSetTarget(mod, triple)
    LLVMDisposeMessage(triple)
    let dl = LLVMCreateTargetDataLayout(tm)
    LLVMSetModuleDataLayout(mod, dl)
    LLVMDisposeTargetData(dl)
}

//! asan instruments only the functions that carry `sanitize_address`
def public mark_sanitize_address(mod : LLVMOpaqueModule?) {
    let attr = LLVMCreateEnumAttribute(LLVMGetModuleContext(mod), LLVMGetEnumAttributeKindForName("sanitize_address"), 0ul)
    var fn = LLVMGetFirstFunction(mod)
    while (fn != null) {
        LLVMAddAttributeToFunction(fn, attr)
        fn = LLVMGetNextFunction(fn)
    }
}

// ===== extern-helper declaration =====
// jit_add_extern declares one C++ runtime helper (add + global-mapping + function attrs, plus optional
// per-arg attrs). Pair with jit_fn_type(signature) (llvm_dsl) to derive `typ` from a das signature.

def public jit_extern_type(t : Type) : LLVMTypeRef =>
    t == Type.tVoid ? g_prim_t.t_void : base_type_to_llvm_type(t)

def public declare_extern_fn(name : string; typ : LLVMTypeRef) : LLVMOpaqueValue? {
    var fn = LLVMGetNamedFunction(g_mod, name)
    if (fn == null) {
        fn = LLVMAddFunctionWithType(g_mod, name, typ)
    }
    return fn
}


def public jit_add_extern(name : string; typ : LLVMTypeRef; fnAddr : void?) {
    var f = LLVMAddFunctionWithType(g_mod, name, typ)
    LLVMAddGlobalMapping(g_engine, f, fnAddr)
    return f
}

def public jit_add_extern(name : string; typ : LLVMTypeRef; fnAddr : void?; attrs : LLVMOpaqueAttributeRef? []) {
    var f = jit_add_extern(name, typ, fnAddr)
    LLVMAddAttributesToFunction(f, attrs)
    return f
}

def public jit_add_extern(name : string; typ : LLVMTypeRef; fnAddr : void?; argAttr : tuple<LLVMOpaqueAttributeRef?; array<int>>) {
    var f = jit_add_extern(name, typ, fnAddr)
    for (i in argAttr._1) {
        LLVMAddAttributeToFunctionArgument(f, uint(i), argAttr._0)
    }
    return f
}

// argAttr: (attribute, argument-indices) — e.g. (nocapture, [0, 1, 2, 3]) or (nocapture, [1]).
def public jit_add_extern(name : string; typ : LLVMTypeRef; fnAddr : void?; attrs : LLVMOpaqueAttributeRef? []; argAttr : tuple<LLVMOpaqueAttributeRef?; array<int>>) {
    var f = jit_add_extern(name, typ, fnAddr, attrs)
    for (i in argAttr._1) {
        LLVMAddAttributeToFunctionArgument(f, uint(i), argAttr._0)
    }
    return f
}

var g_jit_clopts_parsed = false

// Set while emitting an offline AOT object: builtin-accessor globals are emitted
// internal + null and re-resolved by the emit_aot_object_globinit load ctor,
// instead of a baked codegen-process address. run_jit sets/clears it around codegen.
var public g_emit_aot_object_globals = false

//! a shipped artifact (-exe or -lib) is being emitted:
//! $::is_standalone_exe folds to true, no accessor slot is exported
var public g_jit_exe_mode = false

// --jit-split-modules is active: per-das-module partitions, one .o each, one linked DLL. Flips
// function impls to external-hidden (cross-partition calls) and shared-name globs to linkonce_odr.
// run_jit sets/clears it around the run; false = today's monolithic emission, byte-identical.
var public g_jit_split_active = false
//! a split -exe's partitions are being emitted: every accessor slot is one shared null copy, its name collected in g_split_glob_names by set_shared_export_linkage
var public g_jit_split_exe = false
var public g_split_glob_names : table<string>

// Get-or-add das_aot_get_func_by_mnh and build a call resolving `mnh` against `ctx_arg` to the host's
// SimFunction* (void*). Shared by the aot-object register path and the globinit ctor.
def public build_aot_get_func_by_mnh(b : LLVMBuilderRef; types : PrimitiveTypes?; mnh : uint64; ctx_arg : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let gfType = LLVMFunctionType(types.LLVMVoidPtrType(),
        fixed_array<LLVMTypeRef>(types.t_int64, types.LLVMVoidPtrType()))
    var gfFn = LLVMGetNamedFunction(g_mod, "das_aot_get_func_by_mnh")
    if (gfFn == null) {
        gfFn = LLVMAddFunctionWithType(g_mod, "das_aot_get_func_by_mnh", gfType)
    }
    var gf_args = array<LLVMValueRef>(types.ConstI64(mnh), ctx_arg)
    return LLVMBuildCall2(b, gfType, gfFn, gf_args, "")
}

// LLVM's unroll / partial-unroll / unroll-and-jam cost-model thresholds are compiled-in cl::opt
// globals; daslang's JIT path never runs cl::ParseCommandLineOptions, so without these the O3
// pipeline lands different (worse) IR shapes than `opt` on the same input (bisected on dasProfile).
[macro_function]
def public init_jit_clopts() {
    return if (g_jit_clopts_parsed)
    g_jit_clopts_parsed = true
    var argv <- [
        "daslang-jit",
        "-unroll-threshold=400",
        "-unroll-partial-threshold=600",
        "-unroll-max-count=16",
        "-unroll-allow-partial=true",
        "-enable-unroll-and-jam=true",
        "-allow-unroll-and-jam=true"
    ]
    let cli = jit_cli_options_or_default()
    for (flag, llvm_opt, regex in
            fixed_array("jit-remarks", "jit-remarks-missed", "jit-remarks-analysis"),
            fixed_array("pass-remarks", "pass-remarks-missed", "pass-remarks-analysis"),
            fixed_array(cli.remarks, cli.remarks_missed, cli.remarks_analysis)) {
        if (regex |> is_some) {
            let passes = regex |> unwrap
            argv |> push("-{llvm_opt}={passes}")
            to_log(LOG_INFO, "LLVM JIT: --{flag}={passes} prints LLVM's {llvm_opt} to stderr\n")
        }
    }
    LLVMParseCommandLineOptions(length(argv),
        unsafe(addr<string const?>(argv[0])),
        "daslang JIT cl::opts")
}

// host_features=false targets a GENERIC CPU: cpuid gates stay off, since leaked F16C emits libcalls.
[arch(at = "../ARCHITECTURE_TARGET_FEATURES.md#aarch64-feature-truth"), arch(at = "../ARCHITECTURE_TARGET_FEATURES.md#x64-tier-gates")]
def public make_jit_target(target_triple : string; baseline : CpuClass;
                           host_features_asked : bool = true;
                           wasm_threads : bool = false) : JitTarget {
    var t : JitTarget
    // a baseline class replaces the running box: its set is the only
    // tier truth, whatever the caller asked
    let arch = cpu_arch_of(empty(target_triple) ? get_architecture_name() : target_triple)
    t.klass = baseline == CpuClass.none ? arch : baseline
    let host_features = host_features_asked && baseline == CpuClass.none
    t.host_features = host_features
    t.cross = !empty(target_triple)
    t.triple = target_triple
    t.wasm_threads = wasm_threads
    let on_box = empty(target_triple) && host_features
    var f = target_features(t.klass, on_box, cpu_features_of(force_env(t)))
    if (on_box) {
        let native = cpu_features_native()
        f.avx2 = target_x64(t) && native.avx2
        f.f16c = target_x64(t) && native.f16c
        if (target_aarch64(t)) {
            f |= host_llvm_features()
            // Generic-exe builds stay off fullfp16 - the promote/narrow path
            // is ARMv8.0-legal, fullfp16 is not
            f.fullfp16 = f.fullfp16 || get_platform_name() == "darwin"
        }
    }
    t.features = ((target_x64(t) || target_aarch64(t)) ? f & cpu_arch_features(t.klass)
        : CpuFeatures(0u))
    t.os_linux = (empty(target_triple)
        ? get_platform_name() == "linux"
        : find(target_triple, "linux") >= 0)
    return t
}

//! `target` with its tier gates at one CPU class's set - what a
//! fat-mode clone's generator emits for, not the baseline the process
//! targets
[arch(at = "../ARCHITECTURE_CPU_CLASSES.md#fat-mode")]
def public class_target(target : JitTarget; klass : CpuClass) : JitTarget {
    var t = target
    let sameArch = cpu_class_arch(klass) == cpu_class_arch(t.klass)
    t.klass = sameArch ? klass : t.klass
    t.features = ((sameArch && (target_x64(t) || target_aarch64(t)))
        ? jit_cpu_class_row(klass).features : CpuFeatures(0u))
    return t
}

[macro_function]
def public init_jit(target : JitTarget; cg_opt_level : uint; target_triple : string = "") {
    // also prevent double init (g_mod != null)
    return if (is_in_completion() || is_compiling_macros() || g_mod != null)
    init_jit_state(target, cg_opt_level, target_triple, "llvm_jit_module")
}

// nolint:STYLE014 - the overwrite-without-disposal contract below is load-bearing
// One fresh emitter state: context, module (named module_name), MCJIT engine, PrimitiveTypes,
// per-module caches, and the full runtime-extern declaration set. init_jit guards + delegates here.
// The --jit-split-modules driver calls it directly, once per partition, OVERWRITING the g_* state
// WITHOUT disposal - the previous partition's state deliberately stays live in g_split_parts until
// the driver's phase C (its module is still queued for the emit pool); the first partition that
// actually EMITS rides run_jit's init_jit. Any other caller owns the previous state's teardown or leaks it.
[macro_function]
def public init_jit_state(target : JitTarget;  // nolint:STYLE038 - order-coupled LLVM setup calls
                          cg_opt_level : uint; target_triple : string; module_name : string) {
    LLVMLinkInMCJIT()
    LLVMInitializeAllTargetInfos()
    LLVMInitializeAllTargets()
    LLVMInitializeAllTargetMCs()
    LLVMInitializeAllAsmPrinters()
    init_jit_clopts()
    g_ctx = LLVMContextCreate()
    g_prim_t = new PrimitiveTypes(g_ctx)
    g_failed = false
    g_solid_globals = false
    g_fn_types |> clear()
    g_handled_field_offset_globals |> clear()
    g_str2v |> clear()
    g_mod = LLVMModuleCreateWithNameInContext(module_name, g_ctx)
    LLVMCreateJITCompilerForModule(g_engine, g_mod, cg_opt_level)
    // Cross-compile targets (wasm32, etc.): pin the module's data layout to the TARGET's, not the
    // host's, BEFORE any IR is built — LLVM constant-folds GEPs against the current layout, so a
    // host/target pointer-size mismatch bakes wrong strides and the loop never terminates.
    if (!(target_triple |> empty())) {
        // wasm32 target isn't always part of LLVMInitializeAllTargets in
        // prebuilt LLVM.dll — call the explicit wasm initializer when needed.
        if (target_wasm(target)) {
            LLVMInitializeWasmTarget()
        }
        // +simd128 / +nontrapping-fptoint matches the wasm runtime archive (web/CMakeLists.txt),
        // so vec4f returns are v128-in-register on both sides. Meaningless (and a possible
        // target-machine failure) on other cross triples, except x64 cross, where the force env rules.
        with_target_machine(target_triple, "", target_plus_features(target),
                            cg_opt_level) $(targetMachine : LLVMTargetMachineRef) {
            let dl = LLVMCreateTargetDataLayout(targetMachine)
            LLVMSetModuleDataLayout(g_mod, dl)
            LLVMSetTarget(g_mod, target_triple)
            LLVMDisposeTargetData(dl)
        }
    }
    static_if (LLVM_DEBUG_EVERYTHING) {
        let hostcpu = LLVMGetHostCPUName()
        let features = LLVMGetHostCPUFeatures()
        to_log(LOG_DEBUG, "LLVM JIT: host cpu: {hostcpu}\n")
        to_log(LOG_DEBUG, "LLVM JIT: host cpu features: {features}\n")
        LLVMDisposeMessage(hostcpu)
        LLVMDisposeMessage(features)
    }
    let attrs = new Attributes(g_ctx)
    assume noreturn = attrs.noreturn
    assume readonly = attrs.readonly
    assume nounwind = attrs.nounwind
    assume willreturn = attrs.willreturn
    assume nocapture = attrs.nocapture
    assume cold = attrs.cold
    assume nofree = attrs.nofree
    assume memory_inaccessible_rw = attrs.memory_inaccessible_rw
    assume memory_arg_inaccessible_rw = attrs.memory_arg_inaccessible_rw
    assume memory_none = attrs.memory_none
    assume speculatable = attrs.speculatable
    build_jit_types()

    // cold: opt sinks the failure block off the hot path; pairs with noreturn.
    jit_add_extern(FN_JIT_EXCEPTION, jit_fn_type($(text, ctx, at : void?) : void {}),
        get_jit_exception(), fixed_array(noreturn, cold), (nocapture, [0, 1]))
    g_t_jit_function = jit_fn_type($(ctx, args, cmres : void?) : float4 {})
    jit_add_extern(FN_JIT_CALL_OR_FASTCALL, jit_fn_type($(func, args, ctx : void?) : float4 {}),
        get_jit_call_or_fastcall(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_CALL_WITH_CMRES, jit_fn_type($(func, args, cmres, ctx : void?) : float4 {}),
        get_jit_call_with_cmres(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2, 3]))
    jit_add_extern(FN_JIT_INVOKE_BLOCK, jit_fn_type($(blk, args, ctx : void?) : float4 {}),
        get_jit_invoke_block(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_INVOKE_BLOCK_WITH_CMRES, jit_fn_type($(blk, args, cmres, ctx : void?) : float4 {}),
        get_jit_invoke_block_with_cmres(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2, 3]))
    jit_add_extern(FN_JIT_STRING_BUILDER,
        jit_fn_type($(ctx : void?; nArgs : int; types, at, args : void?) : void? {}),
        get_jit_string_builder(), fixed_array(nounwind, willreturn))
    jit_add_extern(FN_JIT_STRING_BUILDER_TEMP,
        jit_fn_type($(ctx : void?; nArgs : int; types, at, args : void?) : void? {}),
        get_jit_string_builder_temp(), fixed_array(nounwind, willreturn))
    jit_add_extern(FN_JIT_GET_GLOBAL_MNH, jit_fn_type($(mnh : int64; ctx : void?) : void? {}),
        get_jit_get_global_mnh(), fixed_array(nounwind, willreturn, memory_none, speculatable),
        (nocapture, [1]))
    jit_add_extern(FN_JIT_GET_SHARED_MNH, jit_fn_type($(mnh : int64; ctx : void?) : void? {}),
        get_jit_get_shared_mnh(), fixed_array(nounwind, willreturn, memory_none, speculatable),
        (nocapture, [1]))
    // Wasm-only: a handled (C++) field offset from the TARGET
    // runtime's annotation registry, into per-field globals at init;
    // declared unconditionally, used only on a wasm target
    jit_add_extern(FN_JIT_GET_HANDLED_FIELD_OFFSET,
        jit_fn_type($(modName, typeName, field : void?) : int {}),
        get_jit_get_handled_field_offset(), fixed_array(nounwind, willreturn))
    // --jit-check-abi safe-check trio (declared unconditionally; called only when g_jit_check_handled_abi).
    // Host bakes its layout into the call args; these resolve the TARGET annotation and record/abort on mismatch.
    jit_add_extern(FN_JIT_CHECK_HANDLED_TYPE_SIZE,
        jit_fn_type($(modName, typeName : void?; hostSize : int) : void {}),
        get_jit_check_handled_type_size(), fixed_array(nounwind, willreturn))
    jit_add_extern(FN_JIT_CHECK_HANDLED_FIELD_OFFSET,
        jit_fn_type($(modName, typeName, field : void?; hostOffset : int) : void {}),
        get_jit_check_handled_field_offset(), fixed_array(nounwind, willreturn))
    // NO willreturn: this aborts (DAS_FATAL_ERROR) on any mismatch, so it may not return normally.
    jit_add_extern(FN_JIT_HANDLED_ABI_CHECK_REPORT, jit_fn_type($() : void {}),
        get_jit_handled_abi_check_report(), fixed_array(nounwind))
    var alloc_heap = jit_add_extern(FN_JIT_ALLOC_HEAP, jit_fn_type($(size : int; ctx : void?) : void? {}),
        get_jit_alloc_heap(), fixed_array(nounwind, willreturn, nofree, memory_inaccessible_rw), (nocapture, [1]))
    var alloc_persistent = jit_add_extern(FN_JIT_ALLOC_PERSISTENT, jit_fn_type($(size : int; ctx : void?) : void? {}),
        get_jit_alloc_persistent(), fixed_array(nounwind, willreturn, nofree, memory_inaccessible_rw), (nocapture, [1]))
    //! the object size LLVM's bounds checks read off the call; a vec3 load past a 12-byte object would be UB against it
    if (!LLVM_JIT_ALLOW_UNALIGNED_VECTOR_READ_OUT_OF_BOUNDS) {
        for (alloc in fixed_array(alloc_heap, alloc_persistent)) {
            LLVMAddAttributeToFunction(alloc, LLVMCreateEnumAttribute(g_ctx, LLVMGetEnumAttributeKindForName("allocsize"), 0xFFFFFFFFul))
        }
    }
    jit_add_extern(FN_JIT_FREE_HEAP, jit_fn_type($(bytes : void?; size : int; ctx : void?) : void {}),
        get_jit_free_heap(), fixed_array(nounwind, willreturn, memory_arg_inaccessible_rw), (nocapture, [0, 2]))
    jit_add_extern(FN_JIT_FREE_PERSISTENT, jit_fn_type($(bytes, ctx : void?) : void {}),
        get_jit_free_persistent(), fixed_array(nounwind, willreturn, memory_arg_inaccessible_rw), (nocapture, [0, 1]))
    jit_add_extern(FN_JIT_ARRAY_RESIZE, jit_fn_type($(arr : void?; newSize, stride : int; ctx, at : void?) : void {}),
        get_jit_array_resize(), fixed_array(nounwind, willreturn), (nocapture, [0]))
    jit_add_extern(FN_JIT_STR_CAT, jit_fn_type($(sA, sB, ctx, at : void?) : void? {}),
        get_jit_str_cat(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_MAKE_BLOCK,
        jit_fn_type($(blk : void?; argStackTop : int; ad : int64; bodyNode, jitImpl, funcInfo, at, ctx : void?) : void {}),
        get_jit_make_block(), fixed_array(nounwind, willreturn), (nocapture, [0, 3, 4, 5, 6]))
    jit_add_extern(FN_JIT_TRY_RECOVER,
        jit_fn_type($(tryBlock, catchBlock, at, ctx : void?) : void {}),
        get_jit_try_recover(), (nocapture, [0, 1, 2, 3]))
    jit_add_extern(FN_JIT_AD_BY_SID, jit_fn_type($(sid : int64; ctx : void?) : int64 {}),
        get_jit_ad_by_sid(), fixed_array(nounwind, willreturn), (nocapture, [1]))
    jit_add_extern(FN_JIT_DEBUG, jit_fn_type($(res : float4; typeInfo, message, ctx, at : void?) : void {}),
        get_jit_debug(), fixed_array(nounwind, willreturn), (nocapture, [1, 2, 3, 4]))
    jit_add_extern(FN_JIT_ITERATOR_ITERATE, jit_fn_type($(it, data, ctx : void?) : bool {}),
        get_jit_iterator_iterate(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_ITERATOR_DELETE, jit_fn_type($(it, ctx : void?) : void {}),
        get_jit_iterator_delete(), fixed_array(nounwind, willreturn), (nocapture, [0, 1]))
    jit_add_extern(FN_JIT_ITERATOR_FIRST, jit_fn_type($(it, data, ctx, at : void?) : bool {}),
        get_jit_iterator_first(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2, 3]))
    jit_add_extern(FN_JIT_ITERATOR_NEXT, jit_fn_type($(it, data, ctx, at : void?) : bool {}),
        get_jit_iterator_next(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2, 3]))
    // jit_simnode_interop / jit_free_simnode_interop: literal names, mapping only, no attrs — kept hand-built.
    // void jit_simnode_interop ( void * ptr, int argCount, TypeInfo ** types )
    let simnode_interop_type = LLVMFunctionType(g_prim_t.t_void,
        fixed_array<LLVMTypeRef>(
            g_prim_t.LLVMVoidPtrType(), // ptr (placement-new buffer for SimNode_AotInteropBase)
            g_prim_t.t_int32,           // argCount
            g_prim_t.LLVMVoidPtrType()  // types
        )
    )
    var jit_simnode_interop = LLVMAddFunctionWithType(
        g_mod, "jit_simnode_interop", simnode_interop_type
    )
    LLVMAddGlobalMapping(g_engine, jit_simnode_interop, get_jit_simnode_interop())
    let simnode_free_interop_type = LLVMFunctionType(g_prim_t.t_void,
        fixed_array<LLVMTypeRef>(
            g_prim_t.LLVMVoidPtrType(), // Node
        )
    )
    var jit_free_simnode_interop = LLVMAddFunctionWithType(
        g_mod, "jit_free_simnode_interop", simnode_free_interop_type
    )
    LLVMAddGlobalMapping(g_engine, jit_free_simnode_interop, get_jit_free_simnode_interop())
    jit_add_extern(FN_JIT_ITERATOR_CLOSE, jit_fn_type($(it, data, ctx : void?) : void {}),
        get_jit_iterator_close(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_DEBUG_ENTER, jit_fn_type($(message, ctx, at : void?) : void {}),
        get_jit_debug_enter(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_DEBUG_EXIT, jit_fn_type($(message, ctx, at : void?) : void {}),
        get_jit_debug_exit(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_DEBUG_LINE, jit_fn_type($(message, ctx, at : void?) : void {}),
        get_jit_debug_line(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
    jit_add_extern(FN_JIT_AST_TYPEDECL, jit_fn_type($(hash : int64; ctx, at : void?) : void? {}),
        get_jit_ast_typedecl(), fixed_array(nounwind, willreturn), (nocapture, [1, 2]))
    jit_add_extern(FN_JIT_INIT_EXTERN, jit_fn_type($(moduleName, funcMangledName, dllGlobal : void?) : void {}),
        get_jit_init_extern_function(), fixed_array(nounwind, willreturn), (nocapture, [0, 1, 2]))
}

def private build_jit_types {
    g_t_simFunction = jit_struct_type(type<tuple<name : void?; mangledName : void?; code : void?; debugInfo : void?; mangledNameHash : int64; aotFunction : void?; stackSize : int; flags : int>>)
    g_t_function = jit_struct_type(type<tuple<func : void?>>)
    g_t_lambda = jit_struct_type(type<tuple<lambdaFn : void?; finalizeFn : void?>>)
    g_t_block = g_prim_t |> StructType(build_block_type())
    g_t_stack_state = jit_struct_type(type<tuple<ep : void?; sp : void?>>)
    g_t_iterator = jit_struct_type(type<tuple<vtable : void?; isOpen : bool>>)
    g_t_sequence = jit_struct_type(type<tuple<iter : void?>>)
}

def public build_block_type {
    return <- [
        g_prim_t.t_int32,                                // uint32_t    stackOffset;
        g_prim_t.t_int32,                                // uint32_t    argumentsOffset;
        g_prim_t.LLVMVoidPtrType(),                      // SimNode *   body;
        g_prim_t.LLVMVoidPtrType(),                      // void *      aotFunction;
        g_prim_t.LLVMVoidPtrType(),                      // void *      jitFunction;
        LLVMPointerType(g_prim_t.LLVMFloat4Type(), 0u),  // vec4f *     functionArguments;
        g_prim_t.LLVMVoidPtrType()                       // FuncInfo *  info;
    ]
}

// opt_level 0..3 → LLVMCodeGenOptLevel. use_host_cpu=true bakes the host CPU+features (JIT /
// DLL-cache, host-only artifacts); false = generic baseline for redistributable exe/dll. Build a
// TargetMachine for a triple/cpu/features — caller owns them; we do NOT LLVMDisposeMessage (plain das strings).
[macro_function]
def public create_target_machine(triple, cpu, features : string; opt_level : uint) : LLVMTargetMachineRef {
    var target : LLVMTarget?
    let rc = LLVMGetTargetFromTriple(triple, unsafe(addr(target)), null)
    if (rc != 0 || target == null) {
        panic("LLVM: unknown / unsupported target triple '{triple}'")
    }

    let cg_level = (
        (opt_level >= 3u) ? LLVMCodeGenOptLevel.LLVMCodeGenLevelAggressive
      : (opt_level >= 2u) ? LLVMCodeGenOptLevel.LLVMCodeGenLevelDefault
      : (opt_level >= 1u) ? LLVMCodeGenOptLevel.LLVMCodeGenLevelLess
                          : LLVMCodeGenOptLevel.LLVMCodeGenLevelNone)

    return LLVMCreateTargetMachine(
        target,
        triple,
        cpu,
        features,
        cg_level,
        LLVMRelocMode.LLVMRelocPIC, // PIC in order to have shared object
        LLVMCodeModel.Default
    )
}

[macro_function]
def public with_target_machine(triple, cpu, features : string; opt_level : uint;
                               blk : block<(LLVMTargetMachineRef) : void>) {
    let targetMachine = create_target_machine(triple, cpu, features, opt_level)
    invoke(blk, targetMachine)
    LLVMDisposeTargetMachine(targetMachine)
}

[arch(at = "../ARCHITECTURE_TARGET_FEATURES.md#wasm-feature-string")]
def private pass_machine_cpu_features(target : JitTarget; use_host_cpu : bool
                                      ) : tuple<cpu : string; features : string> {
    if (target_wasm(target)) return (cpu = "", features = "")
    return default_target_cpu_features(target, use_host_cpu)
}

[macro_function, arch(at = "../ARCHITECTURE_TARGET_FEATURES.md#aarch64-feature-truth")]
def public create_default_target_machine(target : JitTarget; opt_level : uint;
                                         use_host_cpu : bool) : LLVMTargetMachineRef {
    LLVMInitializeAllTargetInfos()
    LLVMInitializeAllTargets()
    LLVMInitializeAllTargetMCs()
    LLVMInitializeAllAsmPrinters()

    if (target.cross && !target_wasm(target)) {
        let cross_machine = default_target_cpu_features(target, false)
        return create_target_machine(target.triple, cross_machine.cpu, cross_machine.features,
            opt_level)
    }
    // host_jit_triple() returns the daslang-host toolchain's triple when it differs from LLVM.dll's
    // default (clang-mingw64: prebuilt MSVC clang-cl LLVM.dll defaults to x86_64-pc-windows-msvc,
    // emitting MSVC-ABI .o that mingw's linker can't satisfy). Empty = use LLVM default.
    let host_triple = host_jit_triple()
    let triple_owned_by_llvm = empty(host_triple)
    let triple_msg = triple_owned_by_llvm ? LLVMGetDefaultTargetTriple() : host_triple
    let machine = pass_machine_cpu_features(target, use_host_cpu)
    let targetMachine = create_target_machine(triple_msg, machine.cpu, machine.features, opt_level)

    if (triple_owned_by_llvm) {
        LLVMDisposeMessage(triple_msg)
    }
    return targetMachine
}

//! The cpu name and feature string the default target machine is built from: the host's under `use_host_cpu`,
//! else the baseline class's cpu (LLVM's default with no class) with the forced features - what a function's
//! target attributes carry where no machine is on hand (the in-memory engine, an LTO link's backend)
def public default_target_cpu_features(target : JitTarget; use_host_cpu : bool
                                       ) : tuple<cpu : string; features : string> {
    if (use_host_cpu) {
        return host_machine_cpu_features(target)
    }
    let feats = target_plus_features(target)
    let env_forced = force_env(target)
    let cpu = jit_cpu_class_row(target.klass).cpu
    if (!empty(env_forced)) {
        let env_name = (target_x64(target) ? "DAS_JIT_X64_FORCE_FEATURES"
            : "DAS_JIT_ARM64_FORCE_FEATURES")
        to_log(LOG_INFO, "LLVM JIT: {env_name} appends {env_forced} to the machine {cpu}\n")
    }
    return (cpu = cpu, features = feats)
}

//! The host machine's cpu name and feature string as the host-cpu target machine is built from
//! them: LLVM's detection, plus dotprod / i8mm on aarch64 and the forced features of the arch.
def public host_machine_cpu_features(target : JitTarget) : tuple<cpu : string; features : string> {
    let cpu_msg = LLVMGetHostCPUName()
    let features_msg = LLVMGetHostCPUFeatures()
    var feats = "{features_msg}"
    if (target_aarch64(target)) {
        feats = empty(feats) ? "+dotprod" : "{feats},+dotprod"
        if (cpu_supports("i8mm")) {
            feats = "{feats},+i8mm"
        }
    }
    // a forced feature's artifact cache-hits on the target box without a linker there
    let forced = target_plus_features(target)
    if (!empty(forced)) {
        feats = empty(feats) ? forced : "{feats},{forced}"
    }
    let cpu = "{cpu_msg}"
    LLVMDisposeMessage(cpu_msg)
    LLVMDisposeMessage(features_msg)
    return (cpu = cpu, features = feats)
}

[arch(at = "../ARCHITECTURE_TARGET_FEATURES.md#aarch64-feature-truth")]
def public stamp_default_target_attrs(target : JitTarget; m : LLVMOpaqueModule?;
                                      use_host_cpu : bool) {
    //! the in-memory engine's functions carry the default machine's cpu
    //! and features - the host's, or a baseline class's - as attributes
    let machine = default_target_cpu_features(target, use_host_cpu)
    stamp_target_attrs(m, machine.cpu, machine.features)
}

//! every defined function of `m` carries `cpu` and `features` as its target attributes; an empty string stamps nothing
[arch(at="../ARCHITECTURE_EXE.md#split-exe-lto")]
def public stamp_target_attrs(m : LLVMOpaqueModule?; cpu, features : string) {
    var fn = LLVMGetFirstFunction(m)
    while (fn != null) {
        //! a function that already names its cpu (a fat-mode clone lowered for its class) keeps it: the stamp is the module default, never an override
        if (LLVMIsDeclaration(fn) == 0 && LLVMGetStringAttributeAtIndex(fn, 0xFFFFFFFFu, "target-cpu", 10u) == null) {
            if (!empty(cpu)) {
                LLVMAddTargetDependentFunctionAttr(fn, "target-cpu", cpu)
            }
            if (!empty(features)) {
                LLVMAddTargetDependentFunctionAttr(fn, "target-features", features)
            }
        }
        fn = LLVMGetNextFunction(fn)
    }
}

[macro_function]
def public with_default_target_machine(target : JitTarget; opt_level : uint; use_host_cpu : bool;
                                       blk : block<(LLVMTargetMachineRef) : void>) {
    let targetMachine = create_default_target_machine(target, opt_level, use_host_cpu)
    invoke(blk, targetMachine)
    LLVMDisposeTargetMachine(targetMachine)
}

// Offline AOT: emit a native object only, no linker step (jit_emit_object path).
// use_host_cpu=false (default) targets a generic CPU so the .o is portable; true bakes host features.

def public emit_object_only(target : JitTarget; mod : LLVMOpaqueModule?; out_path : string;
                            use_host_cpu : bool = false) : bool {
    var ok = true
    // Codegen level 3 deliberate: shipped artifacts, no cache guard (ARCHITECTURE.md 1.2)
    with_default_target_machine(target, 3u, use_host_cpu) $(targetMachine : LLVMTargetMachineRef) {
        // A module with no triple and no layout leaves the object to whatever the backend
        // assumes, which need not be what this target machine emits.
        let dl = LLVMCreateTargetDataLayout(targetMachine)
        LLVMSetModuleDataLayout(mod, dl)
        let mod_triple = LLVMGetDefaultTargetTriple()
        LLVMSetTarget(mod, mod_triple)
        LLVMDisposeMessage(mod_triple)
        LLVMDisposeTargetData(dl)
        let error : string?
        let file = get_platform_name() == "windows" ? "{out_path}.obj" : artifact_path(out_path, JitArtifact.object)
        let filetype = LLVMCodeGenFileType.LLVMObjectFile
        let failed = LLVMTargetMachineEmitToFile(targetMachine, mod, file, filetype, error)
        if (failed != 0) {
            to_log(LOG_ERROR, "emit_object_only: LLVMTargetMachineEmitToFile failed for {file}\n")
            ok = false
        }
    }
    return ok
}

[arch(at="../ARCHITECTURE_EXE.md#split-exe-lto")]
//! the LTO link's flags at the run's opt level: regular LTO (one merged module, its optimization serial - the bitcode carries no summary, so a ThinLTO job count would be inert)
def public lto_linker_args(opt_level : uint) : string {
    let msvc_lld = get_platform_name() == "windows" && empty(host_jit_triple())
    let level = int(opt_level)   // a uint interpolates as hex
    return msvc_lld ? "/opt:lldlto={level}" : "-flto -O{level}"
}

// nolint:STYLE014
// Split link: one DLL or exe from N partition objects. create_shared_library takes one object path;
// the rest ride a @response file (lld-link and the POSIX compiler drivers both accept @file),
// which sidesteps the Windows ~8K command-line ceiling a large module count would hit via the
// verbatim extra-args slot. Under `lto` the objects are bitcode and the link gets the LTO arguments.
// Returns false on any failure - the CALLER resets the jit globals and panics (this module cannot
// reach reset_jit_globals_after_failure).
def public link_from_objects(objs : array<string>; out_path : string; kind : JitArtifact;
                             path_to_dascript_lib, path_to_linker, linker_string : string;
                             link_whole_lib : bool; debug_info : bool; log_time : bool;
                             lto : bool = false; opt_level : uint = 3u;
                             static_runtime : bool = false) : bool {
    assert(!empty(objs))
    // every partition object must exist - create_shared_library checks only its first argument
    for (obj in objs) {
        if (!stat(obj).is_valid) {
            to_log(LOG_ERROR, "link_from_objects: partition object missing: {obj}\n")
            return false
        }
    }
    let rsp = split_link_rsp_path(out_path)
    let rsp_content = join([for (i in range(1, length(objs))); "\"{objs[i]}\""], "\n")
    var rsp_ok = false
    fopen(rsp, "wb") $(f) {
        if (f != null) {
            f |> fwrite(rsp_content)
            rsp_ok = true
        }
    }
    if (!rsp_ok) {
        to_log(LOG_ERROR, "link_from_objects: can't write response file {rsp}\n")
        return false
    }
    let phase_tm = ref_time_ticks()
    let extra = lto ? "\"@{rsp}\" {lto_linker_args(opt_level)} {linker_string}" : "\"@{rsp}\" {linker_string}"
    let ok = create_shared_library(objs[0], artifact_path(out_path, kind), "{path_to_dascript_lib}",
        "{path_to_linker}", extra, kind != JitArtifact.exe, link_whole_lib, debug_info,
        static_runtime)
    if (log_time) {
        to_log(LOG_INFO, "LLVM JIT time: link {jit_sec(get_time_usec(phase_tm))} ({length(objs)} {lto ? "bitcode partitions, LTO" : "objects"})\n")
    }
    return ok
}

//! the split link's response file: it shares the artifact's prefix, so the DLL GC tracks it and an exe build removes it with its objects
def public split_link_rsp_path(out_path : string) : string => "{out_path}.objs.rsp"

//! a partition's bitcode sits beside its object under the LTO arms; the object's path is the bitcode's without this suffix
def public split_part_bitcode_path(obj : string) : string => "{obj}{SPLIT_BITCODE_SUFFIX}"

let public SPLIT_BITCODE_SUFFIX = ".bc"

let public MIN_CONST_DATA_ELEMS = 4   //! below it a store per element is cheaper than a constant array and a memcpy

// @out_path - folder + file name, without extension. @path_to_dascript_lib - required on Windows,
// no effect on Linux. @path_to_linker - linker override; Windows-MSVC defaults to lld-link (link.exe
// flavor) from the LLVM package, c++/clang elsewhere.
def public write_artifact(target : JitTarget; mod : LLVMOpaqueModule?; out_path : string;
                          kind : JitArtifact;
                          path_to_dascript_lib, path_to_linker, linker_string : string;
                          link_whole_lib : bool; use_host_cpu : bool; debug_info : bool = false;
                          log_time : bool = false; codegen_opt_level : uint = 3u;
                          static_runtime : bool = false) : bool {
    var emitted = true
    let host_cpu = kind == JitArtifact.jit_dll ? true : use_host_cpu
    with_default_target_machine(target, codegen_opt_level,
                                host_cpu) $(targetMachine : LLVMTargetMachineRef) {
        let error : string?
        let file = artifact_path(out_path, JitArtifact.object)
        var phase_tm = ref_time_ticks()
        if (LLVMTargetMachineEmitToFile(targetMachine, mod, file, LLVMCodeGenFileType.LLVMObjectFile, error) != 0) {
            to_log(LOG_ERROR, "write_artifact: LLVMTargetMachineEmitToFile failed for {file}\n")
            emitted = false
            return
        }
        let t_emit = get_time_usec(phase_tm)
        phase_tm = ref_time_ticks()
        let artifact = artifact_path(out_path, kind)
        var ok = false
        if (kind == JitArtifact.static_lib) {
            ok = create_static_library(file, artifact, "{path_to_linker}")
        } else {
            ok = create_shared_library(file, artifact, "{path_to_dascript_lib}", "{path_to_linker}",
                "{linker_string}", kind != JitArtifact.exe, link_whole_lib, debug_info,
                static_runtime)
        }
        if (log_time) {
            to_log(LOG_INFO, "LLVM JIT time: emit-obj {jit_sec(t_emit)} link {jit_sec(get_time_usec(phase_tm))}\n")
        }
        if (!ok) {
            let step = kind == JitArtifact.static_lib ? "archive" : "link"
            to_log(LOG_ERROR, "write_artifact: {step} failed for {artifact}\n")
            emitted = false
        } elif (kind == JitArtifact.exe && get_platform_name() == "linux") {
            remove(file)
        }
    }
    return emitted
}


// Locate the wasm64 (memory64) libDaScript_runtime.a. Non-empty `override_path` wins (e.g.
// --jit-runtime-lib CLI flag; a missing file logs a warning); otherwise auto-locate at
// <das_root>/web/output64/lib/liblibDaScript_runtime.a.
def private find_runtime_lib(override_path : string) : Option<string> {
    if (!(override_path |> empty())) {
        if (stat(override_path).is_valid) return some(override_path)
        to_log(LOG_WARNING, "wasm: --jit-runtime-lib path does not exist: {override_path}\n")
        return none(type<string>)
    }
    let candidate = "{get_das_root()}/web/output64/lib/liblibDaScript_runtime.a"
    return stat(candidate).is_valid ? some(candidate) : none(type<string>)
}

//! the wasm triple of a cross run whose command line names none
def private wasm_triple_or_default(triple : string) : string =>
    !(triple |> empty()) ? triple : "wasm64-unknown-emscripten"

//! writes `mod` as one object for the cross triple at `obj`,
//! verbatim; false when no object is on disk after
def public emit_target_object(target : JitTarget; mod : LLVMOpaqueModule?; obj : string;
                              triple : string) : bool {
    var emitted = false
    let real_triple = wasm_triple_or_default(triple)
    LLVMInitializeWasmTarget()
    with_target_machine(real_triple, "", target_plus_features(target),
                        3u) $(targetMachine : LLVMTargetMachineRef) {
        // Pin the module's data layout + triple to the TARGET's so codegen sizes
        // pointers correctly (wasm64 → 8-byte). Without this the module inherits
        // host layout and produces wrong-sized loads/stores.
        let dl = LLVMCreateTargetDataLayout(targetMachine)
        LLVMSetModuleDataLayout(mod, dl)
        LLVMSetTarget(mod, real_triple)
        LLVMDisposeTargetData(dl)
        remove(obj)
        let error : string?
        let failed = LLVMTargetMachineEmitToFile(targetMachine, mod, obj,
            LLVMCodeGenFileType.LLVMObjectFile, error)
        emitted = failed == 0 && stat(obj).is_valid
    }
    if (!emitted) {
        to_log(LOG_ERROR,
            "{real_triple}: failed to emit object {obj} (LLVMTargetMachineEmitToFile error)\n")
    }
    return emitted
}

//! a cross-target library is one object at `<out_path>.o`: the
//! target toolchain's link brings the runtime archive built for that
//! target, and a host archiver need not index a foreign object
def public write_cross_lib(target : JitTarget; mod : LLVMOpaqueModule?; out_path : string;
                           triple : string; link_whole_lib : bool) : bool {
    let obj = artifact_path(out_path, JitArtifact.object)
    return false if (!emit_target_object(target, mod, obj, triple))
    if (link_whole_lib) {
        to_log(LOG_WARNING, "LLVM LIB: {obj} registers a module from the compiler library, " +
            "which a cross-target runtime archive does not carry\n")
    }
    to_log(LOG_INFO, "LLVM LIB: {triple} object {obj} written - link it with that target's " +
        "toolchain and runtime archive ({out_path}.h names the link)\n")
    return true
}

def public write_wasm(target : JitTarget; mod : LLVMOpaqueModule?; out_path : string;
                      triple : string;
                      path_to_emcc : string; needs_runtime : bool; explicit_runtime : string = "";
                      emit_object_only : bool = false) : bool {
    //! an emitted-only object lands at `out_path` verbatim, no `.o.o`:
    //! daspkg links that path
    let obj = emit_object_only ? out_path : artifact_path(out_path, JitArtifact.object)
    return false if (!emit_target_object(target, mod, obj, triple))
    if (emit_object_only) {
        to_log(LOG_INFO, "wasm: emitted object {obj} (link skipped; --jit-emit-object)\n")
        return true
    }
    let runtime_lib = find_runtime_lib(explicit_runtime)
    if (needs_runtime && is_none(runtime_lib)) {
        to_log(LOG_WARNING, "wasm: runtime symbols referenced but libDaScript_runtime.a " +
            "not found at web/output64/lib/ - run the web/ emscripten build to link in the " +
            "runtime, or pass --jit-runtime-lib=<path>\n")
    }
    let runtime_arg = needs_runtime ? (runtime_lib ?? "") : ""
    if (!link_wasm(obj, "{out_path}.wasm", runtime_arg, path_to_emcc, true)) {
        to_log(LOG_ERROR, "write_wasm: link failed for {out_path}.wasm\n")
        return false
    }
    return true
}

def public build_string_constant(message : string) {
    if (g_str2v |> key_exists(message)) return g_str2v[message]
    var str = build_string_constant(message, uint(length(message)))
    g_str2v[message] = str
    return str
}

def public build_string_constant(message : string implicit; msg_size : uint) {
    var strType = LLVMArrayType(g_prim_t.t_int8, msg_size + 1u)
    // Prefix all string variable names to not intersect with function names.
    var str = LLVMAddGlobal(g_mod, strType, ".str.{message}")
    LLVMSetInitializer(str, LLVMConstStringInContext(g_ctx, message, msg_size, 0))
    LLVMSetGlobalConstant(str, 1)
    LLVMSetLinkage(str, LLVMLinkage.LLVMPrivateLinkage)
    LLVMSetUnnamedAddress(str, LLVMUnnamedAddr.LLVMGlobalUnnamedAddr)
    LLVMSetAlignment(str, 1u)
    return str
}

def public get_basetype_pointer(t : Type) : LLVMOpaqueType? =>
    LLVMPointerType(base_type_to_llvm_type(t), 0u)

def public get_type_pointer(t : TypeDeclPtr) : LLVMOpaqueType? {
    if (t == null || t.isVoid) {
        return g_prim_t.LLVMVoidPtrType()
    } else {
        return LLVMPointerType(type_to_llvm_type(t), 0u)
    }
}

def public get_empty_string(builder : LLVMOpaqueBuilder?) =>
    LLVMBuildPointerCast(builder, build_string_constant(""), g_prim_t.get_type_string(), "empty string_constant")

def public get_string_constant_ptr(builder : LLVMOpaqueBuilder?; message : string) {
    if (empty(message)) return LLVMConstPointerNull(g_prim_t.get_type_string())
    var str = build_string_constant(message)
    return LLVMBuildPointerCast(builder, str, g_prim_t.get_type_string(), "string_constant {message}")
}

def public get_string_constant_ptr(builder : LLVMOpaqueBuilder?; value : das_string) {
    if (empty(value)) return LLVMConstPointerNull(g_prim_t.get_type_string())
    var str : LLVMOpaqueValue?
    peek(value) $(text) {
        str = build_string_constant(text, uint(length(value)))
    }
    return LLVMBuildPointerCast(builder, str, g_prim_t.get_type_string(), "string_constant {value}")
}

def public failed(msg : string) {
    to_log(LOG_ERROR, "LLVM JIT: {msg}\n")
    g_errors |> push("{msg}")
}

def public failed_E(e : Expression?; msg : string) {
    failed("{msg}. expr = {e.__rtti} at {describe(e.at)}")
}

def public failed_V(v : VariablePtr; msg : string) {
    failed("{msg}. var = {describe(v)} at {describe(v.at)}")
}

def public failed_T(v : TypeDeclPtr; msg : string) {
    failed("{msg}. var = {describe(v)} at {describe(v.at)}")
}

def public base_type_to_llvm_type(t : Type) {  // nolint:STYLE037 - exact-type dispatch table over Type; one arm per base type is the shape
    return g_prim_t.t_int32 if (t == Type.tInt || t == Type.tUInt || t == Type.tBitfield || t == Type.tEnumeration)
    return g_prim_t.t_int64 if (t == Type.tInt64 || t == Type.tUInt64 || t == Type.tBitfield64 || t == Type.tEnumeration64)
    return g_prim_t.t_int16 if (t == Type.tInt16 || t == Type.tUInt16 || t == Type.tBitfield16 || t == Type.tEnumeration16)
    return g_prim_t.t_int8 if (t == Type.tInt8 || t == Type.tUInt8 || t == Type.tBitfield8 || t == Type.tEnumeration8)
    return g_prim_t.t_float if (t == Type.tFloat)
    return g_prim_t.t_double if (t == Type.tDouble)
    return g_prim_t.t_int1 if (t == Type.tBool)
    return g_prim_t.LLVMInt2Type() if (t == Type.tInt2 || t == Type.tUInt2 || t == Type.tRange || t == Type.tURange)
    return g_prim_t.LLVMInt3Type() if (t == Type.tInt3 || t == Type.tUInt3)
    return g_prim_t.LLVMInt4Type() if (t == Type.tInt4 || t == Type.tUInt4)
    return g_prim_t.LLVMRange64Type() if (t == Type.tRange64 || t == Type.tURange64)
    return g_prim_t.LLVMFloat2Type() if (t == Type.tFloat2)
    return g_prim_t.LLVMFloat3Type() if (t == Type.tFloat3)
    return g_prim_t.LLVMFloat4Type() if (t == Type.tFloat4)
    return g_prim_t.get_type_string() if (t == Type.tString)
    return g_prim_t.LLVMVoidPtrType() if (t == Type.tPointer)
    // 16/8-bit lattice — packed-lane vectors mirror the das value layout
    return g_prim_t.t_half if (t == Type.tFloat16)
    return g_prim_t.LLVMHalfVecType(2u) if (t == Type.tHalf2)
    return g_prim_t.LLVMHalfVecType(3u) if (t == Type.tHalf3)
    return g_prim_t.LLVMHalfVecType(4u) if (t == Type.tHalf4)
    return g_prim_t.LLVMHalfVecType(8u) if (t == Type.tHalf8)
    return g_prim_t.LLVMShortVecType(2u) if (t == Type.tShort2 || t == Type.tUShort2)
    return g_prim_t.LLVMShortVecType(3u) if (t == Type.tShort3 || t == Type.tUShort3)
    return g_prim_t.LLVMShortVecType(4u) if (t == Type.tShort4 || t == Type.tUShort4)
    return g_prim_t.LLVMShortVecType(8u) if (t == Type.tShort8 || t == Type.tUShort8)
    return g_prim_t.LLVMByteVecType(2u) if (t == Type.tByte2 || t == Type.tUByte2)
    return g_prim_t.LLVMByteVecType(3u) if (t == Type.tByte3 || t == Type.tUByte3)
    return g_prim_t.LLVMByteVecType(4u) if (t == Type.tByte4 || t == Type.tUByte4)
    return g_prim_t.LLVMByteVecType(8u) if (t == Type.tByte8 || t == Type.tUByte8)
    return g_prim_t.LLVMByteVecType(16u) if (t == Type.tByte16 || t == Type.tUByte16)
    failed("base_type_to_llvm_type: unsupported type {t}")
    return g_prim_t.t_int32
}

def public type_to_llvm_abi_type(t : TypeDeclPtr) {
    // distinct types are ABI-identical to their underlying type - erased for the JIT,
    // same boundary as runtime TypeInfo (ref-ness lives on the head, so peel after isRef)
    if (t.baseType == Type.tDistinct && t.firstType != null && !t.isRef) {
        return type_to_llvm_abi_type(t.firstType)
    }
    if (t.baseType == Type.tBlock) {
        return LLVMPointerType(g_t_block, 0u)
    } elif (t.isRef) {
        return LLVMPointerType(type_to_llvm_type(t), 0u)
    } elif (t.isPointer) {
        return get_type_pointer(t.firstType)
    } elif (t.isWorkhorseType) {
        return base_type_to_llvm_type(t.baseType)
    } elif (t.baseType == Type.tFunction || t.baseType == Type.tLambda) {
        return type_to_llvm_type(t)
    } elif (t.isVoid) {
        return g_prim_t.t_void
    } elif (t.isAnyType) {
        return g_prim_t.LLVMFloat4Type()
    } elif (t.baseType == Type.fakeContext || t.baseType == Type.fakeLineInfo) {
        return g_prim_t.LLVMVoidPtrType()
    } elif (t.baseType == Type.tHandle) {
        if (t.isRefType) {
            failed("type_to_llvm_abi_type: internal error. unsupported type {describe(t)} at {describe(t.at)}")
            return g_prim_t.LLVMVoidPtrType()
        } else {
            var tp = get_underlying_value_type(t)
            if (tp == null) {
                failed("internal integration error.`get_underlying_value_type` returned null for {describe(t)} at {describe(t.at)}")
                return g_prim_t.LLVMFloat4Type()
            } elif (!tp.isWorkhorseType && tp.baseType != Type.anyArgument) {
                failed("internal error. expecting workhorse type from `get_underlying_value_type` for {describe(t)} at {describe(t.at)}")
                return g_prim_t.LLVMFloat4Type()
            }
            return type_to_llvm_abi_type(tp)
        }
    } elif (t.baseType == Type.anyArgument) {
        return g_prim_t.LLVMFloat4Type()
    } else {
        failed("type_to_llvm_abi_type: unsupported type {describe(t)} at {describe(t.at)}")
        return g_prim_t.t_int32
    }
}

def public type_to_llvm_type(t : TypeDeclPtr) {
    // distinct types are ABI-identical to their underlying type - erased for the JIT
    if (t.baseType == Type.tDistinct && t.firstType != null) {
        return type_to_llvm_type(t.firstType)
    }
    var res : LLVMOpaqueType?
    if (t.baseType == Type.tFixedArray && t.firstType != null) {
        // natural recursion: [outer x [inner x elem]] — same nesting the reversed flat walk built
        res = LLVMArrayType(type_to_llvm_type(t.firstType), uint(t.fixedDim))
    } elif (t.isPointer) {
        res = get_type_pointer(t.firstType)
    } elif (t.isWorkhorseType) {
        res = base_type_to_llvm_type(t.baseType)
    } elif (t.isStructure) {
        res = LLVMArrayType(g_prim_t.t_int8, uint(t.sizeOf))
    } elif (t.isTuple) {
        res = LLVMArrayType(g_prim_t.t_int8, uint(t.sizeOf))
    } elif (t.isVariant) {
        res = LLVMArrayType(g_prim_t.t_int8, uint(t.sizeOf))
    } elif (t.baseType == Type.tFunction) {
        res = g_t_function
    } elif (t.baseType == Type.tLambda) {
        res = g_prim_t.LLVMVoidPtrType()
    } elif (t.baseType == Type.tIterator) {
        res = g_t_sequence
    } elif (t.baseType == Type.tBlock) {
        res = g_t_block
    } elif (t.baseType == Type.tArray) {
        // must mirror Array (arraytype.h) byte-for-byte; padAfterFlags mirrors the trailing alignment pad.
        // Element-typed data pointer is a no-op under LLVM 22 opaque pointers, so it stays void?.
        res = jit_struct_type(type<tuple<data : void?; size : int64; capacity : int64; magic : int; lock : int; flags : int; padAfterFlags : int>>)
    } elif (t.baseType == Type.tTable) {
        // must mirror Table (Array base + keys/hashes/tombstones); padAfterFlags 8-aligns keys on 32-bit ABIs.
        res = jit_struct_type(type<tuple<data : void?; size : int64; capacity : int64; magic : int; lock : int; flags : int; padAfterFlags : int; keys : void?; hashes : void?; tombstones : int64>>)
    } elif (t.isVoid) {
        res = g_prim_t.t_void
    } elif (t.isAnyType) {
        res = g_prim_t.LLVMFloat4Type()
    } elif (t.isHandle) {
        if (t.isRefType) {
            res = LLVMArrayType(g_prim_t.t_int8, uint(t.sizeOf))
        } else {
            var tp = get_underlying_value_type(t)
            if (tp == null) {
                failed("internal integration error.`get_underlying_value_type` returned null for {describe(t)} at {describe(t.at)}")
                return g_prim_t.t_int32
            } elif (!tp.isWorkhorseType && tp.baseType != Type.anyArgument) {
                failed("internal error. expecting workhorse type from `get_underlying_value_type` for {describe(t)} at {describe(t.at)}")
                return g_prim_t.t_int32
            }
            res = type_to_llvm_type(tp)
        }
    } else {
        failed("type_to_llvm_type: unsupported type {describe(t)} at {describe(t.at)}")
        res = g_prim_t.t_int32
    }
    return res
}

def public dim_element_type_to_llvm_type(typ : TypeDeclPtr) {
    if (typ.baseType != Type.tFixedArray) {
        return type_to_llvm_type(typ)
    } else {
        var ndt = clone_type(typ)
        while (ndt.baseType == Type.tFixedArray && ndt.firstType != null) {   // scalar element at the chain leaf
            ndt = clone_type(ndt.firstType)
        }
        return type_to_llvm_type(ndt)
    }
}

def public build_broadcast_vector(builder : LLVMOpaqueBuilder?; opType : TypeDeclPtr; right : LLVMOpaqueValue?) {
    if (opType.isVectorType) {
        var vres = LLVMGetUndef(type_to_llvm_type(opType))
        for (i in range(opType.vectorDim)) {
            vres = LLVMBuildInsertElement(builder, vres, right, g_prim_t.ConstI32(uint64(i)), "")
        }
        return vres
    } else {
        return right
    }
}

def public LLVMBuildLoadData2Aligned(builder : LLVMOpaqueBuilder?; var typ : LLVMOpaqueType?; ptr : LLVMOpaqueValue?; alignment : int; name : string) {
    if (!LLVM_JIT_ALLOW_UNALIGNED_VECTOR_READ_OUT_OF_BOUNDS && alignment != 16 && LLVMIsVector3(typ)) {
        return LLVMBuildLoadVector3(builder, g_prim_t, typ, ptr, name)
    } else {
        return LLVMBuildLoad2Aligned(builder, typ, ptr, alignment |> uint, name)
    }
}

def public LLVMBuildMemSet(builder : LLVMOpaqueBuilder?; ptr : LLVMOpaqueValue?; value : uint64; length : uint64; alignment : uint) {
    LLVMBuildMemSet(builder, ptr, LLVMConstInt(g_prim_t.t_int8, value, 0), g_prim_t.ConstI32(length), alignment)
}

def public expand_scalar(builder : LLVMOpaqueBuilder?; scalar : LLVMOpaqueValue?; vecType : TypeDeclPtr) {
    var vec = LLVMGetUndef(type_to_llvm_type(vecType))
    for (d in range(vecType.vectorDim)) {
        var idx = g_prim_t.ConstI32(uint64(d))
        vec = LLVMBuildInsertElement(builder, vec, scalar, idx, "")
    }
    return vec
}

//! the metadata kind that marks an access the emitter writes by bit
//! pattern into memory of another type (the memset family): the
//! strict-aliasing stamp leaves it untagged, so it aliases everything
let public JIT_UNTYPED_MD = "das.untyped"

def public mark_untyped_access(ctx : LLVMOpaqueContext?; inst : LLVMOpaqueValue?) {
    let kind = LLVMGetMDKindIDInContext(ctx, JIT_UNTYPED_MD, uint(length(JIT_UNTYPED_MD)))
    LLVMSetMetadata(inst, kind, LLVMMetadataAsValue(ctx, LLVMMDNodeInContext2(ctx, null, 0ul)))
}
