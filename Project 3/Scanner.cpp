//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"

#include <utility>
#include <iostream>
#include <fstream>

#include "utils.hpp"

Scanner::Scanner(std::filesystem::path inputPath) {
    // You complete this...

    // set the input path of the class to the given input path
    inputPath_ = inputPath;
}



std::string Scanner::readWord(std::ifstream& in) {
    // define the container to return from the function
    std::string word;
    // define the container to return into word
    char character;

    // skip over any leading non-letter characters
    while (in.get(character) && !isalpha(character)) {

    }

    // if we've reached the end of the file, return an empty string
    if (!in)
        return "";

    // otherwise, we've hit a letter, so add it
    word += tolower(character);

    // process until we hit a non-letter
    while (in.get(character)) {
        // if we hit a letter, add it to word
        if (isalpha(character))
            word += tolower(character);

        // if we hit an apostrophe and the character after is it a letter,
        // add it to word
        else if (character == '\'' && isalpha(in.peek()))
            word += character;

        // otherwise, we hit the end of the word
        else
            break;
    }

    return word;
}



error_type Scanner::tokenize(std::vector<std::string>& words) {

    // define an input file stream with our input path
    std::ifstream inputFile(inputPath_);

    // if it didn't open correctly, throw an error
    if (!inputFile.is_open())
        return UNABLE_TO_OPEN_FILE;

    // define a temporary container to push back into words repeatedly
    std::string word;
    // call readWord and put the result into word
    word = readWord(inputFile);

    // as long as word isn't an empty string,
    // push back word into words and go to the next word
    while (!word.empty()) {
        words.push_back(word);
        word = readWord(inputFile);
    }

    return NO_ERROR;
}



error_type Scanner::tokenize(std::vector<std::string>& words, const std::filesystem::path& outputFile) {
    // define an error type, built to handle exceptions from the other tokenize function.
    error_type status = tokenize(words);

    // if we have an error, throw it.
    if (status != NO_ERROR)
        return status;

    // otherwise, write words to our .tokens file!
    return writeVectorToFile(outputFile.string(), words);
}