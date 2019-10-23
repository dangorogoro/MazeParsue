#include <cmath>
#include <cstdio>
#include "Operation.h"

OperationList loadPath(NodeQueue<NodeIndex> node_queue, bool use_diagonal){
  std::vector<Direction> dir_list = generateDirectionList(node_queue);

  OperationList opList;
  Direction last_dir = NORTH;
  Operation last_op(Operation::FORWARD);
  opList.push_back(last_op);
  for(auto itr = dir_list.begin(); itr != dir_list.end(); itr++){
    const Direction next_dir = *itr;
    auto next_op = nextOperation(last_dir, next_dir);
    if(last_op.op == Operation::FORWARD && next_op.op == Operation::FORWARD){
      auto back_vec = opList.back();
      opList.pop_back();
      opList.push_back(Operation(back_vec.op, back_vec.n + 1));
    }
    else{
      last_op = next_op;
      opList.push_back(next_op);
    }
    if(bit_count(next_dir) == 2) last_dir = next_dir - (last_dir & next_dir);
  }

  /* TASK
   * TO CONSIDER DIAGONAL GOAL
   *
   * */
  /*
  auto fake_queue = dir_list.back();
  dir_list.push_back(fake_queue);
  dir_list.push_back(fake_queue);
  dir_list.push_back(fake_queue);
  OperationList opList;
  Operation present_op(Operation::FORWARD);
  for(size_t i = 0;  i < dir_list.size(); i++){
    if(i < dir_list.size() - 3){
      Direction present_dir = dir_list[i];
      Direction next_dir = dir_list[i+1];
      Direction future_dir = dir_list[i+2];
      Operation next_op = nextOperation(present_op, present_dir, next_dir, future_dir);
      if(present_op.op == Operation::FORWARD && next_op.op == Operation::FORWARD){
        auto back_vec = opList.back();
        opList.pop_back();
        opList.push_back(Operation(back_vec.op, back_vec.n + 1));
      }
      else{
        present_op = next_op;
        opList.push_back(next_op);
      }
    }
  }
  */
  return opList;
}
std::vector<Direction> generateDirectionList(NodeQueue<NodeIndex> node_queue){
  NodeIndex last_node = node_queue.top();
  std::vector<Direction> dir_list;
  node_queue.pop();
  while(!node_queue.empty()){
    //printf("last_node id is %d\n", last_node.get_my_id());
    NodeIndex present_node = node_queue.top();
    //printf("present_node id is %d\n", present_node.get_my_id());
    Direction dir = node_relation(last_node.get_my_id(), present_node.get_my_id());
    //printf("dir is 0x%x\n", dir);
    dir_list.push_back(dir);
    last_node = present_node;
    node_queue.pop();
  }
  return dir_list;
}

Operation nextOperation(const Direction &last_dir, const Direction &present_dir){
  if(bit_count(last_dir) == 1 && bit_count(present_dir) == 1)
    return Operation(Operation::FORWARD, 1);
  Direction first_dir = (last_dir & present_dir);
  Direction second_dir = present_dir - first_dir;
  Operation return_op = getTurnOperation(first_dir, second_dir, Operation::TURN_90);
  return return_op;
}

Operation nextOperation(Operation present_op, Direction present_dir, Direction next_dir, Direction future_dir, bool diagonal){
  Operation return_op;
  //printf("present dir is 0x%x\n", present_dir);
  //printf("next dir is 0x%x\n", next_dir);
  //printf("future dir is 0x%x\n", future_dir);
  //FORWARD
  if(((present_dir | future_dir) == next_dir) || ((present_dir ==  next_dir) && bit_count(present_dir) == 1)){
    return_op = Operation(Operation::FORWARD, 1);
    //printf("FORWARD\n");
  }
  else{
    //TURN START
    if(present_op.op == Operation::FORWARD || present_op.op == Operation::BACK_180){
      //TURN 90
      if(bit_count(present_dir & next_dir) == 1 || diagonal == false){
        //printf("TURN_90\n");
        //New solution!!!!!!!!!!!!!1
        //Direction first_dir = present_dir - next_dir;
        //Direction second_dir = present_dir & next_dir;
        Direction first_dir = present_dir - (present_dir & next_dir);
        Direction second_dir = present_dir - first_dir;
        return_op = getTurnOperation(first_dir, second_dir, Operation::TURN_90);
      }
      //TURN 135
      else if(next_dir == future_dir){
        //printf("TURN_135\n");
        Direction second_dir = present_dir & next_dir;
        Direction first_dir = present_dir - second_dir;
        return_op = getTurnOperation(first_dir, second_dir, Operation::TURN_135);
      }
      //TURN180
      else{
        //printf("TURN_180\n");
        Direction second_dir = present_dir & next_dir;
        Direction first_dir = present_dir - second_dir;
        return_op = getTurnOperation(first_dir, second_dir, Operation::TURN_180);
      }
    }
  }
  return return_op;
}

