#include <algorithm>

#include "Parsue_Conf.h"
#include "Agent.h"
#include "Operation.h"


//std::set<int32_t> GOAL_LIST{64 * 7 + 2 * 7, 64 * 7 + 2 * 7 + 1, 64 * 7 + 2 * 8 + 1, 64 * 7 + 2 * 8};
std::set<int32_t> GOAL_LIST{GOAL, GOAL + 2};
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
  foresightFlag = false;
}
void Agent::addGoal(const std::set<int32_t>& id_set){
  for(auto itr = id_set.begin(); itr != id_set.end(); itr++){
    goalSet.insert(*itr);
  }
}
bool Agent::getGoalBankrupt(){
  for(auto itr = goalSet.begin();itr != goalSet.end();itr++){
    if(node->get_node(*itr).get_wall_visible() == true) return true;
    //if(node->get_node(*itr).get_visible_state() == true) return true;
  }
  return false;
}
void Agent::update(const IndexVec &vec, const Direction &dir){
  dist = vec;
  maze->updateWall(vec, dir, false);
  if(state == Agent::IDLE)  state = Agent::SEARCHING_NOT_GOAL;
  if(node->get_node(presentGoal).get_wall_visible() == true && state != Agent::BACK_TO_START){
    if(state == Agent::SEARCHING_NOT_GOAL){
      while(node->get_node(getGoal()).get_wall_visible() == true){
        deleteGoal(getGoal());
        if(getGoalSize() == 0)  break;
      }
      if(getGoalSize() == 0){
        //goalSet = node->getUnknownFastestWall(0, GOAL);
        goalSet = future_pattern.get_future_goal();
        state = Agent::SEARCHING_REACHED_GOAL;
      }
    }
    if(state == Agent::SEARCHING_REACHED_GOAL){
      //Care
      if(getGoalBankrupt() == true){
        //printf("New Path\n");
        //goalSet = node->getUnknownFastestWall(0, GOAL);
        goalSet = future_pattern.get_future_goal();
      }
      else deleteGoal(presentGoal);
      if(getGoalSize() == 0){
        state = Agent::BACK_TO_START;
        addGoal(0);
      }
    }
  }
  //determinedFutureCalc();
  maze->printWall(currentID, goalSet);
  drawFuture(dir);

  if(currentID == 0 && state == Agent::BACK_TO_START){
    state = Agent::FINISHED;
  }
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
/*
void Agent::determinedFutureCalc(){
  presentGoal = node->startPureEdgeMap(currentID, goalSet);
  auto node_queue = node->getPathQueue(currentID, presentGoal);
  auto dir_list = generateDirectionList(node_queue);

  auto next_dir = dir_list[0];
  nextOP = nextOperation(presentRobotDir, next_dir);

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
*/
void Agent::futureCalc(){
  std::queue<NodeInfo> nodeInfoQueue;
  std::queue<uint8_t> lastStateQueue;
  //このQueueどうにかしろ
  for(auto i = 0;i < 3;i++){
    auto tmpGoal = node->startPureEdgeMap(currentID, goalSet);
    if(tmpGoal < 0) break;
    auto& index_vector = indexVector;
    index_vector.clear();
    node->getPathQueue(currentID, tmpGoal, index_vector);
    auto dir_list = generateDirectionList(index_vector);
    Direction next_dir = dir_list[0];

    //node_queue.pop();
    //auto candidate_node_info = node->get_node(node_queue.top().get_my_id());
    auto itr = index_vector.begin();
    itr++;
    auto candidate_node_info = node->get_node(*itr);

    auto tmpNextOP = nextOperation(presentRobotDir, next_dir);
    if(uint8_t(next_dir & presentRobotDir) == 0){
      tmpNextOP = Operation::BACK_180;
      break;
    }
    else{
      if(tmpNextOP.op == Operation::FORWARD){
        uint16_t count = 0;
        while(itr != index_vector.end()){
          itr++;
          count++;
          auto top_node_info = node->get_node(*itr);
          if(count >= dir_list.size()){
            break;
          }
          if(top_node_info.get_wall_state() == false && top_node_info.get_wall_visible() == true){
            auto future_dir = dir_list[count];
            if(future_dir == presentRobotDir){
              tmpNextOP.n += 1;
            }
            else break;
          }
          else break;
        }
      }
    }
    if(next_dir == presentRobotDir) future_pattern.push(next_dir, tmpNextOP, tmpGoal);
    else future_pattern.push(next_dir - presentRobotDir, tmpNextOP, tmpGoal);
    if(candidate_node_info.get_wall_visible() == true) break;
    lastStateQueue.push((candidate_node_info.get_wall_state() << 1) + candidate_node_info.get_wall_visible());
    candidate_node_info.set_wall_state(true);
    candidate_node_info.set_wall_visible(true);
    nodeInfoQueue.push(candidate_node_info);
  }
  while(!nodeInfoQueue.empty()){
    auto top_info = nodeInfoQueue.front();
    auto last_state = lastStateQueue.front();
    top_info.set_wall_state(last_state & 0x02);
    top_info.set_wall_visible(last_state & 0x01);
    nodeInfoQueue.pop();
    lastStateQueue.pop();
  }

  future_pattern.set_future_goal(node->getUnknownFastestWall(0, GOAL));
  foresightFlag = true;
  //printf("nextID %d\n", currentID);
}
void Agent::drawFuture(const Direction &dir){
  while(foresightIsDone() == false);
  bool hit_flag = false;
  Operation hit_op(Operation::BACK_180);

  while(!future_pattern.empty()){
    auto front_pattern = future_pattern.front();
    auto tmp_byte = front_pattern.get_dir().byte;
    //printf("tmp_byte 0x%x ", tmp_byte);
    //printf("dir 0x%x \n", dir.byte);
    if(hit_flag == false && (0 < ((~dir.byte) & tmp_byte))){
      hit_flag = true;
      hit_op = front_pattern.get_op();
      presentGoal = front_pattern.get_goal();
    }
    future_pattern.pop();
  }
  lastRobotDir = presentRobotDir;
  presentRobotDir = calc_dir_from_operation(presentRobotDir, hit_op);
  nextOP = hit_op;
  if(nextOP.op == Operation::BACK_180){
    lastRobotDir = presentRobotDir;
  }
  currentID = calc_id_from_operation(currentID, lastRobotDir, nextOP);
  //print_operation(nextOP);
  //printf("currentID: %d\n", currentID);
  foresightFlag = false;
}
void futurePattern::push(const Direction &wall, const Operation &op, const int32_t &goal){
  future_pattern.push(patternInfo(wall, op, goal));
}

/*
void futurePattern::debug_print(){
  while(!future_pattern.empty()){
    auto pair = future_pattern.front();
    future_pattern.pop();
    printf("future dir 0x%d: ", pair.first.byte);
    print_operation(pair.second);
  }
}
*/
