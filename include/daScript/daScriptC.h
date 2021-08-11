#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dasTextOutputHandle das_text_writer;
typedef struct dasModuleGroupHandle das_module_group;
typedef struct dasFileAccessHandle das_file_access;
typedef struct dasProgram das_program;
typedef struct dasContext das_context;
typedef struct dasFunction das_function;
typedef struct dasError * das_error;

void das_initialize();
void das_shutdown();

das_text_writer * das_text_make_printer();
das_text_writer * das_text_make_writer();
void das_text_release ( das_text_writer * output );
void das_text_output ( das_text_writer * output, char * text );

das_module_group * das_modulegroup_make ();
void das_modulegroup_release ( das_module_group * group );

das_file_access * das_fileaccess_make_default (  );
das_file_access * das_fileaccess_make_project ( const char * project_file  );
void das_fileaccess_release ( das_file_access * access );

void das_get_root ( char * root, int maxbuf );

das_program * das_program_compile ( char * program_file, das_file_access * access, das_text_writer * tout, das_module_group * libgroup );
void das_program_release ( das_program * program );
int das_program_err_count ( das_program * program );
int das_program_context_stack_size ( das_program * program );
int das_program_simulate ( das_program * program, das_context * ctx, das_text_writer * tout );
das_error * das_program_get_error ( das_program * program, int index );

void das_error_output ( das_error * error, das_text_writer * tout );
void das_error_report ( das_error * error, char * text, int maxLength );

das_context * das_context_make ( int stackSize );
void das_context_release ( das_context * context );
das_function * das_context_find_function ( das_context * context, char * name );
void das_context_eval_with_catch_void ( das_context * context, das_function * fun, void * arguments );
char * das_context_get_exception ( das_context * context );

#ifdef __cplusplus
}
#endif
