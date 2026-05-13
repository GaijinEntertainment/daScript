---
title: Of pipes and blocks
date: 2024-09-17 22:16:07
tags:
    - daScript
---

Das is not a write-only code. The idea is we read more than we write.
There is "write as you speak", but there is also "read as you listen".

<!-- more -->

Picture this

    verify_if_even(map(each(range(50)), @(x:int) => x+x ))

And this

    (       range(50)
        |>  each()
        |>  map( @(x : int) => x + x )
        |>  verify_if_even()
    )

I can read both. First one is hard. I need to flip it in my head first. Second one happens in the order its written.
Welcome to pipes.

Lets lay some more pipes

    add_element(some_collection, 12)
    some_collection |> add_element(12)

I can read both. First one is ever so familiar. Second one makes sense a lot more.

Even more pipes

    field.init |> move_new <| clone(expr.init)

It's a verb. Its a bit obscure, but at least I don't have to guess what we are moving to where.

Left pipe stacks. Plus we can always skip empty bracers

    take_3_elements <| 1 <| 2 <| 3

We can get silly too

    1 |> take_3_elements <| 2 <| 3

Speaking of left pipe. Its mainly there for blocks.
Ruby blocks were an initial inspiration, but this days its more common than not.
So lets pipe some blocks

    get_component(renderSettings) <| $(var shadow : ShadowSettings)
        shadow.powWeight = 0.6

Or better yet

    renderSettings |> get_component <| $(var shadow : ShadowSettings)
        shadow.powWeight = 0.6

Lets read it. We take renderSettings. We get it's component, which is shadow of type ShadowSettings, and then we change powWeight.

Almost good. Almost. Every time there is a right pipe and then block - we know what comes next the moment we see that pipe.
Its a block. Its always a block. Actually, sometimes it's lambda, or local function. But you got the gist.
So lets make it even better

    renderSettings |> get_component $(var shadow : ShadowSettings)
        shadow.powWeight = 0.6

Notice, no left pipe. This is new. Perhaps old syntax will go away entirely at some point. Old initialization sure will.

We can pass types

    some_type_function(type<int>) $ ...
    type<int> |> some_type_function() $ ...

But we can make it a type-function

    [type_function]
    def some_type_function ( t : auto(TT); ...

    some_type_function<int> $ ...

Its the same thing. Only not really. Because 'some_type_function' becomes keyword, and can no longer be specialized. But it sure looks cool.
It can take multiple types

    some_other_type_function_which_takes<int;float>(and,the,rest,of,the,arguments) <| of \
        <| which <| some <| are <| piped

Just like you say it.






