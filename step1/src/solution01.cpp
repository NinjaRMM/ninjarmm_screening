//////////////////////////////////////////////////////////////////////
// This code is an implementation for the ninjarmm screening.
//
// How to compile:
//   g++ -std=c++11 solution01.cpp -o solution01
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <stdint.h>
#include <stdarg.h>
#include <chrono>
#include <thread>

//////////////////////////////////////////////////////////////////////
// Constatnt definitions
//////////////////////////////////////////////////////////////////////
const char ESC = 0x1b;

using namespace std;

//////////////////////////////////////////////////////////////////////
// Job interface definition
//////////////////////////////////////////////////////////////////////
class Job {
public:
  virtual string GetName() = 0; 
  virtual string GetDescription() = 0;
  virtual unsigned int GetHoursRequired() = 0;
  virtual void DoWork() = 0;
};

//////////////////////////////////////////////////////////////////////
// Programmer Class Implementation
//////////////////////////////////////////////////////////////////////
class Programmer : public Job {
public:
  string GetName() override {
    return "Programmer";
  }

  string GetDescription() override {
    return "writing computer programs.";
  }

  unsigned int GetHoursRequired() override {
    return 8;
  }

  void DoWork() override {
    cout << "My work involves " + GetDescription() << endl;
  }
};

//////////////////////////////////////////////////////////////////////
// Pilot Class Implementation
//////////////////////////////////////////////////////////////////////
class Pilot : public Job { 
public:
  string GetName() override {
    return "Pilot";
  }

  string GetDescription() override {
    return "flying an airplane.";
  }

  unsigned int GetHoursRequired() override {
    return 11;
  }

  void DoWork() override {
    cout << "My work involves " + GetDescription() << endl;
  }
};

//////////////////////////////////////////////////////////////////////
// Template function to test if a provided value are within a range 
//////////////////////////////////////////////////////////////////////
template <typename T>
bool IsInBounds(T value_to_test, T min_range, T max_range) {
  bool result = false;
  cout << "Function IsInBounds activated ..." << endl;
  cout << "  - Parameter supplied: " << value_to_test << endl;
  cout << "  - Min value accepted: " << min_range << endl;
  cout << "  - Max value accepted: " << max_range << endl;

  if( value_to_test >= min_range && value_to_test <= max_range ) {
    result = true;
  }

  cout << "  - Test Result.......: ";
  if( result == true) {
    cout << "[Accepted]";
  } else {
    cout << "[ERROR]";
  }
  cout << endl;

  return result;
};

//////////////////////////////////////////////////////////////////////
// Function to check how many items matched the collection provided on a vector
//////////////////////////////////////////////////////////////////////
int ContainsTheString( bool (*test_function)(const string&), const vector<string> &vector_parameter ) {
  int counter = 0;

  for( const auto &item : vector_parameter) {
    if( test_function(item) ) {
      counter++;
    }
  }

  return counter;
}

//////////////////////////////////////////////////////////////////////
// Freestyle coding
//////////////////////////////////////////////////////////////////////
class Screen {
public:
  enum Color { 
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    GRAY = 90,
    LIGHT_RED,
    LIGHT_GREEN,
    LIGHT_YELLOW,
    LIGHT_BLUE,
    LIGHT_MAGENTA,
    LIGHT_CYAN,
    LIGHT_WHITE
  };
  enum Background_Color {
    BG_BLACK = 40,
    BG_RED,
    BG_GREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_WHITE,
    BG_GRAY = 100,
    BG_LIGHT_RED,
    BG_LIGHT_GREEN,
    BG_LIGHT_YELLOW,
    BG_LIGHT_BLUE,
    BG_LIGHT_MAGENTA,
    BG_LIGHT_CYAN,
    BG_LIGHT_WHITE
  };

private:
  // Class attributes
  static int m_brush_color;
  static int m_background_color;
  static unsigned int m_window_width;
  static unsigned int m_window_height;
  static unsigned int m_window_internal_columns;
  static unsigned int m_window_internal_rows;

public:
  static void set_color(Screen::Color color) {
    m_brush_color = color;
    cout << ESC << "[" << m_brush_color << ";" << m_background_color << "m";
  }

  static void set_background_color(Screen::Background_Color bg_color) {
    m_background_color = bg_color;
  }

  static void reset_colors() {
    cout << ESC << "[0m";
  }

