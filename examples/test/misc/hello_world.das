require daslib/lpipe

def take2(a,b:block)
    invoke(a)
    invoke(b)

[export]
def main
    take2 <|
        print("block1\n")
    lpipe <|
        print("block2\n")
