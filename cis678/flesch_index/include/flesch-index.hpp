#ifndef FLESCH_INDEX
#define FLESCH_INDEX

#include <string>
#include <vector>

namespace fi {
    class Flesch_Index {
    public:
        Flesch_Index(std::string& filename);
        void Read();
        void Analyze();
        void Print();

        unsigned int Syllables() const { return _syllable_count; }
        unsigned int Words() const { return _word_count; }
        unsigned int Sentences() const { return _sentence_count; }
        double FI() const { return _fi; }
        double FK() const { return _fk; }

    private:
        std::string filename;
        std::vector<std::vector<std::string>> sentences;
        unsigned int _syllable_count;
        unsigned int _word_count;
        unsigned int _sentence_count;
        double _fi;
        double _fk;

        void Init();
        bool file_exists(const std::string& name);
        std::string strip(const std::string &s);
        std::string to_lower(const std::string &s);
        std::string pattern(const std::string &s);
        bool is_vowel(char c);
        void syllable_count();
        void word_count();
        void sentence_count();
    };
}
#endif
