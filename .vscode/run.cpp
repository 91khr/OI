#include <cstdlib>
#include <string>

int main(int argc, char **argv)
{
    using namespace std::string_literals;

    // Init calling command
    std::string callcmd(argv[1]);
    if (argc >= 3)
    {
        callcmd += " < "s + argv[2];
    }
    if (argc >= 4)
    {
        callcmd += " > "s + argv[3];
    }

    // Call cmd to run program
    //printf("'%s'\n", callcmd.c_str());
    system(callcmd.c_str());

    // Pause to see result
    if (argc <= 3)
    {
        system("pause");
    }
    return 0;
}
