#include "pty.h"

#include <algorithm>
#include <vector>

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace das_terminal {
namespace {

#ifndef PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE
#define PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE 0x00020016
#endif

using CreatePseudoConsoleFn = HRESULT(WINAPI *)(COORD, HANDLE, HANDLE, DWORD, HANDLE *);
using ResizePseudoConsoleFn = HRESULT(WINAPI *)(HANDLE, COORD);
using ClosePseudoConsoleFn = void(WINAPI *)(HANDLE);

struct ConPtyApi {
    CreatePseudoConsoleFn create = nullptr;
    ResizePseudoConsoleFn resize = nullptr;
    ClosePseudoConsoleFn close = nullptr;

    bool resolve(std::string & error);
};

std::string windowsError(const char * operation, DWORD code = GetLastError()) {
    char * message = nullptr;
    const DWORD length = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, 0, reinterpret_cast<char *>(&message), 0, nullptr);
    std::string result = operation;
    result += " failed (" + std::to_string(code) + ")";
    if (length && message) {
        result += ": ";
        result.append(message, length);
        while (!result.empty() && (result.back() == '\r' || result.back() == '\n'))
            result.pop_back();
    }
    if (message) LocalFree(message);
    return result;
}

bool ConPtyApi::resolve(std::string & error) {
    const HMODULE kernel = GetModuleHandleW(L"kernel32.dll");
    create = reinterpret_cast<CreatePseudoConsoleFn>(
        GetProcAddress(kernel, "CreatePseudoConsole"));
    resize = reinterpret_cast<ResizePseudoConsoleFn>(
        GetProcAddress(kernel, "ResizePseudoConsole"));
    close = reinterpret_cast<ClosePseudoConsoleFn>(
        GetProcAddress(kernel, "ClosePseudoConsole"));
    if (create && resize && close) return true;
    error = "Windows ConPTY is unavailable (requires Windows 10 version 1809 or newer)";
    return false;
}

std::wstring utf8ToWide(const std::string & text, std::string & error) {
    if (text.empty()) return std::wstring();
    const int count = MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS, text.data(), static_cast<int>(text.size()),
        nullptr, 0);
    if (count <= 0) {
        error = windowsError("MultiByteToWideChar");
        return std::wstring();
    }
    std::wstring result(static_cast<size_t>(count), L'\0');
    if (MultiByteToWideChar(
            CP_UTF8, MB_ERR_INVALID_CHARS, text.data(), static_cast<int>(text.size()),
            &result[0], count) != count) {
        error = windowsError("MultiByteToWideChar");
        return std::wstring();
    }
    return result;
}

