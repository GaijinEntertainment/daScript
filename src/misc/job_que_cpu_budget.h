#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <initializer_list>

namespace das {
namespace detail {

// src/misc/ARCHITECTURE.md#cpu-budget
inline int cgroup_quota_lanes ( long long quota, long long period ) {
    if ( quota <= 0 || period <= 0 ) return 0;
    long long lanes = quota / period;
    return lanes < 1 ? 1 : int(lanes);
}

inline int cgroup_v2_cpu_max_lanes ( const std::string & path ) {
    FILE * f = fopen(path.c_str(), "r");
    if ( !f ) return 0;
    char quota[32] = {};
    long long period = 0;
    int got = fscanf(f, "%31s %lld", quota, &period);
    fclose(f);
    if ( got != 2 || std::string(quota) == "max" ) return 0;
    return cgroup_quota_lanes(atoll(quota), period);
}

inline long long read_cgroup_ll ( const std::string & path ) {
    FILE * f = fopen(path.c_str(), "r");
    if ( !f ) return 0;
    long long v = 0;
    int got = fscanf(f, "%lld", &v);
    fclose(f);
    return got == 1 ? v : 0;
}

inline int min_lanes ( int a, int b ) {
    if ( a <= 0 ) return b;
    if ( b <= 0 ) return a;
    return a < b ? a : b;
}

// src/misc/ARCHITECTURE.md#cpu-budget
inline int cgroup_cpu_lanes ( const std::string & cgroupRoot, const std::string & selfCgroup ) {
    std::string v2;
    if ( FILE * f = fopen(selfCgroup.c_str(), "r") ) {
        char line[4096];
        while ( fgets(line, sizeof(line), f) ) {
            std::string s(line);
            if ( s.compare(0, 3, "0::") != 0 ) continue;
            v2 = s.substr(3);
            while ( !v2.empty() && (v2.back() == '\n' || v2.back() == '\r') ) v2.pop_back();
            break;
        }
        fclose(f);
    }
    int lanes = 0;
    if ( !v2.empty() ) {
        for ( ;; ) {
            while ( !v2.empty() && v2.back() == '/' ) v2.pop_back();
            lanes = min_lanes(lanes, cgroup_v2_cpu_max_lanes(cgroupRoot + v2 + "/cpu.max"));
            if ( v2.empty() ) break;
            v2.resize(v2.rfind('/') + 1);
        }
        if ( lanes ) return lanes;
    }
    for ( const char * dir : { "/cpu", "/cpu,cpuacct" } ) {
        std::string base = cgroupRoot + dir;
        lanes = min_lanes(lanes, cgroup_quota_lanes(read_cgroup_ll(base + "/cpu.cfs_quota_us"),
            read_cgroup_ll(base + "/cpu.cfs_period_us")));
    }
    return lanes;
}

inline int cpu_budget ( int online, int affinity, int cgroupLanes ) {
    int budget = min_lanes(min_lanes(online, affinity), cgroupLanes);
    return budget < 1 ? 1 : budget;
}

}
}
