expect 30303    // operator_not_found
expect 30504:2  // cant_write_to_const
expect 30505    // cant_move_to_const

def const_iter(a:table<string,int>)
    for k,v in keys(a),values(a)
        k = "foo"   // can't write to constant value
        v = 1       // this one okay

def add(a:const int&; b:int)
    a += b      // no matching operator

def set(a:const string&; b:string)
    a = b       // can't write to const

def move_ar(a:const array<int>;b:array<int>)
    a <- b      // can't move to const




