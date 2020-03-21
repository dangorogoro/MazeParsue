#include <algorithm>

#include "Parsue_Conf.h"
#include "Agent.h"
#include "Operation.h"

//std::set<int32_t> GOAL_LIST{64 * 7 + 2 * 7, 64 * 7 + 2 * 7 + 1, 64 * 7 + 2 * 8 + 1, 64 * 7 + 2 * 8};
std::set<int32_t> GOAL_LIST{GOAL};
//状態をIDLEにし、path関連を全てクリアする
void Agent::reset(){
	state = Agent::IDLE;
	distIndexList.clear();
	dist.x = 0;
	dist.y = 0;
  //lastDir = NORTH;
  //presentDir = NORTH;
  presentRobotDir = NORTH;
  lastRobotDir = NORTH;
  currentID = 0;
  nextOP.op = Operation::FORWARD;
  addGoal(GOAL_LIST);
  presentGoal = getGoal();
  node->get_node(presentGoal).clear_wall_visible();
  maze->updateWall(IndexVec(0,0), 0xfd);
}
void Agent::addGoal(const std::set<int32_t>& id_set){
  for(auto itr = id_set.begin(); itr != id_set.end(); itr++){
    goalSet.insert(*itr);
  }
}
void Agent::update(const IndexVec &vec, const Direction &dir){
  if(state == Agent::IDLE)  state = Agent::SEARCHING_NOT_GOAL;
  dist = vec;
  //auto lastOP = nextOP;
  maze->updateWall(vec, dir, false);
  if(node->get_node(presentGoal).get_wall_visible() == true && state == Agent::SEARCHING_NOT_GOAL){
    deleteGoal(presentGoal);
    if(getGoalSize() == 0){
      auto check_set = node->getUnknownFastestWall(0, GOAL);
      goalSet.merge(check_set);
      while(node->get_node(getGoal()).get_wall_visible() == true){
        deleteGoal(getGoal());
        if(getGoalSize() == 0)  break;
      }
      if(getGoalSize() == 0){
        state = Agent::BACK_TO_START;
        addGoal(0);
      }
      else  state = Agent::SEARCHING_REACHED_GOAL;
    }
  }
  else if(node->get_node(presentGoal).get_wall_visible() == true && state == Agent::SEARCHING_REACHED_GOAL){
    deleteGoal(presentGoal);
    while(node->get_node(getGoal()).get_wall_visible() == true){
      deleteGoal(getGoal());
      if(getGoalSize() == 0)  break;
    }
    if(node->get_node(presentGoal).get_wall_state() == true){
      goalSet.clear();
      goalSet = node->getUnknownFastestWall(0, GOAL);
    }
    if(getGoalSize() == 0){
      state = Agent::BACK_TO_START;
      addGoal(0);
    }
  }
  determinedFutureCalc();
  //IndexVec tmp_vec = getNextIndex();
  //print_operation(nextOP);
  //printf("id == %d, (x,y) == (%d,%d)\n",id, tmp_vec.x, tmp_vec.y);
  //maze->printWall(id, goalSet);
  if(presentGoal == currentID && state == Agent::BACK_TO_START){
    state = Agent::FINISHED;
  }
}
Operation Agent::getNextOperation(){
  return nextOP;
}
IndexVec Agent::getNextIndex(){
  uint16_t steps = nextOP.n;
  IndexVec tmp_vec = dist;
  if(lastRobotDir == NORTH){
    if(presentRobotDir == EAST)  tmp_vec.x = tmp_vec.x + steps;
    else if(presentRobotDir == WEST)  tmp_vec.x = tmp_vec.x - steps;
    else if(presentRobotDir == NORTH) tmp_vec.y = tmp_vec.y + steps;
  }
  else if(lastRobotDir == WEST){
    if(presentRobotDir == NORTH)  tmp_vec.y = tmp_vec.y + steps;
    else if(presentRobotDir == SOUTH)  tmp_vec.y = tmp_vec.y - steps;
    else if(presentRobotDir == WEST) tmp_vec.x = tmp_vec.x - steps;
  }
  else if(lastRobotDir == SOUTH){
    if(presentRobotDir == EAST)  tmp_vec.x = tmp_vec.x + steps;
    else if(presentRobotDir == WEST)  tmp_vec.x = tmp_vec.x - steps;
    else if(presentRobotDir == SOUTH) tmp_vec.y = tmp_vec.y - steps;
  }
  else if(lastRobotDir == EAST){
    if(presentRobotDir == NORTH)  tmp_vec.y = tmp_vec.y + steps;
    else if(presentRobotDir == SOUTH)  tmp_vec.y = tmp_vec.y - steps;
    else if(presentRobotDir == EAST)  tmp_vec.x = tmp_vec.x + steps;
  }
  else{
    printf("wrong!!!!!!!!!!!!!!!!!!!!!!!!!");
  }
  return tmp_vec;
}
/*
void Agent::mazePrint(int32_t id){
  std::priority_queue<int> id_queue;
  id_queue.push(id);
  maze->printWall(id_queue);
}
*/

void Agent::determinedFutureCalc(){
  presentGoal = node->startPureEdgeMap(currentID, goalSet);
  auto node_queue = node->getPathQueue(currentID, presentGoal);
  auto dir_list = generateDirectionList(node_queue);

  auto next_dir = dir_list[0];
  nextOP = nextOperation(presentRobotDir, next_dir);

  //printf("present dir is 0x%d\n", present_dir);
  //printf("next dir is 0x%d\n", next_dir);
  //printf("future dir is 0x%d\n", future_dir);

  lastRobotDir = presentRobotDir;
  if(uint8_t(next_dir & presentRobotDir) == 0){
    nextOP = Operation::BACK_180;
    presentRobotDir = (presentRobotDir << 2) / 16 + (presentRobotDir << 2) % 16;
    lastRobotDir = presentRobotDir;
    //printf("0x%x\n", presentRobotDir);
  }
  else{
    node_queue.pop();
    int16_t next_id = node_queue.top().get_my_id();
    if(nextOP.op == Operation::FORWARD){
      uint16_t count = 0;
      while(!node_queue.empty()){
        node_queue.pop();
        count++;
        auto top_node_info = node->get_node(node_queue.top().get_my_id());
        if(count >= dir_list.size()){
          printf("size\n");
          break;
        }
        if(top_node_info.get_wall_state() == false && top_node_info.get_wall_visible() == true){
          auto future_dir = dir_list[count];
          if(future_dir == presentRobotDir){
            nextOP.n += 1;
            next_id = node_queue.top().get_my_id();
            printf("next id %d\n", next_id);
          }
          else break;
        }
        else break;
      }
    }
    currentID = next_id;
    if(bit_count(next_dir) == 2)  presentRobotDir = next_dir - (presentRobotDir & next_dir);
  }
}
void futurePattern::push(const Direction &dir, const Operation &op){
  patternPair tmp(dir, op);
  future_pattern.push(tmp);
}
