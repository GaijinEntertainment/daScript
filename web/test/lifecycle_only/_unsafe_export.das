options gen2
options gc
options persistent_heap
[export]
def update() : bool {return false}
[export]
def main(){
    let hold <- [1, 2, 3]
    unsafe(heap_collect())
    print("{hold[0]}\n")
}
