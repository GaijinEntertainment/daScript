require macro3

[export]
def main {
    var ct = default < $TemplateCounter<int> >
    ct.next()
    print_counter(ct)
    print("Counter value: {ct.value()}\n")
}
