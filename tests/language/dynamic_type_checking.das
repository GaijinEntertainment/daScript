options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false
options rtti

require dastest/testing_boost public
require daslib/dynamic_cast_rtti

class Shape {
    def abstract const area() : float
}

class Circle : Shape {
    radius : float = 1.0
    def override const area() : float {
        return 3.14159 * radius * radius
    }
}

class Rectangle : Shape {
    width : float = 1.0
    height : float = 1.0
    def override const area() : float {
        return width * height
    }
}

class Square : Rectangle {
    def Square(s : float) {
        self.width = s
        self.height = s
    }
}

[test]
def test_is_instance_of(t : T?) {
    t |> run("circle is Circle") <| @(t : T?) {
        unsafe {
            var inscope c = new Circle()
            t |> equal(is_instance_of(c, type<Circle>), true)
            t |> equal(is_instance_of(c, type<Shape>), true)
            t |> equal(is_instance_of(c, type<Rectangle>), false)
        }
    }
    t |> run("square is Rectangle and Shape") <| @(t : T?) {
        unsafe {
            var inscope sq = new Square(5.0)
            t |> equal(is_instance_of(sq, type<Square>), true)
            t |> equal(is_instance_of(sq, type<Rectangle>), true)
            t |> equal(is_instance_of(sq, type<Shape>), true)
            t |> equal(is_instance_of(sq, type<Circle>), false)
        }
    }
}

[test]
def test_dynamic_type_cast(t : T?) {
    t |> run("cast Shape? to Circle?") <| @(t : T?) {
        unsafe {
            var inscope s : Shape? = new Circle()
            var c = dynamic_type_cast(s, type<Circle>)
            t |> equal(c != null, true)
            var r = dynamic_type_cast(s, type<Rectangle>)
            t |> equal(r == null, true)
        }
    }
    t |> run("cast to base type succeeds") <| @(t : T?) {
        unsafe {
            var inscope sq : Shape? = new Square(3.0)
            var asRect = dynamic_type_cast(sq, type<Rectangle>)
            t |> equal(asRect != null, true)
        }
    }
}

[test]
def test_force_dynamic_type_cast(t : T?) {
    t |> run("force cast succeeds on correct type") <| @(t : T?) {
        unsafe {
            var inscope s : Shape? = new Circle()
            var c = force_dynamic_type_cast(s, type<Circle>)
            t |> equal(c != null, true)
        }
    }
}

[test]
def test_is_as_syntax(t : T?) {
    t |> run("is syntax") <| @(t : T?) {
        unsafe {
            var inscope s : Shape? = new Circle()
            t |> equal(s is Circle, true)
            t |> equal(s is Rectangle, false)
        }
    }
    t |> run("as syntax") <| @(t : T?) {
        unsafe {
            var inscope s : Shape? = new Circle()
            var c = s as Circle
            t |> equal(c != null, true)
        }
    }
}

[test]
def test_polymorphic_area(t : T?) {
    t |> run("area via const method") <| @(t : T?) {
        unsafe {
            var inscope c : Shape? = new Circle()
            t |> equal(c->area() > 3.14, true)
            t |> equal(c->area() < 3.15, true)
        }
    }
    t |> run("square area via inheritance") <| @(t : T?) {
        unsafe {
            var inscope sq : Shape? = new Square(4.0)
            t |> equal(sq->area(), 16.0)
        }
    }
}
