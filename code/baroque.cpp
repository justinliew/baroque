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
// Justin Liew 2018-03-19 22:00 Home. 35 34:00. 
// Justin Liew 2018-03-20 16:00 Work 35 49:00.

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


// TODO - make this larger and in a header file
global_variable uint32 RandomNumberTable[] = {
0x397f074a,0x23c58da7,0x2b981ef5,0x00552d2e,0x2ee1b216,0x1171601a,0x0b50174a,0x2b6c7a23,
0x04550a8b,0x0b6b9874,0x03648bf0,0x20d08914,0x327818f6,0x22ab6922,0x2962719f,0x0bb39803,
0x15ef388d,0x2cdcf957,0x0acce7ed,0x2c4ff9b7,0x2f7178d3,0x03dc732e,0x23b797d1,0x0855ae25,
0x3b8a032b,0x39bc678a,0x06bd6be8,0x27c5f979,0x34a9b80a,0x16f7ab26,0x1c9553ef,0x289f8274,
0x1c1126b4,0x225c9a04,0x1fa43049,0x1b9a4a2b,0x22b1beb6,0x394d3b0b,0x2dcf0abc,0x3b91de70,
0x2b81e3c4,0x29705b81,0x1c9409ab,0x2a58fd1b,0x0e580be4,0x367792bf,0x2695f66b,0x1343f643,
0x190f4cfa,0x18eb3f7a,0x075f80bd,0x34c8f8c7,0x133d4680,0x26a92a8a,0x139ba714,0x0f4ec2f2,
0x071ea2b5,0x1ef35536,0x13afec4e,0x19226915,0x0aa7360c,0x0aa9a404,0x325c24f6,0x02a4b7f4,
0x1d0e01f9,0x1813c883,0x38e586fd,0x033715e9,0x2196aafc,0x295b794e,0x2472caeb,0x0a05783b,
0x0ec4a575,0x2971168e,0x160ccac8,0x37f09105,0x1e1db6d4,0x01da3ee6,0x05d68844,0x0671e0a4,
0x0d2a6106,0x03daf6b7,0x14b3b48a,0x1450a6db,0x269b9bf7,0x1a86fa93,0x1a6c997a,0x12cf13a7,
0x01e13b76,0x142430ed,0x0c8eb803,0x0dc46280,0x134ad33a,0x105b42ec,0x1e69bb5b,0x2b7c690d,
0x1e98ba98,0x19638de3,0x1366fd75,0x11b92f8d,0x1ab68649,0x359b2418,0x111ae352,0x38429888,
0x10470fa8,0x13522b48,0x04adc2de,0x31f1a54c,0x17742685,0x24c48c35,0x1a1a34a6,0x1291beab,
0x350be491,0x20e26e1c,0x1a8d076a,0x1538a266,0x3a0295b3,0x04828aa6,0x23ee3796,0x0b18f0d1,
0x22ff5b35,0x0fd16969,0x0b3c77f4,0x3733f39b,0x0262f7dc,0x1d2207bc,0x24c19d15,0x3136a533,
0x2184b15a,0x2c79f4b5,0x2b4c1698,0x2d3291f8,0x1185beb4,0x019aee2f,0x3521c9cc,0x1bde30ec,
0x283d92ae,0x34030425,0x0becd6df,0x0d669f70,0x253c238d,0x1b53e384,0x2af189f4,0x24525986,
0x330ef244,0x03757133,0x16631b02,0x2f427cfe,0x0e04aff2,0x27ebdb6b,0x05b3d11f,0x3a8cc382,
0x2cc35e56,0x334833da,0x2b6e32dd,0x1af6b2bd,0x25d07e20,0x1999ac4a,0x11bfa5d8,0x12efe3cf,
0x1d77504f,0x21c1cfa1,0x2a343d6d,0x09ebb56b,0x0f433168,0x34e8a391,0x34f98e0a,0x132051aa,
0x2df751c4,0x0b29e21b,0x0a094564,0x3a6fe02e,0x2aaab062,0x39923936,0x2143f2c4,0x27f1e384,
0x1009da4e,0x1fc29ecb,0x1518e70c,0x2e943686,0x36540109,0x29b8af89,0x10a0f5e5,0x2478938f,
0x070a5e29,0x12cd7370,0x1d1cdacd,0x0d8daa4a,0x2b9fbd24,0x31bf3e1e,0x392b2cb7,0x1268b671,
0x120696af,0x3af95ed5,0x1fcb611f,0x1a8786d7,0x1b80f35b,0x069f2604,0x27371817,0x33a79c77,
0x050611bd,0x1d11b5d8,0x25f9d8e2,0x385efbde,0x001a6912,0x0a7d29d2,0x1303be6e,0x1126a5d5,
0x21ed69b5,0x02b1d7e2,0x23db97d5,0x13cf44d5,0x211db1c1,0x168301f1,0x0589cb88,0x108408d8,
0x2d54f5f8,0x0476ca05,0x115df4fc,0x15a4b887,0x1e74ee84,0x27d65ecb,0x2643977e,0x2d33a6f6,
0x05588954,0x0beb2560,0x39265d98,0x0a854c40,0x2132a120,0x1b93d206,0x081641ca,0x27fabfaf,
0x0747ddeb,0x37d35ac2,0x1b82d34c,0x1c056109,0x1e3dcd1d,0x144406c4,0x1a0c7d97,0x26ab356e,
0x01abeb70,0x24f40624,0x04520f66,0x1b5476e2,0x017cc023,0x39f2342e,0x1369141a,0x37eff8f6,
0x26d2aaf7,0x266c2a67,0x1805aa62,0x31f01963,0x0673bfc8,0x335aea32,0x18837a65,0x02f64b84,
0x3631cef3,0x2ac44058,0x225e133d,0x2616d8fd,0x2629288f,0x14267c47,0x0eef50f1,0x2e496f57,
0x11bb38e0,0x2c1381e0,0x0e60e097,0x20addaa8,0x2075ad2c,0x1f36359d,0x1a8b69e2,0x0761af4e,
0x397f074a,0x23c58da7,0x2b981ef5,0x00552d2e,0x2ee1b216,0x1171601a,0x0b50174a,0x2b6c7a23,
0x04550a8b,0x0b6b9874,0x03648bf0,0x20d08914,0x327818f6,0x22ab6922,0x2962719f,0x0bb39803,
0x15ef388d,0x2cdcf957,0x0acce7ed,0x2c4ff9b7,0x2f7178d3,0x03dc732e,0x23b797d1,0x0855ae25,
0x3b8a032b,0x39bc678a,0x06bd6be8,0x27c5f979,0x34a9b80a,0x16f7ab26,0x1c9553ef,0x289f8274,
0x1c1126b4,0x225c9a04,0x1fa43049,0x1b9a4a2b,0x22b1beb6,0x394d3b0b,0x2dcf0abc,0x3b91de70,
0x2b81e3c4,0x29705b81,0x1c9409ab,0x2a58fd1b,0x0e580be4,0x367792bf,0x2695f66b,0x1343f643,
0x190f4cfa,0x18eb3f7a,0x075f80bd,0x34c8f8c7,0x133d4680,0x26a92a8a,0x139ba714,0x0f4ec2f2,
0x071ea2b5,0x1ef35536,0x13afec4e,0x19226915,0x0aa7360c,0x0aa9a404,0x325c24f6,0x02a4b7f4,
0x1d0e01f9,0x1813c883,0x38e586fd,0x033715e9,0x2196aafc,0x295b794e,0x2472caeb,0x0a05783b,
0x0ec4a575,0x2971168e,0x160ccac8,0x37f09105,0x1e1db6d4,0x01da3ee6,0x05d68844,0x0671e0a4,
0x0d2a6106,0x03daf6b7,0x14b3b48a,0x1450a6db,0x269b9bf7,0x1a86fa93,0x1a6c997a,0x12cf13a7,
0x01e13b76,0x142430ed,0x0c8eb803,0x0dc46280,0x134ad33a,0x105b42ec,0x1e69bb5b,0x2b7c690d,
0x1e98ba98,0x19638de3,0x1366fd75,0x11b92f8d,0x1ab68649,0x359b2418,0x111ae352,0x38429888,
0x10470fa8,0x13522b48,0x04adc2de,0x31f1a54c,0x17742685,0x24c48c35,0x1a1a34a6,0x1291beab,
0x350be491,0x20e26e1c,0x1a8d076a,0x1538a266,0x3a0295b3,0x04828aa6,0x23ee3796,0x0b18f0d1,
0x22ff5b35,0x0fd16969,0x0b3c77f4,0x3733f39b,0x0262f7dc,0x1d2207bc,0x24c19d15,0x3136a533,
0x2184b15a,0x2c79f4b5,0x2b4c1698,0x2d3291f8,0x1185beb4,0x019aee2f,0x3521c9cc,0x1bde30ec,
0x283d92ae,0x34030425,0x0becd6df,0x0d669f70,0x253c238d,0x1b53e384,0x2af189f4,0x24525986,
0x330ef244,0x03757133,0x16631b02,0x2f427cfe,0x0e04aff2,0x27ebdb6b,0x05b3d11f,0x3a8cc382,
0x2cc35e56,0x334833da,0x2b6e32dd,0x1af6b2bd,0x25d07e20,0x1999ac4a,0x11bfa5d8,0x12efe3cf,
0x1d77504f,0x21c1cfa1,0x2a343d6d,0x09ebb56b,0x0f433168,0x34e8a391,0x34f98e0a,0x132051aa,
0x2df751c4,0x0b29e21b,0x0a094564,0x3a6fe02e,0x2aaab062,0x39923936,0x2143f2c4,0x27f1e384,
0x1009da4e,0x1fc29ecb,0x1518e70c,0x2e943686,0x36540109,0x29b8af89,0x10a0f5e5,0x2478938f,
0x070a5e29,0x12cd7370,0x1d1cdacd,0x0d8daa4a,0x2b9fbd24,0x31bf3e1e,0x392b2cb7,0x1268b671,
0x120696af,0x3af95ed5,0x1fcb611f,0x1a8786d7,0x1b80f35b,0x069f2604,0x27371817,0x33a79c77,
0x050611bd,0x1d11b5d8,0x25f9d8e2,0x385efbde,0x001a6912,0x0a7d29d2,0x1303be6e,0x1126a5d5,
0x21ed69b5,0x02b1d7e2,0x23db97d5,0x13cf44d5,0x211db1c1,0x168301f1,0x0589cb88,0x108408d8,
0x2d54f5f8,0x0476ca05,0x115df4fc,0x15a4b887,0x1e74ee84,0x27d65ecb,0x2643977e,0x2d33a6f6,
0x05588954,0x0beb2560,0x39265d98,0x0a854c40,0x2132a120,0x1b93d206,0x081641ca,0x27fabfaf,
0x0747ddeb,0x37d35ac2,0x1b82d34c,0x1c056109,0x1e3dcd1d,0x144406c4,0x1a0c7d97,0x26ab356e,
0x01abeb70,0x24f40624,0x04520f66,0x1b5476e2,0x017cc023,0x39f2342e,0x1369141a,0x37eff8f6,
0x26d2aaf7,0x266c2a67,0x1805aa62,0x31f01963,0x0673bfc8,0x335aea32,0x18837a65,0x02f64b84,
0x3631cef3,0x2ac44058,0x225e133d,0x2616d8fd,0x2629288f,0x14267c47,0x0eef50f1,0x2e496f57,
0x11bb38e0,0x2c1381e0,0x0e60e097,0x20addaa8,0x2075ad2c,0x1f36359d,0x1a8b69e2,0x0761af4e,
0x397f074a,0x23c58da7,0x2b981ef5,0x00552d2e,0x2ee1b216,0x1171601a,0x0b50174a,0x2b6c7a23,
0x04550a8b,0x0b6b9874,0x03648bf0,0x20d08914,0x327818f6,0x22ab6922,0x2962719f,0x0bb39803,
0x15ef388d,0x2cdcf957,0x0acce7ed,0x2c4ff9b7,0x2f7178d3,0x03dc732e,0x23b797d1,0x0855ae25,
0x3b8a032b,0x39bc678a,0x06bd6be8,0x27c5f979,0x34a9b80a,0x16f7ab26,0x1c9553ef,0x289f8274,
0x1c1126b4,0x225c9a04,0x1fa43049,0x1b9a4a2b,0x22b1beb6,0x394d3b0b,0x2dcf0abc,0x3b91de70,
0x2b81e3c4,0x29705b81,0x1c9409ab,0x2a58fd1b,0x0e580be4,0x367792bf,0x2695f66b,0x1343f643,
0x190f4cfa,0x18eb3f7a,0x075f80bd,0x34c8f8c7,0x133d4680,0x26a92a8a,0x139ba714,0x0f4ec2f2,
0x071ea2b5,0x1ef35536,0x13afec4e,0x19226915,0x0aa7360c,0x0aa9a404,0x325c24f6,0x02a4b7f4,
0x1d0e01f9,0x1813c883,0x38e586fd,0x033715e9,0x2196aafc,0x295b794e,0x2472caeb,0x0a05783b,
0x0ec4a575,0x2971168e,0x160ccac8,0x37f09105,0x1e1db6d4,0x01da3ee6,0x05d68844,0x0671e0a4,
0x0d2a6106,0x03daf6b7,0x14b3b48a,0x1450a6db,0x269b9bf7,0x1a86fa93,0x1a6c997a,0x12cf13a7,
0x01e13b76,0x142430ed,0x0c8eb803,0x0dc46280,0x134ad33a,0x105b42ec,0x1e69bb5b,0x2b7c690d,
0x1e98ba98,0x19638de3,0x1366fd75,0x11b92f8d,0x1ab68649,0x359b2418,0x111ae352,0x38429888,
0x10470fa8,0x13522b48,0x04adc2de,0x31f1a54c,0x17742685,0x24c48c35,0x1a1a34a6,0x1291beab,
0x350be491,0x20e26e1c,0x1a8d076a,0x1538a266,0x3a0295b3,0x04828aa6,0x23ee3796,0x0b18f0d1,
0x22ff5b35,0x0fd16969,0x0b3c77f4,0x3733f39b,0x0262f7dc,0x1d2207bc,0x24c19d15,0x3136a533,
0x2184b15a,0x2c79f4b5,0x2b4c1698,0x2d3291f8,0x1185beb4,0x019aee2f,0x3521c9cc,0x1bde30ec,
0x283d92ae,0x34030425,0x0becd6df,0x0d669f70,0x253c238d,0x1b53e384,0x2af189f4,0x24525986,
0x330ef244,0x03757133,0x16631b02,0x2f427cfe,0x0e04aff2,0x27ebdb6b,0x05b3d11f,0x3a8cc382,
0x2cc35e56,0x334833da,0x2b6e32dd,0x1af6b2bd,0x25d07e20,0x1999ac4a,0x11bfa5d8,0x12efe3cf,
0x1d77504f,0x21c1cfa1,0x2a343d6d,0x09ebb56b,0x0f433168,0x34e8a391,0x34f98e0a,0x132051aa,
0x2df751c4,0x0b29e21b,0x0a094564,0x3a6fe02e,0x2aaab062,0x39923936,0x2143f2c4,0x27f1e384,
0x1009da4e,0x1fc29ecb,0x1518e70c,0x2e943686,0x36540109,0x29b8af89,0x10a0f5e5,0x2478938f,
0x070a5e29,0x12cd7370,0x1d1cdacd,0x0d8daa4a,0x2b9fbd24,0x31bf3e1e,0x392b2cb7,0x1268b671,
0x120696af,0x3af95ed5,0x1fcb611f,0x1a8786d7,0x1b80f35b,0x069f2604,0x27371817,0x33a79c77,
0x050611bd,0x1d11b5d8,0x25f9d8e2,0x385efbde,0x001a6912,0x0a7d29d2,0x1303be6e,0x1126a5d5,
0x21ed69b5,0x02b1d7e2,0x23db97d5,0x13cf44d5,0x211db1c1,0x168301f1,0x0589cb88,0x108408d8,
0x2d54f5f8,0x0476ca05,0x115df4fc,0x15a4b887,0x1e74ee84,0x27d65ecb,0x2643977e,0x2d33a6f6,
0x05588954,0x0beb2560,0x39265d98,0x0a854c40,0x2132a120,0x1b93d206,0x081641ca,0x27fabfaf,
0x0747ddeb,0x37d35ac2,0x1b82d34c,0x1c056109,0x1e3dcd1d,0x144406c4,0x1a0c7d97,0x26ab356e,
0x01abeb70,0x24f40624,0x04520f66,0x1b5476e2,0x017cc023,0x39f2342e,0x1369141a,0x37eff8f6,
0x26d2aaf7,0x266c2a67,0x1805aa62,0x31f01963,0x0673bfc8,0x335aea32,0x18837a65,0x02f64b84,
0x3631cef3,0x2ac44058,0x225e133d,0x2616d8fd,0x2629288f,0x14267c47,0x0eef50f1,0x2e496f57,
0x11bb38e0,0x2c1381e0,0x0e60e097,0x20addaa8,0x2075ad2c,0x1f36359d,0x1a8b69e2,0x0761af4e,
0x397f074a,0x23c58da7,0x2b981ef5,0x00552d2e,0x2ee1b216,0x1171601a,0x0b50174a,0x2b6c7a23,
0x04550a8b,0x0b6b9874,0x03648bf0,0x20d08914,0x327818f6,0x22ab6922,0x2962719f,0x0bb39803,
0x15ef388d,0x2cdcf957,0x0acce7ed,0x2c4ff9b7,0x2f7178d3,0x03dc732e,0x23b797d1,0x0855ae25,
0x3b8a032b,0x39bc678a,0x06bd6be8,0x27c5f979,0x34a9b80a,0x16f7ab26,0x1c9553ef,0x289f8274,
0x1c1126b4,0x225c9a04,0x1fa43049,0x1b9a4a2b,0x22b1beb6,0x394d3b0b,0x2dcf0abc,0x3b91de70,
0x2b81e3c4,0x29705b81,0x1c9409ab,0x2a58fd1b,0x0e580be4,0x367792bf,0x2695f66b,0x1343f643,
0x190f4cfa,0x18eb3f7a,0x075f80bd,0x34c8f8c7,0x133d4680,0x26a92a8a,0x139ba714,0x0f4ec2f2,
0x071ea2b5,0x1ef35536,0x13afec4e,0x19226915,0x0aa7360c,0x0aa9a404,0x325c24f6,0x02a4b7f4,
0x1d0e01f9,0x1813c883,0x38e586fd,0x033715e9,0x2196aafc,0x295b794e,0x2472caeb,0x0a05783b,
0x0ec4a575,0x2971168e,0x160ccac8,0x37f09105,0x1e1db6d4,0x01da3ee6,0x05d68844,0x0671e0a4,
0x0d2a6106,0x03daf6b7,0x14b3b48a,0x1450a6db,0x269b9bf7,0x1a86fa93,0x1a6c997a,0x12cf13a7,
0x01e13b76,0x142430ed,0x0c8eb803,0x0dc46280,0x134ad33a,0x105b42ec,0x1e69bb5b,0x2b7c690d,
0x1e98ba98,0x19638de3,0x1366fd75,0x11b92f8d,0x1ab68649,0x359b2418,0x111ae352,0x38429888,
0x10470fa8,0x13522b48,0x04adc2de,0x31f1a54c,0x17742685,0x24c48c35,0x1a1a34a6,0x1291beab,
0x350be491,0x20e26e1c,0x1a8d076a,0x1538a266,0x3a0295b3,0x04828aa6,0x23ee3796,0x0b18f0d1,
0x22ff5b35,0x0fd16969,0x0b3c77f4,0x3733f39b,0x0262f7dc,0x1d2207bc,0x24c19d15,0x3136a533,
0x2184b15a,0x2c79f4b5,0x2b4c1698,0x2d3291f8,0x1185beb4,0x019aee2f,0x3521c9cc,0x1bde30ec,
0x283d92ae,0x34030425,0x0becd6df,0x0d669f70,0x253c238d,0x1b53e384,0x2af189f4,0x24525986,
0x330ef244,0x03757133,0x16631b02,0x2f427cfe,0x0e04aff2,0x27ebdb6b,0x05b3d11f,0x3a8cc382,
0x2cc35e56,0x334833da,0x2b6e32dd,0x1af6b2bd,0x25d07e20,0x1999ac4a,0x11bfa5d8,0x12efe3cf,
0x1d77504f,0x21c1cfa1,0x2a343d6d,0x09ebb56b,0x0f433168,0x34e8a391,0x34f98e0a,0x132051aa,
0x2df751c4,0x0b29e21b,0x0a094564,0x3a6fe02e,0x2aaab062,0x39923936,0x2143f2c4,0x27f1e384,
0x1009da4e,0x1fc29ecb,0x1518e70c,0x2e943686,0x36540109,0x29b8af89,0x10a0f5e5,0x2478938f,
0x070a5e29,0x12cd7370,0x1d1cdacd,0x0d8daa4a,0x2b9fbd24,0x31bf3e1e,0x392b2cb7,0x1268b671,
0x120696af,0x3af95ed5,0x1fcb611f,0x1a8786d7,0x1b80f35b,0x069f2604,0x27371817,0x33a79c77,
0x050611bd,0x1d11b5d8,0x25f9d8e2,0x385efbde,0x001a6912,0x0a7d29d2,0x1303be6e,0x1126a5d5,
0x21ed69b5,0x02b1d7e2,0x23db97d5,0x13cf44d5,0x211db1c1,0x168301f1,0x0589cb88,0x108408d8,
0x2d54f5f8,0x0476ca05,0x115df4fc,0x15a4b887,0x1e74ee84,0x27d65ecb,0x2643977e,0x2d33a6f6,
0x05588954,0x0beb2560,0x39265d98,0x0a854c40,0x2132a120,0x1b93d206,0x081641ca,0x27fabfaf,
0x0747ddeb,0x37d35ac2,0x1b82d34c,0x1c056109,0x1e3dcd1d,0x144406c4,0x1a0c7d97,0x26ab356e,
0x01abeb70,0x24f40624,0x04520f66,0x1b5476e2,0x017cc023,0x39f2342e,0x1369141a,0x37eff8f6,
0x26d2aaf7,0x266c2a67,0x1805aa62,0x31f01963,0x0673bfc8,0x335aea32,0x18837a65,0x02f64b84,
0x3631cef3,0x2ac44058,0x225e133d,0x2616d8fd,0x2629288f,0x14267c47,0x0eef50f1,0x2e496f57,
0x11bb38e0,0x2c1381e0,0x0e60e097,0x20addaa8,0x2075ad2c,0x1f36359d,0x1a8b69e2,0x0761af4e,
0x397f074a,0x23c58da7,0x2b981ef5,0x00552d2e,0x2ee1b216,0x1171601a,0x0b50174a,0x2b6c7a23,
0x04550a8b,0x0b6b9874,0x03648bf0,0x20d08914,0x327818f6,0x22ab6922,0x2962719f,0x0bb39803,
0x15ef388d,0x2cdcf957,0x0acce7ed,0x2c4ff9b7,0x2f7178d3,0x03dc732e,0x23b797d1,0x0855ae25,
0x3b8a032b,0x39bc678a,0x06bd6be8,0x27c5f979,0x34a9b80a,0x16f7ab26,0x1c9553ef,0x289f8274,
0x1c1126b4,0x225c9a04,0x1fa43049,0x1b9a4a2b,0x22b1beb6,0x394d3b0b,0x2dcf0abc,0x3b91de70,
0x2b81e3c4,0x29705b81,0x1c9409ab,0x2a58fd1b,0x0e580be4,0x367792bf,0x2695f66b,0x1343f643,
0x190f4cfa,0x18eb3f7a,0x075f80bd,0x34c8f8c7,0x133d4680,0x26a92a8a,0x139ba714,0x0f4ec2f2,
0x071ea2b5,0x1ef35536,0x13afec4e,0x19226915,0x0aa7360c,0x0aa9a404,0x325c24f6,0x02a4b7f4,
0x1d0e01f9,0x1813c883,0x38e586fd,0x033715e9,0x2196aafc,0x295b794e,0x2472caeb,0x0a05783b,
0x0ec4a575,0x2971168e,0x160ccac8,0x37f09105,0x1e1db6d4,0x01da3ee6,0x05d68844,0x0671e0a4,
0x0d2a6106,0x03daf6b7,0x14b3b48a,0x1450a6db,0x269b9bf7,0x1a86fa93,0x1a6c997a,0x12cf13a7,
0x01e13b76,0x142430ed,0x0c8eb803,0x0dc46280,0x134ad33a,0x105b42ec,0x1e69bb5b,0x2b7c690d,
0x1e98ba98,0x19638de3,0x1366fd75,0x11b92f8d,0x1ab68649,0x359b2418,0x111ae352,0x38429888,
0x10470fa8,0x13522b48,0x04adc2de,0x31f1a54c,0x17742685,0x24c48c35,0x1a1a34a6,0x1291beab,
0x350be491,0x20e26e1c,0x1a8d076a,0x1538a266,0x3a0295b3,0x04828aa6,0x23ee3796,0x0b18f0d1,
0x22ff5b35,0x0fd16969,0x0b3c77f4,0x3733f39b,0x0262f7dc,0x1d2207bc,0x24c19d15,0x3136a533,
0x2184b15a,0x2c79f4b5,0x2b4c1698,0x2d3291f8,0x1185beb4,0x019aee2f,0x3521c9cc,0x1bde30ec,
0x283d92ae,0x34030425,0x0becd6df,0x0d669f70,0x253c238d,0x1b53e384,0x2af189f4,0x24525986,
0x330ef244,0x03757133,0x16631b02,0x2f427cfe,0x0e04aff2,0x27ebdb6b,0x05b3d11f,0x3a8cc382,
0x2cc35e56,0x334833da,0x2b6e32dd,0x1af6b2bd,0x25d07e20,0x1999ac4a,0x11bfa5d8,0x12efe3cf,
0x1d77504f,0x21c1cfa1,0x2a343d6d,0x09ebb56b,0x0f433168,0x34e8a391,0x34f98e0a,0x132051aa,
0x2df751c4,0x0b29e21b,0x0a094564,0x3a6fe02e,0x2aaab062,0x39923936,0x2143f2c4,0x27f1e384,
0x1009da4e,0x1fc29ecb,0x1518e70c,0x2e943686,0x36540109,0x29b8af89,0x10a0f5e5,0x2478938f,
0x070a5e29,0x12cd7370,0x1d1cdacd,0x0d8daa4a,0x2b9fbd24,0x31bf3e1e,0x392b2cb7,0x1268b671,
0x120696af,0x3af95ed5,0x1fcb611f,0x1a8786d7,0x1b80f35b,0x069f2604,0x27371817,0x33a79c77,
0x050611bd,0x1d11b5d8,0x25f9d8e2,0x385efbde,0x001a6912,0x0a7d29d2,0x1303be6e,0x1126a5d5,
0x21ed69b5,0x02b1d7e2,0x23db97d5,0x13cf44d5,0x211db1c1,0x168301f1,0x0589cb88,0x108408d8,
0x2d54f5f8,0x0476ca05,0x115df4fc,0x15a4b887,0x1e74ee84,0x27d65ecb,0x2643977e,0x2d33a6f6,
0x05588954,0x0beb2560,0x39265d98,0x0a854c40,0x2132a120,0x1b93d206,0x081641ca,0x27fabfaf,
0x0747ddeb,0x37d35ac2,0x1b82d34c,0x1c056109,0x1e3dcd1d,0x144406c4,0x1a0c7d97,0x26ab356e,
0x01abeb70,0x24f40624,0x04520f66,0x1b5476e2,0x017cc023,0x39f2342e,0x1369141a,0x37eff8f6,
0x26d2aaf7,0x266c2a67,0x1805aa62,0x31f01963,0x0673bfc8,0x335aea32,0x18837a65,0x02f64b84,
0x3631cef3,0x2ac44058,0x225e133d,0x2616d8fd,0x2629288f,0x14267c47,0x0eef50f1,0x2e496f57,
0x11bb38e0,0x2c1381e0,0x0e60e097,0x20addaa8,0x2075ad2c,0x1f36359d,0x1a8b69e2,0x0761af4e,};

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
        TileMap->ChunkShift = 4;
        TileMap->ChunkMask = (1 << TileMap->ChunkShift) - 1;
        TileMap->ChunkDim = (1 << TileMap->ChunkShift);

        TileMap->TileChunkCountX = 128;
        TileMap->TileChunkCountY = 128;
        TileMap->TileChunkCountZ = 2;

        TileMap->TileChunks = PushArray(&GameState->WorldArena, TileMap->TileChunkCountX * TileMap->TileChunkCountY * TileMap->TileChunkCountZ, tile_chunk);
        TileMap->TileSideInMeters = 1.4f;

        // tile_chunk TileMapTileChunk;
        // TileMapTileChunk.Tiles = (uint32*)TempTiles;
        // TileMap->TileChunks = &TileMapTileChunk;

        uint32 TilesPerWidth = 17;
        uint32 TilesPerHeight = 9;
        uint32 ScreenX = 0;
        uint32 ScreenY = 0;
        uint32 AbsTileZ = 0;
		uint32 RandomNumberIndex = 0;

        bool32 DoorLeft = false;
        bool32 DoorRight = false;
        bool32 DoorTop = false;
        bool32 DoorBottom = false;
        bool32 DoorUp = false;
        bool32 DoorDown = false;

        for (uint32 ScreenIndex = 0; ScreenIndex < 100; ++ScreenIndex)
        {
            // TODO - proper random number generator
            Assert(RandomNumberIndex < ArrayCount(RandomNumberTable));
            uint32 RandomChoice;
            if (DoorUp || DoorDown)
            {
                RandomChoice = RandomNumberTable[RandomNumberIndex++] % 2;
            }
            else
            {
                RandomChoice = RandomNumberTable[RandomNumberIndex++] % 3;
            }

			bool32 CreatedZDoor = false;
            if (RandomChoice == 2)
            {
				CreatedZDoor = true;
                if (AbsTileZ == 0)
                {
                    DoorUp = true;
                }
                else
                {
                    DoorDown = true;
                }
            }
            else if (RandomChoice == 1)
            {
                DoorRight = true;
            }
            else
            {
                DoorTop = true;
            }

            for (uint32 TileY = 0; TileY < TilesPerHeight; ++TileY)
            {
                for (uint32 TileX = 0; TileX < TilesPerWidth; ++TileX)
                {
                    uint32 AbsTileX = ScreenX * TilesPerWidth + TileX;
                    uint32 AbsTileY = ScreenY * TilesPerHeight + TileY;

                    uint32 TileValue = 1;
                    if ((TileX == 0) && (!DoorLeft || TileY != (TilesPerHeight / 2)))
                    {
                        TileValue = 2;
                    }
                    if ((TileX == TilesPerWidth - 1) && (!DoorRight || TileY != (TilesPerHeight / 2)))
                    {
                        TileValue = 2;
                    }

                    if ((TileY == 0) && (!DoorBottom || TileX != (TilesPerWidth / 2)))
                    {
                        TileValue = 2;
                    }
                    if ((TileY == TilesPerHeight - 1) && (!DoorTop || TileX != (TilesPerWidth / 2)))
                    {
                        TileValue = 2;
                    }

                    if (TileX == 10 && TileY == 6)
                    {
                        if (DoorUp)
                        {
                            TileValue = 3;
                        }
                        if (DoorDown)
                        {
                            TileValue = 4;
                        }
                    }


                    SetTileValue(&GameState->WorldArena, World->TileMap, AbsTileX, AbsTileY, AbsTileZ, TileValue);
                    //SetTileValue(&GameState->WorldArena, World->TileMap, AbsTileX, AbsTileY, (TileX == TileY) && (TileY % 2) ? 1 : 0);
                }
            }

            DoorLeft = DoorRight;
            DoorBottom = DoorTop;

			if (CreatedZDoor)
			{
				DoorDown = !DoorDown;
				DoorUp = !DoorUp;
			}
			else
			{
				DoorDown = false;
				DoorUp = false;
			}

            DoorRight = false;
            DoorTop = false;

            if (RandomChoice == 2)
            {
                if (AbsTileZ == 0)
                {
                    AbsTileZ = 1;
                }
                else
                {
                    AbsTileZ = 0;
                }
            }
            else if (RandomChoice == 1)
            {
                ScreenX += 1;
            }
            else
            {
                ScreenY += 1;
            }
        }

        Memory->IsInitialized = true; // this may be more appropriate to do in the platform layer
    }

    world* World = GameState->World;
    tile_map* TileMap = World->TileMap;

    int32 TileSideInPixels = 60;
    real32 MetersToPixels = (real32)TileSideInPixels / (real32)TileMap->TileSideInMeters;

    real32 LowerLeftX = -(real32)TileSideInPixels/2;
    real32 LowerLeftY = (real32)Buffer->Height;

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
                if (!AreOnSameTile(&GameState->PlayerP, &NewPlayerP))
                {
                    uint32 NewTileValue = GetTileValue(TileMap, NewPlayerP);
                    if (NewTileValue == 3)
                    {
                        ++NewPlayerP.AbsTileZ;
                    }
                    else if (NewTileValue == 4)
                    {
                        --NewPlayerP.AbsTileZ;
                    }
                }
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
            uint32  TileID = GetTileValue(TileMap, Column, Row, GameState->PlayerP.AbsTileZ);
            if (TileID > 0)
            {
                real32 Gray = 0.5f;
                if (TileID == 2)
                {
                    Gray = 1.f;
                }

                if (TileID > 2)
                {
                    Gray = 0.25f;
                }

                if (Row == (int32)GameState->PlayerP.AbsTileY && Column == (int32)GameState->PlayerP.AbsTileX)
                {
                    Gray = 0.f;
                }

                real32 CentreX = ScreenCenterX + ((real32)RelColumn) * TileSideInPixels - MetersToPixels*GameState->PlayerP.TileRelX;
                real32 CentreY = ScreenCenterY - ((real32)RelRow) * TileSideInPixels + MetersToPixels*GameState->PlayerP.TileRelY;

                real32 MinX = CentreX - 0.5f*TileSideInPixels;
                real32 MinY = CentreY - 0.5f*TileSideInPixels;

                real32 MaxX = CentreX + 0.5f*TileSideInPixels;
                real32 MaxY = CentreY + 0.5f*TileSideInPixels;
                DrawRectangle(Buffer, MinX, MinY, MaxX, MaxY, Gray, Gray, Gray);
            }
        }
    }

    real32 PlayerR = 1.f;
    real32 PlayerG = 1.f;
    real32 PlayerB = 0.f;
    real32 PlayerLeft = ScreenCenterX - 0.5f*MetersToPixels*PlayerWidth;
    real32 PlayerTop = ScreenCenterY - MetersToPixels*PlayerHeight;

    DrawRectangle(Buffer, PlayerLeft, PlayerTop, PlayerLeft + MetersToPixels * PlayerWidth, PlayerTop + MetersToPixels * PlayerHeight, PlayerR, PlayerG, PlayerB);
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
