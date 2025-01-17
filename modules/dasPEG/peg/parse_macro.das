options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true
options no_global_variables = false

module parse_macro shared

require daslib/templates_boost
require daslib/strings_boost
require daslib/macro_boost
require daslib/ast_boost
require daslib/defer
require math

require detail/helpers
require detail/colors

require peg/parser_generator
require peg/meta_ast

require math

// To avoid overflow during template expansion
options stack = 10000000

var times_invoked: uint64 = 0 |> uint64()

struct MacroRule
    [[do_not_delete]] rule: ExprLet? // from let rule: type
    options_: array<string>
    alternatives: array<ExprCall?>


[macro_function]
def add_alternative(var rules: array<MacroRule>; var expr: ExprCall?; prog: ProgramPtr)
    var call_name = expr.name |> string()

    if call_name == "rule"
        (rules |> back).alternatives |> emplace <| expr
    elif call_name == "option"
        var opt = (expr.arguments[0] as ExprVar).name |> string()
        (rules |> back).options_ |> emplace <| opt


[macro_function]
def add_name(var rule: Rule; var name: string)
    match rule
        if [[Rule repeat = $v(rep)]]
            rep.name <- name
        if [[Rule option = $v(opt)]]
            opt.name <- name
        if [[Rule maybe_repeat = $v(rep)]]
            rep.name <- name
        if _
            abort("Unreachable")


def into_terminal(var expr: ExpressionPtr): tuple<int;int>
    if expr is ExprConstInt
        // Single character '\n', '1', 'a'

        let v = (expr as ExprConstInt).value
        return <- [[auto v, v]]

    elif expr is ExprCall && (expr as ExprCall).name == "interval"
        // Range of charachters '1'..'9'

        var inscope left_exp <- expr as ExprCall.arguments[0]
        var inscope right_exp <- expr as ExprCall.arguments[1]

        if !left_exp is ExprConstInt || !right_exp is ExprConstInt
            transfrom_error(expr.at, "Range ends should be integer constants")

        var left <- (left_exp as ExprConstInt).value
        var right <- (right_exp as ExprConstInt).value

        return <- [[auto left, right]]

    transfrom_error(expr.at, "Unknown rule passed to `rule` as argument {describe(expr)} into terminal")
    return <- [[auto 0, 0]]

[macro_function]
def flatten_intervals ( charset : array<tuple<from:int;to:int>>; invert:bool )
    var chars : bool[256]

    for r in charset
        for i in max(r.from,0) .. min(r.to + 1,256)
            chars[i] = true

    if invert
        for Ch in chars
            Ch = !Ch

    var result : array<tuple<int;int>>

    var start : int = -1
    for i in 0 .. 256
        if chars[i]
            if start == -1
                start = i
        else
            if start != -1
                result |> push <| [[auto start, i-1]]
                start = -1
    if start != -1
        result |> push <| [[auto start, 256]]

    return <- [[auto chars, result]]

