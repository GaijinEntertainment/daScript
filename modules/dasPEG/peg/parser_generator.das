options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module parser_generator

require daslib/match public
require daslib/strings
require daslib/ast
require daslib/fio
require daslib/rtti
require daslib/ast_boost
require daslib/utf8_utils
require daslib/templates_boost

require peg/meta_ast


struct ParserGenerator
    // Provides information to generate unique names
    rule_counter: table<string; int>

    // Maps rule name into its declared type (typeof(add))
    // Action block returns the same result as in the mapping
    rule_types: table<string; TypeDeclPtr>

    // E.g. add => ResultAdd aka tuple<bool;typeof(add);int>
    return_types: table<string; TypeDeclPtr>

    // Contains the required result type for the current block return type
    // This is actully the tip of the stack of contexts. A different context
    // is entered during the generation of code for repetition (*) and option (MB)
    current_context: string

    // E.g. jsonParser
    parser_name: string

    // The constructed type for the parser
    parser_type: TypeDeclPtr


def abort(message: string)
    print("{message}\n")
    unsafe { exit(1); }


def iota
    return <- generator<int>() <|
        var i: int = 0
        while true
            i += 1
            return false if i == 0 else yield i


// Array is asserted to be valid utf-8 at the beginning of the parsing.
def public get_next_character(utf8_string: array<uint8>; var index: int&): int
    var codepoint = 0
    var state = 0u

    while true // Guaranteed to terminate
        let byte = utf8_string[index] |> int()
        index += 1
        let type_ = s_utf8d[byte]
        codepoint = (byte & int(0x3F)) | (codepoint << 6)
        state = s_utf8d[256u + state + type_]
        return codepoint if state == UTF8_ACCEPT

    return 0


def public decode_utf8_codepoint(utf8_data: array<uint8>; var index: int): tuple<res:int; len:int>
    var byte1 = utf8_data[index] |> uint()

    if (byte1 & 0x80u) == 0u
        // Single-byte codepoint
        return [[auto byte1 |> int(), 1]]
    elif (byte1 & 0xE0) == 0xC0
        // Two-byte codepoint
        var byte2 = utf8_data[index + 1] |> uint()
        var val = ((byte1 & 0x1F) << 6u) | (byte2 & 0x3F)
        return [[auto val |> int(), 2]]
    // elif (byte1 & 0xF0) == 0xE0
    //     // Three-byte codepoint
    //     var byte2 = utf8_data[index + 1]
    //     var byte3 = utf8_data[index + 2]
    //     var val = ((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F)
    //     return [[auto val, 3]]
    // elif (byte1 & 0xF8) == 0xF0
    //     // Four-byte codepoint
    //     var byte2 = utf8_data[index + 1]
    //     var byte3 = utf8_data[index + 2]
    //     var byte4 = utf8_data[index + 3]
    //     var val = ((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F)
    //     return [[auto val, 4]]
    else
        // Invalid UTF-8 sequence
        return [[auto -1, 0]]


def rule_left_recursive(name: string; rule: Rule): bool
    match rule
        if [[Rule alt = $v(alts)]]
            for a in alts
                return true if rule_left_recursive(name, a.rule.rule)

        if [[Rule seq = $v(seq)]]
            return true if rule_left_recursive(name, seq[0].rule)

        if [[Rule maybe_repeat = $v(rule_)]]
            return true if rule_left_recursive(name, rule_.rule)

        if [[Rule repeat = $v(rule_)]]
            return true if rule_left_recursive(name, rule_.rule)

        if [[Rule not_rule = $v(rule_)]]
            return true if rule_left_recursive(name, rule_.rule)

        if [[Rule and_rule = $v(rule_)]]
            return true if rule_left_recursive(name, rule_.rule)

        if [[Rule nonterminal = $v(name_)]]
            return true if name == name_

        if [[Rule bound_nonterminal = $v(tup)]]
            return true if tup._0 == name

        if [[Rule terminal = _]]
            return false

        if [[Rule text_extraction = $v(rule_)]]
            return true if rule_left_recursive(name, rule_.rule)

        if _
            abort("TODO: rule_left_recursive")

    return false


