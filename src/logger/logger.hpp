#ifndef __LOGGER__
#define __LOGGER__

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;
using namespace std;

namespace fsm {


/*
 * Partial Specialization of class member functions is not allowed.
 * 
 * Ideally, We could have only had one Logger class...
 */

template <typename E>
class LogBase {

        virtual void add(const E* e) = 0;
};

template <typename E, typename S>
class Logger : LogBase<E> {};

/*
 * @Desc:
 *      File logger using ofstream.
 */
template <typename E>
class Logger<E, ofstream> : LogBase<E> {

        public:
        Logger(const fs::path& p);
        ~Logger();

        void add(const E* e);

        private:
        ofstream* m_ostream;
};

template <typename E>
Logger<E, ofstream>::~Logger() {

        //TODO::IMPL
        m_ostream->close();
        delete m_ostream;
}

template <typename E>
Logger<E, ofstream>::Logger(const fs::path& p) {

        m_ostream = new ofstream;
        m_ostream->open(p.u8string());
}

template <typename E>
void Logger<E, ofstream>::add(const E* e) {

}


}

#endif // __LOGGER__
