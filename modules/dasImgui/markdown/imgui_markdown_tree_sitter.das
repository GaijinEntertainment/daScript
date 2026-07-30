options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_markdown_tree_sitter shared

require imgui/imgui_markdown_view public
require tree_sitter/tree_sitter public
require strings

//! Optional Tree-sitter adapter for the Markdown viewer. The base viewer stays
//! parser-neutral and remains usable when dasTreeSitter is disabled.

struct public MarkdownTreeSitterOverride {
    document_revision : int = -1
    node_index : int = -1
    //! `auto` uses the fence label; `none` is neutral; any other value is a
    //! Tree-sitter language alias.
    language : string = "auto"
}

struct public MarkdownTreeSitterApplied {
    document_revision : int = -1
    node_index : int = -1
    language : string
}

struct public MarkdownTreeSitterState {
    //! Viewer-wide policy: `auto`, `none`, or a language alias.
    default_language : string = "auto"
    overrides : array<MarkdownTreeSitterOverride>
    applied : array<MarkdownTreeSitterApplied>
    revision : int = 0
}

def private normalized_markdown_language(language : string) : string {
    let value = to_lower(language)
    if (empty(value) || value == "auto") return "auto"
    if (value == "none" || value == "neutral" || value == "plain"
            || value == "plaintext" || value == "text") return "none"
    return syntax_canonical_language(value)
}

def private override_index(state : MarkdownTreeSitterState const;
                           document_revision, node_index : int) : int {
    for (i in range(length(state.overrides))) {
        let entry = state.overrides[i]
        if (entry.document_revision == document_revision
                && entry.node_index == node_index) return i
    }
    return -1
}

def private applied_index(state : MarkdownTreeSitterState const;
                          document_revision, node_index : int) : int {
    for (i in range(length(state.applied))) {
        let entry = state.applied[i]
        if (entry.document_revision == document_revision
                && entry.node_index == node_index) return i
    }
    return -1
}

def public markdown_tree_sitter_set_default(var state : MarkdownTreeSitterState;
                                            language : string) : bool {
    let normalized = normalized_markdown_language(language)
    if (state.default_language == normalized) return false
    state.default_language := normalized
    state.revision++
    return true
}

def public markdown_tree_sitter_clear_override(var state : MarkdownTreeSitterState;
                                               document_revision,
                                               node_index : int) : bool {
    let index = override_index(state, document_revision, node_index)
    if (index < 0) return false
    state.overrides |> erase(index)
    state.revision++
    return true
}

def public markdown_tree_sitter_set_override(var state : MarkdownTreeSitterState;
                                             document_revision,
                                             node_index : int;
                                             language : string) : bool {
    if (language == "inherit") {
        return markdown_tree_sitter_clear_override(
            state, document_revision, node_index)
    }
    if (document_revision < 0 || node_index < 0) return false
    let normalized = normalized_markdown_language(language)
    let index = override_index(state, document_revision, node_index)
    if (index >= 0) {
        if (state.overrides[index].language == normalized) return false
        state.overrides[index].language := normalized
    } else {
        state.overrides |> push(MarkdownTreeSitterOverride(
            document_revision = document_revision,
            node_index = node_index,
            language = normalized))
    }
    state.revision++
    return true
}

def public markdown_tree_sitter_override_language(
        state : MarkdownTreeSitterState const;
        document_revision, node_index : int) : string {
    let index = override_index(state, document_revision, node_index)
    return index >= 0 ? state.overrides[index].language : "inherit"
}

def public markdown_tree_sitter_effective_language(
        doc : RichDocument const; state : MarkdownTreeSitterState const;
        document_revision, node_index : int) : string {
    if (node_index < 0 || node_index >= length(doc.nodes)) return ""
    let index = override_index(state, document_revision, node_index)
    let policy = (index >= 0
        ? state.overrides[index].language : state.default_language)
    if (policy == "none") return ""
    if (policy == "auto") {
        return syntax_canonical_language(doc.nodes[node_index].language)
    }
    return syntax_canonical_language(policy)
}

def public markdown_tree_sitter_dispose(var state : MarkdownTreeSitterState) {
    delete state.overrides
    delete state.applied
    state = MarkdownTreeSitterState()
}

def private retain_markdown_syntax_revision(var state : MarkdownTreeSitterState;
                                            document_revision : int) {
    var index = length(state.overrides) - 1
    while (index >= 0) {
        if (state.overrides[index].document_revision != document_revision) {
            state.overrides |> erase(index)
        }
        index--
    }
    index = length(state.applied) - 1
    while (index >= 0) {
        if (state.applied[index].document_revision != document_revision) {
            state.applied |> erase(index)
        }
        index--
    }
}

