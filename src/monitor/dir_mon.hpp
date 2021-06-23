#ifndef __MONITOR__
#define __MONITOR__

#include <filesystem>
#include <functional>
#include <initializer_list>
#include <atomic>

#include "./inotify.hpp"

using namespace std;
namespace fs = std::filesystem;

namespace fsm {

uint32_t get_mask(const initializer_list<Event> events) {

        uint32_t mask = 0;
        for (auto e : events) {
                mask |= static_cast<uint32_t>(e);
        }

        return mask;
}

class DirMon {

        typedef std::function<void(list<Notification*>&)> Callback;

        public:

        DirMon(const fs::path& path, initializer_list<Event> events)
                : m_path(path)
        {
               uint32_t mask = get_mask(events);
               m_inotify = new Inotify(path, mask);
        }

        ~DirMon() {

                delete m_inotify;
        }

        void start(Callback action, atomic_bool& flag) {

                /*while (!flag.load()) {
                        
                        NotList nlist;
                        m_inotify->callOnce(nlist);
                        action(nlist);
                }*/
        }

        private:
        fs::path m_path;
        Inotify* m_inotify;

        // Epoll class.
        // files to monitor
};

}

#endif // __MONITOR__
