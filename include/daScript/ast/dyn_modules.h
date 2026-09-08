#include <daScript/misc/platform.h>             // DAS_API
#include <daScript/simulate/debug_info.h>       // FileAccess

namespace das {

class TextWriter;

// Initializes dynamic modules from:
// - das_root/modules
// - project_root/modules (if project_root is non-empty and differs from das_root)
// - each path in load_modules — treated as a single module folder (the one
//   containing `.das_module`), bypassing the modules/<name> scan. Path basenames
//   shadow same-named entries in das_root and project_root.
// During initialization process it calls `initialize` in files `.das_module`.
DAS_CC_API bool require_dynamic_modules(smart_ptr<FileAccess> file_access,
                                     const string &das_root,
                                     const string &project_root,
                                     const vector<string> &load_modules,
                                     TextWriter &tout);

// As above, plus `disabled_modules`: dynamic modules whose folder basename
// matches (case-insensitive, every platform) are never loaded/registered.
// Used to keep a native-only module (e.g. dashv/libhv) out of a wasm
// cross-compile, so a guarded `require ?mod` resolves as absent instead of
// dragging in a module whose target archive can't exist.
DAS_CC_API bool require_dynamic_modules(smart_ptr<FileAccess> file_access,
                                     const string &das_root,
                                     const string &project_root,
                                     const vector<string> &load_modules,
                                     const vector<string> &disabled_modules,
                                     TextWriter &tout);

// src/ast/ARCHITECTURE.md sec.2
struct DynModuleManifestRow {
    bool    dynamic = false;    // false: native path (a=mod_name, b=src, c=dst); true: dynamic module (a=path, b=cpp class, c=das name)
    string  a, b, c;
    int     on_error = 0;       // dynamic only: the RegisterOnError the descriptor used
};
DAS_API void begin_dynamic_module_recording();
DAS_API void end_dynamic_module_recording(vector<DynModuleManifestRow> & rows, bool & optOut);
DAS_API void replay_native_path(const char * mod_name, const char * src, const char * dst);
DAS_API void replay_dynamic_module(const char * path, const char * cpp_class, int on_error);
// a replayed dm row with a das name waits under it for the first require (ARCHITECTURE.md sec.2)
DAS_API void defer_dynamic_module(const char * path, const char * cpp_class, int on_error, const char * das_name);
DAS_API bool load_deferred_dynamic_module(const char * das_name);   // true = the module registered
DAS_API size_t load_all_deferred_dynamic_modules();                 // the count it attempted
DAS_API bool has_deferred_dynamic_modules();
DAS_API bool is_dynamic_module_deferred(const char * das_name);
// a module the recording scan loaded is not "required" until a require names it: a guard reads it
// absent on a cold start as on a warm one, where the row waits (ARCHITECTURE.md sec.2)
DAS_API bool is_dynamic_module_unrequired(const char * das_name);
DAS_API void mark_dynamic_module_required(const char * das_name);
DAS_CC_API void ignore_dynamic_module_manifests(bool ignore);       // -ignore-manifest: no read, no write, every module loads on start
}
