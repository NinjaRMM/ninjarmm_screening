#include <iostream>
#include <list>
#include <vector>
#include "string"
#include <cmath>
#include <cstdio>
#include <cassert>
#include <stdint.h>
#include <string.h>

using namespace std;

class job {
public:
    virtual void doWork() = 0;
    virtual string getJobName() = 0;
    virtual string getJobDescription() = 0;
    virtual int getJobHours() =0;
};

class programmer : public job {
    string jobName, jobDescription;
    int jobNumHours;
public:
    programmer(string jobName, string jobDescription, int  jobNumHours);
    void doWork() override;
    string getJobName() override;
    string getJobDescription() override;
    int getJobHours() override;
};

class pilot : public job {
    string jobName, jobDescription;
    int jobNumHours;
public:
    pilot(string jobName, string jobDescription, int  jobNumHours);
    void doWork() override;
    string getJobName() override;
    string getJobDescription() override;
    int getJobHours() override;
};

programmer::programmer(string jobName, string jobDescription, int jobNumHours  ): jobName(jobName),jobDescription(jobDescription), jobNumHours(jobNumHours){}
void programmer::doWork()
{
    cout << "My work involves" << " " << jobDescription << std::endl;
}
string programmer::getJobName()
{
    return jobName;
}
string programmer::getJobDescription()
{
    return jobDescription;
}
int programmer::getJobHours()
{
    return jobNumHours;
}

pilot::pilot(string jobName, string jobDescription, int jobNumHours  ): jobName(jobName),jobDescription(jobDescription), jobNumHours(jobNumHours){}
void pilot::doWork()
{
    cout << "My work involves" << " " << jobDescription << std::endl;
}
string pilot::getJobName()
{
    return jobName;
}
string pilot::getJobDescription()
{
    return jobDescription;
}
int pilot::getJobHours()
{
    return jobNumHours;
}

int countMatchItemInVector(vector<string> stringList, string searchWord)
{
    auto numItemsMatch = std::count(stringList.begin(), stringList.end(), searchWord);
    return numItemsMatch;
}

template<class...A> void func1(A...arg){
    assert(false);
}

void func1(int a1, int a2, int a3, int a4, int a5, int a6){
    printf("call with(%d,%d,%d,%d,%d,%d)\n",a1,a2,a3,a4,a5,a6);
}

template<class...A> int func(A...args){
    int size = sizeof...(A);
    switch(size){
        case 0: func1(99,99,99,99,99,99);
            break;
        case 1: func1(99,99,args...,99,99,99);
            break;
        case 2: func1(99,99,args...,99,99);
            break;
        case 3: func1(args...,99,99,99);
            break;
        case 4: func1(99,args...,99);
            break;
        case 5: func1(99,args...);
            break;
        case 6: func1(args...);
            break;
        default:
            func1(0,0,0,0,0,0);
    }
    return size;
}

void printarray(int arg[], int length){
    for(int n=0; n<length; n++){
        printf("%d ",arg[n]);
    }
    printf("\n");
}

template<class...A> void funca(A...args){
    const int size = sizeof...(args) +5;
    printf("size %d\n", size);
    int res[sizeof...(args)+5]={99,98,args...,97,96,95};
    printarray(res,size);
}

template <typename T>
bool IsInBounds(const T& value, uint32_t low, uint32_t high) {
    cout << "Range Check Value:" << value << " Low Range:" << low << " High Range:" << high << std::endl;
    return !(value < low) && (value < high);
}

int romanValue(char r);
int calculateNum(const char* data);

int countpairs(const char*data);
int countOnePairs(int numCount, int *checkForThreePair);
int countTripplePairs(int numCount);
int checkforLeftOvers(int numCounts[]);
int checkForMoreThanOnePair(int twoPairsTotal[]);

int datacount[10];
int twoPairTotal[10];
int threePairTotal[10];

