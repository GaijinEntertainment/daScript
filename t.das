require daslib/ast_boost
require daslib/macro_boost

[export]
def main() {
    var inscope x <- quote2(@capture(& a, & b) (c : array<int>; d : vector`int){
        let c = [123, 321, 123];
        let x = 234 ? 123 : dsa;
        let y = $(a : int) { let z = 123 + foo(123, "asd" + 123); };
        if (x > c) {
            print("hey {a}");
        } else {
            let z = quote2($(a : int) { let z = 123 + foo(123, "asd" + 123); });
        }
        return <- struct<$t(blobHandleStruct)>(provider = self.registry |> getProvider(), resUid = self.resUid);
    });
    var inscope y <- quote(@capture(& a, & b) (c : array<int>; d : vector`int){
        let c = [123, 321, 123];
        let x = 234 ? 123 : dsa;
        let y = $(a : int) { let z = 123 + foo(123, "asd" + 123); };
        if (x > c) {
            print("hey {a}");
        } else {
            let z = quote2($(a : int) { let z = 123 + foo(123, "asd" + 123); });
        }
        return <- struct<$t(blobHandleStruct)>(provider = self.registry |> getProvider(), resUid = self.resUid);
    });
    print("{describe(y)}\n\n{describe(x)}\n");
}
