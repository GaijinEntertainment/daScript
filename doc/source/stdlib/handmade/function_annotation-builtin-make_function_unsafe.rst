Makes function unsafe during compilation. This is usually accompanied with the 'static_if'::

    def resize(var Arr:array<auto(numT)>;newSize:int)
	    static_if typeinfo(is_unsafe_when_uninitialized type<numT>)
		    make_function_unsafe()  // make the function unsafe, if the type is unsafe when uninitialized
        ...



