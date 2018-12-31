def test:bool
    profile(10,"try-catch") <|
        let ( i,fail,cnt:int = 0; arr:int[1000] )
            for j in range (0, 1000)
                fail = 0
                for i in range (0, 2000)
                    try
                        cnt += arr[i]
                    catch
                        fail ++
                assert(fail == 1000)
    return true

