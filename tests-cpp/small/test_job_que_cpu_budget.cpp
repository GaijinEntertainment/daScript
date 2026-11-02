#include <doctest/doctest.h>
#include "../../src/misc/job_que_cpu_budget.h"

#include <filesystem>
#include <fstream>
#include <random>

namespace {
namespace fs = std::filesystem;

struct FakeCgroup {
    fs::path root;
    FakeCgroup() {
        std::random_device rd;
        root = fs::temp_directory_path() / ("das_cgroup_" + std::to_string(rd()));
        fs::create_directories(root);
    }
    ~FakeCgroup() {
        std::error_code ec;
        fs::remove_all(root, ec);
    }
    void put(const std::string & rel, const std::string & text) const {
        fs::path p = root / rel;
        fs::create_directories(p.parent_path());
        std::ofstream(p) << text;
    }
    std::string sys() const { return (root / "sys").string(); }
    std::string self() const { return (root / "self_cgroup").string(); }
    int lanes() const { return das::detail::cgroup_cpu_lanes(sys(), self()); }
};
}

TEST_CASE("cgroup v2 quota floors to whole lanes") {
    FakeCgroup cg;
    cg.put("self_cgroup", "0::/\n");
    cg.put("sys/cpu.max", "2720000 100000\n");
    CHECK_EQ(cg.lanes(), 27);
}

TEST_CASE("cgroup v2 max means no limit") {
    FakeCgroup cg;
    cg.put("self_cgroup", "0::/\n");
    cg.put("sys/cpu.max", "max 100000\n");
    CHECK_EQ(cg.lanes(), 0);
}

TEST_CASE("cgroup v2 takes the tightest quota up the hierarchy") {
    FakeCgroup cg;
    cg.put("self_cgroup", "0::/pod/ctr\n");
    cg.put("sys/pod/ctr/cpu.max", "max 100000\n");
    cg.put("sys/pod/cpu.max", "800000 100000\n");
    cg.put("sys/cpu.max", "1600000 100000\n");
    CHECK_EQ(cg.lanes(), 8);
}

TEST_CASE("cgroup quota under one cpu still yields one lane") {
    FakeCgroup cg;
    cg.put("self_cgroup", "0::/\n");
    cg.put("sys/cpu.max", "50000 100000\n");
    CHECK_EQ(cg.lanes(), 1);
}

TEST_CASE("cgroup v1 quota is read when v2 sets none") {
    FakeCgroup cg;
    cg.put("self_cgroup", "4:cpu,cpuacct:/\n0::/\n");
    cg.put("sys/cpu,cpuacct/cpu.cfs_quota_us", "600000\n");
    cg.put("sys/cpu,cpuacct/cpu.cfs_period_us", "100000\n");
    CHECK_EQ(cg.lanes(), 6);
}

TEST_CASE("cgroup v1 quota of -1 means no limit") {
    FakeCgroup cg;
    cg.put("self_cgroup", "4:cpu:/\n");
    cg.put("sys/cpu/cpu.cfs_quota_us", "-1\n");
    cg.put("sys/cpu/cpu.cfs_period_us", "100000\n");
    CHECK_EQ(cg.lanes(), 0);
}

TEST_CASE("no cgroup files means no limit") {
    FakeCgroup cg;
    CHECK_EQ(cg.lanes(), 0);
}

TEST_CASE("cpu budget is the least of online, affinity and quota") {
    CHECK_EQ(das::detail::cpu_budget(128, 128, 27), 27);
    CHECK_EQ(das::detail::cpu_budget(128, 16, 27), 16);
    CHECK_EQ(das::detail::cpu_budget(48, 0, 0), 48);
    CHECK_EQ(das::detail::cpu_budget(0, 0, 0), 1);
}
