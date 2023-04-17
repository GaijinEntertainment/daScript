module engine private

require files
require game
require rast2d

require math

var fov = 1.
var pixelWidth = 320.
var pixelHeight = 200.

[jit]
def drawWalls ( level:Level; var bmp:Bitmap8 )
    for i in range(bmp.size.x)
        // cast a ray for each screen column
        var isPushwall = false
        // current column position on the camera plane
        let shift = fov * (float(i << 1) - pixelWidth) / (pixelWidth)
        // direction of the ray
        var dx = player.dx - shift * player.dy
        var dy = player.dy + shift * player.dx
        // direction in which the ray moves along each axis
        let stepx = dx >= 0. ? 1 : -1
        let stepy = dy >= 0. ? 1 : -1
        // take absolute values of ray direction
        dx = float(stepx) * dx
        dy = float(stepy) * dy
        // cell position of the ray on the map (starting from the player position)
        var cx = floori(player.x)
        var cy = floori(player.y)
        // remaining fractional distance from the ray position to the next cell (0 < rfx, rfy <= 1)
        var rfx = stepx > 0 ? 1. - (player.x % 1.) : player.x % 1.
        if rfx == 0.
            rfx = 1.
            cx += stepx
        var rfy = stepy > 0 ? 1. - (player.y % 1.) : player.y % 1.
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
        while true
            m0 = int(level.plane0[cx+cy*64])
            if m0 <= 63
                // hit a wall
                var wallShift = 0.
                let m1 = int(level.plane1[cx+cy*64])
                if m1 == 98
                    isPushwall = true
                    // pushwall
                    let timer = getWallTimer(cx,cy)
                    if timer.active
                        wallShift = timer.t / 64.
                        if timer.dx == 0.
                            // wall moves horizontally
                            if dx * rfy >= dy * wallShift
                                // ray hits wall
                                let dt = wallShift / dx
                                t += dt
                                rfy -= dt * dy
                                rfx -= wallShift
                            else
                                // ray moves to next cell
                                isPushwall = false
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
                                isPushwall = false
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
                let timer = getDoorTimer(cx,cy)
                if timer.active
                    if timer.opening
                        doorShfit = timer.t / 64.
                    else
                        doorShfit = 1. - timer.t / 64.
                if !plane2[cx+cy*64]
                    doorShfit = 1.
                if m0 % 2 == 0
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
        // draw
        zIndex[i] = t
        drawWallPixels(i,t,tx,textureIndex,bmp)
        // drawWallPixels(i,t,tx,WallPics[textureIndex],bmp) // causes bug in jit. why?

var zIndex : array<float>

[jit,hint(unsafe_range_check,noalias=tex,noalias=bmp)]
def drawWallPixels ( i:int; t,tx:float; index:int; var bmp:Bitmap8 )
    let wallHeight = float(pixelWidth) / (2. * fov)
    var h = wallHeight / (2. * t)
    let y = floori(pixelHeight / 2. - h)
    let ymin = max(0, y)
    let ymax = min(floori(pixelHeight / 2. + h), bmp.size.x-1)
    let dh = 0.5 /  h
    var uvY = (float(ymin - y) * dh) * 64.0
    let dUVY = dh * 64.0
    let texelOffset = 64 * floori(63. * tx)
    unsafe
        var span = addr(bmp.data[i*bmp.size.y])
        // memset8(span,29u8,ymin)
        var tspan = addr(WallPics[index].data[texelOffset])
        var count = ymax - ymin + 1
        var count4 = count / 4
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

def drawThings ( var bmp:Bitmap8 )
    let wallHeight = float(pixelWidth) / (2. * fov)
    for t in things
        if t.rx < player.radius
            // thing is behind the screen
            continue
        elif abs(t.ry) > t.rx + 1.
            // thing is out of field of view
            continue
        let th = wallHeight / t.rx
        let tx = floori((t.ry / t.rx + fov) * wallHeight - th / 2.)
        let ty = floori((pixelHeight - th) / 2.)
        var index = t.spriteIndex
        if t.orientable
            index += (int(4. * atan2(t.x - player.x, t.y - player.y) / PI - float(t.direction)) + 16) % 8
        drawSprite(tx, ty, th, t.rx, Sprites[index], bmp)

[jit,hint(unsafe_range_check,noalias=tex,noalias=bmp)]
def drawSprite ( x,y: int; h:float; dist:float; sprite:SpriteTexture; var bmp:Bitmap8 )
    unsafe
        let height = int(h)
        let xmin = max(0, x)
        let xmax = min(x+height, bmp.size.x-1)
        if xmin > xmax
            return
        let dh = 1. /  h
        var uvX = (float(xmin - x) * dh) * 64.0
        let dUVX = dh * 64.0
        for X in range(xmin,xmax)
            if zIndex[X]>dist
                let col = int(uvX)
                if col >= sprite.firstCol && col <= sprite.lastCol
                    var pspan = addr(bmp.data[X*bmp.size.y])
                    var colOffset = sprite.colOffset[col]
                    var pixelOffset = sprite.pixelOffset[col]
                    while true
                        let endRow = int(sprite.data |> getUInt16(colOffset)) / 2
                        if endRow == 0
                            break
                        let startRow = int(sprite.data |> getUInt16(colOffset + 4)) / 2
                        colOffset += 6
                        let ymin = max(y + floori(float(startRow) * h / 64.), 0)
                        let ymax = min(y + floori(float(endRow) * h / 64.), bmp.size.y-1)
                        if ymin < ymax
                            let tspan = addr(sprite.data[pixelOffset])
                            var count = ymax - ymin
                            let count4 = count / 4
                            count &= 3
                            var PSP = pspan + ymin
                            var uv4 = float4(0.,1.,2.,3.) * float4(dUVX)
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

def drawBackground ( var bmp:Bitmap8 )
    let h = bmp.size.y / 2
    unsafe
        for x in range(bmp.size.x)
            memset8(addr(bmp.data[x*bmp.size.y]),29u8,h)
            memset8(addr(bmp.data[x*bmp.size.y+h]),25u8,h)

def public draw ( level:Level; var bmp:Bitmap8 )
    pixelWidth = float(bmp.size.x)
    pixelHeight = float(bmp.size.y)
    zIndex |> resize(bmp.size.x)
    drawBackground(bmp)
    drawWalls(level, bmp)
    drawThings(bmp)

