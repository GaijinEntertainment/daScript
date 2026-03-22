## Table iterator

See [benchmark](/benchmarks/core/table/test01.das)

Impact: moderate for interpreter, massive for JIT.

```
for ( k,v in keys(tab),values(tab) ) {
    ...
}
```

on interpreter side needs TableKeyValueIterator, like each_kv(tab,&k)
code becomes

```
{
    var k : keytype
    for ( v in each_kv(tab,&k) ) {
        ...
    }
}
...

JIT needs special case, where we essentially generate custom loop.

## Push vs Push clone

See [benchmark](/benchmarks/core/array/test02.das)

Impact: 1ns per operation, roughly - for interpreter.
Massive for JIT.

Replace push_clone with push for the type, which can be copied, and does not have custom clone functions.
On JIT side make an intrinsic for the '__builtin_array_push_back' and possibly '__builtin_array_push'.
