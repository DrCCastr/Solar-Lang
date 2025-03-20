/***
 * @file error.hpp
 */

#pragma once

//////////////
// Includes //
//////////////

#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

//////////
// Code //
//////////

namespace Solar {
namespace Error {

    class Error {
    public:
        string content;
        string file;
        size_t line;
        size_t column;

        Error(string content, string file = "Unknown", size_t line = 0, size_t column = 0)
            : content(move(content)), file(move(file)), line(line), column(column) {}

        string format() const {
            ostringstream oss;
            oss << "[Error] " << file << ":" << line << ":" << column << " -> " << content;
            return oss.str();
        }
    };

    class ErrorSesion {
    private:
        vector<Error> errors;

    public:
        void addError(string content, string file = "Unknown", size_t line = 0, size_t column = 0) {
            errors.emplace_back(move(content), move(file), line, column);
        }

        void debug(bool end = true) {
            if (errors.empty()) {
                cout << "No errors found.\n";
                return;
            }

            for (const auto& error : this->errors) {
                cerr << error.format() << endl;
            }

            if (end) {
                throw runtime_error("Stoping because Error Session");
            }
        }

        bool hasErrors() const {
            return !errors.empty();
        }

        void clear() {
            errors.clear();
        }
    };

}
}
