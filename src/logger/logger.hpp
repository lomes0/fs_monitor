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

        //void add(const N* e);
        void add(list<N*>* list) {
        }

        private:
        ofstream* m_ostream;
};

template <typename N>
Logger<N, ofstream>::~Logger() {

        //TODO::IMPL
        m_ostream->close();
        delete m_ostream;
}

template <typename N>
Logger<N, ofstream>::Logger(const fs::path& p) {

        m_ostream = new ofstream;
        m_ostream->open(p.u8string());
}

/*template <typename N>
void Logger<N, ofstream>::add(const N* e) {

}*/


}

#endif // __LOGGER__
