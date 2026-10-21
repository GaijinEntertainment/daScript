options gen2
expect 30341

def value(input : float) : float {
    return input
}

[export]
def main() {
    sscan_json("1.5", value(7.0))
}
