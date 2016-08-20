#include "include.hpp"

bool FileExists(const std::string& filename) {
    if (FILE *file = fopen(filename.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}