def private markdown_syntax_style(role : SyntaxRole) : int {
    if (role == SyntaxRole.comment) {
        return int(MarkdownSyntaxStyle.comment)
    } elif (role == SyntaxRole.string_ || role == SyntaxRole.character
            || role == SyntaxRole.escape) {
        return int(MarkdownSyntaxStyle.string_)
    } elif (role == SyntaxRole.number || role == SyntaxRole.boolean_) {
        return int(MarkdownSyntaxStyle.number)
    } elif (role == SyntaxRole.keyword || role == SyntaxRole.attribute
            || role == SyntaxRole.module_) {
        return int(MarkdownSyntaxStyle.keyword)
    } elif (role == SyntaxRole.type_) {
        return int(MarkdownSyntaxStyle.type_)
    } elif (role == SyntaxRole.function_) {
        return int(MarkdownSyntaxStyle.function_)
    } elif (role == SyntaxRole.variable || role == SyntaxRole.parameter
            || role == SyntaxRole.label_) {
        return int(MarkdownSyntaxStyle.variable)
    } elif (role == SyntaxRole.property) {
        return int(MarkdownSyntaxStyle.property)
    } elif (role == SyntaxRole.constant) {
        return int(MarkdownSyntaxStyle.constant)
    } elif (role == SyntaxRole.operator_) {
        return int(MarkdownSyntaxStyle.operator_)
    } elif (role == SyntaxRole.punctuation) {
        return int(MarkdownSyntaxStyle.punctuation)
    }
    return -1
}

def private highlight_markdown_code(doc : RichDocument const; node_index : int;
                                    language : string;
                                    var projected : array<MarkdownSyntaxToken>) : bool {
    if (empty(language)) return false
    var content <- md_inline_content(doc, node_index)
    var tokens : array<SyntaxToken>
    let ok = (syntax_highlight(language, content.text, tokens)
        == SyntaxHighlightStatus.ok)
    if (ok) {
        for (token in tokens) {
            let syntax_style = markdown_syntax_style(token.role)
            if (syntax_style >= 0) {
                projected |> push(MarkdownSyntaxToken(
                    start_byte = token.start_byte,
                    end_byte = token.end_byte,
                    style = unsafe(reinterpret<MarkdownSyntaxStyle>(syntax_style))))
            }
        }
    }
    delete tokens
    delete content.runs
    return ok
}

def public markdown_tree_sitter_prepare(doc : RichDocument const;
                                        var state : MarkdownViewState;
                                        document_revision : int = 0) : int {
    //! Tokenize each fenced/indented code block once for this document
    //! revision. Unsupported or unlabelled fences bind an empty result so a
    //! stable frame never repeats provider discovery.
    var highlighted = 0
    for (node_index in range(length(doc.nodes))) {
        let node = doc.nodes[node_index]
        if (node.kind != RichNodeKind.code_block
                || markdown_view_syntax_bound(state, document_revision, node_index)) continue
        var projected : array<MarkdownSyntaxToken>
        if (highlight_markdown_code(doc, node_index,
                                    syntax_canonical_language(node.language),
                                    projected)) highlighted++
        markdown_view_bind_syntax(
            state, document_revision, node_index, projected)
        delete projected
    }
    return highlighted
}

def public markdown_tree_sitter_prepare(doc : RichDocument const;
                                        var provider : MarkdownTreeSitterState;
                                        var state : MarkdownViewState;
                                        document_revision : int = 0) : int {
    //! Apply viewer-wide and per-block language policy. Existing view bindings
    //! are reused until a block's effective language actually changes.
    retain_markdown_syntax_revision(provider, document_revision)
    var highlighted = 0
    for (node_index in range(length(doc.nodes))) {
        if (doc.nodes[node_index].kind != RichNodeKind.code_block) continue
        let language = markdown_tree_sitter_effective_language(
            doc, provider, document_revision, node_index)
        let old_index = applied_index(provider, document_revision, node_index)
        if (old_index >= 0 && provider.applied[old_index].language == language
                && markdown_view_syntax_bound(
                    state, document_revision, node_index)) continue

        let _ = markdown_view_unbind_syntax(
            state, document_revision, node_index)
        var projected : array<MarkdownSyntaxToken>
        if (highlight_markdown_code(
                doc, node_index, language, projected)) highlighted++
        markdown_view_bind_syntax(
            state, document_revision, node_index, projected)
        delete projected

        if (old_index >= 0) {
            provider.applied[old_index].language := language
        } else {
            provider.applied |> push(MarkdownTreeSitterApplied(
                document_revision = document_revision,
                node_index = node_index,
                language = language))
        }
    }
    return highlighted
}

def public markdown_view_tree_sitter(id : string; doc : RichDocument const;
                                     var state : MarkdownViewState;
                                     var style : MarkdownViewStyle;
                                     document_revision : int = 0) : MarkdownViewResult {
    markdown_tree_sitter_prepare(doc, state, document_revision)
    return <- markdown_view(id, doc, state, style, document_revision)
}

def public markdown_view_tree_sitter(id : string; doc : RichDocument const;
                                     var provider : MarkdownTreeSitterState;
                                     var state : MarkdownViewState;
                                     var style : MarkdownViewStyle;
                                     document_revision : int = 0) : MarkdownViewResult {
    markdown_tree_sitter_prepare(doc, provider, state, document_revision)
    return <- markdown_view(id, doc, state, style, document_revision)
}
