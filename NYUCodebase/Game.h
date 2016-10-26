#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SDL_mixer.h"

#include <vector>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Bullet.h"


#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


class Game{
public:
	Game();
	~Game();
	
	void createInvaders();
	void collision();
	bool checkWin();
	void RenderMainMenu();
	void RenderGame();
	void RenderGameOver();

	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	bool UpdateAndRender();
	enum GameState{STATE_MAIN_MENU, STATE_GAME, STATE_GAME_OVER};
	void DrawText(int fontTexture, std::string text, float size, float spacing);
private:
	Entity player;
	std::vector<Bullet*>bullets;
	std::vector<Entity> row1_Invaders;
	std::vector<Entity> row2_Invaders;
	std::vector<Entity> row3_Invaders;

	SDL_Window* displayWindow;
	ShaderProgram* program;
	Mix_Music *music;
	Mix_Chunk *shoot;

	Matrix modelMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;
	
	GLuint fontID;
	GLuint playerID;
	GLuint bulletTexture;
	GLuint enemyID;

	int state;
	float lastFrameTicks;
	float elapsed;
	bool done;
};