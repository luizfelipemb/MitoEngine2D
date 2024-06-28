#include "EventBus.h"

std::map<std::type_index, std::unique_ptr<HandlerList>> EventBus::subscribers;
