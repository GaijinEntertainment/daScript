options gen2

options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options relaxed_pointer_const
options unsafe_table_lookup = false
options no_global_variables = false
options stack = 4_194_304 // On huge files we need huge stack for jit

module llvm_jit_common shared private

require llvm/llvm_boost
require llvm/llvm_dll_utils
require llvm_config

require daslib/ast_boost
require daslib/strings
require daslib/strings_boost
require daslib/defer
require daslib/enum_trait
require jit
require fio

let public LLVM_JIT_ENABLED = true || llvm_config::llvm_jit_enabled()
let public LLVM_JIT_ALL_FUNCTIONS = true || llvm_config::llvm_jit_all_functions()

let public LLVM_DEBUG_RESULT = false || llvm_config::llvm_debug_result()
let public LLVM_DEBUG_EVERYTHING = false || llvm_config::llvm_debug_everything()

let public LLVM_DEBUG_TRACES = false
let public LLVM_DEBUG_LINE_TRACES = false

let public LLVM_JIT_LOG = false || LLVM_DEBUG_EVERYTHING
let public LLVM_DEBUG_INFO = false //  || LLVM_DEBUG_EVERYTHING

let public LLVM_JIT_ALWAYS_SOLID = llvm_config::llvm_jit_always_solid()
var public LLVM_JIT_ALLOW_UNALIGNED_VECTOR_READ_OUT_OF_BOUNDS = false

let public LLVM_OUTPUT_OJBECT_FILE = false
let public LLVM_OUTPUT_OJBECT_FILE_NAME = "out/"

let public LLVM_ENABLE_DLL_LOADING = true
let public JIT_DLL_OUTPUT = "output.dll"
let public DASCRIPT_LIBRARY = "build/liblibDaScript.a" // liblibDaScript.a on linux

let public LLVM_ENABLE_OPT_PASS = false // LLVM_DEBUG_EVERYTHING ? false : true
let public LLVM_OPT_LEVEL = 0u // LLVM_DEBUG_EVERYTHING ? 0u : 3u
let public LLVM_SIZE_LEVEL = 0u // LLVM_DEBUG_EVERYTHING ? 0u : 3u
let public LLVM_JIT_OPT_LEVEL = 0u // LLVM_DEBUG_EVERYTHING ? 0u : 3u
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
let public FN_JIT_ALLOC_HEAP        = "jit_alloc_heap"
let public FN_JIT_ALLOC_PERSISTENT  = "jit_alloc_persistent"
let public FN_JIT_FREE_HEAP         = "jit_free_heap"
let public FN_JIT_FREE_PERSISTENT   = "jit_free_persistent"
let public FN_JIT_ARRAY_LOCK        = "jit_array_lock"
let public FN_JIT_ARRAY_UNLOCK      = "jit_array_unlock"
let public FN_JIT_STR_CMP           = "jit_str_cmp"
let public FN_JIT_STR_CAT           = "jit_str_cat"
let public FN_JIT_PROLOGUE          = "jit_prologue"
let public FN_JIT_EPILOGUE          = "jit_epilogue"
let public FN_JIT_MAKE_BLOCK        = "jit_make_block"
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

struct JitEngine {
    mod : LLVMOpaqueModule?
    engine : LLVMExecutionEngineRef
    ctx : LLVMContextRef
}

var private g_all_engines : array<JitEngine>

var public g_ctx : LLVMContextRef
var public g_prim_t : PrimitiveTypes?
var public g_mod : LLVMOpaqueModule?
var public g_engine : LLVMExecutionEngineRef
var public g_failed : bool = false
var public g_errors : array<string>
var public g_loading_from_dll : bool = true

var public g_t_simFunction : LLVMOpaqueType?
var public g_t_function : LLVMOpaqueType?
var public g_t_lambda : LLVMOpaqueType?
var public g_t_block : LLVMOpaqueType?
var public g_t_stack_state : LLVMOpaqueType?
var public g_t_iterator : LLVMOpaqueType?
var public g_t_sequence : LLVMOpaqueType?

var g_str2v : table<string; LLVMOpaqueValue?>

class Attributes {
    noreturn : LLVMOpaqueAttributeRef?
    readonly : LLVMOpaqueAttributeRef?
    nounwind : LLVMOpaqueAttributeRef?
    willreturn : LLVMOpaqueAttributeRef?
    nocapture : LLVMOpaqueAttributeRef?

    def Attributes(ctx : LLVMContextRef) {
        noreturn = g_ctx |> LLVMGetEnumAttribute("noreturn")
        readonly = g_ctx |> LLVMGetEnumAttribute("readonly")
        nounwind = g_ctx |> LLVMGetEnumAttribute("nounwind")
        willreturn = g_ctx |> LLVMGetEnumAttribute("willreturn")
        nocapture = g_ctx |> LLVMGetEnumAttribute("nocapture")
    }

}

