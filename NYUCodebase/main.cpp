#include "Game.h"


int main(int argc, char *argv[])
{
	Game play;
	while (!play.UpdateAndRender()) {
	}
	return 0;
}