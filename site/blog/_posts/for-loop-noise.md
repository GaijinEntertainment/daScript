---
title: For loop noise
date: 2023-01-27 16:24:05
tags:
    - C++
    - daScript
---

It's stupid. It's ugly. Its also measurable performance gain.

<!-- more -->

Let's write a node-based interpreter, just like the one we have, only smaller:

    #include <vector>

    struct Node {
        virtual void eval() = 0;
    };

    void eval ( std::vector<Node *> & nodes ) {
        for ( auto n : nodes ) {
            n->eval();
        }
    }

Disassembler (CLANG 15 -O3) says its good:

            mov     rbx, qword ptr [rdi]        // this is nodes.begin()
            mov     r14, qword ptr [rdi + 8]    // this is nodes.end()
            cmp     rbx, r14                    // if they are the same, we skip the loop
            je      .LBB0_3
    .LBB0_1:
            mov     rdi, qword ptr [rbx]        // vtable_ptr = nodes_ptr
            mov     rax, qword ptr [rdi]        // call_ptr = vtable_ptr[0]
            call    qword ptr [rax]             // this is n->eval() i.e. (*call_ptr)()
            add     rbx, 8                      // nodes_ptr ++
            cmp     rbx, r14                    // if nodes_ptr!=nodes.end() we continue looping
            jne     .LBB0_1

Sometimes we need an index of a node. Easy-peasy:

    void eval ( std::vector<Node *> & nodes ) {
        for ( int i=0; i!=nodes.size(); ++i ) {
            nodes[i]->eval();   // and we can use index i
        }
    }

But now disassembler hates us:

            mov     rax, qword ptr [rdi]            // this is nodes.begin()
            cmp     qword ptr [rdi + 8], rax        // if its the same as nodes.end() we skip the loop
            je      .LBB0_3
            mov     r14, rdi
            xor     ebx, ebx
    .LBB0_2:
            mov     rdi, qword ptr [rax + 8*rbx]    // vtable_ptr = nodes.data() [i]
            mov     rax, qword ptr [rdi]            // call_ptr = vtable_ptr[0]
            call    qword ptr [rax]                 // this is n->eval() is (*call_ptr)()
            inc     rbx                             // this is our index = i++
            mov     rax, qword ptr [r14]            // this is b = nodes.begin()
            mov     rcx, qword ptr [r14 + 8]        // this is e = nodes.end()
            sub     rcx, rax                        // this is .size() = (e - b) / 8
            sar     rcx, 3
            cmp     rbx, rcx                        // if i!=.size() we keep looping
            jne     .LBB0_2

What happens? In one word - aliasing. n->eval() can technically resize nodes.
So it reads an computes .size() every single iteration.
I know, I know, its nothing new. There are tones of articles about it.
And yet like a mad man I keep writing that code like an incantation every single time I need an index.
But no more:

    void eval3 ( std::vector<Node *> & nodes ) {
        for ( int i=0, is=nodes.size(); i!=is; ++i ) {
            nodes[i]->eval();   // and we can use index i
        }
    }

This is better:

            mov     rax, qword ptr [rdi + 8]        // this is nodes.begin()
            sub     rax, qword ptr [rdi]            // this is nodes.end()
            shr     rax, 3                          // this is .size()
            test    eax, eax                        // if its zero, we skip the loop
            je      .LBB0_3
            mov     r14, rdi
            mov     r15d, eax
            xor     ebx, ebx
    .LBB0_2:
            mov     rax, qword ptr [r14]            // we load nodes.data() here
            mov     rdi, qword ptr [rax + 8*rbx]    // vtable_ptr = nodes.data() [i]
            mov     rax, qword ptr [rdi]            // call_ptr = vtable_ptr[0]
            call    qword ptr [rax]                 // this is n->eval() is (*call_ptr)()
            inc     rbx                             // this is our index=i++
            cmp     r15, rbx                        // if i!=size() we keep looping
            jne     .LBB0_2

Now this is getting to the impractical territory. We now need to cache two variables, write more incantations:

    void eval4 ( std::vector<Node *> & nodes ) {
        auto data = nodes.data(); for ( int i=0, is=nodes.size(); i!=is; ++i ) {
            data[i]->eval();   // and we can use index i
        }
    }

However all is well in the land of disassembly:

            mov     r14, qword ptr [rdi]            // this is b = nodes.begin()
            mov     rax, qword ptr [rdi + 8]        // this is e = nodes.end()
            sub     rax, r14                        // this is nodes.size() = (e-b) / 8
            shr     rax, 3
            test    eax, eax                        // if its 0 we skip the loop
            je      .LBB0_3
            mov     r15d, eax
            xor     ebx, ebx
    .LBB0_2:
            mov     rdi, qword ptr [r14 + 8*rbx]    // vtable_ptr = nodes_data [index]
            mov     rax, qword ptr [rdi]            // call_ptr = vtable_ptr[0]
            call    qword ptr [rax]                 // this is n->eval() is (*call_ptr)()
            inc     rbx                             // this is index = i++
            cmp     r15, rbx                        // if index!=.size() we keep looping
            jne     .LBB0_2

