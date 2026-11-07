options gen2
expect 30299

def f(n : int) : int {
    try {
        return n * 2
    } recover {
        pass
    }
    return n
}

[export]
def main {
    print("{f(1)}\n")
}
