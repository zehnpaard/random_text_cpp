#include <iostream>

#include <vector>
#include <map>
#include <string>

#include <sstream>
#include <iterator>


using std::cout;
using std::cin;
using std::endl;

using std::vector;
using std::pair;
using std::map;
using std::string;

using std::istringstream;
using std::istream_iterator;



vector<string> str2vec(const string sentence) {
    // Take a string, split on whitespace and return a vector of words
    istringstream word_stream{sentence};
    vector<string> tokens {
        istream_iterator<string>{word_stream},
        istream_iterator<string>{}
    };
    return tokens;
}

// maybe make vs a reference to avoid copying?
// but this would make it destructive
vector<vector<string>> ngramify(vector<string> vs, int n) {
    // Take a vector of words and convert to a vector of vectors of n words each
    // using a sliding window rather than chunks
    vs.insert(end(vs), begin(vs), begin(vs)+n-1);

    vector<vector<string>> ngrams;
    for (vector<string>::const_iterator it = begin(vs); 
            it != end(vs) - n + 1;
            ++it) {
        vector<string> ngram {it, it + n};
        ngrams.push_back(ngram);
    }

    return ngrams;
}

using m_str_int = map<string, int>;
using freqmap = map<vector<string>, m_str_int>;

freqmap ngrams2freqmap(vector<vector<string>> vvs) {
    // Take a vector of n-grams and convert to a map of (n-1)grams 
    // and a frequency map of words following the (n-1)gram 
    freqmap m;
    for (auto ngram:vvs) {
        vector<string> n_1gram {begin(ngram), end(ngram)-1};
        m[n_1gram][*(--end(ngram))]++;
    }
    return m;
}

using cumfreq = vector<pair<int, string>>;

cumfreq map2cumfreq(m_str_int m){
    // Take a frequency map of words and their frequency
    // and convert to a vector of pairs of cumulative frequencies and words
    cumfreq v;
    auto i = 0;
    for (m_str_int::const_iterator it = begin(m);
            it != end(m);
            ++it) {
        // it->first is a word
        // it->second is the frequency of that word
        i += it->second;
        v.push_back(pair<int, string>{i, it->first});
    }
    return v;
}

using cumfreqmap = map<vector<string>, cumfreq>;

cumfreqmap freqmap2cumfreqmap(freqmap freqm) {
    // Take a map of (n-1)grams to the frequency map of subsequent words
    // and convert to a map of (n-1)grams to the cumulative frequency vectors
    cumfreqmap m;
    for (freqmap::const_iterator it = begin(freqm);
            it != end(freqm);
            ++it) {
        // it->first is the (n-1)gram
        // it->second is the frequency map
        m[it->first] = map2cumfreq(it->second);
    }
    return m;
}

cumfreqmap str2cumfreqmap(string s, int n) {
    // Take a string and generate a map between (n-1)grams and
    // the vector of cumulative frequencies of subsequent words
    return freqmap2cumfreqmap(
        ngrams2freqmap(
            ngramify(
                str2vec(s), n)));
}

// TODO
// 1. Take a file name, load text file and convert to string
// 2. Use cumulative frequency map to randomly pick one (n-1)gram
// 3. Use cumulative frequency map to generate nth word given (n-1)gram
