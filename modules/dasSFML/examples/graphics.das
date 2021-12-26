require sfml/sfml_boost

[export]
def main
    unsafe
        var window = RenderWindow(VideoMode(640u,480u), "SFML window", uint(WindowStyle Default), ContextSettings())
        while window |> isOpen()
            var event : Event
            while window |> pollEvent(event)
                if event._type == EventType Closed
                    window |> close()
            with_render_target(window) <| $ ( rt )
                rt |> clear( Color(64u,0u,0u) )
                var shape = CircleShape(50.f)
                with_transformable(shape) <| $ ( tr )
                    tr |> move(20.,30.)
                shape |> setFillColor(Color(150u, 50u, 250u))
                shape |> setOutlineThickness(10.f)
                shape |> setOutlineColor(Color(250u, 150u, 100u))
                rt |> draw(shape, RenderStates_Default())
            window |> display()
