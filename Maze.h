#ifndef MAZE_H_
#define MAZE_H_

#include <cstdint>
#include <bitset>
#include <cstddef>
#include "Parsue_Conf.h"

//std::bitset<((2 * MAZE_SIZE * MAZE_SIZE) - 2 * MAZE_SIZE) * 2> WallData;
constexpr uint32_t WALL_AMOUNT  = (2 * MAZE_SIZE * MAZE_SIZE - 2 * MAZE_SIZE) * 2;
template<uint32_t T>
using WallStr = std::bitset<T>;
extern WallStr<WALL_AMOUNT> WallData;

constexpr uint8_t EAST = 0x01;
constexpr uint8_t NORTH = 0x02;
constexpr uint8_t WEST = 0x04;
constexpr uint8_t SOUTH = 0x08;
constexpr uint8_t DONE_EAST = 0x10;
constexpr uint8_t DONE_NORTH = 0x20;
constexpr uint8_t DONE_WEST = 0x40;
constexpr uint8_t DONE_SOUTH = 0x80;
union __attribute__ ((__packed__)) Direction {
public:
	uint8_t byte;
	struct __attribute__ ((__packed__)) {
		uint8_t East:1; 		//bit0
		uint8_t North:1; 		//bit1 LSB
		uint8_t West:1; 		//bit2
		uint8_t South:1; 		//bit3
		uint8_t DoneEast:1; 	//bit4
		uint8_t DoneNorth:1; 	//bit5
		uint8_t DoneWest:1; 	//bit6 MSB
		uint8_t DoneSouth:1; 	//bit7
	} bits;
public:
	Direction(uint8_t value=0) : byte(value) {}

	//演算関連は全てuint8_tにキャストしてから行う
	inline operator uint8_t() const { return byte; }
	inline uint8_t operator|(uint8_t value) const { return byte | value; }
	inline uint8_t operator&(uint8_t value) const { return byte & value; }
	inline uint8_t operator=(uint8_t value) { return byte = value; }
	inline uint8_t operator|=(uint8_t value) { return byte |= value; }
	inline uint8_t operator&=(uint8_t value) { return byte &= value; }
	inline uint8_t operator=(Direction &obj) { return byte = obj.byte; }

	//index番目のビットが立っているかどうかをuint8_tの0x00か0x01で返す
	inline uint8_t operator[](uint8_t index) const {return (byte & (0x01<<index)) ? 1:0; }

	//全ての壁が探索済みかどうか
	inline bool isDoneAll() const { return (byte | 0x0f) == 0xff; }

	//壁の数を数える
	inline int nWall() const {
		uint8_t b = byte & 0x0f;
		b = (b & 0x55) + (b >> 1 & 0x55);
		b = (b & 0x33) + (b >> 2 & 0x33);
		b = (b & 0x0f) + (b >> 4 & 0x0f);
		return b;
	}

	//探索済みの壁を数える
	inline int nDoneWall() const {
		uint8_t b = (byte & 0xf0) >> 4;
		b = (b & 0x55) + (b >> 1 & 0x55);
		b = (b & 0x33) + (b >> 2 & 0x33);
		b = (b & 0x0f) + (b >> 4 & 0x0f);
		return b;
	}
};

class Maze{
  private:
    std::bitset<WALL_AMOUNT>* wall_pointer;
  public:
    Maze(std::bitset<WALL_AMOUNT> &wall): wall_pointer(&wall){}
    Direction getWall(uint32_t x, uint32_t y);
    Direction getNORTH(uint32_t x, uint32_t y);
    Direction getEAST(uint32_t x, uint32_t y);
    Direction getWEST(uint32_t x, uint32_t y);
    Direction getSOUTH(uint32_t x, uint32_t y);
};
#endif
