#pragma once

#include "daScript/simulate/debug_info.h"

namespace das {
#if !DAS_NO_FILEIO
    class FsFileInfo : public das::FileInfo {
    public:
        virtual ~FsFileInfo();
    private:
        virtual void freeSourceData() override;
    };

    class FsFileAccess : public das::ModuleFileAccess {
    public:
        FsFileAccess();
        FsFileAccess ( const string & pak, const FileAccessPtr & access );
        virtual das::FileInfo * getNewFileInfo(const das::string & fileName) override;
        virtual ModuleInfo getModuleInfo ( const string & req, const string & from ) const override;
        string daslibPath;
    };
#endif
}