Operation getTurnOperation(Direction first_dir, Direction second_dir, Operation::OperationType type){
  bool flag = false;  //false -> left true -> right
  Operation return_op;
  if(first_dir == NORTH){
    if(second_dir == EAST)  flag = true;
  }
  else if(first_dir == WEST){
    if(second_dir == NORTH) flag = true;
  }
  else if(first_dir == SOUTH){
    if(second_dir == WEST) flag = true;
  }
  else if(first_dir == EAST){
    if(second_dir == SOUTH) flag = true;
  }
  if(type == Operation::TURN_45){
    if(flag == false) return_op = Operation(Operation::TURN_LEFT45, 1);
    else return_op = Operation(Operation::TURN_RIGHT45, 1);
  }
  else if(type == Operation::TURN_135){
    if(flag == false) return_op = Operation(Operation::TURN_LEFT135, 1);
    else return_op =  Operation(Operation::TURN_RIGHT135, 1);
  }
  else if(type == Operation::TURN_90){
    if(flag == false) return_op = Operation(Operation::TURN_LEFT90, 1);
    else return_op =  Operation(Operation::TURN_RIGHT90, 1);
  }
  else if(type == Operation::TURN_180){
    if(flag == false) return_op = Operation(Operation::TURN_LEFT180, 1);
    else return_op = Operation(Operation::TURN_RIGHT180, 1);
  }
  return return_op;
}
void print_operation(Operation op){
  OperationList opList;
  opList.push_back(op);
  print_operation(opList);
}
void print_operation(OperationList runSequence){
  printf("===============\n");
  for(size_t i = 0;i < runSequence.size();i++){
    if(runSequence[i].op == Operation::FORWARD) printf("FORWARD");
    else if(runSequence[i].op == Operation::FORWARD_DIAG)  printf("FORWARD_DIAG");
    else if(runSequence[i].op == Operation::TURN_RIGHT90) printf("TURN_RIGHT90");
    else if(runSequence[i].op == Operation::TURN_RIGHT45)  printf("TURN_RIGHT45");
    else if(runSequence[i].op == Operation::TURN_LEFT90) printf("TURN_LEFT90");
    else if(runSequence[i].op == Operation::TURN_LEFT45) printf("TURN_LEFT45");
    else if(runSequence[i].op == Operation::STOP) printf("STOP");
    else if(runSequence[i].op == Operation::TURN_RIGHT135) printf("TURN_RIGHT135");
    else if(runSequence[i].op == Operation::TURN_RIGHT180) printf("TURN_RIGHT180");
    else if(runSequence[i].op == Operation::TURN_LEFT135 ) printf("TURN_LEFT135");
    else if(runSequence[i].op == Operation::TURN_LEFT180 ) printf("TURN_LEFT135");
    else if(runSequence[i].op == Operation::TURN_RIGHT90S) printf("TURN_RIGHT90S");
    else if(runSequence[i].op == Operation::TURN_LEFT90S ) printf("TURN_LEFT90S");
    else if(runSequence[i].op == Operation::V_RIGHT90     ) printf("LEFT_V90");
    else if(runSequence[i].op == Operation::V_RIGHT90    ) printf("RIGHT_V90");
    else if(runSequence[i].op == Operation::TURN_135     ) printf("TURN_135");
    else if(runSequence[i].op == Operation::V_90) printf("V90");
    else if(runSequence[i].op == Operation::TURN_45) printf("TURN_45");
    else if(runSequence[i].op == Operation::BACK_180) printf("BACK_180");
    printf(" -> %d\n", runSequence[i].n);
  }
}
