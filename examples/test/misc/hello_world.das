
[export]
def main
	var a <- array(1, 2, 3, 4, 5)
	print("a = {a}\n")

	var b <- array<int>(5)
	print("b = {b}\n")

	var aa = fixed_array(1., 2., 3., 4.5)
	print("aa = {aa}\n")

	var bb = fixed_array<float>(5.5)
	print("bb = {bb}\n")

	var c = tuple(1,2.0,"three")
	print("c = {c}\n")

	let e = fixed_array<int[1]>(1)
	print("e = {e}\n")

