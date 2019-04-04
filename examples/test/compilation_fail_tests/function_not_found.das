expect 30304    // function_not_found

require UnitTest

def testFoo(a:TestObjectFoo)
    a.fooData = 13

def callFoo
    let foo:TestObjectFoo
    testFoo(foo)            //  two candidates ::testFoo and UnitTest::testFoo
    ::testFoo(foo)
    UnitTest::testFoo(foo)
