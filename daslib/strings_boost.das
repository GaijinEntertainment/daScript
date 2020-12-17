options indenting = 4
options remove_unused_symbols = false

module strings_boost shared

require strings public

def wide (text:string; width:int)
    let extra = width - length(text)
    if extra > 0
        return text + repeat(" ",extra)
    else
        return text


def join(it; separator:string)
    static_if !typeinfo(is_iterable it)
        concept_assert(false,"can't join non-iterable type")
        return ""
    else
        let st = build_string() <| $ (var writer)
            var skip_first = true
            for elem in it
                if skip_first
                    skip_first = false
                else
                    write(writer, separator)
                if typeinfo(is_string it)
                    write_char(writer, elem)
                else
                    write(writer, elem)
        return st

def split ( text, delim : string; blk : block< (arg:array<string>#) > )
    builtin_string_split ( text, delim, blk )

def split ( text, delim : string ) : array<string>
    var res : array<string>
    builtin_string_split(text, delim) <| $ ( arr : array<string># ) : void
        res := arr
    return <- res

def split_by_chars ( text, delim : string; blk : block< (arg:array<string>#) > )
    builtin_string_split_by_char ( text, delim, blk )

def split_by_chars ( text, delim : string ) : array<string>
    var res : array<string>
    builtin_string_split_by_char (text, delim) <| $ ( arr : array<string># ) : void
        res := arr
    return <- res
