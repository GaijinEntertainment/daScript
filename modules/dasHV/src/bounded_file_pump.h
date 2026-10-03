#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

namespace das {

enum class FilePumpResult { blocked, progress, complete, failed, canceled };

class BoundedFilePump {
public:
    static constexpr size_t chunk_size = 64 * 1024;
    explicit BoundedFilePump(uint64_t bytes) : remaining_(bytes) {}

    void cancel() { state_ = FilePumpResult::canceled; }

    // modules/dasHV/ARCHITECTURE.md#bounded-file-transfer-lifecycle
    template <typename Read, typename Write>
    FilePumpResult step(bool writable, Read && read, Write && write) {
        if (state_ == FilePumpResult::complete || state_ == FilePumpResult::failed ||
            state_ == FilePumpResult::canceled) return state_;
        if (!writable) return FilePumpResult::blocked;
        if (!remaining_) return state_ = FilePumpResult::complete;
        const size_t count = size_t(std::min<uint64_t>(remaining_, buffer_.size()));
        const size_t received = read(buffer_.data(), count);
        if (!received || received > count || !write(buffer_.data(), received)) {
            return state_ = FilePumpResult::failed;
        }
        remaining_ -= received;
        return FilePumpResult::progress;
    }

private:
    uint64_t remaining_;
    FilePumpResult state_ = FilePumpResult::progress;
    std::array<char, chunk_size> buffer_;
};

}
