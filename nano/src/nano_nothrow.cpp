#include "daScript/misc/platform.h"

#include <cstdlib>

// libstdc++ is prebuilt WITH exceptions, so its throwing helpers carry unwind
// tables and a personality routine. Resolving them here keeps functexcept.o out
// of the link. It does NOT keep the unwinder out - string-inst.o pulls that in
// on its own, for as long as anything calls the out-of-line basic_string core.
//
// A container that overflows on a target which cannot unwind has nowhere to
// throw to, so stopping is the honest answer - the same fail-closed rule the
// rest of nano's seams follow.

#if defined(__GLIBCXX__) && !defined(_GLIBCXX_HAS_GTHREADS)

namespace das {
    // libstdc++ declares every __throw_* noreturn, so this must be too, or the
    // callers keep a return path they cannot use.
    DAS_NORETURN_PREFIX void das_nano_container_fatal ( const char * what ) DAS_NORETURN_SUFFIX;

    void das_nano_container_fatal ( const char * what ) {
        das_fatal_log("%s: nano cannot throw - container limit reached\n", what);
        os_debug_break();
        exit(1);
    }
}

namespace std {
    void __throw_length_error ( const char * ) { das::das_nano_container_fatal("length_error"); }
    void __throw_logic_error ( const char * ) { das::das_nano_container_fatal("logic_error"); }
    void __throw_out_of_range ( const char * ) { das::das_nano_container_fatal("out_of_range"); }
    void __throw_out_of_range_fmt ( const char *, ... ) { das::das_nano_container_fatal("out_of_range"); }
    void __throw_bad_alloc () { das::das_nano_container_fatal("bad_alloc"); }
    void __throw_bad_array_new_length () { das::das_nano_container_fatal("bad_array_new_length"); }
}

#endif
