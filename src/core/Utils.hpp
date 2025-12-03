#pragma once

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

float distance(const sf::Vector2f& a, const sf::Vector2f& b);

sf::Vector2f directivityVector(const sf::Vector2f& a, const sf::Vector2f& b);

float squaredDistance(const sf::Vector2f& a, const sf::Vector2f& b);

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

nlohmann::json getVariablesFromFile(const std::string &fileName);

bool isHasFile(const std::string &fileName);

void deleteFile(const std::string &fileName);