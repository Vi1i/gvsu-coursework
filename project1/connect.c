/**
 *
 * Connect Four: The Game
 * Written by Lawrence O'Boyle and Megan Maher
 *
 */

#include <stdio.h>
#include <stdbool.h>

#define KNRM  "\x1B[0m"     //NORMAL setting that the terminal is
#define KRED  "\x1B[31m"    //RED
#define KGRN  "\x1B[32m"    //GREEN
#define KYEL  "\x1B[33m"    //YELLOW
#define KBLU  "\x1B[34m"    //BLUE
#define KMAG  "\x1B[35m"    //MAGENTA
#define KCYN  "\x1B[36m"    //CYAN
#define KWHT  "\x1B[37m"    //WHITE

int board_size;
char * PLAYER = "XO";
int connect_size;
int * column_count;
char ** board;
int turn_count;
int last_play;

void initiate( int, int );
void initiate_input( int, int );
void play( int, int );
char get_turn( int );
int get_move( char );
void play_piece( char, int );
void print_board( );
bool check_if_legal( int );
bool check_win( );
bool check_diag( );
bool check_hor( );
bool check_ver( );

/**
 * Main Function
 * */

int main( int argc, char * argv[ ] )
{
	 if( argc != 3 )
	 {
		printf( "%sInvalid arguments!%s\n", KRED, KNRM );
		printf( "%sconnect <Board_Size> <Connection_Size>\n%s", KGRN, KNRM );
		return 1;
	 }
	 int b_size = atoi( argv[ 1 ] ),
		 c_size = atoi( argv[ 2 ] );

	 play( b_size, c_size );
	 return 0;
}
 
 void initiate( int b_size, int c_size )
 {
	 last_play = 0;
	 board_size = b_size;
	 connect_size = c_size;
     int z,
		 y;
	 column_count = ( int * ) malloc( board_size * sizeof( int ) );

     board = ( char ** ) malloc( board_size * sizeof( char * ) );
     for( z = 0; z < board_size; z++ )
     {
         board[ z ] = (char * ) malloc( board_size * sizeof( char ) );
	 }

	 for( z = 0; z < board_size; z++ )
	 {
		 column_count[ z ] = 0;
		 for( y = 0; y < board_size; y++ )
		 {
			 board[ z ][ y ] = '-';
		 }
	 }
 }

char get_turn( int turn )
{
	return PLAYER[ turn % 2 ];
}

int get_move( char piece )
{
	int player;
	int play;
	char b_play[ 2 ];
	
	switch( piece )
	{
		case 'X':
			  player = 1;
			  break;
		case 'O':
			  player = 2;
			  break;

		default:
			  printf( "Piece Error!" );
	}

	do
	{
		do
		{
			printf( "%sPlayer %d's turn, select your column of play: %s", KMAG, player, KNRM );
			scanf( "%s", &b_play );
			play = atoi( b_play );
			if( !play )
			{
				printf( "%s%s%s, valid input is: %s1-%d.%s\n", KRED, b_play, KNRM, KGRN, board_size, KNRM );
				print_board( );
			}
		}while( !play );
	}while( !check_if_legal( play ) );

	return play - 1;
}

bool check_if_legal( int play )
{
	if( play <= 0 || play > board_size )
	{
		printf( "\n%sInvalid range for board.%s\n", KRED, KNRM );
		printf( "%sPlease enter a valid move.%s\n", KRED, KNRM );
		print_board( );
		return false;
	}

	if( column_count[ play - 1 ] == board_size )
	{
		printf( "\n%sColumn full, please select another column.%s\n", KRED, KNRM );
		printf( "%sPlease enter a valid move.%s\n", KRED, KNRM );
		print_board( );
		return false;
	}
	return true;
}

void play_piece( char piece, int column )
{
	board[ column_count[ column ] ][ column ] = piece;
	column_count[ column ]++;
}

void play( int b_size, int c_size )
{
	initiate_input( b_size, c_size );
	turn_count = 0;
	print_board( );
	while( !check_win( ) )
	{
		last_play = get_move( get_turn( turn_count ) );
		play_piece( get_turn( turn_count ), last_play );
		turn_count++;
		print_board( );
	}
	printf( "%sPlayer %d is the winner!%s\n", KGRN, ( ( turn_count - 1 ) % 2 ) + 1, KNRM );
}

void initiate_input( int b_size, int c_size )
{
	if( b_size < 10 || b_size > 20 )
	{
		printf( "%sBoard size defaulted to 10.%s\n", KYEL, KNRM );
		b_size = 10;
	}
	if( c_size < 4 || c_size > 8 )
	{
		printf("%sConnect size defaulted to 4.%s\n", KYEL, KNRM );
		c_size = 4;
	}
	initiate( b_size, c_size );
}

