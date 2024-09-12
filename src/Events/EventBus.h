#pragma once

#include "../Logger/Logger.h"
#include "Event.h"
#include "KeyReleasedEvent.h"
#include "KeyPressedEvent.h"
#include "CollisionEnterEvent.h"
#include "CollisionExitEvent.h"
#include "CollisionStayEvent.h"
#include "GameObjectCreatedEvent.h"
#include "MouseButtonPressedEvent.h"

#include <map>
#include <typeindex>
#include <utility>
#include <optional>

#include "../eventbus/event_bus.hpp"

class EventBus
{
private:
    dp::event_bus eventBus;
    
    // Map TOwner* to another map that maps event types to optional handler registrations
    std::map<void*, std::map<std::type_index, std::optional<dp::handler_registration>>> ownerRegistrations;

public:
    EventBus() = default;

    ~EventBus()
    {
        Reset();
    }

    void Reset()
    {
        // Unregister all handlers for all owners
        for (auto& [owner, registrationMap] : ownerRegistrations) {
            for (auto& [type, registration] : registrationMap) {
                if (registration) {
                    registration->unregister();
                }
            }
        }
        ownerRegistrations.clear();
    }

    template <typename TEvent, typename TOwner>
    void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&))
    {
        // Register the event handler
        auto registration = eventBus.register_handler<TEvent>(
            ownerInstance, callbackFunction
        );

        // Store the registration in the ownerRegistrations map
        ownerRegistrations[ownerInstance][typeid(TEvent)] = std::make_optional(std::move(registration));
    }

    template <typename TOwner>
    void UnsubscribeFromOwner(TOwner* ownerInstance)
    {
        if (ownerInstance == nullptr) return;

        auto ownerIt = ownerRegistrations.find(ownerInstance);
        if (ownerIt != ownerRegistrations.end())
        {
            // Unregister all events associated with this owner
            for (auto& [type, registration] : ownerIt->second) {
                if (registration) {
                    registration->unregister();
                }
            }

            // Erase the owner from the map
            ownerRegistrations.erase(ownerIt);
        }
    }

    template <typename TEvent, typename TOwner>
    void UnsubscribeFromEvent(TOwner* ownerInstance)
    {
        if (ownerInstance == nullptr) return;

        auto ownerIt = ownerRegistrations.find(ownerInstance);
        if (ownerIt != ownerRegistrations.end())
        {
            auto& registrationMap = ownerIt->second;
            auto eventIt = registrationMap.find(typeid(TEvent));

            if (eventIt != registrationMap.end())
            {
                if (eventIt->second) {
                    eventIt->second->unregister();  // Unregister the specific event handler
                }
                registrationMap.erase(eventIt);  // Erase the event type from the owner's map

                // If no more events are registered for this owner, remove the owner from the map
                if (registrationMap.empty()) {
                    ownerRegistrations.erase(ownerIt);
                }
            }
        }
    }

    template <typename TEvent, typename... TArgs>
    void EmitEvent(TArgs&&... args)
    {
        TEvent event(std::forward<TArgs>(args)...);
        eventBus.fire_event(event);
    }
};