require UnitTest

expect 30301:2    // type_not_found

struct TestObjectFoo
    a:int

def test
    new Foo                         // not found
    new TestObjectFoo               // 2 candidates UnitTest::TestObjectFoo and ::TestObjectFoo
    new UnitTest::TestObjectFoo     // this one ok
    new ::TestObjectFoo             // this one ok too



