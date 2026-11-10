#pragma once

#include "daScript/ast/ast.h"

#include <optional>

namespace das {

    struct ProgramCfg;

    std::optional<int64_t> constIntValue ( const Expression * e );
    std::optional<std::pair<int64_t,int64_t>> constRange ( const Expression * src );   // (from, to)

    // Mark provably-in-range ExprAt nodes noBoundCheck, using CFG range facts (opt-in via bound_check_elision).
    void markNoBoundCheck ( Program * program, const ProgramCfg * pcfg, TextWriter & logs );

}
