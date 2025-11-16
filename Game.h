#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Leaderboard.h"
#include "RoundedRectangle.h"

enum class GameState {
	Menu, // Menu g³ówne.
	Playing, // Gra w trakcie.
	LeaderboardView, // Widok tabeli wyników.
	EnterName, // Wprowadzanie imienia gracza po zdobyciu wysokiego wyniku.
	Win, // Stan wygranej.
	GameOver // Stan przegranej.
};


class Game {
	sf::RenderWindow window; // Okno renderowania SFML.
	Map map; // Obiekt mapy gry.
	Player player; // Obiekt gracza.
	std::vector<std::unique_ptr<Enemy>> enemies; // Wektor wrogów (duchów).

	sf::Font font; // Czcionka u¿ywana w grze.
	sf::Text scoreText; // Tekst wyœwietlaj¹cy wynik.
	sf::Text statusText; // Tekst wyœwietlaj¹cy status (np. Wygrana/Przegrana).
	sf::Text titleText; // Tekst tytu³owy w menu.
	sf::Text instructionText; // Tekst z instrukcjami.

	// Elementy UI w Menu
	RoundedRectangle startButton; // Graficzny przycisk Start.
	sf::Text startButtonText; // Tekst na przycisku Start.
	RoundedRectangle leaderboardButton; // Graficzny przycisk Tabela Wyników.
	sf::Text leaderboardButtonText; // Tekst na przycisku Tabela Wyników.

	// Elementy UI w widoku Tabeli Wyników
	Leaderboard leaderboard; // Obiekt zarz¹dzaj¹cy tabel¹ wyników.
	RoundedRectangle backButton; // Przycisk Powrót.
	sf::Text backButtonText; // Tekst na przycisku Powrót.
	RoundedRectangle resetButton; // Przycisk Resetowania Wyników.
	sf::Text resetButtonText; // Tekst na przycisku Resetowania.

	// Elementy UI wprowadzania imienia (EnterName)
	std::string inputName; // Ci¹g znaków przechowuj¹cy wprowadzane imiê.
	int pendingScore = 0; // Wynik oczekuj¹cy na zapisanie.
	sf::Text inputPromptText; // Tekst proœby o wprowadzenie imienia.
	sf::Text inputText; // Tekst wyœwietlaj¹cy wpisywane imiê.
	RoundedRectangle submitButton; // Przycisk ZatwierdŸ.
	sf::Text submitButtonText; // Tekst na przycisku ZatwierdŸ.
	RoundedRectangle cancelButton; // Przycisk Anuluj.
	sf::Text cancelButtonText; // Tekst na przycisku Anuluj.

	GameState prevEndState = GameState::Menu; // Zapamiêtuje poprzedni stan koñcowy (dla anulowania wpisu).

	sf::Texture backgroundTexture; // Tekstura t³a.
	sf::Sprite background; // Sprite t³a.
	sf::Image backgroundImage; // Bufor obrazu t³a (do analizy pikseli).
	bool backgroundImageLoaded = false; // Flaga, czy t³o zosta³o za³adowane.

	sf::Music music; // Muzyka w tle.
	sf::SoundBuffer collectBuffer, loseBuffer, winBuffer; // Bufory krótkich dŸwiêków.
	sf::Sound collectSound, loseSound, winSound; // Krótkie dŸwiêki.

	GameState state = GameState::Menu; // Aktualny stan gry.

	float titleTime = 0.f; // Licznik czasu dla efektów pulsowania tytu³u/kursora.
	float titlePulseSpeed = 2.f; // Szybkoœæ pulsowania tekstu tytu³owego.

	float inputDelay = 0.f; // OpóŸnienie przed akceptacj¹ wprowadzania tekstu.
	float inputDelayDuration = 0.5f; // Czas trwania opóŸnienia w sekundach.

public:
	Game();
	void run(); // G³ówna pêtla gry.

private:
	void handleEvents(); // Metoda obs³uguj¹ca zdarzenia (wejœcie, zamkniêcie okna).
	void update(float dt); // Metoda aktualizuj¹ca logikê gry.
	void draw(); // Metoda rysuj¹ca zawartoœæ na ekranie.
	void resetGame(); // Metoda resetuj¹ca grê do stanu pocz¹tkowego.

	void openLeaderboard(); // Metoda zmieniaj¹ca stan na widok tabeli wyników.
	void submitHighScore(); // Metoda zatwierdzaj¹ca wysoki wynik i zapisuj¹ca go.
	void cancelHighScore(); // Metoda anuluj¹ca wprowadzanie wyniku i przywracaj¹ca stan koñcowy.
};