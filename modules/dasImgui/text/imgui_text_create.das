options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_text_create shared

require imgui/imgui_text_source_edit public
require imgui/imgui_commands public
require math
require strings
require daslib/strings_boost

//! The creation registry: a user-extensible catalog of "creatables" — things
//! an editor can create (a markdown link, a das if/else) as wrap / insert
//! snippets or line-prefix transforms. Applications register commands from it
//! (`create.<id>`), user .das code overrides or extends it at init time via
//! `creation_define` (LAST wins — unlike `command_define`'s first-wins).

enum public CreationShape {
    insert // snippet at the caret; may splice $\{SEL}
    wrap // snippet whose $\{SEL} keeps the selection selected after
    transform // line-prefix edit on the touched line block
}

enum public CreationTransform {
    none
    heading_cycle
    list_toggle
    ordered_list
    checkbox_toggle
    quote_toggle
}

struct public CreationContext {
    //! Cheap line-local facts computed by `creation_context`; hosts add
    //! richer facts (e.g. RichDocument-derived) as opaque `tags`.
    format : string // canonical language key ("markdown", "daslang", ...)
    line : int
    column : int
    has_selection : bool = false
    multi_line_selection : bool = false
    blank_line : bool = false
    at_statement_start : bool = false
    in_comment : bool = false
    in_string : bool = false
    in_code_fence : bool = false
    list_prefix : string
    heading_level : int = 0
    tags : table<string>
}

struct public CreatableDefinition {
    id : string // "md.bold", "das.if" -> command "create.<id>"
    title : string
    format : string // canonical language key; "" = any buffer
    shape : CreationShape = CreationShape.insert
    body : string // snippet template ($\{n:label} / $\{0} / $\{SEL})
    transform : CreationTransform = CreationTransform.none
    key : ImGuiKey = ImGuiKey.None // default chord; None = chordless
    key_primary : bool = false
    key_shift : bool = false
    key_alt : bool = false
    tags : array<string> // declarative context requirements
    @safe_when_uninitialized can_create : lambda<(ctx : CreationContext const) : bool>
}

//! The registry itself. Mutate ONLY through `creation_define` (which finalizes
//! a replaced entry's lambda); reads go through `creation_get`/`creation_ids`.
var public creation_registry : table<string; CreatableDefinition>

var private g_builtins_registered = false

def public creation_define(var definition : CreatableDefinition) {
    //! Register (or override) one creatable. LAST wins by design, so plain
    //! application .das code can replace built-ins; built-ins register through
    //! this same path before any user call (no init-order trap).
    ensure_builtins()
    // finalize a replaced entry - its can_create lambda must not leak
    creation_registry |> get(definition.id) $(var old_entry) {
        unsafe {
            delete old_entry
        }
    }
    creation_registry |> emplace(definition.id, definition)
}

def public creation_get(id : string;
                        blk : block<(definition : CreatableDefinition const) : void>) : bool {
    //! Read one creatable by id. The definition is passed by reference —
    //! read it, don't store it.
    ensure_builtins()
    var found = false
    creation_registry |> get(id) $(entry) {
        found = true
        blk |> invoke(entry)
    }
    return found
}

def public creation_ids(format : string) : array<string> {
    //! Ids matching `format` ("" entries match any buffer), sorted for a
    //! stable menu order. The caller owns the returned array.
    ensure_builtins()
    var ids : array<string>
    var inscope names : array<string>
    for (name in keys(creation_registry)) {
        names |> push(name)
    }
    for (name in names) {
        creation_get(name) $(definition) {
            if (empty(definition.format) || definition.format == format) {
                ids |> push(name)
            }
        }
    }
    sort(ids)
    return <- ids
}

// ===== Context =====

def private fence_open_at(ed : TextEditState const; upto_line : int) : bool {
    // markdown fence parity scan: are we inside ``` / ~~~ at upto_line?
    var open_ = false
    for (row in range(min(upto_line, length(ed.lines)))) {
        let rest = slice(ed.lines[row], length(leading_whitespace(ed.lines[row])),
            length(ed.lines[row]))
        if (starts_with(rest, "```") || starts_with(rest, "~~~")) {
            open_ = !open_
        }
    }
    return open_
}

