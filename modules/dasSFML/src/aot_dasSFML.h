#pragma once

namespace das {
    template <> struct das_alias<sf::Vector2i> : das_alias_vec<sf::Vector2i,int2> {};
    template <> struct das_alias<sf::Vector2u> : das_alias_vec<sf::Vector2u,uint2> {};
    template <> struct das_alias<sf::Vector2f> : das_alias_vec<sf::Vector2f,float2> {};
}