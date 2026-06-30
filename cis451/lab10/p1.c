#define ARRAY_SIZE 16*1024 
#define NUM_LOOPS 1000000

int main()
{
    char array[ARRAY_SIZE];
    register int outer_loop;
    register int solution = 0;

    for (outer_loop = 0; outer_loop < NUM_LOOPS; outer_loop++){
            solution *= array[0];
            solution *= array[8192+8];
    }
    return solution;
}
