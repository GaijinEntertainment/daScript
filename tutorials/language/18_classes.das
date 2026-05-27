// Tutorial 18: Classes and Inheritance
//
// This tutorial covers:
//   - Defining classes with fields and methods
//   - Constructors and super()
//   - Inheritance with override
//   - Calling parent methods with super.method()
//   - Abstract and sealed methods
//   - Creating instances with 'new'
//   - Static fields and static methods
//   - Private members
//
// Run: daslang.exe tutorials/language/18_classes.das

options gen2

// === Basic class ===
// Classes are like structs but with virtual methods and inheritance.

class Shape {
    name : string

    def Shape(n : string) {
        name = n
    }

    // Abstract method — subclasses must implement
    def abstract area : float

    // Regular method — subclasses inherit or override
    def describe {
        print("  {name}: area = {area()}\n")
    }
}

// === Inheritance ===
// Single-parent inheritance with 'class Derived : Base'

class Circle : Shape {
    radius : float

    def Circle(r : float) {
        super("Circle")       // call parent constructor with super()
        radius = r
    }

    def override area : float {
        return 3.14159 * radius * radius
    }
}

class Rectangle : Shape {
    width : float
    height : float

    def Rectangle(w, h : float) {
        super("Rectangle")    // super() calls Shape`Shape(self, ...)
        width = w
        height = h
    }

    def override area : float {
        return width * height
    }

    // Override describe to add dimensions, but also call parent version
    def override describe {
        super.describe()      // calls Shape`describe(self)
        print("    (w={width}, h={height})\n")
    }
}

// === Private members ===
class Counter {
    private count : int = 0

    def increment {
        count++
    }

    def get_count : int {
        return count
    }
}

// === Static fields and static methods ===
// 'static' fields are shared across all instances.
// 'def static' methods access static fields but not 'self'.

class Tracker {
    static total : int = 0
    id : int

    def Tracker {
        total++
        id = total
    }

    // Static method — no 'self', can only access static fields
    def static getTotal : int {
        return total
    }

    def static reset {
        total = 0
    }
}

