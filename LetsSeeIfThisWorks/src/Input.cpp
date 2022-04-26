#include "Input.h"

void Input::AddBinding(int key, const Callback& callback)
{
	m_Callbacks[key].push_back(callback);
}

void Input::OnKeyPress(int key)
{
	for (Callback& callback : m_Callbacks[key])
	{
		callback();
	}
}
