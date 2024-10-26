/*
	NOTE - this is WIP example of every single feature new syntax has to offer
	new syntax no longer supports significant whitespace, oxoford commas, etc
*/

/////////////////////
// module declaration

// note - this works, but commented out for now, becasue of WARNING: program is setup as both module, and endpoint
// module hello_world;	// module name

/////////////////////
// struct declaration

struct Foo {
    a : int = 5;
	b : int;
}

struct Bar : Foo {	// structure with inheritance
	c : float;
}

[safe_when_uninitialized]	// structure annotation
struct Baz : Bar {			// structure with methods
	def Baz {
		c = 3.14;
	}
	def add ( value:int ) {
		a += value;
	}
}

class Cls {			// class with methods
	<comment> foobar : int;						// note field annotation TOOD: IS THIS WHAT WE ARE DOING HERE?
	static foobars : float;
	def foo {
		debug("foo");
	}
	def private private_method ( m : int ) {	// todo: make it `private def static_method`
		debug(m);
	}
	def static static_method {
		debug("static_method");
	}
	def abstract abstract_method ( a : int ) : float;
	[unsafe_deref]								// note function annotation
	def base_method {
	}

}

class DerivedClass : Cls {	// class with inheritance
	def override base_method {
		debug("bar");
	}
	def override abstract_method ( a : int ) : float {
		return float(a);
	}
}

[export]
def main {
	var f <- {1=>"a", 2=>"b", 3=>"c", 4=>"d"};
	debug(f);
}

options log;


