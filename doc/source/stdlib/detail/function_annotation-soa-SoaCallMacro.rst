Rewrites ``soa[index].field`` into ``soa.field[index]`` at compile time.
This is the core of the SOA access pattern — it transforms AOS-style element access
into column-wise array indexing for better cache locality.
