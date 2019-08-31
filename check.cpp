#include <cstdio>
#include <cmath>
#include <string>
#include <cstdlib>
#include <algorithm>

const int MaxN = 19;

int n;
int sets[MaxN];

void gendata()
{
}

int solve_std()
{
}

int solve_prog()
{
}

int main(int argc, char **argv)
{
    srand(19260817);
    std::string stdprog(argv[1]), testprog(argv[2]);
    stdprog = stdprog + "<" + outfile + ">" + "std.out";
    testprog = testprog + "<" + outfile + ">" + "prog.out";
    int data = 0;
    do
    {
        out = fopen(outfile, "w");
        printf("\rdata %d", ++data);
        fflush(stdout);
        gendata();
        system(stdprog.c_str());
        system(testprog.c_str());
        fclose(out);
    } while (system(chkprog) == 0);
    printf("\n");
}

