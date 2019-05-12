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


