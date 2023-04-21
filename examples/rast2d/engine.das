module engine private

require files
require game
require rast2d
require entities

require math

[jit,hint(unsafe_range_check,noalias=level,noalias=bmp)]
def drawWalls ( level:Level; var bmp:Bitmap8 )
    for i in range(bmp.size.x)
        // current column position on the camera plane
        let shift = fov * (float(i << 1) - pixelWidth) / (pixelWidth)
        // direction of the ray
        var dx = player.dx - shift * player.dy
        var dy = player.dy + shift * player.dx
        var tres = trace(player.x,player.y,dx,dy,FLT_MAX)
        zIndex[i] = tres.t
        drawWallPixels(i,tres.t,tres.tx,WallPics[tres.textureIndex],bmp) // causes bug in jit. why?

[jit,hint(unsafe_range_check,noalias=tex,noalias=bmp)]
def drawWallPixels ( i:int; t,tx:float; tex:WallTexture; var bmp:Bitmap8 )
    var h = wallHeight / (2. * t)
    let y = floori(pixelHeight / 2. - h)
    let ymin = max(0, y)
    let ymax = min(floori(pixelHeight / 2. + h), bmp.size.x-1)
    let dh = 0.5 /  (h+1.)
    var uvY = (float(ymin - y) * dh) * 64.0
    let dUVY = dh * 64.0
    let texelOffset = 64 * floori(63. * clamp(tx,0.,1.))
    unsafe
        var span = addr(bmp.data[i*bmp.size.y])
        // memset8(span,29u8,ymin)
        var tspan = addr(tex.data[texelOffset])
        var count = ymax - ymin + 1
        var count4 = count >> 2
        count &= 3
        var PSP = span + ymin
        var uv4 = float4(uvY) + float4(0.,1.,2.,3.) * float4(dUVY)
        var duv4 = float4(dUVY) * 4.
        for P in range(count4)
            u8x4_gather_store(PSP,tspan,int4(uv4))
            PSP += 4
            uv4 += duv4
        uvY = uv4.x
        for P in range(count)
            PSP[P] = tspan[int(uvY)]
            uvY += dUVY
        // memset8(PSP+count,25u8,bmp.size.y - ymax - 1)

[jit,hint(unsafe_range_check,noalias=bmp)]
def drawThings ( var bmp:Bitmap8 )
    let dir = float2(player.dx,player.dy)
    var M11 = 0
    var M21 = 0
    var M12 = 0
    var M22 = 0
    var StartX = 0
    var StartY = 0
    if abs(dir.x) >= abs(dir.y)
        M21 = dir.x > 0. ? -1 : 1
        M12 = dir.y > 0. ? -1 : 1
    else
        M11 = dir.x > 0. ? -1 : 1
        M22 = dir.y > 0. ? -1 : 1
    if M11 < 0 || M21 < 0
        StartX = 63
    if M12 < 0 || M22 < 0
        StartY = 63
    for MY in range(0, 64)
        for MX in range(0, 64)
            var X = MX * M11 + MY * M21 + StartX
            var Y = MX * M12 + MY * M22 + StartY
            let ofs = Y * 64 + X
            if !visibility_plane[ofs]
                continue
            if object_plane[ofs].category==Category nothing && enemy_plane[ofs].category==Category nothing
                continue
            // player relative coordinates
            let prc = getPlayerRelativeCoordinates(X,Y)
            // extra clipping
            if prc.rx < player.radius
                // thing is behind the screen
                continue
            elif abs(prc.ry) > prc.rx + 1.
                // thing is out of field of view
                continue
            if object_plane[ofs].category != Category nothing
                drawThing(prc.rx,prc.ry,prc.orx,prc.ory,object_plane[ofs],bmp)
            if enemy_plane[ofs].category != Category nothing
                drawThing(prc.rx,prc.ry,prc.orx,prc.ory,enemy_plane[ofs],bmp)

def getEntityAngle( Direction : float2 ) : int
    // var Direction = Position - Player.Position
    var EntityAngle = floori((8.0f * atan2(Direction.y, Direction.x) / (2.0f * PI)) + 0.5f)
    if EntityAngle < 0
        EntityAngle = 8 - ((-EntityAngle) & 7)
    return EntityAngle & 7

// [jit,hint(unsafe_range_check,noalias=OBJ,noalias=bmp)]
def drawThing(rx,ry,orx,ory:float; OBJ:Entity; var bmp:Bitmap8 )
    // draw
    let th = wallHeight / rx
    let tx = floori((ry / rx + fov) * wallHeight - th / 2.)
    let ty = floori((pixelHeight - th) / 2.)
    var index = OBJ.spriteIndex
    if OBJ.orientable && (OBJ.animation==null || OBJ.animation.orientable)
        let PlayerAngle = getEntityAngle(float2(orx,ory))
        var Angle = getEntityAngle(float2(OBJ.dox, OBJ.doy))
        index = OBJ.spriteIndex + (Angle - PlayerAngle + 8) % 8
    drawSprite(tx, ty, th, rx, Sprites[index], bmp)

