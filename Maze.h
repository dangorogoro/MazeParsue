#ifndef MAZE_H_
#define MAZE_H_

#include <cstdint>
#include <bitset>
#include <cstddef>
#include <vector>
#include <queue>
#include "Parsue_Conf.h"

//std::bitset<((2 * MAZE_SIZE * MAZE_SIZE) - 2 * MAZE_SIZE) * 2> WallData;
constexpr uint32_t WALL_AMOUNT  = 2 * (2 * MAZE_SIZE * MAZE_SIZE);
/*
#define NORTH_STATE(x,y) (2 * ((MAZE_SIZE - 1) * x + y))
#define NORTH_VISIBLE(x,y) (2 * ((MAZE_SIZE - 1) * x + y) + 1)
#define EAST_STATE(x,y) (2 * ((MAZE_SIZE - 1) * MAZE_SIZE + x + (MAZE_SIZE - 1) + y))
#define EAST_VISIBLE(x,y) ((2 * ((MAZE_SIZE - 1) * MAZE_SIZE + x + (MAZE_SIZE - 1) + y) + 1))
#define SOUTH_STATE(x,y) (NORTH_STATE(x,y-1))
#define SOUTH_VISIBLE(x,y) (NORTH_VISIBLE(x,y-1))
#define WEST_STATE(x,y) (EAST_STATE(x-1,y))
#define WEST_VISIBLE(x,y) (EAST_VISIBLE(x-1,y))
*/

constexpr uint32_t NORTH_STATE(int8_t x, int8_t y){return 2 * (2 * (MAZE_SIZE * y + x));}
constexpr uint32_t NORTH_VISIBLE(int8_t x, int8_t y){return NORTH_STATE(x,y) + 1;}
constexpr uint32_t EAST_STATE(int8_t x, int8_t y){return NORTH_STATE(x,y) + 2;}
constexpr uint32_t EAST_VISIBLE(int8_t x, int8_t y){return EAST_STATE(x,y) + 1;}
constexpr uint32_t WEST_STATE(int8_t x, int8_t y){return EAST_STATE(x-1,y);}
constexpr uint32_t WEST_VISIBLE(int8_t x, int8_t y){return EAST_STATE(x-1,y) + 1;}
constexpr uint32_t SOUTH_STATE(int8_t x, int8_t y){return NORTH_STATE(x,y-1);}
constexpr uint32_t SOUTH_VISIBLE(int8_t x, int8_t y){return NORTH_STATE(x,y-1) + 1;}
#define EAST_STATE(x,y) (NORTH_STATE(x,y) + 2)
#define EAST_VISIBLE(x,y) (EAST_STATE(x,y) + 1)
#define SOUTH_STATE(x,y) (NORTH_STATE(x,y-1))
#define SOUTH_VISIBLE(x,y) (NORTH_VISIBLE(x,y-1))
#define WEST_STATE(x,y) (EAST_STATE(x-1,y))
#define WEST_VISIBLE(x,y) (EAST_VISIBLE(x-1,y))

template<uint32_t T>
using WallStr = std::bitset<T>;
extern WallStr<WALL_AMOUNT> WallData;

constexpr uint8_t EAST = 0x01;
constexpr uint8_t NORTH = 0x02;
constexpr uint8_t WEST = 0x04;
constexpr uint8_t SOUTH = 0x08;
constexpr uint8_t DONE_EAST = 0x10;
constexpr uint8_t DONE_NORTH = 0x20;
constexpr uint8_t DONE_WEST = 0x40;
constexpr uint8_t DONE_SOUTH = 0x80;
union __attribute__ ((__packed__)) Direction {
  public:
    uint8_t byte;
    struct __attribute__ ((__packed__)) {
      uint8_t East:1; 		//bit0
      uint8_t North:1; 		//bit1 LSB
      uint8_t West:1; 		//bit2
      uint8_t South:1; 		//bit3
      uint8_t DoneEast:1; 	//bit4
      uint8_t DoneNorth:1; 	//bit5
      uint8_t DoneWest:1; 	//bit6 MSB
      uint8_t DoneSouth:1; 	//bit7
    } bits;
  public:
    Direction(uint8_t value=0) : byte(value) {}

