#include "Light.h"

Light::Light(GameObject& gameObject, const std::string& name, Type type)
	: Component(gameObject, name)
	, m_Type(type)
{
}
