struct Ball
    position : float3
    color : float4

struct BigBall
    position: float3[2]; color: float4[3][4]
    radius: float
    ball: Ball
    pBall: Ball*

struct HollywoodBall {
    position: float3;
    radius:float;
}

struct HugeCurlyBall
{
    position: float3;
    radius:float;
}

let
    a:int
    b:Ball; c:BigBall*

let { d,e:HugeCurlyBall &; f:int[10]; }

let
    g:int&[10]

