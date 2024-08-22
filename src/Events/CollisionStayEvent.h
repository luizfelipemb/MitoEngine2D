#pragma once
#include <memory>

#include "Event.h"

class GameObject;

class CollisionStayEvent : public Event
{
public:
	CollisionStayEvent(const std::shared_ptr<GameObject>& other)
		: Other(other)
	{
	}
	const std::shared_ptr<GameObject>& Other;
};