[jit,hint(unsafe_range_check,noalias=tex,noalias=bmp)]
def drawSprite ( x,y: int; h:float; dist:float; sprite:SpriteTexture; var bmp:Bitmap8 )
    unsafe
        let height = int(h)
        let xmin = max(0, x)
        let xmax = min(x+height, bmp.size.x-1)
        if xmin > xmax
            return
        let dh = 1. /  (h+1.)
        var uvX = (float(xmin - x) * dh) * 64.0
        let dUVX = dh * 64.0
        for X in range(xmin,xmax)
            if zIndex[X]>dist
                let col = int(uvX)
                if col >= sprite.firstCol && col <= sprite.lastCol
                    var pspan = addr(bmp.data[X*bmp.size.y])
                    var pixelOffset = sprite.pixelOffset[col]
                    var colOffsetData = reinterpret<uint16?> addr(sprite.data[sprite.colOffset[col]])
                    while true
                        let endRow = int(*colOffsetData) / 2
                        if endRow == 0
                            break
                        let startRow = int(colOffsetData[2]) / 2
                        colOffsetData += 3
                        let ystart = y + floori(float(startRow) * h / 64.)
                        let ymin = max(ystart, 0)
                        let ymax = min(y + floori(float(endRow) * h / 64.), bmp.size.y-1)
                        if ymin < ymax
                            let tspan = addr(sprite.data[pixelOffset])
                            var count = ymax - ymin
                            let count4 = count >> 2
                            count &= 3
                            var PSP = pspan + ymin
                            var uv0 = float(ymin - ystart) * dUVX
                            var uv4 = float4(uv0) + float4(0.,1.,2.,3.) * float4(dUVX)
                            var duv4 = float4(dUVX) * 4.
                            for P in range(count4)
                                u8x4_gather_store(PSP,tspan,int4(uv4))
                                PSP += 4
                                uv4 += duv4
                            var uvY = uv4.x
                            for P in range(count)
                                PSP[P] = tspan[int(uvY)]
                                uvY += dUVX
                        pixelOffset += endRow - startRow
            uvX += dUVX

def draw2DElement ( x,y,w,h:int; var pic:Picture; var bmp:Bitmap8 )
    unsafe
        var u = 0.
        let du = 1. / float(w+1)
        let dv = 1. / float(h+1)
        var pspan = addr(bmp.data[y])
        let count = h & 3
        let count4 = h >> 2
        var V4 = float4(0.,1.,2.,3.) * float4(dv)  * float(pic.height)
        var dv4 = float4(dv) * 4. * float(pic.height)
        var DV = dv * float(pic.height)
        for X in range(x,x+w)
            let px = floori(u * float(pic.width))
            var PSP = pspan + X*bmp.size.y
            var tspan = addr(pic.data[px*pic.height])
            var v4 = V4
            for P in range(count4)
                u8x4_gather_store(PSP,tspan,int4(v4))
                PSP += 4
                v4 += dv4
            var v = v4.x
            for P in range(count)
                PSP[P] = tspan[floori(v)]
                v += DV
            u += du

def drawBackground ( var bmp:Bitmap8 )
    let h = bmp.size.y / 2
    unsafe
        for x in range(bmp.size.x)
            memset8(addr(bmp.data[x*bmp.size.y]),29u8,h)
            memset8(addr(bmp.data[x*bmp.size.y+h]),25u8,h)

def drawWeapon ( var bmp:Bitmap8 )
    let height = bmp.size.y
    drawSprite(
            bmp.size.x / 2 - height / 2,
            0,
            float(height),
            0.,
            Sprites[player.weaponSprite],
            bmp)

var hudFactor = 1.

def drawHUDElement(x,y,w,h:int; var pic:Picture; var bmp:Bitmap8 )
    var minx = max(floori(float(x)*hudFactor), 0)
    var maxx = min(floori(float(x+w)*hudFactor), bmp.size.x)
    var miny = max(floori(float(y)*hudFactor), 0)
    var maxy = min(floori(float(y+h)*hudFactor), bmp.size.y)
    if minx>maxx || miny>maxy
        return
    draw2DElement(minx,miny,maxx-minx,maxy-miny,pic,bmp)

def drawHUDNum(x,y:int; num:int; var bmp:Bitmap8 )
    let txt = "{num}"
    for Ch,i in txt,count()
        drawHUDElement(x+i*8,y,8,16,Pictures[Ch-'0'+105],bmp)

def drawHUD ( var bmp:Bitmap8 )
    // health head (24x32)
    var frame = 0
    if playerHealth <= 0.
        frame = 136  // dead
    elif playerHealth > 1.
        frame = 137 // >100%
    else
        let hpIndex = floori(6.*clamp(1.-playerHealth,0.,1.))
        frame = floori(hudTime * 3.) + hpIndex*3 + 115
    drawHUDElement(1,200-33,24,32,Pictures[frame],bmp)
    // gold and silver keys (8x16)
    if player.goldKey
        drawHUDElement(1+24,200-33,8,16,Pictures[102],bmp)
    if player.silverKey
        drawHUDElement(1+24+8,200-33,8,16,Pictures[103],bmp)
    // ammo
    drawHUDNum(1+24,200-17,player.ammo,bmp)

def public draw ( level:Level; var bmp:Bitmap8 )
    hudFactor = max(float(bmp.size.y)/200.,1.)
    pixelWidth = float(bmp.size.x)
    pixelHeight = float(bmp.size.y)
    wallHeight = float(pixelWidth) / (2. * fov)
    zIndex |> resize(bmp.size.x)
    for v in visibility_plane
        v = false
    drawBackground(bmp)
    drawWalls(level, bmp)
    drawThings(bmp)
    if dLevelFade == 0.
        drawWeapon(bmp)
        drawHUD(bmp)



