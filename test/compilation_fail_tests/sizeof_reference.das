expect 39901    //  sizeof_reference

def test
    sizeof(type int&)   // can't have sizeof(ref...) because its meaningless
