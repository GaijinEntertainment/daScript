module game

require files
require app
require sound_names
require entities

require daslib/algorithm
require daslib/json_boost
require daslib/random
require math
require fio

var frame_time = 0.66

let LEVEL_FADE_TIME = 1.
let LEVEL_FADE_FACTOR = 1.0 / LEVEL_FADE_TIME / 60.

let PAIN_SOUND_RADIUS = 10.

let DOOR_SOUND_PLAYBACK_RATE = 18000
let DOOR_SOUND_RADIUS = 10.0     // how far away door sound can be heard (in tiles)
let DOOR_OPEN_SPEED = 2.0        // how fast door opens (in tiles per second)
let DOOR_CLOSE_SPPED = 2.0       // how fast door closes (in tiles per second)
let DOOR_STAYS_CLOSED = 2.0      // how long door stays closed (in seconds)

let SHOT_SOUND_RADIUS = 20.

// hud

var hudTime = 0.
var playerHealth = 1.

//

struct Animation
    sprites : array<int>
    spriteIndex : int
    timer : float
    loop : bool
    orientable : bool
    rate : float

def Animation ( SPRITES:array<int>; LOOP:bool=false; ORIENTABLE:bool=false; RATE:float=1.0/6.0 )
    var self : Animation
    with self
        sprites := SPRITES
        spriteIndex = 0
        timer = 0.
        loop = LOOP
        orientable = ORIENTABLE
        rate = RATE
    return <- self

enum Category
    nothing
    prop
    enemy
    deadBody

struct public Entity
    category : Category
    spriteIndex : int
    animation   : Animation?
    orientable  : bool
    dox, doy    : float

var public object_plane : Entity[64*64]

enum EnemyCategory
    guard
    dog
    officer
    ss
    mutant

struct public EnemyClass
    enemyCategory : EnemyCategory
    spriteIndex : int
    deathSprites : array<int>
    damageSprites : array<int>
    attackSprites : array<int>
    orientable : bool
    radius : float
// everything hp
    hp : int
// everything attack
    attackDamage : int
    attackFireRate : float
    attackDistance : float
    attackDelay : float2    // min, max
    attackSound : SoundName

// weaponFireRate : float[4] = [[float 2.5; 2.5; 7.1; 14.2]]

let enemyClasses <- [[EnemyClass
// guard
    hp = 25,
    radius = .25,
    attackDamage = 15,
    attackDistance = 5.,
    attackFireRate = 2.5,
    attackDelay = float2(1., 2.),
    attackSound = SoundName ATKPISTOLSND,
    spriteIndex = 50,
    deathSprites <- [{auto 90; 91; 92; 93; 95}],
    damageSprites <- [{auto 90; 94; 50}],
    attackSprites <- [{auto 96; 97; 98; 97; 50}];
// dog
    hp = 1,
    radius = .3,
    attackDamage = 10,
    attackDistance = 1.5,
    attackFireRate = 3.5,
    attackDelay = float2(0.2, 0.5),
    attackSound = SoundName DOGBARK,
    spriteIndex = 99,
    deathSprites <- [{auto 131; 132; 133; 134}],
    damageSprites <- [{auto 131; 99}],
    attackSprites <- [{auto 135; 136; 137; 136; 99}];
// officer
    hp = 50,
    radius = .3,
    attackDamage = 45,
    attackDistance = 7.,
    attackFireRate = 2.5,
    attackDelay = float2(0.5, 1.),
    attackSound = SoundName ATKPISTOLSND,
    spriteIndex = 238,
    deathSprites <- [{auto 279; 280; 281; 283; 284}],
    damageSprites <- [{auto 279; 282; 238}],
    attackSprites <- [{auto 285; 286; 287; 286; 238}];
// ss
    hp = 50,
    radius = .3,
    attackDamage = 45,
    attackDistance = 7.,
    attackFireRate = 7.1,
    attackDelay = float2(0.2, 0.5),
    attackSound = SoundName ATKMACHINEGUNSND,
    spriteIndex = 138,
    deathSprites <- [{auto 179; 180; 181; 183}],
    damageSprites <- [{auto 179; 182; 138}],
    attackSprites <- [{auto 184; 185; 186; 186; 138}];
// mutant
    hp = 45,
    radius = .4,
    attackDamage = 45,
    attackDistance = 7.,
    attackFireRate = 2.5,
    attackDelay = float2(0.5, 1.),
    attackSound = SoundName ATKGATLINGSND,
    spriteIndex = 187,
    deathSprites <- [{auto 228; 229; 230; 232; 233}],
    damageSprites <- [{auto 228; 231; 187}],
    attackSprites <- [{auto 234; 235; 236; 237; 236; 187}]
]]

struct public Enemy : Entity
    cx, cy : int
    enemyCategory : EnemyCategory
    patrolling : bool
    health : int = 100
    attackTimer : float = 0.

var public enemy_plane : Enemy[64*64]

