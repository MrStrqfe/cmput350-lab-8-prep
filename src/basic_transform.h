// maybe something here...
#include "vector_util.h"
#include <SFML/System/Vector2.hpp>

struct BasicTransform {
    // methods...
    BasicTransform(sf::Vector2f p_scale = {1.0f, 1.0f}, sf::Vector2f p_translation = {0.0f, 0.0f}):
        scale(p_scale), translation(p_translation) {
    }

    sf::Vector2f applyPoint(const sf::Vector2f& p) const {
        // Apply the scale and translation
        sf::Vector2f point;
        point.x = p.x * scale.x + translation.x;
        point.y = p.y * scale.y + translation.y;
        return point; 
    }

    sf::Vector2f applyVector(const sf::Vector2f& v) const {
        // Apply only the scale, translation has no effect on vectors
        sf::Vector2f vector;
        vector.x = v.x * scale.x;
        vector.y = v.y * scale.y;
        return vector;
    }

    bool operator==(const BasicTransform& other) {
        if (this->scale == other.scale && this->translation == other.translation) {
            return true;
        }
        else {
            return false;
        }
    }

    sf::Vector2f scale;
    sf::Vector2f translation;
};
