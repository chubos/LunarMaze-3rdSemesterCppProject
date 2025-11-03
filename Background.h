#pragma once
#include <SFML/Graphics.hpp>

class Background {
    sf::Texture textureFar;
    sf::Texture textureNear;
    sf::Sprite spriteFar1;
    sf::Sprite spriteFar2;
    sf::Sprite spriteNear1;
    sf::Sprite spriteNear2;

    float speedFar;
    float speedNear;
    float offsetFar;
    float offsetNear;
    bool movingUp;

public:
    Background(const std::string& farPath,
        const std::string& nearPath,
        float farSpeed = 20.f,
        float nearSpeed = 40.f);

    void update(float dt);
    void draw(sf::RenderWindow& window);
};