def startAnimation ( var entity:Entity; ANIMATION:Animation? )
    entity.animation = ANIMATION
    entity.spriteIndex = entity.animation.sprites[0]

var seed = random_seed(13)

let random_take_damage_snd = [[SoundName
    SoundName DEATHSCREAM1SND;
    SoundName DEATHSCREAM2SND;
    SoundName DEATHSCREAM3SND]]

let random_death_snd = [[SoundName
    SoundName DEATHSCREAM7SND;
    SoundName DEATHSCREAM8SND;
	SoundName DEATHSCREAM9SND]]

def takeDamage ( var enemy:Enemy; X,Y:int; DAMAGE:int )
    enemy.health -= DAMAGE
    if enemy.health <= 0
        enemy.category = Category deadBody
        die(enemy)
        play_game_sound_2d(random_death_snd[ random_int(seed) % 3], float2(X,Y)+float2(0.5), PAIN_SOUND_RADIUS)
    else
        if enemy.animation == null
            enemy |> startAnimation(new Animation(enemyClasses[int(enemy.enemyCategory)].damageSprites,false,false))
        play_game_sound_2d(random_take_damage_snd[ random_int(seed) % 3], float2(X,Y)+float2(0.5), PAIN_SOUND_RADIUS)
        rotateTowardsPlayer(enemy)

def die ( var enemy:Enemy )
    enemy.health = -100
    enemy.orientable = false
    enemy |> startAnimation(new Animation(enemyClasses[int(enemy.enemyCategory)].deathSprites))

def getPlayerRelativeCoordinates ( X,Y : int ) : tuple<rx:float;ry:float;orx:float;ory:float>
    let ox = float(X) + 0.5
    let oy = float(Y) + 0.5
    let orx = ox - player.x
    let ory = oy - player.y
    let rx =  orx * player.dx + ory * player.dy
    let ry = -orx * player.dy + ory * player.dx
    return [[auto rx,ry,orx,ory]]

enum WeaponType
    knife = 0
    pistol = 1
    machineGun = 2
    chainGun = 3

struct public Player
    x, y : float
    dx, dy : float
    health : int = 100
    silverKey : bool
    goldKey : bool
    ammo : int = 8
    speed : float = 0.065
    speed_a : float = 0.05
    radius : float = 0.25
    moveForwars : bool = false
    moveBackward : bool = false
    strafeLeft : bool = false
    strafeRight : bool = false
    turnLeft : bool = false
    turnRight : bool = false
    turnAngle : float = 0.0
    weaponSprite : int = 421
    weaponAnimation : Animation?
    weaponType : WeaponType = WeaponType pistol
    hasWeapon : bool[4] = [[bool true; true; false; false ]]
    weaponSounds : SoundName[4] = [[SoundName
        SoundName ATKMACHINEGUNSND;
        SoundName ATKPISTOLSND;
        SoundName ATKMACHINEGUNSND;
        SoundName ATKGATLINGSND]]
    weaponFireRate : float[4] = [[float 2.5; 2.5; 7.1; 14.2]]
    weaponDamage : int[4] = [[int 16; 18; 18; 18]]

def switchWeapon ( var self:Player; WEAPON_TYPE:WeaponType )
    with self
        if !hasWeapon[int(WEAPON_TYPE)]
            return
        weaponType = WEAPON_TYPE
        weaponSprite = 416 + int(weaponType) * 5

def getEnemyPosition ( x,y:int )
    return float2(x,y) + float2(0.5)

def canMoveTo ( self:Player; X,Y:float )
    with self
        let r = radius
        let fx = X % 1.
        let ix = floori(X)
        let fy = Y % 1.
        let iy = floori(Y)
        if collision_plane[iy*64+ix]
            return false
        if enemy_plane[iy*64+ix].category == Category enemy
            let erad = enemyClasses[int(enemy_plane[iy*64+ix].enemyCategory)].radius
            let dist = length(getEnemyPosition(ix,iy) - float2(X,Y))
            if dist < radius + erad
                return false
        if fx < r
            if collision_plane[iy*64+ix-1]
                return false
            if fy < r && collision_plane[(iy-1)*64+ix-1]
                return false
            if fy > 1.-r && collision_plane[(iy+1)*64+ix-1]
                return false
        if fx > 1.-r
            if collision_plane[iy*64+ix+1]
                return false
            if fy < r && collision_plane[(iy-1)*64+ix+1]
                return false
            if fy > 1.-r && collision_plane[(iy+1)*64+ix+1]
                return false
        if fy < r && collision_plane[(iy-1)*64+ix]
            return false
        if fy > 1.-r && collision_plane[(iy+1)*64+ix]
            return false
        return true

