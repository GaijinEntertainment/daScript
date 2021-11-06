options indenting = 4

module log shared

require rtti
require uriparser


let verbose = false

[unused_argument(at)]
def info_raw(msg: string; at: rtti::LineInfo)
    print("{msg}")

def info(msg: string; at: rtti::LineInfo)
    if !verbose
        print("{msg}\n")
    else
        print("{msg} @{file_name_to_uri(string(at.fileInfo.name))}:{at.line}\n")

def warn(msg: string; at: rtti::LineInfo)
    if !verbose
        print("[W] {msg}\n")
    else
        print("[W] {msg} @{file_name_to_uri(string(at.fileInfo.name))}:{at.line}\n")

def error(msg: string; at: rtti::LineInfo)
    if !verbose
        print("[E] {msg}\n")
    else
        print("[E] {msg} @{file_name_to_uri(string(at.fileInfo.name))}:{at.line}\n")