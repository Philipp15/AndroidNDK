

/*
#include "opengl_jni_Natives.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdio.h>
#include "open.h"
#include <android/log.h>
#include <malloc.h>
#include <string.h>
#include <GLES2/gl2.h>
#include <stdint.h>
#define APPNAME "OpenGLEngine"
*/




//#include "opengl_jni_Natives.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdio.h>
#include "open.h"
#include <android/log.h>
#include <malloc.h>
#include <string.h>
#include <GLES2/gl2.h>
#include <stdint.h>
#include <stdlib.h>
#define APPNAME "OpenGLEngine"
#include <string>






typedef uint32_t UINT32;
typedef uint16_t UINT16;
typedef uint8_t UINT8;
typedef int INT32;

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




 open graphicsEngine;
  JNIEnv * m_ptrJEnv;
  jobject m_assetManager;


char * LoadAsset( char * directory,  char * filename,  size_t &fileSize);

GLuint LoadBMPTextureFromFile( char * directory, char  const * filename,  GLuint& texture)
{

    AAssetManager* ptrAssetMan =  AAssetManager_fromJava(m_ptrJEnv, m_assetManager);

   AAssetDir* assetDir = AAssetManager_openDir(ptrAssetMan, directory); // get the directory to search in

  const char * currentFile;

     while ( (currentFile = AAssetDir_getNextFileName(assetDir)) != NULL )  //currentFile = Next compare to Wanted File AND currenFILE cannot be NULL
      {
        if( strcmp(currentFile, filename ) != 0)
        {
          continue;
        }
          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File %s found :", currentFile);
          AAsset* asset = AAssetManager_open(ptrAssetMan, filename, AASSET_MODE_UNKNOWN); // open Asset


          UINT16 MagicNumber = 0;

          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Reading magic number:");
          AAsset_read(asset, &MagicNumber, sizeof(UINT16) );


          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Finished reading magic number:");

          if (MagicNumber != BMP_MAGIC)
          {
             __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File is not bmp ");
          }

          BMP_HEADER BMPHeader;
          BMP_INFO_HEADER BMPInfoHeader;

          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, " Reading BMP Header :");

          AAsset_read(asset, &BMPHeader, sizeof(BMP_HEADER) );

          AAsset_read(asset, &BMPInfoHeader, sizeof(BMP_INFO_HEADER) );


          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Finished reading BMP Header:");

          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "BMP Header size contents %d", BMPHeader.Size);



            if ((BMPInfoHeader.Size != sizeof(BMP_INFO_HEADER)) || (BMPInfoHeader.Planes != 1))
            {
              __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "size Problem planes : %d size: %d  ", BMPInfoHeader.Planes, BMPInfoHeader.Size);
              AAsset_close(asset);
              return 0;
            }

            if (BMPInfoHeader.Compression != 0)
            {
              __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "compression Problem" );
              AAsset_close(asset);
              return 0;
            }

            if (BMPInfoHeader.BitCount == 1)
            {
              __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "BITCOUNT Problem 1 " );
              AAsset_close(asset);
              return 0;
            }
            else if (BMPInfoHeader.BitCount == 4)
            {
              __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "BITCOUNT Problem 2" );
              AAsset_close(asset);
              return 0;
            }
            else if (BMPInfoHeader.BitCount == 16)
            {
              __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "BITCOUNT Problem 3" );
              AAsset_close(asset);
              return 0;
            }


          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "BMP Header off bits %d", BMPHeader.OffBits);

            AAsset_seek(asset,  BMPHeader.OffBits, SEEK_SET);



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

              AAsset_read(asset, BMPImageData->Pixels, BufferSize );
    

              GLuint w = BMPInfoHeader.Width;
              GLuint h = BMPInfoHeader.Height;

              __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "BMP Header width %d", w);

              __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "BMP Header height %d", h);



              glGenTextures(1, &texture);

              GLint mode = GL_RGB;                   // Set the mode


              //// Create the texture. We get the offsets from the image, then we use it with the image's
              //// pixel data to create it.
              //must be multiple of 2 1024 x 1024, 512 x 512 , 256 x 256
              glBindTexture(GL_TEXTURE_2D, texture); // Bind that texture temporarily

              glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, BMPImageData->Pixels);

              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


              

              BMPImageData->Width = BMPInfoHeader.Width;
              BMPImageData->Height = BMPInfoHeader.Height;

              delete[] BMPImageData->Pixels;
              AAsset_close(asset);
      }


