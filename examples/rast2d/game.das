module game

require files

require daslib/algorithm
require math

struct public Score
    kills : int
    totalKills : int
    treasures : int
    totalTreasures : int
    secrets : int
    totalSecrets : int

var public score : Score

struct public Player
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

var public player : Player

struct public WallTimer
    x, y : int
    dx, dy : float
    t : float
    steps : int
    active : bool

var public wallTimers : array<WallTimer>

def getWallTimer ( cx,cy : int )
    for timer in wallTimers
        if timer.x == cx && timer.y == cy
            return timer
    return [[WallTimer]]

struct public DoorTimer
    x, y : int
    opening : bool
    t : float
    steps : int
    active : bool

var public doorTimers : array<DoorTimer>

def public getDoorTimer ( cx,cy : int )
    for timer in doorTimers
        if timer.x == cx && timer.y == cy
            return timer
    return [[DoorTimer]]

var plane2 : bool[64*64]

var public level : Level

def setupLevel ( index:int )
    level := Levels[index]
    // things = []
    doorTimers |> clear
    wallTimers |> clear
    player = Player()
    score = Score()
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
            elif m1==98
                // pushwall
                score.totalSecrets ++


/*
function setupLevel() {
    // setup things
    things = [];
    spriteTextures = [];

            } else if (23 <= m1 && m1 <= 70) {
                // props
                let collectible = false;
                if ([29, 43, 44, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56].indexOf(m1) >= 0) {
                    // collectible
                    collectible = true;
                    if (52 <= m1 && m1 <= 56) {
                        score.totalTreasures += 1;
                    }
                }
                if ([24, 25, 26, 28, 30, 31, 33, 34, 35, 36, 39, 40, 41, 45, 58, 59, 60, 62, 63, 68,
                    69].indexOf(m1) >= 0) {
                    // blocking prop
                    things.push(new Thing(x, y, m1 - 21, collectible, false, true));
                    plane2[x][y] = true;
                } else {
                    things.push(new Thing(x, y, m1 - 21, collectible, false, false));
                }
            } else if (m1 === 98) {
                // pushwall
                score.totalSecrets += 1;
            } else if (m1 === 124) {
                // dead guard
                things.push(new Thing(x, y, 95));
            } else if (m1 >= 108) {
                // enemy
                if ((108 <= m1 && m1 < 116)) {
                    things.push(new GuardEnemy(x, y, (m1 - 108) % 4));
                } else if ((144 <= m1 && m1 < 152)) {
                    things.push(new GuardEnemy(x, y, (m1 - 144) % 4));
                } else if ((116 <= m1 && m1 < 124)) {
                    things.push(new OfficerEnemy(x, y, (m1 - 116) % 4));
                } else if ((152 <= m1 && m1 < 160)) {
                    things.push(new OfficerEnemy(x, y, (m1 - 152) % 4));
                } else if ((126 <= m1 && m1 < 134)) {
                    things.push(new SSEnemy(x, y, (m1 - 126) % 4));
                } else if ((162 <= m1 && m1 < 170)) {
                    things.push(new SSEnemy(x, y, (m1 - 162) % 4));
                } else if ((134 <= m1 && m1 < 142)) {
                    things.push(new DogEnemy(x, y, (m1 - 134) % 4));
                } else if ((170 <= m1 && m1 < 178)) {
                    things.push(new DogEnemy(x, y, (m1 - 170) % 4));
                } else if ((216 <= m1 && m1 < 224)) {
                    things.push(new ZombieEnemy(x, y, (m1 - 116) % 4));
                } else if ((234 <= m1 && m1 < 242)) {
                    things.push(new ZombieEnemy(x, y, (m1 - 144) % 4));
                } else if (m1 === 160) {
                    things.push(new FakeHitlerEnemy(x, y));
                } else if (m1 === 178) {
                    things.push(new HitlerEnemy(x, y));
                } else if (m1 === 179) {
                    things.push(new FettgesichtEnemy(x, y));
                } else if (m1 === 196) {
                    things.push(new SchabbsEnemy(x, y));
                } else if (m1 === 197) {
                    things.push(new GretelEnemy(x, y));
                } else if (m1 === 214) {
                    things.push(new HansEnemy(x, y));
                } else if (m1 === 215) {
                    things.push(new OttoEnemy(x, y));
                } else if (224 <= m1 && m1 < 228) {
                    // Ghost
                    let ghost = new Thing(x, y, 0);
                    let spriteIndex = 288 + 2 * (m1 - 224);
                    ghost.startAnimation(new Animation([spriteIndex, spriteIndex + 1], true));
                    things.push(ghost);
                }
            }
        }
    }

    // if the draw loop hasn't started yet, start it
    if (!isDrawing) {
        isDrawing = true;
        window.requestAnimationFrame(update);
    }

    shouldDrawMap = true;
    updateScore();
}
*/

def canMoveTo ( var self:Player; x,y:float )
    let r = self.radius
    let fx = x % 1.
    let ix = int(floor(x))
    let fy = y % 1.
    let iy = int(floor(y))
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

def move ( var self:Player; length,sideways:float )
    let oldx = int(floor(self.x))
    let oldy = int(floor(self.y))
    let x = self.x + self.dx * length - self.dy * sideways
    let y = self.y + self.dy * length + self.dx * sideways
    if self |> canMoveTo(x, self.y)
        self.x = x
    if self |> canMoveTo(self.x, y)
        self.y = y
    let newx = int(floor(self.x))
    let newy = int(floor(self.y))
    if newx != oldx || newy != oldy
        //player.collect(newx, newy)
        // shouldDrawMap = true
        pass

