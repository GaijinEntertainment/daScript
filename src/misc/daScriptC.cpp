#include "daScript/misc/platform.h"

#include "daScript/daScript.h"
#include "daScript/daScriptC.h"

using namespace das;

void das_initialize_modules() {
    NEED_ALL_DEFAULT_MODULES;
}

extern "C" {

void das_initialize() {
    das_initialize_modules();
    Module::Initialize();
}

void das_shutdown() {
    Module::Shutdown();
}

das_text_writer * das_text_make_printer() {
    return (das_text_writer *) new TextPrinter();
}

das_text_writer * das_text_make_writer() {
    return (das_text_writer *) new TextWriter();
}

void das_text_release ( das_text_writer * output ) {
    if ( output ) delete (TextWriter *) output;
}

void das_text_output ( das_text_writer * output, char * text ) {
    *((TextWriter *)output) << text;
}

das_module_group * das_modulegroup_make () {
    return (das_module_group *) new ModuleGroup();
}

void das_modulegroup_release ( das_module_group * group ) {
    if ( group ) delete (ModuleGroup *) group;
}

das_file_access * das_fileaccess_make_default (  ) {
    auto access = make_smart<FsFileAccess>();
    return (das_file_access *) access.orphan();
}

das_file_access * das_fileaccess_make_project ( const char * project_file  ) {
    auto access = make_smart<FsFileAccess>(project_file, make_smart<FsFileAccess>());
    return (das_file_access *) access.orphan();
}

void das_fileaccess_release ( das_file_access * access ) {
    if ( access ) ((FsFileAccess *) access)->delRef();
}

void das_get_root ( char * root, int maxbuf ) {
    auto r = getDasRoot();
    strncpy ( root, r.c_str(), maxbuf );
}

das_program * das_program_compile ( char * program_file, das_file_access * access, das_text_writer * tout, das_module_group * libgroup ) {
    auto program = compileDaScript(program_file,
        (FsFileAccess *) access,
        *((TextWriter *) tout),
        *((ModuleGroup *) libgroup));
    return (das_program *) program.orphan();
}

void das_program_release ( das_program * program ) {
    if ( program ) ((Program *) program)->delRef();
}

int das_program_err_count ( das_program * program ) {
    auto prog = (Program *) program;
    return prog->failed() ? int(prog->errors.size()) : 0;
}

int das_program_context_stack_size ( das_program * program ) {
    return ((Program *)program)->getContextStackSize();
}

int das_program_simulate ( das_program * program, das_context * ctx, das_text_writer * tout ) {
    if ( ((Program *) program)->simulate(*((Context *)ctx), *((TextWriter *)tout)) ) {
        return 1;
    } else {
        return 0;
    }
}

das_error * das_program_get_error ( das_program * program, int index ) {
    auto prog = (Program *) program;
    if ( index<0 || index>=prog->errors.size() ) return nullptr;
    return (das_error *) &(prog->errors[index]);
}

das_context * das_context_make ( int stackSize ) {
    return (das_context *) new Context(stackSize);
}

void das_context_release ( das_context * context ) {
    if ( context ) delete (Context *) context;
}

das_function * das_context_find_function ( das_context * context, char * name ) {
    return (das_function *) ((Context *)context)->findFunction(name);
}

void das_context_eval_with_catch_void ( das_context * context, das_function * fun, void * arguments ) {
    ((Context *)context)->evalWithCatch((SimFunction *)fun,(vec4f *)arguments);
}

char * das_context_get_exception ( das_context * context ) {
    return (char *) ((Context *)context)->getException();
}

void das_error_output ( das_error * error, das_text_writer * tout ) {
    auto err = (Error *) error;
    *((TextWriter *)tout) << reportError(err->at, err->what, err->extra, err->fixme, err->cerr );
}

void das_error_report ( das_error * error, char * text, int maxLength ) {
    auto err = (Error *) error;
    auto str = reportError(err->at, err->what, err->extra, err->fixme, err->cerr );
    strncpy(text, str.c_str(), maxLength);
}

}
