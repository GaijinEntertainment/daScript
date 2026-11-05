#pragma once

#include "daScript/simulate/debug_info.h"

namespace das {

    class DAS_CC_API TextFileInfo : public FileInfo {
    public:
        TextFileInfo ( ) = default;
        TextFileInfo ( const char * src, uint32_t len, bool own )
            : source(src), sourceLength(len), owner(own) {}
        virtual ~TextFileInfo() { if ( owner ) freeSourceData(); }
        virtual void freeSourceData() override;
        virtual void getSourceAndLength ( const char * & src, uint32_t & len ) override;
    protected:
        const char *          source = nullptr;
        uint32_t              sourceLength = 0;
        bool                  owner = true;
    };

    struct ModuleInfo {
        string  moduleName;
        string  fileName;
        string  importName;
        bool extraDepModule = false;
        string  requireName;    // the require string that produced this info (e.g. "daslib/fio"); carried so a promoted shared module records its canonical, directory-independent require identity
    };

    struct BaseRequireRecord {
        string              name;
        int32_t             line;
        vector<FileInfo *>  chain;
    };

    struct RequireRecord : BaseRequireRecord {
        bool                isPublic = false;
        bool                cantBeRequired = false;
    };

    enum class MissingHint {
        ModuleInfoNotFound,
        WrongModuleName,
        FileNotFound,
        DuplicateModule,
    };

    struct MissingRecord : BaseRequireRecord {
        MissingHint         hintType;
        string              hintName;
        string              hintName2;
    };

    struct NamelessModuleReq {
        string              name;
        string              moduleName;
        string              fileName;
        string              fromFile;
    };

    struct NamelessMismatch {
        vector<FileInfo *>  chain;
        int32_t             line;
        string              name1;
        string              fileName1;
        string              fromFile1;
        string              name2;
        string              fileName2;
        string              fromFile2;
    };

    class DAS_CC_API FileAccess : public ptr_ref_count {
    public:
        FileAccess() { ref_count_magic = TRACK_PTR_FILE_ACCESS; }
        virtual ~FileAccess() {}

        FileAccess& operator=(const FileAccess&) = delete;
        FileAccess(const FileAccess&) = delete;

        void reset() { files.clear(); }
        FileInfo * setFileInfo ( const string & fileName, FileInfoPtr && info );
        FileInfo * getFileInfo ( const string & fileName );
        virtual bool invalidateFileInfo ( const string & fileName );
        virtual string getIncludeFileName ( const string & fileName, const string & incFileName ) const;
        void freeSourceData();
        virtual int64_t getFileMtime ( const string & fileName ) const;
        virtual int64_t getFileSize ( const string & fileName ) const;   // -1 when unknown (no filesystem, stat failure)
        FileInfoPtr letGoOfFileInfo ( const string & fileName );
        virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const;
        virtual string getDynModulesFolder () const { return ""; }
        virtual bool isPodInScopeAllowed ( const string & /*moduleName*/, const string & /*fileName*/ ) const { return true; };
        virtual bool isModuleAllowed ( const string &, const string & ) const { return true; };
        virtual bool canModuleBeUnsafe ( const string &, const string & ) const { return true; };
        virtual bool isWithModuleUnsafe ( const string & /*targetModule*/, const string & /*fileName*/ ) const { return false; };
        virtual bool canBeRequired ( const string &, const string &, bool ) const { return true; };
        virtual bool addFsRoot ( const string & , const string & ) { return false; }
        virtual void serialize ( AstSerializer & ser );
        virtual bool isSameFileName ( const string & f1, const string & f2 ) const;
        virtual bool isOptionAllowed ( const string & /*opt*/, const string & /*from*/ ) const { return true; }
        virtual bool isOptionBlocked ( const string & /*opt*/, const string & /*from*/ ) const { return false; }
        virtual bool isAnnotationAllowed ( const string & /*ann*/, const string & /*from*/ ) const { return true; }
        // must stop at word boundary
        virtual bool parseCustomRequire(const char *& /*src*/, const char * /*srcEnd*/,
                                        FileInfo *& /*info*/,
                                        const FileAccessPtr & /*access*/,
                                        vector<RequireRecord> & /*req*/,
                                        vector<FileInfo *> & /*chain*/ ) const {
            return false;
        }

        void addExtraModule ( const string & modName, const string & modFile ) { extraModules.emplace_back(modName, modFile); }
        const vector<pair<string,string>> & getExtraModules () const { return extraModules; }

        void addAutoRequiredModule ( const string & modName ) { autoRequiredModules.insert(modName); }
        const das_set<string> & getAutoRequiredModules () const { return autoRequiredModules; }

        void lock() { locked = true; }
        void unlock() { locked = false; }
        bool isLocked() const { return locked; }
    protected:
        virtual FileInfo * getNewFileInfo ( const string & ) { return nullptr; }
    protected:
        das_hash_map<string, FileInfoPtr>    files;
        vector<pair<string,string>>          extraModules;
        das_set<string>                      autoRequiredModules;
        bool    locked = false;
    };
    template <> struct isCloneable<FileAccess> : false_type {};

    class DAS_CC_API ModuleFileAccess : public FileAccess {
    public:
        ModuleFileAccess();
        ModuleFileAccess ( const string & pak, const smart_ptr<Program> & program );
        virtual ~ModuleFileAccess();
        bool failed() const { return !context || !modGet; }
        virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const override;
        virtual string getDynModulesFolder () const override;
        virtual string getIncludeFileName ( const string & fileName, const string & incFileName ) const override;
        virtual bool isModuleAllowed ( const string &, const string & ) const override;
        virtual bool canModuleBeUnsafe ( const string &, const string & ) const override;
        virtual bool isWithModuleUnsafe ( const string &, const string & ) const override;
        virtual bool canBeRequired ( const string &, const string &, bool ) const override;
        virtual void serialize ( AstSerializer & ser ) override;
        virtual bool isSameFileName ( const string & f1, const string & f2 ) const override;
        virtual bool isOptionAllowed ( const string & opt, const string & from ) const override;
        virtual bool isOptionBlocked ( const string & opt, const string & from ) const override;
        virtual bool isAnnotationAllowed ( const string & /*ann*/, const string & /*from*/ ) const override;
        virtual bool isPodInScopeAllowed ( const string & /*moduleName*/, const string & /*fileName*/ ) const override;
    protected:
        Context *           context = nullptr;
        SimFunction *       modGet = nullptr;
        SimFunction *       includeGet = nullptr;
        SimFunction *       moduleAllowed = nullptr;
        SimFunction *       moduleUnsafe = nullptr;
        SimFunction *       withModuleUnsafe = nullptr;
        SimFunction *       canModuleBeRequired = nullptr;
        SimFunction *       sameFileName = nullptr;
        SimFunction *       optionAllowed = nullptr;
        SimFunction *       optionBlocked = nullptr;
        SimFunction *       annotationAllowed = nullptr;
        SimFunction *       podInScopeAllowed = nullptr;
        SimFunction *       dynModulesFolderGet = nullptr;
    };
    template <> struct isCloneable<ModuleFileAccess> : false_type {};
}
