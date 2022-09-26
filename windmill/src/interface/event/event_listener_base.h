#pragma once

#include "../event/event_system.h"

namespace wm {

    class event_listener_base {
    protected:
        ptr<event_system> get_event_system() const;
    };

}