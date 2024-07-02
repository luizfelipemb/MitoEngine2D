#pragma once

#include <memory>

#include "Event.h"

class GameObject;

class CollisionEvent : public Event
{
public:
	CollisionEvent(const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) :
		A(a), B(b){}
	const std::unique_ptr<GameObject>& A;
	const std::unique_ptr<GameObject>& B;

};
