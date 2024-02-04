options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module parser_generator shared

require daslib/match public
require daslib/strings
require daslib/ast
require daslib/rtti
require daslib/math
require daslib/ast_boost
require daslib/utf8_utils
require daslib/macro_boost
require daslib/templates_boost

require peg/meta_ast
require detail/helpers

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

    // Enable debug output for library development
    debug: bool

    // Whether or not to emit code to trace the execution
    tracing: bool

    // Output ansi escape sequences in trace
    color_output: bool

    // Check for cut operator on fail
    cut_operator: bool

    // Print generated inner parsing functions
    print_generated: bool


def accept_option(var gen: ParserGenerator; opt: string)
    if opt == "tracing"
        gen.tracing = true
    elif opt == "print_generated"
        gen.print_generated = true
    elif opt == "color"
        gen.color_output = true
    else
        abort("Unsupported option `{opt}` passed to the parser generator")


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

        if [[Rule option = $v(rule_)]]
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

        if $v(gen.tracing)
            parser |> log_plain <| "Entered function parse_{$v(rule_name) |> bold}"
            parser.tabs++

        if parser.$f(cache_table) |> key_exists(parser.index) && !parser.error_reporting
            var result := parser.$f(cache_table)[parser.index]

            if $v(gen.tracing)
                parser |> log_info <| "Got result from cache {$v(cache_table)} {parser.index}"
                parser.tabs--

            if result.success // Change the state only on success
                parser.index = result.endpos

            return <- result

        var result <- $c(inner_parsing_fun)(parser)
        parser.$f(cache_table)[mark] := result

        if $v(gen.tracing)
            parser.tabs--
            parser |> log_info <| "Placing result {result} to cache {$v(cache_table)} {mark}"
            parser |> log_info <| "Matched from {mark} to {parser.index}"

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

        if $v(gen.tracing)
            parser |> log_plain <| "Entered function parse_{$v(rule_name) |> bold}"
            parser.tabs++

        with parser
            if $i(cache_table) |> key_exists(index) // For reft-recursive rules memoization must be enabled
                var result = $i(cache_table)[index]
                if result.success // Change the state only on success
                    parser.index = result.endpos

                if $v(gen.tracing)
                    parser |> log_info <| "Got result from cache {$v(cache_table)} {mark}"
                    parser.tabs--

                return result

            // Build new cache entry from scratch

            if $v(gen.debug)
                parser |> log_info <| "Entering the leftrec create cycle"

            var res = [[$t(return_type)]]
            $i(cache_table)[mark] = res

            while true
                parser.index = mark

                var newres = $c(inner_parsing_fun)(parser)
                var endpos = parser.index

                // Break if no movement
                if $v(gen.debug)
                    parser |> log_info <| "Advanced from {res.endpos} to {endpos} this iteration\n"
                break if res.endpos >= endpos || !newres.success

                res = newres

                if $v(gen.debug)
                    parser |> log_info <| "Placing result {res} to cache {$v(cache_table)} {mark}"

                $i(cache_table)[mark] = res

            if $v(gen.tracing)
                parser.tabs--
                parser |> log_info <| "Matched from {mark} to {res.endpos}"

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

    // Add all parser fields: parsing state, tracing, error reporting, etc

    s |> add_structure_field_new("input", qmacro_type(type<array<uint8>>))
    s |> add_structure_field_new("index", qmacro_type(type<int>))
    s |> add_structure_field_new("tabs", qmacro_type(type<int>))
    s |> add_structure_field_new("color_output", qmacro_type(type<bool>))
    s |> add_structure_field_new("error_reporting", qmacro_type(type<bool>))
    // The longest correctly parsed prefix in the grammar, for error reporting
    s |> add_structure_field_new("longest_prefix", qmacro_type(type<int>))
    // Supress errors flag is set when entering modifier like MB or *
    s |> add_structure_field_new("suppress_errors", qmacro_type(type<array<bool>>))
    s |> add_structure_field_new("errors", qmacro_type(type<array<ParsingError>>))

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

    gen.return_types["__repetition__"] |> move_new <| make_default_type_decl()
    gen.return_types["__option__"] |> move_new <| make_default_type_decl()
    gen.return_types["__PEEK__"] |> move_new <| make_default_type_decl()



