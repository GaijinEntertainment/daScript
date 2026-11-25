#pragma once

#include "daScript/misc/platform.h"

#include <memory>

namespace das {

    class JobserverState;

    class DAS_API JobserverToken {
    public:
        JobserverToken ( JobserverToken && other ) noexcept;
        JobserverToken & operator = ( JobserverToken && other ) noexcept;
        ~JobserverToken ();
    private:
        friend class Jobserver;
        JobserverToken ( JobserverState * s, bool i, char b ) : state(s), implicitToken(i), byte(b) {}
        JobserverState *    state = nullptr;
        bool                implicitToken = false;
        char                byte = 0;
    };

    // src/misc/ARCHITECTURE_JOBSERVER.md#jobserver-client
    class DAS_API Jobserver {
    public:
        explicit Jobserver ( int32_t limit = 0 );
        Jobserver ( int32_t limit, const char * makeflags );
        ~Jobserver ();
        Jobserver ( const Jobserver & ) = delete;
        Jobserver & operator = ( const Jobserver & ) = delete;
        JobserverToken acquire ();
    private:
        std::unique_ptr<JobserverState> state;
    };
}