void print_board( )
{
    int x, 
		y, 
		z;
	for( x = 0; x < board_size; x++ )
	{
		if( x + 1 < 10 )
		{
        	printf( "  %s%d%s ", KCYN, x + 1, KNRM );    
		}else
		{
			printf( " %s%d%s ", KCYN, x + 1, KNRM );
		}
	}
	printf( "\n" );

	for( x = 0; x < board_size * 4 + 1; x++ )
	{
        printf( "%s-%s", KYEL, KNRM );    
	}
	printf( "\n" );

    for( x = board_size - 1; x >= 0; x-- ) {
        printf( "%s|%s", KYEL, KNRM );    
        for( y = 0; y < board_size; y++ ) {
			if( board[ x ][ y ] == PLAYER[ 0 ] )
			{
				printf( "%s %c %s", KRED, board[ x ][ y ], KNRM );
			}else if( board[ x ][ y ] == PLAYER[ 1 ] )
			{
				printf( "%s %c %s", KBLU, board[ x ][ y ], KNRM );
			}else
			{

				printf( "   " );
			}
            printf( "%s|%s", KYEL, KNRM );
        }
		printf( "\n" );

		for( z = 0; z < board_size * 4 + 1; z++ )
		{
			printf( "%s-%s", KYEL, KNRM );    
		}
			printf( "\n" );
    }
	printf( "\n" );
}

bool check_win( )
{
	if( turn_count < connect_size )
	{
		return false;
	}
	return check_hor( ) || check_ver( ) || check_diag( );
}

bool check_hor( )
{
	int l_count = 0,
		r_count = 0,
		z,
		count;
	for( z = last_play + 1; z < board_size; z++ )
	{
		if( board[ column_count[ last_play ] - 1 ][ z ] == get_turn( turn_count - 1 ) )
		{
			r_count++;
		}else
		{
			break;
		}
	}

	for( z = last_play - 1; z >= 0; z-- )
	{
		if( board[ column_count[ last_play ] - 1 ][ z ] == get_turn( turn_count - 1 ) )
		{
			l_count++;
		}else
		{
			break;
		}
	}

	count = l_count + r_count + 1;
	if( count >= connect_size )
	{
		return true;
	}
	return false;
}

bool check_ver( )
{
	if( column_count[ last_play ] < connect_size )
	{
		return false;
	}
	int up_count = 0,
		dwn_count = 0,
		z,
		count;

	for( z = last_play + 1; z < board_size; z++ )
	{
		if( board[ z ][ last_play ] == get_turn( turn_count - 1 ) )
		{
			up_count++;
		}else
		{
			break;
		}
	}

	for( z = last_play -1; z >= 0; z-- )
	{
		if( board[ z ][ last_play ] == get_turn( turn_count - 1 ) )
		{
			dwn_count++;
		}else
		{
			break;
		}
	}

	count = up_count + dwn_count + 1;
	if( count >= connect_size )
	{
		return true;
	}
	return false;
}

bool check_diag( )
{
	bool up_r = true,
		 up_l = true,
		 dwn_l = true,
		 dwn_r = true;
	int z,
		y,
		l_up = 0,
		l_dwn = 0,
		r_up = 0,
		r_dwn = 0,
		column = column_count[ last_play ] - 1,
		count1,
		count2;

	y = 0;
	for( z = last_play + 1; z < board_size; z++ )
	{
		y++;
		//Checks the up right section of the diagnol.
		if( column + y < board_size && up_r )
		{
			if( board[ column + y  ][ z ] == get_turn( turn_count - 1 ) )
			{
				r_up++;
			}else
			{
				up_r = false;
			}
		}
		//Checks the down right section of the diagnol.
		if( column - y >= 0 && dwn_r )
		{
			if( board[ column - y  ][ z ] == get_turn( turn_count - 1 ) )
			{
				r_dwn++;
			}else
			{
				dwn_r = false;
			}
		}
	}
	y = 0;
	for( z = last_play - 1 ; z >= 0; z-- )
	{
		y++;
		//Checks the up left section of the diagnol.
		if( column + y < board_size && up_l )
		{
			if( board[ column + y  ][ z ] == get_turn( turn_count - 1 ) )
			{
				l_up++;
			}else
			{
				up_l = false;
			}
		}
		//Checks the down left section of the diagnol.
		if( column - y >= 0 && dwn_l )
		{
			if( board[ column - y  ][ z ] == get_turn( turn_count - 1 ) )
			{
				l_dwn++;
			}else
			{
				dwn_l = false;
			}
		}
	}

	count1 = l_up + r_dwn + 1;
	count2 = l_dwn + r_up + 1;
	if( count1 >= connect_size || count2 >= connect_size )
	{
		return true;
	}
	return false;
}
