options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false

module das_source_formatter shared private

require fio
require strings
require math


let sorted_tokens_source <- [[auto "<<<="; ">>>=";
    "<<="; ">>="; "&&="; "||="; "^^="; "<<<"; ">>>";
    "::"; "+="; "-="; "/="; "*="; "%="; "|="; "&="; "^="; "<<"; ">>"; "++"; "--"; "<="; ">="; "==";
    "!="; "->"; "<-"; "??"; "?."; "?["; "<|"; "|>"; ":="; "=>"; "@@"; "&&"; "||"; "^^";
    "%%"
    ]]

let dont_need_space_around <- [[auto "("; ")"; "["; "]"; "\{"; "\}";
    "."; ";"; ","; "`"; "::"; "++"; "--"; "?."; "?["; "@@"; "!"; "~"; "#"; "->"
    ]]

let type_after_keyword <- [[auto "generator"; "cast"; "upcast"; "smart_ptr";
    "type"; "reinterpret"; "variant"; "variant_index"; "function"
    ]]

enum TokenType
    UNKNOWN
    KEYWORD_OR_IDENTIFIER
    NUMBER
    COMMENT
    STRING


struct TokenTemplate
    firstChar: int = 0
    str: string = ""
    codes: array<uint8>
    length: int = 0

struct Token
    str: string
    spaces: int = 0
    newLines: int = 0
    lineInSource: int = 1
    column: int = 1
    tokenType: TokenType = TokenType UNKNOWN
    isInFunctionParam: bool = false
    isInType: bool = false
    dontFormat: bool = false
    dontAddSpacesAround: bool = false


var tokenTemplates: array<TokenTemplate>
var tokens: array<Token>
var pos: int             // current char pos
var c: int               // current char
var data: array<uint8>   // file data as array of uints
var eof: bool
var haveUtf8Bom: bool
var newLineCounter: int  // new lines before current token
var spaceCounter: int    // space characters before current token
var srcLine: int         // current line in source
var curColumn: int       // current column in source
var curLineIndex: int    // index of the begining of new line in data[]
var debugMode: bool = false
var indenting: int = 0   // convert tab to N spaces
var insideOptions: bool = false
var crCount: int = 0     // CR \r
var lfCount: int = 0     // LF \n

def initialize_token_templates()
    if length(tokenTemplates) > 0
        return
    tokenTemplates |> resize(length(sorted_tokens_source))
    for s, tt in sorted_tokens_source, tokenTemplates
        tt.str = s
        tt.codes <- [{for c in s; uint8(c)}]
        tt.firstChar = int(tt.codes[0])
        tt.length = length(s)


def isUtf8Bom(var text: array<uint8>&): bool
    return (length(text) >= 3 &&
        uint(text[0]) == 0xEF && uint(text[1]) == 0xBB && uint(text[2]) == 0xBF)


def eq(index: int; s: string const implicit)
    return (index < 0 || index >= length(tokens)) ? false : tokens[index].str == s


def new_line_before(index: int)
    return (index <= 1 || index >= length(tokens)) ? true : tokens[index].newLines > 0


def next_char
    c = int(data[pos])
    if c == 0
        eof = true
        return

    if c == '\r' && int(data[pos + 1]) == '\n'
        crCount++
        pos++
        c = int(data[pos])

    if c == ' '
        spaceCounter++

    if c == '\t'
        let indent = indenting != 0 ? indenting : 4
        spaceCounter += indent
        curColumn += indent - 1

    if c == '\n'
        lfCount++
        pos++
        c = int(data[pos])
        curLineIndex = pos
        spaceCounter = 0
        newLineCounter++
        srcLine++
        curColumn = 0
        return
    else
        pos++
        c = int(data[pos])
        curColumn++
        return


