#include "daScript/misc/platform.h"

#include "daScript/simulate/fs_file_info.h"

namespace das {
#if !DAS_NO_FILEIO
    FsFileInfo::~FsFileInfo() {
        freeSourceData();
    }

    void FsFileInfo::freeSourceData() {
        if ( source ) {
            das_aligned_free16((void*)source);
            source = nullptr;
        }
    }

    das::FileInfo * FsFileAccess::getNewFileInfo(const das::string & fileName) {
        if ( FILE * ff = fopen ( fileName.c_str(), "rb" ) ) {
            auto info = das::make_unique<FsFileInfo>();
            fseek(ff,0,SEEK_END);
            info->sourceLength = uint32_t(ftell(ff));
            fseek(ff,0,SEEK_SET);
            char *source = (char *) das_aligned_alloc16(info->sourceLength+1);
            uint32_t bytesRead = uint32_t(fread(source, 1, info->sourceLength, ff));
            if (bytesRead == info->sourceLength) {
                source[info->sourceLength] = 0;
                info->source = source;
                return setFileInfo(fileName, move(info));
            } else {
                return nullptr;
            }
        }
        return nullptr;
    }
#endif
}

