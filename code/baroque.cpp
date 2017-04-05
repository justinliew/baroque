// Justin Liew 2015-05-24 8:24pm Surface Book
// Justin Liew 2015-05-26 3:00pm Work
// Justin Liew 2015-05-27 8am Train
// Justin Liew 2015-05-29 8am Seabus
// Justin Liew 2016-05-31 5pm Home
// Justin Liew 2016-06-02 1pm Work
// Justin Liew 2016-06-14 5pm Seabus (38:36)
// Justin Liew 2016-06-15 8:30pm Seabus (41:!2)
// Justin Liew 2015-07-20 8:30am Seabus (31 - 21:00)
// Justin Liew 2016-07-20 4:30pm Bus (31 - )
// JUstin Liew 2016-10-19 4:30pm Bus (31 - 36:07)
// Justin Liew 2016-10-25 7:43am Bus (31 - 59:30)
// Justin Liew 2016-11-25 3:00pm Work. 32 23:00
// Justin Liew 2016-11-26 3:00pm Home. 32 - 49:00
// Justin Liew 2016-11-29 4:00pm Work. 33 - 53:50
// Justin Liew 2017-02-20 12:00pm Work. 35 start

#include "baroque.h"
#include "baroque_tile.cpp"

internal void 
GameOutputSound(game_state* GameState, game_sound_output_buffer* SoundBuffer, int ToneHz)
{
    int16 ToneVolume = 6000;
    int WavePeriod = SoundBuffer->SamplesPerSecond / ToneHz;

    int16* SampleOut = SoundBuffer->Samples;

    for(int SampleIndex = 0;
		SampleIndex < SoundBuffer->SampleCount;
        ++SampleIndex)
    {
        // TODO(casey): Draw this out for people
#if 0
        real32 SineValue = sinf(GameState->tSine);
        int16 SampleValue = (int16)(SineValue * ToneVolume);
#else
        int16 SampleValue = 0;
#endif
        *SampleOut++ = SampleValue;
        *SampleOut++ = SampleValue;

#if 0
        GameState->tSine += 2.0f*Pi32*1.0f/(real32)WavePeriod;
        if (GameState->tSine > 2.0f*Pi32)
        {
            GameState->tSine -= 2.0f*Pi32;
        }
#endif
    }
}

internal void
DrawRectangle(game_offscreen_buffer *Buffer, 
                real32 RealMinX, real32 RealMinY, 
                real32 RealMaxX, real32 RealMaxY,
                real32 R, real32 G, real32 B)
