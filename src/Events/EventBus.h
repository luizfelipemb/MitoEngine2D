#pragma once

#include "../Logger/Logger.h"
#include "Event.h"
#include "KeyReleasedEvent.h"
#include "CollisionEnterEvent.h"
#include "CollisionExitEvent.h"
#include "CollisionStayEvent.h"

#include <map>
#include <typeindex>
#include <list>
#include <memory>
#include <functional>

class IEventCallback {
private:
    virtual void Call(Event& e) = 0;

public:
    virtual ~IEventCallback() = default;

    void Execute(Event& e) {
        Call(e);
    }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
private:
    typedef void (TOwner::* CallbackFunction)(TEvent&);

    TOwner* ownerInstance;
    CallbackFunction callbackFunction;

    virtual void Call(Event& e) override {
        std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
    }

public:
    EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
        this->ownerInstance = ownerInstance;
        this->callbackFunction = callbackFunction;
    }

    virtual ~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
private:
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;

public:
    EventBus() {
        Logger::Log("EventBus constructor called!");
    }

    ~EventBus() {
        Logger::Log("EventBus destructor called!");
    }

    // Clears the subscribers list
    void Reset() {
        subscribers.clear();
    }

    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::* callbackFunction)(TEvent&)) {
        if (!subscribers[typeid(TEvent)].get()) {
            subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
        subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    template <typename TEvent, typename ...TArgs>
    void EmitEvent(TArgs&& ...args) {
        auto handlers = subscribers[typeid(TEvent)].get();
        if (handlers) {
            for (auto it = handlers->begin(); it != handlers->end(); it++) {
                auto handler = it->get();
                TEvent event(std::forward<TArgs>(args)...);
                handler->Execute(event);
            }
        }
    }
};

