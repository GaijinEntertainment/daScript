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
        FileAccessPtr       projectAccess;
    };
    template <> struct isCloneable<ModuleFileAccess> : false_type {};
}
