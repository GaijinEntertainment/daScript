require tutorial_08

require math

[export]
def get_pi_ptr
    return unsafe(addr(PI))

[export]
def test
    print("this tutorial utilizes basic builin module with constant and function\n")
    let sq2 = SQRT2
    print("sq2 = {sq2} // expecting sqrt(2), 1.41421\n")
    let xma = xmadd(sq2,sq2,1.0,1.0)    // sq2*sq2 + 1*1 => 3
    print("xma = {xma} // expecting 3\n")
    print("testing get_pi_ptr = {get_pi_ptr()}\n")
    return true


