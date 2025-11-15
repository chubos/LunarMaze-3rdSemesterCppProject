#include "Game.h" // Do³¹czenie pliku nag³ówkowego klasy Game.
#include "Random.h" // Do³¹czenie klasy wroga: Losowy.
#include "Chaser.h" // Do³¹czenie klasy wroga: Goni¹cy.
#include "Learner.h" // Do³¹czenie klasy wroga: Ucz¹cy siê.
#include <iostream> // Do operacji wejœcia/wyjœcia (np. b³êdy).
#include <algorithm> // Dla funkcji std::min.
#include <array> // Dla kontenera std::array.

// Helper: lighten color for hover
// Funkcja pomocnicza: Rozjaœnia kolor dla efektu najechania mysz¹.
static sf::Color lightenColor(const sf::Color& c, int amount = 30) {
	return sf::Color(
		static_cast<sf::Uint8>(std::min(255, c.r + amount)), // Zwiêkszenie R (ograniczone do 255).
		static_cast<sf::Uint8>(std::min(255, c.g + amount)), // Zwiêkszenie G.
		static_cast<sf::Uint8>(std::min(255, c.b + amount)), // Zwiêkszenie B.
		c.a); // Kana³ alfa bez zmian.
}

// Definicje sta³ych poza konstruktorem, aby by³y dostêpne globalnie
const float BTN_WIDTH = 220.f; // Szerokoœæ g³ównych przycisków menu.
const float BTN_HEIGHT = 56.f; // Wysokoœæ g³ównych przycisków menu.
const float BTN_RADIUS = 28.f; // Promieñ zaokr¹glenia dla g³ównych przycisków.
const float BTN_OUTLINE = 2.f; // Gruboœæ obrysu dla przycisków.

const float BACK_BTN_WIDTH = 160.f; // Szerokoœæ ma³ych przycisków (Back/Reset).
const float BACK_BTN_HEIGHT = 40.f; // Wysokoœæ ma³ych przycisków.
const float BACK_BTN_RADIUS = 12.f; // Promieñ zaokr¹glenia dla ma³ych przycisków.

const float SUBMIT_BTN_WIDTH = 160.f; // Szerokoœæ przycisków wprowadzania imienia.
const float SUBMIT_BTN_HEIGHT = 48.f; // Wysokoœæ przycisków wprowadzania imienia.
const float SUBMIT_BTN_RADIUS = 16.f; // Promieñ zaokr¹glenia dla przycisków wprowadzania imienia.


// Obliczanie pozycji startowych dla przycisków w Menu
const float WINDOW_WIDTH = 2000.f; // Zak³adamy szerokoœæ okna z konstruktora (u¿ywane do obliczeñ statycznych).
const float MENU_BUTTON_Y = 400.f; // Pozycja Y przycisków w Menu.
const float MENU_GAP = 20.f; // Odstêp miêdzy przyciskami w Menu.
const float LB_POS_X = WINDOW_WIDTH / 2.0f + MENU_GAP; // Pozycja X dla przycisku Leaderboard (na prawo od œrodka).
const float START_POS_X = LB_POS_X - MENU_GAP - BTN_WIDTH; // Pozycja X dla przycisku Start (na lewo od Leaderboard).

