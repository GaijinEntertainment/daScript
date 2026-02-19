Implements `parse` macro.
Usage:
   parse expression
       var add : int

       rule(add as a, "+", mul as m) <|
           return a + m

       rule(add as a, "-", mul as m) <|
           return a - m

       rule(mul as m) <|
           return m

       var mul: int
       ...

   let my_var = expresssion("1+2")

- The pattern is var <rule_name>: <type> followed by a series of calls to `rule` function
- The typing in `var add: int` is significant and will not work otherwise.
- The last argument of the rule function is a semantic action that will be exeuted
    upon the matching of the rule. Names used in actions are defined with
    `<rule> as <name>` syntax. Actions must return the type as defined in let expression.
- By the nature of PEG parsers, in the general case actions are executed nondeterministically.
    THEREFORE, DO NOT PUT STATEFUL ACTIONS in the parser generator. Thank you for your attention.
