options indenting = 4

module testing shared


class T
    name: string
    onLog: lambda<(msg: string): void>
    onFail: lambda<(): void>
    onFailNow: lambda<(): void>
    onSkipNow: lambda<(): void>

    failed: bool = false
    skipped: bool = false

    def T(name_: string)
        name = name_

    def const failNow()
        onFailNow |> invoke()
        panic("test failed")

    def const fail()
        onFail |> invoke()

    def const error(msg: string)
        log(msg)
        fail()

    def const fatal(msg: string)
        log(msg)
        failNow()

    def const log(msg: string)
        onLog |> invoke(msg)

    def const skipNow()
        onSkipNow |> invoke()
        panic("test skipped")

    def const skip(msg: string)
        log(msg)
        skipNow()


def equal(t: T?; a; b)
    if a != b
        t->error("values differ\n\texpected: {a}\n\tgot: {b}")


def success(t: T?; a)
    if !a
        t->error("expected success, got failure")