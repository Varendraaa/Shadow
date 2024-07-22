#include <iostream>
using namespace std;

#include "Game.h"
#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

int main()
{
	Game game;

	if (game.initialise())
	{
		game.Run();
	}

	game.Cleanup();


	return 0;
}