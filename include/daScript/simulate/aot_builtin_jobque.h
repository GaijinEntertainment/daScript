#pragma once

#include "daScript/misc/job_que.h"
#include "daScript/misc/arraytype.h"

namespace das {
    class Context;
    struct LineInfoArg;
    void pinvoke ( Func fn, Context * context, LineInfoArg * lineinfo );
    void new_job_invoke ( Lambda lambda, Func fn, int32_t lambdaSize, Context * context, LineInfoArg * lineinfo );
    __forceinline Context  * thisContext ( Context * context ) { return context; }
    void withJobQue ( const TBlock<void> & block, Context * context, LineInfoArg * lineInfo );
    int getTotalHwJobs( Context * context, LineInfoArg * at );
    void withJobStatus ( int32_t total, const TBlock<void,JobStatus *> & block, Context * context, LineInfoArg * lineInfo );
    void waitForJob ( JobStatus * status );
    void notifyJob ( JobStatus * status );
}