    //演算関連は全てuint8_tにキャストしてから行う
    inline operator uint8_t() const { return byte; }
    inline uint8_t operator|(uint8_t value) const { return byte | value; }
    inline uint8_t operator&(uint8_t value) const { return byte & value; }
    inline uint8_t operator=(uint8_t value) { return byte = value; }
    inline uint8_t operator|=(uint8_t value) { return byte |= value; }
    inline uint8_t operator&=(uint8_t value) { return byte &= value; }
    inline uint8_t operator=(Direction &obj) { return byte = obj.byte; }

    //index番目のビットが立っているかどうかをuint8_tの0x00か0x01で返す
    inline uint8_t operator[](uint8_t index) const {return (byte & (0x01<<index)) ? 1:0; }

    //全ての壁が探索済みかどうか
    inline bool isDoneAll() const { return (byte | 0x0f) == 0xff; }

    //壁の数を数える
    inline int nWall() const {
      uint8_t b = byte & 0x0f;
      b = (b & 0x55) + (b >> 1 & 0x55);
      b = (b & 0x33) + (b >> 2 & 0x33);
      b = (b & 0x0f) + (b >> 4 & 0x0f);
      return b;
    }

    //探索済みの壁を数える
    inline int nDoneWall() const {
      uint8_t b = (byte & 0xf0) >> 4;
      b = (b & 0x55) + (b >> 1 & 0x55);
      b = (b & 0x33) + (b >> 2 & 0x33);
      b = (b & 0x0f) + (b >> 4 & 0x0f);
      return b;
    }
};


struct __attribute__ ((__packed__)) IndexVec {
  int8_t x;
  int8_t y;

  IndexVec(int8_t _x=0, int8_t _y=0) : x(_x), y(_y) {}
  IndexVec(const IndexVec &obj) : x(obj.x), y(obj.y) {}

  //ベクトルとしての演算
  inline IndexVec operator+(const IndexVec &obj) const { return IndexVec(x+obj.x, y+obj.y); }
  inline IndexVec operator-(const IndexVec &obj) const { return IndexVec(x-obj.x, y-obj.y); }
  inline void operator+=(const IndexVec &obj) { x+=obj.x; y+=obj.y; }
  inline void operator-=(const IndexVec &obj) { x-=obj.x; y-=obj.y; }
  inline const IndexVec& operator=(const IndexVec &obj) { x=obj.x; y=obj.y; return *this; }
  inline bool operator==(const IndexVec &obj) const { return x == obj.x && y == obj.y; }
  inline bool operator!=(const IndexVec &obj) const { return x != obj.x || y != obj.y; }

  //自分とobjを足しても迷路座標の範囲に収まるかどうか
  inline bool canSum(const IndexVec &obj) const
  {
    const int8_t res_x = x + obj.x;
    if (res_x<0 || MAZE_SIZE<=res_x) return false;
    const int8_t res_y = y + obj.y;
    if (res_y<0 || MAZE_SIZE<=res_y) return false;
    return true;
  }
  inline bool canSub(const IndexVec &obj) const
  {
    const int8_t res_x = x - obj.x;
    if (res_x<0 || MAZE_SIZE<=res_x) return false;
    const int8_t res_y = y - obj.y;
    if (res_y<0 || MAZE_SIZE<=res_y) return false;
    return true;
  }

  //L1ノルム
  inline uint8_t norm() const
  {
    const int8_t x_abs = x>0?x:-x;
    const int8_t y_abs = y>0?y:-y;
    return x_abs + y_abs;
  }

  inline bool isDiag() const
  {
    const int8_t x_abs = x>0?x:-x;
    const int8_t y_abs = y>0?y:-y;
    return x_abs == 1 && y_abs == 1;
  }

  inline bool isCorner(){ return x == MAZE_SIZE-1 || x == 0 || y == MAZE_SIZE-1 || y == 0; }

