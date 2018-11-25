def test
    let
        i,fail:int = 0
        arr:int[1000]
    in
        while i < 2000
            try
                arr[i] = i
            catch
                fail ++
            i++
        assert(fail==1000,"can only fail so match")

