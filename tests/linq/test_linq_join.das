options gen2
require daslib/linq
require dastest/testing_boost public
require math

require daslib/linq

require _common

[test]
def test_join(t : T?) {
    t |> run("basic join") <| @(t : T?) {
        var query = join(
            persons.to_sequence(),
            pets.to_sequence(),
            $(p : Person) => p,
            $(pet : Pet) => pet.owner,
            $(p : Person, pet : Pet) => (p.name, pet.name)
        )
        var expected = [
            ("Hedlund, Magnus", "Daisy"),
            ("Adams, Terry", "Barley"),
            ("Adams, Terry", "Boots"),
            ("Weiss, Charlotte", "Whiskers")
        ]
        for (c, e in query, expected) {
            t |> equal(c._0, e._0)
            t |> equal(c._1, e._1)
        }
    }
    t |> run("complex join") <| @(t : T?) {
        var qcomplex = join(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0],
            $(c : ComplexType) => c.a[0],
            $(c1 : ComplexType, c2 : ComplexType) => ComplexType(a = [c1.a[0], c2.a[1]])
        )
        var expected_complex = [
            (0, 0),
            (1, 10),
            (2, 20),
            (3, 30),
            (4, 40)
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c.a[0], e._0)
            t |> equal(c.a[1], e._1)
        }
    }
}

[test]
def test_groupjoin(t : T?) {
    t |> run("basic group join") <| @(t : T?) {
        var query = group_join(
            persons.to_sequence(),
            pets.to_sequence(),
            $(p : Person) => p.name,
            $(pet : Pet) => pet.owner.name,
            $(p : Person, var pets : iterator<Pet>) => (p.name, pets.to_array())
        )
        var expected = [
            ("Hedlund, Magnus", [daisy]),
            ("Adams, Terry", [barley, boots]),
            ("Weiss, Charlotte", [whiskers])
        ]
        for (c, e in query, expected) {
            t |> equal(c._0, e._0)
            for (p1, p2 in c._1, e._1) {
                t |> equal(p1.name, p2.name)
                t |> equal(p1.owner.name, p2.owner.name)
            }
        }
    }
    t |> run("complex group join") <| @(t : T?) {
        var qcomplex = group_join(
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            [iterator for(x in 0..5); ComplexType(a = [x, x * 10])],
            $(c : ComplexType) => c.a[0],
            $(c : ComplexType) => c.a[1] / 10,
            $(c : ComplexType, var pets : iterator<ComplexType>) => (c.a[0], pets.to_array())
        )
        var expected_complex = [
            (0, [ComplexType(a = [0, 0])]),
            (1, [ComplexType(a = [1, 10])]),
            (2, [ComplexType(a = [2, 20])]),
            (3, [ComplexType(a = [3, 30])]),
            (4, [ComplexType(a = [4, 40])])
        ]
        for (c, e in qcomplex, expected_complex) {
            t |> equal(c._0, e._0)
            for (p1, p2 in c._1, e._1) {
                t |> equal(p1.a[0], p2.a[0])
                t |> equal(p1.a[1], p2.a[1])
            }
        }
    }
}

