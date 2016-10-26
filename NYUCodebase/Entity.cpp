#include "Entity.h"

Entity::Entity(float x, float y) : position_x(x), position_y(y), direction(1.0f), alive(true){

}

float Entity::getPositionX(){
	return position_x;
}

float Entity::getPositionY(){
	return position_y;
}

bool Entity::getAlive(){
	return alive;
}

void Entity::Draw(GLuint textureID, ShaderProgram* program, Matrix modelMatrix){
	modelMatrix.identity();
	modelMatrix.Translate(position_x, position_y, 0.0f);
	program->setModelMatrix(modelMatrix);

	glBindTexture(GL_TEXTURE_2D, textureID);

	float plane[] = { -0.2f, -0.2f, 0.2f, 0.2f, -0.2f, 0.2f, 0.2f, 0.2f, -0.2f, -0.2f, 0.2f, -0.2f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, plane);
	glEnableVertexAttribArray(program->positionAttribute);

	float planeCoords[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, planeCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Entity::UpdateEnemy(Entity plane, float elapsed){
	//if (position_y > -1.0f){
	//	if (plane.position_x > -2.3f){
	//		position_y -= elapsed * 100.0f;
	//		direction = -1.0f;
	//	}
	//		position_y -= elapsed * 100.0f;
	//		direction = 1.0f;
	//	}
	//	position_x += direction * elapsed * 0.25f;
	//}
}

void Entity::UpdatePlayer(float elapsed){
	const Uint8 * keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT] == 1){
		direction = -1.0f;
		position_x += direction * elapsed * 3.1415926f / 2.0f;
	}
	if (keys[SDL_SCANCODE_RIGHT] == 1){
		position_x += elapsed * 3.1415926f / 2.0f;
	}
}

void Entity::dead(){
	alive = false;
	position_x = 10.0f;
}