enum public JIT_ARRAY {
    DATA
    SIZE
    CAPACITY
    LOOK
    FLAGS
}

enum public JIT_TABLE {
    DATA
    SIZE
    CAPACITY
    LOOK
    FLAGS
    KEYS
    HASHES
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

// We use global variable to store pointer to jit table methods.
def public FN_JIT_TABLE_AT(t : Type) {
    return "`jit`table_at`{t}`"
}

def public FN_JIT_TABLE_ERASE(t : Type) {
    return "`jit`table_erase`{t}`"
}

def public FN_JIT_TABLE_FIND(t : Type) {
    return "`jit`table_find`{t}`"
}

var public g_fn_types : table<string; LLVMTypeRef>

def public LLVMAddFunctionWithType(mod : LLVMModuleRef; name : string; typ : LLVMTypeRef) {
    var rv = LLVMAddFunction(mod, name, typ)
    g_fn_types[name] = typ
    return rv
}

def private add_table_linkage(val : LLVMOpaqueValue?) {
    LLVMSetLinkage(val, LLVMLinkage.LLVMWeakAnyLinkage);
}

[macro_function]
def private add_table_methods(llvm_module : LLVMOpaqueModule?; llvm_engine : LLVMExecutionEngineRef; var types : PrimitiveTypes?; var dll : DLLHandle?; attrs : Attributes?) {
    // table functions
    for (t in get_workhorse_types()) {
        // int32_t jit_table_at ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context )
        let at_type = LLVMFunctionType(types.t_int32,   // int32_t result
                fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(), // table ptr
                base_type_to_llvm_type(t),                           // key
                types.t_int32,                                    // valueTypeSize
                types.LLVMVoidPtrType(),                          // context
                types.LLVMVoidPtrType(),                          // at
                ))
        let at_name = FN_JIT_TABLE_AT(t)
        g_fn_types[at_name] = at_type

        let at_impl = get_jit_table_at(unsafe(reinterpret<int> t))
        let at_ptr_type = LLVMPointerType(at_type, 0u)
        var jit_table_at = LLVMAddGlobal(llvm_module, LLVMPointerType(at_ptr_type, 0u), at_name)
        add_table_linkage(jit_table_at)
        LLVMSetInitializer(jit_table_at, types.ConstPtr(at_impl |> intptr(), at_ptr_type))

        let at_variable = dll.handle == null ? null : dll.get_function_address(DllName(at_name))
        if (at_variable != null) {
            dll.set_function_address(DllName(at_name), at_impl)
        }
        // int32_t jit_table_find ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context )
        let find_type = LLVMFunctionType(types.t_int32,   // int32_t result
                fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(),   // table ptr
                base_type_to_llvm_type(t),                          // key
                types.t_int32,                                    // valueTypeSize
                types.LLVMVoidPtrType()                 // context
                ))
        let find_name = FN_JIT_TABLE_FIND(t)
        g_fn_types[find_name] = find_type

        let find_impl = get_jit_table_find(unsafe(reinterpret<int> t))
        let find_ptr_type = LLVMPointerType(find_type, 0u)
        var jit_table_find = LLVMAddGlobal(llvm_module, LLVMPointerType(find_ptr_type, 0u), find_name)
        add_table_linkage(jit_table_find)
        LLVMSetInitializer(jit_table_find, types.ConstPtr(find_impl |> intptr(), find_ptr_type))

        let find_variable = dll.handle == null ? null : dll.get_function_address(DllName(find_name))
        if (find_variable != null) {
            dll.set_function_address(DllName(find_name), find_impl)
        }

        // bool jit_table_erase ( Table * tab, KeyType key, int32_t valueTypeSize, Context * context )
        let erase_type = LLVMFunctionType(types.t_int1,                        // bool result
                fixed_array<LLVMTypeRef>(types.LLVMVoidPtrType(),   // table ptr
                base_type_to_llvm_type(t),                          // key
                types.t_int32,                                    // valueTypeSize
                types.LLVMVoidPtrType()                 // context
                ))
        let erase_name = FN_JIT_TABLE_ERASE(t)
        let erase_ptr_type = LLVMPointerType(erase_type, 0u)
        let erase_impl = get_jit_table_erase(unsafe(reinterpret<int> t))

        var jit_table_erase = LLVMAddGlobal(llvm_module, LLVMPointerType(erase_ptr_type, 0u), erase_name)
        add_table_linkage(jit_table_erase)
        LLVMSetInitializer(jit_table_erase, types.ConstPtr(erase_impl |> intptr(), erase_ptr_type))

        let erase_variable = dll.handle == null ? null : dll.get_function_address(DllName(erase_name))
        g_fn_types[erase_name] = erase_type
        if (erase_variable != null) {
            dll.set_function_address(DllName(erase_name), find_impl)
        }
    }
}


