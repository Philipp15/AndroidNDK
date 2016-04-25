//--------------------------------------------------------------------------------------
// BMP Loader : 8-bit / 24-bit / 32-bit, Compression RLE not supported
//--------------------------------------------------------------------------------------


#ifndef BMPLOADER_H
#define BMPLOADER_H

#include <Windows.h>
#include <iostream>
#include <GLES2\gl2.h>
#include <GLES2\gl2ext.h>
#include <GLES2\gl2platform.h>

#define BMP_MAGIC 0x4D42 // "MB"

struct BMP_HEADER
{
	UINT32 Size;
	UINT16 Reserved1;
	UINT16 Reserved2;
	UINT32 OffBits;
};

struct BMP_INFO_HEADER
{
	UINT32 Size;
	INT32  Width;
	INT32  Height;
	UINT16 Planes;
	UINT16 BitCount;
	UINT32 Compression;
	UINT32 SizeImage;
	INT32  XPelsPerMeter;
	INT32  YPelsPerMeter;
	UINT32 ClrUsed;
	UINT32 ClrImportant;
}; 

struct BMP_IMAGE_DATA
{
	UINT32 Width;
	UINT32 Height;
	UINT32 Components;
	UINT8* Pixels;
};

int LoadBMPTextureFromFile(const std::string& Filename, GLuint&  textureHandle, GLuint programId);

#endif