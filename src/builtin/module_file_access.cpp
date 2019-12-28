#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"

namespace das {

    ModuleFileAccess::~ModuleFileAccess() {
        delete context;
    }

    ModuleFileAccess::ModuleFileAccess ( const string & pak, const FileAccessPtr & access ) {
        ModuleGroup dummyLibGroup;
        TextWriter tout;
        if ( auto program = compileDaScript(pak, access, tout, dummyLibGroup) ) {
            if ( program->failed() ) {
                for ( auto & err : program->errors ) {
                    tout << reportError(err.at, err.what, err.cerr );
                }
                DAS_ASSERTF(false,"failed to compile:\n%s", tout.str().c_str());
            } else {
                context = new Context();
                if ( !program->simulate(*context, tout) ) {
                    tout << "failed to simulate\n";
                    for ( auto & err : program->errors ) {
                        tout << reportError(err.at, err.what, err.cerr );
                    }
                    delete context;
                    context = nullptr;
                    DAS_ASSERTF(false,"failed to simulate:\n%s", tout.str().c_str());
                    return;
                }
                modGet = context->findFunction("module_get");
                DAS_ASSERTF(false, "can't find module_get function");
                // get it ready
                context->restart();
                context->runInitScript();
            }
        } else {
            DAS_ASSERTF(false, "failed to compile:\n%s", tout.str().c_str());
        }
    }

    pair<string,string> ModuleFileAccess::getModuleInfo ( const string & req, const string & from ) const {
        if ( failed() ) return make_pair("", "");
        vec4f args[2];
        args[0] = cast<const char *>::from(req.c_str());
        args[1] = cast<const char *>::from(from.c_str());
        struct {
            char * modName;
            char * modFileName;
        } res;
        context->evalWithCatch(modGet, args, &res);
        auto exc = context->getException();
        DAS_ASSERTF(!exc, "exception failed: %s", exc);
        return pair<string,string>(res.modName, res.modFileName);
    }
}
