//! under options lint = false an option no module declares still
//! does not compile
options gen2
options lint = false
options no_such_option_anywhere = true

expect 50100

[export]
def main {
    feint("hello\n")
}
