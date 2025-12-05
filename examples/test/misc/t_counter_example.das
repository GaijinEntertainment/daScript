require t_counter

typedef CounterInt32 = $counter(type<int>)

[export]
def main {
    var counter = CounterInt32()
    counter.next()
    print_counter(counter)
}