def generate_wrapper(var gen: ParserGenerator)
    let rule_name = gen.current_context
    let cache_table = "{rule_name}_cache"
    let inner_parsing_fun = "parse_{rule_name}_inner"

    var inscope return_type <- gen.return_types[rule_name] |> clone_type

    var inscope wrapper_fun <- qmacro_function("parse_{rule_name}") <| $ (var parser: $t(gen.parser_type)): $t(return_type)
        var mark = parser.index

        if parser.$f(cache_table) |> key_exists(parser.index)
            var result := parser.$f(cache_table)[parser.index]
            parser.index = result.endpos
            return <- result

        var result <- $c(inner_parsing_fun)(parser)
        parser.$f(cache_table)[mark] := result

        return <- result

    wrapper_fun.moreFlags |= MoreFunctionFlags skipLockCheck

    return <- wrapper_fun


def generate_wrapper_leftrec(var gen: ParserGenerator)
    let rule_name = gen.current_context
    let cache_table = "{rule_name}_cache"
    let inner_parsing_fun = "parse_{rule_name}_inner"

    var inscope return_type <- gen.return_types[rule_name] |> clone_type

    var inscope wrapper_fun <- qmacro_function("parse_{rule_name}") <| $ (var parser: $t(gen.parser_type)): $t(return_type)
        var mark = parser.index

        with parser
            if $i(cache_table) |> key_exists(index)
                var result = $i(cache_table)[index]
                parser.index = result.endpos
                return result

            // Build new cache entry from scratch

            var res = [[$t(return_type)]]
            $i(cache_table)[mark] = res

            while true
                parser.index = mark

                var newres = $c(inner_parsing_fun)(parser)
                var endpos = parser.index

                // Break if no movement
                break if res.endpos >= endpos

                res = newres
                $i(cache_table)[mark] = res


            parser.index = res.endpos
            return res

    wrapper_fun.moreFlags |= MoreFunctionFlags skipLockCheck

    return <- wrapper_fun


def generate_parser_class(var gen: ParserGenerator; parser_name: string)
    var inscope s: StructurePtr <- new [[Structure()]]

    // jsonParser, expressionParser, dasParser
    s.name := "{parser_name}Parser"
    gen.parser_name := s.name

    s._module = compiling_module()

    // Add a field for every type of cache

    for rule_name in keys(gen.rule_types)
        if true
            // Make cache table type: table<int; <result-type-for-rule>>
            var inscope t2 <- qmacro_type(type<table<int; int>>)
            t2.secondType |> move_new <| clone_type(gen.return_types[rule_name])

            s |> add_structure_field("{rule_name}_cache", t2)

    // Add input and index fields

    s |> add_structure_field_new("input", qmacro_type(type<array<uint8>>))
    s |> add_structure_field_new("index", qmacro_type(type<int>))

    s.flags |= StructureFlags skipLockCheck

    // Save constructed parser type for later

    var inscope parser_type <- qmacro_type(type<ParserGenerator>)
    parser_type.structType := s |> get_ptr

    gen.parser_type := parser_type

    compiling_module() |> add_structure(s)


def add_structure_field_new(var struct_: StructurePtr; name: string; var t: TypeDeclPtr)
    add_structure_field(struct_, name, t)

def add_structure_field(var struct_: StructurePtr; name: string; var t: TypeDeclPtr&)
    //! Adds a field to the structure.

    let fi = length(struct_.fields)
    struct_.fields |> resize(fi + 1)

    struct_.fields[fi].name := name
    struct_.fields[fi]._type |> move <| t


def make_default_type_decl: TypeDeclPtr
    //! Make default result (field value to be replaced later)
    return <- qmacro_type(type<tuple<success:bool; value:int; endpos:int>>)


def generate_result_types(var gen: ParserGenerator)
    //! Result types are used internally in the compiler to store in the cache.
    //!
    //! Manually they would be defined like so:
    //!     typedef Result1 = tuple<success:bool; value:int; endpos:int>;
    //! for every possible rule type
    //!

    for rule, type_ in keys(gen.rule_types), values(gen.rule_types)
        if true
            var inscope rule_res_type <- make_default_type_decl()
            rule_res_type.argTypes[1] |> move_new <| clone_type(type_)

            // Map name of the rule to its 'result' type
            gen.return_types[rule] |> move <| rule_res_type

    // Also add for types for the contexts of * and ?
    // They can be anything, they are used for the short-circuiting out of the current context

    gen.return_types["__repetition__"] := make_default_type_decl()
    gen.return_types["__option__"] := make_default_type_decl()
    gen.return_types["__PEEK__"] := make_default_type_decl()



