#pragma once

#include <stdint.h>

#if DAS_ENABLE_DLL
#ifndef DAS_API
#ifdef _MSC_VER
    #define DAS_EXPORT_DLL __declspec(dllexport)
    #define DAS_IMPORT_DLL __declspec(dllimport)
#else
    #define DAS_EXPORT_DLL __attribute__((visibility("default")))
    #define DAS_IMPORT_DLL
#endif
#ifdef DAS_EXPORTS
    #define DAS_API DAS_EXPORT_DLL
#else
    #define DAS_API DAS_IMPORT_DLL
#endif
#endif
#else
#define DAS_API
#endif
//if target is not defined, try to auto-detect target
#ifndef _TARGET_SIMD_SSE
    #if __SSE4_1__ || defined(__AVX__) || defined(__AVX2__)
        #define _TARGET_SIMD_SSE 4
    #elif __SSSE3__
        #define _TARGET_SIMD_SSE 3
    #elif defined(__SSE2__) || defined(_M_AMD64) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP>=1)
        #define _TARGET_SIMD_SSE 2
    #endif
#endif

#if !defined(_TARGET_SIMD_SSE) && !defined(_TARGET_SIMD_NEON)
    #if defined(__ARM_NEON) || defined(__ARM_NEON__)
        #define _TARGET_SIMD_NEON 1
    #endif
#endif

#if defined(_TARGET_SIMD_SSE)
    #include <emmintrin.h>
    typedef __m128 vec4f;
    typedef __m128i vec4i;
#elif defined(_TARGET_SIMD_NEON) // PSP2, iOS
    #include <arm_neon.h>
    typedef float32x4_t vec4f;
    typedef int32x4_t   vec4i;
#else
    #error unsupported target
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern DAS_API uint32_t SIDEEFFECTS_none;
extern DAS_API uint32_t SIDEEFFECTS_unsafe;
extern DAS_API uint32_t SIDEEFFECTS_userScenario;
extern DAS_API uint32_t SIDEEFFECTS_modifyExternal;
extern DAS_API uint32_t SIDEEFFECTS_accessExternal;
extern DAS_API uint32_t SIDEEFFECTS_modifyArgument;
extern DAS_API uint32_t SIDEEFFECTS_modifyArgumentAndExternal;
extern DAS_API uint32_t SIDEEFFECTS_worstDefault;
extern DAS_API uint32_t SIDEEFFECTS_accessGlobal;
extern DAS_API uint32_t SIDEEFFECTS_invoke;
extern DAS_API uint32_t SIDEEFFECTS_inferredSideEffects;

typedef struct dasTextOutputHandle das_text_writer;
typedef struct dasModuleGroupHandle das_module_group;
typedef struct dasFileAccessHandle das_file_access;
typedef struct dasProgram das_program;
typedef struct dasContext das_context;
typedef struct dasFunction das_function;
typedef struct dasError das_error;
typedef struct dasModule das_module;
typedef struct dasNode das_node;
typedef struct dasStructure das_structure;
typedef struct dasEnumeration das_enumeration;
typedef struct dasLambda das_lambda;
typedef struct dasBlock das_block;

typedef struct {
    float x, y, z, w;
} vec4f_unaligned;

typedef struct {
    char *      data;
    uint32_t    size;
    uint32_t    capacity;
    uint32_t    lock;
    uint32_t    flags;
} das_array;

typedef struct {
    char *      data;
    uint32_t    size;
    uint32_t    capacity;
    uint32_t    lock;
    uint32_t    flags;
    char *      keys;
    uint32_t *  hashes;
    uint32_t    tombstones;
} das_table;

typedef vec4f (das_interop_function) ( das_context * ctx, das_node * node, vec4f * arguments );

typedef void (das_interop_function_unaligned) ( das_context * ctx, das_node * node, vec4f_unaligned * arguments, vec4f_unaligned * result );

// --- Lifecycle ---

// Initialize all built-in modules and internal structures.
// Must be called once before any other daScript C API call.
DAS_API void das_initialize();
// Shut down the daScript runtime and free all internal structures and memory.
// Must be called once when the host application is done with daScript.
DAS_API void das_shutdown();

// --- Text output ---