def move ( var self:Player; length,sideways:float )
    with self
        let oldx = floori(x)
        let oldy = floori(y)
        let X = x + dx * length - dy * sideways
        let Y = y + dy * length + dx * sideways
        if self |> canMoveTo(X, y)
            x = X
        if self |> canMoveTo(x, Y)
            y = Y
        let newx = floori(x)
        let newy = floori(y)
        if newx != oldx || newy != oldy
            player |> collect(newx, newy)

let treasureAudio = [[AudioName
    AudioName BONUS1SND;
    AudioName BONUS2SND;
    AudioName BONUS3SND;
    AudioName BONUS3SND]]

def collect ( var self:Player; X,Y:int )
    with self
        if object_plane[Y*64+X].category == Category nothing
            return
        var m1 = int(level.plane1[Y*64+X])
        if isCollectible(m1)
            var col : Collectible
            for c in each_enum(type<Collectible>)
                if int(c)==m1
                    col = c
                    break
            if isHpRestore(m1)
                if health < 100
                    object_plane[Y*64+X].category = Category nothing
                    if col==Collectible DogFood
                        health += 4
                        play_game_sound(SoundName FART)
                    elif col==Collectible Food
                        play_game_audio(AudioName HEALTH1SND)
                        health += 10
                    elif col==Collectible Medkit
                        play_game_audio(AudioName HEALTH2SND)
                        health += 24
                    health = min(health, 100)
            elif col==Collectible LifeUp
                health = 200
                object_plane[Y*64+X].category = Category nothing
                play_game_audio(AudioName BONUS1UPSND)
            elif isKey(m1)
                if col==Collectible KeySilver
                    silverKey = true
                elif col==Collectible KeyGold
                    goldKey = true
                object_plane[Y*64+X].category = Category nothing
                play_game_audio(AudioName GETKEYSND)
            elif col==Collectible AmmoClip
                if ammo < 199
                    object_plane[Y*64+X].category = Category nothing
                    ammo = min(199,ammo+8)
                    play_game_audio(AudioName GETAMMOSND)
            elif isTreasure(m1)
                object_plane[Y*64+X].category = Category nothing
                play_game_audio(treasureAudio[m1-52])
            elif col == Collectible MachineGun || col == Collectible Chaingun
                let wep = (col==Collectible MachineGun) ? WeaponType machineGun : WeaponType chainGun
                if !hasWeapon[int(wep)] || ammo<199
                    object_plane[Y*64+X].category = Category nothing
                    ammo = min(199,ammo+25)
                    if !hasWeapon[int(wep)]
                        play_game_audio((col==Collectible MachineGun) ? AudioName GETMACHINESND : AudioName GETGATLINGSND)
                        hasWeapon[int(wep)] = true
                        self |> switchWeapon(wep)

def turn ( var self:Player; alpha:float )
    with self
        let DX = dx * cos(alpha) - dy * sin(alpha)
        dy = dx * sin(alpha) + dy * cos(alpha)
        dx = DX
        let len = sqrt(dx*dx + dy*dy)
        dx /= len
        dy /= len

def update ( var self:Player )
    with self
        if health <= 0
            if is_key_pressed(VK_SPACE) || is_key_pressed(VK_ENTER)
                switchLevel(levelIndex, true)
            return
        var changed = false
        if weaponAnimation == null
            if is_key_pressed(VK_1)
                self |> switchWeapon(WeaponType knife)
            elif is_key_pressed(VK_2)
                self |> switchWeapon(WeaponType pistol)
            elif is_key_pressed(VK_3)
                self |> switchWeapon(WeaponType machineGun)
            elif is_key_pressed(VK_4)
                self |> switchWeapon(WeaponType chainGun)
        if is_key_pressed(VK_RIGHT)
            turnAngle += 1.
        if is_key_pressed(VK_LEFT)
            turnAngle -= 1.
        if turnAngle != 0.
            self |> turn(turnAngle * speed_a * frame_time)
            turnAngle = 0.
            changed = true
        var forward = 0.
        var sideways = 0.
        if is_key_pressed(VK_UP)
            forward += speed
        if is_key_pressed(VK_DOWN)
            forward -= speed
        if is_key_pressed(VK_A)
            sideways -= speed
        if is_key_pressed(VK_D)
            sideways += speed
        if forward != 0.
            if sideways != 0.
                self |> move(frame_time * forward / sqrt(2.), frame_time * sideways / sqrt(2.))
            else
                self |> move(frame_time * forward, 0.)
            changed = true
        elif sideways != 0.
            self |> move(0., frame_time * sideways)
            changed = true
        if weaponAnimation != null
            assume a = weaponAnimation
            a.timer += weaponFireRate[int(weaponType)] / 16. * frame_time
            if a.timer >= 1.
                a.timer = 0.
                if a.spriteIndex >= a.sprites |> length - 1
                    weaponAnimation = null
                    weaponSprite = 416 + int(weaponType) * 5
                else
                    a.spriteIndex += 1
                    weaponSprite = a.sprites[a.spriteIndex]
        if is_key_pressed(VK_SPACE)
            player |> activate()
        if is_key_pressed(VK_LCONTROL) || is_key_pressed(VK_RCONTROL)
            player |> shoot()