def generate(var gen: ParserGenerator; var def_: Definition)
    gen.current_context = def_.name

    var inscope function_body <- gen |> generate(def_.rule)
    var inscope return_type <- gen.return_types[def_.name] |> clone_type

    var inscope fun <- qmacro_function("parse_{def_.name}_inner") <| $ (var parser: $t(gen.parser_type)): $t(return_type)
        $b(function_body)
        return <- [[$t(return_type)]]

    // XXX: disabling locks for the entire function
    fun.moreFlags |= MoreFunctionFlags skipLockCheck

    //fun |> describe |> print
    compiling_module() |> add_function(fun)

    if rule_left_recursive(def_.name, def_.rule)
        var inscope wrapper <- gen |> generate_wrapper_leftrec
        compiling_module() |> add_function(wrapper)
    else
        var inscope wrapper <- gen |> generate_wrapper
        compiling_module() |> add_function(wrapper)


def generate_grammar(var gen: ParserGenerator; var gram: array<Definition>; name: string)
    for rule in gram
        gen |> set_rule_type(rule.name |> string(), rule.type_)

    // In this order: class depends on types, methods depend on class
    gen |> generate_result_types

    gen |> generate_parser_class(name)

    gen |> generate_parser_methods

    for def_ in gram
        gen |> generate(def_)


def generate_parser_methods(var gen: ParserGenerator)
    var inscope eof <- qmacro_function("reached_EOF") <| $(var parser: $t(gen.parser_type)): bool
        return parser.input |> length <= parser.index

    var inscope skip_whitespace <- qmacro_function("skip_whitespace") <| $(var parser: $t(gen.parser_type))
        while parser |> get_current_char |> is_white_space
            parser.index += 1

    var inscope get_current_char <- qmacro_function("get_current_char") <| $(var parser: $t(gen.parser_type))
        return -1 if parser.input |> length <= parser.index
        return parser.input[parser.index] |> int()

    // TODO: UTF-8
    // var inscope get_current_char <- qmacro_function("get_current_char") <| $(var parser: $t(gen.parser_type))
    //     return [[auto -1, 0]] if parser.input |> length <= parser.index
    //     return decode_utf8_codepoint(parser.input, parser.index)
    // var inscope peek_current_char <- qmacro_function("peek_current_char") <| $(var parser: $t(gen.parser_type))
    //     return -1 if parser.input |> length <= parser.index
    //     return decode_utf8_codepoint(parser.input, parser.index).res

    var inscope match_decimal_literal <- qmacro_function("match_decimal_literal") <| $(var parser: $t(gen.parser_type)): tuple<success:bool;value:int;endpos:int>
        //! Simple lexing of decimal integers, doesn't check for overflow

        if !parser |> get_current_char |> is_number
            // Can't use typedef in this context, sadly
            return [[tuple<success:bool;value:int;endpos:int>]]

        var result = 0

        while parser |> get_current_char |> is_number
            result *= 10
            result +=  parser |> get_current_char - '0'
            parser.index += 1

        return [[auto true, result, parser.index]]

    var inscope match_string_literal <- qmacro_function("match_string_literal") <| $ (var parser: $t(gen.parser_type)): tuple<success:bool; string>
        //! Tries to match everything inside ""

        // Define buffer to store string literal characters
        var inscope buffer: array<uint8>

        var current_char = parser |> get_current_char

        // If the current character is not a double quote, the rule is not a string
        return [[auto false, ""]] if current_char != '"'

        parser.index += 1
        current_char = parser |> get_current_char

        while current_char != '"' && !parser |> reached_EOF
            buffer |> push(current_char |> uint8())
            parser.index += 1
            current_char = parser |> get_current_char

        // If we've reached EOF file without finding a closing quote
        return [[auto false, ""]] if parser |> reached_EOF

        parser.index += 1
        return [[auto true, buffer |> string()]]

    var inscope match_double_literal <- qmacro_function("match_double_literal") <| $ (var parser: $t(gen.parser_type)): tuple<success:bool; double>
        //! Matches doubles in the form of [-+]? [0-9]* .? [0-9]+ ([eE] [-+]? [0-9]+)?
        //! The number is not checked to be representable as defined in IEEE-754

        var current_char = parser |> get_current_char

        if !current_char |> is_number && current_char != '+' && current_char != '-'
            return <- [[auto false, 0.0 |> double()]]

        var inscope buffer: array<uint8>

        if current_char == '-' || current_char == '+'
            buffer |> push(current_char |> uint8())
            parser.index += 1
            current_char = parser |> get_current_char

        // Add everything up to '.' to the buffer
        while current_char |> is_number || current_char == '.'
            buffer |> push(current_char |> uint8())
            parser.index += 1
            current_char = parser |> get_current_char


        // Match exponent part
        if current_char == 'e' || current_char == 'E'
            buffer |> push(current_char |> uint8())
            parser.index += 1
            current_char = parser |> get_current_char

            // Check for '-' or '+' after 'e' or 'E'
            if current_char == '-' || current_char == '+'
                buffer |> push(current_char |> uint8())
                parser.index += 1
                current_char = parser |> get_current_char

            // Continue appending digits after 'e' or 'E'
            while current_char |> is_number
                buffer |> push(current_char |> uint8())
                parser.index += 1
                current_char = parser |> get_current_char

        return [[auto true, buffer |> string() |> double()]]


    compiling_module() |> add_function(eof)
    compiling_module() |> add_function(skip_whitespace)
    compiling_module() |> add_function(get_current_char)
    compiling_module() |> add_function(match_decimal_literal)
    compiling_module() |> add_function(match_string_literal)
    compiling_module() |> add_function(match_double_literal)


