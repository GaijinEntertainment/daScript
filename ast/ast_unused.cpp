#include "precomp.h"

#include "ast.h"

namespace yzg {
    
    class TrackVariableFlags : public Visitor {
    protected:
        // let
    };
    
    class RemoveUnusedLocalVariables : public Visitor {
    public:
        bool didAnything () const { return anyFolding; }
    protected:
        bool        anyFolding = false;
    protected:
    };
    
    // program
    
    bool Program::optimizationUnused() {
        RemoveUnusedLocalVariables context;
        visit(context);
        return context.didAnything();
    }
}

