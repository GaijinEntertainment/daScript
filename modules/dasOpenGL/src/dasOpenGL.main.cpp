// Hand-written user-include for the wasm-only native opengl module.
//
// Binds the six GLES3 buffer functions whose signatures carry GLsizeiptr/GLintptr.
// Those are 32-bit on wasm32, but the shared opengl boost layer casts buffer
// sizes/offsets to int64 (matching the desktop opengl_func.das late-extern decls).
// To keep that one boost source compiling against either backend, these wrappers
// present int64 to daslang and truncate to the real 32-bit GLsizeiptr/GLintptr for
// the actual gl call. The binder skips these six (see bind_opengl.das).
// On Windows the vendored GLES3 GL_APICALL expands to __declspec(dllimport)
// (KHR/khrplatform.h), which makes the desktop gl* stub DEFINITIONS below illegal
// (C2491). KHRONOS_STATIC makes GL_APICALL empty. Must precede any GLES3 include.
#ifdef _WIN32
#define KHRONOS_STATIC
#endif
#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_typefactory_bind.h"
#include "dasOpenGL.h"
#include "need_dasOpenGL.h"

#ifndef __EMSCRIPTEN__
// Desktop link-satisfying stubs for the real gl* that the int64 buffer wrappers and
// the glGetBooleanv wrapper below call. The binder skips these 7 (gl_size_wrapped), so
// dasOpenGL.desktop_stubs.cpp does NOT cover them. On desktop the native module is
// registered for SIGNATURES only (the cross-compile host) and never runs gl, so these
// no-ops are never executed; on emscripten the real gl* come from -sFULL_ES3.
extern "C" {
    void glBufferData(GLenum, GLsizeiptr, const void *, GLenum) {}
    void glBufferSubData(GLenum, GLintptr, GLsizeiptr, const void *) {}
    void glCopyBufferSubData(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr) {}
    void glBindBufferRange(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr) {}
    void glFlushMappedBufferRange(GLenum, GLintptr, GLsizeiptr) {}
    void * glMapBufferRange(GLenum, GLintptr, GLsizeiptr, GLbitfield) { return nullptr; }
    void glGetBooleanv(GLenum, GLboolean *) {}
}
#endif

namespace das {

    void DAS_glBufferData(unsigned int target, int64_t size, const void * data, unsigned int usage) {
        glBufferData(target, (GLsizeiptr)size, data, usage);
    }
    void DAS_glBufferSubData(unsigned int target, int64_t offset, int64_t size, const void * data) {
        glBufferSubData(target, (GLintptr)offset, (GLsizeiptr)size, data);
    }
    void DAS_glCopyBufferSubData(unsigned int readTarget, unsigned int writeTarget, int64_t readOffset, int64_t writeOffset, int64_t size) {
        glCopyBufferSubData(readTarget, writeTarget, (GLintptr)readOffset, (GLintptr)writeOffset, (GLsizeiptr)size);
    }
    void DAS_glBindBufferRange(unsigned int target, unsigned int index, unsigned int buffer, int64_t offset, int64_t size) {
        glBindBufferRange(target, index, buffer, (GLintptr)offset, (GLsizeiptr)size);
    }
    void DAS_glFlushMappedBufferRange(unsigned int target, int64_t offset, int64_t length) {
        glFlushMappedBufferRange(target, (GLintptr)offset, (GLsizeiptr)length);
    }
    void * DAS_glMapBufferRange(unsigned int target, int64_t offset, int64_t length, unsigned int access) {
        return glMapBufferRange(target, (GLintptr)offset, (GLsizeiptr)length, access);
    }

    // Desktop-only diagnostic, absent from WebGL2 (GLES3 has no KHR_debug log).
    // opengl_state.das (a GL state dumper, required-but-unused by opengl_boost) calls
    // it; provide a no-op so that module COMPILES on web. Never invoked by games.
    unsigned int DAS_glGetDebugMessageLog(unsigned int count, int bufSize, unsigned int * sources,
            unsigned int * types, unsigned int * ids, unsigned int * severities, int * lengths, const char * messageLog) {
        (void)count; (void)bufSize; (void)sources; (void)types; (void)ids; (void)severities; (void)lengths; (void)messageLog;
        return 0u;
    }

    // GLboolean* binds as uint8? under DAS_BIND_FUN, but the boost layer expects the
    // desktop bool? signature. bool and GLboolean are both 1 byte -> reinterpret is safe.
    void DAS_glGetBooleanv(unsigned int pname, bool * data) {
        glGetBooleanv(pname, (GLboolean*)data);
    }

