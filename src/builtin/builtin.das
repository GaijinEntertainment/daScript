def resize(Arr:array<auto(numT)>;newSize:int)
    __builtin_array_resize(Arr,newSize,sizeof(type numT))

