#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/aot_builtin_fio.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/aot_builtin_time.h"

#if defined(_MSC_VER)

// TODO:    implement windows version of
//  dirname
//  basename
//
//  _stat == stat

#include <io.h>

#else
#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

MAKE_TYPE_FACTORY(clock, das::Time)// use MAKE_TYPE_FACTORY out of namespace. Some compilers not happy otherwise

namespace das {

    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Equ,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(NotEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(GtEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(LessEqu,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Gt,Time);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Less,Time);
    IMPLEMENT_OP2_EVAL_POLICY(Sub, Time);

    struct TimeAnnotation : ManagedValueAnnotation<Time> {
        TimeAnnotation() : ManagedValueAnnotation<Time>("clock","das::Time") {}
        virtual void walk ( DataWalker & walker, void * data ) override {
            if ( walker.reading ) {
                // there shuld be a way to read time from the stream here
            } else {
                Time * t = (Time *) data;
                char mbstr[100];
                if ( strftime(mbstr, sizeof(mbstr), "%c", localtime(&t->time)) ) {
                    char * str = mbstr;
                    walker.String(str);
                }
            }
        }
    };

    Time builtin_clock() {
        Time t;
        t.time = time(nullptr);
        return t;
    }

    void Module_BuiltIn::addTime(ModuleLibrary & lib) {
        addAnnotation(make_shared<TimeAnnotation>());
        addFunctionBasic<Time>(*this,lib);
        addFunctionOrdered<Time>(*this,lib);
        addFunction( make_shared<BuiltInFn<Sim_Sub<Time>,float,Time,Time>>("-",lib,"Sub"));
        addExtern<DAS_BIND_FUN(builtin_clock)>(*this, lib, "getClock", SideEffects::modifyExternal, "builtin_clock");
    }

    #include "fio.das.inc"

    struct FStat {
        struct stat stats;
        bool        is_valid;
        uint64_t size() const   { return stats.st_size; }
        Time     atime() const  { return stats.st_atimespec.tv_sec; }
        Time     ctime() const  { return stats.st_ctimespec.tv_sec; }
        Time     mtime() const  { return stats.st_mtimespec.tv_sec; }
        bool     is_reg() const { return S_ISREG(stats.st_mode); }
        bool     is_dir() const { return S_ISDIR(stats.st_mode); }
    };

    struct FStatAnnotation : ManagedStructureAnnotation <FStat,true> {
        FStatAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("FStat", ml) {
            addField<DAS_BIND_MANAGED_FIELD(is_valid)>("is_valid");
            addProperty<DAS_BIND_MANAGED_PROP(size)>("size");
            addProperty<DAS_BIND_MANAGED_PROP(atime)>("atime");
            addProperty<DAS_BIND_MANAGED_PROP(ctime)>("ctime");
            addProperty<DAS_BIND_MANAGED_PROP(mtime)>("mtime");
            addProperty<DAS_BIND_MANAGED_PROP(is_reg)>("is_reg");
            addProperty<DAS_BIND_MANAGED_PROP(is_dir)>("is_dir");
        }
        virtual bool canMove() const override { return true; }
        virtual bool canCopy() const override { return true; }
        virtual bool isLocal() const override { return true; }
    };

    MAKE_TYPE_FACTORY(FStat,FStat)

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

    char * builtin_dirname ( const char * name, Context * context ) {
        if ( name ) {
            char * tempName = strdup(name);
            char * dirName = dirname(tempName);
            char * result = context->heap.allocateString(dirName, strlen(dirName));
            free(tempName);
            return result;
        } else {
            return nullptr;
        }
    }

    char * builtin_basename ( const char * name, Context * context ) {
        if ( name ) {
            char * tempName = strdup(name);
            char * dirName = dirname(tempName);
            char * result = context->heap.allocateString(dirName, strlen(dirName));
            free(tempName);
            return result;
        } else {
            return nullptr;
        }
    }

    bool builtin_fstat ( const FILE * f, FStat & fs ) {
        return fstat(fileno((FILE *)f), &fs.stats) == 0;
    }

    bool builtin_stat ( const char * filename, FStat & fs ) {
        if ( filename!=nullptr ) {
            return stat(filename, &fs.stats) == 0;
        } else {
            return false;
        }
    }

     void builtin_dir ( const char * path, const Block & fblk, Context * context ) {
    #ifdef _MSC_VER
        _finddata_t c_file;
        intptr_t hFile;
        string findPath = path + "/*";
        if ((hFile = _findfirst(findPath.c_str(), &c_file)) != -1L) {
            do {
                vec4f args[1];
                args[0] = cast<char *>::from(c_file.name);
                context->invoke(fblk, args, nullptr);
            } while (_findnext(hFile, &c_file) == 0);
        }
        _findclose(hFile);
    #else
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path)) != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                vec4f args[1];
                args[0] = cast<char *>::from(ent->d_name);
                context->invoke(fblk, args, nullptr);
            }
            closedir (dir);
        }
    #endif
    }

    bool builtin_mkdir ( const char * path ) {
        if ( path ) {
            return mkdir(path, ACCESSPERMS) == 0;
        } else {
            return false;
        }
    }

    class Module_FIO : public Module {
    public:
        Module_FIO() : Module("fio") {
            ModuleLibrary lib;
            lib.addModule(this);
            lib.addBuiltInModule();
            // type
            addAnnotation(make_shared<FileAnnotation>(lib));
            addAnnotation(make_shared<FStatAnnotation>(lib));
            // file io
            addExtern<DAS_BIND_FUN(builtin_fopen)>(*this, lib, "fopen", SideEffects::modifyExternal, "builtin_fopen");
            addExtern<DAS_BIND_FUN(builtin_fclose)>(*this, lib, "fclose", SideEffects::modifyExternal, "builtin_fclose");
            addExtern<DAS_BIND_FUN(builtin_fprint)>(*this, lib, "fprint", SideEffects::modifyExternal, "builtin_fprint");
            addExtern<DAS_BIND_FUN(builtin_dirname)>(*this, lib, "dirname", SideEffects::none, "builtin_dirname");
            addExtern<DAS_BIND_FUN(builtin_basename)>(*this, lib, "basename", SideEffects::none, "builtin_basename");
            addExtern<DAS_BIND_FUN(builtin_fstat)>(*this, lib, "fstat", SideEffects::modifyExternal, "builtin_fstat");
            addExtern<DAS_BIND_FUN(builtin_stat)>(*this, lib, "stat", SideEffects::modifyExternal, "builtin_stat");
            addExtern<DAS_BIND_FUN(builtin_dir)>(*this, lib, "builtin_dir", SideEffects::modifyExternal, "builtin_dir");
            addExtern<DAS_BIND_FUN(builtin_mkdir)>(*this, lib, "mkdir", SideEffects::modifyExternal, "builtin_mkdir");
            // add builtin module
            compileBuiltinModule("fio.das",fio_das, fio_das_len);
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_FIO,das);
