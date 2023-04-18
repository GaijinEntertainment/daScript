module game

require files
require app
require sound_names

require daslib/algorithm
require math
require daslib/json_boost
require fio

var frame_time = 1.0

let DOOR_SOUND_PLAYBACK_RATE = 18000
let DOOR_SOUND_RADIUS = 10.0     // how far away door sound can be heard (in tiles)
let DOOR_OPEN_SPEED = 2.0        // how fast door opens (in tiles per second)
let DOOR_CLOSE_SPPED = 2.0       // how fast door closes (in tiles per second)
let DOOR_STAYS_CLOSED = 2.0      // how long door stays closed (in seconds)

struct public Score
    kills : int
    totalKills : int
    treasures : int
    totalTreasures : int
    secrets : int
    totalSecrets : int

var public score : Score

def updateScore
    pass

class public Animation
    sprites : array<int>
    spriteIndex : int
    timer : float
    loop : bool

    def Animation ( SPRITES:array<int>; LOOP:bool = false )
        sprites := SPRITES
        spriteIndex = 0
        timer = 0.
        loop = LOOP

enum Category
    anything
    enemy

class public Thing
    category : Category
    x, y : float                // starting x,y-coordinate on map
    rx, ry : float              // relative to player x,y-coordinate on map
    spriteIndex : int
    collectible : bool          // can be picked up
    orientable : bool           // different sprites based on orientation
    blocking : bool             // blocks movement
    animation : Animation?      // sprite animation ar regular intervals
    direction : int

    def Thing(X,Y:int; SPRITEINDEX:int; COLLECTIBLE:bool = false; ORIENTABLE:bool = false; BLOCKING:bool = false)
        x = float(X) + 0.5
        y = float(Y) + 0.5
        spriteIndex = SPRITEINDEX
        collectible = COLLECTIBLE
        orientable = ORIENTABLE
        blocking = BLOCKING
        category = Category anything

    def startAnimation ( ANIMATION:Animation? )
        animation = ANIMATION
        spriteIndex = animation.sprites[0]

    def update
        rx = x - player.x
        ry = y - player.y
        let RX = rx * player.dx + ry * player.dy
        ry = -rx * player.dy + ry * player.dx
        rx = RX
        if animation != null
            animation.timer += 1. / 6. * frame_time
            if animation.timer >= 1.0
                animation.timer = 0.
                if animation.spriteIndex >= animation.sprites |> length - 1
                    if animation.loop
                        // animation loops
                        animation.spriteIndex = 0
                    else
                        // animation ended (delete?)
                        animation = null
                        return
                else
                    animation.spriteIndex += 1
                spriteIndex = animation.sprites[animation.spriteIndex]

var things : array<Thing?>

class public Enemy : Thing
    deathSprites : array<int>   // sprites to show when enemy dies
    alive : bool

    def Enemy ( X,Y:int; SPRITEINDEX:int; var DEATHSPRITES:array<int>; ORIENTABLE:bool = false; DIRECTION:int = 0 )
        Thing`Thing(self,X,Y,SPRITEINDEX,false,ORIENTABLE)
        deathSprites <- DEATHSPRITES
        direction = DIRECTION
        alive = true
        score.totalKills ++
        category = Category enemy

    def die
        alive = false
        orientable = false
        startAnimation(new Animation(deathSprites))
        score.kills ++

class public GuardEnemy : Enemy
    def GuardEnemy(X,Y:int; DIRECTION:int)
        Enemy`Enemy(self, X, Y, 50, [{auto 90; 91; 92; 93; 95}], true, DIRECTION)

class public DogEnemy : Enemy
    def DogEnemy(X,Y:int; DIRECTION:int)
        Enemy`Enemy(self, X, Y, 99, [{auto 131; 132; 133; 134}], true, DIRECTION)

class public SSEnemy : Enemy
    def SSEnemy(X,Y:int; DIRECTION:int)
        Enemy`Enemy(self, X, Y, 138, [{auto 179; 180; 181; 183}], true, DIRECTION)

class public ZombieEnemy : Enemy
    def ZombieEnemy(X,Y:int; DIRECTION:int)
        Enemy`Enemy(self, X, Y, 187, [{auto 228; 229; 230; 232; 233}], true, DIRECTION)

class public OfficerEnemy : Enemy
    def OfficerEnemy(X,Y:int; DIRECTION:int)
        Enemy`Enemy(self, X, Y, 238, [{auto 279; 280; 281; 283; 284}], true, DIRECTION)

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