def generate(var gen: ParserGenerator; var def_: Definition)
    gen.current_context = def_.name

    var inscope function_body <- gen |> generate(def_.rule)
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

    var inscope fun <- qmacro_function("parse_{def_.name}_inner") <| $ (var parser: $t(gen.parser_type)): $t(return_type)
        $b(function_body)
        return <- [[$t(return_type)]]

    if gen.print_generated
        fun |> describe |> print
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

    gen |> generate_result_types

    gen |> generate_parser_class(name)

    for def_ in gram
        gen |> generate(def_)


def get_rule_type(var gen: ParserGenerator; rule: Rule): TypeDeclPtr
    match rule
        if [[Rule nonterminal = $v(nonterm)]]
            return <- gen.rule_types[nonterm] |> clone_type

        if [[Rule bound_nonterminal = $v(nonterm)]]
            return <- gen.rule_types[nonterm._0] |> clone_type

        if [[Rule subrule = $v(subrule)]]
            abort("Infer subrule type")

    abort("Unreachable `get_rule_type`")
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
                    abort("Matching repeated literals is not allowed")

                if [[Terminal charset = $v(r) ]]
                    // *[1-9] as numbers
                    abort("Matching repeated charsets is not allowed -- use text extraction")

                if [[Terminal number = $v(name)]]
                    name = handle

                if [[Terminal whitespace = _]]
                    abort("Matching repeated whitespace is not allowed")

                if [[Terminal taborspace = _]]
                    abort("Matching repeated tab or space is not allowed")

                if [[Terminal EOF = _]]
                    abort("Matching repeated EOL is not allowed")

                if [[Terminal EOF = _]]
                    abort("Matching repeated EOF is not allowed")

                if [[Terminal string_ = $v(name)]]
                    name = handle

                if [[Terminal double_ = $v(name)]]
                    name = handle

            return

    abort("Unreachable: `set_rule_handle`")


def get_action_block(var alternative: Alternative): ExpressionPtr
    return <- alternative.action


// Contract: block is managed externally and will clean up its own memory
def flatten_block(var blk: ExprBlock?)
    var inscope result : array<ExpressionPtr>
    for e in blk.list
        result |> emplace_new <| (e |> clone_expression)
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
                if !$i(varname).success // Here we don't output an error, unless we impl. quiet or something
                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

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
                if !$i(varname).success
                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]
                var $i(result_name) <- $i(varname).value

            return <- flatten_block(t as ExprBlock)

        if [[Rule text_extraction = $v(subrule)]]
            var result_handle = subrule.name
            var i = gen.rule_counter["extract"]++
            var inscope subrule_ <- gen |> generate(subrule.rule)
            var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

            var inscope t <- qmacro_block <|
                var $i("ext_pos_{i}") = parser.index
                $b(subrule_)
                var $i("str_{i}") = unsafe(reinterpret<string> unsafe(addr(parser.input[0])))
                var $i(result_handle): string <- $i("str_{i}") |> slice($i("ext_pos_{i}"), parser.index)
                if $v(gen.tracing)
                    parser |> log_info <| "Extracted text from the stream: '{$i(result_handle) |> escape}'"

            return <- flatten_block(t as ExprBlock)

        if [[Rule seq = $v(rules)]]
            var inscope result: array<ExpressionPtr>

            for rule in rules
                for expr in gen |> generate(rule.rule)
                    result |> emplace_new <| expr

            return <- result

        if [[Rule alt = $v(alts)]]
            var inscope results: array<ExpressionPtr>

            for alt, i in alts, iota()
                results |> emplace_new <| generate_one_alternative(gen, alt, i, alts |> length == 1)

            return <- results

        if [[Rule maybe_repeat = $v(rule_)]]
            return <- gen |> generate_maybe_repeat(rule_)

        if [[Rule not_rule = $v(rule_)]]
            return <- gen |> generate_not(rule_)

        if [[Rule and_rule = $v(rule_)]]
            return <- gen |> generate_and(rule_)

        if [[Rule repeat = $v(rule_)]]
            return <- gen |> generate_repeat(rule_)

        if [[Rule subrule = $v(subrule)]]
            abort("TODO: Suburle")

        if [[Rule option = $v(rule_)]]
            return <- gen |> generate_option(rule_)

    abort("Unreachable")
    return <- [[array<ExpressionPtr>]]


