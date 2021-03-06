#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include "World.h"
#include "Config.h"
#include "Defines.h"

Pacman* Pacman::Create(Drawer* drawer)
{
	auto* pacman = new Pacman(drawer);

	if (!pacman->Init())
	{
		delete pacman;
		pacman = nullptr;
	}

	return pacman;
}

Pacman::Pacman(Drawer* drawer)
: m_drawer(drawer)
, m_lives(0)
, m_score(0)
, m_fps(0)
{
}

Pacman::~Pacman() = default;

bool Pacman::Init()
{
	Config::ReadConfig("config.txt");
	m_lives = Config::lives;

	m_world.reset(new World());
	m_world->Init();

	return true;
}

bool Pacman::Update(const float dt, SDL_Event& event)
{
	ASSERT_BOOL(m_world, "World is null!");

	if (!UpdateInput(event))
		return false;

	if (CheckEndGameCondition())
		return true;

	m_world->Update(dt, this);

	if (dt > 0)
		m_fps = static_cast<int>(1 / dt);

	return true;
}

bool Pacman::UpdateInput(SDL_Event& event) const
{
	if (event.type == SDL_KEYDOWN) {
		switch(event.key.keysym.sym)
		{
			case SDLK_ESCAPE:
				return false;

			case SDLK_UP:
				m_world->SetAvatarMovement(UP);
				break;
			case SDLK_DOWN:
				m_world->SetAvatarMovement(DOWN);
				break;
			case SDLK_RIGHT:
				m_world->SetAvatarMovement(RIGHT);
				break;
			case SDLK_LEFT:
				m_world->SetAvatarMovement(LEFT);
				break;
		default: ;
		}
	}

	return true;
}

bool Pacman::CheckEndGameCondition() const
{
	ASSERT_BOOL(m_world, "World is null!");
	return !m_world->HasDots() || m_lives <= 0;
}

bool Pacman::Draw() const
{
	ASSERT_BOOL(m_world, "World is null!");
	ASSERT_BOOL(m_drawer, "Drawer is null!");

	m_world->Draw(m_drawer);

	std::string scoreString = "Score: " + std::to_string(m_score);

	TextParams textParams;
	textParams.fontPath = Config::fontHud;
	textParams.size = Config::fontHudSize;
	textParams.text = scoreString.c_str();
	textParams.x = 20;
	textParams.y = 50;

	m_drawer->DrawText(textParams);

	std::string livesString = "Lives: " + std::to_string(m_lives);

	textParams.text = livesString.c_str();
	textParams.x = 20;
	textParams.y = 80;

	m_drawer->DrawText(textParams);

	std::string fpsString = "FPS: " + std::to_string(m_fps);

	textParams.text = fpsString.c_str();
	textParams.x = 880;
	textParams.y = 50;

	m_drawer->DrawText(textParams);

	textParams.fontPath = Config::fontMain;
	textParams.size = Config::fontMainSize;
	textParams.aligned = true;

	if (!m_world->HasDots())
	{
		textParams.text = "You win!";
		m_drawer->DrawText(textParams);
	}
	else if (m_lives <= 0)
	{
		textParams.text = "You lose!";
		m_drawer->DrawText(textParams);	
	}

	return true;
}

void Pacman::UpdateScore(const int value)
{
	m_score += value;
}

void Pacman::UpdateLives()
{
	m_lives--;
}