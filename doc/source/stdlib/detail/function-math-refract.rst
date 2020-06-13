returns vector representing reflection of arg0 from arg1 same as ::

    def refract(v, n: float3; nint: float; outRefracted: float3&)
        let dt = dot(v, n)
        let discr = 1. - nint * nint * (1. - dt * dt)
        if discr > 0.
            outRefracted = nint * (v - n * dt) - n * sqrt(discr)
            return true
        return false
