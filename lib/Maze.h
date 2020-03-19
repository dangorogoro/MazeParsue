#ifndef MAZE_H_
#define MAZE_H_

#include <cstdint>
#include <bitset>
#include <cstddef>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <iterator>
#include <chrono>
#include <algorithm>
#include "Parsue_Conf.h"
#include "Cost.h"

extern std::chrono::system_clock::time_point start_chrono, end_chrono;

constexpr uint32_t WALL_AMOUNT = 2 * (2 * MAZE_SIZE * MAZE_SIZE);
inline uint8_t bit_count(uint8_t byte){
  uint8_t b = byte & 0x0f;
  b = (b & 0x55) + (b >> 1 & 0x55);
  b = (b & 0x33) + (b >> 2 & 0x33);
  b = (b & 0x0f) + (b >> 4 & 0x0f);
  return b;
}

class Node;
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

template<uint32_t T>
using WallStr = std::bitset<T>;
extern WallStr<WALL_AMOUNT> WallData;
template<class T>
using NodeQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template<class T>
using NodePureQueue = std::queue<T>;
struct NodeIndex;

constexpr uint8_t EAST = 0x01;
constexpr uint8_t NORTH = 0x02;
constexpr uint8_t WEST = 0x04;
constexpr uint8_t SOUTH = 0x08;
constexpr uint8_t DONE_EAST = 0x10;
constexpr uint8_t DONE_NORTH = 0x20;
constexpr uint8_t DONE_WEST = 0x40;
constexpr uint8_t DONE_SOUTH = 0x80;
constexpr uint8_t NORTH_EAST = 0x03;
constexpr uint8_t NORTH_WEST = 0x06;
constexpr uint8_t SOUTH_EAST = 0x09;
constexpr uint8_t SOUTH_WEST = 0x0c;

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
struct __attribute__ ((__packed__)) EdgeVec {
  Direction dir;
  uint8_t cnt;

