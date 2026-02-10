Notifies compiler that the argument is consumed, i.e. it will not be used after this call.
This allows to avoid unnecessary clones and moves in some cases, for example when the argument is returned from the function.
Alternative syntax is foo ( <-arg ) instead of foo ( consume_argument(arg) ).
