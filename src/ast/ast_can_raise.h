#pragma once

#include "daScript/ast/ast.h"

namespace das {

    // ===== shared may-raise question for the passes that DELETE an evaluation =====
    // `noSideEffects` answers "writes nothing an observer can see", not "cannot throw",
    // and a checked operation answers yes to the first and no to the second. A pass that
    // drops an evaluation drops its throw, so it asks this too. CSE does not: it reuses
    // the first evaluation, which still throws.
    //
    // The question is about the whole subtree - the root of `a[10] + 1` is an ExprOp2
    // that cannot throw on its own - so it walks, and every caller asks it last, after
    // the cheap flags. Operations that throw AND write are already excluded by the
    // `noSideEffects` half of each caller's condition.
    bool canRaise ( Expression * expr );

    // ExpressionPtr is a raw pointer in some configurations and a smart_ptr in others;
    // the non-template overload wins for the first, this one serves the second.
    template <typename TT>
    inline bool canRaise ( const TT & expr ) {
        return canRaise(expr.get());
    }

    // An evaluation a pass is about to DROP: it must neither write nor throw.
    bool droppable ( Expression * expr );
}
