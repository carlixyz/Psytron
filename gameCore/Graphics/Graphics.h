#pragma once

#include "../Utility/Singleton.h"
#include "ApplicationProperties.h"
#include "raylib-cpp.hpp"

class Graphics : public Singleton<Graphics>
{

	bool CloseApplication		= false;

	raylib::Window* window		= nullptr;
	Rectangle windowArea		{ 0, 0, 640, 480};

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

	const Rectangle& GetWindowArea() { return windowArea; }

	bool GetCloseApplication();

protected:
	Graphics() { ; }					// Protected Constructor

};

