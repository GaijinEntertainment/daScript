options gen2
options persistent_heap
options gc
options indenting = 4

module file_inspection shared public

require strings public
require daslib/strings_boost
require math

//! Raw bytes embedded into a file View response. 8 MiB bounds text JSON and
//! becomes roughly 10.7 MiB for the binary base64 fallback while limiting
//! simultaneous mapped/raw/encoded/decoded allocations across both clients.
let public MAX_FILE_PREVIEW_RAW_BYTES = 8 * 1024 * 1024

enum public GitDiffLineKind {
    context
    removed
    added
    empty
}

enum public InspectorRequestState {
    idle
    queued
    running
}

enum public InspectorContentState {
    absent
    ready
    failed
}

def private strip_local_suffix(path : string) : string {
    let lower = to_lower(path)
    if (ends_with(lower, ".local")) {
        return slice(lower, 0, length(lower) - length(".local"))
    }
    return lower
}

def public inspector_language_for_path(path : string;
                                       requested : string = "auto") : string {
    //! Resolve the syntax producer independently from the renderer. An empty
    //! result deliberately means plain text. `.local` is a Git/worktree
    //! convention in this repository, so `notes.md.local` remains Markdown.
    let choice = to_lower(requested)
    if (!empty(choice) && choice != "auto") {
        if (choice == "plain" || choice == "text") return ""
        if (choice == "das" || choice == "dascript") return "daslang"
        if (choice == "c++") return "cpp"
        if (choice == "md") return "markdown"
        return choice
    }
    let candidate = strip_local_suffix(path)
    if (ends_with(candidate, ".das")) return "daslang"
    if (ends_with(candidate, ".c")) return "c"
    if (ends_with(candidate, ".cc") || ends_with(candidate, ".cpp")
            || ends_with(candidate, ".cxx") || ends_with(candidate, ".h")
            || ends_with(candidate, ".hh") || ends_with(candidate, ".hpp")
            || ends_with(candidate, ".hxx")) return "cpp"
    if (ends_with(candidate, ".md")
            || ends_with(candidate, ".markdown")) return "markdown"
    if (ends_with(candidate, ".json")) return "json"
    return ""
}

// Request lifetime and installed content deliberately do not collapse into a
// single state. A replacement can be running while the previous successful
// payload remains visible, and a failed replacement does not destroy it.
struct public InspectorLoadState {
    generation : int
    request : InspectorRequestState = InspectorRequestState.idle
    latest_outcome : InspectorContentState = InspectorContentState.absent
    has_retained_content : bool
    retained_generation : int
    status : string
    error : string
}

def public inspector_load_queue(var state : InspectorLoadState;
                                status : string = "Queued") : int {
    state.generation++
    state.request = InspectorRequestState.queued
    state.status = status
    state.error = ""
    return state.generation
}

def public inspector_load_start(var state : InspectorLoadState;
                                generation : int;
                                status : string = "Loading") : bool {
    if (generation != state.generation
            || state.request != InspectorRequestState.queued) return false
    state.request = InspectorRequestState.running
    state.status = status
    return true
}

def public inspector_load_succeed(var state : InspectorLoadState;
                                  generation : int;
                                  status : string = "Ready") : bool {
    if (generation != state.generation) return false
    state.request = InspectorRequestState.idle
    state.latest_outcome = InspectorContentState.ready
    state.has_retained_content = true
    state.retained_generation = generation
    state.status = status
    state.error = ""
    return true
}

def public inspector_load_fail(var state : InspectorLoadState;
                               generation : int;
                               error : string) : bool {
    if (generation != state.generation) return false
    state.request = InspectorRequestState.idle
    state.latest_outcome = InspectorContentState.failed
    state.status = "Failed"
    state.error = error
    return true
}

def public inspector_load_reset(var state : InspectorLoadState) {
    state.generation++
    state.request = InspectorRequestState.idle
    state.latest_outcome = InspectorContentState.absent
    state.has_retained_content = false
    state.retained_generation = 0
    state.status = ""
    state.error = ""
}

def public inspector_load_supersede(var state : InspectorLoadState;
                                    status : string = "Refreshing") {
    state.generation++
    state.request = InspectorRequestState.idle
    state.status = status
    state.error = ""
}

