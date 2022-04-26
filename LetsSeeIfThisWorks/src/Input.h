#pragma once
#include <map>
#include <vector>
#include <functional>

class Input
{
public:

	using Callback = std::function<void()>;

	void AddBinding(int key, const Callback& callback);
	void OnKeyPress(int key);

private:

	std::map<int, std::vector<Callback>> m_Callbacks;

};