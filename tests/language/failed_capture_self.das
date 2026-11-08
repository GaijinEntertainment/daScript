//! a lambda in a class method that captures self, or is moved into a
//! field from a constant value, does not compile
options gen2
expect 30912, 30941

class A {
    private onLog : lambda
    private y : int
    def A {
        // 30508: can't move from a constant value, lambda<>& <- lambda<void>
        // const
        onLog <- @ {
            y --                //  30124: can't capture variable self
            feint("onLog\n")
        }
    }
    def bang {
        if (onLog != null) {
            onLog |> invoke()
        }
    }
    def operator delete {
        y ++
    }
}


