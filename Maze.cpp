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
void Maze::printWall(const uint8_t value[MAZE_SIZE][MAZE_SIZE]){
	bool printValueOn = false;
	if (value) printValueOn = true;

	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (int x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
			if(getWall(x, y).bits.North) std::printf("----");
			else std::printf("    ");
		}
		std::printf("+\r\n");

		for (int x=0;x<MAZE_SIZE;x++) {
			if (getWall(x, y).bits.West) std::printf("|");
			else std::printf(" ");
			std::printf(" ");
			if (printValueOn) std::printf("%3u", value[y][x]);
			else std::printf("   ");
		}
		std::printf("|\r\n");
	}
	for (int i=0;i<MAZE_SIZE;i++) {
		std::printf("-----");
	}
	std::printf("+\r\n");
}
void Maze::printWall(const bool value[MAZE_SIZE][MAZE_SIZE]){
	bool printValueOn = false;
	if (value) printValueOn = true;

	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (int x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
			if(getWall(x,y).bits.North) std::printf("----");
			else std::printf("    ");
		}
		std::printf("+\r\n");

		for (int x=0;x<MAZE_SIZE;x++) {
			if (getWall(x, y).bits.West) std::printf("|");
			else std::printf(" ");
			std::printf("  ");
			if (printValueOn){
				if (value[y][x]) std::printf("* ");
				else std::printf("  ");
			}
			else std::printf("   ");
		}
		std::printf("|\r\n");
	}
	for (int i=0;i<MAZE_SIZE;i++) {
		std::printf("-----");
	}
	std::printf("+\r\n");
}
void Maze::printWall(std::priority_queue<int> id_queue){

	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (int x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
      bool flag = false;
			if(getWall(x,y).bits.North){
        std::printf("-----");
        flag = true;
      }
			else{
        auto tmp_queue = id_queue;
        while(!tmp_queue.empty()){
          if(tmp_queue.top() == 2 * MAZE_SIZE * y + 2 * x){
            printf("\x1b[31m");     /* 前景色を赤に */
            printf("  *  ");
            printf("\x1b[39m");     /* 前景色をデフォルトに戻す */
            flag = true;
            break;
          }
          else if(tmp_queue.top() < 2 * MAZE_SIZE * y + 2 * x)  break;
          tmp_queue.pop();
        }
      }
      if(flag == false) std::printf("     ");
		}
		std::printf("+\r\n");

		for (int x=0;x<MAZE_SIZE;x++) {
      bool flag = false;
			if(getWall(x, y).bits.West){
        std::printf("|");
        flag = true;
      }
			else{
        auto tmp_queue = id_queue;
        while(!tmp_queue.empty()){
          if(tmp_queue.top() == 2 * MAZE_SIZE * y + 2 * x - 1){
            printf("\x1b[31m");     /* 前景色を赤に */
            printf("*");
            printf("\x1b[39m");     /* 前景色をデフォルトに戻す */
            flag = true;
            break;
          }
          else if(tmp_queue.top() < 2 * MAZE_SIZE * y + 2 * x - 1)  break;
          tmp_queue.pop();
        }
      }
      if(flag == false) std::printf(" ");
			std::printf("     ");
		}
		std::printf("|\r\n");
	}
	for (int i=0;i<MAZE_SIZE;i++) {
		std::printf("------");
	}
	std::printf("+\r\n");
}
void Maze::printWall(NodeQueue<NodeIndex> node_queue){
  std::priority_queue<int> id_queue;
  while(!node_queue.empty()){
    id_queue.push(node_queue.top().get_my_id());
    node_queue.pop();
  }
  printWall(id_queue);
}

  
  
void Maze::loadFromArray(uint8_t* array){
  uint32_t data_size = 256;
  for(size_t i = 0; i < data_size; i++){
      updateWall(i % (MAZE_SIZE / 2), MAZE_SIZE / 2  - i / (MAZE_SIZE / 2) - 1, Direction(array[i]));
  }
}