// Create a text writer that prints to stdout.
// Use das_text_make_writer() instead if you need to capture output as a string.
DAS_API das_text_writer * das_text_make_printer();
// Create a text writer that accumulates output in an internal string buffer.
// Use das_text_make_printer() instead if you just want stdout output.
DAS_API das_text_writer * das_text_make_writer();
// Release a text writer created by das_text_make_printer() or das_text_make_writer().
DAS_API void das_text_release ( das_text_writer * output );
// Write a null-terminated string to a text writer.
DAS_API void das_text_output ( das_text_writer * output, char * text );

// --- Module groups ---

// Create a module group. A module group holds references to modules
// that the compiler should use when resolving `require` statements.
DAS_API das_module_group * das_modulegroup_make ();
// Add a module to a module group so that compiled scripts can `require` it.
DAS_API void das_modulegroup_add_module ( das_module_group* lib, das_module* mod );
// Release a module group created by das_modulegroup_make().
DAS_API void das_modulegroup_release ( das_module_group * group );

// --- Dynamic modules ---

// Scan <project_root>/modules/ for .das_module descriptor files and
// call `initialize` on each one.
//
// If tout is NULL, diagnostic output goes to stdout.
//
// Returns 0 on success.
DAS_API int das_register_dynamic_modules(das_file_access *file_access,
                                         const char *project_root,
                                         das_text_writer *tout);

// --- File access ---

// Create a default file access that reads directly from the filesystem.
DAS_API das_file_access * das_fileaccess_make_default (  );
// Create a file access backed by a .das_project file.
// The project file is a daScript program that exports callback functions
// controlling module resolution, permissions, and compilation policies.
// See tutorial 06 (sandbox) for a complete example.
DAS_API das_file_access * das_fileaccess_make_project ( const char * project_file  );
// Release a file access created by das_fileaccess_make_default() or das_fileaccess_make_project().
DAS_API void das_fileaccess_release ( das_file_access * access );
// Register a virtual file from a C string.
// The file appears at 'file_name' during compilation even though it does not exist on disk.
// If 'owns' is non-zero, the content is copied internally and the caller may free file_content.
// If 'owns' is zero, the content is borrowed and the caller must keep file_content alive
// for the lifetime of the file access.
DAS_API void das_fileaccess_introduce_file ( das_file_access * access, const char * file_name, const char * file_content, int owns );

// Read a file from disk at 'disk_path' and cache it under the virtual path 'name'.
// 'name' is what module resolution returns (e.g. "daslib/strings.das");
// 'disk_path' is the actual location on the filesystem.
DAS_API void das_fileaccess_introduce_file_from_disk ( das_file_access * access, const char * name, const char * disk_path );
// Pre-load all standard library modules from getDasRoot()/daslib/ into the file access cache.
// Typically called before das_fileaccess_lock() when setting up a sandbox.
DAS_API void das_fileaccess_introduce_daslib ( das_file_access * access );
// Pre-load all native plugin modules listed in external_resolve.inc into the file access cache.
// Modules not present on disk are silently skipped.
DAS_API void das_fileaccess_introduce_native_modules ( das_file_access * access );
// Pre-load a single native module by its require-style path (e.g. "opengl/opengl_boost").
// Returns 1 if the module file was found and cached, 0 otherwise.
DAS_API int das_fileaccess_introduce_native_module ( das_file_access * access, const char * req );

// Lock the file access. While locked, getNewFileInfo() returns NULL for all
// files not already in the cache, so only pre-introduced files can be accessed.
DAS_API void das_fileaccess_lock ( das_file_access * access );
// Unlock the file access, re-enabling filesystem reads and further introduce calls.
DAS_API void das_fileaccess_unlock ( das_file_access * access );
// Return 1 if the file access is locked, 0 otherwise.
DAS_API int das_fileaccess_is_locked ( das_file_access * access );

// Copy the daScript root path into 'root'. At most 'maxbuf' bytes are written
// (including the null terminator). The root path is used to locate daslib/ and other resources.
DAS_API void das_get_root ( char * root, int maxbuf );

// --- Compilation ---