[macro_function]
def into_rule_(var expr: ExpressionPtr): Rule_?
    if expr is ExprAsVariant && (expr as ExprAsVariant).value is ExprVar
        // Bound terminal

        var as_expr <- expr as ExprAsVariant
        var bound_name = as_expr.name |> string()
        var bound_rule = (as_expr.value as ExprVar).name |> string()

        if bound_rule == "number"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal number = bound_name]]]]]]
        elif bound_rule == "double_"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal double_ = bound_name]]]]]]
        elif bound_rule == "string_"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal string_ = bound_name]]]]]]

        return <- new [[Rule_ rule <- [[Rule bound_nonterminal = [[auto bound_rule, bound_name]]]]]]


    elif expr is ExprAsVariant && (expr as ExprAsVariant).value is ExprCall
        // Optional rule (MB) -- zero or one
        var as_expr <- expr as ExprAsVariant
        var subrule = ((expr as ExprAsVariant).value as ExprCall).arguments[0] |> into_rule_
        subrule.name := as_expr.name
        var inscope t <- [[Rule option <- subrule]]
        return <- new [[Rule_ rule <- t, name = as_expr.name |> string()]]

    elif expr is ExprAsVariant && ((expr as ExprAsVariant).value is ExprPtr2Ref)
        var as_expr <- expr as ExprAsVariant

        var subrule = (as_expr.value as ExprPtr2Ref).subexpr |> into_rule_
        subrule.name := as_expr.name
        return <- new [[Rule_ rule <- [[Rule maybe_repeat <- subrule ]]]]


    elif expr is ExprAsVariant && ((expr as ExprAsVariant).value is ExprStringBuilder)
        var as_expr <- expr as ExprAsVariant
        var subrule = (as_expr.value as ExprStringBuilder).elements[0] |> into_rule_
        subrule.name := as_expr.name
        return <- new [[Rule_ rule <- [[Rule text_extraction <- subrule]]]]


    elif expr is ExprAsVariant
        // This way goes *<rule> as <name>,
        //               <rule>? as <name>,
        //               +<rule> as <name>, etc...

        var as_expr <- expr as ExprAsVariant

        var rule: Rule_? <- into_rule_(as_expr.value)
        rule.name = as_expr.name |> string()
        //print("{rule}\n")
        return <- rule


    elif expr is ExprVar
        // Nonbound terminal
        var name = (expr as ExprVar).name |> string()

        if name == "number"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal number = ""]]]]]]
        elif name == "WS"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal whitespace = null]]]]]]
        elif name == "TS"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal taborspace = null]]]]]]
        elif name == "EOF"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal EOF = null]]]]]]
        elif name == "EOL"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal EOL = null]]]]]]
        elif name == "any"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal any = null]]]]]]
        elif name == "commit"
            return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal commit = null]]]]]]
        else
            return <- new [[Rule_ rule <- [[Rule nonterminal <- name]]]]

    elif expr is ExprConstString
        var const_string <- expr as ExprConstString
        return <- new [[Rule_ rule <-
                        [[Rule terminal <-
                          [[Terminal lit = const_string.value |> string()]]]]]]

    elif expr is ExprPtr2Ref
        // Repetition (*) -- zero or more

        var subrule = (expr as ExprPtr2Ref).subexpr |> into_rule_
        return <- new [[Rule_ rule <- [[Rule maybe_repeat <- subrule ]]]]

    elif expr is ExprCall && (expr as ExprCall).name == "MB"
        // Optional rule (MB) -- zero or one

        var subrule = (expr as ExprCall).arguments[0] |> into_rule_
        return <- new [[Rule_ rule <- [[Rule option = subrule ]]]]

    elif expr is ExprCall && (expr as ExprCall).name == "PEEK"
        // Lookahead rule (PEEK) -- match the rule without advancing

        var subrule = (expr as ExprCall).arguments[0] |> into_rule_
        return <- new [[Rule_ rule <- [[Rule and_rule = subrule ]]]]

    elif expr is ExprOp1 && (expr as ExprOp1).op == "!"
        // Negaitve Lookahead rule (!) -- assert unable to match the rule, without advancing

        var subrule = (expr as ExprOp1).subexpr |> into_rule_
        return <- new [[Rule_ rule <- [[Rule not_rule = subrule ]]]]

    elif expr is ExprCall && ((expr as ExprCall).name == "set" || (expr as ExprCall).name == "not_set")
        // Matching set syntax set('1'..'9', 'a'..'z', 'A'..'Z', '_')

        var set = expr as ExprCall
        let inverting = set.name == "not_set"

        var terms: array<tuple<int;int>>

        for arg in set.arguments
            terms |> push (arg |> into_terminal)

        return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal charset <- flatten_intervals(terms, inverting)]] ]] ]]

    elif expr is ExprCall && (expr as ExprCall).name == "log"
        // Log partial matching: log("Here goes your message")

        var log = expr as ExprCall
        var inscope message <- log.arguments[0] |> clone_expression
        return <- new [[Rule_ rule <- [[Rule terminal <- [[Terminal log_msg <- message ]] ]] ]]

    elif expr is ExprOp1 && (expr as ExprOp1).op == "+"
        // Several (+) -- one or more

        var subrule = (expr as ExprOp1).subexpr |> into_rule_
        return <- new [[Rule_ rule <- [[Rule repeat <- subrule ]]]]

    elif expr is ExprConstInt
        transfrom_error(expr.at, "Your rule contains integers (possibly in the form of '_', please use \"\" for literals)")
        return <- new [[Rule_]]

    transfrom_error(expr.at, "Unknown rule passed to `rule` as argument {describe(expr)} into rule")
    return <- new [[Rule_]]

def transfrom_error ( at:LineInfo; message:string )
    macro_error(compiling_program(), at, message)
    failed_to_transform = true

var private failed_to_transform = false

[macro_function]
def transform(var rule: MacroRule): Definition
    unsafe
        failed_to_transform = false
        var name = rule.rule.variables[0].name |> string()
        var inscope type_ <- rule.rule.variables[0]._type

        // Transform array<ExprCall?> into an array of Rules

        var inscope alternatives: array<Alternative>

        for alt in rule.alternatives
            if true // creates scope for inscope
                var inscope blk: ExpressionPtr <- clone_expression(alt.arguments |> back)

                var sequence: array<Rule_?>

                for sequence_elem,_ in alt.arguments,range(length(alt.arguments)-1)
                    sequence |> push <| into_rule_(sequence_elem)

                alternatives |> emplace <| [[Alternative
                                        rule <- new [[Rule_ rule <- [[Rule seq <- sequence]]]],
                                        action <- (blk as ExprMakeBlock)._block]]

        return <- [[Definition name = "{name}",
                    type_ <- type_,
                    rule <- [[Rule alt <- alternatives]]]]