def shoot ( var self:Player )
    with self
        if weaponAnimation == null
            if weaponType!=WeaponType knife && ammo==0
                return
            if weaponType == WeaponType knife
                play_game_audio(AudioName ATKKNIFESND)
            else
                ammo --
                play_game_sound(weaponSounds[int(weaponType)])
            weaponAnimation = new Animation([{for frame in range(4); frame+416+int(weaponType)*5}])
            var d = zIndex[int(pixelWidth) / 2]
            if weaponType == WeaponType knife
                d = min(d,1.) // one meter?
            for Y in range(64)
                for X in range(64)
                    let ofs = X + Y*64
                    if !visibility_plane[ofs]
                        continue
                    if enemy_plane[ofs].category != Category enemy
                        continue
                    var prc = getPlayerRelativeCoordinates(X, Y)
                    if prc.rx < 0.
                        continue
                    if prc.rx >= d
                        continue
                    if abs(prc.ry) <= .3
                        enemy_plane[ofs] |> takeDamage(X,Y,weaponDamage[int(weaponType)])

def isTooCloseToDoor(var self:Player; X,Y:int)
    with self
        let ix = floori(x)
        let iy = floori(y)
        if (X==ix) && (Y==iy)
            return true
        if abs(float(X)+0.5-x)<=radius+0.5 && abs(float(Y)+0.5-y)<=radius+0.5
            return true
        return false

def activate ( var self:Player)
    with self
        var X = floori(x)
        var Y = floori(y)
        var DX = 0
        var DY = 0
        if abs(dx) >= abs(dy)
            DX = dx >= 0. ? 1 : -1
            X += DX
        else
            DY = dy >= 0. ? 1 : -1
            Y += DY
        let m0 = int(level.plane0[Y*64+X])
        let m1 = int(level.plane1[Y*64+X])
        if m0==21 && DX!=0
            // elevator
            switchLevel(levelIndex+1)
        elif m0>=90 && m0<=101
            // door
            if m0==92 || m0==93 && !goldKey
                // gold-locked door
                print("gold-locked door\n")
                return
            if m0==94 || m0==95 && !silverKey
                // silver-locked door
                print("silver-locked door\n")
                return
            assume timer = timer_plane[Y*64+X]
            if !timer.active
                timer = [[Timer
                    t = 0.,
                    opening = true,
                    active = true,
                    isDoor = true
                ]]
                play_game_sound_2d(SoundName OPENDOORSND, float2(X,Y)+float2(0.5), DOOR_SOUND_RADIUS, DOOR_SOUND_PLAYBACK_RATE)
        elif m1 == 98
            // pushwall
            assume timer = timer_plane[Y*64+X]
            if !timer.active && int(level.plane0[(Y+DY)*64+X+DX]) >= 106
                // there is no active timer for this wall, and it can move backwards
                timer = [[Timer
                    t = 0.,
                    dx = DX,
                    dy = DY,
                    steps = 2,
                    opening = true,
                    active = true,
                    isWall = true
                ]]
                play_game_sound_2d(SoundName OPENDOORSND, float2(X,Y)+float2(0.5), DOOR_SOUND_RADIUS)
        else
            assume timer = timer_plane[Y*64+X]
            if !timer.active
                timer = [[Timer
                    t = 0.5,
                    active = true
                ]]
                play_game_audio_2d(AudioName DONOTHINGSND, float2(X,Y)+float2(0.5), DOOR_SOUND_RADIUS)

def clearKeys ( var self:Player )
    with self
        goldKey = false
        silverKey = false

def die ( var self:Player )
    with self
        print("YOU DIE!")
        health = 0

def takeDamage ( var self:Player; DAMAGE:int )
    print("TOOK {DAMAGE} DAMAGE\n")
    with self
        flashScreen(float4(0.,1.,1.,1.),float4(255.,0.,0.,0.), 3. / 60.)
        health -= DAMAGE
        if health <= 0
            self |> die()
            play_game_sound_2d(random_death_snd[ random_int(seed) % 3], float2(x,y), PAIN_SOUND_RADIUS)
        else
            play_game_sound_2d(random_take_damage_snd[ random_int(seed) % 3], float2(x,y), PAIN_SOUND_RADIUS)

var public player = Player()

struct Timer
    t : float
    dx, dy : int
    steps : int
    opening : bool
    active : bool
    isDoor : bool
    isWall : bool

var timer_plane : Timer[64*64]

var collision_plane : bool[64*64]

var visibility_plane : bool[64*64]

var public zIndex : array<float>

var fov = 1.
var pixelWidth = 320.
var pixelHeight = 200.
var wallHeight = float(pixelWidth) / (2. * fov)

var public level : Level
var public levelIndex = 0
var public nextLevelIndex = 0
var public levelFade = 1.
var public dLevelFade = 0.
var public becauseOfDeath = false

