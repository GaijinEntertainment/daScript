require sfml/sfml_boost

[export]
def main
    using(VideoMode(640u,480u), "SFML window", uint(WindowStyle Default), ContextSettings()) <| $ ( var window : Window# )
        window |> setPosition(int2(10, 50))
        while window |> isOpen()
            var event : Event
            while window |> pollEvent(event)
                if event._type == EventType Closed
                    window |> close()

