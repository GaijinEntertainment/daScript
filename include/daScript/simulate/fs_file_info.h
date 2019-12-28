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

    class FsFileAccess : public das::FileAccess {
    public:
        virtual das::FileInfo * getNewFileInfo(const das::string & fileName) override;
    };

    class FsModuleFileAccess : public das::ModuleFileAccess {
    public:
        FsModuleFileAccess ( const string & pak, const FileAccessPtr & access )
            : ModuleFileAccess (pak, access) {}
    protected:
        virtual das::FileInfo * getNewFileInfo(const das::string & fileName) override;
    };
#endif
}

