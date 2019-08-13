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
