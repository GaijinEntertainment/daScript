#pragma once
// Regression fixture for cbind preprocessor-active constant generation.
//
// The opt-in macro CONST_PREPROC_USE_ALT is intentionally left undefined, so
// the preprocessor-ACTIVE branch is the #else one (value 0). A naive raw-text
// regex scan would pick the first textual match (the inactive #ifdef branch,
// value 16); the MacroDefinition-cursor path in cbind_boost.das must pick the
// active value 0. Mirrors imgui.h's IM_COL32_*_SHIFT under
// IMGUI_USE_BGRA_PACKED_COLOR.
#ifdef CONST_PREPROC_USE_ALT
#define CONST_PREPROC_K 16
#else
#define CONST_PREPROC_K 0
#endif

#define CONST_PREPROC_PLAIN 42
