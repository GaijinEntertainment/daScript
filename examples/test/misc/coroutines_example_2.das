require daslib/coroutines

struct Tree
    data : int
    left, right : Tree?

let tree = new [[ Tree
    data = 5,
    left = new [[Tree
        data = 1
    ]],
    right = new [[Tree
        data = 7,
        right = new [[Tree
            data = 10
         ]]
    ]]
]]

[coroutine]
def each_async(tree : Tree?) : int
    if tree.left != null
        co_await <| each_async(tree.left)
    yield tree.data
    if tree.right != null
        co_await <| each_async(tree.right)

[export]
def main
    for t in each_async(tree)
        print("{t}\n")



