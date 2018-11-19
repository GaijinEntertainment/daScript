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

