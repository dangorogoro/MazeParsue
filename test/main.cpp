#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include <list>
#include <vector>
#include <unistd.h>

#include "Parsue_Conf.h"
#include "Maze.h"
#include "Operation.h"
int main(){
  Maze maze(WallData);
  Node node;
  /*
  maze.updateWall(IndexVec(0, 2), 0xff);
  maze.updateWall(IndexVec(1, 3), SOUTH | WEST | NORTH);
  maze.updateWall(IndexVec(1, 2), NORTH);
  maze.updateWall(IndexVec(1, 4), NORTH);
  maze.updateWall(IndexVec(2, 4), NORTH);
  */
  maze.updateWall(IndexVec(1, 0), 0xff);
  maze.updateWall(IndexVec(1, 1), 0xff);
  maze.updateWall(IndexVec(3, 1), 0xff);
  maze.updateWall(IndexVec(3, 2), 0xff);
  node.start_edge_map(0,9);
  auto ans_path = node.getPathQueue(0,9);
  auto runSequence = loadPath(ans_path, 1);
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
    printf(" -> %d\n", runSequence[i].n);
  }


  /*
  printf("0x%x\n", node_relation(NodeIndex(&pi, 66), NodeIndex(&pi, 67)));
  printf("0x%x\n", node_relation(NodeIndex(&pi, 67), NodeIndex(&pi, 66)));
  printf("0x%x\n", node_relation(NodeIndex(&pi, 66), NodeIndex(&pi, 65)));
  printf("0x%x\n", node_relation(NodeIndex(&pi, 65), NodeIndex(&pi, 66)));
  */

  return 0;
}
