options gen2

require dastest/testing_boost
require daslib/strings_boost


[test]
def test_text_match(t : T?) {
    t |> run("AND semantics - every term must match") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "quick fox"), true)
        t |> equal(text_match("the quick brown fox", "quick dog"), false)
        t |> equal(text_match("the quick brown fox", "quick"), true)
        t |> equal(text_match("the quick brown fox", ""), true)
        t |> equal(text_match("the quick brown fox", "   "), true)
    }
    t |> run("Prefix match via *") @(t : T?) {
        t |> equal(text_match("the quick brown fox", "quick*"), true)
        t |> equal(text_match("running jumps", "run*"), true)
        t |> equal(text_match("running jumps", "jump*"), true)
        t |> equal(text_match("running jumps", "walk*"), false)
        t |> equal(text_match("abc", "abcdef*"), false)
        t |> equal(text_match("running jumps", "r* j*"), true)
    }
    t |> run("ASCII case-fold") @(t : T?) {
        t |> equal(text_match("The Quick Brown Fox", "QUICK"), true)
        t |> equal(text_match("the quick brown fox", "QUICK"), true)
        t |> equal(text_match("Running", "run*"), true)
        t |> equal(text_match("RUNNING", "Run*"), true)
    }
    t |> run("Word boundaries split tokens on punctuation") @(t : T?) {
        t |> equal(text_match("hello,world", "hello"), true)
        t |> equal(text_match("hello,world", "world"), true)
        t |> equal(text_match("a.b.c", "b"), true)
        t |> equal(text_match("one-two", "one two"), true)
        t |> equal(text_match("hello, world!", "hello world"), true)
    }
    t |> run("Non-ASCII text passes through tokenizer") @(t : T?) {
        t |> equal(text_match("café latte", "latte"), true)
        t |> equal(text_match("café latte", "café"), true)
        t |> equal(text_match("日本語 テスト", "日本語"), true)
    }
    t |> run("Empty inputs and bare *") @(t : T?) {
        t |> equal(text_match("", ""), true)
        t |> equal(text_match("", "quick"), false)
        t |> equal(text_match("hello world", "*"), true)
    }
    t |> run("Panic on NEAR(") @(t : T?) {
        var panicked = false
        try {
            var _ignored = text_match("a b c", "NEAR(a b)")
        } recover {
            panicked = true
        }
        t |> equal(panicked, true)
    }
    t |> run("Panic on AND / OR / NOT keyword tokens") @(t : T?) {
        var panicked_and = false
        try {
            var _a = text_match("a b c", "a AND b")
        } recover {
            panicked_and = true
        }
        t |> equal(panicked_and, true)
        var panicked_or = false
        try {
            var _o = text_match("a b c", "a OR b")
        } recover {
            panicked_or = true
        }
        t |> equal(panicked_or, true)
        var panicked_not = false
        try {
            var _n = text_match("a b c", "a NOT b")
        } recover {
            panicked_not = true
        }
        t |> equal(panicked_not, true)
    }
    t |> run("Panic on quoted phrase") @(t : T?) {
        var panicked = false
        try {
            var _q = text_match("a b c", "\"a b\"")
        } recover {
            panicked = true
        }
        t |> equal(panicked, true)
    }
    t |> run("Panic on column filter") @(t : T?) {
        var panicked = false
        try {
            var _c = text_match("a b c", "col:term")
        } recover {
            panicked = true
        }
        t |> equal(panicked, true)
    }
    t |> run("Bare NEAR token is also rejected") @(t : T?) {
        var panicked = false
        try {
            var _n = text_match("a b c", "NEAR")
        } recover {
            panicked = true
        }
        t |> equal(panicked, true)
    }
}
