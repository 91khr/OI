#include <string>
#include <locale>
#include <codecvt>
#include <cstdlib>
#ifdef _WIN32
#include <Windows.h>
#else // Linux or mac
#include <unistd.h>
#endif // _WIN32

const std::string PreCompile =
    "\"C:/Program Files (x86)/Microsoft Visual Studio/2017/BuildTools/VC/Auxiliary/Build/vcvars64.bat\" > nul ";
const std::string Compiler = "cl";
const std::string ArgsStr = "/nologo /EHsc /W4 /utf-8 /D_CRT_SECURE_NO_WARNINGS ";

struct FileState
{
    bool exist;
#ifdef _WIN32
    FILETIME
#else  // Linux or mac
    int
#endif // _WIN32
    modification;
};

FileState getModification(std::string fname)
{
#ifdef _WIN32
    //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    //std::string narrow = converter.to_bytes(wide_utf16_source_string);
    //std::wstring wfname = converter.from_bytes(fname);
    HANDLE fhandle = CreateFile(fname.c_str(), GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!fhandle)
    {
        return {false};
    }
    FILETIME ret;
    GetFileTime(fhandle, NULL, NULL, &ret);
    CloseHandle(fhandle);
    return {true, ret};
#else  // Linux or mac
    return 0;
#endif // _WIN32
}

void compile(std::string fname)
{
    std::string compileCmd = PreCompile + " && " + Compiler + " " + ArgsStr + " " + fname;
    //printf("Compiling %s\n", compileCmd.c_str());
    system(compileCmd.c_str());
}

int main(int argc, char **argv)
{
    using namespace std::string_literals;

    // Check if force update is needed
    bool force_update = false;
    if (argc >= 4 && "force"s == argv[3])
    {
        force_update = true;
    }

    // Get file modification
    std::string srcfile(argv[1]);
    std::string dstfile(argv[2]);
#ifdef _WIN32
    FILETIME
#else  // Linux or mac
    int
#endif // _WIN32
    srctime = getModification(srcfile).modification;
    FileState dststat = getModification(dstfile);

    // Dst not exist or force wanted
    //int res = CompareFileTime(&dststat.modification, &srctime);
    if (!dststat.exist || force_update ||
    // Dst is modified eariler than src
#ifdef _WIN32
        CompareFileTime(&dststat.modification, &srctime) == -1
#else  // Linux or mac
        true
#endif // _WIN32
    )
    {
        compile(srcfile);
    }

    return 0;
}
