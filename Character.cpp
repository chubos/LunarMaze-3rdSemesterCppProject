#include "Character.h"
#include <algorithm>

Character::Character(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: position(startPosition), speed(speed) // Inicjalizacja listy: pozycja i prêdkoœæ.
{
	texture.loadFromFile(texturePath); // £adowanie tekstury z pliku.
	sprite.setTexture(texture); // Przypisanie wczytanej tekstury do duszka (sprite).
	sprite.setPosition(position); // Ustawienie pocz¹tkowej pozycji duszka.
}


void Character::scaleToTileSize(float tileSize) // Skaluje duszka tak, aby zmieœci³ siê w kafelku o danym rozmiarze (tileSize).
{
	if (tileSize <= 0.f) return; // Zabezpieczenie przed niepoprawnym rozmiarem.

	auto texSize = texture.getSize(); // Pobranie oryginalnych wymiarów tekstury.
	if (texSize.x == 0 || texSize.y == 0) return; // Zabezpieczenie przed pust¹ tekstur¹.

	const float marginFactor = 0.95f; 	// Wspó³czynnik marginesu, aby duszek nie wype³nia³ ca³ego kafelka.
	float targetSize = tileSize * marginFactor; // Docelowy maksymalny rozmiar dla duszka.

	
	float scaleX = targetSize / static_cast<float>(texSize.x); // Wspó³czynnik skalowania dla X i Y.
	float scaleY = targetSize / static_cast<float>(texSize.y);

	float scale = std::min(scaleX, scaleY); // Wybór mniejszego wspó³czynnika skalowania (scaling "contain"), aby ca³y duszek zmieœci³ siê w docelowym obszarze.

	
	if (scale > 1.f) scale = 1.f; // Ograniczenie skalowania do maksymalnie 1.0, aby nie powiêkszaæ obrazu, jeœli rozmiar kafelka jest wiêkszy ni¿ oryginalny rozmiar tekstury.
	
	sprite.setScale(scale, scale); // Zastosowanie obliczonego skalowania do duszka.

	sprite.setOrigin(0.f, 0.f); // Ustawienie pocz¹tku na (0, 0) – górny lewy róg.

	float scaledW = static_cast<float>(texSize.x) * scale; // Obliczenie wymiarów duszka po skalowaniu.
	float scaledH = static_cast<float>(texSize.y) * scale;

	spriteOffset.x = (tileSize - scaledW) * 0.5f; // Obliczenie przesuniêcia (offset) w celu wyœrodkowania duszka wewn¹trz kafelka.
	spriteOffset.y = (tileSize - scaledH) * 0.5f;

	sprite.setPosition(position + spriteOffset); // Ustawienie koñcowej pozycji duszka (pozycja w œwiecie + wyœrodkowanie).
}



void Character::draw(sf::RenderWindow& window, sf::Vector2f offset) // Rysuje duszka na oknie renderowania
{
	sf::RenderStates states; // Stan renderowania.
	states.transform.translate(offset); // Dodanie przesuniêcia do transformacji.
	window.draw(sprite, states); // Rysowanie duszka z uwzglêdnieniem przesuniêcia.
}



sf::Vector2f Character::getPosition() const // Zwraca pozycjê w œwiecie gry (górny lewy róg logicznej pozycji).
{
	return position;
}



sf::FloatRect Character::getBounds() const // Zwraca globaln¹ ramkê ograniczaj¹c¹ duszka (do kolizji).
{
	return sprite.getGlobalBounds();
}


void Character::setPosition(sf::Vector2f newPosition) // Ustawia now¹ pozycjê postaci w œwiecie gry i aktualizuje duszka.
{
	position = newPosition; // Aktualizacja logicznej pozycji.
	sprite.setPosition(position + spriteOffset); // Aktualizacja pozycji duszka (z uwzglêdnieniem offsetu wyœrodkowania).
}