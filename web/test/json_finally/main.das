options gen2
require daslib/json
var cleanup_count = 0
def cleanup_flow(mode : int) : int {
    for (i in range(4)){
        if (i == 0){continue}
        if (mode == 1 && i == 1){break}
        if (mode == 2 && i == 1){return 17}
    }finally{cleanup_count++}
    return 23
}finally{cleanup_count += 100}
def nested_cleanup() : int {
    return 19
}finally{
    for (i in range(2)){
        if (i == 0){continue}
        break
    }finally{cleanup_count = cleanup_count * 10 + i + 1}
    cleanup_count = cleanup_count * 10 + 3
}
[export]
def main(){
    for (input in ["null", "true", "17", "2.5", "\"text\"", "[1,2,3]", "\{\"a\":1\}"]){
        var error : string
        var value <- read_json(input, error)
        verify(empty(error)); verify(value != null)
        delete_json(value)
    }
    for (input in ["[1,", "[", "\{\"a\":", "\{", "[true false]"]){
        var error : string
        var value <- read_json(input, error)
        verify(value == null); delete_json(value)
    }
    for (mode in range(3)){
        cleanup_count = 0
        let result = cleanup_flow(mode)
        verify(result == (mode == 2 ? 17 : 23))
        verify(cleanup_count == (mode == 0 ? 104 : 102))
    }
    cleanup_count = 0
    verify(nested_cleanup() == 19)
    verify(cleanup_count == 123)
    print("JSON_FINALLY_OK\n")
}
