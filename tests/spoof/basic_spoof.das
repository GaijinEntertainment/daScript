require dastest/testing_boost public

require daslib/spoof

let template_basic_value = %spoof_template~(tname,tvalue)
struct %tname
    value = %tvalue
%%

let VALUE = 2

%spoof_instance~template_basic_value(Foo_1,1)%%;         // number
%spoof_instance~template_basic_value(Foo_2,VALUE)%%;     // literal
%spoof_instance~template_basic_value(Foo_3,(2+1))%%;     // expression in brackets
%spoof_instance~template_basic_value(Foo_4,"hello\n")%%;   // string
%spoof_instance~template_basic_value(Foo_5,#"VALUE")%%;  // unquoted literal

[test]
def test_basic_value ( t:T? )
    let foo1 = Foo_1()
    t |> equal(1,foo1.value)
    let foo2 = Foo_2()
    t |> equal(VALUE,foo2.value)
    let foo3 = Foo_3()
    t |> equal(3,foo3.value)
    let foo4 = Foo_4()
    t |> equal("hello\n",foo4.value)
    let foo5 = Foo_5()
    t |> equal(VALUE,foo5.value)


