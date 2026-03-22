## Table iterator

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
