#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class RoundedRectangle : public sf::Drawable
{
private:
	sf::Vector2f size; // Wymiary prostok¹ta.
	float radius;
	float outlineThickness = 0.f;
	sf::Color fillColor = sf::Color::White;
	sf::Color outlineColor = sf::Color::White;
	sf::Vector2f position = { 0.f, 0.f }; // Pozycja górnego lewego rogu.

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	RoundedRectangle(sf::Vector2f size, float radius);

	void setPosition(sf::Vector2f pos); // Ustawia pozycjê górnego lewego rogu.
	sf::Vector2f getPosition() const;

	void setSize(sf::Vector2f s);
	sf::Vector2f getSize() const;

	void setRadius(float r);

	void setFillColor(const sf::Color& color);
	sf::Color getFillColor() const;

	void setOutlineColor(const sf::Color& color);
	void setOutlineThickness(float thickness);

	sf::FloatRect getGlobalBounds() const; // Zwraca granice (do sprawdzania najechania mysz¹ i kolizji).
};