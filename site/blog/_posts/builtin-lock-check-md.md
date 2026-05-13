---
title: Lock and load
date: 2022-11-13 11:58:49
tags: daScript
---

Consider the following example:

```python
[export]
def main
    var a <- [{int 1;2;3;4}]
    for x in a
        a |> push(13)   // kaboom
```

It causes daScript runtime panic.

<!-- more -->

```
unhandled exception
: can't resize locked array
```

The underlying reason for the panic is that the array data is allocated dynamically.
`push` among other operations like `resize`,`reserve`,`emplace`, and `erase` can sometimes cause the array data to be reallocated and moved to a new memory location, which in turn would cause loop variables to expire while still pointing to the no longer used region of the heap - which is obviously unsafe.

To prevent this from happening locking is implemented.
An array iterator increases the lock count during its initialization, and decreases it during the iterator's finalization.
To access the lock counter during the finalization, the array iterator caches the pointer to the original array.

```python
[export]
def main
    var a <- [{int[] 1}]
    print("before the first loop: {lock_count(a)}\n")
    for x in a
        print("before the second loop: {lock_count(a)}\n")
        for y in a
            print("inside the second loop: {lock_count(a)}\n")
        print("after the second loop: {lock_count(a)}\n")
    print("after the first loop: {lock_count(a)}\n")
```

The result is exactly as expected

```
before the first loop: 0
before the second loop: 1
inside the second loop: 2
after the second loop: 1
after the first loop: 0
```

Similar counter is also implemented for the tables, which also happen to store their data dynamically.

On the surface this provides a fool-proof mechanism which prevents dangling references to the array data, however, this is not the case. Consider another example:

```python
[export]
def main
    var a : array< array<int> >
    a |> emplace <| [{int 1;2;3}]
    a |> emplace <| [{int 4;5;6}]
    for x in a[0]
        for y in a[1]
            print("before resize: {lock_count(a)}\n") // prints '0'
            resize(a, 3)        // kaboom
            print("{y}\n")
```

Lock count on the `a` before the resize is 0. However the code above is unsafe.
Resizing `a` may cause relocation of both `a[0]` and `a[1]`, which are being iterated over.
That way cached pointers to the arrays inside the iterators expire.

However, example above causes daScript runtime panic during the `resize`:

```
unhandled exception
: object contains locked elements and can't be resized
```

If we check `options log` we could see what actually happens:

```python
def private builtin`resize ( var Arr:array<array<int> aka numT> explicit; newSize:int const )
	_builtin_verify_locks(Arr)
	__builtin_array_resize(Arr,newSize,24,__context__)
```

daScript generates custom resize function for the `array<array<int>>`, and calls lock verification function `_builtin_verify_locks`.
Internally, it looks over provided data and checks lock counters on all arrays (and tables). Panic occurs if any of them are non-zero.

```python
[export]
def main
    profile(1,"with lock check") <|
        var a : array< array<int> >
        for t in range(10000)
            a |> emplace <| [{int t;t+1;t+2}]   // lock checked every time
    profile(1,"without lock check") <|
        var a : array< array<int> >
        unsafe
            set_verify_array_locks(a, false)
        for t in range(10000)
            a |> emplace <| [{int t;t+1;t+2}]   // lock check ignored
```

It's important to note that lock verification could have a significant performance overhead.

```
"with lock check", 1.10712, 1
"without lock check", 0.002006, 1
```

The best way to avoid such overhead is to not create data structures with internal locking.
set_verify_array_locks' and `set_verify_table_locks` provide more practical albeit unsafe solution.