class public Player
    x, y : float
    dx, dy : float
    silverKey : bool
    goldKey : bool
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

    def canMoveTo ( X,Y:float )
        let r = radius
        let fx = X % 1.
        let ix = floori(X)
        let fy = Y % 1.
        let iy = floori(Y)
        if plane2[iy*64+ix]
            return false
        if fx < r
            if plane2[iy*64+ix-1]
                return false
            if fy < r && plane2[(iy-1)*64+ix-1]
                return false
            if fy > 1.-r && plane2[(iy+1)*64+ix-1]
                return false
        if fx > 1.-r
            if plane2[iy*64+ix+1]
                return false
            if fy < r && plane2[(iy-1)*64+ix+1]
                return false
            if fy > 1.-r && plane2[(iy+1)*64+ix+1]
                return false
        if fy < r && plane2[(iy-1)*64+ix]
            return false
        if fy > 1.-r && plane2[(iy+1)*64+ix]
            return false
        return true

    def move ( length,sideways:float )
        let oldx = floori(x)
        let oldy = floori(y)
        let X = x + dx * length - dy * sideways
        let Y = y + dy * length + dx * sideways
        if canMoveTo(X, y)
            x = X
        if canMoveTo(x, Y)
            y = Y
        let newx = floori(x)
        let newy = floori(y)
        if newx != oldx || newy != oldy
            //player.collect(newx, newy)
            // shouldDrawMap = true
            pass

    def turn ( alpha:float )
        let DX = dx * cos(alpha) - dy * sin(alpha)
        dy = dx * sin(alpha) + dy * cos(alpha)
        dx = DX

    def update
        var changed = false
        if is_key_pressed(VK_RIGHT)
            turnAngle += 1.
        if is_key_pressed(VK_LEFT)
            turnAngle -= 1.
        if turnAngle != 0.
            turn(turnAngle * speed_a * frame_time)
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
                move(frame_time * forward / sqrt(2.), frame_time * sideways / sqrt(2.))
            else
                move(frame_time * forward, 0.)
            changed = true
        elif sideways != 0.
            move(0., frame_time * sideways)
            changed = true
        if weaponAnimation != null
            assume a = weaponAnimation
            a.timer += 1. / 6. * frame_time
            if a.timer >= 1.
                a.timer = 0.
                if a.spriteIndex >= a.sprites |> length - 1
                    weaponAnimation = null
                    weaponSprite = 421
                else
                    a.spriteIndex += 1
                    weaponSprite = a.sprites[a.spriteIndex]

    def shoot
        if weaponAnimation == null
            play_game_sound(SoundName ATKPISTOLSND)
            weaponAnimation = new Animation([{int 422; 423; 424; 425}])
            let d = 100500. // zIndex[int(pixelWidth) / 2]
            let tl = things |> length
            for I in range(tl)
                let i = tl - I - 1
                var T = things[i]
                if T.category != Category enemy
                    continue
                var t = unsafe(upcast<Enemy?> T)   // TODO: how to do this legally?
                if t.rx < 0.
                    continue
                if t.rx >= d
                    break
                if abs(t.ry) <= .3 && t.alive
                    // flash = new Flash(.5, 0, 0)
                    t->die()
                    return

    def isTooCloseToDoor(X,Y:int)
        let ix = floori(x)
        let iy = floori(y)
        if (X==ix) && (Y==iy)
            return true
        if abs(float(X)+0.5-x)<=radius+0.5 && abs(float(Y)+0.5-y)<=radius+0.5
            return true
        return false

    def activate
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
            loadNextLevel()
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
            assume timer = timers[Y*64+X]
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
            assume timer = timers[Y*64+X]
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
                score.secrets += 1
                updateScore()
                play_game_sound_2d(SoundName OPENDOORSND, float2(X,Y)+float2(0.5), DOOR_SOUND_RADIUS)
        else
            assume timer = timers[Y*64+X]
            if !timer.active
                timer = [[Timer
                    t = 0.5,
                    active = true
                ]]
                play_game_sound_2d(SoundName PUSHWALLSND, float2(X,Y)+float2(0.5), DOOR_SOUND_RADIUS)

var public player = new Player()

struct Timer
    t : float
    dx, dy : int
    steps : int
    opening : bool
    active : bool
    isDoor : bool
    isWall : bool

var timers : Timer[64*64]

var plane2 : bool[64*64]

var public level : Level

