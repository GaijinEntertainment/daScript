module json

struct JsonValue {
    value : variant<
        _object : table<string; JsonValue?>;
        _array  : array<JsonValue?>;
        _string : string;
        _number : double;
        _bool   : bool;
        _null   : void?>;
}

variant JsValue
    _object : table<string; JsonValue?>
    _array  : array<JsonValue?>
    _string : string
    _number : double
    _bool   : bool
    _null   : void?

variant Token
    _string : string
    _number : double
    _bool   : bool
    _null   : void?
    _symbol : int
    _error  : string

let
    Token_string = typeinfo(variant_index<_string> type<Token>)
    Token_symbol = typeinfo(variant_index<_symbol> type<Token>)

[unsafe, private]
def lexer ( text : string )
    var stext = "{text}"
    return <- generator<Token>() <| $()
        var tin <- each(stext)
        var ahead : int = ' '
        while !empty(tin)
            while is_white_space(ahead) & next(tin,ahead)
                pass
            if empty(tin)
                return false
            if ahead=='[' | ahead==']' | ahead=='{' | ahead=='}' | ahead==':' | ahead==','
                yield [[Token _symbol=ahead]]
                next(tin, ahead)
            elif ahead=='"'
                var str : string
                while next(tin,ahead) & ahead!='"'
                    str = append(str, ahead)
                if empty(tin)
                    yield [[Token _error = "string exceeds text"]]
                    return false
                yield [[Token _string = unescape(str)]]
                next(tin, ahead)
            elif ahead=='+' | ahead=='-' | is_number(ahead)
                var num : string
                num = append(num, ahead)
                while next(tin,ahead) & is_number(ahead)
                    num = append(num, ahead)
                if !empty(tin) & ahead=='.'
                    num = append(num,ahead)
                    while next(tin,ahead) & is_number(ahead)
                        num = append(num, ahead)
                yield [[Token _number = double(num)]]
            elif is_alpha(ahead)
                var name : string
                name = append(name, ahead)
                while next(tin,ahead) & is_alpha(ahead)
                    name = append(name, ahead)
                if name == "true"
                    yield [[Token _bool=true]]
                elif name == "false"
                    yield [[Token _bool=false]]
                elif name == "null"
                    yield [[Token _null = null]]
                else
                    yield [[Token _error = "invalid name {name}"]]
                    return false
            else
                yield [[Token _error = "invalid character `{to_char(ahead)}` aka ASCII {ahead}"]]
                return false
        return false

[private]
def expect_token ( var itv : iterator<Token>; var ahead : Token; vindex : int; var error : string & ) : bool
    if !next(itv, ahead)
        error = "unexected eos"
        return false
    elif variant_index(ahead) != vindex
        error = "unexpected {ahead}, expecting variant {vindex}"
        return false
    else
        return true

[private]
def expect_symbol ( var itv : iterator<Token>; var ahead : Token; sym : int; var error : string & ) : bool
    if !next(itv, ahead)
        error = "unexected eos"
        return false
    elif ! ahead is _symbol
        error = "unexpected {ahead}, expecting symbol"
        return false
    elif !(ahead as _symbol == sym)
        error = "unexpected {ahead}, expecting symbol `{to_char(sym)}` aka ASCII {sym}"
        return true
    else
        return true

[private]
def parse_value ( var itv : iterator<Token>; var error : string & ) : JsonValue?
    var ahead : Token
    if !next(itv, ahead)
        return null
    if ahead is _symbol
        let sym = ahead as _symbol
        if sym == '['
            var arr : array<JsonValue?>
            while !empty(itv)
                var value = parse_value(itv, error)
                if value == null
                    return null
                push(arr, value)
                if !expect_token(itv, ahead, Token_symbol, error)
                    return null
                let sepsym = ahead as _symbol
                if sepsym == ']'
                    break
                elif sepsym != ','
                    error = "unsepected array seaprator symbol `{to_char(sepsym)}` aka ASCII {sepsym}"
                    return null
            if empty(itv)
                error = "unexpected eos"
                return null
            return  new [[JsonValue value <- [[JsValue _array <- arr]]]]
        elif sym == '{'
            var tab : table<string; JsonValue?>
            while !empty(itv)
                if !expect_token(itv, ahead, Token_string, error)
                    return null
                var key = ahead as _string
                if !expect_symbol(itv, ahead, ':', error)
                    return null
                var value = parse_value(itv, error)
                if value == null
                    return null
                if key_exists(tab,key)
                    error = "duplicate key {key}"
                    return null
                tab[key] = value
                if !expect_token(itv, ahead, Token_symbol, error)
                    return null
                let sepsym = ahead as _symbol
                if sepsym == '}'
                    break
                elif sepsym != ','
                    error = "unsepected object seaprator symbol `{to_char(sepsym)}` aka ASCII {sepsym}"
                    return null
            if empty(itv)
                error = "unexpected eos"
                return null
            return new [[JsonValue value <- [[JsValue _object <- tab]]]]
        else
            error = "unexpected symbol `{to_char(sym)}` aka ASCII {sym}"
            return null
    elif ahead is _string
        return new [[JsonValue value <- [[JsValue _string = ahead as _string]]]]
    elif ahead is _number
        return new [[JsonValue value <- [[JsValue _number = ahead as _number]]]]
    elif ahead is _bool
        return new [[JsonValue value <- [[JsValue _bool = ahead as _bool]]]]
    elif ahead is _null
        return new [[JsonValue value <- [[JsValue _null = null]]]]
    else
        panic("internal parsing error near {ahead}")
        return null

def read_json ( text : string; var error : string& ) : JsonValue?
    return parse_value(lexer(text),error)

[private] // note - we use StringBuilderWriter for performance reasons here
def write_value ( var writer : StringBuilderWriter; jsv : JsonValue?; depth : int )
    if jsv == null
        write(writer, "null")
    elif jsv.value is _string
        write(writer, "\"")
        write(writer, escape(jsv.value as _string))
        write(writer, "\"")
    elif jsv.value is _number
        write(writer, jsv.value as _number)
    elif jsv.value is _array
        write(writer, "[\n")
        var first = true
        for elem in jsv.value as _array
            if first
                first = false
            else
                write(writer,",\n")
            write_chars(writer,'\t',depth+1)
            write_value(writer, elem, depth+1)
        write(writer, "\n")
        write_chars(writer,'\t',depth)
        write(writer, "]")
    elif jsv.value is _object
        write(writer, "\{\n")
        var first = true
        for elemK, elemV in keys(jsv.value as _object), values(jsv.value as _object)
            if first
                first = false
            else
                write(writer,",\n")
            write_chars(writer,'\t',depth+1)
            write(writer, "\"")
            write(writer, escape(elemK))
            write(writer, "\" : ")
            write_value(writer, elemV, depth+1)
        write(writer, "\n")
        write_chars(writer,'\t',depth)
        write(writer, "\}")
    elif jsv.value is _bool
        if jsv.value as _bool
            write(writer, "true")
        else
            write(writer, "false")
    elif jsv.value is _null
        write(writer, "null")
    else
        panic("unexpected {jsv}")

def write_json ( val : JsonValue? ) : string
    let st = build_string() <| $ (var writer)
        write_value(writer, val, 0)
    return st
