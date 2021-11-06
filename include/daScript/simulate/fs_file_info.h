#pragma once

#include "daScript/simulate/debug_info.h"

namespace das {
#if !DAS_NO_FILEIO
    class FsFileAccess : public das::ModuleFileAccess {
    public:
        FsFileAccess();
        FsFileAccess ( const string & pak, const FileAccessPtr & access );
        virtual das::FileInfo * getNewFileInfo(const das::string & fileName) override;
        virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const override;
        virtual bool addFsRoot ( const string & , const string & ) override;
    protected:
        das_map<string, string> extraRoots;
    };
#endif
}

