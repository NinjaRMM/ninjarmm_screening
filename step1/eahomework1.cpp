// eahomework1.cpp : Reads in a list of commonwords and looks for each in a list of allwords.
// 
// Commonwords is unsorted but should appear sorted when listed for each allword
// Allwords list output should appear in same order as read in
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>    // std::sort
#include <queue>
#include <future>
#include <chrono>

using namespace std;
vector<string> vCommonWords;


string wordFunc(const string allWord)
{
    string outLine = allWord + ":";
    bool firstWord = true;

    for (unsigned int i = 0; i < vCommonWords.size(); i++)
    {
        if (strstr(allWord.c_str(), vCommonWords[i].c_str()))
        {
            outLine = outLine + (firstWord ? " " : ", ") + vCommonWords[i];
            firstWord = false;
        }
    }
    outLine += "\n";

    return outLine;

}

int main(int argc, char* argv[])
{
    string cwLine, awLine;
    string cwFile, awFile, outputFile;
    queue<int> qWordTaskNum;
    queue<future<string>> wordTasks;
    int numAllWords = 0;

    cout << argv[0] << " eahomework1.cpp - show what commonwords appear in each allword\n";
    if (argc != 4)
    {
        cout << "   Usage:  " << argv[0] << " commonfile  allfile  outputfile\n";
        exit(-1);
    }
    cwFile = argv[1];
    awFile = argv[2];
    outputFile = argv[3];
    
    cout << "\n  Using - \nCommonwords: " << cwFile << "\nAllwords: " << awFile << "\nOutput: " << outputFile << "\n";

    ifstream cwHandle(cwFile);
    if (!cwHandle.is_open())
    {
        cout << "Error: Could not open commonword file for input: " << cwFile << "\n";
        exit(-1);
    }
    ifstream awHandle(awFile);
    if (!awHandle.is_open())
    {
        cout << "Error: Could not open allword file for input: " << awFile << "\n";
        exit(-1);
    }

    ofstream oHandle(outputFile, ios::trunc);
    if (!oHandle.is_open())
    {
        cout << "Error: Could not create for output: " << outputFile << "\n";
        exit(-1);
    }

// read/load all commonwords into vCommonWords vector
    while (getline(cwHandle, cwLine))
    {
        vCommonWords.push_back(cwLine);
    }
    cwHandle.close();

// Sort commonwords alphabetically so they appear sorted in output
    sort(vCommonWords.begin(), vCommonWords.end());

// loop thru the allwords.txt file - find commonwords for each allwords
// push current word onto queue for processing. Pop all words that are done processing
    while (!getline(awHandle, awLine).eof())
    {
        numAllWords++;
        wordTasks.push(async(launch::async, wordFunc, awLine));
        
        while( !wordTasks.empty() && wordTasks.front().wait_for(std::chrono::seconds(0)) == 
            std::future_status::ready)
        {
            oHandle << wordTasks.front().get();
            wordTasks.pop();
        }
    }
    awHandle.close();

// Empty the queue of any remaining jobs
    while (!wordTasks.empty()) {
        if (wordTasks.front().wait_for(std::chrono::seconds(0)) ==
            std::future_status::ready)
        {
            oHandle << wordTasks.front().get();
            wordTasks.pop();
        }
    }

    oHandle.close();

    return 0;
}
