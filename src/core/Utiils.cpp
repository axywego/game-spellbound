#include "Utils.hpp"

std::optional<sf::Vector2f> findIntersection(
    const sf::FloatRect& rect,
    const sf::Vector2f& lineStart,
    const sf::Vector2f& lineEnd
) {
    if (rect.contains(lineStart)) return lineStart;
    if (rect.contains(lineEnd)) return lineEnd;

    const sf::Vector2f rectTopLeft(rect.position.x, rect.position.y);
    const sf::Vector2f rectTopRight(rect.position.x + rect.size.x, rect.position.y);
    const sf::Vector2f rectBottomRight(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
    const sf::Vector2f rectBottomLeft(rect.position.x, rect.position.y + rect.size.y);

    std::optional<sf::Vector2f> intersection;

    intersection = lineIntersection(lineStart, lineEnd, rectTopLeft, rectTopRight);
    if (intersection) return intersection;

    intersection = lineIntersection(lineStart, lineEnd, rectTopRight, rectBottomRight);
    if (intersection) return intersection;

    intersection = lineIntersection(lineStart, lineEnd, rectBottomRight, rectBottomLeft);
    if (intersection) return intersection;

    intersection = lineIntersection(lineStart, lineEnd, rectBottomLeft, rectTopLeft);
    if (intersection) return intersection;

    return std::nullopt;
}

std::optional<sf::Vector2f> lineIntersection(
    const sf::Vector2f& A1, const sf::Vector2f& A2,
    const sf::Vector2f& B1, const sf::Vector2f& B2
) {
    const float denom = (A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x);
    if (denom == 0) return std::nullopt;

    const float t = ((A1.x - B1.x) * (B1.y - B2.y) - (A1.y - B1.y) * (B1.x - B2.x)) / denom;
    const float u = -((A1.x - A2.x) * (A1.y - B1.y) - (A1.y - A2.y) * (A1.x - B1.x)) / denom;

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