def generate_maybe_repeat(var gen: ParserGenerator; var rule_: Rule_?): array<ExpressionPtr>
    var result_handle = rule_.name

    if rule_.name == "" // Rule not bound by name, discard result
        var inscope rule_code <- gen |> with_context("__repetition__") <|
            return <- gen |> generate(rule_.rule)

        rule_code |> append_block_new <| (qmacro_block <| {
            return_skip_lockcheck <| [[auto true, 0, parser.index]];
            return <- [[auto true, 0, parser.index]];
        })

        var inscope t <- qmacro_block <|
            parser.suppress_errors |> push <| true
            while true
                var res <- invoke <|
                    $b(rule_code)
                break if !res.success
            parser.suppress_errors |> pop

        return <- flatten_block(t as ExprBlock)

    // Bind result to the provided name

    set_rule_handle(rule_, "temp_name")
    var inscope type_decl <- gen |> get_rule_type(rule_.rule)

    var inscope rule_code <- gen |> with_context("__repetition__") <|
        return <- gen |> generate(rule_.rule)

    rule_code |> append_block_new <| (qmacro_block <| {
        $i(result_handle) |> emplace <| temp_name;
        return_skip_lockcheck <| [[auto true, 0, 0]];
        return <- [[auto true, 0, 0]];
    })

    var inscope t <- qmacro_block <|
        var $i(result_handle) : array<$t(type_decl)>
        parser.suppress_errors |> push <| true
        while true
            var sz = $i(result_handle) |> length
            invoke <|
                $b(rule_code) // Adds an element on success
            break if sz == $i(result_handle) |> length
        parser.suppress_errors |> pop

    return <- flatten_block(t as ExprBlock)


def generate_not(var gen: ParserGenerator; var rule_: Rule_?): array<ExpressionPtr>
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

    var inscope rule_code <- gen |> with_context("__PEEK__") <|
        return <- gen |> generate(rule_.rule)

    rule_code |> append_block_new <| (qmacro_block <| {
        return_skip_lockcheck <| [[auto true, 0, parser.index]];
        return <- [[auto true, 0, parser.index]];
    })

    var inscope t <- qmacro_block <|
        var mark = parser.index

        if $v(gen.debug)
            parser |> log_plain <| "Saved the state for negative assertion: [{mark}, {current}]"

        var success = invoke <|
            $b(rule_code)

        if $v(gen.debug)
            parser |> log_plain <| "Resetting the state: [{parser.index}, {parser.current}] -> [{mark}, {current}]"

        parser.index = mark

        if success.success
            if $v(gen.tracing)
                parser |> log_fail <| "Assertion successful; failing"

            return_skip_lockcheck <| [[$t(return_type)]]
            return <- [[$t(return_type)]]

            if $v(gen.tracing)
                parser |> log_success <| "Assertion failed; continuing"


    return <- flatten_block(t as ExprBlock)


def generate_and(var gen: ParserGenerator; var rule_: Rule_?): array<ExpressionPtr>
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type

    var inscope rule_code <- gen |> with_context("__PEEK__") <|
        return <- gen |> generate(rule_.rule)

    rule_code |> append_block_new <| (qmacro_block <| {
        return_skip_lockcheck <| [[auto true, 0, parser.index]];
        return <- [[auto true, 0, parser.index]];
    })

    var inscope t <- qmacro_block <|
        var mark = parser.index

        var success = invoke <|
            $b(rule_code)

        parser.index = mark

        if !success.success
            if $v(gen.tracing)
                parser |> log_fail <| "Assertion failed; failing"

            return_skip_lockcheck <| [[$t(return_type)]]
            return <- [[$t(return_type)]]

        if $v(gen.tracing)
            parser |> log_success <| "Assertion successful; continuing"


    return <- flatten_block(t as ExprBlock)