struct public GitDiffRow {
    old_text : string
    new_text : string
    old_line : int = -1
    new_line : int = -1
    old_kind : GitDiffLineKind = GitDiffLineKind.empty
    new_kind : GitDiffLineKind = GitDiffLineKind.empty
}

struct public GitDiffHunk {
    row_start : int
    row_count : int
    old_start : int
    new_start : int
}

struct public GitParsedPatch {
    old_text : string
    new_text : string
    old_aligned_text : string
    new_aligned_text : string
    rows : array<GitDiffRow>
    hunks : array<GitDiffHunk>
    old_changed_lines : array<int>
    new_changed_lines : array<int>
    new_deleted_anchors : array<int>
    binary : bool
    error : string
}

def public git_parsed_patch_dispose(var patch : GitParsedPatch) {
    delete patch.rows
    delete patch.hunks
    delete patch.old_changed_lines
    delete patch.new_changed_lines
    delete patch.new_deleted_anchors
    patch = GitParsedPatch()
}

def private trim_cr(value : string) : string {
    if (!empty(value) && ends_with(value, "\r")) {
        return slice(value, 0, length(value) - 1)
    }
    return value
}

def private byte_at(value : string; index : int) : int {
    var result = 0
    peek_data(value) $(data) {
        result = int(data[index])
    }
    return result
}

def private parse_decimal(value : string; var index : int&) : int {
    var result = 0
    let value_length = length(value)
    while (index < value_length && is_number(byte_at(value, index))) {
        result = result * 10 + byte_at(value, index) - '0'
        index++
    }
    return result
}

def private parse_hunk_starts(header : string;
                              var old_line : int&; var new_line : int&) : bool {
    let old_marker = find(header, "-")
    let new_marker = find(header, "+", max(0, old_marker + 1))
    if (old_marker < 0 || new_marker < 0) return false
    var old_at = old_marker + 1
    var new_at = new_marker + 1
    old_line = parse_decimal(header, old_at)
    new_line = parse_decimal(header, new_at)
    return old_line >= 0 && new_line >= 0
}

def private append_change_rows(var result : GitParsedPatch;
                               var removed_text : array<string>;
                               var removed_lines : array<int>;
                               var added_text : array<string>;
                               var added_lines : array<int>) {
    let count = max(length(removed_text), length(added_text))
    result.rows |> reserve(length(result.rows) + count)
    for (index in range(count)) {
        let has_old = index < length(removed_text)
        let has_new = index < length(added_text)
        result.rows |> push(GitDiffRow(
            old_text = has_old ? removed_text[index] : "",
            new_text = has_new ? added_text[index] : "",
            old_line = has_old ? removed_lines[index] : -1,
            new_line = has_new ? added_lines[index] : -1,
            old_kind = has_old ? GitDiffLineKind.removed : GitDiffLineKind.empty,
            new_kind = has_new ? GitDiffLineKind.added : GitDiffLineKind.empty))
    }
    delete removed_text
    delete removed_lines
    delete added_text
    delete added_lines
}

def private hidden_window_lines(patch : GitParsedPatch;
                                windows : array<int2> const;
                                line_count : int) : array<int> {
    // Window lines not already displayed, deduped and sorted.
    var displayed : table<int>
    for (row in patch.rows) {
        if (row.new_line > 0) {
            displayed |> insert(row.new_line)
        }
    }
    var wanted : array<int>
    for (window in windows) {
        let lo = clamp(min(window.x, window.y), 1, line_count)
        let hi = clamp(max(window.x, window.y), 1, line_count)
        for (line in range(lo, hi + 1)) {
            if (!key_exists(displayed, line)) {
                wanted |> push(line)
                displayed |> insert(line)
            }
        }
    }
    sort(wanted)
    delete displayed
    return <- wanted
}

def private synthesized_context_row(source_lines : array<string>;
                                    line, old_offset : int) : GitDiffRow {
    // Outside every hunk the two sides advance in lockstep, so the old line
    // is the new line plus the offset after the last preceding row.
    return GitDiffRow(
        old_text = source_lines[line - 1],
        new_text = source_lines[line - 1],
        old_line = line + old_offset,
        new_line = line,
        old_kind = GitDiffLineKind.context,
        new_kind = GitDiffLineKind.context)
}

