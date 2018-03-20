#ifndef __BAROQUE_TILE_H_
#define __BAROQUE_TILE_H_

struct tile_chunk_position
{
    uint32 TileChunkX;
    uint32 TileChunkY;

    uint32 RelTileX;
    uint32 RelTileY;
};

struct tile_chunk
{
    uint32* Tiles;
};

struct tile_map
{
    uint32 ChunkShift;
    uint32 ChunkMask;
    uint32 ChunkDim;

    real32 TileSideInMeters;

	uint32 TileChunkCountX;
	uint32 TileChunkCountY;
    uint32 TileChunkCountZ;
	tile_chunk* TileChunks;
};

struct tile_map_position
{
    // these are fixed point tile locations
    // high bits are the tile chunk index and the low bits are the tile index in the chunk
    uint32 AbsTileX;
    uint32 AbsTileY;

    // TODO - should these be from the center of the tile?
    // TODO - rename to Offset X and Y
    real32 TileRelX;
    real32 TileRelY;
};

#endif