int main() {

    int completeHand=0;
    int morethanonepaird =0;
    int onepair = 0;
    int i;
    int leftover;
    int checkForThreePair = 0;
    int twoPairTotal[10];
    int threePairTotal[10];
    int oneThanOnePairsInDifferentLocs;

    const char * TestNumerals[] = {
            "I",
            "III",
            "IV",
            "XII",
            "XC",
            "MMXXII"
    };

    int completeHandPassCount = 0;
    char* completeSet[]  = {"True", "False", "True", "True", "False", "True", "False", "False",
                            "True", "False", "False", "False", "True", "True", "False", "False","True","False"};

    const char* tiles[] = {"11133555","111333555", "00000111", "13233121",
                           "11223344555", "99999999", "999999999", "9",
                           "99", "000022", "888889", "889",
                           "88888844", "77777777777777", "1111111", "1111122222","33324424111","333244241115"};

    std::list<job *> lp;

    auto theStrings = vector<string> {"one","two","test","three","four","test","testMe","Five","test"};

    lp.push_back(new programmer("testing Engineer", "Develop testing software", 40));
    lp.push_back(new pilot("Lead Pilot", "Fly 777 jet", 60));
    lp.push_back( new programmer("Hardware Engineer", "Develop Hardware", 40));
    lp.push_back(new pilot("Co Pilot", "Fly 737 jet", 50));
    lp.push_back( new programmer("Java Software Engineer", "Develop Java Scripts", 40));
    lp.push_back(new pilot("Fighter Pilot", "F22 Pilot", 50));

    for (list<job *>::iterator it=lp.begin(); it != lp.end(); ++it)
    {
        auto tu = *it;
        cout << tu->getJobName()  << " " << tu->getJobDescription() << " " << tu->getJobHours() << std::endl;
        tu->doWork();
    }

    auto match_count = countMatchItemInVector(theStrings,"test");
    cout << "Match Count:" << match_count << std::endl;

    theStrings.clear();

    /* Free memory */
    for (list<job *>::iterator it=lp.begin(); it != lp.end(); ++it)
    {
        auto tu = *it;
        delete (tu);
        tu = nullptr;
    }

    lp.clear();

    func();
    func(1);
    func(1,2);
    func(1,2,3);
    func(1,2,3,4);
    func(1,2,3,4,5);
    func(1,2,3,4,5,6);
    func(1,2,3,4,5,6,7);

    funca();
    funca(1);
    funca(1,2);
    funca(1,2,3);
    funca(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20);

    uint32_t checkInbound = 505;

    cout << IsInBounds(checkInbound,500,599)  << std::endl;

    cout << "Calling Tiles Test"  << std::endl;
    for(int i=0; i < 18; i++)
    {
        completeHandPassCount += completeHand = countpairs(tiles[i]);
        printf("Tile%d %s, %s, %s \n", i+1,tiles[i],completeHand ==1 ? "Complete Hand is True":  "Complete Hand is False",strcmp(completeHand == 1 ? "True": "False",completeSet[i])==0 ? "Test Passed": "Test Failed");
    }

    if(completeHandPassCount ==8)
    {
        printf("All %d Tiles 8 Tiles Passed\n",completeHandPassCount);
    }
    else
    {
        printf("%d Tiles Failed out 9\n",9-completeHandPassCount);
    }

    cout << "Decoding Roma Numerials:" << std::endl;

    for (int i=0;
         i<sizeof(TestNumerals) / sizeof(TestNumerals[0]);
         i++)
    {
        printf("\t\"%s\"\t\t\t%i\n", TestNumerals[i], calculateNum(TestNumerals[i]) /* Your method here */);
    }


    return 0;
}

int countTripplePairs(int numCount)
{
    int tripplePair = 0;

    if((numCount % 3) ==0)
    {
        tripplePair+= numCount/3;
    }
    else
    {
        tripplePair+= (numCount - (numCount %3))/ 3;
    }

    return tripplePair;
}
int countOnePairs(int numCount, int *checkForThreePair)
{
    int onePairTotal = 0;

    *checkForThreePair = 0;
    if((numCount % 2) ==0)
    {
        if((numCount %3) ==0)
        {
            *checkForThreePair = 1;
        }
        else {
            onePairTotal += numCount / 2;
        }
    }
    else
    {
        *checkForThreePair = 1;
        if((numCount % 3) == 2)
        {
            onePairTotal++;
        }
    }
    return onePairTotal;

}

int  checkforLeftOvers(int numCounts[])
{
    int numSingleDigit = 0;

    for(int i=0; i<10 ;i++)
    {
        if (numCounts[i] == 1)
            numSingleDigit = 1;
    }
    return numSingleDigit;

}

int checkForMoreThanOnePair(int twoPairsTotal[])
{
    int moreThanOnePairs = 0;

    for(int i=0;i<10;i++)
    {
        if(twoPairsTotal[i] != 0)
        {
            moreThanOnePairs++;
        }
    }

    return moreThanOnePairs;
}

