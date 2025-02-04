# Utility to convert syntax from v1 to v2
## Key differences
First of all, you may want to check this [article about initialization](https://borisbat.github.io/dascf-blog/2024/07/23/data-initialization/). Here described most initialization expressions  in old and new syntax.
However there are some corner cases missed, like 
```
// old
var a = new [[Foo#()]]
// will be converted to new
var a = new struct<Foo#>()
```

## How it works
Generally formatter tools can work on token-level or AST-level. 
AST-level looks much easier to maintain so I decided to use it.

There's copy of bison grammar corresponding to v1 syntax. For required non terminals I edited action on match.

We will maintain `last_printed` position and once rule, 
which should be replaced is matched everything from `last_printed` to this rule
will go directly to `output` 
this rule will be modified and position updated.

## Usage:
I used this command to inplace transform all `.das` files syntax in current folder:
```
find . -name "*.das" | tr '\n' ' ' | xargs ./bin/das-fmt -i
```