def private line_list_prefix(rest : string) : string {
    for (marker in ["- ", "* ", "+ "]) {
        if (starts_with(rest, marker)) return marker
    }
    var digits = 0
    peek_data(rest) $(bytes) {
        while (digits < length(bytes) && is_number(int(bytes[digits]))) {
            digits++
        }
    }
    if (digits > 0 && starts_with(slice(rest, digits, length(rest)), ". ")) {
        return slice(rest, 0, digits + 2)
    }
    return ""
}

def private line_heading_level(rest : string) : int {
    var level = 0
    peek_data(rest) $(bytes) {
        while (level < length(bytes) && int(bytes[level]) == '#') {
            level++
        }
    }
    return 0 if (level == 0 || level > 6)
    let after = slice(rest, level, length(rest))
    return (empty(after) || starts_with(after, " ")) ? level : 0
}

def private caret_run_style(ed : TextEditState const) : TextSourceSyntaxStyle {
    return TextSourceSyntaxStyle.none if (ed.cursor_line >= length(ed.line_styles))
    for (run in ed.line_styles[ed.cursor_line].runs) {
        if (ed.cursor_column >= run.start_byte && ed.cursor_column < run.end_byte) {
            return run.style
        }
    }
    return TextSourceSyntaxStyle.none
}

def public creation_context(ed : TextEditState const) : CreationContext {
    //! Snapshot the cheap line-local facts at the caret. Hosts append their
    //! own `tags` before gating/executing.
    var ctx = CreationContext(format = ed.language,
        line = ed.cursor_line, column = ed.cursor_column,
        has_selection = text_edit_has_selection(ed))
    if (ctx.has_selection) {
        let sel = text_edit_selection(ed)
        ctx.multi_line_selection = sel.end_line > sel.start_line
    }
    let line = ed.cursor_line < length(ed.lines) ? ed.lines[ed.cursor_line] : ""
    let indent = leading_whitespace(line)
    let rest = slice(line, length(indent), length(line))
    ctx.blank_line = empty(rest)
    ctx.at_statement_start = ed.cursor_column <= length(indent)
    let style = caret_run_style(ed)
    ctx.in_comment = style == TextSourceSyntaxStyle.comment
    ctx.in_string = style == TextSourceSyntaxStyle.string_
    if (ed.language == "markdown") {
        ctx.in_code_fence = fence_open_at(ed, ed.cursor_line)
        ctx.list_prefix = line_list_prefix(rest)
        ctx.heading_level = line_heading_level(rest)
    }
    return <- ctx
}

def private tag_holds(tag : string; ctx : CreationContext const) : bool {
    if (tag == "needs-selection") return ctx.has_selection
    if (tag == "not-in-code-fence") return !ctx.in_code_fence
    if (tag == "not-in-comment") return !ctx.in_comment
    if (tag == "not-in-string") return !ctx.in_string
    if (tag == "blank-line") return ctx.blank_line
    if (tag == "statement-start") {
        return ctx.at_statement_start || ctx.has_selection
    }
    return key_exists(ctx.tags, tag) // host-supplied facts
}

def public creation_allowed(definition : CreatableDefinition const;
                            ctx : CreationContext const) : bool {
    //! Format match, then every declarative tag, then the predicate lambda.
    return false if (!empty(definition.format) && definition.format != ctx.format)
    for (tag in definition.tags) {
        return false if (!tag_holds(tag, ctx))
    }
    if (definition.can_create != null) {
        return invoke(definition.can_create, ctx)
    }
    return true
}

def public creation_available(format : string;
                              ctx : CreationContext const) : array<string> {
    //! Ids creatable HERE — `creation_ids` filtered by `creation_allowed`.
    var ids : array<string>
    var inscope all_ids <- creation_ids(format)
    for (id in all_ids) {
        creation_get(id) $(definition) {
            if (creation_allowed(definition, ctx)) {
                ids |> push(id)
            }
        }
    }
    return <- ids
}