def generate_repeat(var gen: ParserGenerator; var rule_: Rule_?): array<ExpressionPtr>
    var result_handle = rule_.name
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type
    var first_result_name = "__repeat_result_{gen.rule_counter["repeat"]++}"

    if rule_.name == "" // Rule not bound by name, discard result
        var inscope rule_code <- gen |> with_context("__repetition__") <|
            return <- gen |> generate(rule_.rule)

        rule_code |> append_block_new <| (qmacro_block <| {
            return_skip_lockcheck <| [[auto true, 0, parser.index]];
            return <- [[auto true, 0, parser.index]];
        })

        var inscope t <- qmacro_block <|
            var $i(first_result_name) <- invoke <|
                $b(rule_code)

            if !$i(first_result_name).success
                if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                    var error = "Error: Expected the rule `" |> red_str($v(gen.color_output))
                    var terminal = "{$v(rule_ |> describe_rule)}" |> bold
                    var rest = "` at {parser.index}" |> red_str($v(gen.color_output))
                    var complete_message = "{error}{terminal}{rest}"
                    parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                return_skip_lockcheck <| [[$t(return_type)]]
                return <- [[$t(return_type)]]

            parser.suppress_errors |> push <| true
            while true
                var res <- invoke <|
                    $b(rule_code)
                break if !res.success
            parser.suppress_errors |> pop


        return <- flatten_block(t as ExprBlock)

    // Bind result to the provided name

    set_rule_handle(rule_, "temp_name")
    var inscope rule_code <- gen |> with_context("__repetition__") <|
        return <- gen |> generate(rule_.rule)

    rule_code |> append_block_new <| (qmacro_block <| {
        $i(result_handle) |> emplace <| temp_name;
        return_skip_lockcheck <| [[auto true, 0, parser.index]];
        return <- [[auto true, 0, parser.index]];
    })

    var inscope type_decl <- gen |> get_rule_type(rule_.rule)

    var inscope t <- qmacro_block <|
        var $i(result_handle) : array<$t(type_decl)>
        var $i(first_result_name) <- invoke <|
            $b(rule_code)

        if !$i(first_result_name).success
            if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                var error = "Error: Expected a `" |> red_str($v(gen.color_output))
                var terminal = "{$v(rule_ |> describe_rule)}" |> bold
                var rest = "` rule at {parser.index}" |> red_str($v(gen.color_output))
                var complete_message = "{error}{terminal}{rest}"
                parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

            return_skip_lockcheck <| [[$t(return_type)]]
            return <- [[$t(return_type)]]

        $i(result_handle) |> emplace <| ($i(first_result_name).value)

        parser.suppress_errors |> push <| true
        while true
            var sz = $i(result_handle) |> length
            invoke <|
                $b(rule_code) // Adds an element on success
            break if sz == $i(result_handle) |> length
        parser.suppress_errors |> pop

    return <- flatten_block(t as ExprBlock)


def describe_terminal(term: Terminal): string
    match term
        if [[Terminal lit = $v(l) ]]
            return "{l}"

        if [[Terminal charset = $v(ranges) ]]
            var sb = build_string <| $ ( writer )
                for i in range() <| min(ranges |> length, 3) // Don't print too many
                    var range_ = ranges[i]

                    if range_._0 == range_._1
                        writer |> write <| " '{range_._0}' "
                    else
                        writer |> write <| " '{range_._0}'-'{range_._1}' "

            return "set({sb})"

        if [[Terminal log_msg = $v(message) ]]
            return "log"

        if [[Terminal number = $v(handle)]]
            return "number"

        if [[Terminal whitespace = _]]
            return "whitespace"

        if [[Terminal taborspace = _]]
            return "taborspace"

        if [[Terminal EOF = _]]
            return "EOF"

        if [[Terminal EOL = _]]
            return "EOL"

        if [[Terminal any = _]]
            return "any"

        if [[Terminal string_ = $v(handle)]]
            return "string_"

        if [[Terminal double_ = $v(handle)]]
            return "double_"

    abort("Unreachable")
    return ""


