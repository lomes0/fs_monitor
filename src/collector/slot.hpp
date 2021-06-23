#ifndef __SLOT__
#define __SLOT__

#include <queue>
#include <mutex>

using namespace std;

namespace fsm {
/*
 * Thread Safe Data Slot.
 */
template <typename P>
struct Slot {

        typedef queue<P> container;

        Slot();
        ~Slot();

        void push(P item);
        P pop();

        container* replace();
        bool empty();

        void lock();
        void unlock();

        private:
        container* m_items;
        std::mutex m_lock;

} __attribute__((__aligned__(64)));


template <typename P>
Slot<P>::Slot() {
	m_items = new container;
}


template <typename P>
Slot<P>::~Slot() {

        lock();
        if (!empty()) {
                //TODO:: throw exception
        }
        delete m_items;
        unlock();
}

template <typename P>
void Slot<P>::push(P item) {

	m_items->push(item);
}


template <typename P>
P Slot<P>::pop() {

	return m_items->front();
}

template <typename P>
typename Slot<P>::container* Slot<P>::replace() {

	auto ret = m_items;
        m_items = new container;
        return ret;
}

template <typename P>
        bool Slot<P>::empty() {

                // TODO:: validate no compiler optimizations...
                return m_items->empty();
        }

template <typename P>
        void Slot<P>::lock() {

                m_lock.lock();
        }
template <typename P>
void Slot<P>::unlock() {

                m_lock.unlock();
        }
}

#endif // __SLOT__
