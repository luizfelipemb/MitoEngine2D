#pragma once
#include "EventBus.h"

class GlobalEventBus
{
public:
    GlobalEventBus() = delete;

    static void Reset() {
        m_eventBus.Reset();
    }

    template <typename TEvent, typename TOwner>
    static void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::* callbackFunction)(TEvent&)) {
        m_eventBus.SubscribeToEvent<TEvent>(ownerInstance, callbackFunction);
    }

    template <typename TEvent>
    static void SubscribeToEvent(std::function<void(TEvent&)> callbackFunction) {
        m_eventBus.SubscribeToEvent<TEvent>(callbackFunction);
    }

    template <typename TEvent, typename ...TArgs>
    static void EmitEvent(TArgs&& ...args) {
        m_eventBus.EmitEvent<TEvent>(std::forward<TArgs>(args)...);
    }
    template <typename TOwner>
    static void UnsubscribeFromOwner(TOwner* ownerInstance) {
        m_eventBus.UnsubscribeFromOwner(ownerInstance);
    }

private:
    static EventBus m_eventBus;
};