void closeHandle(HANDLE & handle) {
    if (handle && handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
    handle = nullptr;
}

class ConPtyProcess final : public PtyProcess {
public:
    explicit ConPtyProcess(const ConPtyApi & api) : api_(api) {}
    ~ConPtyProcess() override;

    bool launch(const PtyProcessOptions & options, std::string & error);
    uint32_t processId() const override { return process_id_; }
    PtyReadStatus read(std::string & bytes, std::string & error,
                       size_t maximum_bytes) override;
    bool write(const uint8_t * bytes, size_t count, std::string & error) override;
    bool resize(int columns, int rows, std::string & error) override;
    bool wait(uint32_t timeout_ms, uint32_t & exit_code, std::string & error) override;
    bool terminate(uint32_t exit_code, std::string & error) override;

private:
    void closePty();

    ConPtyApi api_;
    HANDLE pseudo_console_ = nullptr;
    HANDLE input_write_ = nullptr;
    HANDLE output_read_ = nullptr;
    HANDLE process_ = nullptr;
    HANDLE job_ = nullptr;
    uint32_t process_id_ = 0;
};

void ConPtyProcess::closePty() {
    closeHandle(input_write_);
    // Close our read end before the synchronous pseudoconsole close. ConHost
    // may still be flushing final output; leaving the reader open here can
    // make ClosePseudoConsole wait forever when no thread is draining it.
    closeHandle(output_read_);
    if (pseudo_console_) api_.close(pseudo_console_);
    pseudo_console_ = nullptr;
}

std::string escapeWindowsArgument(const std::string & argument) {
    if (!argument.empty() &&
        argument.find_first_of(" \t\n\v\"") == std::string::npos)
        return argument;
    std::string result = "\"";
    for (size_t index = 0; index < argument.size();) {
        size_t backslashes = 0;
        while (index < argument.size() && argument[index] == '\\') {
            ++backslashes;
            ++index;
        }
        if (index == argument.size()) {
            result.append(backslashes * 2, '\\');
        } else if (argument[index] == '"') {
            result.append(backslashes * 2 + 1, '\\');
            result += '"';
            ++index;
        } else {
            result.append(backslashes, '\\');
            result += argument[index++];
        }
    }
    result += '"';
    return result;
}

std::string buildWindowsCommandLine(const std::vector<std::string> & arguments) {
    std::string result;
    for (const std::string & argument : arguments) {
        if (!result.empty()) result += ' ';
        result += escapeWindowsArgument(argument);
    }
    return result;
}

ConPtyProcess::~ConPtyProcess() {
    closePty();
    closeHandle(job_);
    closeHandle(process_);
}

bool ConPtyProcess::launch(const PtyProcessOptions & options, std::string & error) {
    if (pseudo_console_ || input_write_ || output_read_ || process_ || job_) {
        error = "ConPTY process is already launched";
        return false;
    }
    if (options.command_line.empty() && options.arguments.empty()) {
        error = "ConPTY command line is empty";
        return false;
    }

    HANDLE input_read = nullptr;
    HANDLE output_write = nullptr;
    if (!CreatePipe(&input_read, &input_write_, nullptr, 0)) {
        error = windowsError("CreatePipe(input)");
        return false;
    }
    if (!CreatePipe(&output_read_, &output_write, nullptr, 0)) {
        error = windowsError("CreatePipe(output)");
        closeHandle(input_read);
        closePty();
        return false;
    }

    const COORD size = {
        static_cast<SHORT>(std::max(1, std::min(32767, options.columns))),
        static_cast<SHORT>(std::max(1, std::min(32767, options.rows))),
    };
    const HRESULT create_result = api_.create(size, input_read, output_write, 0, &pseudo_console_);
    if (FAILED(create_result)) {
        error = "CreatePseudoConsole failed (HRESULT " +
            std::to_string(static_cast<long>(create_result)) + ")";
        closeHandle(input_read);
        closeHandle(output_write);
        closePty();
        return false;
    }

    SIZE_T attribute_bytes = 0;
    if (InitializeProcThreadAttributeList(nullptr, 1, 0, &attribute_bytes) ||
        GetLastError() != ERROR_INSUFFICIENT_BUFFER || !attribute_bytes) {
        error = windowsError("InitializeProcThreadAttributeList(size)");
        closeHandle(input_read);
        closeHandle(output_write);
        closePty();
        return false;
    }
    std::vector<uint8_t> attribute_storage(attribute_bytes);
    STARTUPINFOEXW startup = {};
    startup.StartupInfo.cb = sizeof(startup);
    // Some hosts (including test runners) have valid redirected standard
    // handles. Explicit null standard handles prevent those from winning over
    // the pseudoconsole during process initialization; ConPTY supplies the
    // child's actual console handles.
    startup.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
    startup.StartupInfo.hStdInput = nullptr;
    startup.StartupInfo.hStdOutput = nullptr;
    startup.StartupInfo.hStdError = nullptr;
    startup.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(
        attribute_storage.data());
    if (!InitializeProcThreadAttributeList(startup.lpAttributeList, 1, 0, &attribute_bytes)) {
        error = windowsError("InitializeProcThreadAttributeList");
        closeHandle(input_read);
        closeHandle(output_write);
        closePty();
        return false;
    }
    if (!UpdateProcThreadAttribute(
            startup.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
            pseudo_console_, sizeof(pseudo_console_), nullptr, nullptr)) {
        error = windowsError("UpdateProcThreadAttribute");
        DeleteProcThreadAttributeList(startup.lpAttributeList);
        closeHandle(input_read);
        closeHandle(output_write);
        closePty();
        return false;
    }

    const std::string command_line = options.arguments.empty()
        ? options.command_line : buildWindowsCommandLine(options.arguments);
    std::wstring command = utf8ToWide(command_line, error);
    std::wstring directory = utf8ToWide(options.working_directory, error);
    if (command.empty() || (!options.working_directory.empty() && directory.empty())) {
        DeleteProcThreadAttributeList(startup.lpAttributeList);
        closeHandle(input_read);
        closeHandle(output_write);
        closePty();
        return false;
    }
    std::vector<wchar_t> mutable_command(command.begin(), command.end());
    mutable_command.push_back(L'\0');

    job_ = CreateJobObjectW(nullptr, nullptr);
    if (!job_) {
        error = windowsError("CreateJobObjectW");
        DeleteProcThreadAttributeList(startup.lpAttributeList);
        closeHandle(input_read);
        closeHandle(output_write);
        closePty();
        return false;
    }
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION job_limits = {};
    job_limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(
            job_, JobObjectExtendedLimitInformation, &job_limits,
            sizeof(job_limits))) {
        error = windowsError("SetInformationJobObject");
        DeleteProcThreadAttributeList(startup.lpAttributeList);
        closeHandle(input_read);
        closeHandle(output_write);
        closePty();
        closeHandle(job_);
        return false;
    }

    PROCESS_INFORMATION process = {};
    const BOOL created = CreateProcessW(
        nullptr, mutable_command.data(), nullptr, nullptr, FALSE,
        EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED,
        nullptr, directory.empty() ? nullptr : directory.c_str(),
        &startup.StartupInfo, &process);
    const DWORD create_error = GetLastError();
    DeleteProcThreadAttributeList(startup.lpAttributeList);
    // ConPTY keeps these communication ends alive internally only after the
    // hosted process has attached to the pseudoconsole.
    closeHandle(input_read);
    closeHandle(output_write);
    if (!created) {
        error = windowsError("CreateProcessW", create_error);
        closePty();
        closeHandle(job_);
        return false;
    }

    if (!AssignProcessToJobObject(job_, process.hProcess)) {
        const DWORD assign_error = GetLastError();
        TerminateProcess(process.hProcess, 1);
        WaitForSingleObject(process.hProcess, INFINITE);
        CloseHandle(process.hThread);
        CloseHandle(process.hProcess);
        closePty();
        closeHandle(job_);
        error = windowsError("AssignProcessToJobObject", assign_error);
        return false;
    }
    if (ResumeThread(process.hThread) == static_cast<DWORD>(-1)) {
        const DWORD resume_error = GetLastError();
        TerminateJobObject(job_, 1);
        WaitForSingleObject(process.hProcess, INFINITE);
        CloseHandle(process.hThread);
        CloseHandle(process.hProcess);
        closePty();
        closeHandle(job_);
        error = windowsError("ResumeThread", resume_error);
        return false;
    }

    CloseHandle(process.hThread);
    process_ = process.hProcess;
    process_id_ = process.dwProcessId;
    return true;
}