// ## Konstruktor klasy Game
Game::Game()
// 1. Inicjalizacja sta³ych pól (SFML, Player, Leaderboard)
	: window(sf::VideoMode(int(WINDOW_WIDTH), 1200), "Lunar Maze"), // Utworzenie okna.
	player("assets/player3.png", { 100, 100 }, 150.f), // Inicjalizacja gracza.
	leaderboard(10), // Inicjalizacja tablicy wyników.

	// 2. Inicjalizacja przycisków (RoundedRectangle musz¹ byæ zainicjowane w LI)
	leaderboardButton({ BTN_WIDTH, BTN_HEIGHT }, BTN_RADIUS), // Przygotowanie przycisku Leaderboard.
	startButton({ BTN_WIDTH, BTN_HEIGHT }, BTN_RADIUS), // Przygotowanie przycisku Start.
	backButton({ BACK_BTN_WIDTH, BACK_BTN_HEIGHT }, BACK_BTN_RADIUS), // Przygotowanie przycisku Powrót.
	resetButton({ BACK_BTN_WIDTH, BACK_BTN_HEIGHT }, BACK_BTN_RADIUS), // Przygotowanie przycisku Reset.
	submitButton({ SUBMIT_BTN_WIDTH, SUBMIT_BTN_HEIGHT }, SUBMIT_BTN_RADIUS), // Przygotowanie przycisku ZatwierdŸ.
	cancelButton({ SUBMIT_BTN_WIDTH, SUBMIT_BTN_HEIGHT }, SUBMIT_BTN_RADIUS) // Przygotowanie przycisku Anuluj.
{
	map.loadFromFile("assets/map2.txt"); // £adowanie mapy.

	player.scaleToTileSize(map.getTileSize()); // Skalowanie gracza do rozmiaru kafelka.

	// Inicjalizacja i skalowanie wrogów.
	enemies.push_back(std::make_unique<Random>("assets/ghost1.png", sf::Vector2f(650, 200), 90.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost3.png", sf::Vector2f(750, 400), 90.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost4.png", sf::Vector2f(100, 600), 90.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost4.png", sf::Vector2f(400, 600), 90.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost3.png", sf::Vector2f(1200, 600), 90.f));
	enemies.push_back(std::make_unique<Random>("assets/ghost1.png", sf::Vector2f(1100, 700), 90.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost4.png", sf::Vector2f(100, 600), 90.f));
	for (auto& enemy : enemies) enemy->scaleToTileSize(map.getTileSize());

	// Konfiguracja tekstu (wynik, tytu³, instrukcje, status).
	font.loadFromFile("assets/font.ttf");
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10, 10);

	titleText.setFont(font);
	titleText.setCharacterSize(120);
	titleText.setFillColor(sf::Color::Cyan);
	titleText.setString("Lunar Maze");
	auto titleBounds = titleText.getLocalBounds();
	titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f,
		titleBounds.top + titleBounds.height / 2.0f);
	titleText.setPosition(window.getSize().x / 2.0f, 200.f);

	instructionText.setFont(font);
	instructionText.setCharacterSize(30);
	instructionText.setFillColor(sf::Color(200, 200, 255));
	instructionText.setString("Collect all the crystals (*) while avoiding the drones!\nUse W/A/S/D to move your astronaut.");
	auto instrBounds = instructionText.getLocalBounds();
	instructionText.setOrigin(instrBounds.left + instrBounds.width / 2.0f,
		instrBounds.top + instrBounds.height / 2.0f);
	instructionText.setPosition(window.getSize().x / 2.0f, 320.f);


	statusText.setFont(font);
	statusText.setCharacterSize(40);
	statusText.setFillColor(sf::Color::Yellow);
	statusText.setPosition(200, 250);

	// ### W£ASNOŒCI PRZYCISKÓW MENU ###

	// Przycisk Tabela Wyników (Leaderboard)
	leaderboardButton.setFillColor(sf::Color(50, 50, 70));
	leaderboardButton.setOutlineColor(sf::Color::White);
	leaderboardButton.setOutlineThickness(BTN_OUTLINE);
	leaderboardButton.setPosition({ LB_POS_X, MENU_BUTTON_Y }); // Ustawienie pozycji za pomoc¹ sta³ych.

	leaderboardButtonText.setFont(font);
	leaderboardButtonText.setCharacterSize(24);
	leaderboardButtonText.setFillColor(sf::Color::White);
	leaderboardButtonText.setString("Leaderboard");
	auto lbBounds = leaderboardButtonText.getLocalBounds();
	leaderboardButtonText.setOrigin(lbBounds.left + lbBounds.width / 2.f, lbBounds.top + lbBounds.height / 2.f);
	leaderboardButtonText.setPosition(LB_POS_X + BTN_WIDTH / 2.f, MENU_BUTTON_Y + BTN_HEIGHT / 2.f); // Pozycjonowanie tekstu.

	// Przycisk Start Game
	startButton.setFillColor(sf::Color(40, 120, 180));
	startButton.setOutlineColor(sf::Color::White);
	startButton.setOutlineThickness(BTN_OUTLINE);
	startButton.setPosition({ START_POS_X, MENU_BUTTON_Y }); // Ustawienie pozycji za pomoc¹ sta³ych.

	startButtonText.setFont(font);
	startButtonText.setCharacterSize(24);
	startButtonText.setFillColor(sf::Color::White);
	startButtonText.setString("Start Game");
	auto sbt = startButtonText.getLocalBounds();
	startButtonText.setOrigin(sbt.left + sbt.width / 2.f, sbt.top + sbt.height / 2.f);
	startButtonText.setPosition(START_POS_X + BTN_WIDTH / 2.f, MENU_BUTTON_Y + BTN_HEIGHT / 2.f); // Pozycjonowanie tekstu.

	// ### W£ASNOŒCI PRZYCISKÓW WIDOKU LEADERBOARD/ENTERNAME ###

	// Przycisk Powrót (Back)
	backButton.setFillColor(sf::Color(50, 50, 70));
	backButton.setOutlineColor(sf::Color::White);
	backButton.setOutlineThickness(BTN_OUTLINE);
	backButton.setPosition({ 20.f, 20.f }); // Pozycja w lewym górnym rogu.
	backButtonText.setFont(font);
	backButtonText.setCharacterSize(20);
	backButtonText.setString("Back");
	auto bb = backButtonText.getLocalBounds();
	backButtonText.setOrigin(bb.left + bb.width / 2.f, bb.top + bb.height / 2.f);
	backButtonText.setPosition(backButton.getPosition().x + BACK_BTN_WIDTH / 2.f, backButton.getPosition().y + BACK_BTN_HEIGHT / 2.f);

	// Przycisk Resetowania Wyników (Reset)
	resetButton.setFillColor(sf::Color(120, 40, 40));
	resetButton.setOutlineColor(sf::Color::White);
	resetButton.setOutlineThickness(BTN_OUTLINE);
	resetButtonText.setFont(font);
	resetButtonText.setCharacterSize(18);
	resetButtonText.setFillColor(sf::Color::White);
	resetButtonText.setString("Reset scores");
	auto rb = resetButtonText.getLocalBounds();
	resetButtonText.setOrigin(rb.left + rb.width / 2.f, rb.top + rb.height / 2.f);
	// Pozycja tekstu resetu jest ustawiana dynamicznie w draw().

	// Elementy UI wprowadzania imienia
	inputPromptText.setFont(font);
	inputPromptText.setCharacterSize(28);
	inputPromptText.setFillColor(sf::Color::White);
	inputPromptText.setString("New High Score! Enter your name:");

	inputText.setFont(font);
	inputText.setCharacterSize(36);
	inputText.setFillColor(sf::Color::White);

	// Przycisk ZatwierdŸ (Submit)
	submitButton.setFillColor(sf::Color(40, 160, 40));
	submitButton.setOutlineColor(sf::Color::White);
	submitButton.setOutlineThickness(BTN_OUTLINE);

	submitButtonText.setFont(font);
	submitButtonText.setCharacterSize(24);
	submitButtonText.setFillColor(sf::Color::White);
	submitButtonText.setString("Submit");

	// Przycisk Anuluj (Cancel)
	cancelButton.setFillColor(sf::Color(160, 40, 40));
	cancelButton.setOutlineColor(sf::Color::White);
	cancelButton.setOutlineThickness(BTN_OUTLINE);

	cancelButtonText.setFont(font);
	cancelButtonText.setCharacterSize(24);
	cancelButtonText.setFillColor(sf::Color::White);
	cancelButtonText.setString("Cancel");

	// £adowanie tabeli wyników, t³a i dŸwiêków.
	leaderboard.loadFromFile("scores.txt");

	if (!backgroundTexture.loadFromFile("assets/background5.png"))
		std::cerr << "Nie mozna zaladowac tla!\n";
	background.setTexture(backgroundTexture);
	background.setScale(
		window.getSize().x / background.getGlobalBounds().width,
		window.getSize().y / background.getGlobalBounds().height
	);

	backgroundImage = backgroundTexture.copyToImage();
	backgroundImageLoaded = true;

	if (!music.openFromFile("assets/music.ogg"))
		std::cerr << "Failed to load background music\n";
	else {
		music.setLoop(true);
		music.setVolume(100);
		music.play();
	}

	if (!collectBuffer.loadFromFile("assets/collect.wav"))
		std::cerr << "Failed to load collect.wav\n";
	else {
		collectSound.setBuffer(collectBuffer);
		collectSound.setVolume(10.f);
	}

	if (!loseBuffer.loadFromFile("assets/lose.wav"))
		std::cerr << "Failed to load lose.wav\n";
	else {
		loseSound.setBuffer(loseBuffer);
		loseSound.setVolume(30.f);
	}

	if (!winBuffer.loadFromFile("assets/win.wav"))
		std::cerr << "Failed to load win.wav\n";
	else {
		winSound.setBuffer(winBuffer);
		winSound.setVolume(30.f);
	}
}

