options indenting = 2
// array

def resize(Arr:array<auto(numT)>;newSize:int)
  __builtin_array_resize(Arr,newSize,typeinfo(sizeof type numT))

def reserve(Arr:array<auto(numT)>;newSize:int)
  __builtin_array_reserve(Arr,newSize,typeinfo(sizeof type numT))

def push(Arr:array<auto(numT)>;value:numT;at:int=-1)
  Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] = value

def emplace(Arr:array<auto(numT)>;value:numT;at:int=-1)
  Arr[__builtin_array_push(Arr,at,typeinfo(sizeof type numT))] <- value

def erase(Arr:array<auto(numT)>;at:int)
  __builtin_array_erase(Arr,at,typeinfo(sizeof type numT))

// table

def find(Tab:table<auto(keyT),auto(valT)>;at:keyT;blk:block<(p:valT?):void>)
  __builtin_table_lock(Tab)
  let val = __builtin_table_find(Tab,at)
  invoke(blk,val)
  __builtin_table_unlock(Tab)

def erase(Tab:table<auto(keyT),auto(valT)>;at:keyT)
  return __builtin_table_erase(Tab,at)

options removeUnusedSymbols=false

def peek ( src:das_string; subexpr:block<(root:string):void> ) 
  _builtin_peek(src,subexpr)

def binary_save(obj; subexpr:block<(data:string):void>)
  _builtin_binary_save(obj,subexpr)

def binary_load(obj; data:string)
  _builtin_binary_load(obj,data)

options indenting = 0

