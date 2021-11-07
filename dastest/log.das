options indenting = 4

module log shared

require rtti
require uriparser


let verbose = false

def info_raw(msg: string)
    print("{msg}")

def info(msg: string)
    if !verbose
        print("{msg}\n")
    else
        let at = get_line_info(1)
        print("{msg} @{file_name_to_uri(string(at.fileInfo.name))}#{int(at.line)}\n")

def warn(msg: string)
    if !verbose
        print("[W] {msg}\n")
    else
        let at = get_line_info(1)
        print("[W] {msg} @{file_name_to_uri(string(at.fileInfo.name))}#{int(at.line)}\n")

def error(msg: string)
    if !verbose
        print("[E] {msg}\n")
    else
        let at = get_line_info(1)
        print("[E] {msg} @{file_name_to_uri(string(at.fileInfo.name))}#{int(at.line)}\n")