  static void clear_screen() {
    cout << ESC << "[2J";
  }

  static void draw_text(string text) {
    cout << ESC << "[" << m_brush_color << ";" << m_background_color << "m" << text;
  }

  static void move_to(int row, int column) {
    cout << ESC << "[" << row << ";" << column << "f";
  }

  static void repeat_char(char ch, int number_of_chars) {
    for(int i=0 ; i < number_of_chars ; i++) {
      cout << ch;
    }
  }

  static void create_window(int top_row, int top_column, int bottom_row, int bottom_column, Screen::Color border_color, Screen::Background_Color bg_color) {
    int window_width = bottom_column - top_column; 
    int window_height = bottom_row - top_row;

    set_background_color(bg_color);
    
    // Draws top line
    move_to(top_row, top_column);
    set_color(border_color);
    cout << "+";
    repeat_char( '-', window_width - 1 );
    cout << "+";

    // Draws side bars
    for(int i=1 ; i < window_height ; i++) {
      move_to(top_row+i, top_column);
      cout << "|";
      repeat_char(' ', window_width - 1 );
      cout << "|";
    }

    // Draws bottom line
    move_to(bottom_row, top_column);
    cout << "+";
    repeat_char( '-', window_width - 1 );
    cout << "+";
    cout.flush();
  }

  static void create_animated_window(int top_row, int top_column, int bottom_row, int bottom_column, Screen::Color border_color, Screen::Background_Color bg_color) {
    m_window_width = bottom_column - top_column + 1;
    m_window_height = bottom_row - top_row + 1;
    m_window_internal_rows = bottom_row - top_row - 1;
    m_window_internal_columns = bottom_column - top_column - 1;

    int window_center_row = (bottom_row + top_row) / 2;
    int window_center_col = (bottom_column + top_column) / 2;

    int steps_to_increment = (window_center_row - top_row );
   
    float col_distance_to_increment = (window_center_col - top_column) / (float) (steps_to_increment );
    int j=0;

    for( int i = 1 ; i <= steps_to_increment ; i++ ) {
      j = (float) i * (float) col_distance_to_increment;
      create_window( window_center_row-i, window_center_col-j, window_center_row+i , window_center_col+j, border_color, bg_color);
      this_thread::sleep_for(chrono::milliseconds(50));
    }
  }

  static unsigned int get_window_width() {
    return m_window_width;
  }
  static unsigned int get_window_height() {
    return m_window_height;
  }
  static unsigned int get_window_internal_rows() {
    return m_window_internal_rows;
  }
  static unsigned int get_window_internal_cols() {
    return m_window_internal_columns;
  }

};
int Screen::m_brush_color = 0;
int Screen::m_background_color = 40;
unsigned int Screen::m_window_width = 0;
unsigned int Screen::m_window_height= 0;
unsigned int Screen::m_window_internal_rows = 0;
unsigned int Screen::m_window_internal_columns = 0;

//////////////////////////////////////////////////////////////////////
// This routine sums the arguments using variadic functions
//////////////////////////////////////////////////////////////////////
int sum(int number_of_parameters, ...) {
  int total = 0;
  va_list ptr;
  va_start(ptr, number_of_parameters);

  for(int i=0 ; i < number_of_parameters ; i++){
    total += va_arg(ptr, int);
  }

  va_end(ptr);

  return total;
}

//////////////////////////////////////////////////////////////////////
// Template function to test if a expected value is equal to a processing result
//////////////////////////////////////////////////////////////////////
template <typename T>
bool is_equal(T value_to_test, T expected_value) {
  bool result = false;
  Screen scr;

  cout << "Testing if [" << value_to_test << "] is equal to the expected value [" << expected_value << "] ... ";
  cout << "[";
  if( value_to_test == expected_value ) {
    result = true;

    scr.set_color(Screen::GREEN);
    cout << "SUCCESS";
  } else {
    scr.set_color(Screen::RED);
    cout << "ERROR";
  }

  scr.reset_colors();
  cout << "]." << endl;

  return result;
}

