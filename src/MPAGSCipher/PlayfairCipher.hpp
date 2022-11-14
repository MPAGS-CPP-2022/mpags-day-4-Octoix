#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#include "CipherMode.hpp"
#include <string>
#include <map>

class PlayfairCipher {
    public:
        explicit PlayfairCipher(std::string key);
        std::string applyCipher(const std::string& inputText, const CipherMode cipherMode) const;
        // cannot modify the member variables of the class, with const
    private:
        void setKey(std::string key);
        std::string key_{""};
        const std::string alphabet_{"abcdefghijklmnopqrstuvwxyz"};
        std::map<char, std::pair<int, int>> charToCoord_;
        std::map<std::pair<int, int>, char> coordToChar_;
};

#endif