// Compile a daScript program from the file at 'program_file'.
// Compiler diagnostics are written to 'tout'. The returned program is always
// non-NULL; check das_program_err_count() to determine if compilation succeeded.
DAS_API das_program * das_program_compile ( char * program_file, das_file_access * access, das_text_writer * tout, das_module_group * libgroup );
// Release a compiled program.
DAS_API void das_program_release ( das_program * program );
// Return the number of compilation errors. Zero means the program compiled successfully.
DAS_API int das_program_err_count ( das_program * program );
// Return the minimum context stack size (in bytes) needed to simulate this program.
// Pass this value to das_context_make().
DAS_API int das_program_context_stack_size ( das_program * program );
// Simulate (link) a compiled program into a context.
// Returns 1 on success, 0 on failure. On failure, error details can be retrieved
// with das_program_get_error(). The text writer 'tout' receives diagnostic output.
DAS_API int das_program_simulate ( das_program * program, das_context * ctx, das_text_writer * tout );
// Return the i-th compilation or simulation error, or NULL if index is out of range.
// Valid indices are [0, das_program_err_count(program)).
DAS_API das_error * das_program_get_error ( das_program * program, int index );

// --- Errors ---

// Write a human-readable error description to a text writer.
DAS_API void das_error_output ( das_error * error, das_text_writer * tout );
// Write a human-readable error description into 'text' (at most 'maxLength' bytes,
// including the null terminator). Truncated if the message exceeds maxLength.
DAS_API void das_error_report ( das_error * error, char * text, int maxLength );

// --- Context ---

// Create an execution context with the given stack size (in bytes).
// stackSize must be >= das_program_context_stack_size(program) for the program
// you intend to simulate.
DAS_API das_context * das_context_make ( int stackSize );
// Release an execution context.
DAS_API void das_context_release ( das_context * context );
// Find an exported function by name in a simulated context.
// Returns NULL if no function with that name exists.
DAS_API das_function * das_context_find_function ( das_context * context, char * name );

// --- Function evaluation (aligned, vec4f) ---
// All arguments and return values are passed as vec4f (16-byte aligned SIMD registers).
// Use the das_argument_* helpers to pack arguments and das_result_* to unpack results.
// If the function throws an exception, retrieve it with das_context_get_exception().

// Call a function. Returns the result as vec4f.
DAS_API vec4f das_context_eval_with_catch ( das_context * context, das_function * fun, vec4f * arguments );

// --- Function evaluation (unaligned, vec4f_unaligned) ---
// Same semantics as above, but arguments and result use vec4f_unaligned (no alignment requirement).
// Prefer these when calling from plain C code that cannot guarantee 16-byte alignment.
// 'narguments' is the number of elements in the arguments array.

// Call a function with unaligned arguments. Result is written into *result.
DAS_API void das_context_eval_with_catch_unaligned ( das_context * context, das_function * fun, vec4f_unaligned * arguments, int narguments, vec4f_unaligned * result );

// --- Function evaluation returning complex results (cmres) ---
// Functions that return structures, tuples, or other types that do not fit in a single
// register use a caller-allocated buffer (cmres). The caller must allocate enough memory
// for the return type and pass it via the 'cmres' pointer.

// Call a function that returns a complex result (aligned arguments).
DAS_API void das_context_eval_with_catch_cmres ( das_context * context, das_function * fun, vec4f * arguments, void * cmres );
// Call a function that returns a complex result (unaligned arguments).
DAS_API void das_context_eval_with_catch_cmres_unaligned ( das_context * context, das_function * fun, vec4f_unaligned * arguments, int narguments, void * cmres );
// Return the current exception message, or NULL if no exception occurred.
DAS_API char * das_context_get_exception ( das_context * context );

// --- Lambda evaluation ---
// Lambdas are heap-allocated closures. The same aligned / unaligned / cmres
// variants apply as for function evaluation above.

// Call a lambda (aligned). Returns result as vec4f.
DAS_API vec4f das_context_eval_lambda ( das_context * context, das_lambda * lambda, vec4f * arguments );
// Call a lambda (unaligned). Result written into *result.
DAS_API void das_context_eval_lambda_unaligned ( das_context * context, das_lambda * lambda, vec4f_unaligned * arguments, int narguments, vec4f_unaligned * result );
// Call a lambda returning a complex result (aligned).
DAS_API void das_context_eval_lambda_cmres ( das_context * context, das_lambda * lambda, vec4f * arguments, void * cmres );
// Call a lambda returning a complex result (unaligned).
DAS_API void das_context_eval_lambda_cmres_unaligned ( das_context * context, das_lambda * lambda, vec4f_unaligned * arguments, int narguments, void * cmres );

