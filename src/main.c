
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stb_image.h"
#include "stb_image_write.h"

static void error( int result )
{
	fprintf( stderr, "Error: %d", result );
	exit( result );
}

int main( int argc, char *argv[] )
{
	if ( argc <= 0 )
		error( -1 );

	int w;
	int h;
	int component;
	char filename[ 1024 ];
	const int forceComp = 4;

	for ( int input_index = 1; input_index < argc; ++input_index )
	{
		stbi_uc *image = stbi_load( argv[ input_index ], &w, &h, &component, forceComp );

		if ( !image )
		{
			fprintf( stderr, "Failed to open: %s", argv[ input_index ] );
			continue;
		}

		strcpy( filename, argv[ input_index ] );

		int len = (int)strlen( filename );
		int dot_index = -1;

		for ( int string_index = len - 1; string_index >= 0; --string_index )
		{
			if ( filename[ string_index ] == '.' )
			{
				dot_index = string_index;
				break;
			}
		}

		if ( dot_index == -1 )
			dot_index = len - 1;

		filename[ dot_index + 1 ] = 'p';
		filename[ dot_index + 2 ] = 'n';
		filename[ dot_index + 3 ] = 'g';
		filename[ dot_index + 4 ] = '\0';

		fprintf( stdout, "Creating file: %s", filename );

		if ( !stbi_write_png( filename, w, h, forceComp, image, w * forceComp ) )
		{
			fprintf( stderr, "Failed to write: %s", filename );
		}
	}

	return 0;
}

// -- unity build --
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"