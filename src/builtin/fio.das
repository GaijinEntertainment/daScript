require fio

def fopen(name:string;mode:string="r";blk:block<(f:FILE?):void>)
	let f = fopen(name,mode)
    invoke(blk,f)
    fclose(f)

def dir(path:string;blk:block<(filename:string):void>)
    builtin_dir(path,blk)

def stat(path:string):FStat
    var fs : FStat
    if stat(path,fs)
        fs.is_valid = true
    return fs

def fstat(f:FILE?):FStat
    var fs : FStat
    if fstat(f,fs)
        fs.is_valid = true
    return fs

let
	df_magic = 0x12345678  
  
struct df_header
	magic:uint
	size:int
  
def fload(f:FILE?const;buf:auto(BufType)delete const)
	var dfh:df_header
	let r1 = _builtin_read(f,dfh,typeinfo(sizeof type df_header))
	if r1<0
		return false
	if dfh.magic!=df_magic
		return false
	var loaded = false
	_builtin_load(f,dfh.size) <| $(data:string)
		if length(data)!=0
			binary_load(buf,data)
			loaded = true
	return loaded

def fsave(f:FILE?const;buf:auto(BufType)const)
	var r1, r2 : int
	binary_save(buf) <| $(data)
		let len = length(data)
		let dfh = [[df_header magic=df_magic, size=len]]
		r1 = _builtin_write(f,dfh,typeinfo(sizeof type df_header))
		r2 = _builtin_write(f,data,len)
	if r1<0
		return r1
	if r2<0
		return r2
	return (r1+r2)

