#ifndef UTIL_HPP
#define UTIL_HPP

#define instanceof(a, b) (typeid(a) == typeid(b))

bool FileExists(const std::string& filename);

#endif // UTIL_HPP
