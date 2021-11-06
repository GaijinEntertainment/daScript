options indenting = 4

require rtti

module log shared

let verbose = false

[unused_argument(at)]
def info_raw(msg: string; at: rtti::LineInfo)
    print("{msg}")

def info(msg: string; at: rtti::LineInfo)
    if !verbose
        print("{msg}\n")
    else
        print("{msg} @{at.fileInfo.name}:{at.line}\n")

def warn(msg: string; at: rtti::LineInfo)
    if !verbose
        print("[W] {msg}\n")
    else
        print("[W] {msg} @{at.fileInfo.name}:{at.line}\n")

def error(msg: string; at: rtti::LineInfo)
    if !verbose
        print("[E] {msg}\n")
    else
        print("[E] {msg} @{at.fileInfo.name}:{at.line}\n")