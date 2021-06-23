#include <functional>
#include <list>
#include <thread>
#include <atomic>
#include <future>

#include "./utils.hpp"
#include "./monitor/dir_mon.hpp"
#include "./monitor/event.hpp"
#include "./logger/logger.hpp"
#include "./collector/collector.hpp"

using namespace fsm;

Paths dirs;

bool init(int argc, char** argv) {

        if (!validate_args(argc, argv)) {
                return false;
        }

        if (is_help_run(argv[1])) {
                return true;
        }

        if (!parse_dir_list(dirs, argv[1])) {
                return false;
        }

        if (!validate_dirs(dirs)) {
                return false;
        }

        if (!confirm_dirs(dirs)) {
                return false;
        }

        /* limit number of directories to watch */
        if (32 < dirs.size()) {
                return false;
        }
        
        return true;
}

int main(int argc, char** argv) {

        typedef list<DirMon*> MonList;
        typedef Collector<Notification> EventsCol;
        typedef Logger<Notification, ofstream> EventsLog;
        using namespace std::placeholders;


        if (!init(argc, argv)) {
                return 1;
        }
        
        MonList monitors;
        uint8_t n_threads = dirs.size();
        atomic_bool stop;
	stop = false;

        EventsLog log("path to log");
        auto toLogger = bind(&EventsLog::add, &log, _1);
        EventsCol collector( n_threads, toLogger );

        auto init_mon_list = [](MonList& monitors) {

                for (auto p : dirs) {

                        DirMon* mon = new DirMon(p, { /* list of events */ });
                        monitors.push_back(mon);
                }
        };

        auto delete_mon_list = [](MonList& monitors) {

                for (auto mon : monitors) {
                        delete mon;
                }
        };

        auto wait_for_signal = []() {

                /* wait for user to type something */
        };

        /*
         * Start all monitor threads,
         * Wait for exist signal,
         * Notify threads to exit.
         */
        auto run = [&collector, &stop](MonList& monitors) {

                uint8_t i = 0;
                for (auto mon : monitors) {

                        /* 
                         * Insert function to slot i of Collector.
                         * Takes a list of Events.
                         */
                        auto push2SlotI = std::bind(&EventsCol::push, &collector, _1, i++);

                        /*
                         * Directory monitor watch function.
                         * Runs until flag is changed.
                         * Invokes a callback whenever inotify returns.
                         */
                        auto mon_loop = std::bind(&DirMon::start, mon, _1, std::ref(stop));

                        auto ret = std::async(mon_loop, push2SlotI);
                }
                
        };

        collector.start(std::ref(stop));
        init_mon_list(monitors);

        run(monitors);

        wait_for_signal();
        stop.store(true);

        delete_mon_list(monitors);
}