    // Desktop / GLES3.1+ functions absent from WebGL2 (GLES3.0), referenced by the
    // boost layer but not reached in web code paths. No-op stubs so the modules
    // compile; matched to the desktop opengl_func.das signatures.
    void * DAS_glMapBuffer(unsigned int target, unsigned int access) {
        (void)target; (void)access; return nullptr; // WebGL2: use glMapBufferRange
    }
    void DAS_glGetTexLevelParameteriv(unsigned int target, int level, unsigned int pname, int * params) {
        (void)target; (void)level; (void)pname; if (params) *params = 0;
    }
    void DAS_glBindImageTexture(unsigned int unit, unsigned int texture, int level, bool layered,
            int layer, unsigned int access, unsigned int format) {
        (void)unit; (void)texture; (void)level; (void)layered; (void)layer; (void)access; (void)format;
    }
    void DAS_glDispatchCompute(unsigned int num_groups_x, unsigned int num_groups_y, unsigned int num_groups_z) {
        (void)num_groups_x; (void)num_groups_y; (void)num_groups_z; // WebGL2: no compute shaders
    }
    void DAS_glMemoryBarrier(unsigned int barriers) {
        (void)barriers; // WebGL2: no image load/store / memory barriers
    }

    void Module_dasOpenGL::initMain () {
        addExtern<DAS_BIND_FUN(DAS_glBufferData)>(*this, lib, "glBufferData",
            SideEffects::worstDefault, "DAS_glBufferData")
                ->args({"target","size","data","usage"});
        addExtern<DAS_BIND_FUN(DAS_glBufferSubData)>(*this, lib, "glBufferSubData",
            SideEffects::worstDefault, "DAS_glBufferSubData")
                ->args({"target","offset","size","data"});
        addExtern<DAS_BIND_FUN(DAS_glCopyBufferSubData)>(*this, lib, "glCopyBufferSubData",
            SideEffects::worstDefault, "DAS_glCopyBufferSubData")
                ->args({"readTarget","writeTarget","readOffset","writeOffset","size"});
        addExtern<DAS_BIND_FUN(DAS_glBindBufferRange)>(*this, lib, "glBindBufferRange",
            SideEffects::worstDefault, "DAS_glBindBufferRange")
                ->args({"target","index","buffer","offset","size"});
        addExtern<DAS_BIND_FUN(DAS_glFlushMappedBufferRange)>(*this, lib, "glFlushMappedBufferRange",
            SideEffects::worstDefault, "DAS_glFlushMappedBufferRange")
                ->args({"target","offset","length"});
        addExtern<DAS_BIND_FUN(DAS_glMapBufferRange)>(*this, lib, "glMapBufferRange",
            SideEffects::worstDefault, "DAS_glMapBufferRange")
                ->args({"target","offset","length","access"});

        // ===== Desktop-GL compat for the diagnostic boost layer (opengl_state.das) =====
        // Constants/funcs absent from WebGL2 but referenced by the desktop GL state
        // dumper. Provided so the (required-but-unused-on-web) module compiles; the
        // values are harmless and the diagnostic is never called by games.
        addExtern<DAS_BIND_FUN(DAS_glGetBooleanv)>(*this, lib, "glGetBooleanv",
            SideEffects::worstDefault, "DAS_glGetBooleanv")
                ->args({"pname","data"});
        addExtern<DAS_BIND_FUN(DAS_glMapBuffer)>(*this, lib, "glMapBuffer",
            SideEffects::worstDefault, "DAS_glMapBuffer")
                ->args({"target","access"});
        addExtern<DAS_BIND_FUN(DAS_glGetTexLevelParameteriv)>(*this, lib, "glGetTexLevelParameteriv",
            SideEffects::worstDefault, "DAS_glGetTexLevelParameteriv")
                ->args({"target","level","pname","params"});
        addExtern<DAS_BIND_FUN(DAS_glBindImageTexture)>(*this, lib, "glBindImageTexture",
            SideEffects::worstDefault, "DAS_glBindImageTexture")
                ->args({"unit","texture","level","layered","layer","access","format"});
        addExtern<DAS_BIND_FUN(DAS_glGetDebugMessageLog)>(*this, lib, "glGetDebugMessageLog",
            SideEffects::worstDefault, "DAS_glGetDebugMessageLog")
                ->args({"count","bufSize","sources","types","ids","severities","lengths","messageLog"});
        addExtern<DAS_BIND_FUN(DAS_glDispatchCompute)>(*this, lib, "glDispatchCompute",
            SideEffects::worstDefault, "DAS_glDispatchCompute")
                ->args({"num_groups_x","num_groups_y","num_groups_z"});
        addExtern<DAS_BIND_FUN(DAS_glMemoryBarrier)>(*this, lib, "glMemoryBarrier",
            SideEffects::worstDefault, "DAS_glMemoryBarrier")
                ->args({"barriers"});
        // All desktop GL_* constants absent from WebGL2 that the boost layer references
        // (logic-op, GL_DEBUG_*, compute/SSBO, texture-level queries, ...). Generated from
        // opengl_const.das by gen_const_compat.das. Harmless integer values on WebGL2.
        #include "dasOpenGL.const.desktop_extras.inc"
    }

    ModuleAotType Module_dasOpenGL::aotRequire ( TextWriter & tw ) const {
        tw << "#include <GLES3/gl3.h>\n";
        tw << "#include \"daScript/simulate/bind_enum.h\"\n";
        tw << "#include \"../modules/dasOpenGL/src/dasOpenGL.enum.decl.cast.inc\"\n";
        return ModuleAotType::cpp;
    }
}