def private realign_patch_text(var patch : GitParsedPatch) {
    var inscope old_aligned : array<string>
    var inscope new_aligned : array<string>
    old_aligned |> reserve(length(patch.rows))
    new_aligned |> reserve(length(patch.rows))
    for (row in patch.rows) {
        old_aligned |> push(row.old_text)
        new_aligned |> push(row.new_text)
    }
    patch.old_aligned_text = join(old_aligned, "\n")
    patch.new_aligned_text = join(new_aligned, "\n")
}

def public git_patch_inject_context(var patch : GitParsedPatch;
                                    new_source : string;
                                    windows : array<int2> const) {
    //! Make the aligned Diff DISPLAY extra new-file regions (find reveals
    //! matches in the diff): each window is a 1-based inclusive new-file line
    //! range. Lines already displayed are skipped; the synthesized rows are
    //! plain context (a hidden line is by definition unchanged), hunk row
    //! offsets and both aligned texts are rebuilt. Hidden runs never split a
    //! hunk - new-file lines inside a hunk are contiguous by construction.
    return if (patch.binary || !empty(patch.error) || empty(windows))
    var inscope source_lines <- split(new_source, "\n")
    var line_count = length(source_lines)
    if (line_count > 0 && empty(source_lines[line_count - 1])) {
        line_count--
    }
    return if (line_count == 0)
    var inscope wanted <- hidden_window_lines(patch, windows, line_count)
    return if (empty(wanted))

    // A hidden line must insert BEFORE the whole row group it precedes, even
    // when that group opens with removed-only rows (new_line -1): flush
    // against the NEXT new-file line at or after each row.
    let row_count = length(patch.rows)
    var inscope next_new_at : array<int>
    next_new_at |> resize(row_count)
    var upcoming = int(0x7fffffff)
    var walk_back = row_count
    while (walk_back > 0) {
        walk_back--
        if (patch.rows[walk_back].new_line > 0) {
            upcoming = patch.rows[walk_back].new_line
        }
        next_new_at[walk_back] = upcoming
    }

    var inscope rebuilt : array<GitDiffRow>
    rebuilt |> reserve(row_count + length(wanted))
    var inscope new_index_of : array<int>
    new_index_of |> resize(row_count)
    var run_at = 0
    var old_cursor = 0
    var new_cursor = 0
    for (row_index in range(row_count)) {
        while (run_at < length(wanted) && wanted[run_at] < next_new_at[row_index]) {
            rebuilt |> push(synthesized_context_row(source_lines,
                wanted[run_at], old_cursor - new_cursor))
            run_at++
        }
        new_index_of[row_index] = length(rebuilt)
        rebuilt |> push(patch.rows[row_index])
        if (patch.rows[row_index].old_line > 0) {
            old_cursor = patch.rows[row_index].old_line
        }
        if (patch.rows[row_index].new_line > 0) {
            new_cursor = patch.rows[row_index].new_line
        }
    }
    while (run_at < length(wanted)) {
        rebuilt |> push(synthesized_context_row(source_lines,
            wanted[run_at], old_cursor - new_cursor))
        run_at++
    }
    delete patch.rows
    patch.rows <- rebuilt
    for (hunk_index in range(length(patch.hunks))) {
        patch.hunks[hunk_index].row_start = new_index_of[patch.hunks[hunk_index].row_start]
    }
    realign_patch_text(patch)
}

def private git_patch_line_is_binary(line : string) : bool {
    return (line == "GIT binary patch"
        || starts_with(line, "Binary files ") && ends_with(line, " differ"))
}

def public git_patch_is_binary(patch : string) : bool {
    var inscope lines <- split(patch, "\n")
    for (raw_line in lines) {
        if (git_patch_line_is_binary(trim_cr(raw_line))) return true
    }
    return false
}

