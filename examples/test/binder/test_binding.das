require generatedBindings

[export]
def test
    assert(FirstEnum FirstEnum_one == FirstEnum FirstEnum_one)
    assert(FirstEnum FirstEnum_one != FirstEnum FirstEnum_two)

    assert(SecondEnum SecondEnum_one == SecondEnum SecondEnum_one)

    return true
