#include "Bullet.h"


Bullet::Bullet(Entity* shooter) :bullet_x(shooter->getPositionX()), bullet_y(-1.2f), speed(3.0f){}

float Bullet::getPositionX(){
	return bullet_x;
}

float Bullet::getPositionY(){
	return bullet_y;
}

void Bullet::Draw(GLuint textureID, ShaderProgram* program){
	glBindTexture(GL_TEXTURE_2D, textureID);

	float bullet[] = { -0.05f, -0.2f, 0.05f, 0.2f, -0.05f, 0.2f, 0.05f, 0.2f, -0.05f, -0.2f, 0.05f, -0.2f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, bullet);
	glEnableVertexAttribArray(program->positionAttribute);

	float bulletTex[] = { 0.0, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0 };
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, bulletTex);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Bullet::UpdateBullet(float elapsed){
	bullet_y += speed * elapsed;
}