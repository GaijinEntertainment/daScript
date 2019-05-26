options indenting = 2

require fio

struct FooBar
  x:int
  y:float

[export]
def test
  // save and load
  print("saving ftest.bin_save\n")
  fopen("ftest.bin_save","wb") <| $(file)
    if file!=null
      fsave(file,[[FooBar x=1,y=1.0]])
    else
      panic("can't open ftest.bin for writing")
  print("loading ftest.bin_save\n")      
  fopen("ftest.bin_save","rb") <| $(file)
    if file!=null
      var tfb : FooBar
      fload(file,tfb)
      assert(tfb.x==1 & tfb.y==1.0)
    else
      panic("can't open ftest.bin_save for reading")  
  return true
