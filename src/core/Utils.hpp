#pragma once

#include <SFML/Graphics.hpp>

std::optional<sf::Vector2f> findIntersection(
    const sf::FloatRect& rect,
    const sf::Vector2f& lineStart,
    const sf::Vector2f& lineEnd
);

std::optional<sf::Vector2f> lineIntersection(
    const sf::Vector2f& A1, const sf::Vector2f& A2,
    const sf::Vector2f& B1, const sf::Vector2f& B2
);

bool isOnScreen(const sf::Sprite& obj, const sf::RenderTarget& window);

bool isOnScreen(const sf::FloatRect& objRect, const sf::RenderTarget& window);