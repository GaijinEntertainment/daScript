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

// initialize all default modules and internal structures. this is required before any other call to daScriptC API
DAS_API void das_initialize();
// shutdown daScriptC. this is required to free all internal structures and memory allocated by daScriptC API
DAS_API void das_shutdown();

// make a text writer which also prints to stdout. for stringstream use TextWriter
DAS_API das_text_writer * das_text_make_printer();
// make a text writer which writes to stringstream. for stdout use TextPrinter
DAS_API das_text_writer * das_text_make_writer();
// release text writer
DAS_API void das_text_release ( das_text_writer * output );
// write text to text writer
DAS_API void das_text_output ( das_text_writer * output, char * text );

// make a module group
DAS_API das_module_group * das_modulegroup_make ();
// add module to module group
DAS_API void das_modulegroup_add_module ( das_module_group* lib, das_module* mod );
// release module group
DAS_API void das_modulegroup_release ( das_module_group * group );

// Looks for <path>/modules/<module_name>/.das_module files and call
// `initialize` on them.
//
// If tout == nullptr prints logs to stdout.
//
// Returns 0 on success.
DAS_API int das_register_dynamic_modules(das_file_access *file_access,
                                         const char *project_root,
                                         das_text_writer *tout);

// make default file access
DAS_API das_file_access * das_fileaccess_make_default (  );
// make file access from a project file. project file is a .das_project script which specifies access, module lookup paths, permissions, etc.
DAS_API das_file_access * das_fileaccess_make_project ( const char * project_file  );
// release file access
DAS_API void das_fileaccess_release ( das_file_access * access );
// introduce file content to file access. this is used to provide content of a file without actually having it on disk. this is useful for testing and for providing virtual files.
// if owns is non-zero, the content is copied internally and the caller does not need to keep file_content alive.
// if owns is zero, the content is borrowed and the caller must keep file_content alive for the lifetime of the file access.
DAS_API void das_fileaccess_introduce_file ( das_file_access * access, const char * file_name, const char * file_content, int owns );

// introduce file from disk. reads file at disk_path and caches it under name 'name' in the file access.
// 'name' is the virtual path (matching what getModuleInfo returns), 'disk_path' is the actual file location on disk.
DAS_API void das_fileaccess_introduce_file_from_disk ( das_file_access * access, const char * name, const char * disk_path );
// pre-load all daslib modules from getDasRoot()/daslib/ directory into the file access cache.
// this is typically called before locking the file access for sandboxed execution.
DAS_API void das_fileaccess_introduce_daslib ( das_file_access * access );
// pre-load all native modules (from external_resolve.inc) into the file access cache.
// modules not present on disk are silently skipped.
DAS_API void das_fileaccess_introduce_native_modules ( das_file_access * access );
// pre-load a specific native module by require-style path (e.g. "opengl/opengl_boost").
// returns 1 if the module was found and loaded, 0 otherwise.
DAS_API int das_fileaccess_introduce_native_module ( das_file_access * access, const char * req );

// lock the file access. when locked, getNewFileInfo returns nullptr for all files,
// so only pre-cached files (via introduce functions or setFileInfo) can be accessed.
DAS_API void das_fileaccess_lock ( das_file_access * access );
// unlock the file access. allows introducing more files and accessing the filesystem again.
DAS_API void das_fileaccess_unlock ( das_file_access * access );
// returns 1 if the file access is locked, 0 otherwise.
DAS_API int das_fileaccess_is_locked ( das_file_access * access );

// get root path to daScript. this is used to find modules, libraries, etc.
DAS_API void das_get_root ( char * root, int maxbuf );

// compile daScript program from file. return compiled program (regardless is it failed). compiler messages are written to text writer.
DAS_API das_program * das_program_compile ( char * program_file, das_file_access * access, das_text_writer * tout, das_module_group * libgroup );
// release compiled program
DAS_API void das_program_release ( das_program * program );
// get number of errors in compiled program
DAS_API int das_program_err_count ( das_program * program );
// get size of context stack required to simulate compiled program. this is used to allocate context with sufficient stack size.
DAS_API int das_program_context_stack_size ( das_program * program );
// simulate compiled program with provided context and text writer for output. return 1 if simulation succeeded, 0 if it failed. in case of failure, error details can be obtained with das_program_get_error.
DAS_API int das_program_simulate ( das_program * program, das_context * ctx, das_text_writer * tout );
// get error details from compiled program by index. index should be in range [0, das_program_err_count(program)). return nullptr if index is out of range.
DAS_API das_error * das_program_get_error ( das_program * program, int index );

