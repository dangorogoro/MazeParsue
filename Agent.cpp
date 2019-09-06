#include <algorithm>

#include "Parsue_Conf.h"
#include "Agent.h"
#include "Operation.h"

int32_t target_id = GOAL;
void Agent::reset()
{
	state = Agent::IDLE;
	distIndexList.clear();
	dist.x = 0;
	dist.y = 0;
  //lastDir = NORTH;
  //presentDir = NORTH;
  presentRobotDir = NORTH;
  lastRobotDir = NORTH;
  id = 0;
  nextOP.op = Operation::FORWARD;
}

void Agent::update(const IndexVec &vec, const Direction &dir){
  if(state == Agent::IDLE)  state = Agent::SEARCHING_NOT_GOAL;
  dist = vec;
  auto lastOP = nextOP;
  maze->updateWall(vec, dir);
  //node->start_edge_map(id, target_id);
  node->startEdgeMap(id, target_id);
  auto node_queue = node->getPathQueue(id, target_id);
  auto dir_list = generateDirectionList(node_queue);

  auto next_dir = dir_list[0];
  nextOP = nextOperation(lastOP, presentRobotDir, next_dir);


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
    id = node_queue.top().get_my_id();
    if(bit_count(next_dir) == 2)  presentRobotDir = next_dir - (presentRobotDir & next_dir);
  }

  IndexVec tmp_vec = getNextIndex();
  print_operation(nextOP);
  //printf("id == %d, (x,y) == (%d,%d)\n",id, tmp_vec.x, tmp_vec.y);
  mazePrint(id);

  if(target_id == id && state == Agent::SEARCHING_NOT_GOAL){
    //printf("finished");
    target_id = 0;
    state = Agent::BACK_TO_START;
  }
  if(target_id == id && state == Agent::BACK_TO_START){
    state = Agent::FINISHED;
  }
}
Operation Agent::getNextOperation(){
  return nextOP;
}
IndexVec Agent::getNextIndex(){
  IndexVec tmp_vec = dist;
  if(lastRobotDir == NORTH){
    if(presentRobotDir == EAST)  tmp_vec.x = tmp_vec.x + 1;
    else if(presentRobotDir == WEST)  tmp_vec.x = tmp_vec.x - 1;
    else if(presentRobotDir == NORTH) tmp_vec.y = tmp_vec.y + 1;
  }
  else if(lastRobotDir == WEST){
    if(presentRobotDir == NORTH)  tmp_vec.y = tmp_vec.y + 1;
    else if(presentRobotDir == SOUTH)  tmp_vec.y = tmp_vec.y - 1;
    else if(presentRobotDir == WEST) tmp_vec.x = tmp_vec.x - 1;
  }
  else if(lastRobotDir == SOUTH){
    if(presentRobotDir == EAST)  tmp_vec.x = tmp_vec.x + 1;
    else if(presentRobotDir == WEST)  tmp_vec.x = tmp_vec.x - 1;
    else if(presentRobotDir == SOUTH) tmp_vec.y = tmp_vec.y - 1;
  }
  else if(lastRobotDir == EAST){
    if(presentRobotDir == NORTH)  tmp_vec.y = tmp_vec.y + 1;
    else if(presentRobotDir == SOUTH)  tmp_vec.y = tmp_vec.y - 1;
    else if(presentRobotDir == EAST)  tmp_vec.x = tmp_vec.x + 1;
  }
  else{
    printf("wrong!!!!!!!!!!!!!!!!!!!!!!!!!");
  }
  return tmp_vec;
}
void Agent::mazePrint(int32_t id){
  std::priority_queue<int> id_queue;
  id_queue.push(id);
  maze->printWall(id_queue);
}
