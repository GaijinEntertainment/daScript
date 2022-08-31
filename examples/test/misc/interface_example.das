require daslib/interfaces

[interface]
class ITick
    def abstract beforeTick : bool
    def abstract tick ( dt:float ) : void
    def abstract afterTick : void

[interface]
class ILogger
    def abstract log ( message : string ) : void

[implements(ITick),implements(ILogger)]
class Foo
    def Foo
        pass
    def ITick`tick ( dt:float )
        print("tick {dt}\n")
    def ITick`beforeTick
        print("beforeTick\n")
        return true
    def ITick`afterTick
        print("afterTick\n")
    def ILogger`log ( message : string )
        print("log {message}\n")


[export]
def main
    print("hello, world\n")
    var f = new Foo()

    f->get_ITick()->beforeTick()
    f->get_ITick()->tick(1.0)
    f->get_ITick()->afterTick()
    f->get_ILogger()->log("hello")

    unsafe
        delete f

options log
