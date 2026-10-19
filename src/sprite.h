#ifndef SPRITE_H
#define SPRITE_H

#include "basic_transform.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System.hpp>
#include <array>

class Sprite {  // could derive from sf::Drawable if you're willing to pay for a vptr, but not
                // necessary.
public:
    Sprite(const sf::Texture& texture): 
                mTexture(&texture),
                mMinPointTex(0.0f, 0.0f),
                mMaxPointTex(sf::Vector2f(texture.getSize())) {

        mVertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, 6);
    }

    Sprite(const sf::Texture& texture, const sf::Vector2f& texMinPoint,
           const sf::Vector2f& texMaxPoint):
                mTexture(&texture),
                mMinPointTex(texMinPoint),
                mMaxPointTex(texMaxPoint) {

        mVertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, 6);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // Update the shape
        recomputeVertexArray();
        // Pick the image
        states.texture = mTexture;
        // Sent it to the GPU for drawing
        target.draw(mVertexArray, states);
    }

    sf::Vector2f& position() { return mTransform.translation; }
    const sf::Vector2f& position() const { return mTransform.translation; }
    sf::Vector2f& scale() { return mTransform.scale; }
    const sf::Vector2f& scale() const { return mTransform.scale; }
    sf::Vector2f& origin() { return mOrigin; }
    const sf::Vector2f& origin() const { return mOrigin; }

private:
    // It may help you to write this, which is an array of 6 vertices:
    //   - the first 3 correspond to the vertices of the top-left triangle of a "default rectangle"
    //     ranging from [0, 1] in both dimensions.
    //   - the next 3 correspond to the vertices of the bottom-right triangle. of the default
    //     rectangle.
    static constexpr std::array<sf::Vector2f, 6> DEFAULT_VERTICES = {{
        {0.f, 0.f}, {0.f, 1.f}, {1.f, 0.f}, // Top-left triangle: TL, BL, TR
        {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}  // Bottom-right triangle: BL, BR, TR
    }};

    // if mTexture is valid, recomputes the positions and coords of sf::Vertex for each
    // of 6 possible entries in mVertexArray.
    void recomputeVertexArray() const {
        if (!mTexture) return;

        sf::Vector2f size = mMaxPointTex - mMinPointTex;

        for (std::size_t i = 0; i < 6; i++) {
            // Calculate each corner of the triangle
            sf::Vector2f c = DEFAULT_VERTICES[i];
            // Multiple each element in c with size
            sf::Vector2f inTex = c.componentWiseMul(size);

            mVertexArray[i].texCoords = mMinPointTex + inTex;
            mVertexArray[i].position = mTransform.applyPoint(inTex - mOrigin); // apply scale and translation
        }
    }

    const sf::Texture* mTexture;
    mutable sf::VertexArray mVertexArray;  // geometry, uv coords -- mutable means it can be changed
                                           //  even in const methods.
    sf::Vector2f mMinPointTex;             // AABB for subtexture min point along both x, y
    sf::Vector2f mMaxPointTex;             // AABB for subtexture max point along both x, y
    sf::Vector2f mOrigin;  // position in tex coords of the sprite origin, relative to minPoint
    BasicTransform mTransform;
};

#endif
