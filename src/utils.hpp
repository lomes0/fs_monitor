#ifndef __UTLIS__
#define __UTILS__

#include <vector>
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace fs = std::filesystem;
using Paths = vector<fs::path>;

/*
 * A very simple error print function wrapper.
 */
template <typename T, typename ...Tn>
void print_err_msg(T first, Tn... next);

template <typename ...Tn>
void print_err_msg(Tn... next);

template <typename T, typename ...Tn>
void print_err_msg(T first, Tn... next) {

        cout << "[Error] " << first << endl;
        print_err_msg(next...);
}

template <>
void print_err_msg() {

        cout << "[Info]  " << "Run fsmonitor -h for details." << endl;
}

/*
 *
 *
 */
bool validate_args(int argc, char** argv) {

        if (argc < 2) {
                print_err_msg("no input!");
                return false;
        }

        if (argc > 2) {
                print_err_msg("wrong number of arguments!");
                return false;        
        }

        // parse directories input path
        return true;
}

/*
 * @ Desc:
 *      check if p1 is nested in p2.
 */
bool is_nested(const fs::path& p1, const fs::path p2) {

        return false;
}

/*
 * @ Desc:
 *      Check if the new path is nested in a former path.
 *      Check if a former path is nested in the new path.
 *      Prefer the shorter path.
 */
void add_path(Paths& dirs, const string& path) {
	
        auto p = fs::path(path);
        
	for (auto dir : dirs) {

                if (is_nested(dir, path)) {
                        dir = path;
                }

                if (is_nested(path, dir)) {
                        return;
                }
	}

        dirs.push_back(p);
} 

/*
 * @Desc: 
 *      Parse the directory list.
 *      Filter nested paths, and populate to vector.
 */
bool parse_dir_list(Paths& dirs, const char* input) {

        ifstream f(input);
        
        if (!f.good()) {
                print_err_msg("Can't read input file: ", input);
                return false;
        }

        for (string line; getline(f, line);) {

                add_path(dirs, line);
        }

        f.close();

        return true;
 }

/*
 * @Desc
 *      Check all paths exists.
 *      Check all paths are directories.
 */
bool validate_dirs(Paths& dirs) {

        for (auto p : dirs) {

                if (!fs::exists(p)) {
                        
                        print_err_msg("Path does not exist!", p.u8string());
                        return false;
                }

                if (!fs::is_directory(p)) {

                        print_err_msg("Path is not a directory!", p.u8string());
                        return false;
                }
        }

        return true;
}

/*
 * @Desc
 *      Print directories to monitor and confirm.
 */ 
bool confirm_dirs(Paths& dirs) {

        cout << "Filtered Directory List:" << endl;
        for (auto p : dirs) {

                cout << p.u8string() << endl;
        }
        cout << "Continue? [y/n]" << endl;
        string s;
        cin >> s;
        
        return s.compare("y") == 0;
}

void init_logger() {

        // 6. create a log file.
        // 7. create an output stream.
        // 8. Init the logger instance.
}

void span_logger_thread() {
}

void span_monitor_threads(const Paths& dirs) {

        // 8. Span a monitor instance for each directory.
}

void wait() {
}

bool is_help_run(const char* arg) {

        string s{arg};

        if (s.compare("-h") == 0 ||
            s.compare("--h") == 0 ||
            s.compare("-help") == 0 ||
            s.compare("--help") == 0) {
                
                //print_help();
                return true;
        }

        return false;
}


#endif // __UTLIS__
