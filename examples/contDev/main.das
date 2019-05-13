// options logNodes=true, logStack=true

require fio

[export]
def main(fn:string)
    let atdir = dirname(fn) + "/src"
    print("das network message generator at {atdir}\n")
	let main_time = stat(fn).mtime
    var ttab : table<string,clock>
	while stat(fn).mtime == main_time
        dir(atdir) <| $ (fname)
            let fileName = atdir + "/" + fname
            var st : FStat
            if stat(fileName,st)
                if st.is_reg
                    let fmt = st.mtime
                    let tmt = ttab[fname]
                    if (tmt!=fmt)
                        ttab[fname] = fmt
                        print("processing {fname}\n")
            else
                print("can't stat {fileName}\n")
		sleep(1000u)
	print("ok\n")
	return true