def public git_patch_result_is_deleted(patch : string) : bool {
    for (raw_line in split(patch, "\n")) {
        let line = trim_cr(raw_line)
        if (line == "+++ /dev/null" || starts_with(line, "deleted file mode ")
                || (starts_with(line, "Binary files ")
                    && find(line, " and /dev/null differ") >= 0)) return true
    }
    return false
}

def public git_parse_unified_patch(patch : string) : GitParsedPatch { // nolint:STYLE038
    var result = GitParsedPatch()
    var inscope old_lines : array<string>
    var inscope new_lines : array<string>
    var inscope old_aligned : array<string>
    var inscope new_aligned : array<string>
    var inscope removed_text : array<string>
    var inscope removed_lines : array<int>
    var inscope added_text : array<string>
    var inscope added_lines : array<int>
    var inscope lines <- split(patch, "\n")
    var in_hunk = false
    var saw_diff_header = false
    var old_line = 0
    var new_line = 0
    for (raw_line in lines) {
        let line = trim_cr(raw_line)
        if (starts_with(line, "diff --git ")) {
            saw_diff_header = true
        }
        if (git_patch_line_is_binary(line)) {
            result.binary = true
        }
        if (starts_with(line, "@@")) {
            append_change_rows(result, removed_text, removed_lines,
                added_text, added_lines)
            if (!empty(result.hunks)) {
                let previous = length(result.hunks) - 1
                result.hunks[previous].row_count = length(result.rows) - result.hunks[previous].row_start
            }
            if (!parse_hunk_starts(line, old_line, new_line)) {
                result.error = "invalid unified diff hunk header"
                return <- result
            }
            result.hunks |> push(GitDiffHunk(
                row_start = length(result.rows), old_start = old_line,
                new_start = new_line))
            in_hunk = true
            continue
        }
        if (!in_hunk || empty(line) || starts_with(line, "\\ No newline")) continue
        let prefix = byte_at(line, 0)
        let text = slice(line, 1)
        if (prefix == ' ') {
            append_change_rows(result, removed_text, removed_lines,
                added_text, added_lines)
            result.rows |> push(GitDiffRow(
                old_text = text, new_text = text,
                old_line = old_line, new_line = new_line,
                old_kind = GitDiffLineKind.context,
                new_kind = GitDiffLineKind.context))
            old_lines |> push(text)
            new_lines |> push(text)
            old_line++
            new_line++
        } elif (prefix == '-') {
            removed_text |> push(text)
            removed_lines |> push(old_line)
            old_lines |> push(text)
            result.old_changed_lines |> push(old_line - 1)
            old_line++
        } elif (prefix == '+') {
            added_text |> push(text)
            added_lines |> push(new_line)
            new_lines |> push(text)
            result.new_changed_lines |> push(new_line - 1)
            new_line++
        }
    }
    append_change_rows(result, removed_text, removed_lines,
        added_text, added_lines)
    if (!empty(result.hunks)) {
        let last = length(result.hunks) - 1
        result.hunks[last].row_count = length(result.rows) - result.hunks[last].row_start
    }
    if (!in_hunk && !result.binary && !saw_diff_header) {
        result.error = "diff contains no text hunks"
        return <- result
    }
    old_aligned |> reserve(length(result.rows))
    new_aligned |> reserve(length(result.rows))
    var hunk_index = 0
    var new_cursor = 0
    for (row_index in range(length(result.rows))) {
        if (hunk_index < length(result.hunks)
                && result.hunks[hunk_index].row_start == row_index) {
            new_cursor = max(0, result.hunks[hunk_index].new_start - 1)
            hunk_index++
        }
        let row = result.rows[row_index]
        old_aligned |> push(row.old_text)
        new_aligned |> push(row.new_text)
        if (row.old_kind == GitDiffLineKind.removed
                && row.new_kind == GitDiffLineKind.empty) {
            result.new_deleted_anchors |> push(new_cursor)
        }
        if (row.new_line > 0) {
            new_cursor = row.new_line
        }
    }
    result.old_text = join(old_lines, "\n")
    result.new_text = join(new_lines, "\n")
    result.old_aligned_text = join(old_aligned, "\n")
    result.new_aligned_text = join(new_aligned, "\n")
    return <- result
}
