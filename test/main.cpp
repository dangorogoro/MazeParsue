#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include <list>
#include <vector>
#include <unistd.h>

#include "Parsue_Conf.h"
#include "Maze.h"


int main(){
  Maze maze(WallData);
  Node node;
  /*
  auto poi = node.search_neighbor_node(0);
  while(!poi.empty()){
    printf("po");
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }
  */
  node.start_edge_map(0,142);
  auto po = node.get_node(142).get_mother_id();
  while(1){
    printf("mother id is %d\n", po);
    if(po == 0) break;
    po = node.get_node(po).get_mother_id();
  }
  /*
  poi = node.search_neighbor_node(1);
  while(!poi.empty()){
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }
  poi = node.search_neighbor_node(63);
  while(!poi.empty()){
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }
  */

  /*
  uint32_t po = maze.getWall(0,0);
  printf("%x\n", po);
  maze.updateWall(0,0, 0xff);
  po = maze.getWall(0,0);
  */
  return 0;
}
