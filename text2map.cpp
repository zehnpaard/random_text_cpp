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

using freqmap = map<vector<string>, map<string, int>>;

freqmap make_freqmap(vector<vector<string>> vvs) {
    freqmap m;
    for (auto vs:vvs) {
        vector<string> vs1 {begin(vs), end(vs)-1};
        m[vs1][*(--end(vs))]++;
    }
    return m;
}


// Tests

void test_str2vec() {
    string sentence {"whether 'tis nobler in the mind to suffer the slings and arrows of outrageous fortune"};
    auto tokens = str2vec(sentence);
    
    vector<string> expected = {"whether", "'tis", "nobler", "in", "the", "mind", "to", "suffer", "the", "slings", "and", "arrows", "of", "outrageous", "fortune"};

 
    if (tokens != expected) {
        auto print = [](vector<string> vs){
            for (auto s:vs) {
                cout << s << endl;
            }
        };

        cout << "testing str2vec failed!" << endl;

        cout << "Output:" << endl;
        print(tokens);
        cout << endl;

        cout << "Expected:" << endl;
        print(expected);
        string s;

        cout << "Enter any character to continue:";
        cin >> s;
    }
}


void test_ngramify() {
    string sentence {"whether 'tis nobler in the mind"};
    auto tokens = str2vec(sentence);
    auto ngrams = ngramify(tokens, 3);

    vector<vector<string>> expected = {
        vector<string> {"whether", "'tis", "nobler"},
        vector<string> {"'tis", "nobler", "in"},
        vector<string> {"nobler", "in", "the"},
        vector<string> {"in", "the", "mind"},
        vector<string> {"the", "mind", "whether"},
        vector<string> {"mind", "whether", "'tis"} 
    };

    if (ngrams != expected) {

        auto print = [](vector<vector<string>> vss){
            for (auto vs:vss) {
                for (auto s:vs) {
                    cout << s << " ";
                }
                cout << endl;
            }
        };

        cout << "testing ngramify failed!" << endl;

        cout << "Output:" << endl;
        print(ngrams);
        cout << endl;

        cout << "Expected:" << endl;
        print(expected);
        string s;

        cout << "Enter any character to continue:";
        cin >> s;
    }
}

 
void test_make_freqmap() {
    string sentence {"whether 'tis nobler in the mind"};
    auto tokens = str2vec(sentence);
    auto ngrams = ngramify(tokens, 4);
    auto freqm = make_freqmap(ngrams);

    auto print = [](freqmap freqm) {
        for (freqmap::const_iterator it = begin(freqm);
                it != end(freqm);
                ++it) {
            for (auto x: it->first) {
                cout << x << " ";
            }
            cout << ":" << endl;
            for (map<string, int>::const_iterator it2 = begin(it->second);
                    it2 != end(it->second);
                    ++it2) {
                cout << it2->first << "\t\t" << it2->second << endl;
            }
            cout << endl;
        }
    };

    if (1) {
        print(freqm);
    }
}


void test() {
    test_str2vec();
    test_ngramify();
    test_make_freqmap();
}


// Main

int main() {
    test();
    return 0;
}
