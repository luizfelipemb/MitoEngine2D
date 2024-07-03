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

    template <typename TEvent, typename ...TArgs>
    static void EmitEvent(TArgs&& ...args) {
        m_eventBus.EmitEvent<TEvent>(std::forward<TArgs>(args)...);
    }

private:
    static EventBus m_eventBus;

};