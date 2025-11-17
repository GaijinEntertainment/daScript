#pragma once

#include <stdint.h>

#ifdef DAS_ENABLE_DLL
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

typedef struct {
    float x, y, z, w;
} vec4f_unaligned;

typedef vec4f (das_interop_function) ( das_context * ctx, das_node * node, vec4f * arguments );

typedef void (das_interop_function_unaligned) ( das_context * ctx, das_node * node, vec4f_unaligned * arguments, vec4f_unaligned * result );

DAS_API void das_initialize();
DAS_API void das_shutdown();

DAS_API das_text_writer * das_text_make_printer();
DAS_API das_text_writer * das_text_make_writer();
DAS_API void das_text_release ( das_text_writer * output );
DAS_API void das_text_output ( das_text_writer * output, char * text );

DAS_API das_module_group * das_modulegroup_make ();
DAS_API void das_modulegroup_add_module ( das_module_group* lib, das_module* mod );
DAS_API void das_modulegroup_release ( das_module_group * group );

DAS_API das_file_access * das_fileaccess_make_default (  );
DAS_API das_file_access * das_fileaccess_make_project ( const char * project_file  );
DAS_API void das_fileaccess_release ( das_file_access * access );
DAS_API void das_fileaccess_introduce_file ( das_file_access * access, const char * file_name, const char * file_content );

DAS_API void das_get_root ( char * root, int maxbuf );

DAS_API das_program * das_program_compile ( char * program_file, das_file_access * access, das_text_writer * tout, das_module_group * libgroup );
DAS_API void das_program_release ( das_program * program );
DAS_API int das_program_err_count ( das_program * program );
DAS_API int das_program_context_stack_size ( das_program * program );
DAS_API int das_program_simulate ( das_program * program, das_context * ctx, das_text_writer * tout );
DAS_API das_error * das_program_get_error ( das_program * program, int index );

DAS_API void das_error_output ( das_error * error, das_text_writer * tout );
DAS_API void das_error_report ( das_error * error, char * text, int maxLength );

DAS_API das_context * das_context_make ( int stackSize );
DAS_API void das_context_release ( das_context * context );
DAS_API das_function * das_context_find_function ( das_context * context, char * name );
DAS_API vec4f das_context_eval_with_catch ( das_context * context, das_function * fun, vec4f * arguments );
DAS_API void das_context_eval_with_catch_unaligned ( das_context * context, das_function * fun, vec4f_unaligned * arguments, int narguments, vec4f_unaligned * result );
DAS_API char * das_context_get_exception ( das_context * context );

DAS_API das_structure * das_structure_make ( das_module_group * lib, const char * name, const char * cppname, int sz, int al );
DAS_API void das_structure_add_field ( das_structure * st, das_module * mod, das_module_group * lib,  const char * name, const char * cppname, int offset, const char * tname );

DAS_API das_enumeration * das_enumeration_make ( const char * name, const char * cppname, int ext );
DAS_API void das_enumeration_add_value ( das_enumeration * enu, const char * name, const char * cppName, int value );

DAS_API das_module * das_module_create ( char * name );
DAS_API void das_module_bind_interop_function ( das_module * mod, das_module_group * lib, das_interop_function * fun, char * name, char * cppName, uint32_t sideffects, char* args );
DAS_API void das_module_bind_interop_function_unaligned ( das_module * mod, das_module_group * lib, das_interop_function_unaligned * fun, char * name, char * cppName, uint32_t sideffects, char* args );
DAS_API void das_module_bind_alias ( das_module * mod, das_module_group * lib, char * aname, char * tname );
DAS_API void das_module_bind_structure ( das_module * mod, das_structure * st );
DAS_API void das_module_bind_enumeration ( das_module * mod, das_enumeration * en );

DAS_API int    das_argument_int ( vec4f arg );
DAS_API float  das_argument_float ( vec4f arg );
DAS_API double das_argument_double ( vec4f arg );
DAS_API char * das_argument_string ( vec4f arg );
DAS_API void * das_argument_ptr ( vec4f arg );

DAS_API int das_argument_int_unaligned ( vec4f_unaligned * arg );
DAS_API float das_argument_float_unaligned ( vec4f_unaligned * arg );
DAS_API double das_argument_double_unaligned ( vec4f_unaligned * arg );
DAS_API char * das_argument_string_unaligned ( vec4f_unaligned * arg );
DAS_API void * das_argument_ptr_unaligned ( vec4f_unaligned * arg );

DAS_API vec4f das_result_void ();
DAS_API vec4f das_result_int ( int r );
DAS_API vec4f das_result_float ( float r );
DAS_API vec4f das_result_double ( double r );
DAS_API vec4f das_result_string ( char * r );
DAS_API vec4f das_result_ptr ( void * r );

DAS_API void das_result_void_unaligned ( vec4f_unaligned * result );
DAS_API void das_result_int_unaligned ( vec4f_unaligned * result, int r );
DAS_API void das_result_float_unaligned ( vec4f_unaligned * result, float r );
DAS_API void das_result_double_unaligned ( vec4f_unaligned * result, double r );
DAS_API void das_result_string_unaligned ( vec4f_unaligned * result, char * r );
DAS_API void das_result_ptr_unaligned ( vec4f_unaligned * result, void * r );

#ifdef __cplusplus
}
#endif
