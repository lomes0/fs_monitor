#ifndef __LOGGER__
#define __LOGGER__

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <list>

namespace fs = std::filesystem;
using namespace std;

namespace fsm {


/*
 * Partial Specialization of class member functions is not allowed.
 * 
 * Ideally, We could have only had one Logger class...
 */

template <typename N>
class LogBase {

        virtual void add(list<N*>* e) = 0;
};

template <typename N, typename S>
class Logger : LogBase<N> {};

/*
 * @Desc:
 *      File logger using ofstream.
 */
template <typename N>
class Logger<N, ofstream> : LogBase<N> {

        public:
        Logger(const fs::path& p);
        ~Logger();

        void add(list<N*>* list);

        private:
        ofstream* m_ostream;
};

template <typename N>
Logger<N, ofstream>::~Logger() {

        m_ostream->close();
        delete m_ostream;
}

template <typename N>
Logger<N, ofstream>::Logger(const fs::path& p) {

        m_ostream = new ofstream;
        m_ostream->open(p.u8string());
}

/*
 * @Desc:
 *      Invoked by Collector whenever new notifications are identified.
 *      Logger is responsible for deleting all incoming pointers.
 */
template <typename N>
void Logger<N, ofstream>::add(list<N*>* list) {

}


}

#endif // __LOGGER__
