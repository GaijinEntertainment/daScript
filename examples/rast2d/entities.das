module entities

enum Collectible
    DogFood = 29
    KeySilver = 43
    KeyGold = 44
    Food = 47
    Medkit = 48
    AmmoClip = 49
    MachineGun = 50
    Chaingun = 51
    TreasureCross = 52
    TreasureChalice = 53
    TreasureChest = 54
    TreasureCrown = 55
    LifeUp = 56

enum Props
    GreenBarrel = 24
    TableAndChairs = 25
    FloorLamp = 26
    HangedSkeleton = 28
    WhitePillar = 30
    Tree = 31
    Sink = 33
    PottedPlant = 34
    Urn = 35
    BareTable = 36
    SuitOfArmor = 39
    HangingCage = 40
    SkeletonInCage = 41
    Bed = 45
    Barrel = 58
    Well = 59
    EmptyWell = 60
    Flag = 62
    CallApogee = 63
    Stove = 68
    Spears = 69

def isKey ( m1:int )
    return m1==43 || m1==44

def isHpRestore ( m1:int )
    return m1==29 || m1==47 || m1==48

def getEntityDirection(dm:int; var dx,dy:float& )
    if dm==0
        dx = 0.0;   dy = -1.0
    elif dm==1
        dx = 1.0;   dy = 0.0
    elif dm==2
        dx = 0.0;   dy = 1.0
    elif dm==3
        dx = -1.0;  dy = 0.0

def isPlayer(m1:int)
    return m1>=19 && m1<=22

def isProp(m1:int)
    return m1>=23 && m1<=70

def isPushWall(m1:int)
    return m1==98

def isDeadGuard(m1:int)
    return m1==124

def isTreasure(m1:int)
    return (m1==52 || m1==53 || m1==54 || m1==55)

def isCollectible(m1:int)
    return (m1==29 || m1==43 || m1==44 || m1==47 || m1==48 || m1==49 || m1==50 || m1==51 || m1==52 || m1==53 || m1==54 || m1==55 || m1==56)

def isBlocking(m1:int)
    return (m1==24 || m1==25 || m1==26 || m1==28 || m1==30 || m1==31 || m1==33 || m1==34 || m1==35 || m1==36 || m1==39 || m1==40 || m1==41 || m1==45 || m1==58 || m1==59 || m1==60 || m1==62 || m1==63 || m1==68 || m1==69)

def isEnemy(m1:int)
    return (m1>=108 && m1<=259)

def isStandingGuard(m1:int)
    return (m1>=108 && m1<=111 || m1>=144 && m1<=147 || m1>=180 && m1<=183)

def isPatrollingGuard(m1:int)
    return (m1>=112 && m1<=115 || m1>=148 && m1<=151 || m1>=184 && m1<=187)

def isStandingOfficer(m1:int)
    return (m1>=116 && m1<=119 || m1>=152 && m1<=155 || m1>=188 && m1<=191)

def isPtrollingOfficer(m1:int)
    return (m1>=120 && m1<=123 || m1>=156 && m1<=159 || m1>=192 && m1<=195)

def isStandingSS(m1:int)
    return (m1>=126 && m1<=129 || m1>=162 && m1<=165 || m1>=198 && m1<=201)

def isPatrollingSS(m1:int)
    return (m1>=130 && m1<=133 || m1>=166 && m1<=169 || m1>=202 && m1<=205)

def isStandingDog(m1:int)
    return (m1>=134 && m1<=137 || m1>=170 && m1<=173 || m1>=206 && m1<=209)

def isPatrollingDog(m1:int)
    return (m1>=138 && m1<=141 || m1>=174 && m1<=177 || m1>=210 && m1<=213)

def isStandingMutant(m1:int)
    return (m1>=216 && m1<=219 || m1>=234 && m1<=237 || m1>=252 && m1<=255)

def isPatrollingMutant(m1:int)
    return (m1>=220 && m1<=223 || m1>=238 && m1<=241 || m1>=256 && m1<=259)

def isFakeHitler(m1:int)
    return (m1==160)

def isHitler(m1:int)
    return (m1==178)

def isGeneralFettgesicht(m1:int)
    return (m1==179)

def isDoctorSchabbs(m1:int)
    return (m1==196)

def isGretelGrosse(m1:int)
    return (m1==197)

def isHansGrosse(m1:int)
    return (m1==214)

def isOttoGiftmacher(m1:int)
    return (m1==215)

def isRedPacManGhost(m1:int)
    return (m1==224)

def isPinkPacManGhost(m1:int)
    return (m1==225)

def isOrangePacManGhost(m1:int)
    return (m1==226)

def isBluePacManGhost(m1:int)
    return (m1==227)

/*
class public HansEnemy : Enemy
    def HansEnemy(X,Y:int)
        Enemy`Enemy(self, X, Y, 300, [{auto 304; 305; 306; 303}], true)

class public SchabbsEnemy : Enemy
    def SchabbsEnemy(X,Y:int)
        Enemy`Enemy(self, X, Y, 312, [{auto 313; 314; 315; 316}], true)

class public FakeHitlerEnemy : Enemy
    def FakeHitlerEnemy(X,Y:int)
        Enemy`Enemy(self, X, Y, 321, [{auto 328; 329; 330; 331; 332; 333}], true)

class public HitlerEnemy : Enemy
    def HitlerEnemy(X,Y:int)
        Enemy`Enemy(self, X, Y, 349, [{auto 353; 354; 355; 356; 357; 358; 359; 352}], true)

class public OttoEnemy : Enemy
    def OttoEnemy(X,Y:int)
        Enemy`Enemy(self, X, Y, 364, [{auto 366; 367; 368; 369}], true)

class public GretelEnemy : Enemy
    def GretelEnemy(X,Y:int)
        Enemy`Enemy(self, X, Y, 389, [{auto 393; 394; 395; 392}], true)

class public FettgesichtEnemy : Enemy
    def FettgesichtEnemy(X,Y:int)
        Enemy`Enemy(self, X, Y, 400, [{auto 404; 405; 406; 407}], true)
*/