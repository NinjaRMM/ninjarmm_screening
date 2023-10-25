#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

#define IS_TRUE_TEST(d,x,y) { int a; if ( (a=(x)) != y ) os << __FUNCTION__ << " failed. Result should be "<< y << " but got "<<a << std::endl;else os<<d<< " test passed" << std::endl; }

/*items a,b,c,d*/
class Job{
    private:        
        std::string employee_name;
        std::string description;                  

    protected:        
        double value_hour;    
        int worked_hours;   

    public:
        Job(std::string _description, std::string _name, double _value_hour): employee_name(_name),
                                                                value_hour(_value_hour),
                                                                description(_description),
                                                                worked_hours(0){};        
        virtual ~Job(){};                
        virtual int get_total_work_hours()=0;
        virtual double get_wage(){ cout << " os base " <<endl; return 0; };
        std::string get_employee_name() { return this->employee_name; } ;
        std::string get_description() { return this->description; } ;        
        int add_worked_hours( int hours) { return (worked_hours +=hours); };
};


class Programmer:public Job
{    
    public:     
        Programmer( std::string _name, double _value_hour):
                    Job("Software Developer", _name, _value_hour){}
        ~Programmer(){};
        int get_total_work_hours(){ return worked_hours; };
        double get_wage(){  cout << "worked hours " << worked_hours << " " << value_hour << endl;
            double a = (worked_hours*2*value_hour);
            cout << " " << a << endl;
                         return a; 
             } ;      
};

class Pilot:public Job
{
    public:
        Pilot(  std::string _name, double _value_hour):
                Job("Airplane Pilot",_name, _value_hour){}
        ~Pilot(){};

        int get_total_work_hours(){ return worked_hours*2;};  // just for example, lets assume different calculation for wage and hours for programmer and pilot
        double get_wage() { cout << "worked hours " << worked_hours << " " << value_hour << endl;
            double a = ( worked_hours*2*value_hour*1.3);
                         return a; };
};

/*items e,f,g*/
/*Create job objects and allocate them into vector*/
void allocate_jobs()
{
    std::vector< std::shared_ptr<Job> > jobs;  

    auto a = make_shared<Programmer>("Fabiane Watanabe", 12.34);
    auto b= make_shared<Pilot>("Superman" ,45.67);

    a->add_worked_hours(10);
    b->add_worked_hours(10);
    b->add_worked_hours(10);
    
    jobs.push_back( a);
    jobs.push_back( b);

    for( auto it: jobs){
        
        printf("Name: %s\nDescription: %s\nTotal Worked Hours: %d\nTotal Wage: %f\n\n",
                it->get_employee_name().c_str(),
                it->get_description().c_str(), 
                it->get_total_work_hours(),
                it->get_wage() );  
    }
}

/*items h*/
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

/* Test 'IsInBounds' function.*/
void test_range(std::ostream& os = std::cout){
   os << "Testing range function.."<< std::endl; 
   auto test = []
               (std::ostringstream& os,std::string desc, std::stringstream &in, std::string exp)
                { if ( in.str().compare(exp) ==0 ) 
                    os <<"Test " <<desc << " passed"<< std::endl; 
                  else os <<"Test " <<desc << " failed"<< std::endl;
                };

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

/*item i*/
/* Check if string 'input' is equal to string defined */
int  ContainsTheString(auto a, vector<string> &input_strings)  {
    int count =0;    

    for( auto &it: input_strings ) {
        if( a(it) == true ) {            
            count++;
        } 
    }
    
    return count;
}

/* Test ContainsTheString function.*/
void test_count(std::ostream& os = std::cout)
{ 
    auto count = [](const std::string& tested) { return tested == "test"; };

    cout << endl << "Testing match string function.."<< std::endl; 

    auto theStrings = std::vector<std::string> {};
    IS_TRUE_TEST( "Empty vector", ContainsTheString( count, theStrings) ,0 );
    
    theStrings ={ "one", "two"};    
    IS_TRUE_TEST( "No value", ContainsTheString( count, theStrings) ,0 );

    theStrings.push_back("test");        
    IS_TRUE_TEST( "One value", ContainsTheString( count, theStrings) ,1 );
}

/*item j*/
/*Just sum up variadic arguments*/
template<typename... Args>
int execute(Args... args)
{    
    return (args+...);
}

void teste_j(std::ostream& os = std::cout){   
    
    cout << endl << "Testing variadic template .."<< std::endl; 
    IS_TRUE_TEST("No value", execute(0),0); 
    IS_TRUE_TEST("Positive value", execute(1,2,3),6);   
}

int main(){  
    
    cout << "Init Step1" << endl;
    allocate_jobs();
    
    test_range();
    test_count();
    teste_j();

    return 0;
}