  EdgeVec(Direction dir_= 0, uint8_t cnt_= 0) : dir(dir_), cnt(cnt_) {}
  inline void clear(){dir = 0, cnt = 0;}
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
    if (res_x<0 || MAZE_SIZE<=(uint8_t)res_x) return false;
    const int8_t res_y = y + obj.y;
    if (res_y<0 || MAZE_SIZE<=(uint8_t)res_y) return false;
    return true;
  }
  inline bool canSub(const IndexVec &obj) const
  {
    const int8_t res_x = x - obj.x;
    if (res_x<0 || MAZE_SIZE<=(uint8_t)res_x) return false;
    const int8_t res_y = y - obj.y;
    if (res_y<0 || MAZE_SIZE<=(uint8_t)res_y) return false;
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
    inline Direction getWall(const int8_t& x, const int8_t& y){
      Direction dir;
      dir = getEAST(x,y) | getNORTH(x,y) | getWEST(x,y) | getSOUTH(x,y);
      return dir;
    }
    inline Direction getNORTH(const int8_t& x, const int8_t& y){
      uint8_t state = wall_pointer->test(NORTH_STATE(x, y));
      uint8_t visible = wall_pointer->test(NORTH_VISIBLE(x, y));
      return Direction((state ? NORTH : 0) | (visible ? DONE_NORTH : 0));
    }
    inline Direction getEAST(const int8_t& x, const int8_t& y){
      if((uint8_t)x >= MAZE_SIZE - 1) return (EAST | DONE_EAST);
      else{
        uint8_t state = wall_pointer->test(EAST_STATE(x, y));
        uint8_t visible = wall_pointer->test(EAST_VISIBLE(x, y));
        return Direction((state ? EAST : 0) | (visible ? DONE_EAST: 0));
      }
    }
    inline Direction getWEST(const int8_t& x, const int8_t& y){
      if(x <= 0) return WEST | DONE_WEST;
      else return getEAST(x - 1, y) << 0x02;
    }
    inline Direction getSOUTH(const int8_t& x, const int8_t& y){
      if(y <= 0) return SOUTH | DONE_SOUTH;
      else return getNORTH(x, y - 1) << 0x02;
    }
    inline void updateWall(IndexVec index, Direction dir, bool forceWrite = true){ updateWall(index.x, index.y, dir, forceWrite);}
    void updateWall(int8_t x, int8_t y, Direction dir, bool forceWrite = true);
    void printWall();
    void printWall(const bool value[MAZE_SIZE][MAZE_SIZE]);
    void printWall(NodeQueue<NodeIndex> node_queue);
    void printWall(std::priority_queue<int32_t> id_queue);
    void printWall(const Node& node);
    void printWall(std::priority_queue<int32_t> id_queue, const std::set<int32_t> &goal_set);
    void printWall(int32_t id, const std::set<int32_t> &goal_set){
      std::priority_queue<int32_t> id_queue;
      id_queue.push(id);
      printWall(id_queue, goal_set);
    }
    void loadFromArray(uint8_t* array);
    void clear(){
      wall_pointer->reset();
      for(uint16_t i=0; i < MAZE_SIZE; i++){
        wall_pointer->set(EAST_STATE(MAZE_SIZE - 1, i), 1);
        wall_pointer->set(EAST_VISIBLE(MAZE_SIZE - 1, i), 1);
        wall_pointer->set(NORTH_STATE(i, MAZE_SIZE - 1), 1);
        wall_pointer->set(NORTH_VISIBLE(i, MAZE_SIZE - 1), 1);
      }
    }
};
struct NodeInfo{
  public:
    int32_t serial_number;
    int32_t mother_number;
    uint32_t cost;
    std::bitset<WALL_AMOUNT>* wall_pointer;
    EdgeVec edgeVec;
  public:
    NodeInfo(){}
    NodeInfo(int32_t num, uint32_t cost_ = 0): serial_number(num), mother_number(serial_number), cost(cost_), edgeVec(0,0){}
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
    bool isCorner() const{
      if((serial_number % (2 * MAZE_SIZE) == (2 * MAZE_SIZE - 1)) || (serial_number % 2 == 0 &&  (uint32_t)serial_number > WALL_AMOUNT - 2 * MAZE_SIZE))
        return true;
      else return false;
    }
    int32_t get_my_id()const{ return serial_number;}
    int32_t get_mother_id()const{ return mother_number;}
    uint32_t get_cost() const{return cost;}
    EdgeVec get_edgeVec() const{return edgeVec;}
    inline bool get_wall_state() const{return WallData.test(serial_number * 2);}
    inline bool get_wall_visible() const{return WallData.test(serial_number * 2 + 1);}
    inline void clear_wall_visible() const{WallData.reset(serial_number * 2 + 1);}

    inline void set_info(const int32_t &mother, const uint32_t &cost_, EdgeVec edge_){
      set_info(mother, cost_);
      edgeVec = edge_;
    }
    inline void set_info(const int32_t &mother, const uint32_t &cost_){
      mother_number = mother;
      cost = cost_;
    }
};
struct NodeIndex{
  private:
    //std::pair<uint32_t, uint32_t> index;
    NodeInfo* node_pointer;
    int32_t index;
  public:
    NodeIndex(NodeInfo* node, int32_t index_) : node_pointer(node), index(index_){}
    inline bool operator<(const NodeIndex &obj) const{return node_pointer[index].get_cost() < obj.get_cost();}
    inline bool operator>(const NodeIndex &obj) const{return node_pointer[index].get_cost() > obj.get_cost();}
    uint32_t get_cost()const {return node_pointer[index].get_cost();}
    int32_t get_my_id()const {return index;}
    EdgeVec get_edgeVec()const {return node_pointer[index].get_edgeVec();}
};
struct NeighborIndexArray{
  private:
    NodeInfo* node_pointer;
    int32_t correct_size;
    std::array<int32_t, 6> index_array;
  public:
    NeighborIndexArray(NodeInfo* node) : node_pointer(node), correct_size(0){index_array.fill(0);}
    void fill(const int32_t& value){index_array.fill(0);}
    void set_correct_size(const int32_t& correct_size_){correct_size = correct_size_;}
    void increment_correct_size(){correct_size++;}
    int32_t get_correct_size()const {return correct_size;}
    NodeIndex get_node_array_index(int32_t array_index) const {return NodeIndex(node_pointer, index_array.at(array_index));}
    int32_t at(const int32_t& index) const{return index_array.at(index);}
    void set(const int32_t& index, const int32_t& value) {index_array[index] = value;}
};
class Node{
  private:
    NodeInfo node[WALL_AMOUNT];
  public:
    Node(){clear();}
    void clear(){
      for(uint16_t i = 0; i < WALL_AMOUNT; i++){
        node[i].cost = 65535;
        node[i].serial_number = i;
        node[i].mother_number = i;
        node[i].edgeVec.clear();
      }
    }
    NodeQueue<NodeIndex> getPathQueue(const int32_t& start_id, const int32_t& end_id);
    void updateNodeIndex(NodeIndex target_node, bool force_flag = false);
    void updateNodeIndex(NodeQueue<NodeIndex> node_queue, bool force_flag = false);
    NodeQueue<NodeIndex> expandQueue(NodeQueue<NodeIndex> src_queue, NodeQueue<NodeIndex>dst_queue);
    //NodeQueue<NodeIndex> queueTask(int32_t start_id);
    
