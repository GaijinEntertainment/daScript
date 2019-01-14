def resize(Arr:array<auto(numT)>;newSize:int)
    __builtin_array_resize(Arr,newSize,sizeof(type numT))

def reserve(Arr:array<auto(numT)>;newSize:int)
    __builtin_array_reserve(Arr,newSize,sizeof(type numT))
