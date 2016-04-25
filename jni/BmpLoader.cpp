#include "BmpLoader.h"


int LoadBMPTextureFromFile(const std::string& Filename, GLuint& texture, GLuint programId)
{
#ifdef _MSC_VER
	FILE* File = NULL;
	fopen_s(&File, Filename.c_str(), "rb");
#else
	FILE* File = fopen(Filename.c_str(), "rb");
#endif

	if (File == NULL)
	{
		return NULL;
	}

	UINT16 MagicNumber = 0;
	fread(&MagicNumber, sizeof(UINT16), 1, File);
	if (MagicNumber != BMP_MAGIC)
	{
		fclose(File);
		return 0;
	}

	BMP_HEADER BMPHeader;
	fread(&BMPHeader, sizeof(BMP_HEADER), 1, File);

	BMP_INFO_HEADER BMPInfoHeader;
	fread(&BMPInfoHeader, sizeof(BMP_INFO_HEADER), 1, File);

	if ((BMPInfoHeader.Size != sizeof(BMP_INFO_HEADER)) || (BMPInfoHeader.Planes != 1))
	{
		fclose(File);
		return 0;
	}

	if (BMPInfoHeader.Compression != 0)
	{
		fclose(File);
		return 0;
	}

	if (BMPInfoHeader.BitCount == 1)
	{
		fclose(File);
		return 0;
	}
	else if (BMPInfoHeader.BitCount == 4)
	{
		fclose(File);
		return 0;
	}
	else if (BMPInfoHeader.BitCount == 16)
	{
		fclose(File);
		return 0;
	}

	fseek(File, BMPHeader.OffBits, SEEK_SET);

	BMP_IMAGE_DATA* BMPImageData = new BMP_IMAGE_DATA;
	memset(BMPImageData, 0, sizeof(BMP_IMAGE_DATA));

	if (BMPInfoHeader.BitCount == 8)
		BMPImageData->Components = 1;
	else if (BMPInfoHeader.BitCount == 24)
		BMPImageData->Components = 3;
	else if (BMPInfoHeader.BitCount == 32)
		BMPImageData->Components = 4;

	UINT32 BufferSize = BMPInfoHeader.Width * BMPInfoHeader.Height * BMPImageData->Components;

	BMPImageData->Pixels = new UINT8[BufferSize];
	fread(BMPImageData->Pixels, 1, BufferSize, File);


	GLuint w = BMPInfoHeader.Width;
	GLuint h = BMPInfoHeader.Height;
	//GLuint arrayOfTexture[2];

		glGenTextures(1, &texture);             // Generate a texture
	
		//glActiveTexture(GL_TEXTURE0);

//		glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

		/*GLint textureSampler = glGetUniformLocation(programId, "myAnotherTexture");

		glUniform1i(textureSampler, 0);


		 textureSampler = glGetUniformLocation(programId, "myTexture");

		glUniform1i(textureSampler, 1);*/




		GLint mode = GL_RGB;                   // Set the mode
	
		
		//// Create the texture. We get the offsets from the image, then we use it with the image's
		//// pixel data to create it.
		//must be multiple of 2 1024 x 1024, 512 x 512 , 256 x 256
		glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

		glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, BMPImageData->Pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		


	fclose(File);

	BMPImageData->Width = BMPInfoHeader.Width;
	BMPImageData->Height = BMPInfoHeader.Height;

	delete[] BMPImageData->Pixels;

	return 1;
}







// -- Do not need this as we can convert the colors in shader with a simple function 
// e.g. texture2D( myTexture, UV ).bgra

//UINT8 tmpRGB = 0;
//for (ULONG i = 0; i < BMPInfoHeader.SizeImage; i += 3)
//{
//	if (i + 2 > BMPInfoHeader.SizeImage) // prevent going beyond allocated memory
//	{
//		break;
//	}

//	tmpRGB = BMPImageData->Pixels[i];
//	BMPImageData->Pixels[i] = BMPImageData->Pixels[i + 2];
//	BMPImageData->Pixels[i + 2] = tmpRGB;
//}