    std::set<int32_t> getUnknownFastestWall(const int32_t &start_id, const int32_t &end_id);
    //void startEdgeMap(const int32_t &start_id, const int32_t &end_id, bool visible = false);
    int32_t startEdgeMap(const int32_t &start_id, const std::set<int32_t>& end_set, bool visible = false);
    int32_t startEdgeMap(const int32_t &start_id, const int32_t &end_id, bool visible = false);
    int32_t startPureEdgeMap(const int32_t &start_id, const std::set<int32_t>& end_set, bool visible = false); 
    void updateQueue(NodeQueue<NodeIndex> &node_queue, const std::vector<NodeIndex> &node_list, const int32_t& mother_id);
    void updateQueue(NodeQueue<NodeIndex> &node_queue, const NeighborIndexArray &neighbor_array, const int32_t& mother_id);
    void updateQueue(NodePureQueue<NodeIndex> &node_queue, const NeighborIndexArray &neighbor_array, const int32_t& mother_id);
    void updateFastestQueue(NodeQueue<NodeIndex> &node_queue, const std::vector<NodeIndex> &node_list, const int32_t& mother_id);
    void updateFastestQueue(NodeQueue<NodeIndex> &node_queue, const NeighborIndexArray &neighbor_array, const int32_t& mother_id);
    void checkQueueQuality(std::vector<NodeIndex> &target_list, bool visible = false);
    bool checkQueueQuality(const int32_t& target_id, bool visible = false);
    //std::vector<NodeIndex> getOnLineNode(int32_t present_number, bool visible = false);
    //inline std::vector<NodeIndex> getNeighborNode(const int32_t& present_number, bool visible = false);
    inline NeighborIndexArray getNeighborNode(const int32_t& present_number, bool visible = false);
    NeighborIndexArray getDirectedNeighborNode(const int32_t& present_number, const Direction& dir, bool visible = false);
    int32_t startFastestMap(const int32_t &start_id, const int32_t &end_id, bool visible = false);
    int32_t startFastestMap(const int32_t &start_id, const std::set<int32_t> &end_set, bool visible = false);
    NodeInfo get_node(const int32_t &num) const {return node[num];}
};
//void node_debug(NodeQueue<NodeIndex> poi);
bool node_check(const NeighborIndexArray& neighbor_array, const int32_t& end_id);
int32_t node_check(const NeighborIndexArray& neighbor_array, const std::set<int32_t>& end_set);
bool node_check(const std::vector<NodeIndex>& node_list, int32_t end_id);
int32_t node_check(const std::vector<NodeIndex>& node_list, const std::set<int32_t>& end_set);
Direction node_relation(const int32_t &src, const int32_t &dst_index);

#endif