[call_macro(name="parse")]
class ParseMacro : AstCallMacro
    //! Implements `parse` macro.
    //! Usage:
    //!    parse expression
    //!        var add : int
    //!
    //!        rule(add as a, "+", mul as m) <|
    //!            return a + m
    //!
    //!        rule(add as a, "-", mul as m) <|
    //!            return a - m
    //!
    //!        rule(mul as m) <|
    //!            return m
    //!
    //!        var mul: int
    //!        ...
    //!
    //!    let my_var = expresssion("1+2")
    //!
    //! - The pattern is var <rule_name>: <type> followed by a series of calls to `rule` function
    //! - The typing in `var add: int` is significant and will not work otherwise.
    //! - The last argument of the rule function is a semantic action that will be exeuted
    //!     upon the matching of the rule. Names used in actions are defined with
    //!     `<rule> as <name>` syntax. Actions must return the type as defined in let expression.
    //! - By the nature of PEG parsers, in the general case actions are executed nondeterministically.
    //!     THEREFORE, DO NOT PUT STATEFUL ACTIONS in the parser generator. Thank you for your attention.


    def override canVisitArgument ( expr:smart_ptr<ExprCallMacro>; argIndex:int ) : bool
        // if we are reporting errors, and macro did not fold - no point showing errors inside match - there will be too many, and they are meaningless
        return false

    def override canFoldReturnResult ( expr:smart_ptr<ExprCallMacro> ) : bool
        return false

    def override visit ( prog:ProgramPtr; mod:Module?; expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
        g_failed = false // refresh for new errors

        // Verify

        macro_verify(length(expr.arguments) == 2, prog, expr.at, "expecting parse(what) <| block")
        macro_verify(expr.arguments[0] is ExprVar, prog, expr.at, "the first argument to parse should be ExprVar")
        macro_verify(expr.arguments[1] is ExprMakeBlock, prog, expr.at, "parse `block` argument must be immediate block declaration")

        let name = (expr.arguments[0] as ExprVar).name |> string()
        var expression_block = (expr.arguments[1] as ExprMakeBlock)._block as ExprBlock

        macro_verify(length(expression_block.finalList) == 0, prog, expr.at, "parse `block` argument can't have finally section")
        macro_verify(expression_block.list[0] is ExprLet, prog, expr.at, "parse `block` argument should begin with let expression")

        // Collect

        var rules_: array<MacroRule>

        for list_expr in expression_block.list
            if list_expr is ExprLet
                rules_ |> emplace <| [[MacroRule rule = list_expr as ExprLet]]
            elif list_expr is ExprCall
                rules_ |> add_alternative(list_expr as ExprCall, prog)
            else
                macro_verify(false, prog, list_expr.at, "expressions in the parse block should be either let or call")

        // Transform

        var inscope gram: array<Definition>

        var inscope gen <- [[ParserGenerator id = times_invoked++]]

        for mrule in rules_
            gram |> emplace(mrule |> transform)

        if failed_to_transform
            return <- [[ExpressionPtr]]

        // Generate

        for mrule in rules_
            for opt in mrule.options_
                gen |> accept_option(opt) // Set the state of the generator up

        gen |> generate_grammar(gram, name)

        if g_failed
            for err in g_errors
                compiling_program() |> macro_error(expr.at, err)
            g_errors |> clear()
            return <- [[ExpressionPtr]]

        // Emit wrapper code
        var first_method_name = gram[0].name
        let parse_main_func = "parse_{first_method_name}`id_{gen.id}"

        return <- qmacro_block <|
            var inscope parser : $t(gen.parser_type)

            parser.suppress_errors |> push <| false

            if $v(gen.color_output)
                parser.color_output = true

            peek_data($i(name)) <| $ ( inp )
                parser.input := inp

            var result : $t(unsafe(gen.return_types[gram[0].name]))
            result <- $c(parse_main_func)(parser)

            if result.success
                invoke(blk, result.value, [[array<ParsingError>]])
                return

            // Create an error-reporting parser and emit errors

            var inscope error_reporting_parser : $t(gen.parser_type)
            error_reporting_parser.suppress_errors |> push <| false

            peek_data($i(name)) <| $ ( inp )
                error_reporting_parser.input := inp

            error_reporting_parser.error_reporting = true
            error_reporting_parser.longest_prefix = parser.longest_prefix

            result <- $c(parse_main_func)(error_reporting_parser)
            sort_unique(error_reporting_parser.errors)

            invoke(blk, result.value, error_reporting_parser.errors)


[_macro]
def private setup
    if is_compiling_macros_in_module("parse_macro")
        compiling_module() |> add_keyword("parse", false)
