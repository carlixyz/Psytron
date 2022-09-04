#pragma once

class GameState
{
public:
	virtual void OnInit() = 0;
	virtual void OnDeinit() = 0;

	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;

	virtual void OnPause() { ; }
	virtual void OnResume() { ; }

};

