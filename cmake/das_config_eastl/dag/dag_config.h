#pragma once

// Shadows DagorEngine's prog/1stPartyLibs/dag/dag_config.h for the build_eastl CI job.
// The real one hardwires dagor's kernel (debug/dag_assert.h, memrealloc_default);
// dag::Vector itself only needs these four macros, so we take dag_config's own
// fallback values instead of dragging in the engine.

#include <EASTL/internal/config.h>
#include <util/dag_compilerDefs.h>
#include <assert.h>
#include <stdlib.h>

#define DAG_ASSERTF(expression, fmt, ...) assert(expression)
#define DAG_REALLOC realloc
#define DAG_RESIZE_IN_PLACE(a, b) false
#define DAG_STD_EXPAND_MIN_SIZE 4096
