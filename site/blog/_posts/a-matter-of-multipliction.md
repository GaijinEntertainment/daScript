---
title: A matter of multiplication
date: 2023-03-20 23:28:56
tags:
    - OpenAI
    - daScript
---

Its a matter of multiplication, really. Productivity factors do not just add up (thats why they are called factors).

<!-- more -->

Programming environment is a collection of multipliers. When you add more, the growth is exponential. The growth of productivity that is.

There is [GitHub Copilot](https://github.com/features/copilot). Its good for C++. It works so well with daScript. It writes a lot of code for me and makes completion awesome.
Seriously, if u did not, yet - give it a try. There is also copilot labs - but it looks too experimental, and does not quite do it justice.
Integration with vscode is watertight.

Speaking of vscode. I tend to write a lot of code in daScript this days. I am many times more productive with it,
so at some point I'll rewrite entire daScript in daScript. The new year resolution was to start moving parts. But then again, JIT happens.
This being said daScript had [vscode plugin](https://marketplace.visualstudio.com/items?itemName=profelis.dascript-plugin), and I don't want to write anything without it. Because reasons.
Reasons like completion, navigation, debugger, and upcoming release of refactoring; because inlays, and most importantly hint overlays.

But we need to go deeper. Like modules/OpenAI deeper. There is a sample there, under the newly created OpenAI daScript bindings.
What it does, it writes entire unit test, given function. It does good job. It even uses `fuzzer`. Only comment and function were provided.


    def strcat(a,b)
        return a + b

    [test] // use any_string and any_file_name for strcat
    def test_strcat ( t:T? )
        t |> run("testing strcat") <| @@ ( t : T? )
            var fake <- Faker()
            fuzz <|
                let a = fake |> any_string(fake)
                let b = fake |> any_file_name(fake)
                t |> equal(a+b, strcat(a,b))

I suspect at some point this and other prompts will make it into plugin. At the end of the day, it's that simple

    def generate_test ( func, description : string ) : string
        var prompt = build_string <| $ ( writer )
            writer |> write(unit_test_prompt) |> write(func) |> write("\n[test] // ") |> write(description) |> write("\ndef test_")
        var ccr <- openai_create_completion([[Completion()
            model = unit_test_model,    prompt = prompt,    max_tokens = 200,
            top_p = 1.0,    temperature = 0.,   frequency_penalty = 0.52,
            presence_penalty = 0.5, best_of = 1, stop <- [{string "// --"; "\}"}]
        ]])
        if ccr |> is_valid
            var choice = ccr.choices[0].text |> replace("\\r","") |> safe_unescape
            return build_string <| $(writer)
                writer |> write("[test] // ") |> write(description) |> write("\ndef test_") |> write(choice)
        else
            to_log(LOG_ERROR, "error: {openai_get_last_error()}\n")
        return ""

The [OpenAI API](https://platform.openai.com/docs/api-reference) was trivial to integrate.
There was just one catch.I wanted it to work with the plugin, regardless of daScript build.
That means I could not use `libhv/libhv` (did you know we have LibHV integration?), or any integration for that matter. It has to be raw daslib.

Fear not. We now have `daslib/curl`. It calls curl. The command line 'curl', via 'popen'. Its surprisingly robust, and if u need it to not block - there is always `jobque`.
I was contemplating async/await API and ultimately decided against it. One day we'll have a real high load use case with coroutine framework; until then lets not overengineer.

We also have `daslib/base64`, because sometimes we get answers in base64. That, and JSON. We had `daslib/json` for a long time.
The implementations for both of them are all very academic, so to speak. I'm writing the simplest daScript code, and surprisingly it runs just fine.
I've yet to encounter any performance issues worth mentioning with all those straightforward implementations.
"Just use good algorithms", like it ever works. So far so good though. Even with interpreter.

Interestingly, JIT likes it. Somehow it seem to favor simple code. Go figure. I'm going to add base64 to the test suit and actually compare.
It's been JIT > AOT all day.

It's amazing to watch them multiply. Once the critical mass has been reached, adding new legos is a breeze.
I can build so many things now. And I can do it fast.




