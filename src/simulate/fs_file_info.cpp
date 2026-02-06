#include "daScript/misc/platform.h"

#include "daScript/simulate/fs_file_info.h"
#include "daScript/misc/sysos.h"
#include "daScript/ast/ast.h"

#define DASLIB_MODULE_NAME  "daslib"
#define DASTEST_MODULE_NAME "dastest"

#if defined(_WIN32) && defined(__clang__)
    #define fileno _fileno
#endif

#if !defined(DAS_NO_FILEIO)
#include <sys/stat.h>
#endif

namespace das {
#if !defined(DAS_NO_FILEIO)
    FileInfo * FsFileSystem::tryOpenFile ( const string & fileName ) {
        if ( FILE * ff = fopen ( fileName.c_str(), "rb" ) ) {
            struct stat st;
            int fd = fileno((FILE *)ff);
            fstat(fd, &st);
            if ( !st.st_size ) {
                fclose(ff);
                return new TextFileInfo("", st.st_size, false);
            }
            char *source = (char *) das_aligned_alloc16(st.st_size);
            auto info = new TextFileInfo(source, st.st_size, true);
            auto bytesRead = fread(source, 1, st.st_size, ff);
            fclose(ff);
            if ( size_t(bytesRead) == size_t(st.st_size) ) {
                return info;
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    FsReadOnlyCachedFileSystem::~FsReadOnlyCachedFileSystem() {
        lock_guard<mutex> guard(fsm);
        files.clear();
    }

    FileInfo * FsReadOnlyCachedFileSystem::tryOpenFile ( const string & fileName ) {
        lock_guard<mutex> guard(fsm);
        // TODO: normalize fileName?
        auto hname = hash64z(fileName.c_str());
        auto it_ok = files.insert(make_pair(hname, nullptr));
        auto & hfile = it_ok.first->second;
        if ( it_ok.second ) {
            // we inserted
            if ( FILE * ff = fopen ( fileName.c_str(), "rb" ) ) {
                struct stat st;
                int fd = fileno((FILE *)ff);
                fstat(fd, &st);
                if ( st.st_size ) {
                    char *source = (char *) das_aligned_alloc16(st.st_size);
                    hfile.reset(new TextFileInfo(source, st.st_size, true));
                    auto bytesRead = fread(source, 1, st.st_size, ff);
                    fclose(ff);
                    if ( size_t(bytesRead) != size_t(st.st_size) ) {
                        hfile.release();
                    }
                } else {
                    fclose(ff);
                    hfile.reset(new TextFileInfo("", st.st_size, false));
                }
            } else {
                it_ok.first->second = nullptr;
            }
        }
        if ( hfile ) {
            const char * src = nullptr;
            uint32_t len = 0u;
            hfile->getSourceAndLength(src, len);
            return new TextFileInfo(src, len, false);
        } else {
            return nullptr;
        }
    }
#endif

    FsFileAccess::FsFileAccess()
        : ModuleFileAccess() {
        createFileSystems();
    }

    FsFileAccess::FsFileAccess ( const string & pak, const FileAccessPtr & access )
        : ModuleFileAccess (pak, access) {
        createFileSystems();
    }

    void FsFileAccess::createFileSystems() {
#if !defined(DAS_NO_FILEIO)
        addFileSystem(new FsFileSystem(),true, false);
#endif
    }

    FsFileAccess::~FsFileAccess() {
        for ( auto & fsp : fileSystems ) {
            if ( !fsp.second ) {
                delete fsp.first;
            }
        }
        fileSystems.clear();
    }

    void FsFileAccess::addFileSystem ( AnyFileSystem * fs, bool firstToTry, bool sharedFs ) {
        if ( firstToTry ) {
            fileSystems.insert(fileSystems.begin(), make_pair(fs,sharedFs));
        } else {
            fileSystems.push_back(make_pair(fs,sharedFs));
        }
    }

    das::FileInfo * FsFileAccess::getNewFileInfo(const das::string & fileName) {
        for ( auto & fs : fileSystems ) {
            if ( auto info = fs.first->tryOpenFile(fileName) ) {
                return setFileInfo(fileName, FileInfoPtr(info));
            }
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
            auto last_np = req.find_last_of("./");
            string path = last_np == np ? "" : req.substr(np + 1,last_np - np - 1);
            string mod_name = req.substr(last_np+1);
            ModuleInfo info;
            if ( top==DASLIB_MODULE_NAME ) {
                info.moduleName = mod_name;
                info.fileName = getDasRoot() + "/" + DASLIB_MODULE_NAME + "/" + info.moduleName + ".das";
                return info;
            }
            #define NATIVE_MODULE(category, subdir_name, dir_name, das_name) \
                if ( top==#category && mod_name==#das_name && (path.empty() || path == #subdir_name) ) { \
                    info.moduleName = mod_name; \
                    info.fileName = getDasRoot() + "/modules/" + #dir_name + "/" + #subdir_name + "/" + #das_name + ".das"; \
                    return info; \
                }
            #include "modules/external_resolve.inc"
            // Try dynamic modules. From modules/<name>/.das_module.
            auto resolve_mod = daScriptEnvironment::getBound()->g_dyn_modules_resolve;
            if (resolve_mod) {
                for (const auto &mod: *resolve_mod) {
                    if (mod.name == top) {
                        for (const auto &[from_path, to_path] : mod.paths) {
                            if (from_path == req.substr(np + 1)) {
                                info.moduleName = mod_name;
                                info.fileName = to_path;
                                return info;
                            }
                        }
                    }
                }
            }
            #undef NATIVE_MODULE
            auto ert = extraRoots.find(top);
            if ( ert!=extraRoots.end() ) {
                info.moduleName = mod_name;
                info.fileName = ert->second + "/" + mod_name + ".das";
                return info;
            }
            if ( top==DASTEST_MODULE_NAME ) {
                info.moduleName = mod_name;
                info.fileName = getDasRoot() + "/" + DASTEST_MODULE_NAME + "/" + info.moduleName + ".das";
                return info;
            }
        }
        return FileAccess::getModuleInfo(req, from);
    }
}

