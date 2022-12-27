#include "job.hpp"
#include <algorithm>
#include <sstream>

#define IS_TRUE_TEST_COUNT(d,x,y) { int a; if ( (a=(x)) != y ) os << __FUNCTION__ << " failed. Result should be "<< y << " but got "<<a << std::endl;else os<<d<< " test passed" << std::endl; }

/* a Creating Job class base */
Job::Job( int _hours, 
          std::string _name,
          std::string _description):
          hours_required(_hours),
          name(_name),
          description(_description)
{    
}

/* da */
std::string Job::get_name(){
    return this->name;
}
std::string Job::get_description(){
    return this->description;
}
int Job::get_job_hours(){
    return this->hours_required;
}
std::string Job::DoWork(){
    std::string work= "My work involves " + this->description;
    return work;
}

/*Factory for programmer, pilot object creation*/
std::shared_ptr<Job> FactoryJob::create(Job_types type, int hours, std::string name, std::string description){
    switch (type)
    {
        
        case PROGRAMMER:
            return std::shared_ptr<Programmer>(new Programmer(hours,name,description));
        break;

        case PILOT:
            return std::shared_ptr<Pilot>(new Pilot(hours,name,description));
        break;      
    
        default:
            return std::shared_ptr<Job>(new Job(hours,name,description));
        break;     
    }
}
/* e,f,g */
/*Create job objects and allocate them into vector*/
void allocate_jobs()
{
    std::vector<std::shared_ptr<Job> > jobs;    
    FactoryJob creator;

    jobs.push_back(creator.create(FactoryJob::PROGRAMMER, 10,"Programmer", "Senior Software Developer"));
    jobs.push_back(creator.create(FactoryJob::PILOT, 11,"Pilot", "Boeing Pilot"));

    for( auto it: jobs){
        printf("Name: %s\nDescription: %s\nHours Required: %d\nDoWork: %s\n\n",
                it->get_name().c_str(),
                it->get_description().c_str(),
                it->get_job_hours(),
                it->DoWork().c_str() );

    }
}

/* h */\
/*Check if value 'code' is within min and max range*/
template<typename T>
void IsInBounds(T code, T min, T max)
{
    if( min <= code) {
        std::cout <<"The input value ["<< code << "] is lower than mininum range. Range ["<< min << "]["<< max<<"]" << std::endl;
    } else if(  code >= max) {
        std::cout <<"The input value ["<< code << "] is higher than maximum range. Range ["<< min << "]["<< max<<"]" << std::endl;
    } else{
        std::cout << "The input value ["<< code << "] is within range ["<< min << "]["<< max<<"]" << std::endl;
    }    
}

/* Check if string 'input' is equal to string 'toTest' */
bool ContainsTheString( const std::string &input, const std::string &toTest) {
    if( toTest == input){
        return true;
    }else{
        return false;
    }
}
/* Count how many strings equal to 'toTest' there are in vector of string 'theStrings.
The 'func' parameter is a function pointer which compare 2 strings, returning true if
they are equal.*/
int count_match( std::vector<std::string> &theStrings , bool (*func)(const std::string&, const std::string&) , const std::string &toTest)
{    
    auto count= std::count_if(theStrings.begin(), theStrings.end(), [func, toTest](std::string &s) {  return (*func)(s,toTest ); }  );
    return count;
}
/* Unit test that test 'IsInBounds' function.*/
void test_range(std::ostream& os = std::cout){
   os << "Testing range function.."<< std::endl; 
   auto test = [](std::ostringstream& os,std::string desc, std::stringstream &in, std::string exp){ if ( in.str().compare(exp) ==0 ) os <<"Test " <<desc << " passed"<< std::endl; else os <<"Test " <<desc << " failed"<< std::endl;};

    std::stringstream buffer;    
    std::ostringstream outresult;
    std::streambuf* prevcoutbuf = std::cout.rdbuf( buffer.rdbuf() );
       
    IsInBounds<int>(1,2,3);
    test(outresult,"integer",buffer, "The input value [1] is within range [2][3]\n"); 

    buffer.str("");
  
    IsInBounds<char>('e','a','b');
    test(outresult,"char",buffer, "The input value [e] is lower than mininum range. Range [a][b]\n"); 

    std::cout.rdbuf(prevcoutbuf);

    os << outresult.str();
}

/* Unit test that test 'count_match' function.*/
void test_count(std::ostream& os = std::cout)
{ 
    os << "Testing match string function.."<< std::endl; 
    auto theStrings = std::vector<std::string> {};
    IS_TRUE_TEST_COUNT( "Empty vector", count_match( theStrings, ContainsTheString, ""),0 );
    theStrings ={ "one", "two"};
    IS_TRUE_TEST_COUNT( "No value", count_match( theStrings, ContainsTheString, "test"),0 );
    theStrings.push_back("test");    
    IS_TRUE_TEST_COUNT( "One value", count_match( theStrings, ContainsTheString, "test"),1 );  
}


int main(){
  
   test_range();
   test_count();

   allocate_jobs();
  
    return 0;
}