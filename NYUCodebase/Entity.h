#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"

class Entity{
public:
	Entity(float x, float y);
	float getPositionX();
	float getPositionY(); 
	bool getAlive();
	void Draw(GLuint textureID, ShaderProgram* program, Matrix modelMatrix);
	void UpdateEnemy(Entity plane, float elapsed);
	void UpdatePlayer(float elapsed);
	void dead();
private:
	float position_x;
	float position_y;
	float direction;
	bool alive;
};