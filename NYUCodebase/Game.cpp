#include "Game.h"
#include "iostream"

GLuint LoadTexture(const char* image_path){
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void Game::DrawText(int fontTexture, std::string text, float size, float spacing){
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	for (int i = 0; i < text.size(); i++){
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}

	glUseProgram(program->programID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

Game::Game() :done(false), elapsed(0.0f), lastFrameTicks(0.0f), player(0.0f, -1.5f), state(STATE_MAIN_MENU) {
	Setup();
}

Game::~Game(){
	
	Mix_FreeMusic(music);

	delete program;
	SDL_Quit();
}

void Game::Setup(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	program->setModelMatrix(modelMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);

	glUseProgram(program->programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("08-saturn.mp3");

	shoot = Mix_LoadWAV("shoot.wav");

	fontID = LoadTexture("font1.png");
	playerID = LoadTexture("player.png");
	bulletTexture = LoadTexture("laserGreen.png");
	enemyID = LoadTexture("enemyShip.png");

	createInvaders();
}

void Game::createInvaders(){
	for (float x = -2.8f; x < 3; x = x + 0.5f){
		Entity enemy(x, 1.4f);
		//vector push_back 
		row1_Invaders.push_back(enemy);
	}

	for (float x = -2.8f; x < 3; x = x + 0.5f){
		Entity enemy(x, 0.9f);
		//vector push_back 
		row2_Invaders.push_back(enemy);
	}

	for (float x = -2.8f; x < 3; x = x + 0.5f){
		Entity enemy(x, 0.4f);
		//vector push_back 
		row3_Invaders.push_back(enemy);
	}
}

void Game::collision(){
	for (int index = 0; index < row1_Invaders.size(); index++){
		if (row1_Invaders[index].getAlive() == true){
			for (std::vector<Bullet*>::iterator indexBullet = bullets.begin(); indexBullet != bullets.end();){
				if ((*indexBullet)->getPositionX() < (row1_Invaders[index].getPositionX() + 0.15f) && (*indexBullet)->getPositionX() > (row1_Invaders[index].getPositionX() - 0.15f)){
					if ((*indexBullet)->getPositionY() < (row1_Invaders[index].getPositionY() + 0.15f) && (*indexBullet)->getPositionY() > (row1_Invaders[index].getPositionY() - 0.15f)){
						row1_Invaders[index].dead();
						delete *indexBullet;
						indexBullet = bullets.erase(indexBullet);
					}
					else{indexBullet++;}
				}
				else{indexBullet++;}
			}
		}
	}

	for (int index = 0; index < row2_Invaders.size(); index++){
		if (row2_Invaders[index].getAlive() == true){
			for (std::vector<Bullet*>::iterator indexBullet = bullets.begin(); indexBullet != bullets.end();){
				if ((*indexBullet)->getPositionX() < (row2_Invaders[index].getPositionX() + 0.15f) && (*indexBullet)->getPositionX() > (row2_Invaders[index].getPositionX() - 0.15f)){
					if ((*indexBullet)->getPositionY() < (row2_Invaders[index].getPositionY() + 0.15f) && (*indexBullet)->getPositionY() > (row2_Invaders[index].getPositionY() - 0.15f)){
						row2_Invaders[index].dead();
						delete *indexBullet;
						indexBullet = bullets.erase(indexBullet);
					}
					else{indexBullet++;}
				}
				else{indexBullet++;}
			}
		}
	}

	for (int index = 0; index < row3_Invaders.size(); index++){
		if (row3_Invaders[index].getAlive() == true){
			for (std::vector<Bullet*>::iterator indexBullet = bullets.begin(); indexBullet != bullets.end();){
				if ((*indexBullet)->getPositionX() < (row3_Invaders[index].getPositionX() + 0.15f) && (*indexBullet)->getPositionX() > (row3_Invaders[index].getPositionX() - 0.15f)){
					if ((*indexBullet)->getPositionY() < (row3_Invaders[index].getPositionY() + 0.15f) && (*indexBullet)->getPositionY() > (row3_Invaders[index].getPositionY() - 0.15f)){
						row3_Invaders[index].dead();
						delete *indexBullet;
						indexBullet = bullets.erase(indexBullet);
					}
					else{indexBullet++;}
				}
				else{indexBullet++;}
			}
		}
	}
}

bool Game::checkWin(){
	for (int index = 0; index < row1_Invaders.size(); index++){
		if (row1_Invaders[index].getAlive() == true){
			return false;
		}
	}
	for (int index = 0; index < row2_Invaders.size(); index++){
		if (row2_Invaders[index].getAlive() == true){
			return false;
		}
	}
	for (int index = 0; index < row3_Invaders.size(); index++){
		if (row3_Invaders[index].getAlive() == true){
			return false;
		}
	}
	return true;
}

void Game::ProcessInput(){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN){
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				if (state == STATE_MAIN_MENU){
					state = STATE_GAME;
				}
				else if (state == STATE_GAME_OVER){
					state = STATE_MAIN_MENU;
				}
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE){
				Mix_PlayChannel(-1, shoot, 0);
				bullets.push_back(new Bullet(&player));
			}
		}
	}
}

void Game::Update(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	if (state == STATE_GAME){
//--------------------------------Updating Player------------------------------------
	
		player.UpdatePlayer(elapsed);

//--------------------------------Updating Bullets-----------------------------------

		for (std::vector<Bullet*>::iterator index = bullets.begin(); index != bullets.end();){
			(*index)->UpdateBullet(elapsed);
			if ((*index)->getPositionY() > 2.0f){
				delete *index;
				index = bullets.erase(index);
			}
			else{
				index++;
			}
		}
	//--------------------------------Updating Invaders----------------------------------
	
		for (int index = 0; index < row1_Invaders.size(); index++){
			row1_Invaders[index].UpdateEnemy(row1_Invaders[0], elapsed);
			row2_Invaders[index].UpdateEnemy(row1_Invaders[0], elapsed);
			row3_Invaders[index].UpdateEnemy(row1_Invaders[0], elapsed);
		}

		collision();
		if (checkWin() == true){
			state = STATE_GAME_OVER;
		}
		//Mix_PlayMusic(music, -1);
	}
}

void Game::Render(){
	switch (state){
	case STATE_MAIN_MENU:
		RenderMainMenu();
		break;
	case STATE_GAME:
		RenderGame();
		break;
	case STATE_GAME_OVER:
		RenderGameOver();
		break;
	}
}

void Game::RenderMainMenu(){
	//black background or picture background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//create font for the word Space Invaders
	modelMatrix.identity();
	modelMatrix.Translate(-2.6f, 0.5f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Space Invaders", 0.8f, -0.4f);

	//create font for player to know how to start
	modelMatrix.identity();
	modelMatrix.Translate(-1.8f, -0.4f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press Enter To Start", 0.3f, -0.1f);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);
}

void Game::RenderGame(){

	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Rendering Enemies
	for (int index = 0; index < row1_Invaders.size(); index++){
		//Draw enemy
		row1_Invaders[index].Draw(enemyID, program, modelMatrix);
	}

	for (int index = 0; index < row2_Invaders.size(); index++){
		//Draw enemy
		row2_Invaders[index].Draw(enemyID, program, modelMatrix);
	}

	for (int index = 0; index < row3_Invaders.size(); index++){
		//Draw enemy
		row3_Invaders[index].Draw(enemyID, program, modelMatrix);
	}

	//Rendering Player
	player.Draw(playerID, program, modelMatrix);

	//Rendering Bullets
	for (int index = 0; index < bullets.size(); index++){
		modelMatrix.identity();
		modelMatrix.Translate(bullets[index]->getPositionX(), bullets[index]->getPositionY(), 0.0f);
		program->setModelMatrix(modelMatrix);
		bullets[index]->Draw(bulletTexture, program);
	}

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);
	
}

void Game::RenderGameOver(){
	glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//create font for the word Space Invaders
	modelMatrix.identity();
	modelMatrix.Translate(-2.7f, 0.5f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Game Over", 1.0f, -0.3f);

	//create font for player to know how to start
	modelMatrix.identity();
	modelMatrix.Translate(-1.5f, -0.6f, 0.0f);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press Enter", 0.4f, -0.1f);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);
}

bool Game::UpdateAndRender(){
	ProcessInput();
	Update();
	Render();
	return done;
}