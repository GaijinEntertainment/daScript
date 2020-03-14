options log = true
options log_nodes = true
options log_stack = true

require UnitTest

def getEntityTemplateName(eid) : string
    return "foo"

[export]
def test
    let activeRitualTemplate : string const& = getEntityTemplateName(12)
    print("activeRitualTemplate = {activeRitualTemplate}\n")
    return true