def setupLevel ( index:int )
    level := Levels[index]
    // things = []
    for t in timers
        t.active = false
    player = new Player()
    score = Score()
    things |> clear
    for y in range(64)
        for x in range(64)
            // structual
            let m0 = int(level.plane0[x + y * 64])
            if m0 <= 63
                // wall
                plane2[x + y * 64] = true
            elif 90 <= m0 && m0 <= 101
                // door
                plane2[x + y * 64] = true
            // entities
            let m1 = int(level.plane1[x + y * 64])
            if m1>=19 && m1<=22
                // player
                player.x = float(x) + 0.5
                player.y = float(y) + 0.5
                if m1==19
                    player.dx = 0.
                    player.dy = -1.
                elif m1==20
                    player.dx = 1.
                    player.dy = 0.
                elif m1==21
                    player.dx = 0.
                    player.dy = 1.
                elif m1==22
                    player.dx = -1.
                    player.dy = 0.
            elif m1>=23 && m1<=70
                // props
                var collectible = false
                if [{auto 29; 43; 44; 47; 48; 49; 50; 51; 52; 53; 54; 55; 56}] |> find_index(m1) >=0
                    // collectible
                    collectible = true
                    if m1>=52 && m1<=56
                        score.totalTreasures += 1
                if [{auto 24; 25; 26; 28; 30; 31; 33; 34; 35; 36; 39; 40; 41; 45; 58; 59; 60; 62; 63; 68; 69}] |> find_index(m1) >=0
                    // blocking prop
                    things |> push <| new Thing(x,y,m1-21,collectible,false,true)
                    plane2[x + y * 64] = true
                else
                    things |> push <| new Thing(x,y,m1-21,collectible,false,false)
            elif m1==98
                // pushwall
                score.totalSecrets ++
            elif m1==124
                // dead guard
                things |> push <| new Thing(x,y,95)
            elif m1 >= 108
                // enemy
                if m1>=108 && m1<116
                    things |> push <| new GuardEnemy(x,y,(m1-108)%4)
                elif m1>=144 && m1<152
                    things |> push <| new GuardEnemy(x,y,(m1-144)%4)
                elif m1>=116 && m1<124
                    things |> push <| new OfficerEnemy(x,y,(m1-116)%4)
                elif m1>=152 && m1<160
                    things |> push <| new OfficerEnemy(x,y,(m1-152)%4)
                elif m1>=126 && m1<134
                    things |> push <| new SSEnemy(x,y,(m1-126)%4)
                elif m1>=162 && m1<170
                    things |> push <| new SSEnemy(x,y,(m1-162)%4)
                elif m1>=134 && m1<142
                    things |> push <| new DogEnemy(x,y,(m1-134)%4)
                elif m1>=170 && m1<178
                    things |> push <| new DogEnemy(x,y,(m1-170)%4)
                elif m1>=216 && m1<224
                    things |> push <| new ZombieEnemy(x,y,(m1-116)%4)
                elif m1>=234 && m1<242
                    things |> push <| new ZombieEnemy(x,y,(m1-144)%4)
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
    updateScore()

def loadNextLevel
    print("LOAD NEXT LEVEL\n")

def updateThings
    for thing in things
        thing->update()
    things |> sort <| $ ( a,b )
        return b.rx < a.rx

def updateTimers
    for timer,index in timers,count()
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
                    plane2[index] = false
                if timer.t >= DOOR_STAYS_CLOSED
                    timer.t = DOOR_STAYS_CLOSED
                    timer.opening = false
            else
                if timer.t < 1.
                    timer.t -= DOOR_CLOSE_SPPED / 64. * frame_time
                else
                    timer.t -= 1./ 60. * frame_time
                if timer.t <= 1.
                    if !plane2[index]
                        if player->isTooCloseToDoor(X, Y)
                            timer.t = DOOR_STAYS_CLOSED
                            // print("TOO CLOSE TO DOOR {player.x} {player.y} \n")
                        else
                            play_game_sound_2d(SoundName CLOSEDOORSND, float2(X,Y)+float2(0.5), DOOR_SOUND_RADIUS, DOOR_SOUND_PLAYBACK_RATE)
                            plane2[index] = true
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
                plane2[index] = false
                plane2[(X+DX) + (Y+DY) * 64] = true
                timer.steps -= 1
                if timer.steps>0 && !plane2[(X+2*DX) + (Y+2*DY) * 64]
                    // wall moves one more step
                    level.plane1[(X+DX) + (Y+DY) * 64] = uint16(98)
                    timer.t = 0.
                    timers[(X+DX) + (Y+DY) * 64] = timer
                timer.active = false
        else
            // any random timer
            timer.t -= 1. / 60. * frame_time
            if timer.t <= 0.
                timer.t = 0.
                timer.active = false

struct Location
    x, y, dx, dy : float

def updatePlayer
    player->update()
    if is_key_pressed(VK_SPACE)
        player->activate()
    if is_key_pressed(VK_LCONTROL) || is_key_pressed(VK_RCONTROL)
        player->shoot()
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




/*
    // update flashing palette
    if (flash !== undefined) {
        flash.timer += 1;
        if (flash.timer <= flash.duration / 3) {
            flashPalette(flash.red / 2, flash.green / 2, flash.blue / 2);
        } else if (flash.timer <= 2 * flash.duration / 3) {
            flashPalette(flash.red, flash.green, flash.blue);
        } else if (flash.timer <= flash.duration) {
            flashPalette(flash.red / 2, flash.green / 2, flash.blue / 2);
        } else {
            flashPalette(0, 0, 0);
            flash = undefined
        }
    }

    if (fps60) {
        // run at 60fps
        draw();
    } else if (drawNextFrame = !drawNextFrame) {
        // run at 30 fps
        draw();
    }

    // call the function again on next frame
    requestAnimationFrame(update);
}
*/

def public updateGame(dt:float)
    frame_time = dt / 16.0
    updatePlayer()
    updateTimers()
    updateThings()