def describe_rule(r: Rule_?): string
    match r.rule
        if [[Rule terminal = $v(term)]]
            return describe_terminal(term)

        if [[Rule nonterminal = $v(nonterm)]]
            let name = nonterm
            return "{name}"

        if [[Rule bound_nonterminal = $v(nonterm)]]
            let name = nonterm._0
            return "{name}"

        if [[Rule text_extraction = $v(subrule)]]
            return describe_rule(subrule)

        if [[Rule seq = $v(rules)]]
            abort("Don't need this")

        if [[Rule alt = $v(alts)]]
            abort("Don't need this")

        if [[Rule maybe_repeat = $v(rule_)]]
            return "*{describe_rule(rule_)}"

        if [[Rule not_rule = $v(rule_)]]
            return "!{describe_rule(rule_)}"

        if [[Rule and_rule = $v(rule_)]]
            return "PEEK({describe_rule(rule_)})"

        if [[Rule repeat = $v(rule_)]]
            return "+{describe_rule(rule_)}"

        if [[Rule subrule = $v(subrule)]]
            abort("Don't need this")

        if [[Rule option = $v(rule_)]]
            return "?{describe_rule(rule_)}"

    abort("Unreachable")
    return ""


def generate_option(var gen: ParserGenerator; var rule_: Rule_?): array<ExpressionPtr>
    var result_handle = rule_.name

    if rule_.name == "" // Rule not bound by name, discard result
        var inscope rule_code <- gen |> with_context("__option__") <|
            return <- gen |> generate(rule_.rule)

        var inscope t <- qmacro_block <|
            parser.suppress_errors |> push <| true
            invoke <|
                $b(rule_code)
                return_skip_lockcheck <| [[auto false, 0, 0]]
                return <- [[auto false, 0, 0]]
            parser.suppress_errors |> pop

        return <- flatten_block(t as ExprBlock)

    // Bind result to the provided name

    set_rule_handle(rule_, "temp_name")
    var inscope type_decl <- gen |> get_rule_type(rule_.rule)
    var inscope result_type <- gen.return_types[gen.current_context] |> clone_type

    var inscope rule_code <- gen |> with_context("__option__") <|
        return <- gen |> generate(rule_.rule)

    rule_code |> append_block_new <| (qmacro_block <| {
        $i(result_handle) |> emplace <| temp_name;
        return_skip_lockcheck <| [[auto false, 0, 0]];
        return <- [[auto false, 0, 0]];
    })

    var inscope t <- qmacro_block <|
        var $i(result_handle) : array<$t(type_decl)>

        parser.suppress_errors |> push <| true
        invoke <|
            $b(rule_code)
        parser.suppress_errors |> pop

    return <- flatten_block(t as ExprBlock)


def with_context(var gen: ParserGenerator; context: string; block_)
    //! Invokes a block of code in the generator context

    let last_context = gen.current_context
    gen.current_context = context

    var inscope r <- invoke(block_)

    gen.current_context = last_context
    return <- r