def skip_spaces
    while (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        next_char()


def on_token_added()
    assume t = tokens[length(tokens) - 1]
    if indenting == 0 && newLineCounter > 0 && spaceCounter > 0 && t.tokenType != TokenType COMMENT
        indenting = spaceCounter
    if new_line_before(length(tokens) - 1)
        insideOptions = false
        if t.str == "options"
            insideOptions = true
    if (insideOptions && t.tokenType == TokenType NUMBER && length(tokens) >= 3 &&
            tokens[length(tokens) - 3].str == "indenting" && length(t.str) == 1)
        indenting = clamp(to_int(t.str), 1, 8)


def push_token(from_char_idx, to_char_idx, space_count, new_lines, src_line, column: int; token_type: TokenType)
    var buf <- [{ for i in range(from_char_idx, to_char_idx); uint8(data[i]) }]
    tokens |> push([[ Token
        str = string(buf), spaces = space_count, newLines = new_lines,
        lineInSource = src_line, column = column, tokenType = token_type
        ]])

    on_token_added()

    spaceCounter = 0
    newLineCounter = 0
finally
    delete buf


def parse_token()
    skip_spaces()
    if eof
        tokens |> push(tokens[0])
        return

    let from = pos
    let fromColumn = curColumn

    // keywords and identifiers
    if is_alpha(c) || c == '_'
        while !eof && (is_alpha(c) || is_number(c) || c == '_')
            next_char()
        push_token(from, pos, spaceCounter, newLineCounter, srcLine, fromColumn, TokenType KEYWORD_OR_IDENTIFIER)
        return

    // numbers
    if is_number(c) || (c == '.' && is_number(int(data[pos + 1])))
        while (!eof && (is_number(c) || is_alpha(c) || c == '.' ||
                ((c == '-' || c == '+') && (int(data[pos - 1]) == 'e' || int(data[pos - 1]) == 'E'))))
            next_char()
        push_token(from, pos, spaceCounter, newLineCounter, srcLine, fromColumn, TokenType NUMBER)
        return

    // characters and strings
    if c == '"' || c == int(0x27) // TODO: fix ' symbol
        let savedLine = newLineCounter
        let savedSpace = spaceCounter
        let openChar = c
        var depth = 0
        next_char()
        while !eof && (c != openChar || depth > 0)
            if c == '{'
                depth++
            if c == '}'
                depth--
            if c == '\\'
                next_char()
            next_char()

        if (c == openChar)
            next_char()

        push_token(from, pos, savedSpace, savedLine, srcLine, fromColumn, TokenType STRING)
        return

    // single line comment
    if c == '/' && int(data[pos + 1]) == '/'
        let savedLine = newLineCounter
        let savedSpace = spaceCounter
        while !eof && c != '\n' && c != '\r'
            next_char()
        push_token(from, pos, savedSpace, savedLine, srcLine, fromColumn, TokenType COMMENT)
        return

    // multi-line comment
    if c == '/' && int(data[pos + 1]) == '*'
        let savedLine = newLineCounter
        let savedSpace = spaceCounter
        next_char()
        next_char()

        while !eof
            if c == '*' && int(data[pos + 1]) == '/'
                break
            next_char()

        next_char()
        next_char()
        push_token(from, pos, savedSpace, savedLine, srcLine, fromColumn, TokenType COMMENT)
        return

    // symbolic tokens
    for tt in tokenTemplates
        if c == tt.firstChar
            var match = true
            for i, code in range(pos, pos + tt.length), tt.codes
                if data[i] != code
                    match = false
                    break
            if match
                for i in range(0, tt.length)
                    next_char()
                push_token(from, pos, spaceCounter, newLineCounter, srcLine, fromColumn, TokenType UNKNOWN)
                return

    // other symbols
    next_char()
    push_token(from, pos, spaceCounter, newLineCounter, srcLine, fromColumn, TokenType UNKNOWN)
    return


def parse_all_tokens()
    initialize_token_templates()
    let emptyToken: Token
    tokens |> clear()
    tokens |> push(emptyToken)
    pos = 0
    eof = false
    haveUtf8Bom = false
    newLineCounter = 0
    spaceCounter = 0
    srcLine = 1
    curColumn = 0
    indenting = 0
    insideOptions = false
    crCount = 0
    lfCount = 0

    if isUtf8Bom(data)
        pos += 3
        haveUtf8Bom = true

    c = int(data[pos])

    curLineIndex = pos
    while !eof
        parse_token()

    for i in range(8)
        tokens |> push(emptyToken)



struct ParenCounter
    angle: int = 0
    paren: int = 0
    square: int = 0
    curly: int = 0


def update_paren(index: int; var paren_counter: ParenCounter&)
    if eq(index, "<")
        paren_counter.angle++
    if eq(index, ">")
        paren_counter.angle--
    if eq(index, ">>")
        paren_counter.angle -= 2
    if eq(index, ">>>")
        paren_counter.angle -= 3
    if eq(index, "(")
        paren_counter.paren++
    if eq(index, ")")
        paren_counter.paren--
    if eq(index, "[")
        paren_counter.square++
    if eq(index, "?[")
        paren_counter.square++
    if eq(index, "]")
        paren_counter.square--
    if eq(index, "\{")
        paren_counter.curly++
    if eq(index, "\}")
        paren_counter.curly--


def find_pair_paren(from_index: int): int
    var pc: ParenCounter
    for j in range(from_index + 1, length(tokens))
        update_paren(j, pc)
        if pc.paren < 0
            return j
    return from_index


def find_pair_square(from_index: int): int
    var pc: ParenCounter
    for j in range(from_index + 1, length(tokens))
        update_paren(j, pc)
        if pc.square < 0
            return j
    return from_index


def mark_tokens_as_type(i: int)
    var pc: ParenCounter
    for j in range(i, length(tokens))
        update_paren(j, pc)
        if pc.angle <= 0 && pc.paren <= 0
            if new_line_before(j) || pc.paren < 0 || pc.square < 0 || eq(j, ":=") || eq(j, "<-") || eq(j, ";") || eq(j, "=")
                break
        if (tokens[j].tokenType != TokenType COMMENT)
            tokens[j].isInType = true


def mark_tokens_as_type_inside_angle(i: int)
    var pc: ParenCounter
    for j in range(i, length(tokens))
        update_paren(j, pc)
        if (tokens[j].tokenType != TokenType COMMENT)
            tokens[j].isInType = true
        if pc.angle <= 0
            break


def search_token_in_line(from_index: int; str: string const): int
    var pc: ParenCounter
    for j in range(from_index, length(tokens))
        update_paren(j, pc)
        if (j > from_index && new_line_before(j) &&
                (pc.paren <= 0 || pc.square <= 0 || pc.curly <= 0) && !eq(j - 1, ","))
            break
        if pc.paren < 0 || pc.square < 0 || pc.curly < 0
            break
        if eq(j, ";") && pc.paren == 0 && pc.square == 0 && pc.curly == 0
            break
        if eq(j, str)
            return j
    return 0


def mark_token_context()
    // don't format  require / expect / label
    for i in range(0, length(tokens) - 1)
        if new_line_before(i) && (eq(i, "require") || eq(i, "expect") || eq(i, "label") || eq(i, "include"))
            for j in range(i + 1, length(tokens))
                if (new_line_before(j))
                    break
                tokens[j].dontFormat = true

    // reader macro
    for i in range(0, length(tokens) - 4)
        if eq(i, "%") && tokens[i + 1].tokenType == TokenType KEYWORD_OR_IDENTIFIER && eq(i + 2, "~")
            for j in range(i, length(tokens))
                tokens[j].dontFormat = true
                if eq(j, "%%")
                    break

    // var / const declaration
    for i in range(0, length(tokens) - 3)
        if new_line_before(i) && (eq(i, "let") || eq(i, "var"))
            let assignPos0 = search_token_in_line(i, "=")
            let assignPos1 = search_token_in_line(i, ":=")
            let assignPos2 = search_token_in_line(i, "<-")
            let colonPos = search_token_in_line(i, ":")
            if (colonPos > 0 &&
                    (assignPos0 > colonPos || assignPos0 <= 0) &&
                    (assignPos1 > colonPos || assignPos1 <= 0) &&
                    (assignPos2 > colonPos || assignPos2 <= 0))
                mark_tokens_as_type(colonPos)

    // type in angle brackets
    for i in range(0, length(tokens) - 3)
        if tokens[i].tokenType == TokenType KEYWORD_OR_IDENTIFIER
            if (type_after_keyword |> find_index(tokens[i].str) >= 0)
                mark_tokens_as_type_inside_angle(i + 1)

    // array type
    for i in range(0, length(tokens) - 3)
        if eq(i, "[") && eq(i + 1, "[")
            mark_tokens_as_type(i + 2)

    // struct / class / variant or multiline variables
    for i in range(0, length(tokens) - 3)
        if new_line_before(i) && (eq(i, "struct") || eq(i, "class") || eq(i, "variant") ||
                ((eq(i, "typedef") || eq(i, "var") || eq(i, "let")) && new_line_before(i + 1)))
            let ident = tokens[i].spaces
            for j in range(i + 1, length(tokens) - 3)
                if (new_line_before(j))
                    if eq(j, "def")
                        break
                    if (tokens[j].spaces <= ident)
                        break
                    var colonPos = search_token_in_line(j, eq(i, "typedef") ? "=" : ":")
                    if colonPos > 0
                        mark_tokens_as_type(colonPos)

    // functions
    for i in range(0, length(tokens) - 3)
        if new_line_before(i) && eq(i, "def") && eq(i + 2, ":")
            mark_tokens_as_type(i + 2)
        if (new_line_before(i) && eq(i, "def")) || ((eq(i, "$") || eq(i, "@")) && (eq(i + 1, "(") || eq(i + 1, "[")))
            var beginParams: int = i
            if (eq(i + 1, "["))
                beginParams = find_pair_square(i + 1) + 1
            beginParams = search_token_in_line(beginParams, "(")

            if beginParams > 0
                var endParams = find_pair_paren(beginParams)
                for j in range(beginParams, endParams + 1)
                    tokens[j].isInFunctionParam = true
                    if j - 1 == beginParams || eq(j - 1, ";")
                        var colonPos = search_token_in_line(j, ":")
                        if colonPos > 0
                            mark_tokens_as_type(colonPos)
                if endParams < length(tokens) - 1
                    if eq(endParams + 1, ":")
                        mark_tokens_as_type(endParams + 1)

    // annotations
    for i in range(0, length(tokens) - 1)
        if eq(i, "[") && !eq(i + 1, "[") && !eq(i + 1, "\{")
            for j in range(i + 1, length(tokens))
                if eq(j, "]")
                    break
                if eq(j, "=")
                    tokens[j].dontAddSpacesAround = true


def fmt_function_arguments()
    // function arguments
    var lastNewLineToken = 0
    var i = 0
    let n = length(tokens) - 3
    while i < n
        if tokens[i].newLines > 0
            lastNewLineToken = i

        if tokens[i].isInFunctionParam
            var firstBracketColumn = -1
            while i < n && tokens[i].isInFunctionParam
                if tokens[i].newLines > 0
                    lastNewLineToken = i

                if firstBracketColumn == -1
                    if eq(i, "(")
                        firstBracketColumn = 0
                        for k in range(lastNewLineToken, i + 1)
                            if tokens[k].newLines > 0
                                firstBracketColumn = 0
                            firstBracketColumn += tokens[k].spaces + length(tokens[k].str)

                elif tokens[i].newLines > 0
                    lastNewLineToken = i
                    tokens[i].spaces = firstBracketColumn
                    tokens[i].column = firstBracketColumn

                i++
        i++


def debug_print_tokens()
    for t in tokens
        to_log(LOG_DEBUG, "{t.isInFunctionParam ? ">" : " "}{t.isInType ? "*" : " "} '{t.str}'\n");


def generate_source(): string
    var res: array<uint8>
    res |> reserve(1000 + length(tokens) * 8)
    if haveUtf8Bom
        res |> push(uint8(0xEF))
        res |> push(uint8(0xBB))
        res |> push(uint8(0xBF))

    let useCrLf: bool = crCount * 2 > lfCount
    var column: int = 0

    for t in tokens
        for i in range(t.newLines)
            if (useCrLf)
                res |> push(uint8('\r'))
            res |> push(uint8('\n'))
            column = 0

        var spaces = t.spaces
        if t.tokenType == TokenType COMMENT && t.str |> starts_with("//")
            spaces = max(t.column - column, 0)

        for i in range(spaces)
            res |> push(uint8(' '))
            column++

        for c in t.str
            res |> push(uint8(c))
            column++

    return string(res)
finally
    delete res


def need_spaces_around(index: int): bool
    assume t = tokens[index]
    if t.tokenType != TokenType UNKNOWN || t.dontFormat || t.dontAddSpacesAround
        return false
    if dont_need_space_around |> find_index(t.str) >= 0
        return false
    if t.isInType && (t.str == ":" || t.str == "<" || t.str == ">" || t.str == ">>" || t.str == ">>>" || empty(t.str))
        return false
    return true


def need_space_only_before(index: int): bool
    assume t = tokens[index]
    if !t.isInType
        return false
    if t.tokenType != TokenType UNKNOWN || t.dontFormat || t.dontAddSpacesAround
        return false
    if dont_need_space_around |> find_index(t.str) >= 0
        return false
    if t.str == ">" || t.str == ">>" || t.str == ">>>" || t.str == "<" || empty(t.str)
        return false
    if index < length(tokens) - 1
        assume next = tokens[index + 1]
        if next.str == ">" || next.str == ">>" || next.str == ">>>" || next.str == ";"
            return true
    return false


def add_space_before(i)
    if (tokens[i].dontFormat || tokens[i - 1].dontFormat) || new_line_before(i)
        return
    if tokens[i].spaces == 0
        tokens[i].spaces = 1


def remove_space_before(i)
    if (tokens[i].dontFormat || tokens[i - 1].dontFormat) || new_line_before(i)
        return
    tokens[i].spaces = 0


let tokens_before_unary <- [[auto "("; "["; "?["; "\{"; "auto"; "int"; "float"; "int16"; "int32"; "int8"; ";"; ",";
    "if"; "while"; "switch"; "return"; "yield"; "in"; "="
    ]]

def is_unary(index: int): bool
    if eq(index, "!") || eq(index, "~")
        return true
    if eq(index, "+") || eq(index, "-") || eq(index, "*")
        if (tokens_before_unary |> find_index(tokens[index - 1].str) >= 0) || new_line_before(index)
            return true
        return need_spaces_around(index - 1)
    return false


def remove_spaces_around(index: int)
    if !new_line_before(index)
        remove_space_before(index)
    if !new_line_before(index + 1) && tokens[index + 1].tokenType != TokenType COMMENT
        remove_space_before(index + 1)


def make_spaces_around(index: int)
    if !new_line_before(index) && tokens[index].spaces == 0
        add_space_before(index)
    if !new_line_before(index + 1) && tokens[index + 1].spaces == 0
        add_space_before(index + 1)


def fmt_space_after_comma()
    for i in range(1, length(tokens))
        if !new_line_before(i) && eq(i - 1, ",") && tokens[i].spaces == 0
            add_space_before(i)


def fmt_space_after_semicolon()
    for i in range(1, length(tokens))
        if !eq(i, "") && !new_line_before(i) && eq(i - 1, ";") && tokens[i].spaces == 0
            add_space_before(i)


def fmt_remove_space_before_comma()
    for i in range(1, length(tokens))
        if !new_line_before(i) && eq(i, ",")
            remove_space_before(i)


def fmt_remove_space_before_semicolon()
    for i in range(1, length(tokens))
        if !new_line_before(i) && eq(i, ";")
            remove_space_before(i)


def fmt_type_colon()
    for i in range(1, length(tokens) - 1)
        if tokens[i].isInType && eq(i, ":")
            add_space_before(i)
            if !new_line_before(i + 1)
                add_space_before(i + 1)


def fmt_function_decl_param_paren()
    for i in range(1, length(tokens) - 1)
        if tokens[i].isInFunctionParam && !tokens[i - 1].isInFunctionParam && eq(i, "(")
            remove_spaces_around(i)
            if eq(i - 1, "]")
                add_space_before(i)
        if tokens[i].isInFunctionParam && !tokens[i + 1].isInFunctionParam && eq(i, ")")
            remove_space_before(i)


def fmt_function_call_param_paren_call()
    for i in range(1, length(tokens) - 1)
        if eq(i, "(") && !new_line_before(i) && tokens[i - 1].tokenType == TokenType KEYWORD_OR_IDENTIFIER
            remove_spaces_around(i)
            var closingParen = find_pair_paren(i)
            remove_space_before(closingParen)


def fmt_spaces_around_operators()
    for i in range(1, length(tokens) - 1)
        if !empty(tokens[i].str)
            if need_space_only_before(i) && !is_unary(i)
                add_space_before(i)
            elif need_spaces_around(i) && !is_unary(i)
                make_spaces_around(i)


let space_after_keywords <- [[auto "if"; "elif"; "while"; "switch"; "with"; "return";
        "yield"; "catch"; "static_if"; "static_elif"; "for"
    ]]

def fmt_space_after_keyword()
    for i in range(1, length(tokens) - 1)
        if space_after_keywords |> find_index(tokens[i].str) >= 0
            if !new_line_before(i + 1)
                add_space_before(i + 1)


def fmt_space_after_paren()
    for i in range(1, length(tokens) - 1)
        if eq(i, ")") && (tokens[i + 1].tokenType != TokenType UNKNOWN)
            add_space_before(i + 1)


def fmt_space_after_square_brackets()
    for i in range(1, length(tokens) - 1)
        if eq(i, "]") && (tokens[i + 1].tokenType != TokenType UNKNOWN)
            add_space_before(i + 1)


def fmt_remove_space_inside_parens()
    for i in range(1, length(tokens) - 1)
        if eq(i, "(")
            remove_space_before(i + 1)
        if eq(i, ")")
            remove_space_before(i)


let type_specifiers = [[string "-"; "?"; "&"; "#"]]

def fmt_glue_type_specifiers()
    for i in range(1, length(tokens))
        if (tokens[i].isInType && tokens[i].tokenType == TokenType UNKNOWN &&
                type_specifiers |> find_index(tokens[i].str) >= 0)
            remove_space_before(i)


def fmt_space_after_cast_type()
    for i in range(0, length(tokens) - 3)
        if tokens[i].tokenType == TokenType KEYWORD_OR_IDENTIFIER
            if (type_after_keyword |> find_index(tokens[i].str) >= 0)
                for j in range(i + 2, length(tokens))
                    if !tokens[j].isInType
                        add_space_before(j)
                        break


def do_format
    parse_all_tokens()
    mark_token_context()
    //debug_print_tokens()
    fmt_spaces_around_operators()
    fmt_function_call_param_paren_call()
    fmt_space_after_comma()
    fmt_space_after_semicolon()
    fmt_function_decl_param_paren()
    fmt_type_colon()
    fmt_space_after_cast_type()
    fmt_remove_space_before_comma()
    fmt_remove_space_before_semicolon()
    fmt_remove_space_inside_parens()
    fmt_glue_type_specifiers()
    fmt_space_after_keyword()
    fmt_space_after_paren()
    fmt_space_after_square_brackets()


def public format_source_string(file_data: string const& implicit): string
    data <- [{ for c in file_data; uint8(c) }]
    data |> push(uint8(0))
    do_format()
    return <- generate_source()


def public format_source(file_data: array<uint8> const& implicit): string
    data := file_data
    data |> push(uint8(0))
    do_format()
    return <- generate_source()


def public format_file(file_name: string const&)
    var unformatted: string = ""
    var formatted: string = ""
    fopen(file_name, "rb") <| $(fr)
        if fr == null
            to_log(LOG_ERROR, "cannot open '{file_name}'\n")
        else
            fmap(fr) <| $(data)
                unformatted = string(data)
                formatted = format_source(data)

    if debugMode
        to_log(LOG_DEBUG, formatted)

    if !empty(formatted) && formatted != unformatted && !debugMode
        fopen(file_name, "wb") <| $(fw)
            if fw == null
                to_log(LOG_ERROR, "cannot open '{file_name} for write'\n")
            else
                fw |> fprint(formatted)


def public format_files(file_names: array<string>&)
    for fname in file_names
        format_file(fname)

/*
[export]
def main()
    var fileNames: array<string>
    var filesListFn: string = ""
    var ignore = true
    for arg in get_command_line_arguments()
        if arg == "--debug"
            debugMode = true
        if arg == "-args"
            ignore = false
            continue
        if !ignore
            if arg |> character_at(0) != '-'
                fileNames |> push(arg)
            if arg |> starts_with("--files:")
                filesListFn = arg |> slice(8)

    if !empty(filesListFn)
        fopen(filesListFn, "rb") <| $(f)
            if f == null
                to_log(LOG_ERROR, "cannot open '{filesListFn}'\n")
            else
                while !feof(f)
                    var file_name = fgets(f)
                    if !empty(file_name)
                        file_name = replace(file_name, "\n", "")
                        file_name = replace(file_name, "\r", "")
                        fileNames |> push(file_name)

    format_files(fileNames)
*/