PtyReadStatus ConPtyProcess::read(
    std::string & bytes, std::string & error, size_t maximum_bytes) {
    bytes.clear();
    if (!output_read_) {
        error = "ConPTY output pipe is closed";
        return PtyReadStatus::closed;
    }
    DWORD available = 0;
    if (!PeekNamedPipe(output_read_, nullptr, 0, nullptr, &available, nullptr)) {
        const DWORD code = GetLastError();
        closeHandle(output_read_);
        if (code == ERROR_BROKEN_PIPE) return PtyReadStatus::closed;
        error = windowsError("PeekNamedPipe", code);
        return PtyReadStatus::error;
    }
    if (!available) {
        // Root-process exit does not close a pseudoconsole: descendants may
        // still be attached, and ConHost may still have final output to flush.
        // Only the pipe itself can authoritatively report session closure.
        return PtyReadStatus::idle;
    }
    const DWORD requested = static_cast<DWORD>(std::min<size_t>(
        std::max<size_t>(1, maximum_bytes), std::min<size_t>(available, 0xffffffffu)));
    bytes.resize(requested);
    DWORD received = 0;
    if (!ReadFile(output_read_, &bytes[0], requested, &received, nullptr)) {
        const DWORD code = GetLastError();
        bytes.clear();
        closeHandle(output_read_);
        if (code == ERROR_BROKEN_PIPE) return PtyReadStatus::closed;
        error = windowsError("ReadFile(ConPTY output)", code);
        return PtyReadStatus::error;
    }
    bytes.resize(received);
    return received ? PtyReadStatus::data : PtyReadStatus::idle;
}

