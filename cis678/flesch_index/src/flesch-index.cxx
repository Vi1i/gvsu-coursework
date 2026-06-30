#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "flesch-index.hpp"

fi::Flesch_Index::Flesch_Index(std::string& filename) {
    this->filename = filename;

    this->Init();
}

void fi::Flesch_Index::Init() {

}

void fi::Flesch_Index::Read() {
    struct stat filestatus;
    stat(filename.c_str(), &filestatus );
    unsigned int filesize = filestatus.st_size;
    if(!this->file_exists(this->filename)) {
        std::cerr << "File Not Found" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream ifs(this->filename.c_str());

    if(ifs) {
        char c;
        std::string word = "";
        std::vector<std::string> sentence;
        int p_count = 0;
        while(ifs.get(c)) {
            switch(c) {
                case '.':
                case '?':
                case '!':
                    if(!word.empty()) {
                        word = this->to_lower(this->strip(word));
                        if(!word.empty()) {
                            sentence.push_back(word);
                        }
                    }
                    if(!sentence.empty()) {
                        this->sentences.push_back(sentence);
                    }
                    word = "";
                    sentence.clear();
                    break;
                case '\n':
                case ' ':
                    if(!word.empty()) {
                        word = this->to_lower(this->strip(word));
                        if(!word.empty()) {
                            sentence.push_back(word);
                        }
                    }
                    word = "";
                    break;
                default:
                    word += c;
                    break;
            }
        }
    }
}
bool fi::Flesch_Index::is_vowel(char c) {
    char vowels[] = {'a', 'e', 'i', 'u'};
    for(auto vowel : vowels) {
        if(c == vowel) {
            return true;
        }
    }
    return false;
}

void fi::Flesch_Index::Analyze() {
    this->sentence_count();
    this->word_count();
    this->syllable_count();

    this->_fi = 206.835 - (84.6 * (double)(this->Syllables() /
                (double)this->Words())) - (1.015 * (double)(this->Words() /
                (double)this->Sentences()));
    this->_fk = 15.59 - (11.8 * (double)(this->Syllables() /
                (double)this->Words())) + (0.39 * (double)(this->Words() /
                (double)this->Sentences()));
}

void fi::Flesch_Index::syllable_count() {
    unsigned int syllables = 0;
    for(auto sentence : this->sentences) {
        for(auto word : sentence) {
            int temp_syllables = 0;
            for(auto z = 0; z < word.size(); z++) {
                if(this->is_vowel(word[z])) {
                    if(z == (word.size() - 1) && word[z] == 'e') {
                        break;
                    }
                    if(z == 0) {
                        temp_syllables++;
                        break;
                    }
                    if(!this->is_vowel(word[z - 1])) {
                        temp_syllables++;
                    }
                }
            }
            if(temp_syllables == 0) {
                syllables++;
            }else{
                syllables += temp_syllables;
            }
        }
    }

    this->_syllable_count = syllables;
}

void fi::Flesch_Index::word_count() {
    unsigned int words = 0;

    for(auto sentence : this->sentences) {
        words += sentence.size();
    }

    this->_word_count = words;
}
void fi::Flesch_Index::sentence_count() {
    unsigned int sentences = 0;
    sentences = (unsigned int)this->sentences.size();
    this->_sentence_count = sentences;
}

void fi::Flesch_Index::Print() {
    std::cout << "Syllables:\t" << this->Syllables() << std::endl;
    std::cout << "Words:\t\t" << this->Words() << std::endl;
    std::cout << "Sentences:\t" << this->Sentences() << std::endl;
    std::cout << "FI:\t\t" << this->FI() << std::endl;
    std::cout << "FK:\t\t" << this->FK() << std::endl;
}

/**
 * Simple check to make sure the file exists.
 */
bool fi::Flesch_Index::file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

/**
 * Strips a string of all characters that are not a member of alphanumeric
 */
std::string fi::Flesch_Index::strip(const std::string &s) {
    std::string result;
    result.reserve(s.length());

    std::remove_copy_if(s.begin(),
            s.end(),
            std::back_inserter(result),
            std::not1(std::ptr_fun(isalnum)));

    return result;
}
/**
 * Strips a string of all characters that are not a member of alphanumeric
 */
std::string fi::Flesch_Index::pattern(const std::string &s) {
    std::string result;
    for(auto c : s) {
        result += this->is_vowel(c) ? 'v' : 'c';
    }

    return result;
}
/**
 * Turns entire string into a lowercase version.
 */
std::string fi::Flesch_Index::to_lower(const std::string &s) {
    std::string result;
    result.reserve(s.length());

    std::transform(s.begin(),
            s.end(),
            std::back_inserter(result),
            ::tolower);

    return result;
}
