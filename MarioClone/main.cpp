#include <iostream>
#include "GameEngine.h"
int main()
{
	GameEngine game("Assets/ConfigFile.txt");
	game.run();
}