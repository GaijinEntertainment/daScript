#ifndef DAS_DAS_COMMON_H
#define DAS_DAS_COMMON_H

#include <daScript/simulate/simulate.h>
#include <daScript/das_project_specific.h>
#include <daScript/ast/ast.h>

namespace das {
    inline ContextPtr SimulateWithErrReport(ProgramPtr program, TextWriter &tw) {
        ContextPtr pctx ( get_context(program->getContextStackSize()) );
        if ( !program->simulate(*pctx, tw) ) {
            tw << "failed to simulate\n";
            for ( auto & err : program->errors ) {
                tw << reportError(err.at, err.what, err.extra, err.fixme, err.cerr);
            }
            return nullptr;
        }
        return pctx;
    }


}

#endif //DAS_DAS_COMMON_H