def turn ( var self:Player; alpha:float )
    let dx = self.dx * cos(alpha) - self.dy * sin(alpha)
    self.dy = self.dx * sin(alpha) + self.dy * cos(alpha)
    self.dx = dx

require app_opengl_glfw

def update ( var self:Player )
    var changed = false
    if is_key_pressed(VK_RIGHT)
        self.turnAngle += 1.
    if is_key_pressed(VK_LEFT)
        self.turnAngle -= 1.
    if self.turnAngle != 0.
        self |> turn(self.turnAngle * self.speed_a)
        self.turnAngle = 0.
        changed = true
    var forward = 0.
    var sideways = 0.
    if is_key_pressed(VK_UP)
        forward += self.speed
    if is_key_pressed(VK_DOWN)
        forward -= self.speed
    if is_key_pressed(VK_A)
        sideways -= self.speed
    if is_key_pressed(VK_D)
        sideways += self.speed
    if forward != 0.
        if sideways != 0.
            self |> move(forward / sqrt(2.), sideways / sqrt(2.))
        else
            self |> move(forward, 0.)
        changed = true
    elif sideways != 0.
        self |> move(0., sideways)
        changed = true
/*
        if (this.weaponAnimation !== undefined) {
            let a = this.weaponAnimation;
            a.timer += 1;
            if (a.timer >= 6) {
                a.timer = 0;
                if (a.spriteIndex >= a.sprites.length - 1) {
                    this.weaponAnimation = undefined;
                    this.weaponSprite = 421;
                } else {
                    a.spriteIndex += 1;
                    this.weaponSprite = a.sprites[a.spriteIndex];
                }
            }
        }
    }
}
*/

def loadNextLevel
    print("LOAD NEXT LEVEL\n")

def activate
    var x = int(floor(player.x))
    var y = int(floor(player.y))
    var dx = 0
    var dy = 0
    if abs(player.dx) >= abs(player.dy)
        dx = player.dx >= 0. ? 1 : -1
        x += dx
    else
        dy = player.dy >= 0. ? 1 : -1
        y += dy
    let m0 = int(level.plane0[y*64+x])
    let m1 = int(level.plane1[y*64+x])
    if m0==21 && dx!=0
        // elevator
        loadNextLevel()
    elif m0>=90 && m0<=101
        // door
        if m0==92 || m0==93 && !player.goldKey
            // gold-locked door
            print("gold-locked door\n")
            return
        if m0==94 || m0==95 && !player.silverKey
            // silver-locked door
            print("silver-locked door\n")
            return
        var timer = getDoorTimer(x,y)
        if !timer.active
            let opening = plane2[y*64+x]
            if !opening
                if ((dx>0 && float(x)-player.x<=player.radius) ||
                    (dx<0 && player.x-float(x)-1.<=player.radius) ||
                    (dy>0 && float(y)-player.y<=player.radius) ||
                    (dy<0 && player.y-float(y)-1.<=player.radius))
                    print("player too close\n")
                    return
                else
                    plane2[y*64+x] = true
            print("adding door timer\n")
            doorTimers |> push <| [[DoorTimer
                x = x,
                y = y,
                t = 0.,
                opening = opening,
                active = true
            ]]
/*
        } else if (m1 === 98) {
            // pushwall
            let timer = wallTimers.find(function (obj) {
                return obj.x === x && obj.y === y;
            });
            if (!timer && map0(x + dx, y + dy) >= 106) {
                // there is no active timer for this wall, and it can move backwards
                wallTimers.push({x: x, y: y, t: 0, dx: dx, dy: dy, steps: 2});
                score.secrets += 1;
                updateScore();
            }
        }
    };
*/

def updateThings
    pass
/*
    for (let i = 0; i < things.length; i++) {
        things[i].update();
    }
    things.sort((a, b) => b.rx - a.rx);
*/

def updateDoorTimers
    var rem : array<int>
    for timer,i in doorTimers,count()
        timer.t += 1.
        if timer.t >= 64.
            rem |> push(i)
            timer.active = false
            if timer.opening
                plane2[timer.y*64+timer.x] = false
    rem |> reverse
    for i in rem
        doorTimers |> erase(i)

def updateWallTimers
    pass

/*
    // update wall timers
    for (let i = 0; i < wallTimers.length; i++) {
        let timer = wallTimers[i];
        timer.t += 1;
        if (timer.t === 64) {
            let x = timer.x;
            let y = timer.y;
            let dx = timer.dx;
            let dy = timer.dy;
            let wallValue = map0(x, y);
            setMap0(x, y, map0(x - dx, y - dy));
            setMap0(x + dx, y + dy, wallValue);
            setMap1(x, y, 0);
            plane2[x][y] = false;
            plane2[x + dx][y + dy] = true;
            timer.steps -= 1;
            if (timer.steps > 0 && !plane2[x + 2 * dx][y + 2 * dy]) {
                // wall moves one more step
                setMap1(x + dx, y + dy, 98);
                timer.t = 0;
                timer.x += dx;
                timer.y += dy;
            } else {
                // wall finished moving
                doorTimers.splice(i, 1);
                i -= 1;
            }
            shouldDrawMap = true;
        }
    }
*/

def updatePlayer
    updateDoorTimers()
    player |> update
    if is_key_pressed(VK_SPACE)
        activate()

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

def public updateGame
    updatePlayer()