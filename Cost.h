#ifndef COST_H_
#define COST_H_
#include <list>
#include <set>
#include <cmath>
#include <array>
#include <utility>
#include "Parsue_Conf.h"
#include "Maze.h"
#include "Operation.h"

/*
class Cost{
  private:
    uint16_t forward_cost[8];
  public:
    Cost(uint32_t max_forward_velocity, uint32_t max_turn_velocity, uint32_t accel);
};
*/
constexpr float max_forward_velocity = 3;
constexpr float accel = 10;
constexpr float max_turn_velocity = 0.8;


template<typename Real>
constexpr Real factorial_impl(std::uint32_t i) noexcept{
  //return i==0 ? 1 : static_cast<Real>(i) * factorial_impl<Real>(i-1);
}

template<typename Real>
constexpr Real cost_solve_impl(const std::uint32_t &i, const bool &diagonal) noexcept{
  float field_length = MAZE_1BLOCK_LENGTH * i;
  if(diagonal == true) field_length /= sqrt(2.0);
  float offset_time = (max_forward_velocity - max_turn_velocity) / accel;
  float offset_length = (max_forward_velocity + max_turn_velocity) / 2.0 * offset_time;
  if(field_length > offset_length)  return ((field_length - offset_length) / max_forward_velocity + offset_time) * 1000;
  else{
    float v2 = max_turn_velocity * max_turn_velocity + 2 * accel * field_length;
    return (sqrt(v2) - max_turn_velocity) / accel * 1000;
  }
}

template<typename Real, typename Integer, Integer ... vals>
constexpr std::array<Real, sizeof...(vals)> generate_array(const std::integer_sequence<Integer, vals...>&, const bool &diagonal){
  //return {{factorial_impl<Real>(vals)...}};
  return {{cost_solve_impl<Real>(vals, diagonal)...}};
}

template<typename Real, std::size_t N>
struct cost_table{
  constexpr static std::size_t size = N;
  constexpr static std::array<Real, N> straight_cost = generate_array<Real>(std::make_integer_sequence<Real, N>{}, false);
  constexpr static std::array<Real, N> diagonal_cost = generate_array<Real>(std::make_integer_sequence<Real, N>{}, true);
  constexpr static Real get(const std::size_t &i, const bool &diagonal) noexcept{
    //C++11ではstd::array::operator[]() constはconstexprではない。
    //C++14からstd::array::operator[]() constがconstexprに、
    //C++17からstd::arrayのメンバの殆どがconstexprになる。
    return diagonal == true ? cost_table<Real, N>::diagonal_cost[i] : cost_table<Real, N>::straight_cost[i];
  }
};
/*
template<typename Real, std::size_t N>
struct factorial_table{
  constexpr static std::size_t size = N;
  constexpr static std::array<Real, N> value = generate_array<Real>(std::make_integer_sequence<Real, N>{});
  constexpr static Real get(std::size_t i) noexcept{
    //C++11ではstd::array::operator[]() constはconstexprではない。
    //C++14からstd::array::operator[]() constがconstexprに、
    //C++17からstd::arrayのメンバの殆どがconstexprになる。
    return factorial_table<Real, N>::value[i];
  }
};
*/

template<typename Real, std::size_t N>
constexpr static Real cost(std::size_t i) noexcept{
  return cost_table<Real, N>::get(i);
}
#endif