var flashFactor = float4(1.,1.,1.,1.)
var flashBase = float4(0.,0.,0.,0.)
var flashTime = 0.

def switchLevel ( index:int; reasonBecauseOfDeath = false )
    becauseOfDeath = reasonBecauseOfDeath
    play_game_sound(SoundName LEVELDONESND)
    levelFade = 1.
    dLevelFade = - LEVEL_FADE_FACTOR
    nextLevelIndex = index

def flashScreen ( ff,fb:float4; ft:float )
    flashFactor = ff
    flashBase = fb
    flashTime = ft

def updatePalette
    // udpate flash
    if flashTime > 0.
        flashTime -= 1./60. * frame_time
        if flashTime < 0.
            flashFactor = float4(1.,1.,1.,1.)
            flashBase = float4(0.,0.,0.,0.)
            flashTime = 0.
    if dLevelFade != 0.
        // fade between levels
        var pal : uint[256]
        if becauseOfDeath && dLevelFade < 0.
            for p,d in GamePalette, pal
                d = pack_float_to_byte ( float4(255.,0.,0.,255.0) * levelFade )
        else
            for p,d in GamePalette, pal
                d = pack_float_to_byte ( unpack_byte_to_float(p) * levelFade )
        set_palette(pal)
        levelFade += dLevelFade * frame_time
        if dLevelFade > 0. && levelFade >= 1.
            dLevelFade = 0.
            levelFade = 1.
        elif dLevelFade < 0. && levelFade <= 0.
            levelIndex = nextLevelIndex
            setupLevel(nextLevelIndex)
            levelFade = 0.
            dLevelFade = LEVEL_FADE_FACTOR
    elif player.health <= 25 || flashTime > 0.
        // health fading to red + flash
        var factor = max(0., 1. - float(player.health) / 25.)
        factor = sqrt(factor)
        let ifactor = 1. - factor
        var pal : uint[256]
        for p,d in GamePalette,pal
            let fc = unpack_byte_to_float(p) * flashFactor + flashBase
            let tc = float4(255.,fc.y*ifactor,fc.z*ifactor,fc.w)
            let rc = lerp(fc, tc, float4(factor))
            d = pack_float_to_byte ( clamp(rc, float4(0.), float4(255.)) )
        set_palette(pal)
    else
        set_palette(GamePalette)

def setupLevel ( index:int )
    to_log(LOG_INFO, "LOADING LEVEL {index}\n}")
    level := Levels[index]
    delete object_plane
    delete enemy_plane
    for b in collision_plane
        b = false
    for t in timer_plane
        t.active = false
    if becauseOfDeath
        player = Player()
    else
        player |> clearKeys()
    for y in range(64)
        for x in range(64)
            // structual
            let m0 = int(level.plane0[x + y * 64])
            if m0 <= 63
                // wall
                collision_plane[x + y * 64] = true
            elif 90 <= m0 && m0 <= 101
                // door
                collision_plane[x + y * 64] = true
            // entities
            let m1 = int(level.plane1[x + y * 64])
            if isPlayer(m1)
                // player
                player.x = float(x) + 0.5
                player.y = float(y) + 0.5
                getEntityDirection(m1-19,player.dx,player.dy)   // note. why is it dx,dy?
            elif isProp(m1)
                object_plane[x + y * 64] = [[Entity
                    category = Category prop,
                    spriteIndex = m1-21
                ]]
                if isBlocking(m1)
                    collision_plane[x + y * 64] = true
            elif isPushWall(m1)
                pass
                // pushwall
                // score.totalSecrets ++
            elif isDeadGuard(m1)
                // dead guard
                object_plane[x + y * 64] = [[Entity
                    category = Category prop,
                    spriteIndex = 95
                ]]
            elif isEnemy(m1)
                var category : EnemyCategory
                var patrolling = false
                var orientable = true
                if isStandingGuard(m1)
                    category = EnemyCategory guard
                elif isPatrollingGuard(m1)
                    category = EnemyCategory guard
                    patrolling = true
                elif isStandingDog(m1)
                    category = EnemyCategory dog
                elif isPatrollingDog(m1)
                    category = EnemyCategory dog
                    patrolling = true
                elif isStandingSS(m1)
                    category = EnemyCategory ss
                elif isPatrollingSS(m1)
                    category = EnemyCategory ss
                    patrolling = true
                elif isStandingMutant(m1)
                    category = EnemyCategory mutant
                elif isPatrollingMutant(m1)
                    category = EnemyCategory mutant
                    patrolling = true
                // to_log(LOG_INFO, "{x} {y} {m1} {category} {patrolling} {orientable}\n")
                // todo: add bosses
                var edx, edy : float
                getEntityDirection(m1 & 3,edy,edx)
                enemy_plane[x + y * 64] = [[Enemy
                    cx = x, cy = y,
                    dox = edx, doy = edy,
                    category = Category enemy,
                    enemyCategory = category,
                    patrolling = patrolling,
                    spriteIndex = enemyClasses[int(category)].spriteIndex,
                    health = enemyClasses[int(category)].hp,
                    orientable = orientable
                ]]
            /*
            elif m1 >= 108

                elif m1==160
                    things |> push <| new FakeHitlerEnemy(x,y)
                elif m1==178
                    things |> push <| new HitlerEnemy(x,y)
                elif m1==179
                    things |> push <| new FettgesichtEnemy(x,y)
                elif m1==196
                    things |> push <| new SchabbsEnemy(x,y)
                elif m1==197
                    things |> push <| new GretelEnemy(x,y)
                elif m1==198
                    things |> push <| new HansEnemy(x,y)
                elif m1==199
                    things |> push <| new OttoEnemy(x,y)
                elif m1>=224 && m1<228
                    // ghost
                    var ghost = new Thing(x,y,0)
                    let spriteIndex = 288 + 2 * (m1-224)
                    ghost->startAnimation ( new Animation([{int spriteIndex; spriteIndex + 1}], true) )
                    things |> push <| ghost
            */

