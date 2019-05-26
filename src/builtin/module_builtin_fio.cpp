#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/aot_builtin_fio.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/simulate/aot_builtin_time.h"

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
}



#if !DAS_NO_FILEIO

#include <sys/stat.h>

#if defined(_MSC_VER)

#include <io.h>
#include <direct.h>

#else
#include <libgen.h>
#include <dirent.h>
#include <unistd.h>
#endif

namespace das {
    struct FStat {
        struct stat stats;
        bool        is_valid;
        uint64_t size() const   { return stats.st_size; }
#if defined(_MSC_VER)
        Time     atime() const  { return stats.st_atime; }
        Time     ctime() const  { return stats.st_ctime; }
        Time     mtime() const  { return stats.st_mtime; }
        bool     is_reg() const { return stats.st_mode & _S_IFREG; }
        bool     is_dir() const { return stats.st_mode & _S_IFDIR; }

#else
        Time     atime() const  { return stats.st_atime; }
        Time     ctime() const  { return stats.st_ctime; }
        Time     mtime() const  { return stats.st_mtime; }
        bool     is_reg() const { return S_ISREG(stats.st_mode); }
        bool     is_dir() const { return S_ISDIR(stats.st_mode); }

#endif
    };
}


MAKE_TYPE_FACTORY(FStat, das::FStat)
MAKE_TYPE_FACTORY(FILE,FILE)

namespace das {
    void builtin_sleep ( uint32_t msec ) {
#if defined(_MSC_VER)
        _sleep(msec);
#else
        usleep(msec);
#endif
    }

    #include "fio.das.inc"

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

    struct FileAnnotation : ManagedStructureAnnotation <FILE,false> {
        FileAnnotation(ModuleLibrary & ml) : ManagedStructureAnnotation ("FILE", ml) {
        }
    };


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

    FILE * builtin_stdin() {
        return stdin;
    }

    FILE * builtin_stdout() {
        return stdout;
    }

    FILE * builtin_stderr() {
        return stderr;
    }

    char * builtin_fread ( const FILE * _f, Context * context ) {
        FILE * f = (FILE *) _f;
        auto pos = ftell(f);
        fseek(f, 0, SEEK_END);
        auto tail = ftell(f);
        uint32_t len = uint32_t(tail - pos);
        auto buf = new char[len];
        fseek(f, pos, SEEK_SET);
        char * res = nullptr;
        if (fread(buf, 1, len, f) == len)
          res = context->heap.allocateString(buf, len);
        delete [] buf;
        return res;
    }

    vec4f builtin_read ( Context &, SimNode_CallBase * call, vec4f * args ) 
    {
        DAS_ASSERT ( call->types[1]->isRef() || call->types[1]->isRefType() || call->types[1]->type==Type::tString);
        auto fp = cast<FILE *>::to(args[0]);
        auto buf = cast<void *>::to(args[1]);
        auto len = cast<int32_t>::to(args[2]); 
        int32_t res = (int32_t) fread(buf,1,len,fp);
        return cast<int32_t>::from(res);
    }

    vec4f builtin_write ( Context &, SimNode_CallBase * call, vec4f * args ) 
    {
        DAS_ASSERT ( call->types[1]->isRef() || call->types[1]->isRefType() || call->types[1]->type==Type::tString);
        auto fp = cast<FILE *>::to(args[0]);
        auto buf = cast<void *>::to(args[1]);
        auto len = cast<int32_t>::to(args[2]); 
        int32_t res = (int32_t) fwrite(buf,1,len,fp);
        return cast<int32_t>::from(res);
    }

    // loads(file,block<data>)
    vec4f builtin_load ( Context & context, SimNode_CallBase *, vec4f * args ) 
    {
        auto fp = cast<FILE *>::to(args[0]);
        int32_t len = cast<int32_t>::to(args[1]);
        Block * block = cast<Block *>::to(args[2]);    
        void * temp = malloc(len + 1 + sizeof(StringHeader));
        StringHeader * header = (StringHeader *) temp; 
        char * buf = (char *) (header + 1);  
        header->hash = 0;
        header->length = len;  
        if ( fread(buf, 1, len, fp)<0 )
        {
            free(temp);
            temp = nullptr;
        } 
        else 
        {
            buf[len] = 0;
        }
        vec4f bargs[1];
        bargs[0] = cast<char *>::from(buf);
        context.invoke(*block, bargs, nullptr);
        if ( temp )
        {
            free(temp);
        }
        return v_zero();
    }


