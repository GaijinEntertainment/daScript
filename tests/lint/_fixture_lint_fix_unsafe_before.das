//fmt:ignore-file
options gen2
options auto_inline_functions = false

class Base {
    x : int
}

class Derived : Base {
    y : int
}

def twice(a : int) : int => a * 2

def plain_add(a, b : int) : int {
    let c = unsafe(a + b)
    let d = unsafe(a + b) * 2
    return c + d + unsafe(twice(a))
}

def plain_block(a : int) : int {
    var x = a
    unsafe {
        x++
        // a comment rides along
        print("{x}\n")
    }
    return x
}

def block_with_decl(a : int) : int {
    var x = a
    unsafe {
        let y = x + 1
        x = y
    }
    return x
}

def one_line_block(a : int) : int {
    var x = a
    unsafe { x += 2 }
    return x
}

def narrow(a : int) : int {
    var x = a
    unsafe {
        x++
        var p = addr(x)
        *p++
    }
    return x
}

def narrow_last(a : int) : int {
    var x = a
    unsafe {
        x++
        print("{x}\n")
        var p = addr(x)
        *p++
    }
    return x
}

def nested(a : int) : int {
    var x = a
    unsafe {
        var p = addr(x)
        *p++
        unsafe {
            var q = addr(x)
            *q++
        }
    }
    return x
}

def addr_cast(var d : Derived) : int {
    let b = unsafe(reinterpret<Base?>(unsafe(addr(d))))
    return b.x
}

def inert_contract(a : int) : int {
    let p = unsafe(addr(a))
    let q = unsafe(reinterpret<int? -const>(p))
    return *q
}

def kept_comment(a, b : int) : int {
    return unsafe(a /* why */ + b)
}

def kept_brace_comment(a : int) : int {
    var x = a
    unsafe {  // the brace line says something
        x++
    }
    return x
}

def kept_name_reused(a : int) : int {
    var x = a
    unsafe {
        let y = x + 1
        x = y
    }
    let y = 3
    return x + y
}
