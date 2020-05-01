options log = true
options log_infer_passes = true
// options log_nodes=true

typedef
	bits123 = bitfield < one; two; three >

[sideeffects]
def bit(b:int)
	return bitfield(1 << b)

[export]
def test()
	var t : bits123 = bitfield(1<<1) | bit(2)	// 8,13 - t
	assert(!t.one & t.two & t.three)
	assert("{t}"=="(two|three)")
	t ^= bitfield(1<<1)
	assert(!t.one & !t.two & t.three)
	assert(t==bits123 three)
	t |= bitfield(1<<0)
	assert(t.one & !t.two & t.three)
	return true
