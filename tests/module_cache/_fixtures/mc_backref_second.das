options gen2
options indenting = 4

require daslib/json_boost

[export]
def main {
    let v = JV(1)
    print("MC_BACKREF_SECOND {v}\n")
}
