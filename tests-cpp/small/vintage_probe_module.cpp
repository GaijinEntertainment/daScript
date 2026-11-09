// A shared library shaped like a daslang module whose vintage stamp is wrong, for
// test_register_dynamic_module.cpp: the loader must refuse it without calling its
// registrar. It includes no daScript header on purpose - a probe linked against
// nothing cannot carry the host's vintage by accident.
#include <stdint.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
    #define PROBE_EXPORT __declspec(dllexport)
    static void probe_mark_registrar_ran () { _putenv_s("DAS_VINTAGE_PROBE_REGISTRAR_RAN", "1"); }
#else
    #define PROBE_EXPORT __attribute__((visibility("default")))
    static void probe_mark_registrar_ran () { setenv("DAS_VINTAGE_PROBE_REGISTRAR_RAN", "1", 1); }
#endif

extern "C" {
    PROBE_EXPORT void * register_dyn_VintageProbeDrifted ( int ) {
        probe_mark_registrar_ran();
        return nullptr;
    }
    PROBE_EXPORT uint64_t vintage_dyn_VintageProbeDrifted () {
        return 0x5EEDu;
    }
    PROBE_EXPORT void * register_dyn_VintageProbeUnstamped ( int ) {
        probe_mark_registrar_ran();
        return nullptr;
    }
}
