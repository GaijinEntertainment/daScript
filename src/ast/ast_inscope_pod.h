#pragma once

#include "daScript/ast/ast.h"

namespace das {

    enum class InScopePodRewrite { stackLiteral, scopeCollect, moveCollect };

    string inScopePodRefusal ( const Function * func, InScopePodRewrite rewrite );
    string inScopePodSite ( const Function * func );
    ExprMakeArray * heapArrayLiteral ( Expression * expr );
    bool literalElementsOwnHeap ( const ExprMakeArray * literal );
    ExprMakeArray * stackLoopLiteral ( Expression * source );

}
