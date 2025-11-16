#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class RoundedRectangle : public sf::Drawable
{
private:
	sf::Vector2f size; // Wymiary prostok¹ta (szerokoœæ i wysokoœæ).
	float radius; // Promieñ zaokr¹glenia rogów.
	float outlineThickness = 0.f; // Gruboœæ obrysu (domyœlnie brak).
	sf::Color fillColor = sf::Color::White; // Kolor wype³nienia (domyœlnie bia³y).
	sf::Color outlineColor = sf::Color::White; // Kolor obrysu (domyœlnie bia³y).
	sf::Vector2f position = { 0.f, 0.f }; // Pozycja górnego lewego rogu.

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Wewnêtrzna implementacja rysowania, wymagana przez sf::Drawable - odbywa siê w niej ca³a z³o¿ona logika rysowania kó³ i prostok¹tów.

public:
	RoundedRectangle(sf::Vector2f size, float radius); // Konstruktor: przyjmuje rozmiar (sf::Vector2f) i promieñ (float).

	void setPosition(sf::Vector2f pos); // Ustawia pozycjê górnego lewego rogu.
	sf::Vector2f getPosition() const; // Zwraca aktualn¹ pozycjê.

	void setSize(sf::Vector2f s); // Ustawia wymiary.
	sf::Vector2f getSize() const; // Zwraca wymiary.

	void setRadius(float r); // Ustawia promieñ zaokr¹glenia.

	void setFillColor(const sf::Color& color); // Ustawia kolor wype³nienia.
	sf::Color getFillColor() const; // Zwraca aktualny kolor wype³nienia (u¿ywane do pobrania bazowego koloru dla efektu hover).

	void setOutlineColor(const sf::Color& color); // Ustawia kolor obrysu.
	void setOutlineThickness(float thickness); // Ustawia gruboœæ obrysu.

	sf::FloatRect getGlobalBounds() const; // Zwraca globalne granice (do sprawdzania najechania mysz¹ i kolizji).
};