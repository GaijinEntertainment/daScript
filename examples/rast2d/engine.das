module engine private

require files
require game
require rast2d

require math


let fov = 1.

def drawWalls ( level:Level; var bmp:Bitmap )
    let pixelWidth = float(bmp.size.x)
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
        var cx = int(floor(player.x))
        var cy = int(floor(player.y))
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
        drawWallPixels(i,t,tx,WallPics[textureIndex],bmp)

var zIndex : array<float>

[jit,hint(unsafe_range_check,noalias=tex,noalias=bmp)]
def drawWallPixels ( i:int; t,tx:float; tex:WallTexture; var bmp:Bitmap )
    let pixelWidth = float(bmp.size.x)
    let pixelHeight = float(bmp.size.y)
    let wallHeight = float(pixelWidth) / (2. * fov)
    var h = wallHeight / (2. * t)
    var yi = int(floor(pixelHeight / 2. - h))
    var yf = (pixelHeight / 2. - h) % 1.
    var stepi = int(floor(h / 32.))
    var stepf = (h / 32.) % 1.
    var texelOffset = min(64 * int(floor(64. * tx)),4096-64)
    unsafe
        /*
        for j in range(0, yi)
            bmp.data[(j             )*bmp.size.x + i] = GamePalette[29]
            bmp.data[(bmp.size.y-1-j)*bmp.size.x + i] = GamePalette[25]
        */
        for j in range(texelOffset,texelOffset+64)
            let col = GamePalette[int(tex.data[j])]
            yf += stepf
            if yf >= 1.
                for k in range(max(yi,0),min(yi+stepi+1,bmp.size.y))
                    bmp.data[(k             )*bmp.size.x + i] = col
                yi += stepi + 1
                yf -= 1.
            else
                for k in range(max(yi,0),min(yi+stepi,bmp.size.y))
                    bmp.data[(k             )*bmp.size.x + i] = col
                yi += stepi

def drawBackground ( var bmp:Bitmap )
    var ScreenWidth = bmp.size.x
    var ScreenHeight = bmp.size.y
    var CenterX = ScreenWidth / 2
    var CenterY = ScreenHeight / 2
    fill_rect(bmp, 0, 0, ScreenWidth, CenterY, GamePalette[29])
    fill_rect(bmp, 0, CenterY, ScreenWidth, ScreenHeight - CenterY, GamePalette[25])

def public draw ( level:Level; var bmp:Bitmap )
    zIndex |> resize(bmp.size.x)
    drawBackground(bmp)
    drawWalls(level, bmp)
