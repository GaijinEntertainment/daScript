#include "daScript/misc/platform.h"

#include "daScript/simulate/fs_file_info.h"
#include "daScript/misc/sysos.h"
#include "daScript/ast/ast.h"

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

    unique_ptr<FileInfo> getNewFsFileInfo(const das::string & fileName) {
        if ( FILE * ff = fopen ( fileName.c_str(), "rb" ) ) {
            auto info = das::make_unique<FsFileInfo>();
            fseek(ff,0,SEEK_END);
            info->sourceLength = uint32_t(ftell(ff));
            fseek(ff,0,SEEK_SET);
            char *source = (char *) das_aligned_alloc16(info->sourceLength+1);
            uint32_t bytesRead = uint32_t(fread(source, 1, info->sourceLength, ff));
            fclose(ff);
            if (bytesRead == info->sourceLength) {
                source[info->sourceLength] = 0;
                info->source = source;
                return info;
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    FsFileAccess::FsFileAccess()
        : ModuleFileAccess() {
        daslibPath = getDasRoot() + "/daslib";
    }

    FsFileAccess::FsFileAccess ( const string & pak, const FileAccessPtr & access )
        : ModuleFileAccess (pak, access) {
         daslibPath = getDasRoot() + "/daslib";
    }

    das::FileInfo * FsFileAccess::getNewFileInfo(const das::string & fileName) {
        if ( auto info = getNewFsFileInfo(fileName) ) {
            return setFileInfo(fileName, move(info));
        }
        return nullptr;
    }

    ModuleInfo FsFileAccess::getModuleInfo ( const string & req, const string & from ) const {
        if (!failed()) {
            return ModuleFileAccess::getModuleInfo(req, from);
        }
        auto np = req.find_first_of("./");
        if ( np != string::npos ) {
            string top = req.substr(0,np);
            if ( top == "daslib" ) {
                ModuleInfo info;
                info.moduleName = req.substr(np+1);
                info.fileName = daslibPath + "/" + info.moduleName + ".das";

                #define NATIVE_MODULE(dir_name, das_name) \
                    if ( info.moduleName == #das_name ) { \
                        info.fileName = daslibPath + "/../modules/" + #dir_name + "/daslib/" + #das_name + ".das"; \
                    }

                #include "modules/external_resolve.inc"

                #undef NATIVE_MODULE

                return info;
            }
        }
        return FileAccess::getModuleInfo(req, from);
    }
#endif
}

