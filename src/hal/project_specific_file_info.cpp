#include "daScript/misc/platform.h"
#include "daScript/daScriptModule.h"
#include "daScript/simulate/fs_file_info.h"
#include "daScript/simulate/simulate.h"
#include "daScript/das_project_specific.h"

using namespace das;

static GetFileAccessFunc specificGetFileAccess = nullptr;

static GetNewContextFunc specificGetNewContext = nullptr;
static GetCloneContextFunc specificGetCloneContext = nullptr;

namespace das {

    void set_project_specific_fs_callbacks(GetFileAccessFunc getFileAccess) {
        DAS_ASSERT(getFileAccess);
        DAS_ASSERT(specificGetFileAccess == nullptr);
        specificGetFileAccess = getFileAccess;
    }

    void set_project_specific_ctx_callbacks(GetNewContextFunc getNewContext, GetCloneContextFunc getCloneContext) {
        DAS_ASSERT(getNewContext);
        DAS_ASSERT(getCloneContext);
        DAS_ASSERT(specificGetNewContext == nullptr);
        DAS_ASSERT(specificGetCloneContext == nullptr);

        specificGetNewContext = getNewContext;
        specificGetCloneContext = getCloneContext;
    }

    // exposes the project-specific override to the compiler-lib get_file_access
    GetFileAccessFunc get_project_specific_file_access() { return specificGetFileAccess; }

} // namespace das


DAS_API Context * get_context( int stackSize ) {
    if (specificGetNewContext)
        return specificGetNewContext(stackSize);
    return new Context(stackSize);
}


DAS_API Context * get_clone_context( Context * ctx, uint32_t category ) {
    if (specificGetCloneContext)
        return specificGetCloneContext(ctx, category);
    return new Context(*ctx, category);
}
