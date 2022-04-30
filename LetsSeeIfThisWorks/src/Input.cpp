#include "Input.h"
#include "Application.h"

namespace Input
{

	bool Input::IsKeyPressed(int key)
	{

		GLFWwindow* window = Application::Get().GetWindow().GetWindow();
		int state = glfwGetKey(window, key);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePos()
	{
		GLFWwindow* window = Application::Get().GetWindow().GetWindow();
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return { (float)x, (float)y };
	}

	float GetMouseX()
	{
		auto [x, y] = GetMousePos();

		return x;
	}

	float GetMouseY()
	{
		auto [x, y] = GetMousePos();

		return y;
	}

}
