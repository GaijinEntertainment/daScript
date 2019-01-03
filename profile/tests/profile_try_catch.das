def test:bool
    let ( i,fail,cnt:int = 0; arr:int[1000] )
        profile(20,"try-catch") <|
            for j in range (0, 100)
                fail = 0
                for i in range (0, 2000)
                    try
                        cnt += arr[i]
                    catch
                        fail ++
    return true

