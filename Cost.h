#ifndef COST_H_
#define COST_H_
#include <list>
#include <set>
#include <cmath>
#include <array>
#include <utility>
#include "Parsue_Conf.h"

/*
class Cost{
  private:
    uint16_t forward_cost[8];
  public:
    Cost(uint32_t max_forward_velocity, uint32_t max_turn_velocity, uint32_t accel);
};
*/
constexpr float max_forward_velocity = 3500;
constexpr float accel = 13;
constexpr float max_turn_velocity = 900;


template<typename Real>
constexpr Real factorial_impl(std::uint32_t i) noexcept{
  //return i==0 ? 1 : static_cast<Real>(i) * factorial_impl<Real>(i-1);
}

template<typename Real>
constexpr Real cost_solve_impl(const std::uint32_t &i, const bool &diagonal) noexcept{
  float field_length = MAZE_1BLOCK_LENGTH * (i + 1);
  if(diagonal == true) field_length /= sqrt(2.0);
  float offset_time = (max_forward_velocity - max_turn_velocity) / accel * 2;//UP and DOWN
  float offset_length = (max_forward_velocity + max_turn_velocity) / 2.0 * offset_time;
  if(field_length > offset_length)  return ((field_length - offset_length) / max_forward_velocity + offset_time) * 1000;
  else{
    field_length /= 2.0;
    float v2 = max_turn_velocity * max_turn_velocity + 2 * accel * field_length;
    return (sqrt(v2) - max_turn_velocity) / accel * 1000 * 2.0;
  }
}

template<typename Real, typename Integer, Integer ... vals>
constexpr std::array<Real, sizeof...(vals)> generate_array(const std::integer_sequence<Integer, vals...>&, const bool &diagonal){
  //return {{factorial_impl<Real>(vals)...}};
  return {{cost_solve_impl<Real>(vals, diagonal)...}};
}

template<typename Real, std::size_t N>
struct CostTable{
  constexpr static std::size_t size = N;
  constexpr static std::array<Real, N> straight_cost = generate_array<Real>(std::make_integer_sequence<Real, N>{}, false);
  constexpr static std::array<Real, 2 * N> diagonal_cost = generate_array<Real>(std::make_integer_sequence<Real, 2 * N>{}, true);
  constexpr static Real get(const std::size_t &i, const bool &diagonal) noexcept{
    //C++11ではstd::array::operator[]() constはconstexprでない.
    //C++14からstd::array::operator[]() constがconstexprに,
    //C++17からstd::arrayのメンバのほとんどがconstexprになる.
    return diagonal == true ? CostTable<Real, N>::diagonal_cost[i] : CostTable<Real, N>::straight_cost[i];
  }
  constexpr static Real diff_get(const std::size_t &i, const bool &diagonal) noexcept{
    if(i == 0)  return diagonal == true ? CostTable<Real, N>::diagonal_cost[i] : CostTable<Real, N>::straight_cost[i];
    else  return diagonal == true ? CostTable<Real, N>::diagonal_cost[i] - CostTable<Real, N>::diagonal_cost[i-1]: CostTable<Real, N>::straight_cost[i] - CostTable<Real, N>::straight_cost[i-1];
    /*
    if(i == 0) return diagonal == true ? CostTable<Real, N>::diagonal_cost[i] : CostTable<Real, N>::straight_cost[i];

    else{
      return diagonal == true ? CostTable<Real, N>::diagonal_cost[i]  - CostTable<Real, N>::diagonal_cost[i - 1] : CostTable<Real, N>::straight_cost[i] - 
        CostTable<Real, N>::straight_cost[i - 1];
    }
    */
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
  return CostTable<Real, N>::get(i);
}
extern CostTable<uint16_t, MAZE_SIZE> cost_table;
#endif