def get_rule_type(var gen: ParserGenerator; rule: Rule): TypeDeclPtr
    match rule
        if [[Rule nonterminal = $v(nonterm)]]
            // TODO: check for leaks
            var inscope type_ := gen.rule_types[nonterm]
            return <- type_

        if [[Rule subrule = $v(subrule)]]
            abort("Infer subrule type")

    return <- [[TypeDeclPtr]]


def set_rule_type(var gen: ParserGenerator; rule_name: string; type_: TypeDeclPtr)
    gen.rule_types[rule_name] |> move_new(type_ |> clone_type)


def set_rule_handle(var subrule_: Rule_?; handle: string)
    //! Motivating example: *CommaSeparatedElements as els
    //!
    //! Here the name 'els' is bound to the repetition (*)
    //! In order to get the contents of the repetition in the resulting vector
    //! we need to bind the name of the subrule

    assume subrule_variant = subrule_.rule

    match subrule_variant
        if [[Rule nonterminal = $v(nonterm)]]
            unsafe // Moving values that contain smart pointers is generally unsafe
                   // But here we know that the contained variant is nonterminal (string) and cannot
                   // possibly contain valid smart pointer. Therefore it should be safe to overwrite it.
                subrule_.rule <- [[Rule bound_nonterminal = [[auto nonterm, handle]]]]
                return

        if [[Rule terminal = $v(term)]]
            match term
                if [[Terminal lit = $v(l) ]]
                    // Something like (*"123" as foo)
                    abort("Matchint repeated literals is not allowed")

                if [[Terminal charset = $v(r) ]]
                    // *[1-9] as numbers -- should be possible
                    abort("Matchint repeated literals is not allowed")

                if [[Terminal number = $v(name)]]
                    name = handle

                if [[Terminal whitespace = _]]
                    abort("Matchint repeated whitespace is not allowed")

                if [[Terminal EOF = _]]
                    abort("Matchint repeated EOF is not allowed")

                if [[Terminal string_ = $v(name)]]
                    name = handle

                if [[Terminal double_ = $v(name)]]
                    name = handle

            return

        if [[Rule bound_nonterminal = $v(nonterm)]]
            // Cannot bind several times
            abort("Unreachable")

        if [[Rule seq = $v(rules)]]
            abort("Unreachable")

        if [[Rule alt = $v(alts)]]
            abort("Unreachable")

        if [[Rule maybe_repeat = $v(rule_)]]
            abort("Unreachable")

        if [[Rule repeat = $v(rule_)]]
            abort("Unreachable")

        if [[Rule subrule = $v(subrule)]]
            abort("TODO: set_rule_handle option")

        if [[Rule option = $v(rule_)]]
            abort("Unreachable")

    abort("Unreachable")


