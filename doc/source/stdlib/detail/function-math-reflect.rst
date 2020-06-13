returns vector representing reflection of arg0 from arg1 same as ::

    def reflect(v, n: float3)
        return v - 2. * dot(v, n) * n
