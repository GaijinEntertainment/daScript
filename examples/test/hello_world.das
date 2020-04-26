options log = true

require UnitTest

[unsafe]
def get ( src : smart_ptr<auto(TT)> ) : TT?
	return reinterpret<TT?>(src)

def clone ( var dest : smart_ptr<TestObjectSmart>&; src : TestObjectSmart? )
	smart_ptr_clone(dest, src)

def clone ( var dest : smart_ptr<TestObjectSmart>&; src : smart_ptr<TestObjectSmart> )
	smart_ptr_clone(dest, src)

[export, unsafe]
def test

	var ptr = new TestObjectSmart
	var sptr : smart_ptr<TestObjectSmart>
	assert(sptr==null)
	sptr := ptr						// ref_count = 1
	assert(sptr==ptr)
	assert(get(sptr)==ptr)
	var qptr : smart_ptr<TestObjectSmart>
	qptr := sptr					// ref_count = 2
	assert(qptr==ptr)
	assert(get(qptr)==ptr)
	assert(qptr==sptr)
	var fptr := qptr				// ref_count = 3
	assert(fptr!=null)
	assert(get(fptr)==sptr)
	assert(fptr==qptr)
	assert(fptr==sptr)

	delete sptr
	delete qptr
	delete fptr

	return true