// report error details to text writer
DAS_API void das_error_output ( das_error * error, das_text_writer * tout );
// report error details to text buffer. text buffer should have at least maxLength bytes allocated. if error message is longer than maxLength, it will be truncated.
DAS_API void das_error_report ( das_error * error, char * text, int maxLength );

// create daScript context with specified stack size. stack size should be at least das_program_context_stack_size(program) to simulate compiled program.
DAS_API das_context * das_context_make ( int stackSize );
// release daScript context
DAS_API void das_context_release ( das_context * context );
// find function in daScript context by name. return nullptr if function is not found.
DAS_API das_function * das_context_find_function ( das_context * context, char * name );

// evaluate function in daScript context with provided arguments. arguments should be in format of vec4f array, where each argument is represented as vec4f.
// return value is also represented as vec4f. if evaluation throws an exception, it can be obtained with das_context_get_exception.
DAS_API vec4f das_context_eval_with_catch ( das_context * context, das_function * fun, vec4f * arguments );
// evaluate function in daScript context with provided arguments. arguments should be in format of vec4f_unaligned array, where each argument is represented as vec4f_unaligned.
// this is used when arguments are not guaranteed to be 16-byte aligned. result is also represented as vec4f_unaligned. if evaluation throws an exception, it can be obtained with das_context_get_exception.
DAS_API void das_context_eval_with_catch_unaligned ( das_context * context, das_function * fun, vec4f_unaligned * arguments, int narguments, vec4f_unaligned * result );
// evaluate function in daScript context with provided arguments. arguments should be in format of vec4f array, where each argument is represented as vec4f.
// this is used when function returns complex result (structure, tuple, etc which is not passed by value).
// complex result is returned via cmres pointer, which should point to memory allocated by caller with sufficient size to hold the result.
// if evaluation throws an exception, it can be obtained with das_context_get_exception.
DAS_API void das_context_eval_with_catch_cmres ( das_context * context, das_function * fun, vec4f * arguments, void * cmres );
// evaluate function in daScript context with provided arguments. arguments should be in format of vec4f_unaligned array, where each argument is represented as vec4f_unaligned.
// this is used when function returns complex result (structure, tuple, etc which is not passed by value) and arguments are not guaranteed to be 16-byte aligned.
// complex result is returned via cmres pointer, which should point to memory allocated by caller with sufficient size to hold the result.
// if evaluation throws an exception, it can be obtained with das_context_get_exception.
DAS_API void das_context_eval_with_catch_cmres_unaligned ( das_context * context, das_function * fun, vec4f_unaligned * arguments, int narguments, void * cmres );
// get exception message from daScript context. return nullptr if there is no exception.
DAS_API char * das_context_get_exception ( das_context * context );

// evaluate lambda in daScript context with provided arguments. arguments should be in format of vec4f array, where each argument is represented as vec4f.
// return value is also represented as vec4f.
DAS_API vec4f das_context_eval_lambda ( das_context * context, das_lambda * lambda, vec4f * arguments );
// evaluate lambda in daScript context with provided arguments. arguments should be in format of vec4f_unaligned array, where each argument is represented as vec4f_unaligned.
// this is used when arguments are not guaranteed to be 16-byte aligned. result is also represented as vec4f_unaligned.
DAS_API void das_context_eval_lambda_unaligned ( das_context * context, das_lambda * lambda, vec4f_unaligned * arguments, int narguments, vec4f_unaligned * result );
// evaluate lambda in daScript context with provided arguments. arguments should be in format of vec4f array, where each argument is represented as vec4f.
// this is used when lambda returns complex result (structure, tuple, etc which is not passed by value).
// complex result is returned via cmres pointer, which should point to memory allocated by caller with sufficient size to hold the result.
DAS_API void das_context_eval_lambda_cmres ( das_context * context, das_lambda * lambda, vec4f * arguments, void * cmres );
// evaluate lambda in daScript context with provided arguments. arguments should be in format of vec4f_unaligned array, where each argument is represented as vec4f_unaligned.
// this is used when lambda returns complex result (structure, tuple, etc which is not passed by value) and arguments are not guaranteed to be 16-byte aligned.
// complex result is returned via cmres pointer, which should point to memory allocated by caller with sufficient size to hold the result.
DAS_API void das_context_eval_lambda_cmres_unaligned ( das_context * context, das_lambda * lambda, vec4f_unaligned * arguments, int narguments, void * cmres );

