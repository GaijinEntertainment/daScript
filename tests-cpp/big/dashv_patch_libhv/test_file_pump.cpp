#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../../../modules/dasHV/src/bounded_file_pump.h"

TEST_CASE("A blocked file response does not read ahead") {
    das::BoundedFilePump pump(3 * das::BoundedFilePump::chunk_size + 7);
    uint64_t read_bytes = 0, written = 0;
    auto read = [&](char * data, size_t count) {
        for (size_t i = 0; i < count; ++i) data[i] = char((read_bytes + i) % 251);
        read_bytes += count;
        return count;
    };
    auto write = [&](const char * data, size_t count) {
        bool correct = true;
        for (size_t i = 0; i < count; ++i) correct &= static_cast<unsigned char>(data[i]) == (written + i) % 251;
        CHECK(correct);
        written += count;
        return true;
    };
    CHECK(pump.step(true, read, write) == das::FilePumpResult::progress);
    CHECK(read_bytes == das::BoundedFilePump::chunk_size);
    for (int i = 0; i < 100; ++i) CHECK(pump.step(false, read, write) == das::FilePumpResult::blocked);
    CHECK(read_bytes == das::BoundedFilePump::chunk_size);
    while (pump.step(true, read, write) == das::FilePumpResult::progress) {}
    CHECK(written == 3 * das::BoundedFilePump::chunk_size + 7);
    CHECK(pump.step(true, read, write) == das::FilePumpResult::complete);
}

TEST_CASE("Canceled, truncated and failed responses never read further") {
    size_t reads = 0;
    auto read = [&](char *, size_t) { ++reads; return size_t(0); };
    auto write = [](const char *, size_t) { return true; };
    das::BoundedFilePump canceled(100);
    canceled.cancel();
    CHECK(canceled.step(true, read, write) == das::FilePumpResult::canceled);
    CHECK(reads == 0);
    das::BoundedFilePump truncated(100);
    CHECK(truncated.step(true, read, write) == das::FilePumpResult::failed);
    CHECK(truncated.step(true, read, write) == das::FilePumpResult::failed);
    CHECK(reads == 1);
    das::BoundedFilePump rejected(100);
    auto present = [&](char *, size_t count) { ++reads; return count; };
    CHECK(rejected.step(true, present, [](const char *, size_t) { return false; }) == das::FilePumpResult::failed);
    CHECK(rejected.step(true, present, write) == das::FilePumpResult::failed);
    CHECK(reads == 2);
}
