#pragma once

namespace das {
    template <> struct das_alias<sf::Vector2i> : das_alias_vec<sf::Vector2i,int2> {};
    template <> struct das_alias<sf::Vector2u> : das_alias_vec<sf::Vector2u,uint2> {};
    template <> struct das_alias<sf::Vector2f> : das_alias_vec<sf::Vector2f,float2> {};


    void sfml_window_close ( sf::Window & win );

    void sflm_with_render_target ( sf::RenderWindow & win, const TBlock<void,sf::RenderTarget> & block, Context * context, LineInfoArg * at );

    const sf::RenderStates & sfml_render_states_default();

    void sfml_with_transformable ( sf::Shape & shape, const TBlock<void,sf::Transformable> & block, Context * context, LineInfoArg * at );
}