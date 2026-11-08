options gen2

require dastest/testing_boost public
require daslib/spoof
require daslib/linked_list   // nolint:STYLE030 - TLinkedList is reached through the spoof_instance reader macro

let template_basic_value = %spoof_template~(tname,tvalue)
struct %tname {
    value = %tvalue
}
%%

let VALUE = 2

%spoof_instance~template_basic_value(Foo_1,1)%%         // number
%spoof_instance~template_basic_value(Foo_2,VALUE)%%     // literal
%spoof_instance~template_basic_value(Foo_3,(2+1))%%     // expression in brackets
%spoof_instance~template_basic_value(Foo_4,"hello\n")%%   // string
%spoof_instance~template_basic_value(Foo_5,#"VALUE")%%  // unquoted literal
%spoof_instance~template_basic_value(Foo_6,\V\A\L\U\E + 1)%%  // escaped literal

[test(description = "a spoof template instantiates structs whose field value comes from its template arguments")]
def test_basic_value(t : T?) {
    let foo1 = Foo_1()
    t |> equal(1, foo1.value)
    let foo2 = Foo_2()
    t |> equal(VALUE, foo2.value)
    let foo3 = Foo_3()
    t |> equal(3, foo3.value)
    let foo4 = Foo_4()
    t |> equal("hello\n", foo4.value)
    let foo5 = Foo_5()
    t |> equal(VALUE, foo5.value)
    let foo6 = Foo_6()
    t |> equal(VALUE + 1, foo6.value)
}

class Bar {
    a : int
    def Bar(A : int) {
        a = A
    }
}

%spoof_instance~TLinkedList(LinkedList_Bar,Bar)%%

[test(description = "a linked list instantiated through spoof_instance adds and walks its nodes")]
def test_linked_list(t : T?) {
    var list = new LinkedList_Bar()
    list->add(new Bar(1))
    list->add(new Bar(2))
    list->add(new Bar(3))
    for (node, i in list->each(), 1..4) {
        t |> equal(i, node.a)
    }
}
