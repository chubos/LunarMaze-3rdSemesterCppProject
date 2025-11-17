#include "RoundedRectangle.h"
#include <cmath>

RoundedRectangle::RoundedRectangle(sf::Vector2f size, float radius)
	: size(size), radius(radius)
{
}


void RoundedRectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	float r = std::min(radius, std::min(size.x, size.y) / 2.f); // Ograniczenie promienia, aby nie przekroczy³ po³owy mniejszego wymiaru prostok¹ta.

	// Logika rysowania obrysu
	if (outlineThickness > 0.f) { 
		float orad = r + outlineThickness; // Promieñ zewnêtrzny.
		sf::Vector2f opos(position.x - outlineThickness, position.y - outlineThickness); // Pozycja górny-lewy róg obrysu.
		sf::Vector2f osize(size.x + 2.f * outlineThickness, size.y + 2.f * outlineThickness); // Rozmiar ca³kowity obrysu.

		// Rysowanie zewnêtrznych kó³ek naro¿nych.
		sf::CircleShape oc(orad, 32); // Tworzenie ko³a dla naro¿nika.
		oc.setFillColor(outlineColor); // Ustawienie koloru obrysu.
		oc.setOrigin(orad, orad); // Ustawienie punktu pocz¹tkowego na œrodek ko³a.

		oc.setPosition(opos.x + orad, opos.y + orad); target.draw(oc, states); // Górny lewy róg.
		oc.setPosition(opos.x + osize.x - orad, opos.y + orad); target.draw(oc, states); // Górny prawy róg.
		oc.setPosition(opos.x + orad, opos.y + osize.y - orad); target.draw(oc, states); // Dolny lewy róg.
		oc.setPosition(opos.x + osize.x - orad, opos.y + osize.y - orad); target.draw(oc, states); // Dolny prawy róg.

		// Rysowanie prostok¹tów wype³niaj¹cych obrys.
		sf::RectangleShape oCenter(sf::Vector2f(osize.x - 2.f * orad, osize.y - 2.f * orad)); // Centralny prostok¹t.
		oCenter.setPosition(opos.x + orad, opos.y + orad); oCenter.setFillColor(outlineColor); // Pozycjonowanie i kolor.
		target.draw(oCenter, states); // Rysowanie centralnego elementu obrysu.

		sf::RectangleShape oTop(sf::Vector2f(osize.x - 2.f * orad, orad)); // Górny prostok¹t obrysu.
		oTop.setPosition(opos.x + orad, opos.y); oTop.setFillColor(outlineColor); target.draw(oTop, states);
		sf::RectangleShape oBottom = oTop; // Dolny prostok¹t obrysu.
		oBottom.setPosition(opos.x + orad, opos.y + osize.y - orad); target.draw(oBottom, states);

		sf::RectangleShape oLeft(sf::Vector2f(orad, osize.y - 2.f * orad)); // Lewy prostok¹t obrysu.
		oLeft.setPosition(opos.x, opos.y + orad); oLeft.setFillColor(outlineColor); target.draw(oLeft, states);
		sf::RectangleShape oRight = oLeft; // Prawy prostok¹t obrysu.
		oRight.setPosition(opos.x + osize.x - orad, opos.y + orad); target.draw(oRight, states);
	}

	//Logika rysowania wype³nienia
	// Rysowanie kó³ek naro¿nych.
	sf::CircleShape c(r, 32); // Ko³o o promieniu r.
	c.setFillColor(fillColor); // Kolor wype³nienia.
	c.setOrigin(r, r); // Ustawienie punktu pocz¹tkowego na œrodek ko³a.

	c.setPosition(position.x + r, position.y + r); target.draw(c, states); // Górny lewy róg.
	c.setPosition(position.x + size.x - r, position.y + r); target.draw(c, states); // Górny prawy róg.
	c.setPosition(position.x + r, position.y + size.y - r); target.draw(c, states); // Dolny lewy róg.
	c.setPosition(position.x + size.x - r, position.y + size.y - r); target.draw(c, states); // Dolny prawy róg.

	// Rysowanie prostok¹tów wype³niaj¹cych œrodek.
	sf::RectangleShape center(sf::Vector2f(size.x - 2.f * r, size.y - 2.f * r)); // Centralny prostok¹t wype³nienia.
	center.setPosition(position.x + r, position.y + r); center.setFillColor(fillColor); // Pozycjonowanie.
	target.draw(center, states); // Rysowanie œrodka.

	sf::RectangleShape top(sf::Vector2f(size.x - 2.f * r, r)); // Górny prostok¹t wype³nienia.
	top.setPosition(position.x + r, position.y); top.setFillColor(fillColor); target.draw(top, states);
	sf::RectangleShape bottom = top; // Dolny prostok¹t wype³nienia.
	bottom.setPosition(position.x + r, position.y + size.y - r); target.draw(bottom, states);

	sf::RectangleShape left(sf::Vector2f(r, size.y - 2.f * r)); // Lewy prostok¹t wype³nienia.
	left.setPosition(position.x, position.y + r); left.setFillColor(fillColor); target.draw(left, states);
	sf::RectangleShape right = left; // Prawy prostok¹t wype³nienia.
	right.setPosition(position.x + size.x - r, position.y + r); target.draw(right, states);
}


void RoundedRectangle::setPosition(sf::Vector2f pos) { position = pos; } 
sf::Vector2f RoundedRectangle::getPosition() const { return position; } 

void RoundedRectangle::setSize(sf::Vector2f s) { size = s; } 
sf::Vector2f RoundedRectangle::getSize() const { return size; }

void RoundedRectangle::setRadius(float r) { radius = r; } 

void RoundedRectangle::setFillColor(const sf::Color& color) { fillColor = color; } 
sf::Color RoundedRectangle::getFillColor() const { return fillColor; } 

void RoundedRectangle::setOutlineColor(const sf::Color& color) { outlineColor = color; } 
void RoundedRectangle::setOutlineThickness(float thickness) { outlineThickness = thickness; } 

sf::FloatRect RoundedRectangle::getGlobalBounds() const {
	float t = outlineThickness; 
	return sf::FloatRect(position.x - t, position.y - t, size.x + 2 * t, size.y + 2 * t); // Ramka jest powiêkszona o gruboœæ obrysu na ka¿dym brzegu.
}