def get_action_block(var alternative: Alternative): ExpressionPtr
    return <- alternative.action


def flatten_block(var blk: ExprBlock?)
    var inscope result : array<ExpressionPtr>
    for e in blk.list
        result |> emplace <| e
    return <- result


def generate(var gen: ParserGenerator; var rule_: Rule): array<ExpressionPtr>
    match rule_
        if [[Rule terminal = $v(term)]]
            var inscope termb <- gen |> generate_terminal(term)
            return <- flatten_block(termb as ExprBlock)

        if [[Rule nonterminal = $v(nonterm)]]
            let name = nonterm
            let count = "{gen.rule_counter[name]++}"

            // Type of the function that we are now generating code for
            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

            // varname is a generic name for unnamed nonterminals
            // e.g. add4, add5, mul0
            let varname = "{name}{count}"
            let parse_fun = "parse_{name}"

            var inscope t  <- qmacro_block <|
                var $i(varname) <- $c(parse_fun)(parser)
                return <- [[$t(return_type)]] if !$i(varname).success

            return <- flatten_block(t as ExprBlock)


        if [[Rule bound_nonterminal = $v(nonterm)]]
            let name = nonterm._0
            let result_name = nonterm._1

            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

            let count = "{gen.rule_counter[name]}"
            let varname = "{name}{count}"
            let parse_fun = "parse_{name}"

            var inscope t <- qmacro_block <|
                var $i(varname) <- $c(parse_fun)(parser)
                return <- [[$t(return_type)]] if !$i(varname).success
                var $i(result_name) <- $i(varname).value

            return <- flatten_block(t as ExprBlock)

        if [[Rule text_extraction = $v(subrule)]]

            var result_handle = subrule.name
            var inscope subrule_ <- gen |> generate(subrule.rule)
            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type
            var i = gen.rule_counter["extract"]++

            var inscope t <- qmacro_block <|
                var $i("ext_pos{i}") = parser.index
                $b(subrule_)
                var $i("str{i}") = unsafe(reinterpret<string> unsafe(addr(parser.input[0])))
                var $i(result_handle): string <- $i("str{i}") |> slice($i("ext_pos{i}"), parser.index)

            return <- flatten_block(t as ExprBlock)

        if [[Rule seq = $v(rules)]]

            var inscope result: array<ExpressionPtr>

            for rule in rules
                for expr in gen |> generate(rule.rule)
                    result |> emplace <| expr

            return <- result

        if [[Rule alt = $v(alts)]]

            var inscope results: array<ExpressionPtr>

            for alt, i in alts, iota()
                results |> emplace_new <| generate_one_alternative(gen, alt, i)

            return <- results

        if [[Rule maybe_repeat = $v(rule_)]]

            var result_handle = rule_.name
            var inscope type_decl <- gen |> get_rule_type(rule_.rule)

            if rule_.name == "" // Rule not bound by name, discard result
                var inscope rule_code <- gen |> with_context("__repetition__") <|
                    return <- gen |> generate(rule_.rule)

                rule_code |> append_block <| ( qmacro_block <| {
                    return <- [[$t(make_default_type_decl()) true, 0, parser.index]];
                } )

                var inscope t <- qmacro_block <|
                    while true
                        var res <- invoke <|
                            $b(rule_code)
                        break if !res.success

                return <- flatten_block(t as ExprBlock)

	        // Bind result to the provided name

            set_rule_handle(rule_, "temp_name")
            var inscope rule_code <- gen |> with_context("__repetition__") <|
                return <- gen |> generate(rule_.rule)

            rule_code |> append_block <| ( qmacro_block <| {
                $i(result_handle) |> emplace <| temp_name;
                return <- [[$t(make_default_type_decl())]];
            } )

            var inscope t <- qmacro_block <|
                var $i(result_handle) : array<$t(type_decl)>

                while true
                    var sz = $i(result_handle) |> length

                    invoke <|
                        $b(rule_code) // Adds an element on success

                    break if sz == $i(result_handle) |> length

            return <- flatten_block(t as ExprBlock)

        if [[Rule not_rule = $v(rule_)]]

            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

            var inscope rule_code <- gen |> with_context("__PEEK__") <|
                return <- gen |> generate(rule_.rule)

            var inscope epilogue <- qmacro_block <|
                return <- [[$t(make_default_type_decl()) true, 0, parser.index]];

            for e in flatten_block(epilogue as ExprBlock)
                rule_code |> emplace <| e

            var inscope t <- qmacro_block <|
                var mark = parser.index

                var success = invoke <|
                    $b(rule_code)

                parser.index = mark
                return <- [[$t(return_type)]] if success.success

            return <- flatten_block(t as ExprBlock)

        if [[Rule and_rule = $v(rule_)]]

            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

            var inscope rule_code <- gen |> with_context("__PEEK__") <|
                return <- gen |> generate(rule_.rule)

            var inscope epilogue <- qmacro_block <|
                return <- [[$t(make_default_type_decl()) true, 0, parser.index]];

            for e in flatten_block(epilogue as ExprBlock)
                rule_code |> emplace <| e

            var inscope t <- qmacro_block <|
                var mark = parser.index

                var success = invoke <|
                    $b(rule_code)

                parser.index = mark
                return <- [[$t(return_type)]] if !success.success

            return <- flatten_block(t as ExprBlock)


        if [[Rule repeat = $v(rule_)]]

            var result_handle = rule_.name
            var inscope type_decl <- gen |> get_rule_type(rule_.rule)
            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type


            if rule_.name == "" // Rule not bound by name, discard result
                var inscope rule_code <- gen |> with_context("__repetition__") <|
                    return <- gen |> generate(rule_.rule)

                rule_code |> append_block <| ( qmacro_block <| {
                    return <- [[$t(make_default_type_decl()) true, 0, parser.index]];
                } )

                var inscope t <- qmacro_block <|
                    var res1 <- invoke <|
                        $b(rule_code)

                    return [[$t(return_type)]] if !res1.success

                    while true
                        var res <- invoke <|
                            $b(rule_code)
                        break if !res.success

                return <- flatten_block(t as ExprBlock)

	       // Bind result to the provided name

            set_rule_handle(rule_, "temp_name")
            var inscope rule_code <- gen |> with_context("__repetition__") <|
                return <- gen |> generate(rule_.rule)

            rule_code |> append_block <| ( qmacro_block <| {
                $i(result_handle) |> emplace <| temp_name;
                return <- [[$t(make_default_type_decl()) true, 0, parser.index]];
            } )

            var inscope t <- qmacro_block <|
                var $i(result_handle) : array<$t(type_decl)>

                var res1 <- invoke <|
                    $b(rule_code)

                return [[$t(return_type)]] if !res1.success
                $i(result_handle) |> emplace <| res1.value

                while true
                    var sz = $i(result_handle) |> length

                    invoke <|
                        $b(rule_code) // Adds an element on success

                    break if sz == $i(result_handle) |> length

            return <- flatten_block(t as ExprBlock)

        if [[Rule subrule = $v(subrule)]]
            // Create a new unnamed subrule for the (...)
            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type
            var inscope rule_code <- gen |> generate(subrule.rule)

            var inscope t <- qmacro_block <|
                var res <- invoke($b(rule_code))
                return [[$t(return_type)]] if !res.success

            return <- flatten_block(t as ExprBlock)

        if [[Rule option = $v(rule_)]]
            var result_handle = rule_.name

            var inscope type_decl <- gen |> get_rule_type(rule_.rule)
            var inscope result_type <- gen.return_types[gen.current_context] |> clone_type

            if rule_.name == "" // Rule not bound by name, discard result
                var inscope rule_code <- gen |> generate(rule_.rule)
                var inscope t <- qmacro_block <|
                    var res <- invoke($b(rule_code))

                return <- flatten_block(t as ExprBlock)

	        // Bind result to the provided name

            set_rule_handle(rule_, "temp_name")
            var inscope rule_code <- gen |> with_context("__option__") <|
                return <- gen |> generate(rule_.rule)

            var inscope epilogue <- qmacro_block <|
                $i(result_handle) |> emplace <| temp_name
                return <- [[$t(make_default_type_decl())]]

            for e in flatten_block(epilogue as ExprBlock)
                rule_code |> emplace <| e

            var inscope t <- qmacro_block <|
                var $i(result_handle) : array<$t(type_decl)>

                invoke <|
                    $b(rule_code)

            return <- flatten_block(t as ExprBlock)


    abort("Unreachable")
    return <- [[array<ExpressionPtr>]]


