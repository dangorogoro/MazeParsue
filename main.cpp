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
int main(){
  
  /*
  auto table = CostTable<uint32_t, 30>();
  //std::cout<<table.get(2)<<", "<<table.get(3)<<factorial<uint32_t, 10>(5)<<std::endl;
  for(size_t i = 0; i < 30; i++){
    std::cout<<table.diff_get(i, true)<<",";
  }
  std::cout<<std::endl;
  for(size_t i = 0; i < 30; i++){
    std::cout<<table.diff_get(i, false)<<",";
  }
  std::cout<<std::endl;
  return 0;
  */

  Maze maze(WallData);
  Node node;
  maze.printWall();
  Agent agent(maze, node);
  IndexVec po;
  Direction dir;
  auto sampleData = alljapan2011x_exp_fin;
  //auto sampleData = H2019_East;
  dir = sampleData[0 + 16 * (15 - 1)];
  agent.futureCalc();
  agent.update(IndexVec(0,1), dir.byte | 0xf0);
  start_chrono = std::chrono::system_clock::now();
  while(1){

    po = agent.getNextIndex();
    dir = sampleData[po.x + 16 *(15 - po.y)];
    //なにかの事前計算
    agent.futureCalc();
    agent.update(po, dir.byte | 0xf0);
    //usleep(10 * 10 * 1000);
    //maze.printWall(node);
    if(agent.getState() == Agent::FINISHED) break;

  }
  end_chrono = std::chrono::system_clock::now();
  double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_chrono - start_chrono).count() / 1000.0);
  printf("time %lf[ms]\n", time);
  //node.startEdgeMap(0, GOAL, true);
  node.startFastestMap(0, GOAL, true);
  auto ans_path = node.getPathQueue(0, GOAL);
  maze.printWall(ans_path);
  maze.printWall(node);
  //print_operation(loadPath(ans_path,1));
  return 0;
}
