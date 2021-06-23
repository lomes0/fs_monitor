#ifndef __EVENT__
#define __EVENT__


#include <chrono>
#include <sys/inotify.h>

#include <filesystem>
#include <list>

namespace fs = std::filesystem;

namespace fsm {

enum class Event : std::uint32_t {

    access = IN_ACCESS,
    attrib = IN_ATTRIB,
    close_write = IN_CLOSE_WRITE,
    close_nowrite = IN_CLOSE_NOWRITE,
    close = IN_CLOSE,
    create = IN_CREATE,
    remove = IN_DELETE,
    remove_self = IN_DELETE_SELF,
    modify = IN_MODIFY,
    move_self = IN_MOVE_SELF,
    moved_from = IN_MOVED_FROM,
    moved_to = IN_MOVED_TO,
    move = IN_MOVE,
    open = IN_OPEN,
    is_dir = IN_ISDIR,
    unmount = IN_UNMOUNT,
    q_overflow = IN_Q_OVERFLOW,
    ignored = IN_IGNORED,
    oneshot = IN_ONESHOT,
    all = IN_ALL_EVENTS
};

struct Notification {

        typedef std::chrono::steady_clock::time_point time_point;

        Notification(const Event& event,
                        const time_point& time_point)
                : m_event(event), m_time(time_point)
        {
        }

        const Event m_event;
        const time_point m_time;
};

struct Notifications {

        const fs::path m_path;
        std::list<Notification*> m_notifications;
};

}

#endif // __EVENT__
