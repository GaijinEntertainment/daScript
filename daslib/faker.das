options no_unused_block_arguments = false
options no_unused_function_arguments = false
options indenting = 4

module faker shared private

require daslib/regex_gen
require daslib/regex_boost
require daslib/random

var private re_positive_int <- %regex~[1-9][0-9]*%%
var private re_int <- %regex~\-?[1-9][0-9]*%%

def public fake ( var count:int; var re:Regex; blk : block<(tmp:string):void> )
    var reg <- re_gen_compile(re)
    var rnd <- each_random_uint()
    for c in range(count)
        let str = re_gen(re,reg,rnd)
        invoke(blk,str)
    delete rnd
    delete reg

def public fake_positive_int ( var count:int; blk : block<(tmp:string):void> )
    fake ( count, re_positive_int, blk )

def public fake_int ( var count:int; blk : block<(tmp:string):void> )
    fake ( count, re_int, blk )