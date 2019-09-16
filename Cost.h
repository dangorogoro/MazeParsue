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

/*
uint16_t cost_solve_impl(const size_t &i, const Direction &dir){
  float field_length = MAZE_1BLOCK_LENGTH * i;
  if(bit_count(dir) == 2) field_length /= sqrt(2.0);
  float offset_time = (max_forward_velocity - max_turn_velocity) / accel;
  float offset_length = (max_forward_velocity + max_turn_velocity) / 2.0 * offset_time;
  if(field_length > offset_length)  return ((field_length - offset_length) / max_forward_velocity + offset_time) * 1000;
  else{
    float v2 = max_turn_velocity * max_turn_velocity + 2 * accel * field_length;
    return (sqrt(v2) - max_turn_velocity) / accel * 1000;
  }
}
*/

template<typename Real>
constexpr Real factorial_impl(std::uint32_t i) noexcept
{
      return i==0 ? 1 : static_cast<Real>(i) * factorial_impl<Real>(i-1);
}

template<typename Real, typename Integer, Integer ... vals>
constexpr std::array<Real, sizeof...(vals)>
generate_array(std::integer_sequence<Integer, vals...>)
{
      return {{factorial_impl<Real>(vals)...}};
}

template<typename Real, std::size_t N>
struct factorial_table
{
  constexpr static std::size_t size = N;
  //constexpr static std::array<Real, N> value = generate_array<Real>(std::make_integer_sequence<std::uint32_t, N>{});
  constexpr static std::array<Real, N> value = generate_array<Real>(std::make_integer_sequence<Real, N>{});
  constexpr static Real get(std::size_t i) noexcept{
    //C++11ではstd::array::operator[]() constはconstexprではない。
    //C++14からstd::array::operator[]() constがconstexprに、
    //C++17からstd::arrayのメンバの殆どがconstexprになる。
    return factorial_table<Real, N>::value[i];
  }
};

//template<typename Real, std::size_t N>
//constexpr std::size_t factorial_table<Real, N>::size;
//template<typename Real, std::size_t N>
//constexpr Real factorial_table<Real, N>::value[N];
template<typename Real, std::size_t N>
constexpr static Real factorial(std::size_t i) noexcept
{
      return factorial_table<Real, N>::get(i);
}
/*
template<size_t N>
struct costTable{
  constexpr std::size_t size = N;
  constexpr std::array<uint16_t, N> straight_value;
  constexpr std::array<uint16_t, 2 * N> diagonal_value;

  constexpr costTable() : straight_value(), diagonal_value() {
    for(int i = 0; i < 2 * N; i++){
      if(N > i) straight_value[i] = cost_solve_impl(i, NORTH);
      diagonal_value[i] = cost_solve_impl(i, NORTH | WEST);
    }
  }
  constexpr static uint16_t get(const std::size_t &i, const Direction &dir) noexcept{
    //C++11ではstd::array::operator[]() constはconstexprではない。
    //C++14からstd::array::operator[]() constがconstexprに、
    //C++17からstd::arrayのメンバの殆どがconstexprになる。
    if(bit_count(dir) == 1) return costTable::straight_value[i];
    else if(bit_count(dir) == 2)  return costTable::diagonal_value[i];
  }
};
auto cost_table = costTable<MAZE_SIZE>();

constexpr uint16_t cost_solve(const size_t &i, const Direction &dir) noexcept{
  return costTable::get(i, dir);
}
template<std::size_t N>
constexpr std::size_t cost_table<uint16_t, N>::size;
template<std::size_t N>
constexpr uint16_t cost_table<Real, N>::straight_value[N];
template<std::size_t N>
constexpr uint16_t cost_table<Real, N>::diagonal_value[N];


template<typename Real, typename Integer, Integer ... vals>
std::array<Real, sizeof...(vals)>
generate_array(std::integer_sequence<Integer, vals...>){
      return {{factorial_impl(vals)...}};
}
constexpr auto a = generate_array<double>(
        std::make_integer_sequence<std::uint32_t, 100>{});
*/
#endif