// ===== Transforms =====

def private creation_line_block(ed : TextEditState const) : int2 {
    return int2(ed.cursor_line, ed.cursor_line) if (!text_edit_has_selection(ed))
    let sel = text_edit_selection(ed)
    var last = sel.end_line
    if (last > sel.start_line && sel.end_column == 0) {
        last--
    }
    return int2(sel.start_line, last)
}

def private strip_line_prefixes(rest : string) : string {
    // markers stack: checkbox implies list; strip the deepest known form
    for (marker in ["- [ ] ", "- [x] ", "- [X] "]) {
        if (starts_with(rest, marker)) {
            return slice(rest, length(marker), length(rest))
        }
    }
    let list_prefix = line_list_prefix(rest)
    if (!empty(list_prefix)) {
        return slice(rest, length(list_prefix), length(rest))
    }
    return rest
}

def private transform_heading(rest : string; target : int) : string {
    let level = line_heading_level(rest)
    var body = rest
    if (level > 0) {
        body = slice(rest, level, length(rest))
        if (starts_with(body, " ")) {
            body = slice(body, 1, length(body))
        }
    }
    return body if (target == 0)
    return repeat("#", target) + " " + body
}

def private transform_line(kind : CreationTransform; rest : string;
                           target, ordinal : int) : string {
    if (kind == CreationTransform.heading_cycle) {
        return transform_heading(rest, target)
    }
    if (kind == CreationTransform.quote_toggle) {
        if (target != 0) {
            return "> " + rest
        }
        return starts_with(rest, "> ") ? slice(rest, 2, length(rest)) : rest
    }
    let body = strip_line_prefixes(rest)
    if (kind == CreationTransform.list_toggle) {
        return target != 0 ? "- " + body : body
    }
    if (kind == CreationTransform.ordered_list) {
        return target != 0 ? "{ordinal}. " + body : body
    }
    if (kind == CreationTransform.checkbox_toggle) {
        return target != 0 ? "- [ ] " + body : "- [x] " + body
    }
    return rest
}

def private transform_target(kind : CreationTransform; rest : string) : int {
    // the FIRST line's current state decides the uniform direction
    if (kind == CreationTransform.heading_cycle) {
        return (line_heading_level(rest) + 1) % 7
    }
    if (kind == CreationTransform.quote_toggle) {
        return starts_with(rest, "> ") ? 0 : 1
    }
    if (kind == CreationTransform.checkbox_toggle) {
        // unchecked -> checked; checked (or boxless) -> unchecked box
        return starts_with(rest, "- [ ] ") ? 0 : 1
    }
    if (kind == CreationTransform.list_toggle) {
        // dash-family strips; anything else (incl. ordered) becomes dashes
        for (marker in ["- ", "* ", "+ "]) {
            if (starts_with(rest, marker)) return 0
        }
        return 1
    }
    if (kind == CreationTransform.ordered_list) {
        let list_prefix = line_list_prefix(rest)
        return (!empty(list_prefix) && is_number(first_character(list_prefix))) ? 0 : 1
    }
    return 0
}