// --- Block evaluation ---
// Blocks are stack-allocated closures. The same aligned / unaligned / cmres
// variants apply as for function evaluation above.

// Call a block (aligned). Returns result as vec4f.
DAS_API vec4f das_context_eval_block ( das_context * context, das_block * block, vec4f * arguments );
// Call a block (unaligned). Result written into *result.
DAS_API void das_context_eval_block_unaligned ( das_context * context, das_block * block, vec4f_unaligned * arguments, int narguments, vec4f_unaligned * result );
// Call a block returning a complex result (aligned).
DAS_API void das_context_eval_block_cmres ( das_context * context, das_block * block, vec4f * arguments, void * cmres );
// Call a block returning a complex result (unaligned).
DAS_API void das_context_eval_block_cmres_unaligned ( das_context * context, das_block * block, vec4f_unaligned * arguments, int narguments, void * cmres );

// --- Type binding: structures ---

// Create a handled structure type that mirrors a C struct.
// 'name' is the daScript name, 'cppname' is the C/C++ type name used for mangling,
// 'sz' and 'al' are sizeof() and alignof() of the C struct.
// The returned handle must be populated with das_structure_add_field() and then
// registered into a module with das_module_bind_structure().
DAS_API das_structure * das_structure_make ( das_module_group * lib, const char * name, const char * cppname, int sz, int al );
// Add a field to a structure created by das_structure_make().
// 'offset' is offsetof() of the field in the C struct.
// 'tname' is the field type in mangled-name format (see type_mangling.rst).
DAS_API void das_structure_add_field ( das_structure * st, das_module * mod, das_module_group * lib,  const char * name, const char * cppname, int offset, const char * tname );

// --- Type binding: enumerations ---

// Create an enumeration type.
// 'name' is the daScript name, 'cppname' is the C/C++ type name.
// 'ext' is non-zero if the enum's underlying storage is int64 (otherwise int).
// Populate with das_enumeration_add_value(), then register with das_module_bind_enumeration().
DAS_API das_enumeration * das_enumeration_make ( const char * name, const char * cppname, int ext );
// Add a named value to an enumeration created by das_enumeration_make().
DAS_API void das_enumeration_add_value ( das_enumeration * enu, const char * name, const char * cppName, int value );

// --- Modules ---

// Create a new module with the given name.
// After populating it with bindings, add it to a module group with das_modulegroup_add_module().
DAS_API das_module * das_module_create ( char * name );
// Bind a C interop function (aligned, vec4f calling convention) to a module.
// 'name' is the daScript function name, 'cppName' is the C symbol name.
// 'sideffects' is a combination of SIDEEFFECTS_* flags.
// 'args' is a mangled signature string describing argument and return types
// (see type_mangling.rst).
DAS_API void das_module_bind_interop_function ( das_module * mod, das_module_group * lib, das_interop_function * fun, char * name, char * cppName, uint32_t sideffects, char* args );
// Bind a C interop function (unaligned, vec4f_unaligned calling convention) to a module.
// Same parameters as das_module_bind_interop_function(); use this variant when the
// interop function uses vec4f_unaligned arguments and result.
DAS_API void das_module_bind_interop_function_unaligned ( das_module * mod, das_module_group * lib, das_interop_function_unaligned * fun, char * name, char * cppName, uint32_t sideffects, char* args );
// Bind a type alias to a module. 'aname' is the alias name visible in daScript,
// 'tname' is the target type in mangled-name format.
DAS_API void das_module_bind_alias ( das_module * mod, das_module_group * lib, char * aname, char * tname );
// Register a structure (from das_structure_make()) into a module.
DAS_API void das_module_bind_structure ( das_module * mod, das_structure * st );
// Register an enumeration (from das_enumeration_make()) into a module.
DAS_API void das_module_bind_enumeration ( das_module * mod, das_enumeration * en );

// --- String allocation ---

// Allocate a copy of 'str' on the daScript string heap.
// Use this to return strings from interop functions; the returned pointer
// is managed by the context and must not be freed by the caller.
char * das_allocate_string ( das_context * context, char * str );

