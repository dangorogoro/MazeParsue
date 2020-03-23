#include "Maze.h"
#include <array>
WallStr<WALL_AMOUNT> WallData;
std::chrono::system_clock::time_point start_chrono, end_chrono;
void Maze::updateWall(int8_t x, int8_t y, Direction dir, bool forceWrite){
  Direction present_wall = getWall(x, y);
  if(!forceWrite && present_wall.isDoneAll()) return;
  if((uint8_t)y < MAZE_SIZE - 1){
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
  if((uint8_t)x < MAZE_SIZE - 1){
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
void Maze::printWall(){
	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (uint8_t x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
			//if(getWall(x,y).bits.North) std::printf("----");
			//else std::printf("    ");
			if(getWall(x,y).bits.North) std::printf("-----");
      else std::printf("     ");
		}
		std::printf("+\r\n");
		for (uint8_t x=0;x<MAZE_SIZE;x++) {
      if(x == 0){
        if (getWall(x, y).bits.West) std::printf("|  ");
      }
			else{
        std::printf(" ");
        if(getWall(x,y).bits.West) std::printf("  |  ");
        else  std::printf("     ");
      }
		}
		std::printf("  |\r\n");
	}
	for (uint8_t i=0;i<MAZE_SIZE;i++) {
		std::printf("------");
	}
	std::printf("+\r\n");
}
void Maze::printWall(const bool value[MAZE_SIZE][MAZE_SIZE]){
	bool printValueOn = false;
	if (value) printValueOn = true;

	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (uint8_t x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
			if(getWall(x,y).bits.North) std::printf("----");
			else std::printf("    ");
		}
		std::printf("+\r\n");

		for (uint8_t x=0;x<MAZE_SIZE;x++) {
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
	for (uint8_t i=0;i<MAZE_SIZE;i++) {
		std::printf("-----");
	}
	std::printf("+\r\n");
}
void Maze::printWall(const Node& node){
	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (uint8_t x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
			//if(getWall(x,y).bits.North) std::printf("----");
			//else std::printf("    ");
			if(getWall(x,y).bits.North) std::printf("-----");
      else if(node.get_node(2 * MAZE_SIZE * y + 2 * x).get_cost() == 65535)  std::printf("     ");
      else std::printf("%5d", node.get_node(2 * MAZE_SIZE * y + 2 * x).get_cost());
		}
		std::printf("+\r\n");

		for (uint8_t x=0;x<MAZE_SIZE;x++) {
      if(x == 0){
        if (getWall(x, y).bits.West) std::printf("|  ");
      }
			else{
        std::printf(" ");
        if(getWall(x,y).bits.West) std::printf("  |  ");
        else if(node.get_node(2 * MAZE_SIZE * y + 2 * x - 1).get_cost() == 65535)  std::printf("     ");
        else  std::printf("%5d", node.get_node(2 * MAZE_SIZE * y + 2 * x - 1).get_cost());
      }
		}
		std::printf("  |\r\n");
	}
	for (uint8_t i=0;i<MAZE_SIZE;i++) {
		std::printf("------");
	}
	std::printf("+\r\n");
}
  
void Maze::printWall(std::priority_queue<int32_t> id_queue,const std::set<int32_t>& goal_set){
	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (uint8_t x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
      bool flag = false;
			if(getWall(x,y).bits.North){
        std::printf("-----");
        flag = true;
      }
			else{
        auto tmp_queue = id_queue;
        while(!tmp_queue.empty()){
          if((uint32_t)tmp_queue.top() == 2 * MAZE_SIZE * y + 2 * x){
            printf("\x1b[31m");     /* 前景色を赤に */
            printf("  *  ");
            printf("\x1b[39m");     /* 前景色をデフォルトに戻す */
            flag = true;
            break;
          }
          else if((uint32_t)tmp_queue.top() < 2 * MAZE_SIZE * y + 2 * x)  break;
          tmp_queue.pop();
        }
      }
      if(flag == false){
        if(goal_set.find(2 * MAZE_SIZE * y + 2 * x) != goal_set.end())  printf("\x1b[45m");     /* 背景色をマゼンタに */
        std::printf("     ");
        printf("\x1b[49m");     /* 背景色をデフォルトに */
      }
		}
		std::printf("+\r\n");

		for (uint8_t x=0;x<MAZE_SIZE;x++) {
      bool flag = false;
			if(getWall(x, y).bits.West){
        std::printf("|");
        flag = true;
      }
			else{
        auto tmp_queue = id_queue;
        while(!tmp_queue.empty()){
          if((uint32_t)tmp_queue.top() == 2 * MAZE_SIZE * y + 2 * x - 1){
            printf("\x1b[31m");     /* 前景色を赤に */
            printf("*");
            printf("\x1b[39m");     /* 前景色をデフォルトに戻す */
            flag = true;
            break;
          }
          else if((uint32_t)tmp_queue.top() < 2 * MAZE_SIZE * y + 2 * x - 1)  break;
          tmp_queue.pop();
        }
      }
      if(flag == false){
        if(goal_set.find(2 * MAZE_SIZE * y + 2 * x - 1) != goal_set.end())  printf("\x1b[45m");     /* 背景色をマゼンタに */
        std::printf(" ");
        printf("\x1b[49m");     /* 背景色をデフォルトに */
      }
			std::printf("     ");
		}
		std::printf("|\r\n");
	}
	for (uint8_t i=0;i<MAZE_SIZE;i++) {
		std::printf("------");
	}
	std::printf("+\r\n");
}
void Maze::printWall(std::priority_queue<int32_t> id_queue){
	for (int y=MAZE_SIZE-1;y>=0;y--) {
		for (uint8_t x=0;x<MAZE_SIZE;x++) {
			std::printf("+");
      bool flag = false;
			if(getWall(x,y).bits.North){
        std::printf("-----");
        flag = true;
      }
			else{
        auto tmp_queue = id_queue;
        while(!tmp_queue.empty()){
          if((uint32_t)tmp_queue.top() == 2 * MAZE_SIZE * y + 2 * x){
            printf("\x1b[31m");     /* 前景色を赤に */
            printf("  *  ");
            printf("\x1b[39m");     /* 前景色をデフォルトに戻す */
            flag = true;
            break;
          }
          else if((uint32_t)tmp_queue.top() < 2 * MAZE_SIZE * y + 2 * x)  break;
          tmp_queue.pop();
        }
      }
      if(flag == false) std::printf("     ");
		}
		std::printf("+\r\n");

		for (uint8_t x=0;x<MAZE_SIZE;x++) {
      bool flag = false;
			if(getWall(x, y).bits.West){
        std::printf("|");
        flag = true;
      }
			else{
        auto tmp_queue = id_queue;
        while(!tmp_queue.empty()){
          if((uint32_t)tmp_queue.top() == 2 * MAZE_SIZE * y + 2 * x - 1){
            printf("\x1b[31m");     /* 前景色を赤に */
            printf("*");
            printf("\x1b[39m");     /* 前景色をデフォルトに戻す */
            flag = true;
            break;
          }
          else if((uint32_t)tmp_queue.top() < 2 * MAZE_SIZE * y + 2 * x - 1)  break;
          tmp_queue.pop();
        }
      }
      if(flag == false){
        std::printf(" ");
      }
			std::printf("     ");
		}
		std::printf("|\r\n");
	}
	for (uint8_t i=0;i<MAZE_SIZE;i++) {
		std::printf("------");
	}
	std::printf("+\r\n");
}
void Maze::printWall(NodeQueue<NodeIndex> node_queue){
  std::priority_queue<int32_t> id_queue;
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
NeighborIndexArray Node::getDirectedNeighborNode(const int32_t& present_number, const Direction& dir, bool visible){
  NeighborIndexArray neighbor_array(node);
  std::array<int32_t, 6> north_edge = {-1, 0, 1};
  std::array<int32_t, 6> east_edge = {1, 1, 2};
  auto& edge_array = (present_number % 2 == 0) ? north_edge : east_edge;
  if((dir == NORTH) || (dir == SOUTH)){
    int32_t offset = (dir == NORTH) ? MAZE_SIZE * 2 : - MAZE_SIZE * 2;
    for(auto itr = edge_array.begin(); itr != edge_array.end(); itr++) *itr += offset;
  }
  else if((dir == EAST) || (dir == WEST)){
    if(dir == WEST) for(auto itr = edge_array.begin(); itr != edge_array.end(); itr++) *itr = -*itr;
    edge_array[0] -= MAZE_SIZE * 2;
  }
  for(auto itr = edge_array.begin(); itr != edge_array.end(); itr++){
    auto target_id = present_number + *itr;
    auto current_index = neighbor_array.get_correct_size();
    if(target_id < (int32_t)WALL_AMOUNT && target_id >= 0 && checkQueueQuality(target_id, visible)){
      neighbor_array.set(current_index, target_id);
      neighbor_array.increment_correct_size();
    }
  }
  return neighbor_array;
}
inline NeighborIndexArray Node::getNeighborNode(const int32_t& present_number, bool visible){
  NeighborIndexArray neighbor_array(node);
  //Also return the edge node
  //->To check corner
  //サイズは4でもいける気がする
  constexpr std::array<int32_t, 6> north_edge = {-2 * (int16_t)MAZE_SIZE, -1, 1, 2 * (int16_t)MAZE_SIZE - 1, 2 * (int16_t)MAZE_SIZE, 2 * (int16_t)MAZE_SIZE + 1};
  constexpr std::array<int32_t, 6> east_edge = {-2 * (int16_t)MAZE_SIZE - 1 , -2 * (int16_t)MAZE_SIZE + 1, -2, -1, 1, 2};
  auto& edge_array = (present_number % 2 == 0) ? north_edge : east_edge;
  for(auto itr = edge_array.begin(); itr != edge_array.end(); itr++){
    auto target_id = present_number + *itr;
    auto current_index = neighbor_array.get_correct_size();
    if(target_id < (int32_t)WALL_AMOUNT && target_id >= 0 && checkQueueQuality(target_id, visible)){
      neighbor_array.set(current_index, target_id);
      neighbor_array.increment_correct_size();
    }
  }
  return neighbor_array;
}
int32_t Node::startEdgeMap(const int32_t& start_id, const std::set<int32_t>& end_set, bool visible){
  //start_chrono = std::chrono::system_clock::now();
  clear();
  NodeQueue<NodeIndex> node_queue;
  node[start_id].set_info(start_id, 0);
  auto index_array = getNeighborNode(start_id, visible);
  updateQueue(node_queue, index_array, start_id);
  int32_t flag = node_check(index_array, end_set);
  while(flag < 0){
    while(!node_queue.empty()){
      //printf("queue size is %d \n", node_queue.size());
      NodeIndex top_index = node_queue.top();
      node_queue.pop();
      auto new_list = getNeighborNode(top_index.get_my_id(), visible);
      updateQueue(node_queue, new_list, top_index.get_my_id());
      flag = node_check(new_list, end_set);
      if(flag >= 0)  break;
    }
  }
  //end_chrono = std::chrono::system_clock::now();
  //double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_chrono - start_chrono).count() / 1000.0);
  //printf("time %lf[ms]\n", time);
  return flag;
}
int32_t Node::startEdgeMap(const int32_t &start_id, const int32_t &end_id, bool visible){
  std::set<int32_t> id_set;
  id_set.insert(end_id);
  //return startEdgeMap(start_id, id_set, visible);
  return startPureEdgeMap(start_id, id_set, visible);
}
int32_t Node::startPureEdgeMap(const int32_t &start_id, const std::set<int32_t>& end_set, bool visible){
  //start_chrono = std::chrono::system_clock::now();
  clear();
  NodePureQueue<NodeIndex> node_queue;
  node[start_id].set_info(start_id, 0);
  auto index_array = getNeighborNode(start_id, visible);
  updateQueue(node_queue, index_array, start_id);
  int32_t flag = node_check(index_array, end_set);
  while(flag < 0){
    if(node_queue.empty() == true) return -1;
    while(!node_queue.empty()){
      //printf("queue size is %d \n", node_queue.size());
      NodeIndex top_index = node_queue.front();
      node_queue.pop();
      auto new_list = getNeighborNode(top_index.get_my_id(), visible);
      updateQueue(node_queue, new_list, top_index.get_my_id());
      flag = node_check(new_list, end_set);
      if(flag >= 0)  break;
      if(node_queue.empty() == true) return -1;
    }
  }
  //end_chrono = std::chrono::system_clock::now();
  //double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_chrono - start_chrono).count() / 1000.0);
  //printf("time %lf[ms]\n", time);
  return flag;
}
int32_t Node::startFastestMap(const int32_t &start_id, const std::set<int32_t> &end_set, bool visible){
  clear();
  NodeQueue<NodeIndex> node_queue;
  node[start_id].set_info(start_id, 0, EdgeVec(NORTH, 0));
  auto index_array = getNeighborNode(start_id, visible);
  updateFastestQueue(node_queue, index_array, start_id);
  int32_t flag = node_check(index_array, end_set);
  while(flag < 0){
    while(!node_queue.empty()){
      NodeIndex top_index = node_queue.top();
      node_queue.pop();

      auto new_list = getNeighborNode(top_index.get_my_id(), visible);
      updateFastestQueue(node_queue, new_list, top_index.get_my_id());
      flag = node_check(new_list, end_set);
      if(flag >= 0) break;
    }
  }
  return flag;
}
int32_t Node::startFastestMap(const int32_t &start_id, const int32_t &end_id, bool visible){
  std::set<int32_t> id_set;
  id_set.insert(end_id);
  return startFastestMap(start_id, id_set, visible);
}

NodeQueue<NodeIndex> Node::getPathQueue(const int32_t& start_id, const int32_t& end_id){
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
std::set<int32_t> Node::getUnknownFastestWall(const int32_t &start_id, const int32_t &end_id){
  startEdgeMap(start_id, end_id);
  //startFastestMap(start_id, end_id);
  auto path_queue = getPathQueue(start_id, end_id);
  std::set<int32_t> check_set;
  while(!path_queue.empty()){
    NodeIndex top_node = path_queue.top();
    int32_t top_id = top_node.get_my_id();
    if(node[top_id].get_wall_visible() == false)  check_set.insert(top_id);
    path_queue.pop();
  }
  return check_set;
}

inline bool Node::checkQueueQuality(const int32_t& top_id, bool visible){
  return  !((node[top_id].get_wall_state() == 1 || node[top_id].isCorner()) || ((visible == true && node[top_id].get_wall_visible() == false) || (node[top_id].get_cost() != 65535)));
}
void Node::checkQueueQuality(std::vector<NodeIndex> &target_list, bool visible){
  auto removeIt = remove_if(target_list.begin(), target_list.end(), [&](NodeIndex top_node) {
      int32_t top_id = top_node.get_my_id();
      return  ((node[top_id].get_wall_state() == 1 || node[top_id].isCorner()) || ((visible == true && node[top_id].get_wall_visible() == false) || (node[top_id].get_cost() != 65535)));
  });
  target_list.erase(removeIt, target_list.end());
}
void Node::updateFastestQueue(NodeQueue<NodeIndex> &node_queue, const NeighborIndexArray &neighbor_array, const int32_t& mother_id){
  EdgeVec motherEdge = node[mother_id].get_edgeVec();
  for(int i = 0; i < neighbor_array.get_correct_size(); i++){
    NodeIndex top_index = neighbor_array.get_node_array_index(i);
    EdgeVec nextEdge;
    uint32_t cost;
    nextEdge.dir = node_relation(mother_id, top_index.get_my_id());
    if(nextEdge.dir == motherEdge.dir){
      nextEdge.cnt = motherEdge.cnt + 1;
    }
    else if((uint8_t)(nextEdge.dir & motherEdge.dir) == 0) continue;

    /*
    if(nextEdge.cnt < 10)  cost = 100 - 5 * nextEdge.cnt;
    else  cost = 1;
    */
    cost = (bit_count(nextEdge.dir) == 2) ? cost_table.diff_get(nextEdge.cnt, true) : cost_table.diff_get(nextEdge.cnt, false);

    if(top_index.get_cost() > node[mother_id].get_cost() + cost){
      node[top_index.get_my_id()].set_info(mother_id, node[mother_id].get_cost() + cost, nextEdge);
      node_queue.push(top_index);
    }
  }
}
void Node::updateFastestQueue(NodeQueue<NodeIndex> &node_queue, const std::vector<NodeIndex> &node_vector, const int32_t& mother_id){
  EdgeVec motherEdge = node[mother_id].get_edgeVec();
  for(auto itr = node_vector.begin(); itr != node_vector.end(); itr++){
    NodeIndex top_index = *itr;
    EdgeVec nextEdge;
    uint32_t cost;
    nextEdge.dir = node_relation(mother_id, top_index.get_my_id());
    if(nextEdge.dir == motherEdge.dir){
      nextEdge.cnt = motherEdge.cnt + 1;
    }
    else if((uint8_t)(nextEdge.dir & motherEdge.dir) == 0) continue;

    cost = (bit_count(nextEdge.dir) == 2) ? cost_table.diff_get(nextEdge.cnt, true) : cost_table.diff_get(nextEdge.cnt, false);

    if(top_index.get_cost() > node[mother_id].get_cost() + cost){
      node[top_index.get_my_id()].set_info(mother_id, node[mother_id].get_cost() + cost, nextEdge);
      node_queue.push(top_index);
    }
  }
}
void Node::updateQueue(NodeQueue<NodeIndex> &node_queue, const std::vector<NodeIndex> &node_vector, const int32_t& mother_id){
  //update cost of NodeInfo
  //set mother id to node in the priority queue
  uint32_t cost = 1;
  for(auto itr = node_vector.begin(); itr != node_vector.end(); itr++){
    NodeIndex top_index = *itr;
    if(top_index.get_cost() > node[mother_id].get_cost() + cost){
      node[top_index.get_my_id()].set_info(mother_id, node[mother_id].get_cost() + cost);
      node_queue.push(top_index);
    }
  }
}
void Node::updateQueue(NodeQueue<NodeIndex> &node_queue, const NeighborIndexArray &neighbor_array, const int32_t& mother_id){
  //update cost of NodeInfo
  //set mother id to node in the priority queue
  uint32_t cost = 1;
  for(int32_t i = 0; i < neighbor_array.get_correct_size(); i++){
    NodeIndex top_index = NodeIndex(node, neighbor_array.at(i));
    if(top_index.get_cost() > node[mother_id].get_cost() + cost){
      node[top_index.get_my_id()].set_info(mother_id, node[mother_id].get_cost() + cost);
      node_queue.push(top_index);
    }
  }
}

void Node::updateQueue(NodePureQueue<NodeIndex> &node_queue, const NeighborIndexArray &neighbor_array, const int32_t& mother_id){
  //update cost of NodeInfo
  //set mother id to node in the priority queue
  uint32_t cost = 1;
  for(int32_t i = 0; i < neighbor_array.get_correct_size(); i++){
    NodeIndex top_index = NodeIndex(node, neighbor_array.at(i));
    if(top_index.get_cost() > node[mother_id].get_cost() + cost){
      node[top_index.get_my_id()].set_info(mother_id, node[mother_id].get_cost() + cost);
      node_queue.push(top_index);
    }
  }
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
/*
void node_debug(NodeQueue<NodeInfo> poi){
  printf("Node Debug\n");
  while(!poi.empty()){
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }
}
*/
int32_t node_check(const NeighborIndexArray& neighbor_array, const int32_t& end_id){
  std::set<int32_t> id_set;
  id_set.insert(end_id);
  return node_check(neighbor_array, id_set);
}
int32_t node_check(const NeighborIndexArray& neighbor_array, const std::set<int32_t>& end_set){
  for(int32_t i = 0; i < neighbor_array.get_correct_size(); i++){
    NodeIndex hoge = neighbor_array.get_node_array_index(i);
    if(end_set.find(hoge.get_my_id()) != end_set.end())
      return hoge.get_my_id();
  }
  return -1;
}

int32_t node_check(const std::vector<NodeIndex>& node_vector, const int32_t& end_id){
  std::set<int32_t> id_set;
  id_set.insert(end_id);
  return node_check(node_vector, id_set);
}
int32_t node_check(const std::vector<NodeIndex>& node_vector, const std::set<int32_t>& end_set){
  for(auto itr = node_vector.begin(); itr != node_vector.end(); itr++){
    NodeIndex hoge = *itr;
    if(end_set.find(hoge.get_my_id()) != end_set.end())
      return hoge.get_my_id();
  }
  return -1;
}

Direction node_relation(const int32_t &src_index, const int32_t &dst_index){
  int32_t index_diff = dst_index - src_index;
  Direction dir;
  //NORTH
  if(src_index % 2 == 0){
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