// TODO floating point color
{
    int32 MinX = RoundReal32ToInt32(RealMinX);
    int32 MinY = RoundReal32ToInt32(RealMinY);
    int32 MaxX = RoundReal32ToInt32(RealMaxX);
    int32 MaxY = RoundReal32ToInt32(RealMaxY);

    if (MinX < 0)
    {
        MinX = 0;
    }

    if (MinY < 0)
    {
        MinY = 0;
    }

    if (MaxX > Buffer->Width)
    {
        MaxX = Buffer->Width;
    }

    if (MaxY > Buffer->Height)
    {
        MaxY = Buffer->Height;
    }

    uint32 Colour = (RoundReal32ToUInt32(R * 255.f) << 16) |
                   (RoundReal32ToUInt32(G * 255.f) << 8) |
                   (RoundReal32ToUInt32(B * 255.f));

    uint8* Row = (uint8 *)Buffer->Memory + 
                    MinX * Buffer->BytesPerPixel + 
                    MinY * Buffer->Pitch;

    for (int Y = MinY; Y < MaxY; ++Y)
    {
        uint32* Pixel = (uint32*)Row;
        for (int X = MinX; X < MaxX; ++X)
        {
            *Pixel++ = Colour;
        }
        Row += Buffer->Pitch;
    }
}

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

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    game_state* GameState = (game_state*)Memory->PermanentStorage;

    real32 PlayerHeight =  1.4f;
    real32 PlayerWidth = 0.75f * PlayerHeight;

    if (!Memory->IsInitialized)
    {
        GameState->PlayerP.AbsTileX = 1;
        GameState->PlayerP.AbsTileY = 3;

        GameState->PlayerP.TileRelX = 5.f;
        GameState->PlayerP.TileRelY = 5.f;
        InitializeArena(&GameState->WorldArena, Memory->PermanentStorageSize - sizeof(game_state), (uint8 *)Memory->PermanentStorage + sizeof(game_state));

        GameState->World = PushSize(&GameState->WorldArena, world);
        world* World = GameState->World;
        World->TileMap = PushSize(&GameState->WorldArena, tile_map);

        tile_map* TileMap = World->TileMap;

        // 256x256 tile chunks
        TileMap->ChunkShift = 8;
        TileMap->ChunkMask = (1 << TileMap->ChunkShift) - 1;
        TileMap->ChunkDim = (1 << TileMap->ChunkShift);

        TileMap->TileChunkCountX = 4;
        TileMap->TileChunkCountY = 4;

        TileMap->TileChunks = PushArray(&GameState->WorldArena, TileMap->TileChunkCountX*TileMap->TileChunkCountY, tile_chunk);

        for (uint32 Y = 0; Y < TileMap->TileChunkCountY; ++Y)
        {
            for (uint32 X = 0; X < TileMap->TileChunkCountX; ++X)
            {
                TileMap->TileChunks[Y * TileMap->TileChunkCountX + X].Tiles = PushArray(&GameState->WorldArena, TileMap->ChunkDim * TileMap->ChunkDim, uint32);
            }
        }

        // TODO - begin using TileSideInMeters
        TileMap->TileSideInMeters = 1.4f;
        TileMap->TileSideInPixels = 60;
        TileMap->MetersToPixels = (real32)TileMap->TileSideInPixels / (real32)TileMap->TileSideInMeters;

        real32 LowerLeftX = -(real32)TileMap->TileSideInPixels/2;
        real32 LowerLeftY = (real32)Buffer->Height;

        // tile_chunk TileMapTileChunk;
        // TileMapTileChunk.Tiles = (uint32*)TempTiles;
        // TileMap->TileChunks = &TileMapTileChunk;

        uint32 TilesPerWidth = 17;
        uint32 TilesPerHeight = 9;
        for (uint32 ScreenY = 0; ScreenY < 32; ++ScreenY)
        {
            for (uint32 ScreenX = 0; ScreenX < 32; ++ScreenX)
            {
                for (uint32 TileY = 0; TileY < TilesPerHeight; ++TileY)
                {
                    for (uint32 TileX = 0; TileX < TilesPerWidth; ++TileX)
                    {
                        uint32 AbsTileX = ScreenX * TilesPerWidth + TileX;
                        uint32 AbsTileY = ScreenY * TilesPerHeight + TileY;

                        uint32 TileValue = 0;
                        if ((TileX == 0) || (TileX == (TilesPerWidth - 1)))
                        {
                            TileValue = 1;
                        }

                        SetTileValue(&GameState->WorldArena, World->TileMap, AbsTileX, AbsTileY, TileValue);
                        //SetTileValue(&GameState->WorldArena, World->TileMap, AbsTileX, AbsTileY, (TileX == TileY) && (TileY % 2) ? 1 : 0);
                    }
                }
            }
        }

        Memory->IsInitialized = true; // this may be more appropriate to do in the platform layer
    }

    world* World = GameState->World;
    tile_map* TileMap = World->TileMap;

    for (int ControllerIndex = 0; ControllerIndex < ArrayCount(Input->Controllers); ++ControllerIndex)
    {
        game_controller_input* CurrentInput = GetController(Input, ControllerIndex);

        if (CurrentInput->IsAnalog)
        {
            // TODO - analog movement tuning
        }
        else
        {
            real32 dPlayerX = 0.f;
            real32 dPlayerY = 0.f;

            if (CurrentInput->MoveUp.EndedDown)
            {
                dPlayerY = 1.f;
            }
            if (CurrentInput->MoveDown.EndedDown)
            {
                dPlayerY = -1.f;
            }
            if (CurrentInput->MoveLeft.EndedDown)
            {
                dPlayerX = -1.f;
            }
            if (CurrentInput->MoveRight.EndedDown)
            {
                dPlayerX = 1.f;
            }

            real32 PlayerSpeed = 2.f;
            if (CurrentInput->ActionUp.EndedDown)
            {
                PlayerSpeed = 10.f;
            }

            dPlayerX *= PlayerSpeed;
            dPlayerY *= PlayerSpeed;

            // TODO - diagonal will be faster. Fix when we have vectors.
            tile_map_position NewPlayerP = GameState->PlayerP;
            NewPlayerP.TileRelX += Input->dtForFrame * dPlayerX;
            NewPlayerP.TileRelY += Input->dtForFrame * dPlayerY;
            NewPlayerP = RecanonicalizePosition(TileMap, NewPlayerP);
            // TODO - delta function that does update/recanonicalize

            tile_map_position PlayerLeft = NewPlayerP;
            PlayerLeft.TileRelX -= 0.5f * PlayerWidth;
            PlayerLeft = RecanonicalizePosition(TileMap, PlayerLeft);

            tile_map_position PlayerRight = NewPlayerP;
            PlayerRight.TileRelX += 0.5f * PlayerWidth;
            PlayerRight = RecanonicalizePosition(TileMap, PlayerRight);

            bool32 IsValid = IsTileMapPointEmpty(TileMap, NewPlayerP);
            IsValid &= IsTileMapPointEmpty(TileMap, PlayerLeft);
            IsValid &= IsTileMapPointEmpty(TileMap, PlayerRight);

            if (IsValid)
            {
                GameState->PlayerP = NewPlayerP;
            }

        }
    }

    DrawRectangle(Buffer, 0.f, 0.f, (real32)Buffer->Width, (real32)Buffer->Height, 1.f, 0.f, 0.f);

    real32 ScreenCenterX = 0.5f*(real32)Buffer->Width;
    real32 ScreenCenterY = 0.5f*(real32)Buffer->Height;

    for (int32 RelRow = -10; RelRow < 10; ++RelRow)
    {
        for (int32 RelColumn = -20; RelColumn < 20; ++RelColumn)
        {
            int32 Column = GameState->PlayerP.AbsTileX + RelColumn;
            int32 Row = GameState->PlayerP.AbsTileY + RelRow;
            uint32  TileID = GetTileValue(TileMap, Column, Row);
            real32 Gray = 0.5f;
            if (TileID == 1)
            {
                Gray = 1.f;
            }

            if (Row == (int32)GameState->PlayerP.AbsTileY && Column == (int32)GameState->PlayerP.AbsTileX)
            {
                Gray = 0.f;
            }

            real32 CentreX = ScreenCenterX + ((real32)RelColumn) * TileMap->TileSideInPixels - TileMap->MetersToPixels*GameState->PlayerP.TileRelX;
            real32 CentreY = ScreenCenterY - ((real32)RelRow) * TileMap->TileSideInPixels + TileMap->MetersToPixels*GameState->PlayerP.TileRelY;

            real32 MinX = CentreX - 0.5f*TileMap->TileSideInPixels;
            real32 MinY = CentreY - 0.5f*TileMap->TileSideInPixels;

            real32 MaxX = CentreX + 0.5f*TileMap->TileSideInPixels;
            real32 MaxY = CentreY + 0.5f*TileMap->TileSideInPixels;
            DrawRectangle(Buffer, MinX, MinY, MaxX, MaxY, Gray, Gray, Gray);
        }
    }

    real32 PlayerR = 1.f;
    real32 PlayerG = 1.f;
    real32 PlayerB = 0.f;
    real32 PlayerLeft = ScreenCenterX - 0.5f*TileMap->MetersToPixels*PlayerWidth;
    real32 PlayerTop = ScreenCenterY - TileMap->MetersToPixels*PlayerHeight;

    DrawRectangle(Buffer, PlayerLeft, PlayerTop, PlayerLeft + TileMap->MetersToPixels * PlayerWidth, PlayerTop + TileMap->MetersToPixels * PlayerHeight, PlayerR, PlayerG, PlayerB);
}

extern "C" GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
    game_state* GameState = (game_state*)Memory->PermanentStorage;
    GameOutputSound(GameState, SoundBuffer, 400);
}

#if 0
internal void
RenderWeirdGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
    uint8 *Row = (uint8 *)Buffer->Memory;    
    for(int Y = 0;
        Y < Buffer->Height;
        ++Y)
    {
        uint32 *Pixel = (uint32 *)Row;
        for(int X = 0;
            X < Buffer->Width;
            ++X)
        {
            uint8 Blue = (uint8)(X + BlueOffset);
            uint8 Green = (uint8)(Y + GreenOffset);

            *Pixel++ = ((Green << 16) | Blue);
        }
        
        Row += Buffer->Pitch;
    }
}
#endif
