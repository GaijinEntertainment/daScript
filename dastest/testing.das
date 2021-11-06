options indenting = 4

module testing shared

require rtti
require strings

class T
    name: string
    onLog: lambda<(msg: string; at: LineInfo): void>
    onFail: lambda<(now: bool): void>
    onSkipNow: lambda<(): void>
    onRun: lambda<(name: string; fn: function<(t: T?): void>): void>

    failed: bool = false
    skipped: bool = false

    def T(name_: string)
        name = name_

    def const failNow()
        onFail |> invoke(true)
        panic("test failed")

    def const fail()
        onFail |> invoke(false)

    def const error(msg: string)
        errorAt(msg, get_line_info(1))

    def const errorAt(msg: string; at: LineInfo)
        logAt(msg, at)
        fail()

    def const fatal(msg: string)
        fatalAt(msg, get_line_info(1))

    def const fatalAt(msg: string; at: LineInfo)
        logAt(msg, at)
        failNow()

    def const log(msg: string)
        logAt(msg, get_line_info(1))

    def const logAt(msg: string; at: LineInfo)
        onLog |> invoke(msg, at)

    def const skipNow()
        onSkipNow |> invoke()
        panic("test skipped")

    def const skip(msg: string)
        skipAt(msg, get_line_info(1))

    def const skipAt(msg: string; at: LineInfo)
        logAt(msg, at)
        skipNow()

    def const run(sub_name: string; func: function<(t: T?): void>)
        onRun |> invoke(sub_name, func)


def equal(t: T?; a; b; msg = "")
    if a != b
        t->errorAt("{!empty(msg) ? msg : "values differ"}\n\texpected: {a}\n\tgot: {b}", get_line_info(1))


def success(t: T?; a; msg = "")
    if !a
        t->errorAt(!empty(msg) ? msg : "expected success, got failure", get_line_info(1))