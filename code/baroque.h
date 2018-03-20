#ifndef __BAROQUE_H_

#include "baroque_platform.h"

#define Pi32 3.14159265359f

#if BAROQUE_SLOW
	#define Assert(Expression) \
		if (!(Expression)) {*(int *)0 = 0;}
#else
	#define Assert(Expression)
#endif

// TODO - swap, min, max, macros?
#define KiloBytes(Value) ((Value) * 1024)
#define Megabytes(Value) (KiloBytes(Value) * 1024)
#define Gigabytes(Value) (Megabytes(Value) * 1024)
#define Terabytes(Value) (Gigabytes(Value) * 1024)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

inline uint32
SafeTruncateUInt64(uint64 Value)
{
	// TODO - MAX_INT
    Assert(Value <= 0xFFFFFFFF);
    return (uint32)Value;	
}

inline game_controller_input* GetController(game_input* Input, int ControllerIndex)
{
	Assert(ControllerIndex < ArrayCount(Input->Controllers));
	return &Input->Controllers[ControllerIndex];
}

#include "baroque_intrinsics.h"
#include "baroque_tile.h"

struct memory_arena
{
    memory_index Size;
    uint8* Base;
    memory_index Used;
};

internal void
InitializeArena(memory_arena* Arena, memory_index Size, uint8* Base)
{
    Arena->Size = Size;
    Arena->Base = Base;
    Arena->Used = 0;
}

#define PushSize(Arena, type) (type *)PushSize_(Arena, sizeof(type))
#define PushArray(Arena, Count, type) (type *)PushSize_(Arena, (Count)*sizeof(type))

void*
PushSize_(memory_arena* Arena, memory_index Size)
{
    Assert((Arena->Used + Size) <= Arena->Size);
    void* Result = Arena->Base + Arena->Used;
    Arena->Used += Size;
    return Result;
}

struct world
{
    tile_map *TileMap;
};

struct game_state
{
    memory_arena WorldArena;
    world* World;
    tile_map_position PlayerP;
};


// services that the platform provides to the game


#define __BAROQUE_H_
#endif