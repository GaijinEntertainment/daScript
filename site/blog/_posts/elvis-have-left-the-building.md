---
title: Bye-bye Elvis
date: 2026-06-11 23:30:24
tags:
    - daScript
    - Claude
---

I'm happy. I'm feeling glad.

<!-- more -->

Because the proverbial Elvis has left the proverbial building.

```daslang
def foo ( a : auto(TT) ) {
    var b : TT = a
}
var arr : int[4]
foo(arr)
```

This is about as fundamental as it gets. Or at least it should be. Only it was not. Well, it mostly was. It pretended that it was. But it was not. Not really. Elvis - I hate your guts. Won't miss u. Don't let the door hit u on the way out.

Two words. Fixed arrays. Since the dawn of day. Since the inception of daScript. From before generics were part of the language, I decided that having `vector<int> dim` as part of the base type was a good idea. Like float[4][4] is baseType=Type::tFloat, and dim = {4,4}. Awesome. Cool beans.

By the time generics landed, it was too late. There was the game running, and 'changing everything' was already not an option. I did not know it yet, until few months later someone decided to push int[4] into `array<int[4]>`. Thats when it dawned on me. Until yesterday in the example above TT would resolve to 'int'. Just like that.

Inheriting the full line was not it either. typedef mat = float[4][4] and then passing mat[3] to an auto(TT)[] generic - and u don't know where to split it. At least int was manageable. Just add [], [][], and [][][] override. 640kb is enough for everyone.

The longer it dragged, the more it annoyed me. But, the transition cost was off-putting. "Touch five execution tiers, every classifier, three native modules, the IDE plugin, the doc toolchain, and don't break a language people ship games on". I'll just add another generic. No one uses fixed array anyways. And it kept dragging on.

Sometimes stars align. 10k tests (10920 on the day of the refactor) make it possible to break things. Solid CI matrix is there to check the infrastructure breakdown. MCP allowed rapid iterations. The whole ecosystem is organized to be sweepable. Thats how a story turns into a fable.

Bye Elvis. Until we meet again. One dead Elvis at a time.

The future is coming on.
