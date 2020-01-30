// options log_require = true
// options log = true
options optimize = false

require event
require sub/event as sub_event

[export]
def test
    verify(event::fn_event())
    verify(!sub_event::fn_event())
	return true
