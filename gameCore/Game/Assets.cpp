#include "Assets.h"


bool Assets::Init()
{
	LoadSprites();
	LoadImages();
	LoadFonts();
	LoadSounds();
	LoadMusic();

	return true;
}

bool Assets::Deinit()
{
	UnloadSprites();
	UnloadFonts();
	UnloadSounds();
	UnloadMusic();

	return true;
}

void Assets::LoadSprites()
{
	const auto Load = [&](const std::string Name, const char* FileName)
		{
			const Texture2D Sprite = LoadTexture(FileName);
			Sprites[Name] = Sprite;
		};

	Load("Sprites", "Data/Sprites/VisualSprites.png");
	Load("Prop", "Data/Sprites/Car5.png");

	Load("BlockA", "Data/Sprites/BarricadeA.png");	// ok
	Load("BlockB", "Data/Sprites/BarricadeB.png");	// okish
	Load("BlockC", "Data/Sprites/BarricadeC.png");	// ok
	Load("BlockD", "Data/Sprites/BarricadeD.png");	// Meh
	Load("BlockE", "Data/Sprites/BarricadeE.png");	// Nice!

	Load("Truck", "Data/Sprites/Truck.png");// Ok
	Load("Car1", "Data/Sprites/Car1.png");	// Ok
	Load("Car2", "Data/Sprites/Car2.png");	// Ok
	Load("Car3", "Data/Sprites/Car3.png");	// No, too dark
	Load("Car4", "Data/Sprites/Car4.png");	// Ok
	Load("Car5", "Data/Sprites/Car5.png");	// Ok
	//Load("Car6", "Data/Sprites/Car6.png");	// NO!
	//Load("Car7", "Data/Sprites/Car7.png");	// NO!
	//BulletOwner->Sprite = new raylib::Texture2D("Data/Sprites/VisualSprites.png");
}

void Assets::UnloadSprites()
{
	for (auto sprite : Sprites)
	{
		UnloadTexture(GetSprite(sprite.first));
	}
	//UnloadTexture(GetSprite("Player"));
}

void Assets::LoadImages()
{
	const auto Load = [&](const std::string Name, const char* FileName)
	{
		const Image Image = LoadImage(FileName);
		Images[Name] = Image;
	};

}

void Assets::LoadFonts()
{
	const auto Load = [&](const std::string Name, const char* FileName)
		{
			const Font Font = LoadFont(FileName);
			Fonts[Name] = Font;
		};

	Load("Noto", "Data/NotoJp.fnt");
	Load("PC98", "Data/pc-9800.ttf");
	//Load("Start", "Data/PressStart2P.ttf");
	//LoadFont("Data/NotoJp.fnt");
}

void Assets::UnloadFonts()
{
	for (auto font : Fonts)
	{
		UnloadFont(GetFont(font.first));
	}
}

void Assets::LoadSounds()
{
	const auto Load = [&](const std::string Name, const char* FileName)
		{
			const Sound Sound = LoadSound(FileName);
			Sounds[Name] = Sound;
		};

	Load("Fire", "Data/Sound/MiniGun_A.wav");
	Load("Explode", "Data/Sound/Explode.wav");
	//Load("Fire", "Sounds/BulletFire.wav");
}

void Assets::UnloadSounds()
{
	//UnloadSound(GetSound("Fire"));
	for (auto sound : Sounds)
	{
		UnloadSound(GetSound(sound.first));
	}
}

void Assets::LoadMusic()
{
	const auto Load = [&](const std::string Name, const char* FileName, bool loop = true)
		{
			const Music Music = LoadMusicStream(FileName);
			Musics[Name] = Music;
		};
	
	Load("FireLoop", "Data/Sound/stg_st003_88pro-loop.ogg", false);
	Load("FireIntro", "Data/Sound/stg_st003_88pro-intro.ogg");
}

void Assets::UnloadMusic()
{
	//UnloadMusicStream(GetMusic("Menu"));
	for (auto music : Musics)
	{
		UnloadMusicStream(GetMusic(music.first));
	}
}

