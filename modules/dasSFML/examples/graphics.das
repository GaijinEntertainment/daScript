require sfml/sfml_boost

[export]
def main
    using(VideoMode(640u,480u), "SFML window", uint(WindowStyle Default), ContextSettings()) <| $ ( var window : RenderWindow# )
        while window |> isOpen()
            var event : Event
            while window |> pollEvent(event)
                if event._type == EventType Closed
                    window |> close()
            interface(window) <| $ ( var rt : RenderTarget )
                rt |> clear( Color(64u,0u,0u,255u) )
                using(50.0f,0x1eul) <| $ ( var shape : CircleShape# )
                    interface(shape) <| $ ( var tr : Transformable )
                        tr |> move(20.,30.)
                    shape |> setFillColor(Color(150u,50u,250u,255u))
                    shape |> setOutlineThickness(10.f)
                    shape |> setOutlineColor(Color(250u,150u,100u,255u))
                    rt |> draw(shape, RenderStates_Default())
            window |> display()
