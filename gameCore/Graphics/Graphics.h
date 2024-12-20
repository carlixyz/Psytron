#pragma once

#include "../Utility/Singleton.h"
#include "ApplicationProperties.h"
#include "raylib-cpp.hpp"

class Graphics : public Singleton<Graphics>
{

	bool CloseApplication		= false;

	raylib::Window* Window		= nullptr;

	Vector2 NativeArea			{ 640, 480 };
	Vector2 ScaledFactor		{ 1.f, 1.f };

	Rectangle WindowArea		{ 0, 0, NativeArea.x, NativeArea.y};

public:
	friend class Singleton<Graphics>;

	bool Init(ApplicationProperties* appProperties);
	bool Deinit();

	void SwitchFullScreen();

	//void DrawPoint(const cVec3& pos, const cVec3& color);
	//void DrawLine(const cVec3& pos1, const cVec3& pos2, const cVec3& color);
	//void DrawGrid();
	//void DrawAxis();

	//void ActivateCamera(Camera* cam);
	//void SetWorldMatrix(const cMatrix& matrix);
	//void RefreshWorldView();

	const Rectangle& GetWindowArea()	{ return WindowArea; }

	const Vector2 GetFactorArea()		{ return ScaledFactor; }

	bool GetCloseApplication();

	inline int GetHorizontalCenter()	{ return (int)(GetWindowArea().width * 0.5f); }
	inline int GetVerticalCenter()		{ return (int)(GetWindowArea().height * 0.5f); }
	inline Vector2 GetScreenCenter()	{ return Vector2(GetWindowArea().width * 0.5f, GetWindowArea().height * 0.5f); }

protected:
	Graphics() { ; }					// Protected Constructor

};

