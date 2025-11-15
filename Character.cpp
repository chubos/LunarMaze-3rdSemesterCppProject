#include "Character.h" // Do³¹czenie pliku nag³ówkowego klasy Character.
#include <algorithm> // Do³¹czenie nag³ówka dla funkcji std::min.

// ## Konstruktor klasy Character
// Wczytuje teksturê, ustawia pozycjê i prêdkoœæ pocz¹tkow¹.
Character::Character(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: position(startPosition), speed(speed) // Inicjalizacja listy: pozycja i prêdkoœæ.
{
	// £adowanie tekstury z pliku.
	texture.loadFromFile(texturePath);
	// Przypisanie wczytanej tekstury do duszka (sprite).
	sprite.setTexture(texture);
	// Ustawienie pocz¹tkowej pozycji duszka.
	sprite.setPosition(position);
}

// ## Skalowanie duszka do rozmiaru kafelka
// Skaluje duszka tak, aby zmieœci³ siê w kafelku o danym rozmiarze (tileSize).
void Character::scaleToTileSize(float tileSize)
{
	if (tileSize <= 0.f) return; // Zabezpieczenie przed niepoprawnym rozmiarem.

	auto texSize = texture.getSize(); // Pobranie oryginalnych wymiarów tekstury.
	if (texSize.x == 0 || texSize.y == 0) return; // Zabezpieczenie przed pust¹ tekstur¹.

	// Wspó³czynnik marginesu, aby duszek nie wype³nia³ ca³ego kafelka.
	const float marginFactor = 0.95f;
	float targetSize = tileSize * marginFactor; // Docelowy maksymalny rozmiar dla duszka.

	// Obliczenie wspó³czynników skalowania dla X i Y.
	float scaleX = targetSize / static_cast<float>(texSize.x);
	float scaleY = targetSize / static_cast<float>(texSize.y);
	// Wybór mniejszego wspó³czynnika skalowania (scaling "contain")
	// aby ca³y duszek zmieœci³ siê w docelowym obszarze.
	float scale = std::min(scaleX, scaleY);

	// Ograniczenie skalowania do maksymalnie 1.0, aby nie powiêkszaæ obrazu,
	// jeœli rozmiar kafelka jest wiêkszy ni¿ oryginalny rozmiar tekstury.
	if (scale > 1.f) scale = 1.f;
	// Zastosowanie obliczonego skalowania do duszka.
	sprite.setScale(scale, scale);

	// Ustawienie pocz¹tku na (0, 0) – górny lewy róg.
	sprite.setOrigin(0.f, 0.f);

	// Obliczenie wymiarów duszka po skalowaniu.
	float scaledW = static_cast<float>(texSize.x) * scale;
	float scaledH = static_cast<float>(texSize.y) * scale;

	// Obliczenie przesuniêcia (offset) w celu wyœrodkowania duszka wewn¹trz kafelka.
	spriteOffset.x = (tileSize - scaledW) * 0.5f;
	spriteOffset.y = (tileSize - scaledH) * 0.5f;

	// Ustawienie koñcowej pozycji duszka (pozycja w œwiecie + wyœrodkowanie).
	sprite.setPosition(position + spriteOffset);
}

// ## Rysowanie postaci
// Rysuje duszka na oknie renderowania, z opcjonalnym przesuniêciem mapy/kamery.
void Character::draw(sf::RenderWindow& window, sf::Vector2f offset)
{
	sf::RenderStates states; // Stan renderowania.
	states.transform.translate(offset); // Dodanie przesuniêcia do transformacji.
	window.draw(sprite, states); // Rysowanie duszka z uwzglêdnieniem przesuniêcia.
}

// ## Pobieranie pozycji
// Zwraca pozycjê w œwiecie gry (górny lewy róg logicznej pozycji).
sf::Vector2f Character::getPosition() const
{
	return position;
}

// ## Pobieranie granic
// Zwraca globaln¹ ramkê ograniczaj¹c¹ duszka (do kolizji).
sf::FloatRect Character::getBounds() const
{
	return sprite.getGlobalBounds();
}

// ## Ustawianie pozycji
// Ustawia now¹ pozycjê postaci w œwiecie gry i aktualizuje duszka.
void Character::setPosition(sf::Vector2f newPosition)
{
	position = newPosition; // Aktualizacja logicznej pozycji.
	// Aktualizacja pozycji duszka (z uwzglêdnieniem offsetu wyœrodkowania).
	sprite.setPosition(position + spriteOffset);
}