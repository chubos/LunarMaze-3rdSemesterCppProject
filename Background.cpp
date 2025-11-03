#include "Background.h"

Background::Background(const std::string& farPath,
    const std::string& nearPath,
    float farSpeed,
    float nearSpeed)
    : speedFar(farSpeed), speedNear(nearSpeed),
    offsetFar(0), offsetNear(0), movingUp(true)
{
    textureFar.loadFromFile(farPath);
    textureNear.loadFromFile(nearPath);

    textureFar.setRepeated(true);
    textureNear.setRepeated(true);

    spriteFar1.setTexture(textureFar);
    spriteFar2.setTexture(textureFar);
    spriteNear1.setTexture(textureNear);
    spriteNear2.setTexture(textureNear);

    spriteFar1.setPosition(0, 0);
    spriteFar2.setPosition(0, -600);
    spriteNear1.setPosition(0, 0);
    spriteNear2.setPosition(0, -600);
}

void Background::update(float dt) {
    // ruch góra-dó³ w pêtli
    float moveFar = speedFar * dt * (movingUp ? -1.f : 1.f);
    float moveNear = speedNear * dt * (movingUp ? -1.f : 1.f);

    offsetFar += moveFar;
    offsetNear += moveNear;

    // zmiana kierunku po kilku sekundach
    if (offsetFar < -200) movingUp = false;
    if (offsetFar > 200) movingUp = true;

    // ustawianie pozycji sprite’ów dla p³ynnego zapêtlenia
    spriteFar1.setPosition(0, offsetFar);
    spriteFar2.setPosition(0, offsetFar - 600);
    spriteNear1.setPosition(0, offsetNear);
    spriteNear2.setPosition(0, offsetNear - 600);

    // reset, jeœli przesuniêcie przekroczy rozmiar tekstury
    if (offsetFar <= -600) offsetFar = 0;
    if (offsetFar >= 600) offsetFar = 0;
    if (offsetNear <= -600) offsetNear = 0;
    if (offsetNear >= 600) offsetNear = 0;
}

void Background::draw(sf::RenderWindow& window) {
    // najpierw dalsza warstwa, potem bli¿sza
    window.draw(spriteFar1);
    window.draw(spriteFar2);
    window.draw(spriteNear1);
    window.draw(spriteNear2);
}
