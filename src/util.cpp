#include "util.h"

namespace util {
    std::string FromFile(const char* filepath)
    {
        std::ifstream ist(filepath);
        if (!ist) {
            return {};
        }

        std::ostringstream os;
        os << ist.rdbuf();

        return os.str();
    }
}
