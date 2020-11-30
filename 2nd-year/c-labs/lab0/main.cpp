#include <fstream>
#include <map>
#include <string>

using namespace std;

int main(int argc, char** argv) {

    ifstream input(argv[1]);
    ofstream output(argv[2]);
    string string1;
    string basicstr = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    map <string, int> wordMap;
    getline(input, string1);
    int pos = 0, prevPos = 0;
    int wordCount = 0;
    while(true){
        pos = string1.find_first_not_of(basicstr, prevPos);
        if (pos == -1){
            if (prevPos != size(string1)){
                pos = size(string1);
            }
            else
                break;
        }
        if (prevPos != pos) {
            string newWord = string1.substr(prevPos, pos-prevPos);

            if (wordMap.find(newWord) == wordMap.end())
                wordMap[newWord] = 1;
            else
                wordMap[newWord]++;
            wordCount++;
            prevPos = pos + 1;
        }
        else{
            prevPos++;
        }
    }
    map <string, int> :: iterator it;
    int max = 0;
    while(wordMap.begin() != wordMap.end() ) {
        max = 0;
        for (it = wordMap.begin(); it != wordMap.end(); it++) {
            if (max < it->second)
                max = it->second;
        }
        for (it = wordMap.begin(); it != wordMap.end(); it++) {
            if (it->second == max){
                output << it->first << ';' << it->second << ';' << int(float(it->second) / wordCount * 100) << std::endl;
                break;
            }
        }
        wordMap.erase(it);
    }
    return 0;
}
