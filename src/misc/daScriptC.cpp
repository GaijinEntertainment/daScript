#include "daScript/misc/platform.h"

#include "daScript/daScript.h"
#include "daScript/daScriptC.h"

using namespace das;

namespace das {
    struct SimNode_CFuncCall : SimNode_ExtFuncCallBase {
        SimNode_CFuncCall ( const LineInfo & at, const char * fnName, das_interop_function * FN )
            : SimNode_ExtFuncCallBase(at,fnName) { fn = FN; }
        virtual vec4f eval ( Context & context ) override {
            DAS_PROFILE_NODE
            vec4f * args = (vec4f *)(alloca(nArguments * sizeof(vec4f)));
            evalArgs(context, args);
            return (*fn)((das_context *)&context,(das_node *)this,args);
        }
        das_interop_function * fn;
    };

    class CFunction : public BuiltInFunction {
    public:
        __forceinline CFunction(const char * name, const ModuleLibrary &, const char * cppName, das_interop_function * FN )
            : BuiltInFunction(name,cppName) {
            this->callBased = true;
            this->interopFn = true;
            fn = FN;
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            const char * fnName = context.code->allocateName(this->name);
            return context.code->makeNode<SimNode_CFuncCall>(BuiltInFunction::at,fnName,fn);
        }
        das_interop_function * fn;
    };
}

das::FileAccessPtr get_file_access( char * pak );//link time resolved dependencies

Context * get_context( int stackSize = 0 );

void das_initialize_modules() {
    NEED_ALL_DEFAULT_MODULES;
}

extern "C" {

uint32_t SIDEEFFECTS_none = uint32_t(SideEffects::none);
uint32_t SIDEEFFECTS_unsafe = uint32_t(SideEffects::unsafe);
uint32_t SIDEEFFECTS_userScenario = uint32_t(SideEffects::userScenario);
uint32_t SIDEEFFECTS_modifyExternal = uint32_t(SideEffects::modifyExternal);
uint32_t SIDEEFFECTS_accessExternal = uint32_t(SideEffects::accessExternal);
uint32_t SIDEEFFECTS_modifyArgument = uint32_t(SideEffects::modifyArgument);
uint32_t SIDEEFFECTS_modifyArgumentAndExternal = uint32_t(SideEffects::modifyArgumentAndExternal);
uint32_t SIDEEFFECTS_worstDefault = uint32_t(SideEffects::worstDefault);
uint32_t SIDEEFFECTS_accessGlobal = uint32_t(SideEffects::accessGlobal);
uint32_t SIDEEFFECTS_invoke =  uint32_t(SideEffects::invoke);
uint32_t SIDEEFFECTS_inferredSideEffects =  uint32_t(SideEffects::inferredSideEffects);

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
    auto access = get_file_access(nullptr);
    return (das_file_access *) access.orphan();
}

das_file_access * das_fileaccess_make_project ( const char * project_file  ) {
    auto access = get_file_access((char *)project_file);
    return (das_file_access *) access.orphan();
}

void das_fileaccess_release ( das_file_access * access ) {
    if ( access ) ((FileAccess *) access)->delRef();
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
    return (das_context *) get_context(stackSize);
}

void das_context_release ( das_context * context ) {
    if ( context ) delete (Context *) context;
}

das_function * das_context_find_function ( das_context * context, char * name ) {
    return (das_function *) ((Context *)context)->findFunction(name);
}

vec4f das_context_eval_with_catch ( das_context * context, das_function * fun, vec4f * arguments ) {
    return ((Context *)context)->evalWithCatch((SimFunction *)fun,(vec4f *)arguments);
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

das_module * das_module_create ( char * name ) {
    return (das_module *) new Module(name);
}

void das_module_bind_interop_function ( das_module * mod, das_module_group * lib, das_interop_function * fun, char * name, char * cppName, uint32_t sideffects, char* args ) {
    auto fn = make_smart<CFunction>(name, *(ModuleLibrary *)lib, cppName, fun);
    fn->setSideEffects((das::SideEffects) sideffects);
    vector <TypeDeclPtr> arguments;
    const char * arg = args;
    while ( *arg ) {
        auto tt = parseTypeFromMangledName(arg, *(ModuleLibrary*)lib,((Module *)mod));
        arguments.push_back(tt);
        while (*arg==' ') arg ++;
    }
    fn->constructInterop(arguments);
    ((Module *)mod)->addFunction(fn, false);
}

int    das_argument_int ( vec4f arg ) { return cast<int>::to(arg); }
float  das_argument_float ( vec4f arg ) { return cast<float>::to(arg); }
char * das_argument_string ( vec4f arg ) { char * a = cast<char *>::to(arg); return a ? a : ((char *)""); }

vec4f das_result_void () { return v_zero(); }
vec4f das_result_int ( int r ) { return cast<int>::from(r); }
vec4f das_result_float ( float r ) { return cast<float>::from(r); }
vec4f das_result_string ( char * r ) { return cast<char *>::from(r); }

}
