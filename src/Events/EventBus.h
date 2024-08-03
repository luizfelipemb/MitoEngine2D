#pragma once

#include "../Logger/Logger.h"
#include "Event.h"
#include "KeyReleasedEvent.h"
#include "KeyPressedEvent.h"
#include "CollisionEnterEvent.h"
#include "CollisionExitEvent.h"
#include "CollisionStayEvent.h"

#include <map>
#include <typeindex>
#include <list>
#include <memory>
#include <functional>
#include <utility>

class IEventCallback
{
private:
    virtual void Call(Event& e) = 0;

public:
    virtual ~IEventCallback() = default;

    void Execute(Event& e)
    {
        Call(e);
    }
};

template <typename TEvent>
class EventCallback : public IEventCallback
{
private:
    std::function<void(TEvent&)> callbackFunction;

    virtual void Call(Event& e) override
    {
        callbackFunction(static_cast<TEvent&>(e));
    }

public:
    EventCallback(std::function<void(TEvent&)> callbackFunction)
        : callbackFunction(std::move(callbackFunction))
    {
    }

    virtual ~EventCallback() override = default;
};

typedef std::list<std::pair<void*, std::unique_ptr<IEventCallback>>> HandlerList;

class EventBus
{
private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus()
    {
        Logger::Log("EventBus constructor called!");
    }

    ~EventBus()
    {
        //Logger::Log("EventBus destructor called!");
    }

    // Clears the subscribers list
    void Reset()
    {
        subscribers.clear();
    }

    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&))
    {
        SubscribeToEvent<TEvent>(ownerInstance, [=](TEvent& event)
        {
            (ownerInstance->*callbackFunction)(event);
        });
    }

    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, std::function<void(TEvent&)> callbackFunction)
    {
        if (!subscribers[typeid(TEvent)].get())
        {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber = std::make_unique<EventCallback<TEvent>>(std::move(callbackFunction));
        subscribers[typeid(TEvent)]->emplace_back(ownerInstance, std::move(subscriber));
    }

    template <typename TOwner>
    void UnsubscribeFromOwner(TOwner* ownerInstance)
    {
        for (auto& [type, handlers] : subscribers)
        {
            handlers->remove_if(
            [ownerInstance](const std::pair<void*, std::unique_ptr<IEventCallback>>& handlerPair)
            {
                return handlerPair.first == ownerInstance;
            });
        }
    }

    template <typename TEvent, typename... TArgs>
    void EmitEvent(TArgs&&... args)
    {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers)
        {
            for (auto& handlerPair : *handlers)
            {
                auto handler = handlerPair.second.get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }
};
