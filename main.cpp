#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cstdlib>

#include "Benchmark.cpp"

std::vector<int> naiveSearch(const std::string &text, const std::string &word)
{
    std::vector<int> result;
    for(unsigned int i = 0; i < text.size() - word.size() + 1; i++)
    {
        unsigned int j;
        for(j = 0; j < word.size(); j++)
        {
              if(word.at(j) != text.at(i + j))
                  break;
        }
        if(j == word.size())
            result.push_back(i);
            
    }

    return result;
}

std::vector<int> createLPS(const std::string& word)
{
    std::vector<int> lps(word.size());
    lps[0] = 0;
    unsigned int i = 1;
    int len = 0;
    while(i < word.size())
    {
            if(word.at(i) == word.at(len))
            {
                len++;
                lps[i] = len;
                i++;
            }
            else
            {
                if(len != 0)
                {
                        len = lps.at(len - 1);
                }
                else
                {
                        lps[i] = 0;
                        i++;
                }
            }
    }
    return lps;
}


std::vector<int> kmpSearch(const std::string &text, const std::string &word)
{
    std::vector<int> lps = createLPS(word);
    std::vector<int> result;
    unsigned int i = 0, j = 0;
    while(i < text.size())
    {
            if(word.at(j) == text.at(i))
            {
                    j++;
                    i++;
            }

            if(j == word.size())
            {
                result.push_back(i - j);
                j = lps.at(j - 1);
            }
            else if(i < text.size() && word.at(j) != text.at(i))
            {
                j != 0 ? j = lps.at(j - 1) : i++; 
            }
    }

    return result;
}

std::string randomString(const int len) {
    static const char alphanum[] = "ABC";
    std::string result;

    for(int i = 0; i < len; ++i)
    {
        result.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
    }
    return result;
}



int main()
{
    std::string test = "ABAB";
    std::vector<int> result;
    srand(time(0));

    assert(naiveSearch("BB", "BB").size() == 1);
    assert(naiveSearch("BB", "BB")[0] == 0);
    assert(naiveSearch("BBB", "BB").size() == 2);
    assert(naiveSearch("BBB", "BB")[1] == 1);

    assert(kmpSearch("BB", "BB").size() == 1);
    assert(kmpSearch("BB", "BB")[0] == 0);
    assert(kmpSearch("BBB", "BB").size() == 2);
    assert(kmpSearch("BBB", "BB")[1] == 1);


    static int testSize[5] = {1000, 10000, 100000, 1000000, 10000000};
    std::string testString;
    int len;
    for(int i = 0; i < 5; i++)
    {
        testString = randomString(testSize[i]);

        auto t1 = std::chrono::high_resolution_clock::now();
        len = naiveSearch(testString, test).size();
        auto t2 = std::chrono::high_resolution_clock::now();

        auto time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        std::cout << "Naive Search\n";
        std::cout << "\t" << len << "\t" << test << "\t" << time << "\t" << testSize[i] << std::endl;

        t1 = std::chrono::high_resolution_clock::now();
        len = kmpSearch(testString, test).size();
        t2 = std::chrono::high_resolution_clock::now();

        time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        std::cout << "KMP Search\n";
        std::cout << "\t" << len << "\t" << test << "\t" << time << "\t" << testSize[i] << std::endl;
    }

}
