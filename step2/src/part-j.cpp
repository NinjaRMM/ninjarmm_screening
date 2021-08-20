/*
This is an example of a hierarchy generation pattern adapted to modern C++ as described
by Andre Alexandrescu in his book Modern C++ Design. This pattern permits composing various discrete behavior
policies into a rich concrete types with maximum reuse.
*/

#include <iostream>
#include <cstddef>

/*
This is the generic form of the hierarchy generator. It accepts a variable number of 'policy' class templates.
The 'policy' class templates are discrete functionality which maybe injected into any number of concrete 
implementations similar to dependency injection. The resulting concrete class is a composition of the various
'policies' with the advantage of being a linear class hierarchy. This avoids multiple inheritance or virtual 
inhertance which can cause the classic 'diamond' inhertance problem.
*/
template <template <typename> typename ...> struct hierarchy_gen;

/*
this is the specialized form which accepts zero policies. It acts as a terminator during compile time
construction of the class hierarchy
*/
template <> struct hierarchy_gen<>{};

/*
This is the partially specialized form of the hierarchy generator which iteratively inherits and constructs
a class hierarchy based on the passed in 'policy' classes
*/
template <template <typename> typename _head_t, template <typename> typename ... _tail_ts> 
struct hierarchy_gen<_head_t, _tail_ts...> : _head_t<hierarchy_gen<_tail_ts...>>{};

/*
These are various discrete behavior policies which can be mixed to compose rich concrete implementations.
First are some color policies
*/
template <typename _super_t> struct red_policy : _super_t {
  void color() { std::cout << "red\n"; }
};
template <typename _super_t> struct green_policy : _super_t {
  void color() { std::cout << "green\n"; }
};
template <typename _super_t> struct blue_policy : _super_t {
  void color() { std::cout << "blue\n"; }
};

/*
These are some sound policies
*/
template <typename _super_t> struct bing_policy : _super_t {
  void sound() { std::cout << "bing\n"; }
};
template <typename _super_t> struct ring_policy : _super_t {
  void sound() { std::cout << "ring\n"; }
};
template <typename _super_t> struct boom_policy : _super_t {
  void sound() { std::cout << "boom\n"; }
};

/*
* Here are some shape policies
*/
template <typename _super_t> struct square_policy : _super_t {
  void shape() { std::cout << "square\n"; }
};
template <typename _super_t> struct circle_policy : _super_t {
  void shape() { std::cout << "circle\n"; }
};
template <typename _super_t> struct triangle_policy : _super_t {
  void shape() { std::cout << "triangle\n"; }
};

/*
Here the polcies are composed using the hierarchy generator to declare concrete types.
Notice there is no multiple inheritance in the class hierarchy, its linear so avoids the diamond problem
*/

using t1 = hierarchy_gen<red_policy, bing_policy, square_policy>;
using t2 = hierarchy_gen<red_policy, boom_policy, circle_policy>;
using t3 = hierarchy_gen<red_policy, bing_policy, triangle_policy>;
using t4 = hierarchy_gen<green_policy, bing_policy, square_policy>;
using t5 = hierarchy_gen<green_policy, boom_policy, circle_policy>;
using t6 = hierarchy_gen<green_policy, bing_policy, triangle_policy>;
using t7 = hierarchy_gen<red_policy, bing_policy, square_policy>;
using t8 = hierarchy_gen<red_policy, ring_policy, circle_policy>;
using t9 = hierarchy_gen<red_policy, bing_policy, triangle_policy>;

template <typename _ty> void ExerciseType() {
  _ty oType;
  oType.color();
  oType.sound();
  oType.shape();
}

int main() {
  ExerciseType<t1>();
  ExerciseType<t2>();
  ExerciseType<t3>();
  ExerciseType<t4>();
  ExerciseType<t5>();
  ExerciseType<t6>();
  ExerciseType<t7>();
  ExerciseType<t8>();
  ExerciseType<t9>();
}