def getEntityOrientation(dx,dy:float)
    if abs(dx) > abs(dy)
        return dx > 0. ? 1 : 3
    else
        return dy > 0. ? 2 : 0

def rotateTowardsPlayer ( var self:Enemy )
    let dxy = normalize((float2(self.cx,self.cy)+float2(0.5)) - float2(player.x,player.y))
    self.dox = dxy.x
    self.doy = dxy.y

def shoot ( var self:Enemy )
    with self
        let xy = float2(cx,cy) + float2(0.5)
        var dxy = float2(player.x,player.y) - xy
        let dist = length(dxy)
        dxy /= dist
        let tres = trace(xy.x,xy.y,dxy.x,dxy.y,dist,false)
        if tres.t >= dist
            let attackDamage = enemyClasses[int(enemyCategory)].attackDamage
            let attackDistance = enemyClasses[int(enemyCategory)].attackDistance
            let distanceFactor = clamp(1. -  dist / attackDistance, 0., 1.)
            let randomFactor = random_float(seed) * 0.3 // todo: tune?
            let damage = float(attackDamage) * (distanceFactor + randomFactor)
            let finalDamage = clamp ( int(damage), 3, attackDamage )
            player |> takeDamage(finalDamage)

def attack ( var self:Enemy )
    with self
        assume eclass = enemyClasses[int(enemyCategory)]
        let delay = eclass.attackDelay
        attackTimer = delay.x + random_float(seed)*(delay.y-delay.x)
        let fireRate = eclass.attackFireRate / 16.
        self |> startAnimation(new Animation(eclass.attackSprites,false,false,fireRate))
        play_game_sound_2d(eclass.attackSound, float2(cx,cy)+float2(0.5), PAIN_SOUND_RADIUS)
        rotateTowardsPlayer(self)
        self |> shoot()

def updateThings
    for enemy in enemy_plane
        if enemy.category == Category nothing
            continue
        // very basic AI
        if enemy.category == Category enemy
            enemy.attackTimer = max ( enemy.attackTimer - 1. / 60. * frame_time, 0. )
            if enemy.animation == null && enemy.attackTimer == 0. && player.health > 0
                if visibility_plane[enemy.cx + enemy.cy * 64]   // player sees us, so we may (or may not) be able to see them
                    let xy = float2(enemy.cx,enemy.cy) + float2(0.5)
                    var dxy = float2(player.x,player.y) - xy
                    let dist = length(dxy)
                    let attackDistance = enemyClasses[int(enemy.enemyCategory)].attackDistance
                    if dist>0. && dist<=attackDistance
                        enemy |> attack()
        // animation
        if enemy.animation != null
            enemy.animation.timer += enemy.animation.rate * frame_time
            if enemy.animation.timer >= 1.0
                enemy.animation.timer = 0.
                if enemy.animation.spriteIndex >= enemy.animation.sprites |> length - 1
                    if enemy.animation.loop
                        // animation loops
                        enemy.animation.spriteIndex = 0
                    else
                        // animation ended (delete?)
                        enemy.spriteIndex = enemy.animation.sprites |> back
                        enemy.animation = null
                        return
                else
                    enemy.animation.spriteIndex += 1
                enemy.spriteIndex = enemy.animation.sprites[enemy.animation.spriteIndex]

