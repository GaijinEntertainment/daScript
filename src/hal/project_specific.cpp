#include "daScript/ast/ast.h"
#include "daScript/daScript.h"

#include "daScript/das_common.h"
#include "daScript/misc/string_writer.h"
#include "daScript/misc/sysos.h"
#include "daScript/daScriptModule.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/aot_builtin_fio.h"

das::FileAccessPtr get_file_access( char * pak );

namespace das {

static constexpr const char *MODULE_SUFFIX = ".das_module";
static constexpr const char *INIT_NAME = "initialize";

enum class Result {
    OK,
    // NoModule, // That's ok, we don't force every subfolder folder be module.
    CE,
    SimError,
    Exception,
};

static Result init_dyn_modules(string path, TextWriter &tout, bool debug = false) {
    auto fa = get_file_access(nullptr);
    const auto mod_filename = path + "/" + MODULE_SUFFIX;
    if (debug) {
        tout << "try file: " << mod_filename << ".\n";
    }
    auto fi = fa->getFileInfo(mod_filename);
    if (fi) {
        ModuleGroup dummyGroup;
        if (debug) {
            tout << "file found: " << mod_filename << ".\n";
        }
        auto program = compileDaScript(mod_filename, fa, tout, dummyGroup);
        if ( program->failed() ) {
            for ( auto & err : program->errors ) {
                tout << reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
            }
            return Result::CE;
        } else {
            auto pctx = SimulateWithErrReport(program, tout);
            if ( !pctx ) {
                return Result::SimError;
            } else {
                auto fnVec = pctx->findFunctions(INIT_NAME);
                das::vector<SimFunction *> fnMVec;
                for ( auto fnAS : fnVec ) {
                    if ( verifyCall<void, const char *>(fnAS->debugInfo, dummyGroup) ) {
                        fnMVec.push_back(fnAS);
                    }
                }
                if ( fnMVec.size()==0 ) {
                    tout << "function '"  << INIT_NAME << "' not found in '" << path << "'\n";
                    return Result::CE;
                } else if ( fnMVec.size()>1 ) {
                    tout << "too many options for '" << INIT_NAME << "'\ncandidates are:\n";
                    for ( auto fnAS : fnMVec ) {
                        tout << "    " << fnAS->mangledName << "\n";
                    }
                    return Result::CE;
                } else {
                    auto fnTest = fnMVec.back();
                    pctx->restart();
                    char * fname = pctx->allocateString(path.c_str(),uint32_t(path.length()),nullptr);
                    vec4f args[1] = {
                        cast<char *>::from(fname)
                    };
                    auto _res = pctx->evalWithCatch(fnTest, args);
                    if ( auto ex = pctx->getException() ) {
                        tout << "EXCEPTION: " << ex << " at " << pctx->exceptionAt.describe() << "\n";
                        return Result::Exception;
                    }
                    return Result::OK;
                }
            }
        }
    } else {
        if (debug) {
            tout << "file not found: " << mod_filename << ".\n";
        }
        return Result::OK;
    }
}

static bool init_modules_for_folder(const das::string &path, das::TextWriter &tout) {
    using namespace das;
    string modules_path = path + "/modules/";
    bool all_good = true;
#if defined(_MSC_VER)
    _finddata_t c_file;
    intptr_t hFile;
    string findPath = modules_path + "/*";
    if ((hFile = _findfirst(findPath.c_str(), &c_file)) != -1L) {
        do {
            if (strcmp(c_file.name, ".") == 0 || strcmp(c_file.name, "..") == 0) {
                continue;
            }
            all_good &= Result::OK == init_dyn_modules(modules_path + c_file.name, tout, false);
        } while (_findnext(hFile, &c_file) == 0);
    }
    _findclose(hFile);
 #else
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (modules_path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            all_good &= Result::OK == init_dyn_modules(modules_path + ent->d_name, tout, false);
        }
        closedir (dir);
    }
 #endif
    return all_good;
}
}

// Nothing by default.
DAS_API void require_project_specific_modules() {}

// Initializes dynamic modules from:
// - das_root/modules
// - project_root/modules
// If project_root is empty it will be ignored.
DAS_API bool require_dynamic_modules(const das::string &das_root,
                                     const das::string &project_root,
                                     das::TextWriter &tout) {
    // Always init for dasroot.
    bool all_good = das::init_modules_for_folder(das_root, tout);
    if (!project_root.empty() &&
        das::normalizeFileName(das_root.c_str()) !=
        das::normalizeFileName(project_root.c_str())) {
        // Init for project_root.
        all_good &= das::init_modules_for_folder(project_root, tout);
    }
    return all_good;
}