return texture;

}



char * LoadAsset( char * directory,  char * filename,  size_t &fileSize)
{

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "In loading assets function");

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Loading --  %s -- now", filename);


      char * resourceContents = NULL;
size_t fileSizesss;
    if(m_assetManager == NULL)
    {
      __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Asset manager is NULL ");
      return NULL;
    }
  AAssetManager* ptrAssetMan =  AAssetManager_fromJava(m_ptrJEnv, m_assetManager);

     

    if(m_ptrJEnv->ExceptionCheck())
    {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Got exception in opengl_jni_Natives::LoadAsset() ");
    }

    if (ptrAssetMan == NULL)
    {
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Failed to initialise Asset_manager "); 

    }else
    {
      __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Succeeded in getting Asset manager : %s", (char *)ptrAssetMan);
    }

    AAssetDir* assetDir = AAssetManager_openDir(ptrAssetMan, directory); // get the directory to search in

  const char * currentFile;

     while ( (currentFile = AAssetDir_getNextFileName(assetDir)) != NULL )  //currentFile = Next compare to Wanted File AND currenFILE cannot be NULL
      {
        if( strcmp(currentFile, filename ) != 0)
        {
          continue;
        }
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File %s found :", currentFile);
        AAsset* asset = AAssetManager_open(ptrAssetMan, filename, AASSET_MODE_UNKNOWN); // open Asset
          size_t fileLength =  AAsset_getLength(asset);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Length of file: %d ", fileLength);      

          resourceContents = (char *)malloc(fileLength+1); // allocate space
             
          AAsset_read(asset, resourceContents, fileLength); // read file in
          resourceContents[fileLength] = '\0'; //append delimieter
          fileSize = fileLength+1;

       /*   for (size_t i =0; i < fileLength -1 ; i++)
          {
                  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Contents of Asset: %c ", resourceContents[i]);            
          }
*/


          AAsset_close(asset); // close asset
          __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Closing asset");      
      }
        AAssetDir_close(assetDir); // close directory
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Closing directory");   


//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Loading TExture now ");      


//LoadTexture("","textureRed.bmp",fileSizesss) ;

//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Loading TExture now ");      


  return resourceContents;
}