// ## G³ówna pêtla gry
void Game::run() {
	sf::Clock clock; // Zegar do mierzenia czasu klatki (dt).
	while (window.isOpen()) {
		float dt = clock.restart().asSeconds(); // Czas klatki.

		handleEvents(); // Obs³uga zdarzeñ.

		if (inputDelay > 0.f) inputDelay -= dt; // Licznik opóŸnienia klawiatury.

		titleTime += dt * titlePulseSpeed; // Aktualizacja czasu do efektu pulsowania/kursora.

		if (state == GameState::Playing)
			update(dt); // Aktualizacja logiki gry.

		draw(); // Rysowanie zawartoœci.
	}
}

// ## Obs³uga zdarzeñ (wejœcie u¿ytkownika)
void Game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close(); // Zamykanie okna.
		}

		// Obs³uga wprowadzania tekstu (dla imienia).
		if (state == GameState::EnterName && event.type == sf::Event::TextEntered) {
			if (inputDelay > 0.f) continue; // Ignorowanie, jeœli jest opóŸnienie.
			if (event.text.unicode >= 32 && event.text.unicode < 128) { // Tylko znaki drukowalne.
				if (inputName.size() < 20)
					inputName.push_back(static_cast<char>(event.text.unicode));
				inputText.setString(inputName);
			}
			continue;
		}

		// Obs³uga wciœniêæ klawiszy.
		if (event.type == sf::Event::KeyPressed) {
			if (state == GameState::EnterName && inputDelay > 0.f) continue;

			if (event.key.code == sf::Keyboard::Escape) {
				if (state == GameState::EnterName) { cancelHighScore(); continue; }
				window.close();
			}

			if (state == GameState::EnterName && event.key.code == sf::Keyboard::BackSpace) {
				if (!inputName.empty()) { inputName.pop_back(); inputText.setString(inputName); } // Backspace.
				continue;
			}

			if (state == GameState::EnterName && (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Enter)) {
				submitHighScore(); // Zatwierdzenie imienia.
				continue;
			}

			if (state == GameState::Menu && event.key.code == sf::Keyboard::Space) {
				state = GameState::Playing; // Start gry ze spacji.
			}
			if ((state == GameState::Win || state == GameState::GameOver) && event.key.code == sf::Keyboard::R) {
				resetGame(); // Restart po zakoñczeniu.
			}
		}
		// Obs³uga klikniêæ mysz¹.
		else if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mp(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)); // Pozycja klikniêcia.

				if (state == GameState::Menu) {
					if (startButton.getGlobalBounds().contains(mp)) { state = GameState::Playing; continue; }
					if (leaderboardButton.getGlobalBounds().contains(mp)) { state = GameState::LeaderboardView; continue; }
				}

				if (state == GameState::LeaderboardView) {
					if (resetButton.getGlobalBounds().contains(mp)) { leaderboard.clear(); leaderboard.saveToFile("scores.txt"); continue; }
					if (backButton.getGlobalBounds().contains(mp)) { state = GameState::Menu; resetGame(); continue; }
				}

				if (state == GameState::EnterName) {
					if (submitButton.getGlobalBounds().contains(mp)) { submitHighScore(); continue; }
					if (cancelButton.getGlobalBounds().contains(mp)) { cancelHighScore(); continue; }
				}

				if (state == GameState::Menu) {
					if (titleText.getGlobalBounds().contains(mp)) { state = GameState::Playing; continue; }
				}
			}
		}
	}
}