bool ConPtyProcess::write(const uint8_t * bytes, size_t count, std::string & error) {
    if (!bytes && count) {
        error = "ConPTY input bytes are null";
        return false;
    }
    if (!input_write_) {
        error = "ConPTY input pipe is closed";
        return false;
    }
    size_t offset = 0;
    while (offset != count) {
        const DWORD requested = static_cast<DWORD>(
            std::min<size_t>(count - offset, 0xffffffffu));
        DWORD written = 0;
        if (!WriteFile(input_write_, bytes + offset, requested, &written, nullptr)) {
            const DWORD code = GetLastError();
            closeHandle(input_write_);
            error = windowsError("WriteFile(ConPTY input)", code);
            return false;
        }
        if (!written) {
            closeHandle(input_write_);
            error = "WriteFile(ConPTY input) wrote zero bytes";
            return false;
        }
        offset += written;
    }
    return true;
}

bool ConPtyProcess::resize(int columns, int rows, std::string & error) {
    const COORD size = {
        static_cast<SHORT>(std::max(1, std::min(32767, columns))),
        static_cast<SHORT>(std::max(1, std::min(32767, rows))),
    };
    const HRESULT result = api_.resize(pseudo_console_, size);
    if (SUCCEEDED(result)) return true;
    error = "ResizePseudoConsole failed (HRESULT " +
        std::to_string(static_cast<long>(result)) + ")";
    return false;
}

bool ConPtyProcess::wait(
    uint32_t timeout_ms, uint32_t & exit_code, std::string & error) {
    const DWORD result = WaitForSingleObject(process_, timeout_ms);
    if (result == WAIT_TIMEOUT) return false;
    if (result != WAIT_OBJECT_0) {
        error = windowsError("WaitForSingleObject");
        return false;
    }
    DWORD code = 0;
    if (!GetExitCodeProcess(process_, &code)) {
        error = windowsError("GetExitCodeProcess");
        return false;
    }
    exit_code = code;
    return true;
}

bool ConPtyProcess::terminate(uint32_t exit_code, std::string & error) {
    if (job_ && TerminateJobObject(job_, exit_code)) return true;
    if (!job_ && TerminateProcess(process_, exit_code)) return true;
    const DWORD code = GetLastError();
    if (code == ERROR_ACCESS_DENIED && WaitForSingleObject(process_, 0) == WAIT_OBJECT_0)
        return true;
    error = windowsError(job_ ? "TerminateJobObject" : "TerminateProcess", code);
    return false;
}

} // namespace

std::unique_ptr<PtyProcess> launchPtyProcess(
    const PtyProcessOptions & options, std::string & error) {
    ConPtyApi api;
    if (!api.resolve(error)) return std::unique_ptr<PtyProcess>();
    std::unique_ptr<ConPtyProcess> process(new ConPtyProcess(api));
    if (!process->launch(options, error)) return std::unique_ptr<PtyProcess>();
    return std::unique_ptr<PtyProcess>(process.release());
}

} // namespace das_terminal

#else

namespace das_terminal {

std::unique_ptr<PtyProcess> launchPtyProcess(
    const PtyProcessOptions &, std::string & error) {
    error = "PTY transport is not implemented on this platform";
    return std::unique_ptr<PtyProcess>();
}

} // namespace das_terminal

#endif
