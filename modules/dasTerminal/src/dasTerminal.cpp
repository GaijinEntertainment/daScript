#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
#include "daScript/ast/ast_interop.h"

#include "aot_builtin_terminal.h"

#include <algorithm>
#include <cstring>

MAKE_TYPE_FACTORY(pty_handle, das_terminal::PtyHandle)

namespace das_terminal {
namespace {

bool refreshProcessStatus(PtyHandle & pty) {
    if (pty.process_exited) return true;
    if (!pty.process) {
        if (pty.error.empty()) pty.error = "PTY has no process";
        return false;
    }
    std::string error;
    uint32_t exit_code = 0;
    if (pty.process->wait(0, exit_code, error)) {
        pty.process_exited = true;
        pty.process_exit_code = exit_code;
        pty.error.clear();
        return true;
    }
    if (!error.empty()) pty.error = error;
    return false;
}

} // namespace

das::smart_ptr<PtyHandle> builtin_pty_launch(
    const char * command_line, int32_t command_count,
    const char * working_directory, int32_t directory_count,
    int32_t columns, int32_t rows) {
    das::smart_ptr<PtyHandle> pty = das::make_smart<PtyHandle>();
    PtyProcessOptions options;
    if (command_line && command_count > 0)
        options.command_line.assign(command_line, static_cast<size_t>(command_count));
    if (working_directory && directory_count > 0)
        options.working_directory.assign(
            working_directory, static_cast<size_t>(directory_count));
    options.columns = columns;
    options.rows = rows;
    pty->process = launchPtyProcess(options, pty->error);
    return pty;
}

int32_t builtin_pty_read(
    const das::smart_ptr<PtyHandle> & pty, das::TArray<uint8_t> & bytes,
    int32_t maximum_bytes, das::Context * context, das::LineInfoArg * at) {
    das::array_resize(*context, bytes, 0, sizeof(uint8_t), false, at);
    if (!pty || !pty->process) {
        if (pty && pty->error.empty()) pty->error = "PTY has no process";
        return static_cast<int32_t>(PtyReadStatus::error);
    }
    pty->error.clear();
    std::string read_bytes;
    const PtyReadStatus status = pty->process->read(
        read_bytes, pty->error,
        static_cast<size_t>(std::max(1, maximum_bytes)));
    if (status == PtyReadStatus::data) {
        das::array_resize(*context, bytes, static_cast<uint32_t>(read_bytes.size()),
                          sizeof(uint8_t), false, at);
        if (!read_bytes.empty())
            std::memcpy(bytes.data, read_bytes.data(), read_bytes.size());
    }
    return static_cast<int32_t>(status);
}

bool builtin_pty_write(
    const das::smart_ptr<PtyHandle> & pty, const das::TArray<uint8_t> & bytes) {
    if (!pty || !pty->process) {
        if (pty && pty->error.empty()) pty->error = "PTY has no process";
        return false;
    }
    if (!pty->process->write(
            reinterpret_cast<const uint8_t *>(bytes.data), bytes.size, pty->error))
        return false;
    pty->error.clear();
    return true;
}

bool builtin_pty_resize(
    const das::smart_ptr<PtyHandle> & pty, int32_t columns, int32_t rows) {
    if (!pty || !pty->process) {
        if (pty && pty->error.empty()) pty->error = "PTY has no process";
        return false;
    }
    if (!pty->process->resize(columns, rows, pty->error)) return false;
    pty->error.clear();
    return true;
}

int32_t builtin_pty_process_id(const das::smart_ptr<PtyHandle> & pty) {
    return pty && pty->process ? static_cast<int32_t>(pty->process->processId()) : 0;
}

bool builtin_pty_process_exited(const das::smart_ptr<PtyHandle> & pty) {
    return pty && refreshProcessStatus(*pty);
}

int64_t builtin_pty_exit_code(const das::smart_ptr<PtyHandle> & pty) {
    if (!pty || !refreshProcessStatus(*pty)) return -1;
    return static_cast<int64_t>(pty->process_exit_code);
}

bool builtin_pty_terminate(
    const das::smart_ptr<PtyHandle> & pty, int32_t exit_code) {
    if (!pty || !pty->process) {
        if (pty && pty->error.empty()) pty->error = "PTY has no process";
        return false;
    }
    if (refreshProcessStatus(*pty)) return true;
    pty->error.clear();
    if (!pty->process->terminate(static_cast<uint32_t>(exit_code), pty->error))
        return false;
    refreshProcessStatus(*pty);
    return true;
}

char * builtin_pty_error(
    const das::smart_ptr<PtyHandle> & pty,
    das::Context * context, das::LineInfoArg * at) {
    const std::string value = pty ? pty->error : std::string();
    return context->allocateString(value.data(), value.size(), at);
}

} // namespace das_terminal

namespace das {

struct PtyHandleAnnotation final
    : ManagedStructureAnnotation<das_terminal::PtyHandle, false, true> {
    PtyHandleAnnotation(ModuleLibrary & library)
        : ManagedStructureAnnotation("pty_handle", library, "das_terminal::PtyHandle") {}
};

class Module_Terminal final : public Module {
public:
    Module_Terminal() : Module("terminal_core") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();
        addAnnotation(new PtyHandleAnnotation(lib));

        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_launch)>(*this, lib,
            "_pty_launch", SideEffects::modifyExternal,
            "das_terminal::builtin_pty_launch")
                ->args({"command_line", "command_count", "working_directory",
                    "directory_count", "columns", "rows"});
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_read)>(*this, lib,
            "_pty_read", SideEffects::modifyArgumentAndExternal,
            "das_terminal::builtin_pty_read")
                ->args({"pty", "bytes", "maximum_bytes", "context", "at"});
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_write)>(*this, lib,
            "_pty_write", SideEffects::modifyArgumentAndExternal,
            "das_terminal::builtin_pty_write")->args({"pty", "bytes"});
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_resize)>(*this, lib,
            "_pty_resize", SideEffects::modifyArgumentAndExternal,
            "das_terminal::builtin_pty_resize")->args({"pty", "columns", "rows"});
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_process_id)>(*this, lib,
            "_pty_process_id", SideEffects::none,
            "das_terminal::builtin_pty_process_id")->arg("pty");
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_process_exited)>(*this, lib,
            "_pty_process_exited", SideEffects::modifyArgumentAndExternal,
            "das_terminal::builtin_pty_process_exited")->arg("pty");
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_exit_code)>(*this, lib,
            "_pty_exit_code", SideEffects::modifyArgumentAndExternal,
            "das_terminal::builtin_pty_exit_code")->arg("pty");
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_terminate)>(*this, lib,
            "_pty_terminate", SideEffects::modifyArgumentAndExternal,
            "das_terminal::builtin_pty_terminate")->args({"pty", "exit_code"});
        addExtern<DAS_BIND_FUN(das_terminal::builtin_pty_error)>(*this, lib,
            "_pty_error", SideEffects::modifyExternal,
            "das_terminal::builtin_pty_error")->args({"pty", "context", "at"});

        verifyBuiltinNames(uint32_t(VerifyBuiltinFlags::verifyAll));
        verifyAotReady();
    }

    ModuleAotType aotRequire(TextWriter & writer) const override {
        writer << "#include \"../modules/dasTerminal/src/aot_builtin_terminal.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_Terminal, Module_Terminal);

} // namespace das

REGISTER_MODULE_IN_NAMESPACE(Module_Terminal, das);