// --- Argument getters (aligned) ---
// Extract a C value from a vec4f argument inside an interop function.

DAS_API int    das_argument_int ( vec4f arg );
DAS_API unsigned int   das_argument_uint ( vec4f arg );
DAS_API long long das_argument_int64 ( vec4f arg );
DAS_API unsigned long long das_argument_uint64 ( vec4f arg );
DAS_API int    das_argument_bool ( vec4f arg );
DAS_API float  das_argument_float ( vec4f arg );
DAS_API double das_argument_double ( vec4f arg );
DAS_API char * das_argument_string ( vec4f arg );
DAS_API void * das_argument_ptr ( vec4f arg );
DAS_API das_function * das_argument_function ( vec4f arg );
DAS_API das_lambda * das_argument_lambda ( vec4f arg );
DAS_API das_block * das_argument_block ( vec4f arg );

// --- Argument getters (unaligned) ---
// Extract a C value from a vec4f_unaligned argument pointer.
// Use these in unaligned interop functions.

DAS_API int das_argument_bool_unaligned ( vec4f_unaligned * arg );
DAS_API int das_argument_int_unaligned ( vec4f_unaligned * arg );
DAS_API unsigned int das_argument_uint_unaligned ( vec4f_unaligned * arg );
DAS_API long long das_argument_int64_unaligned ( vec4f_unaligned * arg );
DAS_API unsigned long long das_argument_uint64_unaligned ( vec4f_unaligned * arg );
DAS_API float das_argument_float_unaligned ( vec4f_unaligned * arg );
DAS_API double das_argument_double_unaligned ( vec4f_unaligned * arg );
DAS_API char * das_argument_string_unaligned ( vec4f_unaligned * arg );
DAS_API void * das_argument_ptr_unaligned ( vec4f_unaligned * arg );
DAS_API das_function * das_argument_function_unaligned ( vec4f_unaligned * arg );
DAS_API das_lambda * das_argument_lambda_unaligned ( vec4f_unaligned * arg );
DAS_API das_block * das_argument_block_unaligned ( vec4f_unaligned * arg );

// --- Result setters (aligned) ---
// Pack a C value into vec4f for returning from an aligned interop function.

DAS_API vec4f das_result_void ();
DAS_API vec4f das_result_int ( int r );
DAS_API vec4f das_result_uint ( unsigned int r );
DAS_API vec4f das_result_int64 ( long long r );
DAS_API vec4f das_result_uint64 ( unsigned long long r );
DAS_API vec4f das_result_bool ( int r );
DAS_API vec4f das_result_float ( float r );
DAS_API vec4f das_result_double ( double r );
DAS_API vec4f das_result_string ( char * r );
DAS_API vec4f das_result_ptr ( void * r );
DAS_API vec4f das_result_function ( das_function * r );
DAS_API vec4f das_result_lambda ( das_lambda * r );
DAS_API vec4f das_result_block ( das_block * r );

// --- Result setters (unaligned) ---
// Write a C value into a vec4f_unaligned result pointer.
// Use these in unaligned interop functions.

DAS_API void das_result_void_unaligned ( vec4f_unaligned * result );
DAS_API void das_result_int_unaligned ( vec4f_unaligned * result, int r );
DAS_API void das_result_uint_unaligned ( vec4f_unaligned * result, unsigned int r );
DAS_API void das_result_int64_unaligned ( vec4f_unaligned * result, long long r );
DAS_API void das_result_uint64_unaligned ( vec4f_unaligned * result, unsigned long long r );
DAS_API void das_result_bool_unaligned ( vec4f_unaligned * result, int r );
DAS_API void das_result_float_unaligned ( vec4f_unaligned * result, float r );
DAS_API void das_result_double_unaligned ( vec4f_unaligned * result, double r );
DAS_API void das_result_string_unaligned ( vec4f_unaligned * result, char * r );
DAS_API void das_result_ptr_unaligned ( vec4f_unaligned * result, void * r );
DAS_API void das_result_function_unaligned ( vec4f_unaligned * result, das_function * r );
DAS_API void das_result_lambda_unaligned ( vec4f_unaligned * result, das_lambda * r );
DAS_API void das_result_block_unaligned ( vec4f_unaligned * result, das_block * r );

#ifdef __cplusplus
}
#endif