[macro_function]
def public init_jit(var dll : DLLHandle?) {
    return if (is_in_completion() || is_compiling_macros())
    return if (g_mod != null)    // prevent double init
    LLVMLinkInMCJIT()
    LLVMInitializeNativeTarget()
    LLVMInitializeNativeAsmPrinter()
    LLVMInitializeNativeAsmParser()
    g_ctx = LLVMContextCreate()
    g_prim_t = new PrimitiveTypes(g_ctx)
    LLVMContextSetOpaquePointers(g_ctx, 1)
    if (LLVM_DEBUG_INFO) {
        set_context_diagnostics_to_log(g_ctx)
    }
    g_failed = false
    g_fn_types |> clear()
    g_mod = LLVMModuleCreateWithNameInContext("llvm_jit_module", g_ctx)
    LLVMCreateJITCompilerForModule(g_engine, g_mod, LLVM_JIT_OPT_LEVEL)
    static_if (LLVM_DEBUG_EVERYTHING) {
        let hostcpu = LLVMGetHostCPUName()
        let features = LLVMGetHostCPUFeatures()
        print("LLVM host cpu: {hostcpu}\nLLVM host cpu features: {features}\n")
        LLVMDisposeMessage(hostcpu)
        LLVMDisposeMessage(features)
    }
    let attrs = new Attributes(g_ctx)
    assume noreturn = attrs.noreturn
    assume readonly = attrs.readonly
    assume nounwind = attrs.nounwind
    assume willreturn = attrs.willreturn
    assume nocapture = attrs.nocapture
    build_jit_types()

    // add default functions
    var jit_exception = LLVMAddFunctionWithType(g_mod, FN_JIT_EXCEPTION,    // jit_exception(text,context *,lineinfo *)
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(g_prim_t.get_type_string(), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_exception, get_jit_exception())
    LLVMAddAttributeToFunction(jit_exception, noreturn)
    LLVMAddAttributeToFunctionArgumentRange(jit_exception, urange(0, 2), nocapture)
    // jit_call_or_fastcall(func,args *,context *)
    var jit_call_or_fastcall = LLVMAddFunctionWithType(g_mod, FN_JIT_CALL_OR_FASTCALL,
        LLVMFunctionType(g_prim_t.LLVMFloat4Type(),
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), LLVMPointerType(g_prim_t.LLVMFloat4Type(), 0u), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_call_or_fastcall, get_jit_call_or_fastcall())
    LLVMAddAttributeToFunctionArgumentRange(jit_call_or_fastcall, urange(0, 3), nocapture)
    LLVMAddAttributesToFunction(jit_call_or_fastcall, fixed_array(nounwind, willreturn))
    // jit_call_with_cmres(func,args *,cmres *,context *)
    var jit_call_with_cmres = LLVMAddFunctionWithType(g_mod, FN_JIT_CALL_WITH_CMRES,
        LLVMFunctionType(g_prim_t.LLVMFloat4Type(),
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), LLVMPointerType(g_prim_t.LLVMFloat4Type(), 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_call_with_cmres, get_jit_call_with_cmres())
    LLVMAddAttributeToFunctionArgumentRange(jit_call_with_cmres, urange(0, 4), nocapture)
    LLVMAddAttributesToFunction(jit_call_with_cmres, fixed_array(nounwind, willreturn))
    // vec4f jit_invoke_block ( const Block & blk, vec4f * args, Context * context )
    var jit_invoke_block = LLVMAddFunctionWithType(g_mod, FN_JIT_INVOKE_BLOCK,
        LLVMFunctionType(g_prim_t.LLVMFloat4Type(),
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_block, 0u), LLVMPointerType(g_prim_t.LLVMFloat4Type(), 0u), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_invoke_block, get_jit_invoke_block())
    LLVMAddAttributeToFunctionArgumentRange(jit_invoke_block, urange(0, 3), nocapture)
    LLVMAddAttributesToFunction(jit_invoke_block, fixed_array(nounwind, willreturn))
    // vec4f jit_invoke_block_with_cmres ( const Block & blk, vec4f * args, void * cmres, Context * context )
    var jit_invoke_block_with_cmres = LLVMAddFunctionWithType(g_mod, FN_JIT_INVOKE_BLOCK_WITH_CMRES,
        LLVMFunctionType(g_prim_t.LLVMFloat4Type(),
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_block, 0u), LLVMPointerType(g_prim_t.LLVMFloat4Type(), 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_invoke_block_with_cmres, get_jit_invoke_block_with_cmres())
    LLVMAddAttributeToFunctionArgumentRange(jit_invoke_block_with_cmres, urange(0, 4), nocapture)
    LLVMAddAttributesToFunction(jit_invoke_block_with_cmres, fixed_array(nounwind, willreturn))
    // jit_string_builder ( context *, node *, args * )
    var jit_string_builder = LLVMAddFunctionWithType(g_mod, FN_JIT_STRING_BUILDER,
        LLVMFunctionType(g_prim_t.get_type_string(),
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType(), LLVMPointerType(g_prim_t.LLVMFloat4Type(), 0u))))
    LLVMAddGlobalMapping(g_engine, jit_string_builder, get_jit_string_builder())
    LLVMAddAttributesToFunction(jit_string_builder, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_string_builder, urange(0, 3), nocapture)
    // jit_string_builder_temp ( context *, node *, args * )
    var jit_string_builder_temp = LLVMAddFunctionWithType(g_mod, FN_JIT_STRING_BUILDER_TEMP,
        LLVMFunctionType(g_prim_t.get_type_string(),
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType(), LLVMPointerType(g_prim_t.LLVMFloat4Type(), 0u))))
    LLVMAddGlobalMapping(g_engine, jit_string_builder_temp, get_jit_string_builder_temp())
    LLVMAddAttributesToFunction(jit_string_builder_temp, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_string_builder_temp, urange(0, 3), nocapture)
    // void * jit_get_global_mnh ( uint64_t mnh, context * )
    var jit_get_global_mnh = LLVMAddFunctionWithType(g_mod, FN_JIT_GET_GLOBAL_MNH,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.t_int64, g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_get_global_mnh, get_jit_get_global_mnh())
    LLVMAddAttributesToFunction(jit_get_global_mnh, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgument(jit_get_global_mnh, 1u, nocapture)
    // void * jit_get_shared_mnh ( uint64_t mnh, context * )
    var jit_get_shared_mnh = LLVMAddFunctionWithType(g_mod, FN_JIT_GET_SHARED_MNH,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.t_int64, g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_get_shared_mnh, get_jit_get_shared_mnh())
    LLVMAddAttributesToFunction(jit_get_shared_mnh, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgument(jit_get_shared_mnh, 1u, nocapture)
    // jit_alloc_heap ( uint64_t mnh, context * )
    var jit_alloc_heap = LLVMAddFunctionWithType(g_mod, FN_JIT_ALLOC_HEAP,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.t_int32, g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_alloc_heap, get_jit_alloc_heap())
    LLVMAddAttributesToFunction(jit_alloc_heap, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgument(jit_alloc_heap, 1u, nocapture)
    // jit_alloc_persistent ( uint64_t mnh, context * )
    var jit_alloc_persistent = LLVMAddFunctionWithType(g_mod, FN_JIT_ALLOC_PERSISTENT,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.t_int32, g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_alloc_persistent, get_jit_alloc_persistent())
    LLVMAddAttributesToFunction(jit_alloc_persistent, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgument(jit_alloc_persistent, 1u, nocapture)
    // void jit_free_heap ( void * bytes, uint32_t size, Context * context )
    var jit_free_heap = LLVMAddFunctionWithType(g_mod, FN_JIT_FREE_HEAP,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), g_prim_t.t_int32, g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_free_heap, get_jit_free_heap())
    LLVMAddAttributesToFunction(jit_free_heap, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgument(jit_free_heap, 0u, nocapture)
    LLVMAddAttributeToFunctionArgument(jit_free_heap, 2u, nocapture)
    // void jit_free_persistent ( void * bytes, Context * context )
    var jit_free_persistent = LLVMAddFunctionWithType(g_mod, FN_JIT_FREE_PERSISTENT,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_free_persistent, get_jit_free_persistent())
    LLVMAddAttributesToFunction(jit_free_persistent, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_free_persistent, urange(0, 2), nocapture)
    // void jit_array_lock ( Array & array, Context * context )
    var jit_array_lock = LLVMAddFunctionWithType(g_mod, FN_JIT_ARRAY_LOCK,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_array_lock, get_jit_array_lock())
    LLVMAddAttributesToFunction(jit_array_lock, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_array_lock, urange(0, 2), nocapture)
    // void jit_array_unlock ( Array & array, Context * context )
    var jit_array_unlock = LLVMAddFunctionWithType(g_mod, FN_JIT_ARRAY_UNLOCK,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(
                g_prim_t.LLVMVoidPtrType(), // arr
                g_prim_t.LLVMVoidPtrType(), // ctx
                g_prim_t.LLVMVoidPtrType(), // lineInfo
            )
        ))
    LLVMAddGlobalMapping(g_engine, jit_array_unlock, get_jit_array_unlock())
    LLVMAddAttributesToFunction(jit_array_unlock, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_array_unlock, urange(0, 2), nocapture)

    add_table_methods(g_mod, g_engine, g_prim_t, dll, attrs)
    // int jit_str_cmp ( char * a, char * b )
    var jit_str_cmp = LLVMAddFunctionWithType(g_mod, FN_JIT_STR_CMP,
        LLVMFunctionType(g_prim_t.t_int32,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_prim_t.t_int8, 0u), LLVMPointerType(g_prim_t.t_int8, 0u))))
    LLVMAddGlobalMapping(g_engine, jit_str_cmp, get_jit_str_cmp())
    LLVMAddAttributesToFunction(jit_str_cmp, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_str_cmp, urange(0, 2), nocapture)
    // char * jit_str_cat ( char * a, char * b, Copntext * context )
    var jit_str_cat = LLVMAddFunctionWithType(g_mod, FN_JIT_STR_CAT,
        LLVMFunctionType(LLVMPointerType(g_prim_t.t_int8, 0u),
            fixed_array<LLVMTypeRef>(
                LLVMPointerType(g_prim_t.t_int8, 0u), // sA
                LLVMPointerType(g_prim_t.t_int8, 0u), // sB
                g_prim_t.LLVMVoidPtrType(),           // ctx
                g_prim_t.LLVMVoidPtrType(),           // lineInfo
            )
        ))
    LLVMAddGlobalMapping(g_engine, jit_str_cat, get_jit_str_cat())
    LLVMAddAttributesToFunction(jit_str_cat, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_str_cat, urange(0, 3), nocapture)
    // void jit_prologue ( void * line_info, int32_t stackSize, JitStackState * stackState, Context * context )
    var jit_prologue = LLVMAddFunctionWithType(g_mod, FN_JIT_PROLOGUE,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMVoidPtrType(), g_prim_t.t_int32, LLVMPointerType(g_t_stack_state, 0u), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_prologue, get_jit_prologue())
    LLVMAddAttributesToFunction(jit_prologue, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_prologue, urange(2, 4), nocapture)
    // void jit_epilogue ( JitStackState * stackState, Context * context )
    var jit_epilogue = LLVMAddFunctionWithType(g_mod, FN_JIT_EPILOGUE,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_stack_state, 0u), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_epilogue, get_jit_epilogue())
    LLVMAddAttributesToFunction(jit_epilogue, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_epilogue, urange(0, 2), nocapture)
    // void jit_make_block ( Block * blk, int32_t argStackTop, uint64_t annotationData, void * bodyNode, void * jitImpl, void * funcInfo, Context * context )
    var jit_make_block = LLVMAddFunctionWithType(g_mod, FN_JIT_MAKE_BLOCK,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(
                LLVMPointerType(g_t_block, 0u), // blk
                g_prim_t.t_int32,               // argStackTop
                g_prim_t.t_int64,               // ad
                g_prim_t.LLVMVoidPtrType(),     // bodyNode
                g_prim_t.LLVMVoidPtrType(),     // jitImpl
                g_prim_t.LLVMVoidPtrType(),     // funcInfo
                g_prim_t.LLVMVoidPtrType(),     // lineInfo
                g_prim_t.LLVMVoidPtrType()      // context
            )))
    LLVMAddGlobalMapping(g_engine, jit_make_block, get_jit_make_block())
    LLVMAddAttributesToFunction(jit_make_block, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgument(jit_make_block, 0u, nocapture)
    LLVMAddAttributeToFunctionArgumentRange(jit_make_block, urange(3, 7), nocapture)
    // void jit_debug ( vec4f res, TypeInfo * typeInfo, char * message, Context * context, LineInfoArg * at )
    var jit_debug = LLVMAddFunctionWithType(g_mod, FN_JIT_DEBUG,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(g_prim_t.LLVMFloat4Type(), g_prim_t.LLVMVoidPtrType(), g_prim_t.get_type_string(), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_debug, get_jit_debug())
    LLVMAddAttributesToFunction(jit_debug, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_debug, urange(1, 5), nocapture)
    // bool builtin_iterator_iterate ( const Sequence & it, void * data, Context * context ) {
    var jit_iterator_iterate = LLVMAddFunctionWithType(g_mod, FN_JIT_ITERATOR_ITERATE,
        LLVMFunctionType(g_prim_t.t_int1,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_sequence, 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_iterator_iterate, get_jit_iterator_iterate())
    LLVMAddAttributesToFunction(jit_iterator_iterate, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_iterator_iterate, urange(0, 3), nocapture)
    // void builtin_iterator_delete ( const Sequence & it, Context * context ) {
    var jit_iterator_delete = LLVMAddFunctionWithType(g_mod, FN_JIT_ITERATOR_DELETE,
        LLVMFunctionType(g_prim_t.t_int1,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_sequence, 0u), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_iterator_delete, get_jit_iterator_delete())
    LLVMAddAttributesToFunction(jit_iterator_delete, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_iterator_delete, urange(0, 2), nocapture)
    // bool builtin_iterator_first ( const Sequence & it, void * data, Context * context, LineInfoArg * at ) {
    var jit_iterator_first = LLVMAddFunctionWithType(g_mod, FN_JIT_ITERATOR_FIRST,
        LLVMFunctionType(g_prim_t.t_int1,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_sequence, 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_iterator_first, get_jit_iterator_first())
    LLVMAddAttributesToFunction(jit_iterator_first, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_iterator_first, urange(0, 4), nocapture)
    // bool builtin_iterator_next ( const Sequence & it, void * data, Context * context, LineInfoArg * at ) {
    var jit_iterator_next = LLVMAddFunctionWithType(g_mod, FN_JIT_ITERATOR_NEXT,
        LLVMFunctionType(g_prim_t.t_int1,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_sequence, 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_iterator_next, get_jit_iterator_next())
    LLVMAddAttributesToFunction(jit_iterator_next, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_iterator_next, urange(0, 4), nocapture)
    // void builtin_iterator_close ( const Sequence & it, void * data, Context * context ) {
    var jit_iterator_close = LLVMAddFunctionWithType(g_mod, FN_JIT_ITERATOR_CLOSE,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_t_sequence, 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_iterator_close, get_jit_iterator_close())
    LLVMAddAttributesToFunction(jit_iterator_close, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_iterator_close, urange(0, 3), nocapture)
    // void das_jit_debug_enter ( char * message, Context * context, LineInfoArg * at ) {
    var jit_debug_enter = LLVMAddFunctionWithType(g_mod, FN_JIT_DEBUG_ENTER,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_prim_t.t_int8, 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_debug_enter, get_jit_debug_enter())
    LLVMAddAttributesToFunction(jit_debug_enter, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_debug_enter, urange(0, 3), nocapture)
    // void das_jit_debug_exit ( char * message, Context * context, LineInfoArg * at ) {
    var jit_debug_exit = LLVMAddFunctionWithType(g_mod, FN_JIT_DEBUG_EXIT,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_prim_t.t_int8, 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_debug_exit, get_jit_debug_exit())
    LLVMAddAttributesToFunction(jit_debug_exit, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_debug_exit, urange(0, 3), nocapture)
    // void das_jit_debug_line ( char * message, Context * context, LineInfoArg * at ) {
    var jit_debug_line = LLVMAddFunctionWithType(g_mod, FN_JIT_DEBUG_LINE,
        LLVMFunctionType(g_prim_t.t_void,
            fixed_array<LLVMTypeRef>(LLVMPointerType(g_prim_t.t_int8, 0u), g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_debug_line, get_jit_debug_line())
    LLVMAddAttributesToFunction(jit_debug_line, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_debug_line, urange(0, 3), nocapture)
    // void * jit_ast_typedecl ( uint64_t hash, Context * context, LineInfoArg * at )
    var jit_ast_typedecl = LLVMAddFunctionWithType(g_mod, FN_JIT_AST_TYPEDECL,
        LLVMFunctionType(g_prim_t.LLVMVoidPtrType(),
            fixed_array<LLVMTypeRef>(g_prim_t.t_int64, g_prim_t.LLVMVoidPtrType(), g_prim_t.LLVMVoidPtrType())))
    LLVMAddGlobalMapping(g_engine, jit_ast_typedecl, get_jit_ast_typedecl())
    LLVMAddAttributesToFunction(jit_ast_typedecl, fixed_array(nounwind, willreturn))
    LLVMAddAttributeToFunctionArgumentRange(jit_ast_typedecl, urange(1, 3), nocapture)
}


def private build_jit_types {
    // simfunction type
    var simfunction_fields <- fixed_array(
        LLVMPointerType(g_prim_t.t_int8, 0u),// char * name
        LLVMPointerType(g_prim_t.t_int8, 0u),// char * mangledName
        g_prim_t.LLVMVoidPtrType(),// SimNode * code               // note void *
        g_prim_t.LLVMVoidPtrType(),// FuncInfo *  debugInfo        // note void *
        g_prim_t.t_int64,                    // uint64_t    mangledNameHash
        g_prim_t.LLVMVoidPtrType(),// void *      aotFunction
        g_prim_t.t_int32,                    // uint32_t    stackSize
        g_prim_t.t_int32                     // uint32_t    flags
    )
    g_t_simFunction = g_prim_t |> StructType(simfunction_fields)
    // function type
    var function_fields <- fixed_array(
        LLVMPointerType(g_t_simFunction, 0u)// SimFunction * func
    )
    g_t_function = g_prim_t |> StructType(function_fields)
    // lambda type
    var lambda_fields <- fixed_array(
        LLVMPointerType(g_t_simFunction, 0u),   // SimFunction * __lambda
        LLVMPointerType(g_t_simFunction, 0u)    // SimFunction * __finalize
    )
    g_t_lambda = g_prim_t |> StructType(lambda_fields)
    // block type
    var block_fields <- build_block_type()
    g_t_block = g_prim_t |> StructType(block_fields)
    // prologue\epilogue storage for the stack
    var stack_state_fields <- fixed_array(
        LLVMPointerType(g_prim_t.t_int8, 0u),   // char * EP
        LLVMPointerType(g_prim_t.t_int8, 0u)    // char * SP
    )
    g_t_stack_state = g_prim_t |> StructType(stack_state_fields)
    // iterator
    var iterator_fields <- fixed_array(
        g_prim_t.LLVMVoidPtrType(),   // __vtable
        g_prim_t.t_int1                         // bool isOpen
    )
    g_t_iterator = g_prim_t |> StructType(iterator_fields)
    // sequence
    var sequence_fields <- fixed_array(
        LLVMPointerType(g_t_iterator, 0u)       // iterator * iter
    )
    g_t_sequence = g_prim_t |> StructType(sequence_fields)
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

[macro_function]
def public with_default_target_machine(blk : block<(LLVMTargetMachineRef) : void>) {
    LLVMInitializeAllTargetInfos()
    LLVMInitializeAllTargets()
    LLVMInitializeAllTargetMCs()
    LLVMInitializeAllAsmPrinters()

    // let triple = "x86_64-pc-linux-gnu"
    let triple = LLVMGetDefaultTargetTriple()

    // Find the target using the triple
    var target : LLVMTarget?
    let error : string?
    LLVMGetTargetFromTriple(triple, unsafe(addr(target)), error)

    let targetMachine = LLVMCreateTargetMachine(
        target,
        triple,
        "", // CPU
        "", // Features
        LLVMCodeGenOptLevel.LLVMCodeGenLevelDefault,
        LLVMRelocMode.LLVMRelocPIC, // PIC in order to have shared object
        LLVMCodeModel.Default
    )

    invoke(blk, targetMachine)

    LLVMDisposeMessage(triple)
    LLVMDisposeTargetMachine(targetMachine)
}


[macro_function]
def public finalize_jit(prog : Program?) {
    return if (is_in_completion() || is_compiling_macros())
    return if (g_mod == null)    // prevent double finalize
    if (LLVM_LOG_RESULT) {
        LLVMDumpModule(g_mod)
        fflush(fstderr())
    }
    if (LLVM_OUTPUT_OJBECT_FILE) {
        with_default_target_machine <| $(targetMachine : LLVMTargetMachineRef) {
            let error : string?
            let file = LLVM_OUTPUT_OJBECT_FILE_NAME
            let filetype = LLVMCodeGenFileType.LLVMObjectFile
            LLVMTargetMachineEmitToFile(targetMachine, g_mod, "{file}{prog.thisNamespace}", filetype, error)
            // create_shared_library(file, JIT_DLL_OUTPUT, DASCRIPT_LIBRARY)
        }
    }
    g_engine |> LLVMRunStaticConstructors()
    g_all_engines |> push(JitEngine(
        mod <- g_mod,
        engine <- g_engine,
        ctx <- g_ctx
    ))
    unsafe {
        delete g_str2v
    }
}

[finalize, no_jit] // no_jit is necessary!
def private finalize_all_jit {
    for (je in g_all_engines) {
        LLVMDisposeExecutionEngine(je.engine); je.engine = null
        g_t_function = null
        g_t_simFunction = null
        g_t_block = null
        g_t_lambda = null
        g_t_stack_state = null
        g_t_iterator = null
        g_t_sequence = null
    }
        // TODO: figure out why this crashes
        // LLVMDisposeModule(je.mod); je.mod = null
    unsafe {
        delete g_all_engines
    }
}

def public build_string_constant(message : string) {
    if (g_str2v |> key_exists(message)) {
        return g_str2v[message]
    }
    let msg_size = uint(length(message))
    var strType = LLVMArrayType(g_prim_t.t_int8, msg_size + 1u)
    var str = LLVMAddGlobal(g_mod, strType, "{message}")
    LLVMSetInitializer(str, LLVMConstStringInContext(g_ctx, message, msg_size, 0))
    LLVMSetGlobalConstant(str, 1)
    LLVMSetLinkage(str, LLVMLinkage.LLVMPrivateLinkage)
    LLVMSetUnnamedAddress(str, LLVMUnnamedAddr.LLVMGlobalUnnamedAddr)
    LLVMSetAlignment(str, 1u)
    g_str2v[message] = str
    return str
}

def public get_basetype_pointer(t : Type) : LLVMOpaqueType? {
    return LLVMPointerType(base_type_to_llvm_type(t), 0u)
}

def public get_type_pointer(t : TypeDeclPtr) : LLVMOpaqueType? {
    if (t == null || t.isVoid) {
        return g_prim_t.LLVMVoidPtrType()
    } else {
        return LLVMPointerType(type_to_llvm_type(t), 0u)
    }
}

def public get_string_constant_ptr(builder : LLVMOpaqueBuilder?; message : string) {
    if (message |> length == 0) {
        return LLVMConstPointerNull(g_prim_t.get_type_string())
    }
    var str = build_string_constant(message)
    return LLVMBuildPointerCast(builder, str, g_prim_t.get_type_string(), "string_constant {message}")
}

def public failed(msg : string) {
    static_if (LLVM_DEBUG_EVERYTHING) {
        print("FAILED: {msg}\n")
    }
    g_errors |> push("{msg}")
}

def public failed_E(e : ExpressionPtr; msg : string) {
    failed("{msg}. expr = {e.__rtti} at {describe(e.at)}")
}

def public failed_E(e : Expression?; msg : string) {
    failed("{msg}. expr = {e.__rtti} at {describe(e.at)}")
}

def public failed_V(v : VariablePtr; msg : string) {
    failed("{msg}. var = {describe(v)} at {describe(v.at)}")
}

def public failed_V(v : Variable?; msg : string) {
    failed("{msg}. var = {v.name} at {describe(v.at)}")
}

def public failed_T(v : TypeDeclPtr; msg : string) {
    failed("{msg}. var = {describe(v)} at {describe(v.at)}")
}

def public base_type_to_llvm_type(t : Type) {
    return g_prim_t.t_int32 if (t == Type.tInt || t == Type.tUInt || t == Type.tBitfield || t == Type.tEnumeration)
    return g_prim_t.t_int64 if (t == Type.tInt64 || t == Type.tUInt64 || t == Type.tEnumeration64)
    return g_prim_t.t_int16 if (t == Type.tInt16 || t == Type.tUInt16 || t == Type.tEnumeration16)
    return g_prim_t.t_int8 if (t == Type.tInt8 || t == Type.tUInt8 || t == Type.tEnumeration8)
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
    failed("base_type_to_llvm_type: unsupported type {t}")
    return g_prim_t.t_int32
}

def public type_to_llvm_abi_type(t : TypeDeclPtr) {
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
            var inscope tp <- get_underlying_value_type(t)
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
    var res : LLVMOpaqueType?
    if (t.dim |> length != 0) {
        var inscope ndt <- clone_type(t)
        ndt.dim |> clear()
        res = type_to_llvm_type(ndt)
        for (d in t.dim) {
            res = LLVMArrayType(res, uint(d))
        }
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
        // array type
        var array_fields <- fixed_array(
            g_prim_t.LLVMVoidPtrType(),// char * data
            g_prim_t.t_int32,                    // uint32_t size
            g_prim_t.t_int32,                    // uint32_t capacity
            g_prim_t.t_int32,                    // uint32_t lock
            g_prim_t.t_int32                     // uint32_t flags
        )
        if (t.firstType != null) {
            array_fields[int(JIT_ARRAY.DATA)] = LLVMPointerType(type_to_llvm_type(t.firstType), 0u)
        }
        res = g_prim_t |> StructType(array_fields)
    } elif (t.baseType == Type.tTable) {
        // table type
        var table_fields <- fixed_array(
            g_prim_t.LLVMVoidPtrType(),// char * data
            g_prim_t.t_int32,                    // uint32_t size
            g_prim_t.t_int32,                    // uint32_t capacity
            g_prim_t.t_int32,                    // uint32_t lock
            g_prim_t.t_int32,                    // uint32_t flags
            g_prim_t.LLVMVoidPtrType(),// char * keys
            g_prim_t.LLVMVoidPtrType(),// char * hashes
            g_prim_t.t_int32)
        if (t.firstType != null) {
            table_fields[int(JIT_TABLE.KEYS)] = LLVMPointerType(type_to_llvm_type(t.firstType), 0u)
        }
        if (t.secondType != null) {
            table_fields[int(JIT_ARRAY.DATA)] = t.secondType.isVoid ? g_prim_t.LLVMVoidPtrType() : LLVMPointerType(type_to_llvm_type(t.secondType), 0u)
        }
        res = g_prim_t |> StructType(table_fields)
    } elif (t.isVoid) {
        res = g_prim_t.t_void
    } elif (t.isAnyType) {
        res = g_prim_t.LLVMFloat4Type()
    } elif (t.isHandle) {
        if (t.isRefType) {
            res = LLVMArrayType(g_prim_t.t_int8, uint(t.sizeOf))
        } else {
            var inscope tp <- get_underlying_value_type(t)
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
    if (typ.dim |> length == 0) {
        return type_to_llvm_type(typ)
    } else {
        var inscope ndt <- clone_type(typ)
        ndt.dim |> clear()
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
        // slow path
        return LLVMBuildLoadVector3(builder, g_prim_t, typ, ptr, name)
    } else {
        return LLVMBuildLoad2Aligned(builder, typ, ptr, alignment |> uint, name)
    }
}

def public LLVMBuildMemSet(builder : LLVMOpaqueBuilder?; ptr : LLVMOpaqueValue?; value : uint64; length : uint64; alignment : uint) {
    LLVMBuildMemSet(builder, ptr, LLVMConstInt(g_prim_t.t_int8, value, 0), g_prim_t.ConstI32(length), uint(alignment))
}

def public expand_scalar(builder : LLVMOpaqueBuilder?; scalar : LLVMOpaqueValue?; vecType : TypeDeclPtr) {
    var vec = LLVMGetUndef(type_to_llvm_type(vecType))
    for (d in range(vecType.vectorDim)) {
        var idx = g_prim_t.ConstI32(uint64(d))
        vec = LLVMBuildInsertElement(builder, vec, scalar, idx, "")
    }
    return vec
}

