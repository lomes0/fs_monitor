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
 *      Each thread writes events to some slot, identified by some id.
 *
 *      Collector scans for new updates, and transmit to log.
 *
 *      The log deletes incoming messages.
 */
template <typename N>
class Collector {

        typedef std::function<void(list<N*>*)> Callback;
        typedef std::atomic<bool> atomic_bool;
        typedef typename Slot<list<N*>*>::container SlotContainer;

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

        void log_and_delete(SlotContainer* container) const;

        private:

        auto get_slot(uint8_t i) {

                return m_slots + i;
        }

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

                auto slot = m_slots + i;
                auto container = slot->replace();
                log_and_delete(container);
        }

        delete m_slots;
}

bool flag_down(const atomic_bool& flag) {

	return flag.load();
}

/*
 * @Desc:
 *      Run untils main thread set flag=true.
 *      Iterativly reads each slot, and forwards notifications
 *      to Logger.
 */
template <typename N>
void Collector<N>::start(const atomic_bool& flag) {

        while(flag_down(flag)) {

                uint8_t i = 0;
                while (i++ < m_size) {

                        auto slot = get_slot(i);
                        if (!slot->empty()) {

                                /* Replace slot container, and forward notifications to the logger. */
                                slot->lock();
                                auto container = slot->replace();
                                slot->unlock();
                                log_and_delete(container);
                        }
                }
        }
}


/*
 * @Desc:
 *      Invoked by MonDir whenever inotify() returns new events.
 *      Takes a list of notifications, and safely inserts to slot_i.
 */
template <typename N>
void Collector<N>::push(list<N*>* nlist, uint8_t i) {

	auto slot = get_slot(i);
        slot->lock();
        slot->push(nlist);
        slot->unlock();
}

/*
 * @Desc:
 *      Invoked whenever incoming notifications are identified.
 *      Takes the slot notifications, and forwards them to Logger.
 *      Logger deletes each notification.
 */
template <typename N>
void Collector<N>::log_and_delete(SlotContainer* container) const {

        /* Forward each list in Slot Queue to the logger */
        /* Idealy, slot should provide iterator api. */

        while (!container->empty()) {
                
                auto list = container->front();
                container->pop();
                m_callback(list);
        }

        delete container;
}

}


#endif // __COLLECTOR__
