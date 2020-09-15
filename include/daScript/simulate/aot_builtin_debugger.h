#pragma once

namespace das {

    void debuggerStackWalk ( Context & context, const LineInfo & lineInfo );
    void debuggerSetContextSingleStep ( Context & context, bool step );
}
