options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module strings_boost shared public

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

[generic]
def is_character_at(foo:array<uint8>#; idx:int; ch:int)
    return int(foo[idx])==ch

[generic]
def eq ( a:string; b:das_string )
    var res : bool
    peek(b) <| $ ( B )
        res = a==B
    return res

[generic]
def eq ( b:das_string; a:string )
    var res : bool
    peek(b) <| $ ( B )
        res = a==B
    return res