    char * builtin_dirname ( const char * name, Context * context ) {
        if ( name ) {
#if defined(_MSC_VER)
            char full_path[ _MAX_PATH ];
            char dir[ _MAX_DIR ];
            char fname[ _MAX_FNAME ];
            char ext[ _MAX_EXT ];
            _splitpath(name, full_path, dir, fname, ext);
            strcat(full_path, dir);
            uint32_t len = uint32_t(strlen(full_path));
            if (len) {
                if (full_path[len - 1] == '/' || full_path[len - 1] == '\\') {
                    full_path[--len] = 0;
                }
            }
            return context->heap.allocateString(full_path, len);
#else
            char * tempName = strdup(name);
            char * dirName = dirname(tempName);
            char * result = context->heap.allocateString(dirName, strlen(dirName));
            free(tempName);
            return result;
#endif
        } else {
            return nullptr;
        }
    }

    char * builtin_basename ( const char * name, Context * context ) {
        if ( name ) {
#if defined(_MSC_VER)
            char drive[ _MAX_DRIVE ];
            char full_path[ _MAX_PATH ];
            char dir[ _MAX_DIR ];
            char ext[ _MAX_EXT ];
            _splitpath(name, drive, dir, full_path, ext);
            strcat(full_path, ext);
            return context->heap.allocateString(full_path, uint32_t(strlen(full_path)));
#else
            char * tempName = strdup(name);
            char * dirName = basename(tempName);
            char * result = context->heap.allocateString(dirName, strlen(dirName));
            free(tempName);
            return result;
#endif
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
#if defined(_MSC_VER)
        _finddata_t c_file;
        intptr_t hFile;
        string findPath = string(path) + "/*";
        if ((hFile = _findfirst(findPath.c_str(), &c_file)) != -1L) {
            do {
                char * fname = context->heap.allocateString(c_file.name, uint32_t(strlen(c_file.name)));
                vec4f args[1] = {
                    cast<char *>::from(fname)
                };
                context->invoke(fblk, args, nullptr);
            } while (_findnext(hFile, &c_file) == 0);
        }
        _findclose(hFile);
 #else
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (path)) != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                char * fname = context->heap.allocateString(ent->d_name,uint32_t(strlen(ent->d_name)));
                vec4f args[1] = {
                    cast<char *>::from(fname)
                };
                context->invoke(fblk, args, nullptr);
            }
            closedir (dir);
        }
 #endif
    }

    bool builtin_mkdir ( const char * path ) {
        if ( path ) {
#if defined(_MSC_VER)
            return _mkdir(path) == 0;
#else
            return mkdir(path, ACCESSPERMS) == 0;
#endif
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
            addExtern<DAS_BIND_FUN(builtin_fread)>(*this, lib, "fread", SideEffects::modifyExternal, "builtin_fread");
            // builtin file functions
            addInterop<builtin_read,int,const FILE*,vec4f,int32_t>(*this, lib, "_builtin_read",SideEffects::modifyExternal);
            addInterop<builtin_write,int,const FILE*,vec4f,int32_t>(*this, lib, "_builtin_write",SideEffects::modifyExternal);
            addInterop<builtin_load,void,const FILE*,int32_t,const Block &>(*this, lib, "_builtin_load",das::SideEffects::modifyExternal);   
            addExtern<DAS_BIND_FUN(builtin_dirname)>(*this, lib, "dirname", SideEffects::none, "builtin_dirname");
            addExtern<DAS_BIND_FUN(builtin_basename)>(*this, lib, "basename", SideEffects::none, "builtin_basename");
            addExtern<DAS_BIND_FUN(builtin_fstat)>(*this, lib, "fstat", SideEffects::modifyExternal, "builtin_fstat");
            addExtern<DAS_BIND_FUN(builtin_stat)>(*this, lib, "stat", SideEffects::modifyExternal, "builtin_stat");
            addExtern<DAS_BIND_FUN(builtin_dir)>(*this, lib, "builtin_dir", SideEffects::modifyExternal, "builtin_dir");
            addExtern<DAS_BIND_FUN(builtin_mkdir)>(*this, lib, "mkdir", SideEffects::modifyExternal, "builtin_mkdir");
            addExtern<DAS_BIND_FUN(builtin_stdin)>(*this, lib, "fstdin", SideEffects::modifyExternal, "builtin_stdin");
            addExtern<DAS_BIND_FUN(builtin_stdout)>(*this, lib, "fstdout", SideEffects::modifyExternal, "builtin_stdout");
            addExtern<DAS_BIND_FUN(builtin_stderr)>(*this, lib, "fstderr", SideEffects::modifyExternal, "builtin_stderr");
            addExtern<DAS_BIND_FUN(builtin_sleep)>(*this, lib, "sleep", SideEffects::modifyExternal, "builtin_sleep");
            // add builtin module
            compileBuiltinModule("fio.das",fio_das, fio_das_len);
        }
        virtual void aotRequire ( TextWriter & tw ) const override {
            tw << "#include \"daScript/simulate/aot_fio.h\"\n";
        }
    };
}

REGISTER_MODULE_IN_NAMESPACE(Module_FIO,das);

#endif
