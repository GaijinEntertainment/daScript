---
title: How to EDSL
date: 2025-06-05 19:56:40
tags:
    - daScript
    - edsl
---

There are ways to ~~die~~ EDSL, and there are ways to ~~die~~ EDSL.

<!-- more -->

This is a bit off tangent, but using the right language to define and solve problem is paramount.
Lets take two extremes: a javascript program to compute a fibonacci sequence, and an equivalent system of polynomials, which when solved produces the same result.
Like seriously see Matiyasevich's Theorem (or MRDP Theorem) for more details. Gotta love those Diophantine equations, but they are not exactly very readable.
Writing them down for the fibonacci would be way harder than the classic example from [the main page of DAS website](https://daslang.io).

If you came from C++, template meta-programming is probably your EDSL tool of choice (sprinkle with preprocessor tricks on as-needed basis).
You'll end up with boost::spirit if you are lucky. That, and clear understanding, that this is not the way.
Perhaps you'll discover other forms of code generation in the process. Something-something->C++ or even C.

If you remember LISP, or played with HAXE, or just know better - then maybe DAS is for you; and in more ways then one.

Lets start with examples. First there were reader macros:

    require daslib/regex_boost

    var r1 <- %regex~[\w\.+-]+@[\w\.-]+\.[\w\.-]+%%

Just like that. It reads random input, it generates bunch of code\data. It can even tell when to stop, although default implementation looks for %%.
Its all in the AstReaderMacro. After all it does not get any more free-form than that.

But wait, there is even more. It can transform text and just output text. Which is _then_ parsed:

    require daslib/spoof

    let template_basic_value = %spoof_template~(tname,tvalue)
    struct %tname {
        value = %tvalue
    }
    %%

    %spoof_instance~template_basic_value(Foo_1,1)%%

Its very strong and flexible. But... we are not here for that. Surely manual sunrise is fun, but automatic one is a lot more practical.
Luckily we have several good examples. Lets start with match:

    require daslib/match

    [sideeffects]
    def static_array_match(A : int[3]) {
        match(A) {
            if (fixed_array($v(a), $v(b), $v(c)) && (a + b + c) == 6) { // total of 3 elements, sum is 6
                return 1
            }
            if (fixed_array<int>(0, ...)) { // starts with 0
                return 0
            }
            if (fixed_array<int>(..., 13)) { // ends with 13
                return 2
            }
            if (fixed_array<int>(12, ..., 12)) { // starts and ends with 12
                return 3
            }
            if (_) {
                return -1
            }
        }
    }

It looks like DAS, but its not exactly it. There is a lot of code-transforming machinery behind 'match' AstCallMacro.
I specifically don't go into implementation details - nothing reads better than code anyway. It's pretty small, too.

And the reason that it's pretty small, is the reification. Turns out 'match' implementation is written in yet another EDSL.
All this qmacro_... struff in 'daslib/templates_boost' is, in fact, another set of macros.

Which in turn are written on top of 'daslib/templates' via replace EDSL.
I'm telling u, its turtles all the way down.

That, and sideways. If you take a look at spoof again - there is argument parser:

    require peg/peg

    [macro_function]
    def spoof_args(invocation : string; blk : block<(var res : array<tuple<string; string>>; errors : array<ParsingError>) : void>) {
        parse(invocation) {
            var element_list : array<tuple<string; string>>
            rule(WS, "(", WS, *comma_separated_elements as els, WS, element as last, WS, ")")  {
                els |> push <| last
                return <- els
            }
            var comma_separated_elements : tuple<string; string>
            rule(WS, element as e, WS, ",")  {
                return e
            }
            var element : tuple<string; string>
            rule(ident_ as e, WS, "=", WS, element_value as v)  {
                return (e, v)
            }
            rule(ident_ as e)  {
                return (e, "")
            }
            var sub_element : string
            rule("\\", "{set(0..255)}" as Ch)  {    // escape
                return string(Ch)
            }
            rule("#", string_ as e, commit)  {      // contents of the string (do we need unescape?)
                return e
            }
            rule(string_ as e, commit)  {           // "e"
                return "\"{e}\""
            }
            rule("(", element as e, ")")  {         // (e)
                return "({e})"
            }
            rule("{not_set(')',',')}" as e)  {      // any character except ')' and ','
                return e
            }
            var element_value : string
            rule(sub_element as head, *sub_element as tail)  {
                return head + tail |> join("")
            }
            var ident_ : string
            rule("{set('a'..'z','A'..'Z','_')}" as fc, "{*set('a'..'z','A'..'Z','_','0'..'9')}" as tail)  {
                return fc + tail
            }
        }
    }

And just like that we are back in the land of boost::spirit. The DAS way. With call macros, reader macros, ast rewrites, and many other tricks.
That, and absolutely no preprocessor.

There are many more tricks up my sleeve. There are generics, operator and property overloads, nameless blocks, and even pass macros.
Sky really is the limit. There is no excuse to keep writing boilerplate code. Imagine how do you want your code to look, and then try to get very close to it.

