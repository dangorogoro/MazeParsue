#include <algorithm>

#include "Parsue_Conf.h"
#include "Agent.h"
#include "Operation.h"


int32_t target_id = 64 * 7 + 2 * 8;
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
  dist = vec;
  auto lastOP = nextOP;
  maze->updateWall(vec, dir);
  node->start_edge_map(id, target_id);
  auto node_queue = node->getPathQueue(id, target_id);
  auto dir_list = generateDirectionList(node_queue);
  /* どうにかしたい
   *
   *
   *
   */
  //if(dir_list.size() >= 3){
  //  next_dir = dir_list[1];
  //  future_dir = dir_list[2];
  //  nextOP = nextOperation(lastOP, present_dir, next_dir, future_dir);
  //}
  //else{
  //  if(dir_list.size() == 2){
  //    next_dir = dir_list[1];
  //    nextOP = nextOperation(lastOP, present_dir, next_dir);
  //  }
  //  else{
  //    Direction first_dir = present_dir;
  //    Direction second_dir = present_dir - lastDir;
  //    nextOP = nextOperation(lastOP, first_dir, second_dir);
  //  }
  //}
  //if(bit_count(present_dir) == 1 && bit_count(lastDir) == 1)nextOP.op = Operation::FORWARD; 
  //else{
  //  Direction first_dir = present_dir & lastDir;
  //  Direction second_dir;
  //  if(bit_count(present_dir) == 1 && bit_count(lastDir) == 2) second_dir = lastDir - present_dir;
  //  else  second_dir = present_dir - lastDir;
  //  nextOP = nextOperation(lastOP, first_dir, second_dir);
  //}
  auto next_dir = dir_list[0];
  nextOP = nextOperation(lastOP, presentRobotDir, next_dir);


  //printf("present dir is 0x%d\n", present_dir);
  //printf("next dir is 0x%d\n", next_dir);
  //printf("future dir is 0x%d\n", future_dir);

  lastRobotDir = presentRobotDir;
  if(uint8_t(next_dir & presentRobotDir) == 0){
    nextOP = Operation::BACK_180;
    //lastDir = (lastDir << 2) / 16 + (lastDir << 2) % 16;
    presentRobotDir = (presentRobotDir << 2) / 16 + (presentRobotDir << 2) % 16;
    lastRobotDir = presentRobotDir;
    printf("0x%x\n", presentRobotDir);
  }
  else{
    node_queue.pop();
    id = node_queue.top().get_my_id();
    if(bit_count(next_dir) == 2)  presentRobotDir = next_dir - (presentRobotDir & next_dir);
  }

  IndexVec tmp_vec = getNextIndex();
  print_operation(nextOP);
  printf("id == %d, (x,y) == (%d,%d)\n",id, tmp_vec.x, tmp_vec.y);
  mazePrint(id);
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
