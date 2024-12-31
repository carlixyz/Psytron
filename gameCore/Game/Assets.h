#pragma once

#include "../Utility/Singleton.h"
#include <iostream>
#include <raylib.h>
#include <map>



class Assets : public Singleton<Assets>
{
	friend class Singleton<Assets>;

	std::map<std::string, Texture2D> Sprites;
	std::map<std::string, Font> Fonts;
	std::map<std::string, Sound> Sounds;
	std::map<std::string, Music> Musics;
	std::map<std::string, Image> Images;

public:
	bool Init();
	bool Deinit();

	Texture2D GetSprite(const std::string& Name)
	{
		return Sprites[Name];
	}

	Image GetImage(const std::string& Name)
	{
		return Images[Name];
	}

	Font GetFont(const std::string& Name)
	{
		return Fonts[Name];
	}

	Sound GetSound(const std::string& Name)
	{
		return Sounds[Name];
	}

	Music GetMusic(const std::string& Name)
	{
		return Musics[Name];
	}

private:
	void LoadSprites();
	void UnloadSprites();

	void LoadImages();

	void LoadFonts();
	void UnloadFonts();

	void LoadSounds();
	void UnloadSounds();

	void LoadMusic();
	void UnloadMusic();
};

#define GetAsset( Name ) Assets::Get().GetSprite( Name )
#define GetFont( Name ) Assets::Get().GetFont( Name )
#define GetMusic( Name ) Assets::Get().GetMusic( Name )
#define GetSound( Name ) Assets::Get().GetSound( Name )
//#define Stringify(name) #name
//
//#define ASSETS Assets::Get()
//#define GetAsset(Name) ASSETS.GetSprite(#Name)