// ## Aktualizacja logiki gry
void Game::update(float dt) {

	if (state == GameState::Menu) {
		titleTime += dt * titlePulseSpeed;
		float alpha = 200 + std::sin(titleTime) * 55;
		titleText.setFillColor(sf::Color(100, 200, 255, static_cast<sf::Uint8>(alpha)));

		instructionText.setPosition(window.getSize().x / 2.0f, 320.f + std::sin(titleTime * 1.5f) * 5);
		return;
	}


	player.update(dt, &map); // Aktualizacja gracza.

	if (map.collectCrystalAt(player.getBounds()))
		collectSound.play(); // Zebranie kryszta³u.

	for (auto& e : enemies)
		e->update(dt, &map, player.getPosition()); // Aktualizacja wrogów (AI).

	// Sprawdzenie kolizji gracza z wrogami.
	for (auto& enemy : enemies) {
		if (player.getBounds().intersects(enemy->getBounds())) {
			int finalScore = map.getScore();
			if (leaderboard.isHighScore(finalScore)) {
				pendingScore = finalScore;
				inputName.clear();
				inputText.setString(inputName);
				prevEndState = GameState::GameOver;
				state = GameState::EnterName; // High Score -> Wprowadzanie imienia.
				inputDelay = inputDelayDuration;
				loseSound.play();
				return;
			}
			state = GameState::GameOver; // Przegrana.
			statusText.setString("Game Over! You were caught! Press R to restart.");
			statusText.setFillColor(sf::Color::Red);
			loseSound.play();
			return;
		}
	}

	// Sprawdzenie warunku zwyciêstwa.
	bool allCollected = true;
	for (const auto& tile : map.getTiles()) {
		if (tile.getType() == TileType::Crystal && !tile.isCollected()) {
			allCollected = false;
			break;
		}
	}
	if (allCollected) {
		int finalScore = map.getScore();
		if (leaderboard.isHighScore(finalScore)) {
			pendingScore = finalScore;
			inputName.clear();
			inputText.setString(inputName);
			prevEndState = GameState::Win;
			state = GameState::EnterName; // High Score -> Wprowadzanie imienia.
			inputDelay = inputDelayDuration;
			winSound.play();
			return;
		}
		state = GameState::Win; // Wygrana.
		statusText.setString("You Win! All crystals collected! Press R to restart.");
		statusText.setFillColor(sf::Color::Green);
		winSound.play();
		return;
	}
	scoreText.setString("Score: " + std::to_string(map.getScore())); // Aktualizacja wyœwietlanego wyniku.

}

