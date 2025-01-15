def acall(a:lambda<():void>) {
    invoke(a);
}

[export]
def main() {

    unsafe {
        var arr = array<float2>() ;
        acall(@(){
            let v = float2(0,1);  // it is ok, if not comment~~
            arr.push(v);            // it is ok, if not comment~~
            arr.push(float2(0,2));
        });
    }

}