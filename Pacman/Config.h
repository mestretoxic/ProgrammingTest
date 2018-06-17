#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <SDL_stdinc.h>
#include <cassert>

#define MSG_MALFORMED_CONFIG "Malformed Config file!"

#define GET_INT_VAR(name) {								\
	bool exists = vars.count(#name) != 0;				\
	assert(exists && MSG_MALFORMED_CONFIG);				\
	std::string value = vars.at(#name);					\
	name = std::strtol(value.c_str(), nullptr, 10);		\
}														\

#define GET_FLOAT_VAR(name) {							\
	bool exists = vars.count(#name) != 0;				\
	assert(exists && MSG_MALFORMED_CONFIG);				\
	std::string value = vars.at(#name);					\
	name = std::strtof(value.c_str(), nullptr);			\
}														\

#define GET_STRING_VAR(name) {							\
	bool exists = vars.count(#name) != 0;				\
	assert(exists && MSG_MALFORMED_CONFIG);				\
	name = vars.at(#name);								\
}														\

typedef std::map<std::string, std::string> NameToValueMap;

class Config
{
public:
	static int worldOffsetX;
	static int worldOffsetY;
	static int tileSize;
	static int avatarStartX;
	static int avatarStartY;
	static int ghostStartX;
	static int ghostStartY;
	static int lives;
	static float avatarVelocity;
	static float powerupVelocity;
	static float ghostVelocity;
	static float ghostDeadVelocity;
	static std::string fontMain;
	static std::string fontHud;

	static int fontMainSize;
	static int fontHudSize;

	static bool ReadConfig(const char* fileName)
	{
		NameToValueMap vars;

		std::string line;
		std::ifstream file(fileName);
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::getline(file, line);
				if (line.empty() || line.at(0) == '#')
					continue;

				const auto splitAt = line.find(" = ");
				assert(splitAt != std::string::npos && MSG_MALFORMED_CONFIG);
				const auto varName = line.substr(0, splitAt);
				const auto strValue = line.substr(splitAt + 3);
				assert((!varName.empty() && !strValue.empty()) && MSG_MALFORMED_CONFIG);
				vars[varName] = strValue;
			}
			file.close();
		}

		GET_INT_VAR(worldOffsetX);
		GET_INT_VAR(worldOffsetY);
		GET_INT_VAR(tileSize);
		GET_INT_VAR(avatarStartX);
		GET_INT_VAR(avatarStartY);
		GET_INT_VAR(ghostStartX);
		GET_INT_VAR(ghostStartY);
		GET_INT_VAR(lives);
		GET_FLOAT_VAR(avatarVelocity);
		GET_FLOAT_VAR(powerupVelocity);
		GET_FLOAT_VAR(ghostVelocity);
		GET_FLOAT_VAR(ghostDeadVelocity);

		GET_STRING_VAR(fontMain);
		GET_INT_VAR(fontMainSize);
		GET_STRING_VAR(fontHud);
		GET_INT_VAR(fontHudSize);

		return true;
	}
};

#endif