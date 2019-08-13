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
  auto poi = node.search_neighbor_node(62);
  while(!poi.empty()){
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }
  poi = node.search_neighbor_node(66);
  while(!poi.empty()){
    NodeInfo hoge = poi.top();
    hoge.debug_info();
    poi.pop();
  }

  uint32_t po = maze.getWall(0,0);
  printf("%x\n", po);
  maze.updateWall(0,0, 0xff);
  po = maze.getWall(0,0);
  return 0;
}

/*
#include "MazeSolver_conf.h"
#include "Maze.h"
#include "mazeData.h"
#include "ShortestPath.h"
#include "Agent.h"


void test_Size()
{
	printf("%lu \n", sizeof(Direction)); //1
	printf("%lu \n", sizeof(Maze)); //512
	printf("%lu \n", sizeof(ShortestPath)); //1112
	printf("%lu \n", sizeof(Path)); //24
	printf("%lu \n", sizeof(std::vector<Path>)); //24
	printf("%lu \n", sizeof(Agent)); //1168
}

void test_Maze(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_66test);
	field.printWall();
}

void test_ShortestPath(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_66test);

	ShortestPath path(field);
	for (int i=0;i<100000;i++) {
	path.calcShortestDistancePath(IndexVec(0,0), MAZE_GOAL_LIST, false);
	}
	bool route[MAZE_SIZE][MAZE_SIZE] = {false};
	for (auto index : path.getShortestDistancePath())
	{
		route[index.y][index.x] = true;
	}
	//field.printWall(route);
}

void test_KShortestPath(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_66test);

	ShortestPath path(field);
	path.calcKShortestDistancePath(IndexVec(0,0), MAZE_GOAL_LIST, 5, false);

	int cnt = 0;
	for (auto &p : path.getKShortestDistancePath()) {
		cnt++;
		printf("path#%d length %lu\n", cnt, p.size());
		bool route[MAZE_SIZE][MAZE_SIZE] = {false};
		for (auto &index : p) {
			route[index.y][index.x] = true;
		}
		field.printWall(route);
	}
	printf("found %lu route\n", path.getKShortestDistancePath().size());
}

void test_ShortestPathInTime(const char *filename)
{
	Maze field;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_maze);

	ShortestPath path(field,true);
	path.calcShortestTimePath(IndexVec(0,0), MAZE_GOAL_LIST, 20, false, true);

	auto &p = path.getShortestTimePath();
	printf("length %lu\n", p.size());
	printf("cost %f\n", path.getShortestTimePathCost());
	bool route[MAZE_SIZE][MAZE_SIZE] = {false};
	for (auto &index : p) {
		route[index.y][index.x] = true;
	}
	field.printWall(route);
}

void test_Agent(const char *filename)
{
	Maze field;
	Maze mazeInRobot;
	field.loadFromFile(filename);
	//field.loadFromArray(mazeData_66test);

	Agent agent(mazeInRobot);

	IndexVec cur(0,0);
	while(1) {
		bool pos[MAZE_SIZE][MAZE_SIZE] = {false};
		pos[cur.y][cur.x] = true;
		mazeInRobot.printWall(pos);

		agent.update(cur, field.getWall(cur));
		if (agent.getState() == Agent::FINISHED) break;

		Direction dir = agent.getNextDirection();
		for (int i=0;i<4;i++) {
			if (dir[i]) cur += IndexVec::vecDir[i];
		}
		usleep(1000000/10);
	}

	agent.caclRunSequence(true);
	bool route[MAZE_SIZE][MAZE_SIZE] = {false};
	for (auto &index : agent.getShortestPath()) {
		route[index.y][index.x] = true;
	}
	mazeInRobot.printWall(route);
	OperationList runSequence = agent.getRunSequence();
	runSequence.print();
}


int main(int argc, char **argv)
{
	Maze maze,maze_backup;
	Agent agent(maze);
	Agent::State prev_State = Agent::IDLE;
	printf("hello\r\n");
	IndexVec po(0,0);
	
		bool pos[MAZE_SIZE][MAZE_SIZE] = {true};
	agent.update(IndexVec(0,0),0b1110);
	agent.update(IndexVec(0,1),0b1010);
	agent.update(IndexVec(0,2),0b1010);
	agent.update(IndexVec(0,3),0b1010);
	agent.update(IndexVec(0,4),0b1010);
	agent.update(IndexVec(0,5),0b1010);
	agent.update(IndexVec(0,6),0b1010);
	agent.update(IndexVec(0,7),0b1010);
	agent.update(IndexVec(0,8),0b1010);
	agent.update(IndexVec(0,7),0b1010);
	agent.update(IndexVec(0,6),0b1010);
	agent.update(IndexVec(0,5),0b1010);
	agent.update(IndexVec(0,4),0b1010);
	agent.update(IndexVec(0,3),0b1010);
	agent.update(IndexVec(0,2),0b1010);
	agent.update(IndexVec(0,1),0b1010);
	agent.update(IndexVec(0,0),0b1110);

	agent.caclRunSequence(true);
	maze.printWall(pos);
	OperationList runSequence = agent.getRunSequence();
	runSequence.print();
	if ( argc != 2) {
		printf("error\n");
		return 1;

	}
	//test_Maze(argv[1]);
	//test_Size();
	//ii/test_Agent(argv[1]);
	//test_ShortestPath(argv[1]);
	//test_KShortestPath(argv[1]);
	//test_ShortestPathInTime(argv[1]);

	//test_Maze(argv[1]);
	//test_Size();
	//test_Agent(argv[1]);
	//test_ShortestPath(argv[1]);
	//test_KShortestPath(argv[1]);
	//test_ShortestPathInTime(argv[1]);

	printf("finish\n");

	return 0;
}
*/
