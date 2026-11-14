options gen2
options rtti
require daslib/fio
[export]
def main(){
    verify(fexist("/"))
    var recovered = false
    try { panic("expected recovery") } recover { recovered = true }
    verify(recovered)
    print("FILEINFO_CONSTRUCTOR_OK\n")
}
