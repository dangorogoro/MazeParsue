#ifndef OPERATION_H_
#define OPERATION_H_

#include <vector>
#include <cstddef>
#include "Maze.h"
#include "Parsue_Conf.h"



typedef std::vector<IndexVec> Path;

/**************************************************************
 * Operation
 *	ロボットがすべき動作を表現
 *	走行ルートを表現するのに使う
 *	opが動きの種類で、「opをn回実行する」という意味
 **************************************************************/
//Operation Solver
struct Operation {
  typedef enum {
    FORWARD,
    FORWARD_DIAG,
    TURN_RIGHT90,
    TURN_RIGHT45,
    TURN_LEFT90,
    TURN_LEFT45,
    STOP,
    TURN_RIGHT135,
    TURN_RIGHT180,
    TURN_LEFT135,
    TURN_LEFT180,
    TURN_RIGHT90S,
    TURN_LEFT90S,
    V_RIGHT90,
    V_LEFT90,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_45,
    TURN_90,
    TURN_135,
    TURN_180,
    V_90,
    BACK_180
  } OperationType;

  OperationType op;
  uint8_t n;
  Operation(OperationType _op = STOP, uint8_t _n = 1) : op(_op), n(_n) {}
};


/**************************************************************
 * Operation List
 *	スタートからゴールまでの一連のOperationの保持する
 *	コンストラクタのPathを入れると勝手に変換する
 **************************************************************/
class OperationList {
  private:
    std::vector<Operation> opList;

  public:
    OperationList() { }
    const OperationList &operator=(const OperationList &rhs)
    {
      opList = rhs.opList;
      return (*this);
    }

    //std::vectorとおんなじようなインターフェース
    inline std::vector<Operation>::const_iterator begin() const {return opList.begin(); }
    inline std::vector<Operation>::const_iterator end() const { return opList.end(); }
    inline Operation back() const { return opList.back(); }
    inline size_t size() const { return opList.size(); }
    inline void push_back(const Operation& op) { opList.push_back(op); }
    inline void pop_back() { opList.pop_back(); }
    const Operation &operator[](size_t i) const { return opList[i]; }
};
OperationList loadPath(const GeneralIndexVector &index_vector, bool use_diagonal);
Operation nextOperation(const Direction &last_dir, const Direction &present_dir);
Operation nextOperation(Operation op, Direction present_dir, Direction next_dir, Direction future_dir, bool diagonal = false);
std::vector<Direction> generateDirectionList(const GeneralIndexVector &index_vector);
Operation getTurnOperation(Direction first_dir, Direction second_dir, Operation::OperationType type);
void print_operation(Operation op);
void print_operation(OperationList list);
int32_t calc_id_from_operation(const int32_t& currentID, const Direction& dir, const Operation& nextOP);
Direction calc_dir_from_operation(const Direction& dir, const Operation& nextOP);
std::vector<int16_t> generate_degree_diff_list(std::vector<Direction> dir_list);
Operation get_turn_direction(const Direction& last_dir, const Direction& present_dir);
#endif /* OPERATION_H_ */
