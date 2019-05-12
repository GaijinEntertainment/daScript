#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/aot_builtin_fio.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"

namespace das {

    #include "fio.das.inc"

    struct FileAnnotation : ManagedStructureAnnotation <FILE,false> {
        FileAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("FILE", ml) {
        }
    };

    MAKE_TYPE_FACTORY(FILE,FILE)

    void builtin_fprint ( const FILE * f, const char * text ) {
        if ( text ) {
            fprintf((FILE *)f,"%s",text);
        }
    }

    FILE * builtin_fopen  ( const char * name, const char * mode ) {
        if ( name && mode ) {
            return fopen(name, mode);
        } else {
            return nullptr;
        }
    }

    void builtin_fclose ( const FILE * f ) {
        fclose((FILE *)f);
    }

    class Module_FIO : public Module {
    public:
        Module_FIO() : Module("fio") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // type
            addAnnotation(make_shared<FileAnnotation>(lib));
            // file io
            addExtern<DAS_BIND_FUN(builtin_fopen)>(*this, lib, "fopen", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(builtin_fclose)>(*this, lib, "fclose", SideEffects::modifyExternal);
            addExtern<DAS_BIND_FUN(builtin_fprint)>(*this, lib, "fprint", SideEffects::modifyExternal, "builtin_fprint");
            // add builtin module
            compileBuiltinModule("fio.das",fio_das, fio_das_len);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_FIO,das);
