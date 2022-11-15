#include "PlayfairCipher.hpp"
#include <string>
#include <algorithm>
#include <iostream>

PlayfairCipher::PlayfairCipher(std::string key) {
    PlayfairCipher::setKey(key);
}

void PlayfairCipher::setKey(std::string key) {
    // store the original key
    key_ = key;
    // Append the alphabet
    key_ += alphabet_;

    // Make sure the key is upper case
    std::transform(key_.cbegin(), key_.cend(), key_.begin(), ::toupper);
    
    // Remove non-alpha characters
    auto isNotAlpha {std::remove_if(key_.begin(), key_.end(), [] (char c) { return !std::isalpha(c); })};
    key_.erase(isNotAlpha, key_.end());

    // Change J -> I
    std::transform(key_.begin(), key_.end(), key_.begin(), [] (char c) { return c == 'J' ? 'I' : c; });
    
    // Remove duplicated letters
    std::string encounterdLetters{""};
    auto isNotDuplicate {std::remove_if(key_.begin(), key_.end(), [&encounterdLetters] (char c) {
        auto it = std::find(encounterdLetters.begin(), encounterdLetters.end(), c);
        if (it == encounterdLetters.end()) {
            // Letter has not yet been seen in the key
            encounterdLetters += c;
            return false;
        } else {
            return true;
        }
    })};
    key_.erase(isNotDuplicate, key_.end());

    // Store the coords of each letter
    int i{0}; // counter
    std::pair<int, int> coords {0, 0}; // this is an (x,y) pair
    for (char& c : key_) {
        charToCoord_.insert(std::make_pair(c, coords));
        coordToChar_.insert(std::make_pair(coords, c));
        if ((i+1) % 5 == 0) {
            // We are on the last element, so next element needs to jump down a row
            coords.first = -1; // Set to -1 as we always add 1 to the column
            coords.second += 1;
        }
        coords.first++;
        i++;
    }
}

std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode) const {
    // TODO: enable the decrypt mechanism in this as well 
    if (cipherMode == CipherMode::Decrypt) {
        std::cout << "Decrypting has not yet been implemented!" << std::endl;
        return inputText;
    }

    std::string output{""};
    output.resize(inputText.size()); // our output should be the same size as our input
    // ChangeJ→I
    std::transform(inputText.begin(), inputText.end(), output.begin(), [] (char c) { return c == 'J' ? 'I' : c; });

    // If repeated chars in a digraph add an X or Q if XX
    for (uint i {1}; i < output.length(); i++) {
        if (output[i-1] == output[i]) {
            output[i] = (output[i-1] == 'X') ? 'Q' : 'X';
        }
    }
    
    // if the size of input is odd, add a trailing Z
    if (output.size() % 2 != 0) { 
        output += 'Z';
    }

    // Loop over the input in Digraphs
    for (uint i {0}; i < output.length(); i += 2) {
        // Find the coords in the grid for each digraph
        std::pair<int, int> newCoords1 {charToCoord_.at(output[i])};
        std::pair<int, int> newCoords2 {charToCoord_.at(output[i+1])};
        // Apply the rules to these coords to get 'new' coords
        if (newCoords1.first == newCoords2.first) {
            // The digraph form a column (same x values)
            newCoords1 = {newCoords1.first, (newCoords1.second+1)%5};
            newCoords2 = {newCoords2.first, (newCoords2.second+1)%5}; 
        } else if (newCoords2.second == newCoords1.second) {
            // The digraph form a row (same y values)
            newCoords1 = {(newCoords1.first+1)%5, newCoords1.second};
            newCoords2 = {(newCoords2.first+1)%5, newCoords2.second};  
        } else {
            // Must form a rectangle on the 5x5 grid
            newCoords1 = {newCoords2.first, newCoords1.second};
            newCoords2 = {newCoords1.first, newCoords2.second};
        }
        // Find the letter associated with the new coords 
        auto newChar1 {coordToChar_.find(newCoords1)}; // Find the new "encrypted" character
        auto newChar2 {coordToChar_.find(newCoords2)};
        output[i] = newChar1->second; // update the output text with the new character
        output[i+1] = newChar2->second;
    }
    return output;
}