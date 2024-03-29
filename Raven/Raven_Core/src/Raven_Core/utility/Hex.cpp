#include "Hex.h"

namespace rvn {

    static constexpr char upper_chars[] = "0123456789ABCDEF";
    static constexpr char lower_chars[] = "0123456789abcdef";

    std::string Hex::toHexUpper(std::uint64_t num)
    {
        std::string res;
        char c = 0;
        for (int i = 0; i < 16; i++) {
            c = ((num >> ((15 - i) * 4)) & 0xF);
            res.push_back(upper_chars[c]);
        }
        return res;
    }

    std::string Hex::toHexLower(std::uint64_t num)
    {
        std::string res;
        char c = 0;
        for (int i = 0; i < 16; i++) {
            c = ((num >> ((15 - i) * 4)) & 0xF);
            res.push_back(lower_chars[c]);
        }
        return res;
    }

}