def private apply_transform(var ed : TextEditState;
                            kind : CreationTransform) : bool {
    return false if (empty(ed.lines))
    let block_ = creation_line_block(ed)
    let had_selection = text_edit_has_selection(ed)
    let caret_line = ed.cursor_line
    let caret_column = ed.cursor_column
    let first_rest_start = length(leading_whitespace(ed.lines[block_.x]))
    let target = transform_target(kind,
        slice(ed.lines[block_.x], first_rest_start, length(ed.lines[block_.x])))
    var caret_delta = 0
    var changed = false
    var new_lines : array<string>
    new_lines |> reserve(block_.y - block_.x + 1)
    var ordinal = 1
    for (row in range(block_.x, block_.y + 1)) {
        let indent = leading_whitespace(ed.lines[row])
        let rest = slice(ed.lines[row], length(indent), length(ed.lines[row]))
        let out_ = indent + transform_line(kind, rest, target, ordinal)
        if (row == caret_line) {
            caret_delta = length(out_) - length(ed.lines[row])
        }
        changed = changed || out_ != ed.lines[row]
        new_lines |> push(out_)
        ordinal++
    }
    if (!changed) {
        delete new_lines
        return false
    }
    let span = TextEditRange(start_line = block_.x, start_column = 0,
        end_line = block_.y, end_column = length(ed.lines[block_.y]))
    text_edit_replace_range(ed, span, join(new_lines, "\n"))
    delete new_lines
    if (had_selection) {
        text_edit_select(ed, block_.x, 0, block_.y, length(ed.lines[block_.y]))
    } else {
        text_edit_set_cursor(ed, caret_line,
            max(caret_column + caret_delta, 0))
    }
    text_edit_patch_redo_cursor(ed)
    text_edit_push_undo_stop(ed)
    return true
}

// ===== Execution + command registration =====

def public creation_execute(var ed : TextEditState; id : string;
                            ctx : CreationContext const) : bool {
    //! Gate against `ctx`, then run: snippets expand through the editor-core
    //! engine (one undo unit, session armed), transforms rewrite the touched
    //! line block (one undo unit). False = declined or unknown id.
    var executed = false
    creation_get(id) $(definition) {
        return if (!creation_allowed(definition, ctx))
        if (definition.shape == CreationShape.transform) {
            executed = apply_transform(ed, definition.transform)
        } else {
            executed = text_edit_snippet_expand(ed, definition.body)
        }
    }
    return executed
}

def public creation_execute(var ed : TextEditState; id : string) : bool {
    //! Convenience form: computes the context itself (no host tags).
    var inscope ctx <- creation_context(ed)
    return creation_execute(ed, id, ctx)
}

def public creation_register_commands(var registry : CommandRegistry) {
    //! Define `create.<id>` commands (component scope) with default chords.
    //! `command_define` is first-wins, so a re-run only adds NEW ids; call
    //! before `command_load_bindings` so persisted chords overlay defaults.
    ensure_builtins()
    var inscope names : array<string>
    for (name in keys(creation_registry)) {
        names |> push(name)
    }
    for (name in names) {
        creation_get(name) $(definition) {
            let command_id = "create." + definition.id
            if (definition.key != ImGuiKey.None) {
                command_define(registry, CommandDefinition(id = command_id,
                    title = definition.title, category = "Create",
                    scope = CommandScope.component),
                    command_binding(command_id, definition.key,
                        definition.key_primary, definition.key_shift,
                        definition.key_alt, false, CommandScope.component))
            } else {
                command_define(registry, CommandDefinition(id = command_id,
                    title = definition.title, category = "Create",
                    scope = CommandScope.component))
            }
        }
    }
}

// ===== Built-in sets =====

def private define_builtin(id, title, format : string; shape : CreationShape;
                           body : string;
                           key : ImGuiKey = ImGuiKey.None;
                           key_primary : bool = false;
                           key_shift : bool = false) {
    var definition = CreatableDefinition(id = id, title = title, format = format,
        shape = shape, body = body, key = key,
        key_primary = key_primary, key_shift = key_shift)
    if (format == "markdown") {
        definition.tags |> push("not-in-code-fence")
    }
    creation_define(definition)
}

def private define_transform(id, title, format : string;
                             kind : CreationTransform;
                             key : ImGuiKey = ImGuiKey.None;
                             key_primary : bool = false;
                             key_shift : bool = false) {
    var definition = CreatableDefinition(id = id, title = title, format = format,
        shape = CreationShape.transform, transform = kind, key = key,
        key_primary = key_primary, key_shift = key_shift)
    if (format == "markdown") {
        definition.tags |> push("not-in-code-fence")
    }
    creation_define(definition)
}

