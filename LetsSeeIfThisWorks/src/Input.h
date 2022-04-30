#pragma once
#include <utility>

namespace Input
{

	bool IsKeyPressed(int key);
	std::pair<float, float> GetMousePos();
	float GetMouseX();
	float GetMouseY();

};