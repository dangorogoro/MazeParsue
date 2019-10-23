#ifndef AGENT_H_
#define AGENT_H_

#include <list>
#include <set>
#include <vector>

#include "Maze.h"
#include "Operation.h"

/**************************************************************
 * Agent
 *	探索時にロボットに動きの指示を与える
 *	探索・最短経路の計算において最も上位に位置する
 *
 *	一区画進むごとにupdateを呼び出して今の座標と壁情報を入力していく
 *	updateを実行すると次に進むべき方向が計算される
 *
 *	迷路情報は外に保存をするが、Agent::updateを通して更新をしていく
 **************************************************************/
constexpr int32_t GOAL = 64 * 7 + 2 * 7;
extern std::set<int32_t> GOAL_LIST;
class Agent {
public:
	typedef enum {
		IDLE, 					//まだ実行されていない
		SEARCHING_NOT_GOAL, 	//探索中　まだ一度もゴールに到達していない
		SEARCHING_REACHED_GOAL, //探索中　一度ゴールに到達したが、追加で探索をしている
		BACK_TO_START, 			//スタートに戻っている
		FINISHED 				//スタート地点に到着をし、最短経路の計算の準備ができた
	} State;

private:
	Maze* maze;
  Node* node;
	State state;

	//現在目指している目標座標
	IndexVec dist;

  int32_t id;

	//目標座標リスト
	std::list<IndexVec> distIndexList;

	//Direction lastDir;
	//Direction presentDir;
  Direction lastRobotDir;
  Direction presentRobotDir;

  Operation nextOP;
  std::set<int32_t> goalSet;
  int32_t present_goal;
public:
	Agent(Maze &_maze, Node &_node) :maze(&_maze), node(&_node), state(Agent::IDLE){ reset(); }
  inline void addGoal(const int32_t &id){goalSet.insert(id);}
  inline void deleteGoal(const int32_t &id){goalSet.erase(id);}
  void addGoal(const std::set<int32_t>& id_list);
  inline int32_t getGoal(){return *goalSet.begin();}
  int32_t getNearlyGoal();
  inline int32_t getGoalSize(){return goalSet.size();}
	//状態をIDLEにし、path関連を全てクリアする
	void reset();

	void update(const IndexVec &cur, const Direction &cur_wall);
  Operation getNextOperation();
  IndexVec getNextIndex();
  inline const State &getState() const {return state;}
  //void mazePrint(){maze->printWall();}
  //void mazePrint(int32_t id);
};



#endif /* AGENT_H_ */
