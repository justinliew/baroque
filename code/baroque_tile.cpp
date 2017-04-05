#include "baroque_tile.h"

inline void RecanonicalizeCoord(tile_map* TileMap, uint32* Tile, real32* TileRel)
{
    // TODO - need to do something that doesn't use the divide/multiply method because we can end up rounding back
    // onto the same tile we just came from

    // TileMap is assumed to be toroidal.

    int32 Offset = RoundReal32ToInt32(*TileRel / TileMap->TileSideInMeters);
    *Tile += Offset;
    *TileRel -= Offset * TileMap->TileSideInMeters;

    // TODO - fix floating point math so this can be < ?
    Assert(*TileRel >= -0.5f*TileMap->TileSideInMeters);
    Assert(*TileRel <= 0.5*TileMap->TileSideInMeters);
}

inline tile_map_position
RecanonicalizePosition(tile_map* TileMap, tile_map_position Pos)
{
    tile_map_position Result = Pos;

    RecanonicalizeCoord(TileMap, &Result.AbsTileX, &Result.TileRelX);
    RecanonicalizeCoord(TileMap, &Result.AbsTileY, &Result.TileRelY);
    return Result;
}

inline tile_chunk* 
GetTileChunk(tile_map* TileMap, uint32 TileMapX, uint32 TileMapY)
{
    if (TileMapX >= 0 && TileMapX < TileMap->TileChunkCountX &&
        TileMapY >= 0 && TileMapY < TileMap->TileChunkCountY)
    {
        tile_chunk* TileChunk = &TileMap->TileChunks[TileMapY * TileMap->TileChunkCountX + TileMapX];
        return TileChunk;
    }
    return nullptr;
}

inline uint32
GetTileValueUnchecked(tile_map* TileMap, tile_chunk* TileChunk, uint32 TileX, uint32 TileY)
{
    Assert(TileChunk);
    Assert(TileX < TileMap->ChunkDim);
    Assert(TileY < TileMap->ChunkDim);

    uint32 TileMapValue = TileChunk->Tiles[TileY * TileMap->ChunkDim + TileX];
    return TileMapValue;
}

inline void
SetTileValueUnchecked(tile_map* TileMap, tile_chunk* TileChunk, uint32 TileX, uint32 TileY, uint32 TileValue)
{
    Assert(TileChunk);
    Assert(TileX < TileMap->ChunkDim);
    Assert(TileY < TileMap->ChunkDim);

    TileChunk->Tiles[TileY * TileMap->ChunkDim + TileX] = TileValue;
}

internal uint32 
GetTileValue(tile_map* TileMap, tile_chunk* TileChunk, uint32 TestTileX, uint32 TestTileY)
{
    uint32 TileChunkValue = 0;

    if (TileChunk != nullptr)
    {
        TileChunkValue = GetTileValueUnchecked(TileMap, TileChunk, TestTileX, TestTileY);
    }

    return TileChunkValue;
}

internal void
SetTileValue(tile_map* TileMap, tile_chunk* TileChunk, uint32 TestTileX, uint32 TestTileY, uint32 TileValue)
{
    if (TileChunk != nullptr)
    {
        SetTileValueUnchecked(TileMap, TileChunk, TestTileX, TestTileY, TileValue);
    }
}


inline tile_chunk_position 
GetChunkPositionFor(tile_map* TileMap, uint32 AbsTileX, uint32 AbsTileY)
{
    tile_chunk_position Result;

    Result.TileChunkX = AbsTileX >> TileMap->ChunkShift;
    Result.TileChunkY = AbsTileY >> TileMap->ChunkShift;
    Result.RelTileX = AbsTileX & TileMap->ChunkMask;
    Result.RelTileY = AbsTileY & TileMap->ChunkMask;

    return Result;
}

internal uint32
GetTileValue(tile_map* TileMap, uint32 AbsTileX, uint32 AbsTileY)
{
    tile_chunk_position ChunkPos = GetChunkPositionFor(TileMap, AbsTileX, AbsTileY);
    tile_chunk *TileChunk = GetTileChunk(TileMap, ChunkPos.TileChunkX, ChunkPos.TileChunkY);
    uint32 TileChunkValue = GetTileValue(TileMap, TileChunk, ChunkPos.RelTileX, ChunkPos.RelTileY);

    return TileChunkValue;
}

internal bool32 
IsTileMapPointEmpty(tile_map* TileMap, tile_map_position CanPos)
{
    uint32 TileChunkValue = GetTileValue(TileMap, CanPos.AbsTileX, CanPos.AbsTileY);
    bool32 Empty = (TileChunkValue == 0);

    return Empty;
}

internal void
SetTileValue(memory_arena* Arena, tile_map* TileMap, uint32 AbsTileX, uint32 AbsTileY, uint32 TileValue)
{
    tile_chunk_position ChunkPos = GetChunkPositionFor(TileMap, AbsTileX, AbsTileY);
    tile_chunk *TileChunk = GetTileChunk(TileMap, ChunkPos.TileChunkX, ChunkPos.TileChunkY);
    Assert(TileChunk); // TODO - on demand tile chunk creation

    SetTileValue(TileMap, TileChunk, ChunkPos.RelTileX, ChunkPos.RelTileY, TileValue);
}