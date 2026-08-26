#pragma once

// The one hook nano asks an embedder for.
//
// Everything the runtime prints - `print`, a panic report, a fatal - leaves
// through this sink. The default writes to stdout, which is right on a host and
// absent on a board; point it at printk, a UART write, or a ring buffer before
// constructing the context and the runtime needs no other I/O.

namespace das {

    typedef void ( * das_nano_print_sink ) ( const char * text );

    void das_nano_set_print ( das_nano_print_sink sink );
}
