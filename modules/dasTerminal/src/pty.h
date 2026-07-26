#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace das_terminal {

struct PtyProcessOptions {
    std::string command_line;
    std::vector<std::string> arguments;
    std::string working_directory;
    // Extra KEY=VALUE entries merged over the inherited environment (same-key
    // entries replace inherited ones). Empty = inherit unchanged. Secrets ride
    // here instead of the command line, which any local process can read.
    std::vector<std::string> environment;
    int columns = 80;
    int rows = 25;
};

enum class PtyReadStatus : uint8_t {
    data,
    idle,
    closed,
    error,
};

class PtyProcess {
public:
    virtual ~PtyProcess() = default;

    virtual uint32_t processId() const = 0;
    virtual PtyReadStatus read(std::string & bytes, std::string & error,
                               size_t maximum_bytes = 64 * 1024) = 0;
    virtual bool write(const uint8_t * bytes, size_t count, std::string & error) = 0;
    bool write(const std::string & bytes, std::string & error) {
        return write(reinterpret_cast<const uint8_t *>(bytes.data()), bytes.size(), error);
    }
    virtual bool resize(int columns, int rows, std::string & error) = 0;
    virtual bool wait(uint32_t timeout_ms, uint32_t & exit_code, std::string & error) = 0;
    virtual bool terminate(uint32_t exit_code, std::string & error) = 0;
};

std::unique_ptr<PtyProcess> launchPtyProcess(
    const PtyProcessOptions & options, std::string & error);

// Spawns a plain process that outlives the caller: detached from the caller's
// console, broken away from its job object, in its own process group, with no
// inherited pipe handles. The child is deliberately not tracked — discovery is
// the caller's business (stamp files). Returns the pid, 0 on failure. A pid
// with a non-empty error is a degraded success (e.g. job breakaway denied).
uint32_t spawnDetachedProcess(
    const std::vector<std::string> & arguments,
    const std::string & working_directory,
    const std::vector<std::string> & environment,
    std::string & error);

} // namespace das_terminal
