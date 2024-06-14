expect 10006:12, 10010:4

[export]
def main
	// ALL THE POSSIBLE NUMBER TYPES AND THEIR LITERALS, OUT OF RANGE VALUES
	// uint64
	var ui64 = 10000000000000000000000000000000000000ul
	print("ui64 = {ui64}\n")
	// int64
	var i64 = 10000000000000000000000000000000000000l
	print("i64 = {i64}\n")
	// uint8
	var ui8 = 1000u8
	print("ui8 = {ui8}\n")
	// uint
	var ui = 1000000000000u
	print("ui = {ui}\n")
	// int
	var i = 1000000000000
	print("i = {i}\n")
	// int range
	var ri = 120000000000000000..1000000000000
	print("ri = {ri}\n")
	// uint64 hex
	var ui64h = 0x100abcd0000000000000000000000000000000ul
	print("ui64h = {ui64h}\n")
	// int64 hex
	var i64h = 0x100abcd0000000000000000000000000000l
	print("i64h = {i64h}\n")
	// uint8 hex
	var ui8h = 0xab0u8
	print("ui8h = {ui8h}\n")
	// uint hex
	var uih = 0x100000000000u
	print("uih = {uih}\n")
	// int hex
	var ih = 0x100000000000
	print("ih = {ih}\n")
	// scientific notation float
	var f = 1.0e1000
	print("f = {f}\n")
	// regular float without fractional part (and out of range)
	var f2 = 1000000000000000000000000000000000000000000000000000f	// this returns inf, dunno why
	print("f2 = {f2}\n")
	// scientific notation float without fractional part
	var f3 = 1e1000
	print("f3 = {f3}\n")
	// regular float with . but no fractional part
	var f4 = 100000000000000000000000000000000000000000000000000.
	print("f4 = {f4}\n")
	// scientific notation double
	var d = 1.0e1000lf
	print("d = {d}\n")
	// regular double without fractional part (and out of range)
	var d2 = 100000000000000000000000000000000000000000000000000000000000000000000lf	// this returns inf, dunno why
	print("d2 = {d2}\n")
	// scientific notation double without fractional part
	var d3 = 1e1000lf
	print("d3 = {d3}\n")


















