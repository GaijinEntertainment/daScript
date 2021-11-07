options indenting = 4

module log shared

require rtti
require uriparser


let verbose = false
let uriPaths = true

def info_raw(msg: string)
    print("{msg}")

def info(msg: string)
    if !verbose
        print("{msg}\n")
    else
        let at = get_line_info(1)
        print("{msg} @{file_info_hr(get_line_info(1), uriPaths)}\n")

def warn(msg: string)
    if !verbose
        print("[W] {msg}\n")
    else
        let at = get_line_info(1)
        print("[W] {msg} @{file_info_hr(get_line_info(1), uriPaths)}\n")

def error(msg: string)
    if !verbose
        print("[E] {msg}\n")
    else
        let at = get_line_info(1)
        print("[E] {msg} @{file_info_hr(get_line_info(1), uriPaths)}\n")


def file_info_hr(at: LineInfo; uri_path: bool)
    return (uri_path
        ? "{file_name_to_uri(at.fileInfo != null ? string(at.fileInfo.name) : "")}#{int(at.line)}"
        : "{at.fileInfo != null ? string(at.fileInfo.name) : ""}:{int(at.line)}"
    )
