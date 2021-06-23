#ifndef __MONITOR__
#define __MONITOR__

#include <filesystem>
#include "./inotify.hpp"

using namespace std;
namespace fs = std::filesystem;

namespace fsm {

template <typename F>
class DirMon {

        public:
        DirMon();

        private:
        fs::path m_dir;
        Inotify m_inotify;
};

}

#endif // __MONITOR__
