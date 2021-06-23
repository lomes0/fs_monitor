#ifndef __INOTIFY__
#define __INOTIFY__


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include <filesystem>
#include <map>

#include "./ievent.hpp"
#include "./event.hpp"

namespace fs = std::filesystem;
using namespace std;

#define EVENT_SIZE      ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN   ( 1024 * ( EVENT_SIZE + 16 ) )

namespace fsm {

/*
 * @Desc:
 *      A wrapper class for the inotify() system call.
 *      Idealy, Inotify class should be generic as possible,
 *      and expose all of inotify() functionality.
 */
class Inotify {

        public:
        Inotify(const fs::path& path, uint32_t mask);
	~Inotify();

        void callOnce(list<Notification*>& nlist) const;

        private:
        uint32_t m_mask;        /* events flags */ 
        int m_wd;               /* file descriptor for watch event on a file */
        int m_fd;               /* file descriptor for inotify() instance */
};

Inotify::Inotify(const fs::path& path, uint32_t mask) 
        : m_mask(mask)
{
        m_fd = inotify_init();

        if (m_fd < 0) {
                // Exception...
        }

        m_wd = inotify_add_watch(m_fd, "/tmp", mask);
}

Inotify::~Inotify() {

        inotify_rm_watch(m_fd, m_wd);
        close(m_fd);
}

void Inotify::callOnce(list<Notification*>& nlist) const {

        int len;
        char buffer[EVENT_BUF_LEN];

        len = read(m_fd, buffer, EVENT_BUF_LEN);
        if (len < 0) {
                return;
        }

        int i = 0;
        while (i < len) {
		struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
                if (event->len) {
			// create notification from event...
    		}

    		i += EVENT_SIZE + event->len;
        }
}

}

#endif //__INOTIFY__