int countpairs(const char*data)
{
    int completeHand=0;
    int morethanonepaird =0;
    int onepair = 0;
    int len  = strlen(data);
    int i;
    int leftover;
    int checkForThreePair = 0;
    int twoPairTotal[10];
    int threePairTotal[10];
    int oneThanOnePairsInDifferentLocs;

    memset(datacount, 0, sizeof(datacount));
    memset(twoPairTotal, 0, sizeof(twoPairTotal));
    memset(threePairTotal, 0, sizeof(threePairTotal));

    for(i=0;i<len;i++)
    {
        switch(data[i])
        {
            case '0':
                datacount[0]++;
                break;
            case '1':
                datacount[1]++;
                break;
            case '2':
                datacount[2]++;
                break;
            case '3':
                datacount[3]++;
                break;
            case '4':
                datacount[4]++;
                break;
            case '5':
                datacount[5]++;
                break;
            case '6':
                datacount[6]++;
                break;
            case '7':
                datacount[7]++;
                break;
            case '8':
                datacount[8]++;
                break;
            case '9':
                datacount[9]++;
                break;
        }

    }

    leftover = checkforLeftOvers(datacount);

    if (leftover ==0 )
    {
        for (i = 0; i < 10; i++) {

            onepair += twoPairTotal[i] = countOnePairs(datacount[i], &checkForThreePair);

            if (checkForThreePair == 1) {
                morethanonepaird += threePairTotal[i] = countTripplePairs(datacount[i]);
            } else {
                threePairTotal[i] = 0;
            }

        }

        oneThanOnePairsInDifferentLocs = checkForMoreThanOnePair(twoPairTotal);

        //printf("One pair counts :%d Three pairs :%d \n",onepair,morethanonepaird);
        if ((onepair > 1) && (morethanonepaird == 0) && (oneThanOnePairsInDifferentLocs == 1)) {
            completeHand = 1;
        }
        if ((onepair == 1) && (morethanonepaird > 0))
            completeHand = 1;
        else if ((onepair == 1) && (morethanonepaird == 0))
            completeHand = 1;
    }

    return completeHand;
}

int romanValue(char r)
{
    int rValue=0;

    switch(r)
    {

        case 'I':
            rValue = 1;
            break;
        case 'V':
            rValue = 5;
            break;
        case 'X':
            rValue = 10;
            break;
        case 'L':
            rValue = 50;
            break;
        case 'C':
            rValue = 100;
            break;
        case 'D':
            rValue = 500;
            break;
        case 'M':
            rValue = 1000;
            break;
    }

    return rValue;
}

int calculateNum(const char* data)
{
    int len = strlen(data);
    int number =0;
    int i;
    char previousRomanValue;
    int prevNum;

    previousRomanValue = (data[0]);

    number = romanValue(data[0]);
    prevNum  = romanValue(data[0]);

    for(i=1;i < len; i++)
    {
        switch(data[i])
        {
            case 'I':
                number+=1;
                previousRomanValue = data[i];
                break;
            case 'V':
                if (previousRomanValue == 'I')
                    number =5-prevNum;
                else {
                    number+=5;
                }
                previousRomanValue = data[i];
                break;
            case 'X':
                if ((previousRomanValue == 'V') || (previousRomanValue == 'I'))
                    number = 10-prevNum;
                else {
                    number+=10;
                }
                previousRomanValue = data[i];
                prevNum  = romanValue(data[i]);
                break;
            case 'L':
                if ((previousRomanValue == 'X') || (previousRomanValue == 'V') || (previousRomanValue == 'I'))
                    number = 50 -prevNum;
                else {
                    number+=50;
                }
                previousRomanValue = data[i];
                prevNum  = romanValue(data[i]);
                break;
            case 'C':
                if ((previousRomanValue == 'L')  || (previousRomanValue == 'X')|| (previousRomanValue == 'V') || (previousRomanValue == 'I'))
                    number = 100-prevNum;
                else {
                    number+=100;
                }
                previousRomanValue = data[i];
                prevNum  = romanValue(data[i]);
                break;
            case 'D':
                if ((previousRomanValue == 'C')  || (previousRomanValue == 'L') || (previousRomanValue == 'X')|| (previousRomanValue == 'V') || (previousRomanValue == 'I'))
                    number = 500-prevNum;
                else {
                    number+=500;
                }
                previousRomanValue = data[i];
                prevNum  = romanValue(data[i]);
                break;

            case 'M':
                if ((previousRomanValue == 'D') || (previousRomanValue == 'C') || (previousRomanValue == 'L') || (previousRomanValue == 'X')|| (previousRomanValue == 'V') || (previousRomanValue == 'I'))
                    number = 1000-prevNum;
                else {
                    number+=1000;
                }
                previousRomanValue = data[i];
                prevNum  = romanValue(data[i]);
                break;
        }
    }

    return number;
}