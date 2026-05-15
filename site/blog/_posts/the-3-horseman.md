---
title: The three horsemen
date: 2026-05-14 20:20:20
tags: daScript
---

The lint apocalypse is coming and you are not prepared.

<!-- more -->

They are lint, perf_lint, and style_lint.

Lint is WRONG.

    x %= 0                      // this is obviously division by zero
    return huh ? blah : blah    // i bet you expected different outcomes

Perf lint is SLOW.

    var s = "["
    for ( x in 0..100 ) {
        if ( x!=0 ) s += ","
        s += "{x}"              // did u know, that this is strlen on growing string
                                // like O(N^2)
    }

Style lint is UGLY.

    var a : array<int>          // its var a <- [1,2,3]
    a |> push(1)                // its also, slow. u forgot a |> reserve(3)
    a |> push(2)
    a |> push(3)

Not much to it, though. There is an MCP lint tool. There is CLI version under /utils/lint.
At some point soon this becomes part of the CI - anything new lints with 0 errors.

I keep adding rules. All I see is rules: every time AI writes something wrong, slow, or ugly.
Then I run it on our codebase and it slaps me in the face - hard. Boris, you wrote this same shit, look here - its all in the history, back in 2024, yellow on black. Its as if they were trained on my shortcomings as an engineer.

Better yet, when u add rules - you can't stop. Here I am - working on dasIMGUI and Claude keeps reusing old imgui_boost. I don't want old. I want new, and shiny, and every function wrapped in telemetry with MCP hooks and validations. So I add imgui_lint - with its first lint error. If you use unwrapped IMGUI - its kaboom time.

But then I read output, and its ugly. I don't want text((text="blah")) cause text("blah") is fine. So I add more rules. Rules specific to how I want my beautiful EDSL to be used, abused, integrated, and otherwise horribly mistreated.

Ugly is not ok. Ugly is long. Ugly is cognitive load. Its hard to read. It wastes time and tokens.

I add red flags. I step back. I see shapes now. Its synergetic.