[export]
def main {

    // === Creating instances ===

    // 'new' creates a class on the heap, returns a pointer
    var c = new Circle(5.0)
    var r = new Rectangle(4.0, 6.0)

    // Call methods with dot syntax
    c.describe()
    r.describe()    // calls overridden describe → super.describe() + dimensions

    // === Polymorphism ===

    // Base pointer can hold any derived instance
    var shapes <- array<Shape?>(new Circle(3.0), new Rectangle(2.0, 5.0), new Circle(1.0))

    print("all shapes:\n")
    for (s in shapes) {
        s.describe()    // virtual dispatch calls correct area()
    }

    // === Private members ===

    var ctr = new Counter()
    ctr.increment()
    ctr.increment()
    ctr.increment()
    print("counter: {ctr.get_count()}\n")

    // ctr.count = 10  // ERROR: 'count' is private

    // === Static fields and methods ===

    // Static fields are shared across all instances
    var t1 = new Tracker()
    var t2 = new Tracker()
    var t3 = new Tracker()
    print("tracker ids: {t1.id}, {t2.id}, {t3.id}\n")

    // Access static field via backtick syntax
    print("total created: {Tracker`total}\n")

    // Call static methods via backtick syntax
    print("getTotal(): {Tracker`getTotal()}\n")
    Tracker`reset()
    print("after reset: {Tracker`getTotal()}\n")

    // === super() summary ===

    // super() — calls parent constructor:
    //     def Circle(r : float) {
    //         super("Circle")       // → Shape`Shape(self, "Circle")
    //     }
    //
    // super.method() — calls parent method (bypasses virtual dispatch):
    //     def override describe {
    //         super.describe()      // → Shape`describe(self)
    //     }
    //
    // The compiler rewrites super to the explicit Parent`Method(self, ...) form.
    // You can also use the backtick syntax directly:
    //     Shape`Shape(self, "Circle")   // same as super("Circle")

    // === Implicit super() chain — no ctor needed ===

    // If the parent has a user-defined constructor that's callable with no
    // arguments, the compiler synthesizes a default ctor for the derived
    // class that chains super() automatically:
    //
    //   class Animal {
    //       def Animal { print("Animal init\n") }   // 0-arg user ctor
    //   }
    //
    //   class Pet : Animal {
    //       name : string = "rex"
    //   }
    //
    //   new Pet()    // prints "Animal init" — synth ctor calls super()
    //
    // The synth runs only when:
    //   - the parent has a user-defined ctor (any signature)
    //   - the derived class has NO ctor of its own (none at all)
    //   - that parent ctor is 0-arg-callable (or all args have defaults)
    //
    // Defining ANY user ctor — even one that only takes arguments —
    // SUPPRESSES the 0-arg synth. `new Pet()` then errors unless the user
    // also declares `def Pet()`:
    //
    //   class Pet : Animal {
    //       def Pet(name : string) { super(); this.name = name }
    //   }
    //   new Pet()             // ERROR: no matching ctor _::Pet()
    //   new Pet("rex")        // OK
    //
    // If the parent has only ctors that require arguments, the derived class
    // MUST declare its own ctor:
    //
    //   class A { def A(val : int) { /* ... */ } }
    //   class B : A {}                  // ERROR: no 0-arg parent ctor to chain
    //   class B : A { def B { super(0) } }   // OK

    // === super() must be called exactly once per path ===

    // When the parent has a user-defined constructor, the lint requires
    // every control-flow path through the derived ctor to call super(...)
    // exactly once:
    //
    //   class Bad : Base {
    //       def Bad(f : bool) {
    //           if (f) {
    //               super()       // ERROR: false branch has zero super() calls
    //           }
    //       }
    //   }
    //
    //   class AlsoBad : Base {
    //       def AlsoBad {
    //           super()
    //           super()           // ERROR: super() called more than once
    //       }
    //   }
    //
    // Both if/else branches calling super is fine — the lint sees min=max=1:
    //   class OK : Shape {
    //       def OK(big : bool) {
    //           if (big) { super("Big") } else { super("Small") }
    //       }
    //   }

    // === super.X() may not skip an intermediate with its own ctor ===

    //   class A { def A { /* ... */ } }
    //   class B : A { def B { super() } }     // B has its own ctor
    //   class C : B {
    //       def C { super.A() }               // ERROR: would skip B's invariants
    //   }
    //
    // Empty intermediates (no user ctor) are still legal to skip — that's
    // the SkipMid pattern in tests/language/super.das.

    // === Summary ===

    // - 'class' defines a type with virtual methods
    // - 'abstract' methods must be implemented by subclasses
    // - 'override' replaces a parent method
    // - 'sealed' prevents further overriding
    // - 'new ClassName(args)' creates an instance (returns pointer)
    // - 'super()' calls the parent constructor
    // - 'super.method()' calls a parent method explicitly
    // - 'private' restricts access to class internals
    // - 'static' fields are shared; 'def static' methods access them
    // - Call static methods: ClassName`methodName()
    //
    // Note: structs can also have methods via [class_method] annotation
    // from daslib/class_boost. It adds an implicit 'self' parameter to
    // 'def static' functions, enabling obj.method() syntax on structs.
    // Generic static methods (with `auto` parameters) are also supported —
    // the macro rewrites the template once and every instantiation inherits
    // the injected `self`. See tests/language/generic_class_method.das.
}

// output:
//   Circle: area = 78.53975
//   Rectangle: area = 24
//     (w=4, h=6)
// all shapes:
//   Circle: area = 28.274311
//   Rectangle: area = 10
//     (w=2, h=5)
//   Circle: area = 3.14159
// counter: 3
// tracker ids: 1, 2, 3
// total created: 3
// getTotal(): 3
// after reset: 0
