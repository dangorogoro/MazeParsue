#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include <list>
#include <vector>
#include <unistd.h>
#include "Parsue_Conf.h"
#include "Maze.h"
#include "MazeData.h"
#include "Operation.h"
#include "Agent.h"
#include "Cost.h"
int main(){
  
  auto table = cost_table<uint32_t, 10>();
  //std::cout<<table.get(2)<<", "<<table.get(3)<<factorial<uint32_t, 10>(5)<<std::endl;
  for(size_t i = 0; i < 10; i++){
    std::cout<<table.get(i, true)<<",";
  }
  std::cout<<std::endl;
  for(size_t i = 0; i < 10; i++){
    std::cout<<table.get(i, false)<<",";
  }
  std::cout<<std::endl;
  return 0;
  Maze maze(WallData);
  Node node;
  Agent agent(maze, node);
  IndexVec po;
  Direction dir;
  auto sampleData = alljapan2011x_exp_fin;
  dir = sampleData[0 + 16 * (15 - 1)];
  agent.update(IndexVec(0,1), dir.byte | 0xf0);
  printf("============\n");
  po = agent.getNextIndex();
  dir = sampleData[po.x + 16 *(15 - po.y)];
  printf("wall is 0x%x\n", dir.byte);
  agent.update(po, dir.byte | 0xf0);
  while(1){

    po = agent.getNextIndex();
    dir = sampleData[po.x + 16 *(15 - po.y)];
    agent.update(po, dir.byte | 0xf0);
    usleep(10 * 10 * 1000);
    if(agent.getState() == Agent::FINISHED) break;
  }
  //node.startEdgeMap(0, GOAL, true);
  node.startFastestMap(0, GOAL, true);
  auto ans_path = node.getPathQueue(0, GOAL);
  maze.printWall(ans_path);
  //print_operation(loadPath(ans_path,1));
#if 0
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
#endif 
//  maze.loadFromArray(Data);
//  node.start_edge_map(6, 64 * 7 + 2 * 8);
//  auto ans_path = node.getPathQueue(6, 64 * 7 + 2 * 8 - 1);
//  maze.printWall(ans_path);
//  auto runSequence = loadPath(ans_path, 1);
//  print_operation(runSequence);
//  node.start_edge_map(65, 64 * 7 + 2 * 8);
//  ans_path = node.getPathQueue(65, 64 * 7 + 2 * 8 - 1);
//  maze.printWall(ans_path);

  /*
  printf("0x%x\n", node_relation(NodeIndex(&pi, 66), NodeIndex(&pi, 67)));
  printf("0x%x\n", node_relation(NodeIndex(&pi, 67), NodeIndex(&pi, 66)));
  printf("0x%x\n", node_relation(NodeIndex(&pi, 66), NodeIndex(&pi, 65)));
  printf("0x%x\n", node_relation(NodeIndex(&pi, 65), NodeIndex(&pi, 66)));
  */

  return 0;
}