def with_context(var gen: ParserGenerator; context: string; block_)
    //! Invokes a block of code in the generator context

    let last_context = gen.current_context
    gen.current_context = context

    var inscope r <- invoke(block_)

    gen.current_context = last_context
    return <- r


def generate_one_alternative(var gen: ParserGenerator; var alt: Alternative; var i)
    let block_name = "blk{i}"
    let parse_pos = "pos{i}"
    let result  = "res{i}"

    var inscope action <- alt.action
    var inscope action_block <- action as ExprBlock |> flatten_block
    var inscope block_contents <- gen |> generate(alt.rule.rule)

    // Add epilogue (code that executes the associated action)

    block_contents |> alternative_add_epilogue(gen, action_block)

    // Generate the alternative

    return <- qmacro_block <|
        var parse_pos = parser.index

        var $i(result) <- invoke <|
            $b(block_contents)

        return <- $i(result) if $i(result).success

        parser.index = parse_pos


def alternative_add_epilogue(var block_contents: array<ExpressionPtr>; var gen: ParserGenerator; var action_block)
    var inscope action_type <- gen.rule_types[gen.current_context] |> clone_type()
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type()

    var inscope t <- qmacro_block <|
        var val: $t(action_type) <- invoke <| $(): $t(action_type)
            $b(action_block)

        // For some reason I cannot directly return [[Result]]
        var result: $t(return_type) <- [[$t(return_type) true, val, parser.index]]
        return <- result

    for e in flatten_block(t as ExprBlock)
        block_contents |> emplace <| e


