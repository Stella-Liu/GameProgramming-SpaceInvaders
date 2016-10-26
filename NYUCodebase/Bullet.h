#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Entity.h"

class Bullet{
public:
	Bullet(Entity* shooter);
	float getPositionX();
	float getPositionY();
	void Draw(GLuint textureID, ShaderProgram* program);
	void UpdateBullet(float elapsed);
private:
	Entity* shooter;
	const float bullet_x;
	float timeLeftOver;
	float bullet_y;
	float speed;
};
