#include <cmath>
#include <cstdio>
#include "Operation.h"

OperationList loadPath(const GeneralIndexVector &index_vector, bool use_diagonal){
  std::vector<Direction> dir_list = generateDirectionList(index_vector);

  OperationList opList;
  Direction last_dir = NORTH;
  Operation last_op(Operation::FORWARD);
  opList.push_back(last_op);
  if(use_diagonal == false){
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
  }
  else{
    auto rad_list = generate_degree_diff_list(dir_list);
    int16_t degree_sum = 0;
    for(auto itr = rad_list.begin();itr != rad_list.end(); itr++){
      degree_sum += *itr;
      Operation next_op;
      if(*itr == 0){
        next_op = (degree_sum % 2 == 0) ? Operation(Operation::FORWARD) : Operation(Operation::FORWARD_DIAG);
        if(last_op.op == next_op.op){
          next_op = opList.back();
          next_op.n++;
          opList.pop_back();
        }
        opList.push_back(next_op);
      }
      else{
        auto present_rad = *itr;
        auto start_rad = *itr;
        while(itr != rad_list.end()){
          itr++;
          if((start_rad > 0 && *itr <= 0) || (start_rad < 0 && *itr >= 0)){
            itr--;
            break;
          }
          if(degree_sum % 2 == 0){
            itr--;
            break;
          }
          present_rad += *itr;
          degree_sum += *itr;
        }
        if(last_op.op == Operation::FORWARD_DIAG){
          if(last_op.n == 1)  opList.pop_back();
          else{
            auto back_vec = opList.back();
            opList.pop_back();
            opList.push_back(Operation(back_vec.op, back_vec.n - 1));
          }
        }
        if(present_rad > 0){
          if(present_rad == 1) next_op.op = Operation::TURN_LEFT45;
          else if(present_rad == 2) next_op.op = (degree_sum % 2 == 0) ? Operation::TURN_LEFT90 : Operation::V_LEFT90;
          else if(present_rad == 3) next_op.op = Operation::TURN_LEFT135;
          else if(present_rad == 4) next_op.op = Operation::TURN_LEFT180;
        }
        else{
          if(present_rad == -1) next_op.op = Operation::TURN_RIGHT45;
          else if(present_rad == -2) next_op.op = (degree_sum % 2 == 0) ? Operation::TURN_RIGHT90 : Operation::V_RIGHT90;
          else if(present_rad == -3) next_op.op = Operation::TURN_RIGHT135;
          else if(present_rad == -4) next_op.op = Operation::TURN_RIGHT180;
        }
        opList.push_back(next_op);
      }
      if(itr == rad_list.end()) break;
      //printf("degree sum %d *itr %d\n", degree_sum, *itr);
      last_op = next_op;
    }
  }
  return opList;
}
std::vector<int16_t> generate_degree_diff_list(std::vector<Direction> dir_list){
  std::vector<int16_t> degree_list;
  degree_list.reserve(dir_list.size());
  Direction FIRST_DIR = NORTH;
  Direction present_dir, last_dir;
  last_dir = FIRST_DIR;
  for(auto itr = dir_list.begin(); itr != dir_list.end(); itr++){
    present_dir = *itr;
    if(present_dir == last_dir) degree_list.push_back(0);
    else{
      Operation tmp_op = get_turn_direction(last_dir, present_dir);
      int16_t val = (tmp_op.op == Operation::TURN_RIGHT) ? -1 : 1;
      if(bit_count(present_dir) == 2 && bit_count(last_dir) == 2) val *= 2;
      degree_list.push_back(val);
    }
    //printf("last_dir 0x%x present_dir 0x%x rad 0x%d\n", last_dir.byte, present_dir.byte, degree_list.back());
    last_dir = present_dir;
  }
  return degree_list;
}
Operation get_turn_direction(const Direction& last_dir, const Direction& present_dir){
  Operation op;
  Direction first_dir, second_dir;
  if(bit_count(last_dir) == 1){
    first_dir = last_dir;
    second_dir = present_dir - last_dir;
  }
  if(bit_count(last_dir) == 2){
    if(bit_count(present_dir) == 1){
      first_dir = present_dir - last_dir;
      second_dir = present_dir;
    }
    else{
      first_dir = last_dir & present_dir;
      second_dir = present_dir - first_dir;
    }
  }
  if(first_dir > second_dir){
    if(first_dir == SOUTH && second_dir == EAST)  op.op = Operation::TURN_LEFT;
    else op.op = Operation::TURN_RIGHT;
  }
  else{
    if(first_dir == EAST && second_dir == SOUTH)  op.op = Operation::TURN_RIGHT;
    else op.op = Operation::TURN_LEFT;
  }
  return op;
}
std::vector<Direction> generateDirectionList(const GeneralIndexVector& index_vector){
  std::vector<Direction> dir_list;
  dir_list.reserve(64);
  auto itr = index_vector.begin();
  auto last_id = *itr;
  itr++;
  for(; itr != index_vector.end(); itr++){
    //printf("last_node id is %d\n", last_node.get_my_id());
    auto present_id = *itr;
    //printf("present_node id is %d\n", present_node.get_my_id());
    Direction dir = node_relation(last_id, present_id);
    //printf("dir is 0x%x\n", dir);
    dir_list.push_back(dir);
    last_id = present_id;
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
    else if(runSequence[i].op == Operation::TURN_LEFT180 ) printf("TURN_LEFT180");
    else if(runSequence[i].op == Operation::TURN_RIGHT90S) printf("TURN_RIGHT90S");
    else if(runSequence[i].op == Operation::TURN_LEFT90S ) printf("TURN_LEFT90S");
    else if(runSequence[i].op == Operation::V_LEFT90     ) printf("LEFT_V90");
    else if(runSequence[i].op == Operation::V_RIGHT90    ) printf("RIGHT_V90");
    else if(runSequence[i].op == Operation::TURN_135     ) printf("TURN_135");
    else if(runSequence[i].op == Operation::V_90) printf("V90");
    else if(runSequence[i].op == Operation::TURN_45) printf("TURN_45");
    else if(runSequence[i].op == Operation::BACK_180) printf("BACK_180");
    else printf("wrong!");
    printf(" -> %d\n", runSequence[i].n);
  }
}