// ## Rysowanie elementów na ekranie
void Game::draw() {
	window.clear();
	window.draw(background); // Rysowanie t³a.

	// Obliczenie przesuniêcia (offset) mapy w celu wyœrodkowania.
	float mapPixelW = static_cast<float>(map.getWidth()) * map.getTileSize();
	float mapPixelH = static_cast<float>(map.getHeight()) * map.getTileSize();
	sf::Vector2f mapOffset((window.getSize().x - mapPixelW) * 0.5f, (window.getSize().y - mapPixelH) * 0.5f);

	auto mouseI = sf::Mouse::getPosition(window);
	sf::Vector2f mousePos(static_cast<float>(mouseI.x), static_cast<float>(mouseI.y)); // Pozycja myszy.

	if (state == GameState::Menu) {
		// Rysowanie tytu³u (z cieniem).
		{
			sf::Text shadow = titleText;
			shadow.setFillColor(sf::Color(0, 0, 0, 160));
			shadow.move(6.f, 6.f);
			window.draw(shadow);
		}
		window.draw(titleText);

		// Rysowanie instrukcji (z cieniem).
		{
			sf::Text shadowInstr = instructionText;
			shadowInstr.setFillColor(sf::Color(0, 0, 0, 140));
			shadowInstr.move(3.f, 3.f);
			window.draw(shadowInstr);
		}
		window.draw(instructionText);

		// --- PRZYCISKI MENU ---
		const sf::Color START_BASE_COLOR = sf::Color(40, 120, 180);
		const sf::Color LB_BASE_COLOR = sf::Color(50, 50, 70);

		// Rysowanie przycisku Start (z efektem hover).
		sf::Color startFill = START_BASE_COLOR;
		if (startButton.getGlobalBounds().contains(mousePos)) startFill = lightenColor(startFill, 30);
		startButton.setFillColor(startFill); // Ustawienie koloru wype³nienia.
		window.draw(startButton); // Rysowanie zaokr¹glonego prostok¹ta.

		// Rysowanie tekstu przycisku Start (z cieniem).
		{
			sf::Text shadowStart = startButtonText;
			shadowStart.setFillColor(sf::Color(0, 0, 0, 140));
			shadowStart.move(3.f, 3.f);
			window.draw(shadowStart);
		}
		window.draw(startButtonText);

		// Rysowanie przycisku Leaderboard (z efektem hover).
		sf::Color lbFill = LB_BASE_COLOR;
		if (leaderboardButton.getGlobalBounds().contains(mousePos)) lbFill = lightenColor(lbFill, 30);
		leaderboardButton.setFillColor(lbFill); // Ustawienie koloru wype³nienia.
		window.draw(leaderboardButton); // Rysowanie zaokr¹glonego prostok¹ta.

		// Rysowanie tekstu przycisku Leaderboard (z cieniem).
		{
			sf::Text shadowBtn = leaderboardButtonText;
			shadowBtn.setFillColor(sf::Color(0, 0, 0, 140));
			shadowBtn.move(3.f, 3.f);
			window.draw(shadowBtn);
		}
		window.draw(leaderboardButtonText);
	}
	else if (state == GameState::Playing) {
		map.draw(window, mapOffset); // Rysowanie mapy.
		player.draw(window, mapOffset); // Rysowanie gracza.
		for (auto& enemy : enemies)
			enemy->draw(window, mapOffset); // Rysowanie wrogów.
		window.draw(scoreText); // Rysowanie wyniku.
	}
	else if (state == GameState::LeaderboardView) {
		// Rysowanie czarnego modalu na œrodku.
		float winW = static_cast<float>(window.getSize().x);
		float winH = static_cast<float>(window.getSize().y);
		float modalW = std::min(1000.f, winW * 0.75f);
		float modalH = std::min(700.f, winH * 0.75f);
		sf::Vector2f modalPos((winW - modalW) * 0.5f, (winH - modalH) * 0.5f);

		// --- STA£E WYMIARY PRZYCISKÓW ---
		const float BACK_BTN_WIDTH = 160.f;
		const float BACK_BTN_HEIGHT = 40.f;
		const sf::Color BACK_BASE_COLOR = sf::Color(50, 50, 70);
		const sf::Color RESET_BASE_COLOR = sf::Color(120, 40, 40);

		// T³o modalu (prostok¹tne)
		sf::RectangleShape modalRect(sf::Vector2f(modalW, modalH));
		modalRect.setPosition(modalPos);
		modalRect.setFillColor(sf::Color(0, 0, 0, 230));
		modalRect.setOutlineColor(sf::Color::White);
		modalRect.setOutlineThickness(2.f);
		window.draw(modalRect);

		// Nag³ówek.
		sf::Text hdr("Leaderboard", font, 48);
		hdr.setFillColor(sf::Color::White);
		auto hdrBounds = hdr.getLocalBounds();
		hdr.setOrigin(hdrBounds.left + hdrBounds.width / 2.f, hdrBounds.top + hdrBounds.height / 2.f);
		hdr.setPosition(modalPos.x + modalW / 2.f, modalPos.y + 44.f);
		window.draw(hdr);

		// Rysowanie wpisów.
		auto entries = leaderboard.getEntries();
		float paddingX = 36.f;
		float startY = modalPos.y + 90.f;
		float leftX = modalPos.x + paddingX;
		float rightX = modalPos.x + modalW - paddingX;
		int rank = 1;
		for (const auto& e : entries) {
			sf::Text nameText(std::to_string(rank) + ". " + e.first, font, 28);
			nameText.setFillColor(sf::Color::White);
			nameText.setPosition(leftX, startY);
			window.draw(nameText);

			sf::Text scoreTextLocal(std::to_string(e.second), font, 28);
			auto sb = scoreTextLocal.getLocalBounds();
			scoreTextLocal.setOrigin(sb.left + sb.width, 0.f);
			scoreTextLocal.setPosition(rightX, startY);
			scoreTextLocal.setFillColor(sf::Color::White);
			window.draw(scoreTextLocal);

			startY += 36.f;
			rank++;
			if (startY > modalPos.y + modalH - 60.f) break;
		}

		// Rysowanie przycisku Reset (z efektem hover).
		resetButton.setPosition({ modalPos.x + 20.f, modalPos.y + 20.f });
		resetButtonText.setPosition(resetButton.getPosition().x + BACK_BTN_WIDTH / 2.f, resetButton.getPosition().y + BACK_BTN_HEIGHT / 2.f);

		sf::Color resetFill = RESET_BASE_COLOR;
		if (resetButton.getGlobalBounds().contains(mousePos)) resetFill = lightenColor(resetFill, 30);
		resetButton.setFillColor(resetFill);
		window.draw(resetButton);
		window.draw(resetButtonText);

		// Rysowanie przycisku Powrót (Back) (z efektem hover).
		backButton.setPosition({ modalPos.x + modalW - BACK_BTN_WIDTH - 20.f, modalPos.y + 20.f });
		backButtonText.setPosition(backButton.getPosition().x + BACK_BTN_WIDTH / 2.f, backButton.getPosition().y + BACK_BTN_HEIGHT / 2.f);

		sf::Color backFill = BACK_BASE_COLOR;
		if (backButton.getGlobalBounds().contains(mousePos)) backFill = lightenColor(backFill, 30);
		backButton.setFillColor(backFill);
		window.draw(backButton);
		window.draw(backButtonText);
	}
	else if (state == GameState::EnterName) {
		// Rysowanie czarnej nak³adki i proœby o imiê.
		sf::RectangleShape overlay(sf::Vector2f((float)window.getSize().x, (float)window.getSize().y));
		overlay.setFillColor(sf::Color(0, 0, 0, 220));
		overlay.setPosition(0.f, 0.f);
		window.draw(overlay);

		inputPromptText.setString("New High Score! Enter your name (max 20 chars):");
		auto pBounds = inputPromptText.getLocalBounds();
		inputPromptText.setOrigin(pBounds.left + pBounds.width / 2.f, pBounds.top + pBounds.height / 2.f);
		inputPromptText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 80.f);
		window.draw(inputPromptText);

		std::string display = inputName + ((static_cast<int>(titleTime * 2) % 2) ? "_" : ""); // Kursor migaj¹cy.
		inputText.setString(display);
		auto tBounds = inputText.getLocalBounds();
		inputText.setOrigin(tBounds.left + tBounds.width / 2.f, tBounds.top + tBounds.height / 2.f);
		inputText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f - 20.f);
		window.draw(inputText);

		// --- PRZYCISKI WPROWADZANIA IMIENIA ---
		const float SUBMIT_BTN_WIDTH = 160.f;
		const float SUBMIT_BTN_HEIGHT = 48.f;
		const sf::Color SUBMIT_BASE_COLOR = sf::Color(40, 160, 40);
		const sf::Color CANCEL_BASE_COLOR = sf::Color(160, 40, 40);

		// Pozycjonowanie przycisków ZatwierdŸ/Anuluj.
		float btnY = window.getSize().y / 2.f + 40.f;
		float totalW = SUBMIT_BTN_WIDTH + 20.f + SUBMIT_BTN_WIDTH;
		float startX = window.getSize().x / 2.f - totalW / 2.f;

		submitButton.setPosition({ startX, btnY });
		cancelButton.setPosition({ startX + SUBMIT_BTN_WIDTH + 20.f, btnY });

		// Rysowanie przycisku ZatwierdŸ (Submit).
		sf::Color sFill = SUBMIT_BASE_COLOR;
		if (submitButton.getGlobalBounds().contains(mousePos)) sFill = lightenColor(sFill, 30);
		submitButton.setFillColor(sFill);
		window.draw(submitButton);
		auto sb = submitButtonText.getLocalBounds();
		submitButtonText.setOrigin(sb.left + sb.width / 2.f, sb.top + sb.height / 2.f);
		submitButtonText.setPosition(submitButton.getPosition().x + SUBMIT_BTN_WIDTH / 2.f, submitButton.getPosition().y + SUBMIT_BTN_HEIGHT / 2.f);
		window.draw(submitButtonText);

		// Rysowanie przycisku Anuluj (Cancel).
		sf::Color cFill = CANCEL_BASE_COLOR;
		if (cancelButton.getGlobalBounds().contains(mousePos)) cFill = lightenColor(cFill, 30);
		cancelButton.setFillColor(cFill);
		window.draw(cancelButton);
		auto cb = cancelButtonText.getLocalBounds();
		cancelButtonText.setOrigin(cb.left + cb.width / 2.f, cb.top + cb.height / 2.f);
		cancelButtonText.setPosition(cancelButton.getPosition().x + SUBMIT_BTN_WIDTH / 2.f, cancelButton.getPosition().y + SUBMIT_BTN_HEIGHT / 2.f);
		window.draw(cancelButtonText);
	}
	else if (state == GameState::GameOver || state == GameState::Win) {
		// Rysowanie pe³nej czarnej nak³adki i tekstu statusu.
		sf::RectangleShape overlay(sf::Vector2f((float)window.getSize().x, (float)window.getSize().y));
		overlay.setFillColor(sf::Color::Black);
		overlay.setPosition(0.f, 0.f);
		window.draw(overlay);

		if (state == GameState::GameOver)
			statusText.setFillColor(sf::Color::Red);
		else
			statusText.setFillColor(sf::Color::Green);
		auto bounds = statusText.getLocalBounds();
		statusText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
		statusText.setPosition(static_cast<float>(window.getSize().x) / 2.f, static_cast<float>(window.getSize().y) / 2.f);
		window.draw(statusText);
	}

	window.display(); // Wyœwietlenie zawartoœci okna.
}

