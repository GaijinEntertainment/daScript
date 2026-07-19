#pragma once

#include "daScript/misc/platform.h"
#include "daScript/simulate/simulate.h"

#include "pty.h"

namespace das_terminal {

//! Native code owns only the OS process and pseudo-terminal handles. Terminal
//! parsing, state, input encoding, snapshots, and rendering live in daScript.
struct PtyHandle : das::ptr_ref_count {
    std::unique_ptr<PtyProcess> process;
    std::string error;
    bool process_exited = false;
    uint32_t process_exit_code = 0;
};

das::smart_ptr<PtyHandle> builtin_pty_launch(
    const char * command_line, int32_t command_count,
    const char * working_directory, int32_t directory_count,
    int32_t columns, int32_t rows);
int32_t builtin_pty_read(
    const das::smart_ptr<PtyHandle> & pty, das::TArray<uint8_t> & bytes,
    int32_t maximum_bytes, das::Context * context, das::LineInfoArg * at);
bool builtin_pty_write(
    const das::smart_ptr<PtyHandle> & pty, const das::TArray<uint8_t> & bytes);
bool builtin_pty_resize(
    const das::smart_ptr<PtyHandle> & pty, int32_t columns, int32_t rows);
int32_t builtin_pty_process_id(const das::smart_ptr<PtyHandle> & pty);
bool builtin_pty_process_exited(const das::smart_ptr<PtyHandle> & pty);
int64_t builtin_pty_exit_code(const das::smart_ptr<PtyHandle> & pty);
bool builtin_pty_terminate(const das::smart_ptr<PtyHandle> & pty, int32_t exit_code);
char * builtin_pty_error(
    const das::smart_ptr<PtyHandle> & pty,
    das::Context * context, das::LineInfoArg * at);

} // namespace das_terminal
