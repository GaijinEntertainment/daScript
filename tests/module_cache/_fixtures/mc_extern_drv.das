options gen2
options indenting = 4

require mc_extern_lib

[export]
def main() {
    print("EXTERN_DRV={mc_extern_probe()}\n")
}
