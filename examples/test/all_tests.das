// 8Mb heap
options heap=8388608, logCompileTime = true

require math

def makeRandomSequence(src:array<string>)
    let n = 500000
    let mod = uint(n)
    resize(src,n)
    for i in range(0,n)
        let num = (271828183u ^ uint(i*119))%mod
        src[i] = "{num}"

def dict(tab:table<string,int>; src:array<string>)
    clear(tab)
    let maxOcc = 0
    for s in src
        maxOcc = max(++tab[s],maxOcc)
    return maxOcc

def test_dict
    let in scope tab : table<string,int>
    let in scope src : array<string>
    makeRandomSequence(src)
	profile(20,"dictionary") <|
        dict(tab,src)

[sideeffects]
def expLoop(n)
    let sum = 0.
    for i in range(0, n)
        sum += exp(1./(1. + float(i)))
    return sum

[sideeffects]
def expLoopU(n)
    let sum = 0.
    for i in range(0, n/4)
        let k = float(i*4)
        sum += exp(1./(1. + k)) + exp(1./(2. + k)) + exp(1./(3. + k)) + exp(1./(4. + k))
    return sum

def test_exp
    profile(20,"exp loop") <|
        expLoop(1000000)
    profile(20,"exp loop, unrolled") <|
        expLoopU(1000000)

[sideeffects]
def fibR(n)
    if (n < 2)
        return n
	return fibR(n - 1) + fibR(n - 2)

[sideeffects]
def fibI(n)
    let last = 0
    let cur = 1
    for i in range(0, n-1)
        let tmp = cur
        cur += last
        last = tmp
    return cur

[sideeffects]
def fibIU(n)
    let last = 0
    let cur = 1
    for i in range(0, (n-1)/4)
        let tmp1 = cur
        cur += last
        let tmp2 = cur
        cur += tmp1
        let tmp3 = cur
        cur += tmp2
        let tmp4 = cur
        cur += tmp3
        last = tmp4
    return cur

def test_fib
    profile(20,"fibbonacci loop") <|
        fibI(6511134)
    profile(20,"fibbonacci loop, unrolled") <|
        fibIU(6511134)
    profile(20,"fibbonacci recursive") <|
        fibR(31)

struct NObject
    position, velocity : float3

def update(a:NObject)
    a.position+=a.velocity

def testSim(objects:array<NObject>)
    for obj in objects
        update(obj)

def testSim2(objects:array<NObject>; count:int)
    for i in range(0,count)
        testSim(objects)

def testSimI(objects:array<NObject>)
    for obj in objects
        obj.position += obj.velocity

def testSim2I(objects:array<NObject>; count:int)
    for i in range(0,count)
        testSimI(objects)

def init(objects:array<NObject>)
    resize(objects, 50000)
    let i = 0
    for obj in objects
        obj.position=float3(i++,i+1,i+2)
        obj.velocity=float3(1.0,2.0,3.0)
    assert(i==length(objects))

def test_particles
    let in scope objects:array<NObject>
    init(objects)
    let(total=20)
        profile(total,"particles kinematics") <|
            testSim2(objects,100)
        profile(total,"particles kinematics, inlined") <|
            testSim2I(objects,100)

def isprime(n)
    for i in range(2, n)
        if (n % i == 0)
            return false
    return true

[sideeffects]
def primes(n)
    let count = 0
    for i in range(2, n+1)
        if (isprime(i))
            ++count
    return count

[sideeffects]
def primesI(n)
    let count = 0
    for i in range(2, n+1)
		count ++
		for j in range(2,i)
			if i % j == 0
				count --
				break
    return count

def test_primes
    profile(20,"primes loop") <|
        primes(14000)
    profile(20,"primes loop, inline") <|
        primesI(14000)

[export]
def test
    test_dict()
    test_primes()
    test_exp()
    test_fib()
    test_particles()
    return true


