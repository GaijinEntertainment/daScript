require dastest/testing_boost public

require daslib/linked_list

class Bar
    a : int
    def Bar ( A : int )
        a = A

%spoof_instance~TLinkedList(LinkedList_Bar,Bar)%%;

[test]
def test_linked_list ( t:T? )
    var list = new LinkedList_Bar()
    list->add(new Bar(1))
    list->add(new Bar(2))
    list->add(new Bar(3))
    for node,i in list->each(), 1..4
        t |> equal ( i, node.a )