// evaluate block in daScript context with provided arguments. arguments should be in format of vec4f array, where each argument is represented as vec4f.
// return value is also represented as vec4f.
DAS_API vec4f das_context_eval_block ( das_context * context, das_block * block, vec4f * arguments );
// evaluate block in daScript context with provided arguments. arguments should be in format of vec4f_unaligned array, where each argument is represented as vec4f_unaligned.
// this is used when arguments are not guaranteed to be 16-byte aligned. result is also represented as vec4f_unaligned.
DAS_API void das_context_eval_block_unaligned ( das_context * context, das_block * block, vec4f_unaligned * arguments, int narguments, vec4f_unaligned * result );
// evaluate block in daScript context with provided arguments. arguments should be in format of vec4f array, where each argument is represented as vec4f.
// this is used when block returns complex result (structure, tuple, etc which is not passed by value).
// complex result is returned via cmres pointer, which should point to memory allocated by caller with sufficient size to hold the result.
DAS_API void das_context_eval_block_cmres ( das_context * context, das_block * block, vec4f * arguments, void * cmres );
// evaluate block in daScript context with provided arguments. arguments should be in format of vec4f_unaligned array, where each argument is represented as vec4f_unaligned.
// this is used when block returns complex result (structure, tuple, etc which is not passed by value) and arguments are not guaranteed to be 16-byte aligned.
// complex result is returned via cmres pointer, which should point to memory allocated by caller with sufficient size to hold the result.
DAS_API void das_context_eval_block_cmres_unaligned ( das_context * context, das_block * block, vec4f_unaligned * arguments, int narguments, void * cmres );

// introduce handled type for C structure. this is used to bind C structures to daScript.
// once structure is introduced, it can be used in daScript as a regular structure, and its fields can be accessed from daScript.
DAS_API das_structure * das_structure_make ( das_module_group * lib, const char * name, const char * cppname, int sz, int al );
// add field to introduced structure. this is used to define fields of C structure introduced with das_structure_make.
// offset is offset of the field in the structure. tname is type of the field in mangled name format
// see parseTypeFromMangledName from ast_typedecl.cpp for details on mangled name format
DAS_API void das_structure_add_field ( das_structure * st, das_module * mod, das_module_group * lib,  const char * name, const char * cppname, int offset, const char * tname );

// introduce handled type for C enumeration. this is used to bind C enumerations to daScript.
// once enumeration is introduced, it can be used in daScript as a regular enumeration, and its values can be accessed from daScript.
DAS_API das_enumeration * das_enumeration_make ( const char * name, const char * cppname, int ext );
// add value to introduced enumeration. this is used to define values of C enumeration introduced with das_enumeration_make.
DAS_API void das_enumeration_add_value ( das_enumeration * enu, const char * name, const char * cppName, int value );

// create module given name
DAS_API das_module * das_module_create ( char * name );
// bind interop function to module. this is used to bind C functions to daScript. once function is bound, it can be called from daScript.
DAS_API void das_module_bind_interop_function ( das_module * mod, das_module_group * lib, das_interop_function * fun, char * name, char * cppName, uint32_t sideffects, char* args );
// bind interop function with unaligned arguments to module. this is used to bind C functions to daScript when arguments are not guaranteed to be 16-byte aligned. once function is bound, it can be called from daScript.
DAS_API void das_module_bind_interop_function_unaligned ( das_module * mod, das_module_group * lib, das_interop_function_unaligned * fun, char * name, char * cppName, uint32_t sideffects, char* args );
// bind alias to module. this is used to bind C type aliases to daScript. once alias is bound, it can be used in daScript as a regular type.
DAS_API void das_module_bind_alias ( das_module * mod, das_module_group * lib, char * aname, char * tname );
// bind structure to module. this is used to bind C structures introduced with das_structure_make to daScript. once structure is bound, it can be used in daScript as a regular structure.
DAS_API void das_module_bind_structure ( das_module * mod, das_structure * st );
// bind enumeration to module. this is used to bind C enumerations introduced with das_enumeration_make to daScript. once enumeration is bound, it can be used in daScript as a regular enumeration.
DAS_API void das_module_bind_enumeration ( das_module * mod, das_enumeration * en );

// allocate string in daScript context. this is used to return strings from interop functions to daScript. string should be allocated in daScript context to ensure its lifetime is managed correctly.
char * das_allocate_string ( das_context * context, char * str );

// argument getters. these are used to convert arguments from vec4f format to C types in interop functions.
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

// argument getters for unaligned arguments. these are used to convert arguments from vec4f_unaligned format to C types in interop functions when arguments are not guaranteed to be 16-byte aligned.
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

// result setters. these are used to convert C types to vec4f format in interop functions when returning values to daScript.
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

// result setters for unaligned results. these are used to convert C types to vec4f_unaligned format in interop functions when returning values to daScript and result is not guaranteed to be 16-byte aligned.
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