def private register_markdown_builtins() {
    define_builtin("md.bold", "Bold", "markdown",
        CreationShape.wrap, "**$\{SEL}**", ImGuiKey.B, key_primary = true)
    define_builtin("md.italic", "Italic", "markdown",
        CreationShape.wrap, "*$\{SEL}*", ImGuiKey.I, key_primary = true)
    define_builtin("md.code", "Inline code", "markdown",
        CreationShape.wrap, "`$\{SEL}`")
    define_builtin("md.link", "Link", "markdown",
        CreationShape.insert, "[$\{SEL}]($\{1:url})", ImGuiKey.K, key_primary = true)
    define_builtin("md.image", "Image", "markdown",
        CreationShape.insert, "![$\{1:alt}]($\{2:src})")
    define_builtin("md.code_fence", "Code fence", "markdown",
        CreationShape.insert, "```$\{1:lang}\n$\{SEL}$\{0}\n```")
    define_builtin("md.table", "Table", "markdown",
        CreationShape.insert,
        "| $\{1:col} | $\{2:col} |\n| --- | --- |\n| $\{0} |  |")
    define_transform("md.heading", "Heading level", "markdown",
        CreationTransform.heading_cycle, ImGuiKey.H,
        key_primary = true, key_shift = true)
    define_transform("md.list", "List item", "markdown",
        CreationTransform.list_toggle)
    define_transform("md.ordered_list", "Ordered list", "markdown",
        CreationTransform.ordered_list)
    define_transform("md.checkbox", "Checkbox", "markdown",
        CreationTransform.checkbox_toggle)
    define_transform("md.quote", "Quote", "markdown",
        CreationTransform.quote_toggle)
}

def private define_das_statement(id, title, body : string) {
    var definition = CreatableDefinition(id = id, title = title,
        format = "daslang", shape = CreationShape.insert, body = body)
    definition.tags |> push("statement-start")
    creation_define(definition)
}

def private register_daslang_builtins() { // nolint:STYLE038 — flat builtin table, one define per construct
    define_das_statement("das.if", "if",
        "if ($\{1:condition}) \{\n\t$\{0}\n\}")
    define_das_statement("das.if_else", "if / else",
        "if ($\{1:condition}) \{\n\t$\{2}\n\} else \{\n\t$\{0}\n\}")
    define_das_statement("das.if_elif_else", "if / elif / else",
        "if ($\{1:condition}) \{\n\t$\{2}\n\} elif ($\{3:condition}) \{\n\t$\{4}\n\} else \{\n\t$\{0}\n\}")
    define_das_statement("das.for", "for",
        "for ($\{1:item} in $\{2:range(10)}) \{\n\t$\{0}\n\}")
    define_das_statement("das.while", "while",
        "while ($\{1:condition}) \{\n\t$\{0}\n\}")
    define_das_statement("das.def", "def",
        "def $\{1:name}($\{2:args}) \{\n\t$\{0}\n\}")
    define_das_statement("das.struct", "struct",
        "struct $\{1:Name} \{\n\t$\{0}\n\}")
    define_das_statement("das.class", "class",
        "class $\{1:Name} \{\n\t$\{0}\n\}")
    define_das_statement("das.enum", "enum",
        "enum $\{1:Name} \{\n\t$\{0}\n\}")
    define_builtin("das.try", "try / recover", "daslang",
        CreationShape.insert,
        "try \{\n\t$\{SEL}$\{0}\n\} recover \{\n\tpass\n\}")
    define_builtin("das.unsafe", "wrap in unsafe", "daslang",
        CreationShape.wrap, "unsafe \{\n\t$\{SEL}$\{0}\n\}")
    define_builtin("das.block", "wrap in block", "daslang",
        CreationShape.wrap, "\{\n\t$\{SEL}$\{0}\n\}")
}

def private ensure_builtins() {
    return if (g_builtins_registered)
    g_builtins_registered = true // set FIRST: creation_define re-enters here
    register_markdown_builtins()
    register_daslang_builtins()
}
