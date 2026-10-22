#pragma once

#include "daScript/ast/ast.h"

namespace das {
    bool nodeRaises ( Expression * expr );
    bool callRaises ( Function * fn );
    bool droppable ( Expression * expr );
}