def updateTimers
    for timer,index in timer_plane,count()
        if !timer.active
            continue
        let X = index & 63
        let Y = index >> 6
        if timer.isDoor
            if timer.opening
                if timer.t < 1.
                    timer.t += DOOR_OPEN_SPEED / 64. * frame_time
                else
                    timer.t += 1. / 60. * frame_time
                if timer.t >= 1.
                    collision_plane[index] = false
                if timer.t >= DOOR_STAYS_CLOSED
                    timer.t = DOOR_STAYS_CLOSED
                    timer.opening = false
            else
                if timer.t < 1.
                    timer.t -= DOOR_CLOSE_SPPED / 64. * frame_time
                else
                    timer.t -= 1./ 60. * frame_time
                if timer.t <= 1.
                    if !collision_plane[index]
                        if player |> isTooCloseToDoor(X, Y)
                            timer.t = DOOR_STAYS_CLOSED
                            // print("TOO CLOSE TO DOOR {player.x} {player.y} \n")
                        else
                            play_game_sound_2d(SoundName CLOSEDOORSND, float2(X,Y)+float2(0.5), DOOR_SOUND_RADIUS, DOOR_SOUND_PLAYBACK_RATE)
                            collision_plane[index] = true
                if timer.t <= 0.
                    timer.t = 0.
                    timer.active = false
        elif timer.isWall
            timer.t += 1. / 60. * frame_time
            if timer.t >= 1.
                let DX = timer.dx
                let DY = timer.dy
                let wallValue = level.plane0[index]
                level.plane0[index] = level.plane0[(X-DX) + (Y-DY) * 64]
                level.plane0[(X+DX) + (Y+DY) * 64] = wallValue
                level.plane1[index] = uint16(0)
                collision_plane[index] = false
                collision_plane[(X+DX) + (Y+DY) * 64] = true
                timer.steps -= 1
                if timer.steps>0 && !collision_plane[(X+2*DX) + (Y+2*DY) * 64]
                    // wall moves one more step
                    level.plane1[(X+DX) + (Y+DY) * 64] = uint16(98)
                    timer.t = 0.
                    timer_plane[(X+DX) + (Y+DY) * 64] = timer
                timer.active = false
        else
            // any random timer
            timer.t -= 1. / 60. * frame_time
            if timer.t <= 0.
                timer.t = 0.
                timer.active = false

struct Location
    x, y, dx, dy : float

var ENABLE_CHEAT_CODEDS = true

def updatePlayer
    player |> update()
    // update hud
    hudTime += 1./60./10. * frame_time
    if hudTime > 1.
        hudTime -= 1.
    playerHealth = clamp(float(player.health) / 100., 0., 1.0)
    // note: the section bellow is for debugging only. It allows to save and load player position
    if !ENABLE_CHEAT_CODEDS
        return
    if is_key_pressed(VK_F1)
        to_log(LOG_INFO, "player was given everything\n")
        player.goldKey = true
        player.silverKey = true
        player.hasWeapon[int(WeaponType machineGun)] = true
        player.hasWeapon[int(WeaponType chainGun)] = true
        player.ammo = 199
    if is_key_pressed(VK_F2)
        fopen("{get_das_root()}/temp/player.json","wb") <| $ ( f )
            if f != null
                to_log(LOG_INFO,"player at {player.x} {player.y} {player.dx} {player.dy}\n")
                let loc = JV([[Location x=player.x, y=player.y, dx=player.dx, dy=player.dy]])
                let txt = write_json(loc)
                f |> fwrite(txt)
                to_log(LOG_INFO, "saved\n")
    if is_key_pressed(VK_F3)
        fopen("{get_das_root()}/temp/player.json","rb") <| $ ( f )
            if f != null
                let txt = fread(f)
                var error = ""
                var jv = read_json(txt, error)
                if jv != null
                    to_log(LOG_INFO, "loaded\n")
                    var loc = from_JV(jv, type<Location>)
                    player.x = loc.x
                    player.y = loc.y
                    player.dx = loc.dx
                    player.dy = loc.dy
                    to_log(LOG_INFO,"player at {player.x} {player.y} {player.dx} {player.dy}\n")

    if is_key_pressed(VK_F10)
        switchLevel(levelIndex+1)

def public updateGame(dt:float)
    frame_time = dt / 16.0
    updatePalette()
    if dLevelFade != 0.
        return
    updatePlayer()
    updateTimers()
    updateThings()

