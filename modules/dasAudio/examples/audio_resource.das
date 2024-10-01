options indenting = 4
options no_global_variables = false
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module audio_resource private

require fio

def public audio_fopen(name:string;mode:string;blk:block<(f:file):void>)
    var fname = "{get_das_root()}/modules/dasAudio/examples/{name}"
    var f = fopen(name,mode)
    if f == null
        fname = "{get_das_root()}/examples/audio/{name}"
        f = fopen(name,mode)
    invoke(blk,f)
    if f!=null
        fclose(f)

def public audio_fname(name) : string
    var fname = "{get_das_root()}/modules/dasAudio/examples/{name}"
    if !stat(fname).is_valid
        fname = "{get_das_root()}/examples/audio/{name}"
    return fname