std::list<NodeIndex> Node::getNeighborNode(int32_t present_number, bool visible){
  std::list<NodeIndex> node_list;
  //Also return the edge node
  //->To check corner
  //NORTH
  if(present_number % 2 == 0){
    if(present_number < WALL_AMOUNT - 2 * MAZE_SIZE){
      //NORTH
      node_list.push_back(NodeIndex(node, present_number + 2 * MAZE_SIZE));
      //NE
      node_list.push_back(NodeIndex(node, present_number + 2 * MAZE_SIZE + 1));
      //NW
      if(present_number % (2 * MAZE_SIZE) != 0)
        node_list.push_back(NodeIndex(node, present_number + 2 * MAZE_SIZE - 1));
    }
    if(present_number > 2 * MAZE_SIZE - 1){
      //SOUTH
      node_list.push_back(NodeIndex(node, present_number - 2 * MAZE_SIZE));
    }
    //SE
    node_list.push_back(NodeIndex(node, present_number + 1));
    //SW
    if(present_number % (2 * MAZE_SIZE) != 0)
      node_list.push_back(NodeIndex(node, present_number - 1));
  }
  //EAST
  else{
    if(present_number % (2 * MAZE_SIZE) != 2 * MAZE_SIZE - 1){
      //EAST
      node_list.push_back(NodeIndex(node, present_number + 2));
      //NE
      node_list.push_back(NodeIndex(node, present_number + 1));
      //SE
      if(present_number > 2 * MAZE_SIZE - 1)
        node_list.push_back(NodeIndex(node, present_number + 1 - 2 * MAZE_SIZE));
    }
    //NW
    node_list.push_back(NodeIndex(node, present_number - 1));
    if(present_number % 64 != 1){
      //WEST
      node_list.push_back(NodeIndex(node, present_number - 2));
    }
    //SW
    if(present_number > 2 * MAZE_SIZE - 1)
      node_list.push_back(NodeIndex(node, present_number - 1 - 2 * MAZE_SIZE));
  }
  return checkQueueQuality(node_list, visible);
}

NodeQueue<NodeIndex> Node::search_neighbor_node(int32_t present_number, bool visible){
  NodeQueue<NodeIndex> node_queue;
  //Also return the edge node
  //->To check corner
  //NORTH
  if(present_number % 2 == 0){
    if(present_number < WALL_AMOUNT - 2 * MAZE_SIZE){
      //NORTH
      node_queue.push(NodeIndex(node, present_number + 2 * MAZE_SIZE));
      //NE
      node_queue.push(NodeIndex(node, present_number + 2 * MAZE_SIZE + 1));
      //NW
      if(present_number % (2 * MAZE_SIZE) != 0)
        node_queue.push(NodeIndex(node, present_number + 2 * MAZE_SIZE - 1));
    }
    if(present_number > 2 * MAZE_SIZE - 1){
      //SOUTH
      node_queue.push(NodeIndex(node, present_number - 2 * MAZE_SIZE));
    }
    //SE
    node_queue.push(NodeIndex(node, present_number + 1));
    //SW
    if(present_number % (2 * MAZE_SIZE) != 0)
      node_queue.push(NodeIndex(node, present_number - 1));
  }
  //EAST
  else{
    if(present_number % (2 * MAZE_SIZE) != 2 * MAZE_SIZE - 1){
      //EAST
      node_queue.push(NodeIndex(node, present_number + 2));
      //NE
      node_queue.push(NodeIndex(node, present_number + 1));
      //SE
      if(present_number > 2 * MAZE_SIZE - 1)
        node_queue.push(NodeIndex(node, present_number + 1 - 2 * MAZE_SIZE));
    }
    //NW
    node_queue.push(NodeIndex(node, present_number - 1));
    if(present_number % 64 != 1){
      //WEST
      node_queue.push(NodeIndex(node, present_number - 2));
    }
    //SW
    if(present_number > 2 * MAZE_SIZE - 1)
      node_queue.push(NodeIndex(node, present_number - 1 - 2 * MAZE_SIZE));
  }
  return check_queue_quality(node_queue, visible);
}
void Node::startEdgeMap(int32_t start_id, int32_t end_id, bool visible){
  clear();
  NodeQueue<NodeIndex> node_queue;
  node[start_id].set_info(start_id, 0);
  auto node_list = getNeighborNode(start_id, visible);
  updateQueue(node_queue, node_list, start_id);
  bool flag = node_check(node_list, end_id);
  while(flag == false){
    while(!node_queue.empty()){
      NodeIndex top_index = node_queue.top();
      node_queue.pop();

      auto new_list = getNeighborNode(top_index.get_my_id(), visible);
      updateQueue(node_queue, new_list, top_index.get_my_id());
      flag = node_check(new_list, end_id);
      if(flag == true)  break;
    }
  }
}

