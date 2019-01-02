require testProfile

[es (pass="ks")]
def update(pos:float3&;vel:float3 const&)   // all by ref
    pos += vel

// note, when optimization off won't compile, since float3(...) is not a const, but a function
[es (pass="ks2")]
def update2(pos:float3&;vel:float3 const = float3(1.0f,2.0f,3.0f))   // deref .
    pos += vel

[es (pass="ks3")]
def update3(pos:float3&;vel:float3 const &;dt:float = 1.0f)
    pos += vel * dt

def queryOne(dt:float=1.0f)
    testProfile::queryEs() <| $ [es] (pos:float3&;vel:float3 const)
        pos += vel * dt

def queryTwo(dt:float=0.5f)
    testProfile::queryEs() <| $ [es] (pos:float3&;vel:float3 const;defT:float=0.5f)
        pos += vel * (defT + dt)

def queryThree()
    let count : int = 0
    testProfile::queryEs() <| $ [es] (pos:float3& const)
        let position : float3 = pos
        testProfile::queryEs() <| $ [es] (pos:float3& const)
            if position==pos
                count ++
    assert(count==10000)

def test:bool
    testProfile::initEsComponents()
    testProfile::testEsUpdate("ks")
    testProfile::verifyEsComponents()
    testProfile::initEsComponents()
    testProfile::testEsUpdate("ks2")
    testProfile::verifyEsComponents()
    testProfile::initEsComponents()
    testProfile::testEsUpdate("ks3")
    testProfile::verifyEsComponents()
    testProfile::initEsComponents()
    queryOne()
    testProfile::verifyEsComponents()
    testProfile::initEsComponents()
    queryTwo()
    testProfile::verifyEsComponents()
    testProfile::initEsComponents()
    profile(1000,"es-update") <|
        testProfile::testEsUpdate("ks")
    profile(1000,"es-update-ref") <|
        testProfile::testEsUpdate("ks2")
    profile(1000,"es-update-ref-const") <|
        testProfile::testEsUpdate("ks3")
    profile(1000,"query-1") <|
        queryOne()
    profile(1000,"query-2") <|
        queryTwo()
    profile(1,"query-3") <|
        queryThree()
    return true