int32_t calc_id_from_operation(const int32_t& currentID, const Direction& dir, const Operation& nextOP){
  //NORTH
  if(nextOP.op == Operation::FORWARD){
    int32_t offset = 0;
    if(dir == NORTH || dir == SOUTH)  offset = (dir == NORTH) ? MAZE_SIZE * 2 : -MAZE_SIZE * 2;
    else if(dir == WEST || dir == EAST)  offset = (dir == EAST) ? 2 : -2;
    return currentID + offset * nextOP.n;
  }
  else if(nextOP.op == Operation::BACK_180) return currentID;
  else if(nextOP.op == Operation::TURN_RIGHT90S || nextOP.op == Operation::TURN_RIGHT90){
    if(dir == NORTH)  return currentID + MAZE_SIZE * 2 + 1;
    else if(dir == SOUTH || dir == WEST)  return currentID - 1;
    else if(dir == EAST)  return currentID + 1 - MAZE_SIZE * 2;
  }
  else if(nextOP.op == Operation::TURN_LEFT90S || nextOP.op == Operation::TURN_LEFT90){
    if(dir == NORTH)  return currentID + MAZE_SIZE * 2 - 1;
    else if(dir == SOUTH || dir == EAST)  return currentID + 1;
    else if(dir == WEST)  return currentID - 1 - MAZE_SIZE * 2;
  }
  //if nothing
  printf("error!\n");
  return currentID;
}
Direction calc_dir_from_operation(const Direction& dir, const Operation& nextOP){
  if(nextOP.op == Operation::FORWARD) return dir;
  else if(nextOP.op == Operation::TURN_RIGHT90S || nextOP.op == Operation::TURN_RIGHT90){
    if(dir == NORTH)  return EAST;
    else if(dir == EAST) return SOUTH;
    else if(dir == SOUTH) return WEST;
    else if(dir == WEST) return NORTH;
  }
  else if(nextOP.op == Operation::TURN_LEFT90S || nextOP.op == Operation::TURN_LEFT90){
    if(dir == NORTH)  return WEST;
    else if(dir == WEST) return SOUTH;
    else if(dir == SOUTH) return EAST;
    else if(dir == EAST) return NORTH;
  }
  else if(nextOP.op == Operation::BACK_180){
    if(dir == NORTH)  return SOUTH;
    else if(dir == SOUTH) return NORTH;
    else if(dir == EAST) return WEST;
    else if(dir == WEST) return EAST;
  }
  //if nothing
  printf("error\n");
  return dir;
}
