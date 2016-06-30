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
    vs.insert(end(vs), begin(vs), begin(vs)+n-1);

    vector<vector<string>> ngrams;
    for (vector<string>::const_iterator it = begin(vs); 
            it != end(vs) - n + 1;
            ++it) {
        vector<string> v {it, it + n};
        ngrams.push_back(v);
    }

    return ngrams;
}

using m_str_int = map<string, int>;
using freqmap = map<vector<string>, m_str_int>;

freqmap make_freqmap(vector<vector<string>> vvs) {
    freqmap m;
    for (auto vs:vvs) {
        vector<string> vs1 {begin(vs), end(vs)-1};
        m[vs1][*(--end(vs))]++;
    }
    return m;
}

using cumfreq = vector<pair<int, string>>;

cumfreq map2cumfreq(m_str_int m){
    cumfreq v;
    auto i = 0;
    for (m_str_int::const_iterator it = begin(m);
            it != end(m);
            ++it) {
        i += it->second;
        v.push_back(pair<int, string>{i, it->first});
    }
    return v;
}

using cumfreqmap = map<vector<string>, cumfreq>;

cumfreqmap make_cumfreqmap(freqmap freqm) {
    cumfreqmap m;
    for (freqmap::const_iterator it = begin(freqm);
            it != end(freqm);
            ++it) {
        m[it->first] = map2cumfreq(it->second);
    }
    return m;
}

cumfreqmap str2cumfreqmap(string s) {
    return make_cumfreqmap(
        make_freqmap(
            ngramify(
                str2vec(s), 3)));
}
