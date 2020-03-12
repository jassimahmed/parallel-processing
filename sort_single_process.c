#include "A4_sort_helpers.h"

int main( int argc, char *argv[] ){

	if( argc != 2 ){
		printf( "Usage: %s input_file\n", argv[0] );
		return 1;
	}

	read_all( argv[1] );
	sort_words( );

	int curr_line = 0;
	while( text_array[curr_line][0] != '\0' ){
		sprintf( buf, "%s",  text_array[curr_line] );
		write(1,buf,strlen(buf));
		curr_line++;
	}

	sprintf( buf, "Sorting complete!\n" );
	write( 1, buf, strlen(buf) );

	return 0;
}