//////////////////////////////////////////////////////////////////////
// Program Entry Point
//////////////////////////////////////////////////////////////////////
int main() {
  list<Job *> job_list;
  list<Job *>::iterator job_list_iterator;
  uint32_t value_to_check=0;

  cout << "- Creating objects ..." << endl;
  Job *programmer = new Programmer();
  Job *pilot = new Pilot();

  // Adds the Programmer object to the list
  cout << "- Adding the Programmer object to the container ..." << endl;
  job_list.push_back(programmer);

  // Adds the Pilot object to the list
  cout << "- Adding the Pilot object to the container ..." << endl;
  job_list.push_back(pilot);


  // Iterate throught the objects
  cout << endl << "- Iterating over the list and Calling objects interface functions:" << endl;
  cout << "==================================================================" << endl;
  for(Job *element : job_list) {
    cout << "Calling GetName().........: " << element->GetName() << endl;
    cout << "Calling GetDescription()..: " << element->GetDescription() << endl;
    cout << "Calling GetHoursRequired(): " << element->GetHoursRequired() << endl;
    cout << "Calling DoWork()..........: ";
    element->DoWork();
    cout << endl;
  }

  // Test if the Working hours is within a range
  cout << "Testing programmer object consistency" << endl;
  value_to_check = programmer->GetHoursRequired();
  IsInBounds<uint32_t>( value_to_check, 1, 12 );

  cout << "Testing pilot object consistency" << endl;
  value_to_check = pilot->GetHoursRequired();
  IsInBounds<uint32_t>( value_to_check, 1, 12 );
  
  // Checks how many items in the collection matched the test
  auto theStrings  = vector<std::string> { "one", "two", "test" };
  auto count = ContainsTheString([](const std::string& tested) 
  { 
    return tested == "test";
  }, theStrings);
  cout << "Number of occurrences that matched the test: " << count << endl;

  // Freestyle coding
  cout << endl << "Variadic Demonstrration:" << endl;
  auto result = sum(2, 1,2);
  cout << "Variadic sum(2, 1,2)   = " << result << endl;
  result = sum(3, 1, 2, 3);
  cout << "Variadic sum(3, 1,2,3) = " << result << endl;

  // Performs the tests for thefuncion sum
  is_equal(sum(2, 1,2), 3);
  is_equal(sum(3, 1,2,3), 6);
  is_equal(sum(1, -5), -5);
  is_equal(sum(2, -10,-20), -30);

  cout << endl << "Press ENTER to start Freestyle demo." ;
  cin.get();
  Screen scr;
  scr.clear_screen();
  scr.set_color(Screen::YELLOW);
  scr.draw_text("Yellow Text\n");
  scr.set_color(Screen::WHITE);
  scr.draw_text("White Text\n");

  scr.move_to(1,1);
  scr.set_color(Screen::BLUE);
  scr.draw_text("This blue text is in the top corner\n");

  scr.move_to(2,1);
  scr.set_color(Screen::GREEN);
  scr.draw_text("This green text is below\n");

  //scr.create_window( 8,10, 22, 60, Screen::LIGHT_BLUE, Screen::BG_BLUE);
  scr.create_animated_window( 8,10, 24, 60, Screen::LIGHT_BLUE, Screen::BG_BLUE);
  scr.set_color(Screen::LIGHT_WHITE);
  scr.move_to(10,12);
  scr.draw_text("  This is a simple example of what you can do");
  scr.move_to(11,12);
  scr.draw_text("with a small class abstraction in C++.");
  scr.move_to(13,12);
  scr.draw_text("  At least, even on a text window being used");
  scr.move_to(14,12);
  scr.draw_text("for an interview test, we may have a different");
  scr.move_to(15,12);
  scr.draw_text("presentation than just raw text being displayed");
  scr.move_to(16,12);
  scr.draw_text("on screen.");

  scr.move_to(18,12);
  scr.draw_text("Window information:");
  scr.move_to(19,12);
  cout << "  - Window Width.: " << Screen::get_window_width() << " chars.";
  scr.move_to(20,12);
  cout << "  - Window Height: " << Screen::get_window_height() << " chars.";
  scr.move_to(21,12);
  cout << "  - Internal cols: " << Screen::get_window_internal_cols() << " chars.";
  scr.move_to(22,12);
  cout << "  - Internal rows: " << Screen::get_window_internal_rows() << " chars.";

  scr.create_animated_window( 4, 62, 14, 102, Screen::LIGHT_GREEN, Screen::BG_GREEN);
  scr.move_to(5,63);
  cout << " Other Window for further information.";


  scr.move_to(25,1);
  scr.reset_colors();
  

  // Release resources
  delete programmer;
  delete pilot;

  // Returns to the operating system
  return EXIT_SUCCESS;
}