def trace ( px,py,pdx,pdy,maxDist:float; writeVis:bool ) : tuple<t:float; tx:float; textureIndex:int>
    // direction of the ray
    var dx = pdx
    var dy = pdy
    // direction in which the ray moves along each axis
    let stepx = dx >= 0. ? 1 : -1
    let stepy = dy >= 0. ? 1 : -1
    // take absolute values of ray direction
    dx = float(stepx) * dx
    dy = float(stepy) * dy
    // cell position of the ray on the map (starting from the player position)
    var cx = floori(px)
    var cy = floori(py)
    // remaining fractional distance from the ray position to the next cell (0 < rfx, rfy <= 1)
    var rfx = stepx > 0 ? 1. - (px % 1.) : px % 1.
    if rfx == 0.
        rfx = 1.
        cx += stepx
    var rfy = stepy > 0 ? 1. - (py % 1.) : py % 1.
    if rfy == 0.
        rfy = 1.
        cy += stepy
    // total time traveled by the ray
    var t = 0.
    // plane0 value of the cell visited by the ray
    var m0 : int
    // coordinate on the wall tile where the ray hit (0 <= tx <= 1)
    var tx : float
    // index of tile to display
    var textureIndex : int
    // ray casting loop
    while t < maxDist
        if writeVis
            visibility_plane[cx+cy*64] = true
        m0 = int(level.plane0[cx+cy*64])
        if m0 <= 63
            // hit a wall
            var wallShift = 0.
            let m1 = int(level.plane1[cx+cy*64])
            if m1 == 98
                // pushwall
                assume timer = timer_plane[cx+cy*64]
                if timer.active
                    wallShift = timer.t
                    if timer.dx != 0
                        // wall moves horizontally
                        if dx * rfy >= dy * wallShift
                            // ray hits wall
                            let dt = wallShift / dx
                            t += dt
                            rfy -= dt * dy
                            rfx -= wallShift
                        else
                            // ray moves to next cell
                            let dt = rfy / dy
                            t += dt
                            rfy = 1.
                            cy += stepy
                            rfx -= dt * dx
                            continue
                    else
                        // wall moves vertically
                        if dy * rfx >= dx * wallShift
                            // ray hits wall
                            let dt = wallShift / dy
                            t += dt
                            rfx -= dt * dx
                            rfy -= wallShift
                        else
                            // ray moves to next cell
                            let dt = rfx / dx
                            t += dt
                            rfx = 1.
                            cx += stepx
                            rfy -= dt * dy
                            continue
            if rfx == 1. - wallShift
                // NS wall
                textureIndex = 2 * m0 - 1
                // fix texture orientation depending on ray direction
                tx = stepx * stepy > 0 ? 1. - rfy : rfy
            else
                // EW wall
                textureIndex = 2 * m0 - 2
                // fix texture orientation depending on ray direction
                tx = stepx * stepy < 0 ? 1. - rfx : rfx
            break
        elif m0 <= 101
            // hit a door
            var doorShfit = 0.
            assume timer = timer_plane[cx+cy*64]
            if timer.active
                doorShfit = timer.t
            if (m0 & 1) == 0
                // NS door
                if rfx >= .5 && (rfx - .5) * dy < rfy * dx
                    // ray hits the central door line
                    let dt = (rfx - .5) / dx
                    t += dt
                    rfy -= dt * dy
                    rfx = .5
                    tx = stepy > 0 ? 1. - rfy : rfy
                    tx -= doorShfit
                    if tx >= 0.
                        // ray hits the door
                        if m0 == 90
                            textureIndex = 99
                        elif m0 == 92
                            textureIndex = 105
                        elif m0 == 94
                            textureIndex = 105
                        elif m0 == 100
                            textureIndex = 103
                        else
                            assert(false,"we should not be here?")
                        break
                if rfx * dy >= rfy * dx
                    // hit the side wall
                    let dt = rfy / dy
                    t += dt
                    rfx -= dt * dx
                    rfy = 1.
                    cy += stepy
                    textureIndex = 100
                    tx = stepx > 0 ? 1. - rfx : rfx
                    break
                else
                    // pass through
                    let dt = rfx / dx
                    t += dt
                    rfy -= dt * dy
                    rfx = 1.
                    cx += stepx
            else
                // EW door
                if rfy >= .5 && (rfy - .5) * dx < rfx * dy
                    // ray hits the central door line
                    let dt = (rfy - .5) / dy
                    t += dt
                    rfx -= dt * dx
                    rfy = .5
                    tx = stepx > 0 ? 1. - rfx : rfx
                    tx -= doorShfit
                    if tx >= 0.
                        // ray hits the door
                        if m0 == 91
                            textureIndex = 98
                        elif m0 == 93
                            textureIndex = 104
                        elif m0 == 95
                            textureIndex = 104
                        elif m0 == 101
                            textureIndex = 102
                        else
                            assert(false,"we should not be here?")
                        break
                if rfy * dx >= rfx * dy
                    // hit the side wall
                    let dt = rfx / dx
                    t += dt
                    rfy -= dt * dy
                    rfx = 1.
                    cx += stepx
                    textureIndex = 101
                    tx = stepy > 0 ? 1. - rfy : rfy
                    break
                else
                    // pass through
                    let dt = rfy / dy
                    t += dt
                    rfx -= dt * dx
                    rfy = 1.
                    cy += stepy
        // move to the next cell
        if rfx * dy <= rfy * dx
            // move to next cell horizontally
            let dt = rfx / dx
            t += dt
            rfx = 1.
            cx += stepx
            rfy -= dt * dy
        else
            // move to next cell vertically
            let dt = rfy / dy
            t += dt
            rfy = 1.
            cy += stepy
            rfx -= dt * dx
    return [[auto t, tx, textureIndex]]
