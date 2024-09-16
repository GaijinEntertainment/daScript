Hints the compiler to use specific optimization::

    [hint(noalias=objects,vec3_ldu)]    // object never aliases, we use vec3_ldu to load vectors, not 3-x float reads
    def testSimI(var objects:array<NObject>)
        for obj in objects
            obj.position += obj.velocity