void Node::start_edge_map(int32_t start_id, int32_t end_id, bool visible){
  clear();
  node[start_id].set_info(start_id, 0);
  //auto node_queue = queueTask(start_id);
  auto node_queue = search_neighbor_node(start_id, visible);
  node_queue = updateQueue(node_queue, start_id);
  bool flag = false;
  while(flag == false){
    while(!node_queue.empty()){
      NodeIndex top_index = node_queue.top();
      //printf("node id is %d\n", top_index.get_my_id());
      node_queue.pop();

      auto new_queue = search_neighbor_node(top_index.get_my_id(), visible);
      new_queue = updateQueue(new_queue, top_index.get_my_id());

      node_queue = expandQueue(node_queue, new_queue);
      flag = node_check(node_queue, end_id);
      if(flag == true){
        //printf("search was finished\n");
        break;
      }
    }
  }
}

NodeQueue<NodeIndex> Node::getPathQueue(int32_t start_id, int32_t end_id){
  NodeQueue<NodeIndex> node_queue;
  node_queue.push(NodeIndex(node, end_id));
  auto po = get_node(end_id).get_mother_id();
  while(1){
    //printf("mother id is %d\n", po);
    node_queue.push(NodeIndex(node, po));
    if(po == start_id) break;
    po = get_node(po).get_mother_id();
  }
  return node_queue;
}
NodeQueue<NodeIndex> Node::check_queue_quality(NodeQueue<NodeIndex> target_queue, bool visible){
  NodeQueue<NodeIndex> que;
  while(!target_queue.empty()){
    NodeIndex top_node = target_queue.top();
    int32_t top_id = top_node.get_my_id();
    //printf("top_id is %d\n", top_id);
    if(node[top_id].get_wall_state() == 0 && !node[top_id].isCorner() && node[top_id].get_cost() > 5000){
      if(visible == true && node[top_id].get_wall_visible() == false){
        //printf("wrong top id %d \n", top_id);
      }
      //if(visible == true && node[top_id].get_wall_visible() == false) continue;
      else  que.push(top_node);
    }
    target_queue.pop();
  }
  //printf("check finished\n");
  return que;
}
std::list<NodeIndex> Node::checkQueueQuality(const std::list<NodeIndex> &target_list, bool visible){
  std::list<NodeIndex> node_list;
  for(auto itr = target_list.begin(); itr != target_list.end(); itr++){
    auto top_node = *itr;
    int32_t top_id = top_node.get_my_id();
    if(node[top_id].get_wall_state() == 0 && !node[top_id].isCorner() && node[top_id].get_cost() > 5000){
      if(visible == true && node[top_id].get_wall_visible() == false){
        //printf("wrong top id %d \n", top_id);
      }
      //if(visible == true && node[top_id].get_wall_visible() == false) continue;
      else  node_list.push_back(top_node);
    }
  }
  return node_list;
}

