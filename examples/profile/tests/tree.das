//this is port from https://github.com/frol/completely-unscientific-benchmarks
//almost direct kotlin implementation
options heap = 200000//not required hint to increase initial heap footprint. <1% of performance. 200kb is enough

require random

struct Node
    x,y:int = 0
    left, right: Node?

var
    seed:int4 = int4(1,1,1,1)

[unsafe_deref]
def merge(var lower, greater: Node?): Node?
    if lower == null 
    	return greater
    
    if greater == null
    	return lower
    
    if lower.y < greater.y
        lower.right = merge(lower.right, greater)
        return lower
    else
        greater.left = merge(lower, greater.left)
        return greater

struct Pair
    first,second: Node?

[unsafe_deref]
def splitBinary(var orig: Node?; value: int): Pair
    if orig == null
    	return [[ Pair first=null, second=null ]]
    
    if orig.x < value
        let splitPair = splitBinary(orig.right, value)
        orig.right = splitPair.first
        return [[ Pair first=orig, second=splitPair.second ]]
    else
        let splitPair = splitBinary(orig.left, value)
        orig.left = splitPair.second
        return [[ Pair first=splitPair.first, second=orig ]]

def merge(var lower,equal,greater: Node?): Node?
    return merge(merge(lower, equal), greater)

struct SplitResult
    lower,equal,greater:Node?

def split(var orig: Node?; value: int): SplitResult
    var lower_eg = splitBinary(orig, value)
    var equal_greater = splitBinary(lower_eg.second, value + 1)
    return [[ SplitResult lower = lower_eg.first, equal = equal_greater.first, greater = equal_greater.second ]]

def hasValue(var mRoot:Node? &; x: int): bool
    var splited = split(mRoot, x)
    let res = splited.equal != null
    mRoot = merge(splited.lower, splited.equal, splited.greater)
    return res

def insert(var mRoot:Node? &; x: int)
    var splited = split(mRoot, x)
    if splited.equal == null
    	splited.equal = new [[Node x=x, y=random_int(seed)]]
    mRoot = merge(splited.lower, splited.equal, splited.greater)

[unsafe, unsafe_deref]
def destroy(var root:Node?)
    if root != null
		destroy(root.left)
		destroy(root.right)
		delete root

//GC version - relies on GC phase
def erase(var mRoot:Node? &; x: int)
    var splited = split(mRoot, x)
    mRoot = merge(splited.lower, splited.greater)

//raw delete version - even with GC will be faster delete
def erase_raw(var mRoot:Node? &; x: int)
    var splited = split(mRoot, x)
    mRoot = merge(splited.lower, splited.greater)
    destroy(splited.equal)

def main
    var tree:Node?
    var cur = 5
    var res = 0

    for i in range(1, 1000000)
        let a = i % 3
        cur = (cur * 57 + 43) % 10007
        if a == 0
            tree |> insert(cur)
        elif a == 1
            tree |> erase_raw(cur)
        elif a == 2
            res += (tree |> hasValue(cur)) ? 1 : 0
	destroy(tree)
    //print("{res}")

[export]
def test
    profile(10, "tree")  <|
        main()
	// print("total = {int(heap_bytes_allocated())}\n")
    // print("depth = {heap_depth()}\n")
    verify(heap_depth()==1)
    return true
