options gen2

require daslib/interfaces

[interface]
class ITick {
    def abstract beforeTick : bool
    def abstract tick(dt : float) : void
    def abstract afterTick : void
}

[interface]
class ILogger {
    def abstract log(message : string) : void
}

[implements(ITick), implements(ILogger)]
class Foo {
    def Foo() {
        pass
    }
    def ITick`tick(dt : float) {
        print("tick {dt}\n")
    }
    def ITick`beforeTick() {
        print("beforeTick\n")
        return true
    }
    def ITick`afterTick() {
        print("afterTick\n")
    }
    def ILogger`log(message : string) {
        print("log {message}\n")
    }
}

[export]
def main() {
    var f = new Foo()

    // direct getter calls (original pattern)
    f->get`ITick()->beforeTick()
    f->get`ITick()->tick(1.0)
    f->get`ITick()->afterTick()
    f->get`ILogger()->log("hello")

    // is — compile-time interface check
    print("f is ITick  = {f is ITick}\n")   // true
    print("f is ILogger = {f is ILogger}\n") // true

    // as — get the interface proxy
    var tick = f as ITick
    tick->tick(2.0)

    // ?as — safe interface access (null if pointer is null)
    var logger = f ?as ILogger
    if (logger != null) {
        logger->log("safe access")
    }

    // ?as on null pointer
    var nothing : Foo?
    var maybe_tick = nothing ?as ITick
    print("null ?as ITick = {maybe_tick}\n")  // null

    unsafe {
        delete f
    }
}

