---
title: Wake up and test the damn thing
date: 2023-02-25 18:44:19
tags: daScript
---

It has to be stable. The time has come to test the damn thing.

<!-- more -->

Project can live on its own use cases for a long time, especially if it piggybacks on a bunch of other projects with the big boy processes in place.
Its particularly true for the compiler, or the interpreter, or whatever daScript is this days.

Large codebase happens to also be a large collection of fairly specific user scenarios and coding styles.
Supplemented with measurable amount of basic tests it accounts for reasonable workflow.
Spice it up with some limited self hosting for good measure.
Add an evangelist or better yet an Apostle to merge the damn thing and you got yourself a winner.
Until you don't that is.

Now to get to the next level one needs a solid foundation. It all started with proper testing framework.
Ironically that immediately created bunch of technical debt, namely "lets take old tests and move them to the new framework".
Which of cause we have not. Yet.

Here is a taste. It showcases `testing_boost` itself, with some limited fuzzy testing (`fuzzer`) and basic data faking (`faker`).

    require dastest/testing_boost public
    require daslib/fuzzer
    require daslib/faker

    [test]
    def test_bit_counting_everything ( t : T? )
        ...
        t |> run("popcnt") <| @ ( t : T? )
            var fake <- Faker()
            fuzz <|
                var x = fake |> random_uint()
                var bits = x
                var count = 0u
                while bits != 0u
                    bits &= bits - 1u
                    count ++
                t |> equal ( count, popcnt(x) )

The idea is to cover entire language with a basic functionality test, as well as fuzzy tests for just about everything testable in `daslib`.
[We are making good progress.](https://github.com/GaijinEntertainment/daScript/tree/master/tests)

Then came the JIT. As I was writing it, I was writing a test for every single JIT feature there was, be that constructs, intrinsic functions, or annotations.
Turns out JIT is a very good _cover entire language_ estimator. It just about covers it.

[For now JIT tests live next to JIT,](https://github.com/borisbat/dasLLVM/tree/main/tests) but onces fully featured they'll live as part of the main big set of tests.
We'll run them with and without JIT, with and without AOT.

The test is not a test when it does not run every single time something changes. So it's been integrated with the pull-request process.
The process is not the process if it does not run every single time. So now I do pull requests for everything I check in.
And then it gets built. For every reasonable platform, including WASM. They run tests, and if nothing fails - I merge.

If the change is significant I run code analyzer. In fact we run several. There are couple on the piggyback projects.
So far there are two which we have integrated as part of Github infrastructure (CodeQL and Microsoft C++ Code Analysis).
Every now and then I run them just because. They've made a lot of noise, they've also pointed at some very real issues.

On top of all that there is daScript runtime itself. Linter gets bigger every day (did u know we have lint macros? of cause we do)
We now verify CMRES aliasing, and we are about to verify array and table argument aliasing.
Runtime checks locks. With the right options runtime checks lifetime of smart_ptr. If we don't catch it early, we will.

It is time to finish JIT. But it is also time to test the damn thing.
It has to be fast. It has to be stable. Its inevitable.










