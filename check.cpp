#include <cstdio>
#include <cmath>
#include <string>
#include <cstdlib>

const char outfile[] = "data.in";
const char chkprog[] = "diff std.out prog.out";
FILE *out;

void gendata()
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