// ## Resetowanie stanu gry
void Game::resetGame()
{
	map.loadFromFile("assets/map2.txt"); // Ponowne ³adowanie mapy.

	player.setPosition({ 100, 100 }); // Reset pozycji gracza.
	player.scaleToTileSize(map.getTileSize());

	// Wyczyœæ i dodaj wrogów na nowo.
	enemies.clear();
	enemies.push_back(std::make_unique<Random>("assets/ghost1.png", sf::Vector2f(650, 200), 90.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost3.png", sf::Vector2f(750, 400), 90.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost4.png", sf::Vector2f(100, 600), 90.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost4.png", sf::Vector2f(400, 600), 90.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost3.png", sf::Vector2f(1200, 600), 90.f));
	enemies.push_back(std::make_unique<Random>("assets/ghost1.png", sf::Vector2f(1100, 700), 90.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost4.png", sf::Vector2f(100, 600), 90.f));
	for (auto& enemy : enemies) {
		enemy->scaleToTileSize(map.getTileSize());
		enemy->setPosition(enemy->getPosition());
	}

	scoreText.setString("Score: 0"); // Reset wyniku.
	pendingScore = 0;
	inputName.clear();
	state = GameState::Playing; // Przejœcie do stanu gry.
}

// ## Przejœcie do widoku tabeli wyników
void Game::openLeaderboard()
{
	state = GameState::LeaderboardView;
}

// ## Zapis wysokiego wyniku
void Game::submitHighScore()
{
	if (inputName.empty()) inputName = "Anonymous";
	leaderboard.addScore(inputName, pendingScore);
	leaderboard.saveToFile("scores.txt");
	state = GameState::LeaderboardView; // Po zapisie przejdŸ do tabeli wyników.
}

// ## Anulowanie wprowadzania wyniku
void Game::cancelHighScore()
{
	// Przywrócenie poprzedniego stanu koñcowego.
	if (prevEndState == GameState::GameOver) {
		statusText.setString("Game Over! You were caught! Press R to restart.");
		statusText.setFillColor(sf::Color::Red);
		state = GameState::GameOver;
	}
	else if (prevEndState == GameState::Win) {
		statusText.setString("You Win! All crystals collected! Press R to restart.");
		statusText.setFillColor(sf::Color::Green);
		state = GameState::Win;
	}
	else {
		state = GameState::Menu; // Domyœlnie powrót do Menu.
	}
}