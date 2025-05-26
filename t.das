require daslib/ast_boost
require daslib/macro_boost

[export]
def main() {
unsafe {
    let x <- quote(@(){
        let x = 1 + 3;
        doom($(){
                let y = 1 + 3;
                let yy <- new ExprCall(name:="foo2", arguments:=array<ExpressionPtr>(
                                                                           new ExprConstInt(value=1),
                                                                           new ExprConstInt(value=2)
                                                                       ));

                return 321;
        })
        return 321;
    });
    let y <- quote2($(){
        let x = 1 + 3;
        doom($(){
                let y = 1 + 3;
                let yy <- new ExprCall(name:="foo2", arguments:=array<ExpressionPtr>(
                                                                           new ExprConstInt(value=1),
                                                                           new ExprConstInt(value=2)
                                                                       ));

                return 321;
        })
        return 321;
    });
    let res = 123;
    let z = @capture(& res)(a:int) { return 123; }

    // let xx = dasvector`smart_ptr`Expression();
    // tst |> emplace(reinterpret<ExpressionPtr>(new ExprConstInt(value=123)));
    // let x = $(a) { return 123; };
    let yy <- new ExprCall(name:="foo2", arguments:=array<ExpressionPtr>(
                                                               new ExprConstInt(value=1),
                                                               new ExprConstInt(value=2)
                                                           ));
    // let yy <- new ExprOp2(at=LineInfo(), op:="+", left<-new ExprConstInt(value=123), right<-new ExprConstInt(value=123));
    print("{describe(y)}\n\n\n\n{describe(x)}\n{z}");
}
}
