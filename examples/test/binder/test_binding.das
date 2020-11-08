require generatedBindings

[export]
def test
    // enums
    assert(FirstEnum FirstEnum_one == FirstEnum FirstEnum_one)
    assert(FirstEnum FirstEnum_one != FirstEnum FirstEnum_two)
    assert(SecondEnum SecondEnum_one == SecondEnum SecondEnum_one)

    // structs
    let struct1 = [[FirstStruct bool_field=true]];
    let struct2 = [[SecondStruct bool_field=false]];
    assert(struct1.bool_field == !struct2.bool_field);

    return true