def generate_terminal(var gen: ParserGenerator; var terminal: Terminal): ExpressionPtr
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type()

    match terminal
        if [[Terminal lit = $v(l) ]]
            var precomputed_length = l |> length

            return <- qmacro_block <|
                return <- [[$t(return_type)]] if !parser |> matches($v(l), $v(precomputed_length))

        if [[Terminal charset = $v(ranges) ]]

            // Get the currect charachter from the stream

            var inscope rule_code: array<ExpressionPtr>

            rule_code |> append_block <| ( qmacro_block <| {
                let current_char = parser |> get_current_char;
            } )

            // Check in it is inside any of the ranges

            for r in ranges
                rule_code |> append_range(r)

            // Otherwise return false

            rule_code |> append_block <| ( qmacro_block <| {
                return false;
            } )

            return <- qmacro_block <|
                var success = invoke <|
                    $b(rule_code)

                return <- [[$t(return_type)]] if !success

                parser.index += 1


        if [[Terminal number = $v(handle)]]

            return <- qmacro_block <|
                var res : tuple<bool;int;int> = parser |> match_decimal_literal
                return [[$t(return_type)]] if !res._0
                var $i(handle) = res._1

        if [[Terminal whitespace = _]]

            return <- qmacro_block <|
                parser |> skip_whitespace

        if [[Terminal EOF = _]]

            return <- qmacro_block <|
                return <- [[$t(return_type)]] if !parser |> reached_EOF

        if [[Terminal any = _]]

            return <- qmacro_block <|
                return [[$t(return_type)]] if parser |> reached_EOF
                parser.index += 1


        if [[Terminal string_ = $v(handle)]]

            return <- qmacro_block <|
                var res <- parser |> match_string_literal
                return [[$t(return_type)]] if !res.success
                var $i(handle) : string = res._1

        if [[Terminal double_ = $v(handle)]]

            return <- qmacro_block <|
                var res <- parser |> match_double_literal
                return [[$t(return_type)]] if !res.success
                var $i(handle) = res._1


    abort("Unreachable")
    return <- [[ExpressionPtr]]


def append_range(var rule_code; r)
    if r._0 == r._1
        rule_code |> append_block <| ( qmacro_block <| {
            return true if current_char == $v(r._0);
        } )
    else
        rule_code |> append_block <| ( qmacro_block <| {
            return true if current_char >= $v(r._0) && current_char <= $v(r._1);
        } )


def append_block(var old: array<ExpressionPtr>; var new_code: ExpressionPtr)
    for e in flatten_block(new_code as ExprBlock)
        old |> emplace <| e
