//  Written by:  Lawrence L. O'Boyle II && Megan Kelly Maher
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "stack.h"
#include "infix2postfix.h"

/* function to convert an infix to postfix */
char *infixToPostfix(char *infixStr)
{
    stack s;
    stackInit( &s );

    char *token = strtok( infixStr, " " );
    char *postfixStr = calloc( postfixStr, 0 );
    char *pop;
    char *space = " ";
    while( token )
    {
        if( isOperand( token ) )
        {
            postfixStr = realloc( postfixStr, strlen( postfixStr )
                    + strlen( token ) + 1 );
            strcat( postfixStr, token );
            
            postfixStr = realloc( postfixStr, strlen( postfixStr )
                    + strlen( space ) + 1 );
            strcat( postfixStr, space );
        }else if( isLeftParen( token ) )
        {
            stackPush( &s, token );
        }else if( isOperator( token ) )
        {
            while( !stackIsEmpty( &s ) && stackPrecedence( stackPeek(
                            &s ) ) >= inputPrecedence( token ) )
            {
                pop = stackPop( &s );
                postfixStr = realloc( postfixStr,strlen( postfixStr )
                        + strlen( pop ) + 1 );
                strcat( postfixStr, pop );
                
                postfixStr = realloc( postfixStr, strlen( postfixStr )
                        + strlen( space ) + 1 );
                strcat( postfixStr, space );
            }
            stackPush( &s, token );
        }else if( isRightParen( token ) )
        {
            while( !isLeftParen( stackPeek( &s ) ) )
            {
                pop = stackPop( &s );
                postfixStr = realloc( postfixStr, strlen( postfixStr )
                        + strlen( pop ) + 1 );
                strcat( postfixStr, pop );
                
                postfixStr = realloc( postfixStr, strlen( postfixStr )
                        + strlen( space ) + 1 );
                strcat( postfixStr, space );
            }
            stackPop( &s );
        }
        token = strtok( NULL, " " );
    }

    while( !stackIsEmpty( &s ) ) 
    {

        pop = stackPop( &s );
        postfixStr = realloc( postfixStr, strlen( postfixStr )
                + strlen( pop ) + 1 );
        strcat( postfixStr, pop );
        
        postfixStr = realloc( postfixStr, strlen( postfixStr )
                + strlen( space ) + 1 );
        strcat( postfixStr, space );
    }
    stackDestroy( &s );
	return postfixStr;
}

/* function that returns true if the string is an operator */
bool isOperator(char *str)
{
    if( *str == '+' || *str == '-' || *str == '*' || *str == '/'
            || *str == '%' || *str == '^' )
        {
            return true;
        }
	return false;
}

/* function that returns true if the string is an operand/integer */
bool isOperand(char *str)
{
    if( !isOperator( str ) && !( *str == '(' || *str == ')' ) ) 
    {
        return true;
    }
	return false;
}

/* function that returns true if the string is a left parenthesis */
bool isLeftParen(char *str)
{
    if( *str == '(' )
        return true;
	return false;
}

/* function that returns true if the string is a right parenthesis */
bool isRightParen(char *str)
{
    if( *str == ')' )
        return true;
	return false;
}

/* function that returns the stack precedence of given operator */
int stackPrecedence(char *str)
{
    if( *str == '+' || *str == '-' ) 
    {
        return 1;
    }
    else if( *str == '*' || *str == '/' || *str == '%' )
    { 
        return 2;
    }
    else if( *str == '^' ) 
    {
        return 3;
    }
    else if( *str == '(' )
    {
        return -1;
    }
    
	return 0;
}

/* function that returns the input precedence of given operator */
int inputPrecedence(char *str)
{
    if( *str == '+' || *str == '-' ) 
    {
        return 1;
    }
    else if( *str == '*' || *str == '/' || *str == '%' )
    { 
        return 2;
    }
    else if( *str == '^' ) 
    {
        return 4;
    }
    else if( *str == '(' )
    {
        return 5;
    }
    
	return 0;
}

/*function that evaluates a postfix expression and returns the result */
int evaluatePostfix(char *postfixStr)
{
    int result,
        num1,
        num2;

    stack s;
    stackInit( &s );
 
    char *token = strtok( postfixStr, " " );
    while( token )
    {
        if( isOperator( token ) )
        {
            num2 = stackPop( &s );
            num1 = stackPop( &s );
            result = applyOperator( num1, num2, token );
            stackPush( &s, result );
        }
        else
        {
            stackPush( &s, atoi( token ) );
        }
        
        token = strtok( NULL, " " );
    }
    result = stackPop( &s );
    stackDestroy( &s );
	return result;
}

/* apply operator to num1 and num2 and return the result */
int applyOperator(int num1, int num2, char *opr)
{
    int result;

    switch( *opr )
    {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            result = num1 / num2;
            break;
        case '%':
            result = num1 % num2;
            break;
        case '^':
            result = pow( num1, num2 );
            break;
        default:
            printf( "Unknown operation\n" );
            result = 0; 
    }
	return result;
}