extern "C" {

 
 

//JNIEXPORT jint JNICALL Java_opengl_jni_Natives_NativeRender
 // (JNIEnv * env, jclass jc)
  JNIEXPORT jint JNICALL Java_com_example_philipp_myapplication_Natives_NativeRender
  (JNIEnv * env, jclass jc)
  {
   //__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Entering DRAW FUNCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  		
  		graphicsEngine.DrawFrame();
     
  }

//GLuint LoadBMPTextureFromFile( char * directory, char const * filename,  GLuint& texture);











/*
 * Class:     opengl_jni_Natives
 * Method:    NativeInit
 * Signature: ()V
 */
//JNIEXPORT void JNICALL Java_opengl_jni_Natives_NativeInit
  //(JNIEnv * env, jclass, jobject assetMan)
 JNIEXPORT void JNICALL Java_com_example_philipp_myapplication_Natives_NativeInit
  (JNIEnv *env, jclass, jobject assetMan)
  {

    m_ptrJEnv = env;
    m_assetManager = assetMan;


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Entering INIT FUNCTION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

 //size_t FragmentShaderCodeSize =0;

//char * FragmentShader = LoadAsset("",(const char *) "FragmentShaderCode.glsl", FragmentShaderCodeSize);

//size_t VertexShaderCodeSize =0;

//char * VertexShader = LoadAsset("",(const char *) "VertexShaderCode.glsl", VertexShaderCodeSize);
  //      __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Alive");      

//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Size %d ",  FragmentShaderCodeSize);      
//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Size %d ",  VertexShaderCodeSize);      

/*for (int i =0; i < FragmentShaderCodeSize; i++)
{
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "FragmentShader Contents");      
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "buf contents %c", FragmentShader[i]);      
}

for (int i =0; i < VertexShaderCodeSize; i++)
{
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "VertexShader Contents");      
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "buf contents %c", VertexShader[i]);      
}*/


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Calling INIT on GRAPHICS ENGINE");         
  		graphicsEngine.Init(NULL, NULL);
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "INIT DONE");        	

//free(FragmentShader);
//free(VertexShader);




  
  

  }


}









































  /*    jclass ResLoader = NULL;

if(env->ExceptionCheck())
{
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Got exception finding class :( ");

}

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Gooooooood so far!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

if(assetMan == NULL)
{

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "jobject points to null");

}

      //AAssetManager* ptrAssetMan =  AAssetManager_fromJava(env, assetMan);

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "past line getting it from java");

if(env->ExceptionCheck())
{
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Got exception :( ");
}

      

    AAssetDir* assetDir = AAssetManager_openDir(ptrAssetMan, "");


    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Got directory :");

    const char Fragmentfilename[50] = "FragmentShaderCode.glsl";
    const char Vertexfilename[50] = "VertexShaderCode.glsl";

    const char* vertexFile = (const char*) Vertexfilename;
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Assigning filename :");

const char * file;

    while ( (file = AAssetDir_getNextFileName(assetDir) ) != NULL )  // search for file
    {
      if(strcmp (file, vertexFile ) != 0 )
      {
      __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Compared value  %s :", vertexFile);                
      __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Missing file %s :", file);              
        continue;
      }

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "In the loop :");      

__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "vertexfile %s :" , vertexFile);


        AAsset* asset = AAssetManager_open(ptrAssetMan, vertexFile, AASSET_MODE_UNKNOWN);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Opened asset :");      
//        char buf[BUFSIZ];
        int nb_read = 0;
 __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Still Alive :");      
 __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Opened file for reading ");      
//  while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
 //     fread(buf, nb_read, 1, out);
 size_t TextLength =  AAsset_getLength(asset);
//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Length of file: %d ", TextLength);      

  char * VertexShader = (char *)malloc(TextLength );

  AAsset_read(asset, VertexShader, TextLength);


    VertexShader[TextLength] = '\0';



__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Printing contents no iteration %s", VertexShader);      
  AAsset_close(asset);
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Closing  asset");      
    }



/*
const char* FragmentFile = (const char*) Fragmentfilename;
while ((FragmentFile = AAssetDir_getNextFileName(assetDir)) != NULL)  // search for file
    {
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "In the loop :");      
        AAsset* asset = AAssetManager_open(ptrAssetMan, FragmentFile, AASSET_MODE_UNKNOWN);
        __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Opened asset :");      
//        char buf[BUFSIZ];
        int nb_read = 0;
 __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Still Alive :");      
 __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Opened file for reading ");      
//  while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
 //     fread(buf, nb_read, 1, out);
 size_t TextLength =  AAsset_getLength(asset);
__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Length of file: %d ", TextLength);      

  char * VertexShader = (char *)malloc(TextLength);

  AAsset_read(asset, VertexShader, TextLength);


__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "buf contents %s", VertexShader);      
  AAsset_close(asset);
  __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Closing  asset");      
    }

*/




//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "out of the loop");      


//    AAssetDir_close(assetDir);
//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Closing directory");      
//__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Victory :");