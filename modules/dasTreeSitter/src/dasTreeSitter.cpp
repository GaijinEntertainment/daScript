#include "daScript/misc/platform.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_interop.h"

#include "aot_builtin_tree_sitter.h"
#include "dasTreeSitterQueries.h"

#include <tree_sitter/api.h>

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <mutex>
#include <regex>
#include <set>
#include <string>
#include <vector>

extern "C" {
const TSLanguage * tree_sitter_daslang(void);
const TSLanguage * tree_sitter_c(void);
const TSLanguage * tree_sitter_cpp(void);
const TSLanguage * tree_sitter_markdown(void);
const TSLanguage * tree_sitter_markdown_inline(void);
}

namespace das {
namespace {

enum HighlightStatus : int32_t {
    highlight_ok = 0,
    highlight_unknown_language = 1,
    highlight_invalid_source = 2,
    highlight_incompatible_language = 3,
    highlight_query_error = 4,
    highlight_parse_error = 5,
};

struct QueryState {
    const char * canonical_name;
    const char * query_source;
    const TSLanguage * (*language_fn)();
    TSQuery * query = nullptr;
    bool initialized = false;
    bool valid = false;
    std::mutex mutex;
};

QueryState & daslangQuery() {
    static QueryState value{"daslang", DAS_TS_DASLANG_QUERY, tree_sitter_daslang};
    return value;
}

QueryState & cQuery() {
    static QueryState value{"c", DAS_TS_C_QUERY, tree_sitter_c};
    return value;
}

QueryState & cppQuery() {
    static QueryState value{"cpp", DAS_TS_CPP_QUERY, tree_sitter_cpp};
    return value;
}

QueryState & markdownQuery() {
    static QueryState value{"markdown", DAS_TS_MARKDOWN_QUERY, tree_sitter_markdown};
    return value;
}

QueryState & markdownInlineQuery() {
    static QueryState value{
        "markdown_inline", DAS_TS_MARKDOWN_INLINE_QUERY, tree_sitter_markdown_inline};
    return value;
}

std::string normalizedLanguage(const char * language) {
    std::string value = language ? language : "";
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    if (!value.empty() && value.front() == '.') value.erase(value.begin());
    return value;
}

QueryState * resolveLanguage(const char * language) {
    const std::string value = normalizedLanguage(language);
    if (value == "das" || value == "daslang" || value == "dascript") {
        return &daslangQuery();
    }
    if (value == "c") return &cQuery();
    if (value == "cpp" || value == "c++" || value == "cc" || value == "cxx" ||
            value == "h" || value == "hh" || value == "hpp" || value == "hxx") {
        return &cppQuery();
    }
    if (value == "md" || value == "markdown") return &markdownQuery();
    return nullptr;
}

bool ensureQuery(QueryState & state) {
    std::lock_guard<std::mutex> guard(state.mutex);
    if (state.initialized) return state.valid;
    state.initialized = true;

    uint32_t errorOffset = 0;
    TSQueryError errorType = TSQueryErrorNone;
    state.query = ts_query_new(
        state.language_fn(), state.query_source,
        static_cast<uint32_t>(std::char_traits<char>::length(state.query_source)),
        &errorOffset, &errorType);
    state.valid = state.query != nullptr && errorType == TSQueryErrorNone;
    return state.valid;
}

struct CaptureSpan {
    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t priority = 0;
    uint32_t order = 0;
    std::string capture;
};

struct FlatToken {
    uint32_t start = 0;
    uint32_t end = 0;
    std::string capture;
};

std::string nodeText(TSNode node, const char * source, uint32_t sourceSize) {
    const uint32_t start = std::min(ts_node_start_byte(node), sourceSize);
    const uint32_t end = std::min(ts_node_end_byte(node), sourceSize);
    if (end <= start) return {};
    return std::string(source + start, source + end);
}

bool captureText(
    const TSQueryMatch & match,
    uint32_t captureId,
    const char * source,
    uint32_t sourceSize,
    std::string & value) {
    for (uint16_t index = 0; index != match.capture_count; ++index) {
        const TSQueryCapture & capture = match.captures[index];
        if (capture.index == captureId) {
            value = nodeText(capture.node, source, sourceSize);
            return true;
        }
    }
    return false;
}

std::string queryString(const TSQuery * query, uint32_t id) {
    uint32_t length = 0;
    const char * value = ts_query_string_value_for_id(query, id, &length);
    return value ? std::string(value, value + length) : std::string();
}

bool evaluatePredicate(
    const TSQuery * query,
    const TSQueryMatch & match,
    const TSQueryPredicateStep * steps,
    uint32_t count,
    uint32_t & index,
    const char * source,
    uint32_t sourceSize) {
    if (index >= count || steps[index].type != TSQueryPredicateStepTypeString) return false;
    std::string operation = queryString(query, steps[index++].value_id);
    if (!operation.empty() && operation.front() == '#') operation.erase(operation.begin());

    std::vector<std::string> arguments;
    std::vector<bool> argumentPresent;
    while (index < count && steps[index].type != TSQueryPredicateStepTypeDone) {
        const TSQueryPredicateStep & step = steps[index++];
        std::string value;
        bool present = true;
        if (step.type == TSQueryPredicateStepTypeCapture) {
            present = captureText(match, step.value_id, source, sourceSize, value);
        } else if (step.type == TSQueryPredicateStepTypeString) {
            value = queryString(query, step.value_id);
        } else {
            present = false;
        }
        arguments.push_back(std::move(value));
        argumentPresent.push_back(present);
    }
    if (index < count && steps[index].type == TSQueryPredicateStepTypeDone) ++index;

    if (arguments.size() < 2 || !argumentPresent[0] || !argumentPresent[1]) return false;
    const bool negate = operation == "not-match?" || operation == "not-eq?";
    bool matched = false;
    if (operation == "match?" || operation == "not-match?") {
        try {
            matched = std::regex_search(arguments[0], std::regex(arguments[1]));
        } catch (const std::regex_error &) {
            return false;
        }
    } else if (operation == "eq?" || operation == "not-eq?") {
        matched = arguments[0] == arguments[1];
    } else {
        // Highlight packs are intentionally restricted to predicates the
        // native provider evaluates. Unknown predicates must not over-highlight.
        return false;
    }
    return negate ? !matched : matched;
}

bool predicatesMatch(
    const TSQuery * query,
    const TSQueryMatch & match,
    const char * source,
    uint32_t sourceSize) {
    uint32_t count = 0;
    const TSQueryPredicateStep * steps =
        ts_query_predicates_for_pattern(query, match.pattern_index, &count);
    uint32_t index = 0;
    while (index < count) {
        if (steps[index].type == TSQueryPredicateStepTypeDone) {
            ++index;
            continue;
        }
        if (!evaluatePredicate(query, match, steps, count, index, source, sourceSize)) {
            return false;
        }
    }
    return true;
}

void collectQueryCaptures(
    TSNode root,
    const TSQuery * query,
    const char * source,
    uint32_t sourceSize,
    uint32_t baseOffset,
    uint32_t priorityBias,
    uint32_t & order,
    std::vector<CaptureSpan> & captures) {
    TSQueryCursor * cursor = ts_query_cursor_new();
    if (!cursor) return;
    ts_query_cursor_exec(cursor, query, root);

    TSQueryMatch match{};
    while (ts_query_cursor_next_match(cursor, &match)) {
        if (!predicatesMatch(query, match, source, sourceSize)) continue;
        for (uint16_t index = 0; index != match.capture_count; ++index) {
            const TSQueryCapture & captured = match.captures[index];
            uint32_t nameLength = 0;
            const char * name =
                ts_query_capture_name_for_id(query, captured.index, &nameLength);
            const uint32_t start = ts_node_start_byte(captured.node);
            const uint32_t end = ts_node_end_byte(captured.node);
            if (!name || end <= start || start >= sourceSize) continue;
            captures.push_back(CaptureSpan{
                baseOffset + start,
                baseOffset + std::min(end, sourceSize),
                priorityBias + match.pattern_index,
                order++,
                std::string(name, name + nameLength)});
        }
    }
    ts_query_cursor_delete(cursor);
}

TSTree * parseSource(
    const TSLanguage * language,
    const char * source,
    uint32_t sourceSize,
    bool & compatible) {
    TSParser * parser = ts_parser_new();
    if (!parser) return nullptr;
    compatible = ts_parser_set_language(parser, language);
    TSTree * tree = compatible
        ? ts_parser_parse_string_encoding(
              parser, nullptr, source, sourceSize, TSInputEncodingUTF8)
        : nullptr;
    ts_parser_delete(parser);
    return tree;
}

void collectInlineNodes(TSNode node, std::vector<TSNode> & result) {
    const char * type = ts_node_type(node);
    if (type && std::string(type) == "inline") {
        result.push_back(node);
        return;
    }
    const uint32_t count = ts_node_named_child_count(node);
    for (uint32_t index = 0; index != count; ++index) {
        collectInlineNodes(ts_node_named_child(node, index), result);
    }
}

int32_t collectHighlights(
    QueryState & state,
    const char * source,
    uint32_t sourceSize,
    std::vector<CaptureSpan> & captures,
    TSTree ** keep_tree = nullptr) {
    if (!ensureQuery(state)) return highlight_query_error;
    bool compatible = false;
    TSTree * tree = parseSource(state.language_fn(), source, sourceSize, compatible);
    if (!compatible) return highlight_incompatible_language;
    if (!tree) return highlight_parse_error;

    uint32_t order = 0;
    collectQueryCaptures(
        ts_tree_root_node(tree), state.query, source, sourceSize, 0, 0, order, captures);

    if (&state == &markdownQuery()) {
        QueryState & inlineState = markdownInlineQuery();
        if (!ensureQuery(inlineState)) {
            ts_tree_delete(tree);
            return highlight_query_error;
        }
        std::vector<TSNode> inlineNodes;
        collectInlineNodes(ts_tree_root_node(tree), inlineNodes);
        for (TSNode inlineNode : inlineNodes) {
            const uint32_t start = ts_node_start_byte(inlineNode);
            const uint32_t end = std::min(ts_node_end_byte(inlineNode), sourceSize);
            if (end <= start) continue;
            bool inlineCompatible = false;
            TSTree * inlineTree = parseSource(
                inlineState.language_fn(), source + start, end - start, inlineCompatible);
            if (!inlineCompatible) {
                ts_tree_delete(tree);
                return highlight_incompatible_language;
            }
            if (!inlineTree) {
                ts_tree_delete(tree);
                return highlight_parse_error;
            }
            collectQueryCaptures(
                ts_tree_root_node(inlineTree), inlineState.query,
                source + start, end - start, start, 1000000u, order, captures);
            ts_tree_delete(inlineTree);
        }
    }

    if (keep_tree) {
        *keep_tree = tree; // caller owns it — reused for the structure walk
    } else {
        ts_tree_delete(tree);
    }
    return highlight_ok;
}

using HighlightBlock = TBlock<void, int32_t, int32_t, TTemporary<const char *>>;
using StructureBlock =
    TBlock<void, TTemporary<const char *>, int32_t, int32_t, int32_t, int32_t>;

void walkStructure(
    TSTree * tree,
    int32_t source_size,
    const StructureBlock & block,
    Context * context,
    LineInfoArg * at) {
    // Named-node walk for structure consumers (folding, future decorations).
    // Only multi-line nodes are reported; single-line nodes cannot fold and
    // filtering here keeps the das-side callback volume proportional to the
    // outline, not the token count.
    TSTreeCursor cursor = ts_tree_cursor_new(ts_tree_root_node(tree));
    bool descending = true;
    while (true) {
        if (descending) {
            const TSNode node = ts_tree_cursor_current_node(&cursor);
            const TSPoint startPoint = ts_node_start_point(node);
            const TSPoint endPoint = ts_node_end_point(node);
            if (ts_node_is_named(node) && endPoint.row > startPoint.row) {
                das_invoke<void>::invoke<const char *, int32_t, int32_t, int32_t, int32_t>(
                    context, at, block,
                    ts_node_type(node),
                    static_cast<int32_t>(ts_node_start_byte(node)),
                    static_cast<int32_t>(std::min(
                        ts_node_end_byte(node), static_cast<uint32_t>(source_size))),
                    static_cast<int32_t>(startPoint.row),
                    static_cast<int32_t>(endPoint.row));
            }
            // A single-line node cannot contain a multi-line child.
            if (endPoint.row > startPoint.row && ts_tree_cursor_goto_first_child(&cursor)) {
                continue;
            }
        }
        if (ts_tree_cursor_goto_next_sibling(&cursor)) {
            descending = true;
        } else if (ts_tree_cursor_goto_parent(&cursor)) {
            descending = false;
        } else {
            break;
        }
    }
    ts_tree_cursor_delete(&cursor);
}

std::vector<FlatToken> flattenCaptures(const std::vector<CaptureSpan> & captures) {
    std::vector<FlatToken> result;
    if (captures.empty()) return result;

    std::vector<size_t> starts(captures.size());
    std::vector<size_t> ends(captures.size());
    for (size_t index = 0; index != captures.size(); ++index) {
        starts[index] = ends[index] = index;
    }
    std::sort(starts.begin(), starts.end(), [&](size_t left, size_t right) {
        if (captures[left].start != captures[right].start) {
            return captures[left].start < captures[right].start;
        }
        return left < right;
    });
    std::sort(ends.begin(), ends.end(), [&](size_t left, size_t right) {
        if (captures[left].end != captures[right].end) {
            return captures[left].end < captures[right].end;
        }
        return left < right;
    });

    struct ActiveLess {
        const std::vector<CaptureSpan> * captures = nullptr;
        bool operator()(size_t left, size_t right) const {
            const CaptureSpan & a = (*captures)[left];
            const CaptureSpan & b = (*captures)[right];
            if (a.priority != b.priority) return a.priority < b.priority;
            const uint32_t aLength = a.end - a.start;
            const uint32_t bLength = b.end - b.start;
            if (aLength != bLength) return aLength > bLength;
            if (a.order != b.order) return a.order < b.order;
            return left < right;
        }
    };

    std::set<size_t, ActiveLess> active((ActiveLess{&captures}));
    size_t startIndex = 0;
    size_t endIndex = 0;
    uint32_t position = std::min(captures[starts[0]].start, captures[ends[0]].end);
    while (startIndex < starts.size() || endIndex < ends.size()) {
        while (endIndex < ends.size() && captures[ends[endIndex]].end == position) {
            active.erase(ends[endIndex++]);
        }
        while (startIndex < starts.size() && captures[starts[startIndex]].start == position) {
            active.insert(starts[startIndex++]);
        }

        uint32_t next = UINT32_MAX;
        if (startIndex < starts.size()) next = std::min(next, captures[starts[startIndex]].start);
        if (endIndex < ends.size()) next = std::min(next, captures[ends[endIndex]].end);
        if (next == UINT32_MAX || next <= position) break;

        if (!active.empty()) {
            const CaptureSpan & selected = captures[*active.rbegin()];
            if (selected.capture != "none") {
                if (!result.empty() && result.back().end == position &&
                        result.back().capture == selected.capture) {
                    result.back().end = next;
                } else {
                    result.push_back(FlatToken{position, next, selected.capture});
                }
            }
        }
        position = next;
    }
    return result;
}

void emitHighlights(
    const std::vector<CaptureSpan> & captures,
    const HighlightBlock & block,
    Context * context,
    LineInfoArg * at) {
    const std::vector<FlatToken> tokens = flattenCaptures(captures);
    for (const FlatToken & token : tokens) {
        das_invoke<void>::invoke<int32_t, int32_t, const char *>(
            context, at, block,
            static_cast<int32_t>(token.start),
            static_cast<int32_t>(token.end),
            token.capture.c_str());
    }
}

} // namespace

bool builtin_tree_sitter_language_available(const char * language) {
    QueryState * state = resolveLanguage(language);
    return state && ensureQuery(*state);
}

int32_t builtin_tree_sitter_structure(
    const char * language,
    const char * source,
    int32_t source_size,
    const TBlock<void, TTemporary<const char *>, int32_t, int32_t, int32_t, int32_t> & block,
    Context * context,
    LineInfoArg * at) {
    QueryState * state = resolveLanguage(language);
    if (!state) return highlight_unknown_language;
    if (!source || source_size < 0) return highlight_invalid_source;

    bool compatible = false;
    TSTree * tree = parseSource(
        state->language_fn(), source, static_cast<uint32_t>(source_size), compatible);
    if (!compatible) return highlight_incompatible_language;
    if (!tree) return highlight_parse_error;

    walkStructure(tree, source_size, block, context, at);
    ts_tree_delete(tree);
    return highlight_ok;
}

int32_t builtin_tree_sitter_highlight(
    const char * language,
    const char * source,
    int32_t source_size,
    const TBlock<void, int32_t, int32_t, TTemporary<const char *>> & block,
    Context * context,
    LineInfoArg * at) {
    QueryState * state = resolveLanguage(language);
    if (!state) return highlight_unknown_language;
    if (!source || source_size < 0) return highlight_invalid_source;

    std::vector<CaptureSpan> captures;
    const int32_t status = collectHighlights(
        *state, source, static_cast<uint32_t>(source_size), captures);
    if (status != highlight_ok) return status;

    emitHighlights(captures, block, context, at);
    return highlight_ok;
}

int32_t builtin_tree_sitter_highlight_and_structure(
    const char * language,
    const char * source,
    int32_t source_size,
    const TBlock<void, int32_t, int32_t, TTemporary<const char *>> & highlight_block,
    const TBlock<void, TTemporary<const char *>, int32_t, int32_t, int32_t, int32_t> & structure_block,
    Context * context,
    LineInfoArg * at) {
    // One parse serves both consumers: highlight captures AND the multi-line
    // structure outline (folds). Same results as calling the two split externs,
    // minus the second full parse of the document.
    QueryState * state = resolveLanguage(language);
    if (!state) return highlight_unknown_language;
    if (!source || source_size < 0) return highlight_invalid_source;

    std::vector<CaptureSpan> captures;
    TSTree * tree = nullptr;
    const int32_t status = collectHighlights(
        *state, source, static_cast<uint32_t>(source_size), captures, &tree);
    if (status != highlight_ok) return status;

    emitHighlights(captures, highlight_block, context, at);
    walkStructure(tree, source_size, structure_block, context, at);
    ts_tree_delete(tree);
    return highlight_ok;
}

class Module_TreeSitter final : public Module {
public:
    Module_TreeSitter() : Module("tree_sitter_core") {
        ModuleLibrary lib(this);
        lib.addBuiltInModule();

        addExtern<DAS_BIND_FUN(builtin_tree_sitter_language_available)>(
            *this, lib, "_tree_sitter_language_available", SideEffects::none,
            "builtin_tree_sitter_language_available")
                ->args({"language"});
        addExtern<DAS_BIND_FUN(builtin_tree_sitter_highlight)>(
            *this, lib, "_tree_sitter_highlight", SideEffects::invoke,
            "builtin_tree_sitter_highlight")
                ->args({"language", "source", "source_size", "block", "context", "at"});
        addExtern<DAS_BIND_FUN(builtin_tree_sitter_structure)>(
            *this, lib, "_tree_sitter_structure", SideEffects::invoke,
            "builtin_tree_sitter_structure")
                ->args({"language", "source", "source_size", "block", "context", "at"});
        addExtern<DAS_BIND_FUN(builtin_tree_sitter_highlight_and_structure)>(
            *this, lib, "_tree_sitter_highlight_and_structure", SideEffects::invoke,
            "builtin_tree_sitter_highlight_and_structure")
                ->args({"language", "source", "source_size", "highlight_block",
                    "structure_block", "context", "at"});

        verifyBuiltinNames(uint32_t(VerifyBuiltinFlags::verifyAll));
        verifyAotReady();
    }

    ModuleAotType aotRequire(TextWriter & tw) const override {
        tw << "#include \"../modules/dasTreeSitter/src/aot_builtin_tree_sitter.h\"\n";
        return ModuleAotType::cpp;
    }
};

REGISTER_DYN_MODULE(Module_TreeSitter, Module_TreeSitter);

} // namespace das

REGISTER_MODULE_IN_NAMESPACE(Module_TreeSitter, das);
