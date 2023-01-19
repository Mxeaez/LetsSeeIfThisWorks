#pragma once
#include "Component.h"

class Light : public Component
{
public:
	enum Type
	{
		PointLight,
		SpotLight,
		DirectionalLight
	};

public:

	Light(GameObject& gameObject, const std::string& name = "Light", Type type = PointLight);
	virtual ~Light() {}


private:

	Type m_Type;


};