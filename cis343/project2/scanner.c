/*	
	Simple Scanner 
	Written by Lawrence O'Boyle and Megan Maher
	*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

enum State { START, STOP, IDENTIFIER, INTEGER, FLOATPOINTNUMBER, BUILD_ID,
			 BUILD_NUM, ADDOP, MINUSOP, MULTOP, DIVOP, ASSIGNOP, STMTTERMOP
};

typedef enum State STATE;

STATE transition( STATE, char );

/*
   The Main Function
   */
int main(  ) {

	char transition_char;

	//Code provided
	STATE current_state = START;
	do {
		if( current_state == IDENTIFIER )
		{
			printf( " – Identifier\n" );
			current_state = START;
		} else if( current_state == INTEGER )
		{
			printf( " – Integer\n" );
			current_state = START;
		}else if( current_state == ADDOP )
		{
			printf( " – AddOp\n" );
			current_state = START;
		}else if( current_state ==  MINUSOP )
		{
			printf( " – MinusOp\n" );
			current_state = START;
		}else if( current_state == MULTOP )
		{
			printf( " – MultOp\n" );
			current_state = START;
		}else if( current_state == DIVOP )
		{
			printf( " – DivOp\n" );
			current_state = START;
		}else if( current_state == ASSIGNOP )
		{
			printf( " – AssignOp\n" );
			current_state = START;
		}else if( current_state == STMTTERMOP )
		{
			printf( " – StmTermOp\n" );
			current_state = START;
		}

		scanf( "%c", &transition_char );

		current_state = transition( current_state,
				transition_char );
	} while( current_state != STOP );   

	return 0;
}

/*
   Transition Function
   */
STATE transition( STATE state, char transition_char )
{
	switch( state )
	{
		case START :
			if( isdigit( transition_char ) )
			{
				state = BUILD_NUM;
				printf( "%c", transition_char );
			}else if( isblank( transition_char ) ||
						transition_char == '\n' )
			{
				state = START;
			}else if( transition_char == '.' )
			{
				state = STOP;
			}else if( isalpha( transition_char ) )
			{
				state = BUILD_ID;
				printf( "%c", transition_char );
			}else if( transition_char == '+' )
			{
				state = ADDOP;
				printf( "%c", transition_char );
			}else if( transition_char == '-' )
			{
				state = MINUSOP;
				printf( "%c", transition_char );
			}else if( transition_char == '*' )
			{
				state = MULTOP;
				printf( "%c", transition_char );
			}else if( transition_char == '/' )
			{
				state = DIVOP;
				printf( "%c", transition_char );
			}else if( transition_char == '=' )
			{
				state = ASSIGNOP;
				printf( "%c", transition_char );
			}else if( transition_char == ';' )
			{
				state = STMTTERMOP;
				printf( "%c", transition_char );
			}
			break;

		case BUILD_ID :
			if( isblank( transition_char ) || transition_char == '\n' )
			{
				state = IDENTIFIER;
				break;
			}
			printf( "%c", transition_char );
			break;

		case BUILD_NUM :
			if( isblank( transition_char ) )
			{
				state = INTEGER;
				break;
			}
			if( !isdigit( transition_char ) )
			{
				state = FLOATPOINTNUMBER;
			}
			printf( "%c", transition_char );
			break;

		case FLOATPOINTNUMBER :
			if( isblank( transition_char ) )
			{
				state = START;
				printf( " - FloatingPointNumber\n" );
				break;
			}
			printf( "%c", transition_char );
			break;

		default:
			printf( "Transition Error.\n" );
			state = STOP;
			break;
	}
	return state;
}
