#include "event_listener_base.h"

#include "../core/engine.h"

namespace wm {

    ptr<event_system> event_listener_base::get_event_system() const {
        return engine::get_event_system();
    } 

}