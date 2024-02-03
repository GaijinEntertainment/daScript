require daslib/spoof

let template_xy = %spoof_template~(tname,arga,argb)
class %tname
    x : %arga
    y : %argb
    def %tname(X:%arga;Y:%argb)
        x = X
        y = Y
        debug(self)
%%

%spoof_instance~template_xy(Foo_ii,int,int)%%;
%spoof_instance~template_xy(Foo_ff,float,float)%%;

[export]
def main
    var a = new Foo_ii(1,2)
    var b = new Foo_ff(1.0,2.0)
    print("{a}\n{b}\n")