The prologue of the loop is longer, but at least the loop itself is good.

Let's add an index, and actually pass it to the node like this:

    struct Node {
        virtual void eval(int i) = 0;
    };

    void eval ( std::vector<Node *> & nodes ) {
        int i=0;
        for ( auto n : nodes ) {
            n->eval(i);
            i ++;
        }
    }

And look, all is well in the land of disassembly:

            mov     rbx, qword ptr [rdi]        // nodes_ptr = nodes.begin
            mov     r14, qword ptr [rdi + 8]    // nodes.end
            cmp     rbx, r14                    // if they are the same, skip the loop
            je      .LBB0_3
            xor     ebp, ebp                    // index = 0
    .LBB0_2:
            mov     rdi, qword ptr [rbx]        // vtable = nodes_ptr
            mov     rax, qword ptr [rdi]        // call_ptr = vtable_ptr[0]
            mov     esi, ebp                    // pass i as first argument
            call    qword ptr [rax]             // (*call_ptr) ( i )
            inc     ebp                         // i ++
            add     rbx, 8                      // nodes_ptr ++
            cmp     rbx, r14                    // if nodes_ptr != nodes_end continue looping
            jne     .LBB0_2

Also what if there are multiple sources? Do we cache every data() if size() is the same?

So, if u need index, there are indeed two and a half "good" options
* cache .data() and .size()
    not exactly readable, not easy to maintain
* use range based for, make index variable, increase manually
    error prone, there could be continue in the middle
* its ok to cache just .size()
    data is still aliased

In daScript there is multiple source syntax, which can be used for multiple sources and indices:

    for n1,n2 in arr1,arr2
        ...
    for n,i in nodes,count(start,step)  // or just count()
        ...

The upside is that iterators cache values. The downside is that each source is tested for the end of loop individually.
Jit manages to keep everything in registers.

AOT is an interesting story. Lets write the minimal framework necessary to simulate daScript `Array` and `das_iterator`:

    #include <assert.h>

    #define __forceinline   inline

    struct Node {
        virtual void eval() = 0;
    };

    struct Context { };

    struct Array {
        char * data;
        int    size;
        int    lock;
    };

    template <typename TT>
    struct TArray : Array { };

    void array_lock ( Context & ctx, Array & arr ) { arr.lock ++; }
    void array_unlock ( Context & ctx, Array & arr ) { assert(arr.lock); arr.lock --; }

    template <typename TT>
    struct das_iterator;

    template <typename TT>
    struct das_iterator<TArray<TT>> {
        __forceinline das_iterator(TArray<TT> & r) {
            that = &r;
            array_end = (TT *)(that->data + that->size * sizeof(TT));
        }
        template <typename QQ>
        __forceinline bool first(Context * __context__, QQ * & i) {
            context = __context__;
            array_lock(*__context__, *that);
            i = (QQ *)that->data;
            return i != (QQ *)array_end;
        }
        template <typename QQ>
        __forceinline bool next(Context *, QQ * & i) {
            i++; return i != (QQ *)array_end;
        }
        template <typename QQ>
        __forceinline void close(Context * __context__, QQ * & i) {
            array_unlock(*__context__, *that);
            context = nullptr;
            i = nullptr;
        }
        ~das_iterator() {
            TT * dummy = nullptr;
            if (context) close(context, dummy);
        }
        Array * that;
        TT * array_end;
        Context * context = nullptr;
    };

    void eval ( TArray<Node *> & nodes, Context * __context__ ) {
        das_iterator<TArray<Node *>> node(nodes);
        Node ** n = nullptr;
        bool needLoop = true;
        needLoop = node.first(__context__, n) && needLoop;
        for ( ; needLoop; needLoop = node.next(__context__,n) ) {
            (*n)->eval();
        }
        node.close(__context__,n);
    }

Things are good in the land of disassembler:

    .LBB2_2:
            mov     rdi, qword ptr [r15 + rbx]  // vtable_ptr = node_ptr
            mov     rax, qword ptr [rdi]        // call_ptr = vtable_ptr[0]
            call    qword ptr [rax]             // (*call_ptr)()
            add     rbx, 8                      // node_ptr ++
            cmp     r12, rbx                    // if node_ptr != data.end() keep looping
            jne     .LBB2_2

Prologue and epilogue are slightly longer, but everything is in the registers.

`count` happens to be my daScript incantation of the day. Just add `count' when you need index.
AOT recognizes it, JIT recognizes it. Its fast. It keeps disassembler happy.

To summarize when possible use daScript; it will do all those incantations for you.
For C++
* use range based loop when you can
* at least cache size when you can't
* when writing performance critical C++ - cache everything
* reconsider rewriting in daScript


