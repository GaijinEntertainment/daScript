options gen2

require common public
require daslib/das_source_formatter
require daslib/json_boost

struct FormatResult {
    file : string
    status : string  // "formatted" | "already_formatted" | "reverted" | "error"
    message : string
}

def private write_text(file : string; text : string) : bool {
    var ok = false
    fopen(file, "wb") $(fw) {
        if (fw == null) {
            return
        }
        ok = true
        fw |> fprint(text)
    }
    return ok
}

def private format_file_single(file : string; keep_comments : bool; project_root : string; load_modules : array<string>) : FormatResult {
    var result = FormatResult(file = file)
    var unformatted = ""
    var formatted = ""
    var stripped_any = false
    var read_ok = false
    fopen(file, "rb") $(fr) {
        if (fr == null) {
            return
        }
        read_ok = true
        fmap(fr) $(data) {
            unformatted = string(data)
            formatted = format_source(data, !keep_comments)
            if (!keep_comments) {
                stripped_any = formatted != format_source(data)
            }
        }
    }
    if (!read_ok) {
        result.status = "error"
        result.message = "Cannot open file: {file}"
        return result
    }
    if (empty(formatted)) {
        result.status = "error"
        result.message = "Formatting produced empty output for: {file}"
        return result
    }
    if (formatted == unformatted) {
        result.status = "already_formatted"
        result.message = "File already formatted: {file}"
        return result
    }
    if (!write_text(file, formatted)) {
        result.status = "error"
        result.message = "Cannot write file: {file}"
        return result
    }
    if (stripped_any) {
        let verify = run_mcp_subtool("compile_check", [file, "", "true"], project_root, load_modules)
        var envelope : ToolResult
        if (!sscan_json(verify, envelope) || envelope.isError) {
            write_text(file, unformatted)
            result.status = "reverted"
            result.message = "comment strip broke compile_check; original restored: {file}"
            return result
        }
    }
    result.status = "formatted"
    result.message = stripped_any ? "Formatted (comments stripped): {file}" : "Formatted: {file}"
    return result
}

def do_format_file(file : string; keep_comments : bool = false; project_root : string = ""; load_modules : array<string> = array<string>()) : string {
    var files : array<string>
    parse_file_list(file, files)
    if (empty(files)) {
        let err = [FormatResult(file = file, status = "error", message = "no files matched: {file}")]
        return make_tool_result(sprint_json(err, false), true)
    }
    var results : array<FormatResult>
    results |> reserve(length(files))
    var any_errors = false
    for (f in files) {
        let r = format_file_single(f, keep_comments, project_root, load_modules)
        if (r.status == "error" || r.status == "reverted") {
            any_errors = true
        }
        results |> push(r)
    }
    return make_tool_result(sprint_json(results, false), any_errors)
}