def generate_one_alternative(var gen: ParserGenerator; var alt: Alternative; i; only)
    let block_name = "blk_{i}"
    let parse_pos = "pos_{i}"
    let result  = "res_{i}"

    var inscope action <- alt.action
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type
    var inscope action_block <- action as ExprBlock |> flatten_block
    var inscope block_contents <- gen |> generate(alt.rule.rule)

    // Add epilogue (code that executes the associated action)

    block_contents |> alternative_add_epilogue(gen, action_block)

    // Generate the alternative

    return <- qmacro_block <|
        var parse_pos = parser.index

        if $v(gen.tracing) && !$v(only)
            parser |> log_info <| "Trying alternative {$v(i)}"

        parser.tabs++

        var cut = false

        var $i(result) <- invoke <|
            $b(block_contents)

        if $i(result).success
            parser.tabs--
            if $v(gen.tracing) && !$v(only)
                parser |> log_success <| "Alternative {$v(i)} successful"
            if $v(gen.debug) && !$v(only)
                parser |> log_plain <|  "State: index: {parser.index}, current: {parser.current}"
            return_skip_lockcheck($i(result))
            return <- $i(result)
        parser.tabs--

        if $v(gen.debug) && !$v(only)
            parser |> log_plain <| "Resetting index to {parse_pos}, current: {parser.current}"

        parser.index = parse_pos

        if cut // Committed but failed; let's fail, too
            return_skip_lockcheck <| [[$t(return_type)]]
            return <- [[$t(return_type)]]




def alternative_add_epilogue(var block_contents: array<ExpressionPtr>; var gen: ParserGenerator; var action_block)
    var inscope action_type <- gen.rule_types[gen.current_context] |> clone_type()
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type()

    var inscope t <- qmacro_block <|

        if $v(gen.debug)
            parser |> log_plain <| "Executing action"

        var val: $t(action_type) <- invoke <| $(): $t(action_type)
            $b(action_block)

        if $v(gen.debug)
            parser |> log_plain <| "Returning result at index {parser.index}"

        var result: $t(return_type) <- [[$t(return_type) true, val, parser.index]]
        return_skip_lockcheck <| result
        return <- result

    block_contents |> append_block_contents <| t


