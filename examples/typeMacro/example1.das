require macro1

def print_counter(c : $TemplateCounter < auto(CType) >) {
    print("Counter value: {c.value()}\n")
}

[export]
def main {
    var ct = default < $TemplateCounter < int > >
    ct.next()
    print_counter(ct)
    print("Counter value: {ct.value()}\n")
}

options log
