require fio

def fopen(name:string;mode:string="r";blk:block<(f:FILE?):void>)
	let f = fopen(name,mode)
    invoke(blk,f)
    fclose(f)