def generate_terminal(var gen: ParserGenerator; var terminal: Terminal): ExpressionPtr
    var name = "_terminal_result_{gen.rule_counter["terminal"]++}"
    var inscope return_type <- gen.return_types[gen.current_context] |> clone_type()

    match terminal
        if [[Terminal lit = $v(l) ]]
            var precomputed_length = l |> length

            return <- qmacro_block <|
                if $v(gen.debug)
                    parser |> log_plain <| "Trying to match terminal {$v(l) |> escape}"

                if !parser |> matches($v(l), $v(precomputed_length))
                    if $v(gen.tracing)
                        parser |> log_fail <| "Did not match the terminal `{$v(l) |> escape}`"
                        parser |> log_fail <| "Suppressing errors :`{parser.suppress_errors |> back}`"
                        parser |> log_fail <| "{parser.index} - {parser.longest_prefix}"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        var error = "Error: Expected the terminal `" |> red_str($v(gen.color_output))
                        var terminal = "{$v(l) |> escape}" |> bold
                        var rest = "` at {parser.index}" |> red_str($v(gen.color_output))
                        var complete_message = "{error}{terminal}{rest}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]


                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

                if $v(gen.tracing)
                    parser |> log_success <| "Matched the terminal `{$v(l) |> escape}`"

        if [[Terminal charset = $v(ranges) ]]

            // Get the currect charachter from the stream
            var inscope rule_code: array<ExpressionPtr>
            rule_code |> append_block_new <| (qmacro_block <| {
                let current_char = parser |> get_current_char;
            })

            for r in ranges // Check in it is inside any of the ranges
                rule_code |> append_block_new <| generate_range_check(r)

            // Otherwise return false
            rule_code |> append_block_new <| (qmacro_block <| {
                return false;
            })

            return <- qmacro_block <|
                var $i(name) = invoke <|
                    $b(rule_code)

                if !$i(name)
                    if $v(gen.tracing)
                        parser |> log_fail <| "Failed the range check for character {parser.index}"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        var error = "Error: Expected charset item `" |> red_str($v(gen.color_output))
                        var terminal = "{$v(terminal |> describe_terminal)}" |> bold
                        var rest = "` at {parser.index}" |> red_str($v(gen.color_output))
                        var complete_message = "{error}{terminal}{rest}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]
                parser|> move(1)

        if [[Terminal log_msg = $v(message) ]]

            return <- qmacro_block <|
                if $v(gen.tracing)
                    // Respect tabulation
                    parser |> log_plain <| "Logged: {$e(message)}"
                else
                    $e(message) |> print

        if [[Terminal number = $v(handle)]]

            return <- qmacro_block <|
                var $i(name) : tuple<bool;int;int> = parser |> match_decimal_literal
                if !$i(name)._0

                    if $v(gen.tracing)
                        parser |> log_fail <| "Could not match a decimal number"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        let complete_message ="Error: Expected a decimal number at {parser.index}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

                if $v(gen.tracing)
                    parser |> log_success <| "Matched a decimal number {$i(name)._1}"

                var $i(handle) = $i(name)._1

        if [[Terminal whitespace = _]]

            return <- qmacro_block <|
                parser |> skip_whitespace

        if [[Terminal taborspace = _]]

            return <- qmacro_block <|
                parser |> skip_taborspace

        if [[Terminal EOF = _]]

            return <- qmacro_block <|
                if !parser |> reached_EOF
                    if $v(gen.tracing)
                        parser |> log_fail <| "Did not match EOF"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        let complete_message = "Error: Expected EOF at {parser.index}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

        if [[Terminal EOL = _]]

            return <- qmacro_block <|
                if !parser |> reached_EOL
                    if $v(gen.tracing)
                        parser |> log_fail <| "Did not match EOL"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        let complete_message = "Error: Expected EOL at {parser.index}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

        if [[Terminal any = _]]

            return <- qmacro_block <|
                if parser |> reached_EOF
                    if $v(gen.tracing)
                        parser |> log_fail <| "Failed matching any, reached EOF"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        let complete_message = "Error: Unexpected EOF at {parser.index}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

                if $v(gen.tracing)
                    parser |> log_success <| "Matched any character"

                parser |> move(1)

        if [[Terminal commit = _]]
            return <- qmacro_block <|
                // Now every failure that checks cut operator will report an error and quit
                cut = true

        if [[Terminal string_ = $v(handle)]]

            return <- qmacro_block <|
                var $i(name) <- parser |> match_string_literal
                if !$i(name).success

                    if $v(gen.tracing)
                        parser |> log_fail <| "Could not match a string"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        let complete_message = "Error: Expected a string at {parser.index}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

                if $v(gen.tracing)
                    parser |> log_success <| "Matched a string {$i(name)._1}"
                var $i(handle) : string = $i(name)._1

        if [[Terminal double_ = $v(handle)]]

            return <- qmacro_block <|
                var $i(name) <- parser |> match_double_literal
                if !$i(name).success

                    if $v(gen.tracing)
                        parser |> log_fail <| "Could not match a double"

                    if parser.error_reporting && !parser.suppress_errors |> back && parser.index == parser.longest_prefix
                        let complete_message = "Error: Expected a double at {parser.index}"
                        parser.errors |> push <| [[ParsingError text=complete_message,index=parser.index]]

                    return_skip_lockcheck <| [[$t(return_type)]]
                    return <- [[$t(return_type)]]

                if $v(gen.tracing)
                    parser |> log_success <| "Matched a double {$i(name)._1}"
                var $i(handle) = $i(name)._1


    abort("Unreachable")
    return <- [[ExpressionPtr]]


def generate_range_check(r)
    if r._0 == r._1
        return <- qmacro_block <|
            return true if current_char == $v(r._0)
    else
        return <- qmacro_block <|
            return true if current_char >= $v(r._0) && current_char <= $v(r._1)


def append_block_new(var old: array<ExpressionPtr>; var new_code: ExpressionPtr)
    for e in (new_code as ExprBlock).list
        old |> emplace <| e
    delete new_code


def append_block_contents(var old: array<ExpressionPtr>; var new_code: ExpressionPtr&)
    for e in (new_code as ExprBlock).list
        old |> emplace <| e
