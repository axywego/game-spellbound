#include "Raycast.hpp"

#include <iostream>

Raycast::Raycast(sf::Sprite& targetSprite_, const Tilemap& tilemap_) : targetSprite(targetSprite_), tilemap(tilemap_) {
    init();
}

void Raycast::init() {
    lines.resize(numOfLines);
    for (int i = 0; i < numOfLines; i++) {
        lines[i].resize(2);
        lines[i].setPrimitiveType(sf::PrimitiveType::Lines);
        lines[i][0].color = sf::Color::White;
        lines[i][1].color = sf::Color::White;
    }
}

void Raycast::setRange(const float &range_) {
    range = range_;
    numOfLines = range / 75 + numOfLines;
    init();
}

void Raycast::update() {
    const float angleStep = 3.14f / 180.f * (360.f / numOfLines);
    const sf::Vector2f lineStart = targetSprite.getPosition();

    std::vector<float> angles(numOfLines);
    for (size_t i = 0; i < numOfLines; ++i) {
        angles[i] = angleStep * i;
    }

    const auto collisionTiles = tilemap.getCollisionTilesInRange(lineStart, range);

    std::mutex linesMutex;
    auto processRays = [&](size_t start, size_t end) {
        for (size_t i = start; i < end; ++i) {
            const float rad = angles[i];
            const sf::Vector2f lineEnd = lineStart + sf::Vector2f(range * cosf(rad), range * sinf(rad));

            sf::Vector2f endPoint = lineEnd;

            for (const auto& tile : collisionTiles) {
                if (const auto intersection = findIntersection(
                    tile.getGlobalBounds(),
                    lineStart,
                    lineEnd
                )) {
                    endPoint = *intersection;
                    break;
                }
            }

            std::lock_guard<std::mutex> lock(linesMutex);
            lines[i][0].position = lineStart;
            lines[i][1].position = endPoint;
        }
    };

    const size_t numThreads = std::thread::hardware_concurrency();
    const size_t raysPerThread = numOfLines / numThreads;

    std::vector<std::thread> threads;
    for (size_t t = 0; t < numThreads; ++t) {
        size_t start = t * raysPerThread;
        size_t end = (t == numThreads - 1) ? numOfLines : start + raysPerThread;
        threads.emplace_back(processRays, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // float rad = 0;
    //
    // const float angleStep = 3.14f / 180.f * (360.f / numOfLines);
    //
    // for (size_t i = 0; i < numOfLines; i++) {
    //     const sf::Vector2f lineStart = targetSprite.getPosition();
    //     const sf::Vector2f lineEnd = lineStart + sf::Vector2f(range * cosf(rad), range * sinf(rad));
    //
    //     lines[i][0].position = lineStart;
    //     lines[i][1].position = lineEnd;
    //
    //     for (const auto& tilesInRange = tilemap.getCollisionTilesInRange(targetSprite.getPosition(), range);
    //         const auto& tile : tilesInRange) {
    //         const auto intersection = findIntersection(
    //             tile.getGlobalBounds(),
    //             lineStart,
    //             lineEnd
    //         );
    //         if (intersection) {
    //             lines[i][1].position = *intersection;
    //             break;
    //         }
    //     }
    //
    //
    //     rad += angleStep;
    // }
}

bool Raycast::isInView(const sf::FloatRect& rect) const {
    return std::any_of(lines.begin(), lines.end(), [&](const sf::VertexArray& line) {
        return findIntersection(rect, line[0].position, line[1].position);
    });
}

void Raycast::render(sf::RenderTarget& renderTarget) const {
    for (int i = 0; i < numOfLines; i++) {
        renderTarget.draw(lines[i]);
    }
}