NodeQueue<NodeIndex> Node::updateQueue(NodeQueue<NodeIndex> node_queue, int32_t mother_id){
  //update cost of NodeInfo
  //set mother id to node in the priority queue
  NodeQueue<NodeIndex> return_queue;
  uint32_t cost = 1;
  //printf("mother id %d\n", mother_id);
  while(!node_queue.empty()){
    NodeIndex top_index = node_queue.top();
    /* check present cost to determine whether to update
     *
     *
     */
    if(top_index.get_cost() > node[mother_id].get_cost() + cost){
      node[top_index.get_my_id()].set_info(mother_id, node[mother_id].get_cost() + cost);
      return_queue.push(top_index);
      //printf("serial_number %d, cost %f mother %d\n", top_index.get_my_id(), top_index.get_cost(), node[top_index.get_my_id()].get_mother_id());
    }
    node_queue.pop();
  }
  return return_queue;
}
void Node::updateQueue(NodeQueue<NodeIndex> &node_queue, const std::list<NodeIndex> &node_list, int32_t mother_id){
  uint32_t cost = 1;
  for(auto itr = node_list.begin(); itr != node_list.end(); itr++){
    NodeIndex top_index = *itr;
    if(top_index.get_cost() > node[mother_id].get_cost() + cost){
      node[top_index.get_my_id()].set_info(mother_id, node[mother_id].get_cost() + cost);
      node_queue.push(top_index);
      //printf("serial_number %d, cost %f mother %d\n", top_index.get_my_id(), top_index.get_cost(), node[top_index.get_my_id()].get_mother_id());
    }
  }
}
NodeQueue<NodeIndex> Node::expandQueue(NodeQueue<NodeIndex> src_queue, NodeQueue<NodeIndex> dst_queue){
  while(!dst_queue.empty()){
    src_queue.push(dst_queue.top());
    dst_queue.pop();
  }
  return src_queue;
}

/*
NodeQueue<NodeInfo> Node::queueTask(int32_t start_id){
  //updateNodeInfo(node_queue);
  auto node_queue = search_neighbor_node(start_id);
  printf("hoge\n");
  node_queue = updateQueue(node_queue, start_id);
  return node_queue;
}
*/
void node_debug(NodeQueue<NodeInfo> poi){
  printf("Node Debug\n");
  while(!poi.empty()){
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }
}
bool node_check(NodeQueue<NodeIndex> node_queue, int32_t end_id){
  while(!node_queue.empty()){
    NodeIndex hoge = node_queue.top();
    if(hoge.get_my_id() == end_id)
      return true;
    node_queue.pop();
  }
  return false;
}
bool node_check(std::list<NodeIndex> node_list, int32_t end_id){
  for(auto itr = node_list.begin(); itr != node_list.end(); itr++){
    NodeIndex hoge = *itr;
    if(hoge.get_my_id() == end_id)
      return true;
  }
  return false;
}

Direction node_relation(NodeIndex src_index, NodeIndex dst_index){
  int32_t index_diff = dst_index.get_my_id () - src_index.get_my_id();
  Direction dir;
  //NORTH
  if(src_index.get_my_id() % 2 == 0){
    //NORTH
    if(index_diff - 1 > 0) dir |= NORTH;
    //SOUTH
    else  dir |= SOUTH;
    //EAST
    if(index_diff % (2 * MAZE_SIZE) == 1) dir |= EAST;
    //WEST
    else if(index_diff % (2 * MAZE_SIZE) == (2 * MAZE_SIZE) - 1) dir |= WEST;
  }
  //EAST
  else{
    //EAST
    if(index_diff > 0 || std::abs(index_diff) == (2 * MAZE_SIZE - 1)) dir |= EAST;
    //WEST
    else dir |= WEST;
    //NORTH
    if(std::abs(index_diff) == 1) dir |= NORTH;
    //SOUTH
    else if((std::abs(index_diff) == 2 * MAZE_SIZE - 1) || (std::abs(index_diff) == 2 * MAZE_SIZE + 1))  dir |= SOUTH;
  }
  return dir;
}


/*
OperationList loadPath(NodeQueue<NodeIndex> node_queue, bool use_diagonal){

  NodeIndex last_node = node_queue.top();
  while(!node_queue.empty()){
    node_queue.pop();
    NodeIndex present_node = node_queue.top();
  }
}
*/
/*
   void Node::update_queue(std::priority_queue<uint32_t, std::vector<uint32_t>, std::greater<uint32_t>> queue){
   }
   */
