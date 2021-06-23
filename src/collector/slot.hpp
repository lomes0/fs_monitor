#ifndef __SLOT__
#define __SLOT__

#include <queue>
#include <mutex>

namespace fsm {
/*
 * Thread Safe Data Slot.
 */
template <typename D>
struct Slot {

        using Container = std::queue<D*>;

        void push(D*);
        D* pop();

        private:
        Container m_container;
        std::mutex m_lock;
        /* std::lock_guard */
        /* reader writer lock */

} __attribute__((__aligned__(64)));

}

#endif // __SLOT__
