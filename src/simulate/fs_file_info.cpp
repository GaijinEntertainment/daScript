#include "daScript/misc/platform.h"

#include "daScript/simulate/fs_file_info.h"
#include "daScript/misc/sysos.h"
#include "daScript/ast/ast.h"

#define DASLIB_MODULE_NAME  "daslib"

namespace das {
#if !DAS_NO_FILEIO

    unique_ptr<FileInfo> getNewFsFileInfo(const das::string & fileName) {
        if ( FILE * ff = fopen ( fileName.c_str(), "rb" ) ) {

            fseek(ff,0,SEEK_END);
            auto sourceLength = uint32_t(ftell(ff));
            fseek(ff,0,SEEK_SET);
            char *source = (char *) das_aligned_alloc16(sourceLength);
            auto info = das::make_unique<TextFileInfo>(source, sourceLength, true);
            uint32_t bytesRead = uint32_t(fread(source, 1, sourceLength, ff));
            fclose(ff);
            if (bytesRead == sourceLength) {
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
    }

    FsFileAccess::FsFileAccess ( const string & pak, const FileAccessPtr & access )
        : ModuleFileAccess (pak, access) {
    }

    das::FileInfo * FsFileAccess::getNewFileInfo(const das::string & fileName) {
        if ( auto info = getNewFsFileInfo(fileName) ) {
            return setFileInfo(fileName, move(info));
        }
        return nullptr;
    }

    bool FsFileAccess::addFsRoot ( const string & mod, const string & path ) {
        auto & eroots = extraRoots[mod];
        if ( !eroots.empty() && eroots!=path ) return false;
        eroots = path;
        return true;
    }

    ModuleInfo FsFileAccess::getModuleInfo ( const string & req, const string & from ) const {
        if (!failed()) {
            return ModuleFileAccess::getModuleInfo(req, from);
        }
        auto np = req.find_first_of("./");
        if ( np != string::npos ) {
            string top = req.substr(0,np);
            string mod_name = req.substr(np+1);
            ModuleInfo info;
            if ( top==DASLIB_MODULE_NAME ) {
                info.moduleName = mod_name;
                info.fileName = getDasRoot() + "/" + DASLIB_MODULE_NAME + "/" + info.moduleName + ".das";
                return info;
            }
            #define NATIVE_MODULE(category, subdir_name, dir_name, das_name) \
                if ( top==#category && mod_name==#das_name ) { \
                    info.moduleName = mod_name; \
                    info.fileName = getDasRoot() + "/modules/" + #dir_name + "/" + #subdir_name + "/" + #das_name + ".das"; \
                    return info; \
                }
            #include "modules/external_resolve.inc"
            #undef NATIVE_MODULE
            auto ert = extraRoots.find(top);
            if ( ert!=extraRoots.end() ) {
                info.moduleName = mod_name;
                info.fileName = ert->second + "/" + mod_name + ".das";
                return info;
            }
        }
        return FileAccess::getModuleInfo(req, from);
    }
#endif
}

