#include "pch.h"
#include "Face.h"

CFace::CFace(void)
{
}

CFace::~CFace(void)
{
	if (vertexIndex != NULL)
	{
		delete[] vertexIndex;
		vertexIndex = NULL;
	}
	if (normalIndex != NULL)
	{
		delete[] normalIndex;
		normalIndex = NULL;
	}
	if (textureIndex != NULL)
	{
		delete[] textureIndex;
		textureIndex = NULL;
	}
}

void CFace::InitializeQueue(void)
{
	vertexIndex = new int[vertexNumber];
	normalIndex = new int[vertexNumber];
	textureIndex = new int[vertexNumber];
}