  //便利な定数
  //各方角を表すベクトル
  static const IndexVec vecNorth;
  static const IndexVec vecEast;
  static const IndexVec vecSouth;
  static const IndexVec vecWest;
  //0->EAST 1->NORTH 2->WEST 3->SOUTH
  static const IndexVec vecDir[4];
};

class Maze{
  private:
    std::bitset<WALL_AMOUNT>* wall_pointer;
    uint16_t step_map[MAZE_SIZE][MAZE_SIZE];
  public:
    Maze(std::bitset<WALL_AMOUNT> &wall): wall_pointer(&wall){clear();}
    inline Direction getWall(const IndexVec index) { return getWall(index.x, index.y); }
    Direction getWall(int8_t x, int8_t y);
    Direction getNORTH(int8_t x, int8_t y);
    Direction getEAST(int8_t x, int8_t y);
    Direction getWEST(int8_t x, int8_t y);
    Direction getSOUTH(int8_t x, int8_t y);
    inline void updateWall(IndexVec &index, Direction dir, bool forceWrite = true){ updateWall(index.x, index.y, dir, forceWrite);}
    void updateWall(int8_t x, int8_t y, Direction dir, bool forceWrite = true);
    void clear(){
      wall_pointer->reset();
      for(int i=0; i < MAZE_SIZE; i++){
        wall_pointer->set(EAST_STATE(MAZE_SIZE - 1, i), 1);
        wall_pointer->set(EAST_VISIBLE(MAZE_SIZE - 1, i), 1);
        wall_pointer->set(NORTH_STATE(i, MAZE_SIZE - 1), 1);
        wall_pointer->set(NORTH_VISIBLE(i, MAZE_SIZE - 1), 1);
      }
    }
};
struct NodeInfo{
  public:
    uint32_t serial_number;
    float cost;
  public:
    NodeInfo(){}
    NodeInfo(uint32_t num, float cost_ = 0) : serial_number(num), cost(cost_){}
    inline bool operator<(const NodeInfo &obj) const { return cost < obj.cost;}
    inline bool operator>(const NodeInfo &obj) const { return cost > obj.cost;}
    std::pair<IndexVec, Direction> getIndexInfo(){
      int8_t x = (serial_number % 64) / 2;;
      int8_t y = serial_number / 64;
      Direction dir = ((serial_number) / 2 == 0) ? NORTH : EAST;
      return {IndexVec(x,y), dir};
    }
    void debug_info(){
      printf("serial_number is %d, cost is %d\n", serial_number, cost);
    }
};
class Node{
  private:
    NodeInfo node[WALL_AMOUNT];
  public:
    Node(){clear();}
    void clear(){
      for(int i = 0; i < WALL_AMOUNT; i++){
        node[i].cost = 0.0;
        node[i].serial_number = i;
      }
    }
    std::priority_queue<NodeInfo> search_neighbor_node(uint32_t present_number){
      std::priority_queue<NodeInfo> list;
      //NORTH
      if(present_number % 2 == 0){
        //NORTH
        if(present_number < WALL_AMOUNT - 2 * MAZE_SIZE){
          list.push(node[present_number + 2 * MAZE_SIZE]);
          //NW
          if(present_number % (2 * MAZE_SIZE) != 0) list.push(node[present_number + 2 * MAZE_SIZE - 1]);
          //NE
          if(present_number % (2 * MAZE_SIZE) != 2 * (MAZE_SIZE - 1)) list.push(node[present_number + 2 * MAZE_SIZE +1]);
        //SOUTH EAST
        if(present_number % (2 * MAZE_SIZE) != 2 * (MAZE_SIZE - 1)) list.push(node[present_number + 1]);
        //SOUTH
        if(present_number >= (2 * MAZE_SIZE))  list.push(node[present_number -  2 * MAZE_SIZE]);
        //SW
        if(present_number % (2 * MAZE_SIZE) != 0) list.push(node[present_number - 1]);
        }
      }
      return list;
    }
};
#endif
