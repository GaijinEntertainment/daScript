#include <doctest/doctest.h>

#include "daScript/ast/ast.h"
#include "daScript/misc/sysos.h"
#include "../../modules/dasTerminal/src/pty.h"

#include <algorithm>
#include <chrono>
#include <string>
#include <thread>

using das_terminal::PtyProcess;
using das_terminal::PtyProcessOptions;
using das_terminal::PtyReadStatus;

namespace {

bool waitForBytes(PtyProcess & process, std::string & received,
                  const std::string & needle, uint32_t timeout_ms,
                  std::string & error) {
    constexpr size_t retained_output_bytes = 64 * 1024;
    const size_t retained_size = std::max(retained_output_bytes, needle.size());
    const auto deadline = std::chrono::steady_clock::now() +
        std::chrono::milliseconds(timeout_ms);
    while (std::chrono::steady_clock::now() < deadline) {
        std::string bytes;
        const PtyReadStatus status = process.read(bytes, error);
        if (status == PtyReadStatus::data) {
            received += bytes;
            if (received.find(needle) != std::string::npos) return true;
            if (received.size() > retained_size) {
                received.erase(0, received.size() - retained_size);
            }
            continue;
        }
        if (status == PtyReadStatus::error) return false;
        if (status == PtyReadStatus::closed) {
            if (received.find(needle) != std::string::npos) return true;
            error = "PTY closed before output contained '" + needle + "'";
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    error = "timed out waiting for PTY output to contain '" + needle + "'";
    return false;
}

struct PtyProcessGuard {
    explicit PtyProcessGuard(PtyProcess * value) : process(value) {}

    ~PtyProcessGuard() {
        if (!process) return;
        uint32_t exit_code = 0;
        std::string error;
        if (!process->wait(0, exit_code, error)) {
            error.clear();
            process->terminate(99, error);
            error.clear();
            process->wait(5000, exit_code, error);
        }
    }

    PtyProcess * process;
};

} // namespace

TEST_CASE("terminal: native PTY is a raw byte transport") {
#if defined(_WIN32)
    const int integration_timeout_ms = 30000;
    PtyProcessOptions options;
    options.columns = 40;
    options.rows = 10;
    const std::string probe_path =
        das::getDasRoot() + "/modules/dasTerminal/tests/conpty_probe.ps1";
    const std::string arguments =
        " -NoLogo -NoProfile -NonInteractive -ExecutionPolicy Bypass"
        " -File \"" + probe_path + "\"";
    options.command_line = "pwsh.exe" + arguments;

    std::string error;
    std::unique_ptr<PtyProcess> process = das_terminal::launchPtyProcess(options, error);
    if (!process) {
        INFO("pwsh.exe unavailable, exercising the same fixture with Windows PowerShell: " << error);
        error.clear();
        options.command_line = "powershell.exe" + arguments;
        process = das_terminal::launchPtyProcess(options, error);
    }
    INFO(error);
    REQUIRE(process);
    PtyProcessGuard guard{process.get()};
    CHECK(process->processId() != 0);

    std::string received;
    REQUIRE(waitForBytes(
        *process, received, "DASHERD_READY:", integration_timeout_ms, error));
    INFO(error);

    REQUIRE(process->resize(100, 30, error));
    const std::string input = "semantic-probe\r";
    REQUIRE(process->write(
        reinterpret_cast<const uint8_t *>(input.data()), input.size(), error));
    REQUIRE(waitForBytes(
        *process, received, "DASHERD_ECHO:semantic-probe",
        integration_timeout_ms, error));
    INFO(error);

    uint32_t exit_code = 0;
    REQUIRE(process->wait(integration_timeout_ms, exit_code, error));
    INFO(error);
    CHECK(exit_code == 23);
#else
    MESSAGE("PTY transport is not implemented on this platform yet");
#endif
}
