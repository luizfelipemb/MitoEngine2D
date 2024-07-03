#pragma once
#include <memory>

#include "Event.h"

class GameObject;

class CollisionStayEvent : public Event
{
public:
	CollisionStayEvent(const std::unique_ptr<GameObject>& other)
		: Other(other)
	{
	}
	const std::unique_ptr<GameObject>& Other;
};

