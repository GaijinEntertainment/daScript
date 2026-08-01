#pragma once

#include "daScript/daScript.h"

namespace das {

bool builtin_tree_sitter_language_available(const char * language);

int32_t builtin_tree_sitter_highlight(
    const char * language,
    const char * source,
    int32_t source_size,
    const TBlock<void, int32_t, int32_t, TTemporary<const char *>> & block,
    Context * context,
    LineInfoArg * at);

int32_t builtin_tree_sitter_structure(
    const char * language,
    const char * source,
    int32_t source_size,
    const TBlock<void, TTemporary<const char *>, int32_t, int32_t, int32_t, int32_t> & block,
    Context * context,
    LineInfoArg * at);

int32_t builtin_tree_sitter_highlight_and_structure(
    const char * language,
    const char * source,
    int32_t source_size,
    const TBlock<void, int32_t, int32_t, TTemporary<const char *>> & highlight_block,
    const TBlock<void, TTemporary<const char *>, int32_t, int32_t, int32_t, int32_t> & structure_block,
    Context * context,
    LineInfoArg * at);

}
