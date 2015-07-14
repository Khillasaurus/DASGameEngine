//=============================================================================
// File:		Texture.cpp
// Created:		2015/02/12
// Last Edited:	2015/02/17
// Copyright:	Daniel Schenker
// Description:	Texture
//=============================================================================

//=============================================================================
//Includes
//=============================================================================

// Daniel Schenker
#include "Texture.h"

//=============================================================================
//Statics
//=============================================================================

//=============================================================================
//Class Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Constuctors
//-----------------------------------------------------------------------------

DSGraphics::Texture::Texture(const char* pImageFile, GLint minMagFiler, GLint wrapMode)
:	mIsTextureLoaded(false)
,	mObjectID(0)
,	mWidth(0)
,	mHeight(0)
{
	//Test if pImageFile is a png by checking the header
	png_byte header[8];

	//Open pImageFile in read only binary mode
	FILE* pFile = nullptr;
	errno_t err;
	err = fopen_s(&pFile, pImageFile, "rb");
	//If the file opened successfully
	if(err == 0)
	{
		//Read the header
		fread(header, 1, 8, pFile);

		//Is the image of type png?
		if(png_sig_cmp(header, 0, 8) == 0)
		{
			//Create png object struct
			png_structp pPngObj = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if(pPngObj != nullptr)
			{
				//Create png info struct
				png_infop pPngInfo = png_create_info_struct(pPngObj);
				if(pPngInfo != nullptr)
				{
					//Create png end info struct
					png_infop pPngInfoEnd = png_create_info_struct(pPngObj);
					if(pPngInfoEnd != nullptr)
					{
						//Png error handling, taken from OpenGL Programming Wikibook, which is unsure if libpng man suggests this, hence switch in if else error logic flow.
						if(setjmp(png_jmpbuf(pPngObj)))
						{
							png_destroy_read_struct(&pPngObj, &pPngInfo, &pPngInfoEnd);
							fclose(pFile);

							mIsTextureLoaded = false;
						}
						else
						{
							//Initialize png reading
							png_init_io(pPngObj, pFile);

							//Inform libpng that the first 8 bytes have already been read
							png_set_sig_bytes(pPngObj, 8);

							//Read all the info up to the image data
							png_read_info(pPngObj, pPngInfo);

							//Image data
							int bitDepth = 0;
							int colorType = 0;

							//Get info about png
							png_get_IHDR(pPngObj, pPngInfo, &mWidth, &mHeight, &bitDepth, &colorType, NULL, NULL, NULL);

							//Row size in bytes
							int rowBytes = png_get_rowbytes(pPngObj, pPngInfo);

							//Allocate the pImageData as a big block, to be given to OpenGL
							png_byte* pImageData = new png_byte[rowBytes * mHeight];
							if(pImageData != nullptr)
							{
								//pRowPointers points to pImageData for reading the png using libpng
								png_bytep* pRowPointers = new png_bytep[mHeight];
								if(pRowPointers != nullptr)
								{
									//Set the individual pRowPointers to point at the correct offsets in pImageData
									for(unsigned int i = 0; i < mHeight; ++i)
									{
										pRowPointers[mHeight - 1 - i] = pImageData + (i * rowBytes);
									}

									//Read the png into pImageData through pRowPointers
									png_read_image(pPngObj, pRowPointers);

									//Generate the OpenGL texture object
									glGenTextures(1, &mObjectID);
									glBindTexture(GL_TEXTURE_2D, mObjectID);

									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
									glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

									glTexImage2D
									(
										GL_TEXTURE_2D,			//target texture
										0,						//level-of-detail. 0 is the base image level.
										GL_RGBA,				//internal format
										mWidth,					//texture width
										mHeight,				//texture height
										0,						//border. In older OpenGL specifications you could create a border around texture (it's really useless), specification 3.3 onwards, this parameter MUST be zero.
										GL_RGBA,				//format of the pixel data
										GL_UNSIGNED_BYTE,		//data type of the pixel data
										(GLvoid*) pImageData	//pointer to the image data in memory
									);

									//Clean up memory
									glBindTexture(GL_TEXTURE_2D, 0);
									png_destroy_read_struct(&pPngObj, &pPngInfo, &pPngInfoEnd);
									delete[] pImageData;
									pImageData = nullptr;
									delete[] pRowPointers;
									pRowPointers = nullptr;
									pPngInfoEnd = nullptr;
									pPngInfo = nullptr;
									pPngObj = nullptr;
									fclose(pFile);
									pFile = nullptr;

									mIsTextureLoaded = true;
								}
								else
								{
									//Clean up memory
									png_destroy_read_struct(&pPngObj, &pPngInfo, &pPngInfoEnd);
									pRowPointers = nullptr;
									delete[] pImageData;
									pImageData = nullptr;
									pPngInfoEnd = nullptr;
									pPngInfo = nullptr;
									pPngObj = nullptr;
									fclose(pFile);
									pFile = nullptr;
									
									mIsTextureLoaded = false;
								}
							}
							else
							{
								//Clean up memory
								png_destroy_read_struct(&pPngObj, &pPngInfo, &pPngInfoEnd);
								pImageData = nullptr;
								pPngInfoEnd = nullptr;
								pPngInfo = nullptr;
								pPngObj = nullptr;
								fclose(pFile);
								pFile = nullptr;

								mIsTextureLoaded = false;
							}

						}
					}
					else
					{
						printf("ERRPR: Could not create png info end struct for image \"%s\".\n", pImageFile);
						png_destroy_read_struct(&pPngObj, &pPngInfo, (png_infopp) NULL);
						pPngInfoEnd = nullptr;
						pPngInfo = nullptr;
						pPngObj = nullptr;
						fclose(pFile);
						pFile = nullptr;

						mIsTextureLoaded = false;
					}
				}
				else
				{
					printf("ERRPR: Could not create png info struct for image \"%s\".\n", pImageFile);
					png_destroy_read_struct(&pPngObj, (png_infopp) NULL, (png_infopp) NULL);
					pPngInfo = nullptr;
					pPngObj = nullptr;
					fclose(pFile);
					pFile = nullptr;

					mIsTextureLoaded = false;
				}
			}
			else
			{
				printf("ERROR: Could not allocate and initialize png struct for image \"%s\".\n", pImageFile);
				pPngObj = nullptr;
				fclose(pFile);
				pFile = nullptr;

				mIsTextureLoaded = false;
			}
		}
		else
		{
			printf("ERROR: Image \"%s\" is not of type png.\n", pImageFile);
			fclose(pFile);
			pFile = nullptr;

			mIsTextureLoaded = false;
		}
	}
	else
	{
		printf("ERROR: Image \"%s\" could not be opened.\n", pImageFile);
		pFile = nullptr;

		mIsTextureLoaded = false;
	}
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

DSGraphics::Texture::~Texture()
{
	glDeleteTextures(1, &mObjectID);
}

//-----------------------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------

bool DSGraphics::Texture::GetIsTextureLoaded() const
{
	return mIsTextureLoaded;
}

//-----------------------------------------------------------------------------

GLuint DSGraphics::Texture::GetObjectID() const
{
	return mObjectID;
}

//-----------------------------------------------------------------------------

png_uint_32 DSGraphics::Texture::GetWidth() const
{
	return mWidth;
}

//-----------------------------------------------------------------------------

png_uint_32 DSGraphics::Texture::GetHeight() const
{
	return mHeight;
}

//-----------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------