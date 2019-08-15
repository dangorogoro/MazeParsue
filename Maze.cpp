#include "Maze.h"
WallStr<WALL_AMOUNT> WallData;

Direction Maze::getWall(int8_t x, int8_t y){
  Direction dir;
  dir = getEAST(x,y) | getNORTH(x,y) | getWEST(x,y) | getSOUTH(x,y);
  return dir;
}
Direction Maze::getNORTH(int8_t x, int8_t y){
  Direction dir;
  //NORTH
  uint8_t state = wall_pointer->test(NORTH_STATE(x, y));
  uint8_t visible = wall_pointer->test(NORTH_VISIBLE(x, y));
  dir |= (state ? NORTH : 0) | (visible ? DONE_NORTH : 0);
  return dir;
}
Direction Maze::getEAST(int8_t x, int8_t y){
  Direction dir;
  //EAST
  if(x >= MAZE_SIZE - 1) dir = EAST | DONE_EAST;
  else{
    uint8_t state = wall_pointer->test(EAST_STATE(x, y));
    uint8_t visible = wall_pointer->test(EAST_VISIBLE(x, y));
    dir |= (state ? EAST : 0) | (visible ? DONE_EAST: 0);
  }
  return dir;
}

Direction Maze::getWEST(int8_t x, int8_t y){
  if(x <= 0) return WEST | DONE_WEST;
  else return getEAST(x - 1, y) << 0x02;
}
Direction Maze::getSOUTH(int8_t x, int8_t y){
  if(y <= 0) return SOUTH | DONE_SOUTH;
  else return getNORTH(x, y - 1) << 0x02;
}
void Maze::updateWall(int8_t x, int8_t y, Direction dir, bool forceWrite){
  Direction present_wall = getWall(x, y);
  if(!forceWrite && present_wall.isDoneAll()) return;
  if(y < MAZE_SIZE - 1){
    if((dir & NORTH) > 0)
      wall_pointer->set(NORTH_STATE(x,y), 1);
    else 
      wall_pointer->set(NORTH_STATE(x,y), 0);
    wall_pointer->set(NORTH_VISIBLE(x,y), 1);
  }
  //SOUTH
  if(y > 0){
    if((dir & SOUTH) > 0)
      wall_pointer->set(SOUTH_STATE(x,y), 1);
    else 
      wall_pointer->set(SOUTH_STATE(x,y), 0);
    wall_pointer->set(SOUTH_VISIBLE(x,y), 1);
  }
  //EAST
  if(x < MAZE_SIZE - 1){
    if((dir & EAST) > 0)
      wall_pointer->set(EAST_STATE(x,y), 1);
    else 
      wall_pointer->set(EAST_STATE(x,y), 0);
    wall_pointer->set(EAST_VISIBLE(x,y), 1);
  }
  //WEST
  if(x > 0){
    if((dir & WEST) > 0)
      wall_pointer->set(WEST_STATE(x,y), 1);
    else 
      wall_pointer->set(WEST_STATE(x,y), 0);
    wall_pointer->set(WEST_VISIBLE(x,y), 1);
  }
}
/*
std::priority_queue<NodeInfo> Node::search_neighbor_node(uint32_t present_number){
  std::priority_queue<NodeInfo> list;
  //Also return the edge node
  //->To check corner
  //NORTH
  if(present_number % 2 == 0){
    if(present_number < WALL_AMOUNT - 2 * MAZE_SIZE){
      //NORTH
      list.push(node[present_number + 2 * MAZE_SIZE);
      //NE
      list.push(node[present_number + 2 * MAZE_SIZE + 1);
      //NW
      if(present_number % (2 * MAZE_SIZE) != 0)
        list.push(node[present_number + 2 * MAZE_SIZE - 1);
    }
    if(present_number > 2 * MAZE_SIZE - 1){
      //SOUTH
      list.push(node[present_number - 2 * MAZE_SIZE);
    }
    //SE
    list.push(node[present_number + 1);
    //SW
    if(MAZE_SIZE % (2 * MAZE_SIZE) != 0)
      list.push(node[present_number - 1);
  }
  //EAST
  else{
    if(present_number % (2 * MAZE_SIZE) != 2 * MAZE_SIZE - 1){
      //EAST
      list.push(node[present_number + 2);
      //NE
      list.push(node[present_number + 1);
      //SE
      if(present_number > 2 * MAZE_SIZE - 1)
        list.push(node[present_number + 1 - 2 * MAZE_SIZE);
    }
    if(present_number % 64 != 1){
      //WEST
      list.push(node[present_number - 2);
      //NW
      list.push(node[present_number - 1);
      if(present_number > 2 * MAZE_SIZE - 1)
        list.push(node[present_number - 1 - 2 * MAZE_SIZE);
    }
  }
  return list;
}
*/
NodeQueue<NodeInfo> Node::search_neighbor_node(uint32_t present_number){
  NodeQueue<NodeInfo> node_queue;
  //Also return the edge node
  //->To check corner
  //NORTH
  if(present_number % 2 == 0){
    if(present_number < WALL_AMOUNT - 2 * MAZE_SIZE){
      //NORTH
      node_queue.push(node[present_number + 2 * MAZE_SIZE]);
      //NE
      node_queue.push(node[present_number + 2 * MAZE_SIZE + 1]);
      //NW
      if(present_number % (2 * MAZE_SIZE) != 0)
        node_queue.push(node[present_number + 2 * MAZE_SIZE - 1]);
    }
    if(present_number > 2 * MAZE_SIZE - 1){
      //SOUTH
      node_queue.push(node[present_number - 2 * MAZE_SIZE]);
    }
    //SE
    node_queue.push(node[present_number + 1]);
    //SW
    if(present_number % (2 * MAZE_SIZE) != 0)
      node_queue.push(node[present_number - 1]);
  }
  //EAST
  else{
    if(present_number % (2 * MAZE_SIZE) != 2 * MAZE_SIZE - 1){
      //EAST
      node_queue.push(node[present_number + 2]);
      //NE
      node_queue.push(node[present_number + 1]);
      //SE
      if(present_number > 2 * MAZE_SIZE - 1)
        node_queue.push(node[present_number + 1 - 2 * MAZE_SIZE]);
    }
    //NW
    node_queue.push(node[present_number - 1]);
    if(present_number % 64 != 1){
      //WEST
      node_queue.push(node[present_number - 2]);
    }
    //SW
    if(present_number > 2 * MAZE_SIZE - 1)
      node_queue.push(node[present_number - 1 - 2 * MAZE_SIZE]);
  }
  return check_queue_quality(node_queue);
}
void Node::start_edge_map(uint32_t start_id, uint32_t end_id){
  node[start_id].set_info(0, 0.0f);
  //auto node_queue = queueTask(start_id);
  auto node_queue = search_neighbor_node(start_id);
  node_queue = updateQueue(node_queue, start_id);
  bool flag = false;
  while(flag == false){
    while(!node_queue.empty()){
      NodeInfo top_node = node_queue.top();
      printf("node id is %d\n", top_node.get_my_id());
      node_queue.pop();

      auto new_queue = search_neighbor_node(top_node.get_my_id());
      new_queue = updateQueue(new_queue, top_node.get_my_id());

      node_queue = expandQueue(node_queue, new_queue);
      flag = node_check(node_queue, end_id);
      if(flag == true){
        printf("search was finished\n");
        break;
      }
    }
  }
}
NodeQueue<NodeInfo> Node::check_queue_quality(NodeQueue<NodeInfo> target_queue){
  NodeQueue<NodeInfo> que;
  while(!target_queue.empty()){
    NodeInfo top_node = target_queue.top();
    uint32_t top_id = top_node.get_my_id();
    printf("top_id is %d\n", top_id);
    if(node[top_id].get_wall_info() == 0 && !node[top_id].isCorner() && node[top_id].get_cost() > 5000){
      printf("accept\n");
      que.push(top_node);
    }
    target_queue.pop();
  }
  printf("check finished\n");
  return que;
}
void Node::updateNodeInfo(NodeQueue<NodeInfo> node_queue, bool force_flag){
  while(!node_queue.empty()){
    NodeInfo top_node = node_queue.top();
    printf("serial_number %d, cost %f\n", top_node.get_my_id(), top_node.get_cost());
    updateNodeInfo(top_node, force_flag);
    node_queue.pop();
  }
}
void Node::updateNodeInfo(NodeInfo target_node, bool force_flag){
  //update node info in the Node
  uint32_t id = target_node.get_my_id();
  if(force_flag == false){
    if(node[id].get_cost() > target_node.get_cost())
      node[id] = target_node;
  }
  else
    node[id] = target_node;
}
NodeQueue<NodeInfo> Node::updateQueue(NodeQueue<NodeInfo> node_queue, uint32_t mother_id){
  //update cost of NodeInfo
  //set mother id to node in thr priority queue
  NodeQueue<NodeInfo> return_queue;
  float cost = 0.0;
  while(!node_queue.empty()){
    cost += 0.1;
    NodeInfo top_node = node_queue.top();
    /* check present cost to determine whether to update
     *
     *
     */
    if(node[top_node.get_my_id()].get_cost() > node[mother_id].get_cost() + cost){
      top_node.set_info(mother_id, node[mother_id].get_cost() + cost);
      node[top_node.get_my_id()] = top_node;
      return_queue.push(top_node);
      printf("serial_number %d, cost %f mother %d\n", top_node.get_my_id(), top_node.get_cost(), top_node.get_mother_id());
    }
    node_queue.pop();
  }
  return return_queue;
}
NodeQueue<NodeInfo> Node::expandQueue(NodeQueue<NodeInfo> src_queue, NodeQueue<NodeInfo>dst_queue){
  while(!dst_queue.empty()){
    src_queue.push(dst_queue.top());
    dst_queue.pop();
  }
  return src_queue;
}

NodeQueue<NodeInfo> Node::queueTask(uint32_t start_id){
  //updateNodeInfo(node_queue);
  auto node_queue = search_neighbor_node(start_id);
  printf("hoge\n");
  node_queue = updateQueue(node_queue, start_id);
  return node_queue;
}
void node_debug(NodeQueue<NodeInfo> poi){
  printf("Node Debug\n");
  while(!poi.empty()){
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }
}
bool node_check(NodeQueue<NodeInfo> node_queue, uint32_t end_id){
  while(!node_queue.empty()){
    NodeInfo hoge = node_queue.top();
    if(hoge.get_my_id() == end_id)
      return true;
    node_queue.pop();
  }
  return false;
}
/*
   void Node::update_queue(std::priority_queue<uint32_t, std::vector<uint32_t>, std::greater<uint32_t>> queue){
   }
   */
