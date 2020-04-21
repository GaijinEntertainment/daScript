options log = true

[unsafe]
def safe_addr(var a)
	return addr(a)

[export]
def test()

	var ar : array<int>
	var dummy : int
	ar?[0] ?? dummy = 1
	assert(dummy==1)
	resize(ar,1)
	ar?[0] ?? dummy = 2
	assert(length(ar)==1 & ar[0]==2)
	assert(dummy==1)

	var par = safe_addr(ar)
	clear(ar)
	dummy = 0
	par?[0] ?? dummy = 1
	assert(dummy==1)
	resize(ar,1)
	par?[0] ?? dummy = 2
	assert(length(ar)==1 & ar[0]==2)
	assert(dummy==1)
	par = null
	par?[0] ?? dummy = 3
	assert(dummy==3)

	var ta : table<string;int>
	dummy = 0
	ta?["one"] ?? dummy = 1
	assert(dummy==1)
	ta["one"] = 0
	ta?["one"] ?? dummy = 2
	assert(length(ta)==1 & ta["one"]==2)
	assert(dummy==1)

	var pta = safe_addr(ta)
	clear(ta)
	dummy = 0
	pta?["one"] ?? dummy = 1
	assert(dummy==1)
	assert(length(ta)==0)
	ta["one"] = 1
	pta?["one"] ?? dummy = 2
	assert(length(ta)==1 & ta["one"]==2)
	assert(dummy==1)
	pta = null
	pta?["one"] ?? dummy = 3
	assert(dummy==3)

	return true
