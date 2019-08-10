#include "Maze.h"
WallStr<WALL_AMOUNT> WallData;
Direction Maze::getWall(uint32_t x, uint32_t y){
  Direction dir;
  dir = getEAST(x,y) | getNORTH(x,y) | getWEST(x,y) | getSOUTH(x,y);
  return dir;
}
Direction Maze::getNORTH(uint32_t x, uint32_t y){
  Direction dir;
  //NORTH
  if(y >= MAZE_SIZE - 1) dir = NORTH | DONE_NORTH;
  else{
    uint8_t state = wall_pointer->test(2 * ((MAZE_SIZE - 1) * x + y));
    uint8_t visible = wall_pointer->test(2 * ((MAZE_SIZE - 1) * x + y) + 1);
    dir |= (state ? NORTH : 0) | (visible ? DONE_NORTH : 0);
  }
  return dir;
}
Direction Maze::getEAST(uint32_t x, uint32_t y){
  Direction dir;
  //EAST
  if(x >= MAZE_SIZE - 1) dir = EAST | DONE_EAST;
  else{
    uint8_t state = wall_pointer->test(2 * ((MAZE_SIZE - 1) * MAZE_SIZE + x + (MAZE_SIZE - 1) + y));
    uint8_t visible = wall_pointer->test(2 * ((MAZE_SIZE - 1) * MAZE_SIZE + x + (MAZE_SIZE - 1) + y) + 1);
    dir |= (state ? EAST : 0) | (visible ? DONE_EAST: 0);
  }
  return dir;
}

Direction Maze::getWEST(uint32_t x, uint32_t y){
  if(x <= 0 ) return WEST | DONE_WEST;
  else return getEAST(x - 1, y) << 0x02;
}
Direction Maze::getSOUTH(uint32_t x, uint32_t y){
  if(y <= 0 ) return SOUTH | DONE_SOUTH;
  else return getNORTH(x, y - 1) << 0x02;
}
