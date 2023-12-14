
// read_double_bin

// write_double_ascii

#include "numlib.h"
#include <assert.h>

int main(int argc, char *argv[])
{
    assert(argc == 1);
    int count = 0;
    double current_number = 0.0;
    double sum = 0.0;
    int reached_EOF = 0;

    while (reached_EOF == 0)
    {

        int r = read_double_bin(stdin, &current_number);
        if (r == EOF)
        {
            reached_EOF = 1;
        }
        if (r == 0)
        {

            sum += current_number;
            count++;
        }
    }

    double average = sum / (double)count;

    write_double_ascii(stdout, average);
    printf("\n");
}
