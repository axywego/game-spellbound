#include "Utils.hpp"

#include <cmath>
#include <fstream>
#include <iostream>

float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}

sf::Vector2f directivityVector(const sf::Vector2f& a, const sf::Vector2f& b) {
    return (a - b) / distance(a, b);
}

float squaredDistance(const sf::Vector2f& a, const sf::Vector2f& b) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

std::optional<sf::Vector2f> findIntersection(
    const sf::FloatRect& rect,
    const sf::Vector2f& lineStart,
    const sf::Vector2f& lineEnd
) {
    if (rect.contains(lineStart)) return lineStart;
    if (rect.contains(lineEnd)) return lineEnd;

    const sf::Vector2f rectPoints[4] = {
        {rect.position.x, rect.position.y},
        {rect.position.x + rect.size.x, rect.position.y},
        {rect.position.x + rect.size.x, rect.position.y + rect.size.y},
        {rect.position.x, rect.position.y + rect.size.y}
    };

    std::optional<sf::Vector2f> closestIntersection;
    float minDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < 4; ++i) {
        const sf::Vector2f& p1 = rectPoints[i];
        const sf::Vector2f& p2 = rectPoints[(i + 1) % 4];

        if (auto intersection = lineIntersection(lineStart, lineEnd, p1, p2)) {
            float dist = squaredDistance(lineStart, *intersection);
            if (dist < minDistance) {
                minDistance = dist;
                closestIntersection = intersection;
            }
        }
    }

    return closestIntersection;
}

std::optional<sf::Vector2f> lineIntersection(
    const sf::Vector2f& A1, const sf::Vector2f& A2,
    const sf::Vector2f& B1, const sf::Vector2f& B2
) {
    const float denom = (A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x);
    if (std::abs(denom) < 1e-6f) return std::nullopt;

    const float t = ((A1.x - B1.x) * (B1.y - B2.y) - (A1.y - B1.y) * (B1.x - B2.x)) / denom;
    const float u = -((A1.x - A2.x) * (A1.y - B1.y) - (A1.y - A2.y) * (A1.x - B1.x)) / denom;

    //why its working?
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        return sf::Vector2f(
            A1.x + t * (A2.x - A1.x),
            A1.y + t * (A2.y - A1.y)
        );
    }

    return std::nullopt;
}

bool isOnScreen(const sf::Sprite& obj, const sf::RenderTarget& window) {
    const sf::FloatRect bounds = obj.getGlobalBounds();

    const sf::View& view = window.getView();
    const sf::Vector2f viewCenter = view.getCenter();
    const sf::Vector2f viewSize = view.getSize();

    const sf::FloatRect viewport({{viewCenter.x - viewSize.x/2, viewCenter.y - viewSize.y/2},
        {viewSize.x, viewSize.y}});

    return bounds.findIntersection(viewport).has_value();
}

bool isOnScreen(const sf::FloatRect& objRect, const sf::RenderTarget& window) {

    const sf::View& view = window.getView();
    const sf::Vector2f viewCenter = view.getCenter();
    const sf::Vector2f viewSize = view.getSize();

    const sf::FloatRect viewport({{viewCenter.x - viewSize.x/2, viewCenter.y - viewSize.y/2},
        {viewSize.x, viewSize.y}});

    return objRect.findIntersection(viewport).has_value();
}

nlohmann::json getVariablesFromFile(const std::string &fileName) {
    std::fstream file(fileName);

    nlohmann::json json = nlohmann::json::parse(file);

    file.close();
    return json;
}

bool isHasFile(const std::string &fileName) {
    std::fstream file(fileName);
    if (file.is_open()) {
        file.close();
        return true;
    }
    file.close();
    return false;
}
