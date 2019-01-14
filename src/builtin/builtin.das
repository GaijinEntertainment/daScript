def resize(Arr:array<auto(numT)>;newSize:int)
    __builtin_array_resize(Arr,newSize,sizeof(type numT))

def reserve(Arr:array<auto(numT)>;newSize:int)
    __builtin_array_reserve(Arr,newSize,sizeof(type numT))

def push(Arr:array<auto(numT)>;value:numT;at:int=-1)
    Arr[__builtin_array_push(Arr,at,sizeof(type numT))] = value

def emplace(Arr:array<auto(numT)>;value:numT;at:int=-1)
    Arr[__builtin_array_push(Arr,at,sizeof(type numT))] <- value

