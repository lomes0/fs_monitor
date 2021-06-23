#ifndef __COLLECTOR__
#define __COLLECTOR__

#include <functional>
#include <atomic>

#include <list>
#include "./slot.hpp"

using namespace std;

namespace fsm {

/*
 * @Desc:
 *      Receives incoming events from spawned threads.
 * 
 *      Each thread writes events to some unique slot.
 *
 *      The Collector instance scan the memory for updates.
 *
 *      Responsible for realsing memory of incoming events.
 */
template <typename N>
class Collector {

        typedef std::function<void(N*)> Callback;
        typedef std::atomic<bool> atomic_bool;

        public:
        Collector(uint8_t size, Callback);
        ~Collector();

        /*
         * @Desc:
         *      Scan for incoming Events.
         *      Forward to Log whenever possible.
         */
        void start(const atomic_bool& flag);

        /*
         * @Desc:
         *      Thread Safe insert to Collector storage space.
         */
        void push(list<N*>*, uint8_t i);

        private:

        Slot<list<N*>*>* m_slots;
        Callback m_callback;
        uint8_t m_size;
};

template <typename N>
Collector<N>::Collector(uint8_t size, Callback Callback)
        : m_callback(std::move(Callback))
{
        m_slots = new Slot<list<N*>*>[size];
        m_size = size;
}

template <typename N>
Collector<N>::~Collector() {

        for (uint8_t i = 0; i < m_size; i++) {

                list<N*>* l = m_slots[i];
                for (auto n : *l) {

                        // write n to log
                        delete n;
                }
                delete l;
        }

        delete m_slots;
}

template <typename N>
void Collector<N>::start(const atomic_bool& flag) {

        while(!flag.load()) {
        }
}


template <typename N>
void Collector<N>::push(list<N*>* nlist, uint8_t i) {
}

}


#endif // __COLLECTOR__
