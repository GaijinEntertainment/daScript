def test:bool
    profile(100,"try-catch") <|
        let ( i,fail:int = 0; arr:int[1000] )
            while i < 2000
                try
                    arr[i] = i
                catch
                    fail ++
                i++
            assert(fail==1000,"can only fail so match")
    return true

