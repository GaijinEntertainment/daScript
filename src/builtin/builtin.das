// array

def resize(Arr:array<auto(numT)>;newSize:int)
    __builtin_array_resize(Arr,newSize,sizeof(type numT))

def reserve(Arr:array<auto(numT)>;newSize:int)
    __builtin_array_reserve(Arr,newSize,sizeof(type numT))

def push(Arr:array<auto(numT)>;value:numT;at:int=-1)
    Arr[__builtin_array_push(Arr,at,sizeof(type numT))] = value

def emplace(Arr:array<auto(numT)>;value:numT;at:int=-1)
    Arr[__builtin_array_push(Arr,at,sizeof(type numT))] <- value

def erase(Arr:array<auto(numT)>;at:int)
    __builtin_array_erase(Arr,at,sizeof(type numT))

// table

def find(Tab:table<auto(keyT),auto(valT)>;at:keyT)
    return __builtin_table_find(Tab,at)

def erase(Tab:table<auto(keyT),auto(valT)>;at:keyT)
    return __builtin_table_erase(Tab,at)

