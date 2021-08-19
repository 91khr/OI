# // {{{ Premable and constants
#if 0
if [ `stat -c %Y check.cpp` -lt `stat -c %Y check` ] ||
    g++ $0 -std=c++20 -Wall -Wextra -Weffc++ -g -O0 -Wpedantic -o check
then
    echo End compilation
    exec ./check
else
    exit $?
fi
#endif
#include <concepts>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <csignal>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <sys/wait.h>

#include <tuple>

const char progfname[] = "./a.out";
const char datafname[] = "test.in";
const char ansfname[] = "test.ans";
const char logfname[] = "test.log";
const char outfname[] = "test.out";
// }}} End premable and constants

// {{{ Utils
// {{{ Random generation

/// Generate an integer in range [lo, hi]
int randint(int lo, int hi)
{
    return rand() % (hi - lo + 1) + lo;
}
/// Generate a vector from generator `gen`
/// Ensure that `gen` is called sequently
/// @param gen index -> value
template<class T>
requires requires(T a, int b) { { a(b) } -> std::movable; }
auto randarr(int size, const T &gen)
{
    std::vector<std::invoke_result_t<T, int>> res;
    res.reserve(size);
    for (int i = 0; i < size; ++i)
        res.push_back(gen(i));
    return res;
}
/// Generate a vector from generator `gen`
/// Ensure that `gen` is called sequently
template<class T>
requires requires(T a) { { a() } -> std::movable; }
auto randarr(int size, const T &gen)
{
    return randarr(size, [&gen] (int) { return gen(); });
}
// }}} End random generation

// {{{ Containers
template<class ...Elem>
struct Cons;
template<>
struct Cons<>
{
    static constexpr int Size = 0;
    template<unsigned Index>
    consteval void at() const noexcept { static_assert(Index < 0, "Null Cons is not indicible"); }
};
template<class First, class ...Rest>
struct Cons<First, Rest...>
{
    First car;
    Cons<Rest...> cdr;
    static constexpr int Size = Cons<Rest...>::Size + 1;
    template<unsigned Index>
    constexpr const auto &at() const noexcept
    {
        static_assert(Index < Size, "Index overflow");
        if constexpr (Index == 0)
            return car;
        else
            return cdr.template at<Index - 1>();
    }
    template<unsigned Index>
    constexpr auto &at() noexcept
    {
        const auto &self = *this;
        return self.template at<Index>();
    }
    constexpr Cons(const First &a, const Cons<Rest...> &r) : car(a), cdr(r) {}
    constexpr Cons(const First &a, const Rest &...r) : car(a), cdr(r...) {}
};
template<class Car, class ...Cdr>
Cons(Car, Cons<Cdr...>) -> Cons<Car, Cdr...>;
template<class ...It>
Cons(It...) -> Cons<It...>;
// }}} End containers

// {{{ Stringify and format
using std::to_string;
std::string to_string(const std::string &s) { return s; }
template<class T>
std::string to_string(const std::vector<T> &val, const std::string &delim = " ")
{
    if (val.empty())
        return "";
    std::string res = to_string(val.front());
    for (auto it = val.begin() + 1; it != val.end(); ++it)
        res += delim + to_string(*it);
    return res;
}
template<bool First = true, class ...Elem>
std::string to_string(const Cons<Elem...> &val, const std::string delim = " ")
{
    if constexpr (sizeof...(Elem) != 0)
        return (First ? "" : delim) + to_string(val.car) + to_string<false>(val.cdr, delim);
    else
        return "";
}

/// Forcefully provide/override a `to_string` for given object
template<class F, class T>
requires requires(F a, T b) { { a(b) } -> std::convertible_to<std::string>; }
class Stringifier
{
private:
    F fmt;
    T data;
public:
    Stringifier(const F &f, const T &d) : fmt(f), data(d) {}
    std::string to_string() const { return fmt(data); }
};
template<class F, class T>
std::string to_string(const Stringifier<F, T> &fmt) { return fmt.to_string(); }

std::string format(const char *fmt) { return fmt; }
std::string format(const char *fmt, const auto &car, const auto &...cdr)
{
    std::string res;
    const char *it = fmt;
    for (; *it; ++it)
        if (*it == '$')
        {
            if (*++it && *it == '$')
            {
                res.append(fmt, it);
                fmt = it + 1;
            }
            else
            {
                res.append(fmt, it - 1);
                res += to_string(car) + format(it, cdr...);
                return res;
            }
        }
    res.append(fmt, it);
    return res;
}
// }}} End stringify and format
// }}} End utils

namespace DataGen
{
void refresh();
std::string getdata();
std::string getans();
//bool SPJ(const std::string &out);
struct SPJ_struct { bool operator()(const std::string &out); } SPJ;
}  // End namespace DataGen

// {{{ Check utils
void printStat(auto ...);
struct CheckUtil
{
public:
    int timestamp = 0;
private:
    using PipeT = int[2];
    PipeT inpipe, outpipe, errpipe;
    char buf[BUFSIZ];
    static constexpr int ReadEnd = 0, WriteEnd = 1;

    void execChild()  // {{{
    {
        pipe(inpipe);
        pipe(outpipe);
        pipe(errpipe);
        if (fork() == 0)
        {
            dup2(inpipe[ReadEnd], STDIN_FILENO);
            dup2(outpipe[WriteEnd], STDOUT_FILENO);
            dup2(errpipe[WriteEnd], STDERR_FILENO);
            for (auto fd : { inpipe[0], inpipe[1], outpipe[0], outpipe[1], errpipe[0], errpipe[1] })
                close(fd);
            execl(progfname, progfname, NULL);
            perror("Failed to exec");
        }
        for (auto fd : { inpipe[ReadEnd], outpipe[WriteEnd], errpipe[WriteEnd] })
            close(fd);
    }  // }}}

    void readResult()  // {{{
    {
        outstr.clear();
        errstr.clear();
        DataGen::refresh();
        datastr = DataGen::getdata();
        write(inpipe[WriteEnd], datastr.c_str(), datastr.size());
        close(inpipe[WriteEnd]);
        ansstr = DataGen::getans();
        timestamp = clock();
        wait(NULL);
        auto readall = [this] (int fd, std::string &target) {
            int len = 0;
            while ((len = read(fd, buf, BUFSIZ)) > 0)
                target.append(buf, buf + len);
        };
        readall(outpipe[ReadEnd], outstr);
        readall(errpipe[ReadEnd], errstr);
        for (auto fd : { inpipe[WriteEnd], outpipe[ReadEnd], errpipe[ReadEnd] })
            close(fd);
    }  // }}}

    struct DiffIter  // {{{
    {
    private:
        std::string::iterator pos, eof, lastch, eol;
        void refreshLine()
        {
            for (eol = pos; eol != eof && *eol != '\n'; ++eol)
                if (!isspace(*eol))
                    lastch = eol;
            if (eol == eof)
                return;
            auto peek = eol + 1;
            for (; peek != eof && isspace(*peek); ++peek)
                ;
            if (peek == eof)
                eol = peek;
        }
    public:
        DiffIter(std::string::iterator it, std::string::iterator e)
            : pos(it), eof(e), lastch(it), eol(it)
        {
            refreshLine();
        }
        bool iseof() { return pos == eof; }
        char operator*() { return *pos; }
        DiffIter &operator++()
        {
            if (*pos++ == '\n')
                refreshLine();
            else if (pos >= lastch)
                pos = eol;
            return *this;
        }
    };  // }}}
public:

    std::string datastr{}, outstr{}, errstr{}, ansstr{};
    CheckUtil() = default;
    CheckUtil(const CheckUtil &) = delete;
    CheckUtil(CheckUtil &&) = default;
    CheckUtil &operator=(const CheckUtil &) = delete;
    CheckUtil &operator=(CheckUtil &&) = default;

    bool operator()()
    {
        execChild();
        readResult();
        if constexpr (std::is_function_v<decltype(DataGen::SPJ)>)
            return DataGen::SPJ(outstr);
        else
        {
            DiffIter ansit(ansstr.begin(), ansstr.end()), outit(outstr.begin(), outstr.end());
            for (; !ansit.iseof() && !outit.iseof() && *ansit == *outit; ++ansit, ++outit)
                ;
            return ansit.iseof() && outit.iseof();
        }
    }
} check;

void printStat(auto ...)
{
    struct FileOut
    {
        FILE *handle;
        FileOut(const char *fname)
            : handle(fopen(fname, "w")) {}
        FileOut(const FileOut &) = default;
        FileOut(FileOut &&) = default;
        FileOut &operator=(const FileOut &) = default;
        FileOut &operator=(FileOut &&) = default;
        ~FileOut() { fclose(handle); }
        void write(const std::string &val) { fprintf(handle, "%s", val.c_str()); }
    };
    FileOut(datafname).write(check.datastr);
    FileOut(ansfname).write(check.ansstr);
    FileOut(outfname).write(check.outstr);
    FileOut(logfname).write(check.errstr);
    exit(EXIT_FAILURE);
}
// }}} End check utils

// {{{ DataGen
namespace DataGen
{
int n, m;
std::vector<int> orig, arr;
std::vector<Cons<int, int, int, int>> ops;
std::vector<int> ans;

// {{{ Actual generation
void refresh()
{
    n = randint(7, 23);
    m = randint(5, 21);
    auto gennum = [] { return randint(1, 50); };
    orig = randarr(n, gennum);
    ops = randarr(m, [&] {
        int id, l, r, val;
        id = randint(1, 5);
        l = randint(1, n);
        r = randint(1, n);
        if (l > r)
            std::swap(l, r);
        if (id == 2)
            val = randint(1, r - l + 1);
        else if (id == 3)
            val = gennum();
        else
            val = randint(0, 21);
        return Cons(id, l, r, val);
    });
}

std::string getdata()
{
    std::string res = format("$ $\n$\n", n, m, orig);
    for (auto it : ops)
        if (it.at<0>() == 3)
            res += format("3 $ $\n", it.at<1>(), it.at<3>());
        else
            res += format("$\n", to_string(it));
    return res;
}

std::string getans()
{
    arr = orig;
    ans.clear();
    ans.reserve(m);
    std::vector<int> shadow;
    shadow.reserve(n);
    auto mkshadow = [&] (decltype(ops[0]) data) {
        shadow.clear();
        shadow.insert(shadow.begin(), arr.begin() + data.at<1>() - 1, arr.begin() + data.at<2>());
        std::sort(shadow.begin(), shadow.end());
    };
    for (auto data : ops)
        switch (data.at<0>())
        {
        case 1:
            {
                mkshadow(data);
                int index = 0;
                for (; static_cast<size_t>(index) < shadow.size() && shadow[index] < data.at<3>(); ++index)
                    ;
                ans.push_back(index + 1);
                break;
            }
        case 2:
            mkshadow(data);
            ans.push_back(shadow[data.at<3>() - 1]);
            break;
        case 3:
            arr[data.at<1>() - 1] = data.at<3>();
            break;
        case 4:
            {
                mkshadow(data);
                auto it = shadow.begin();
                for (; it != shadow.end() && *it < data.at<3>(); ++it)
                    ;
                if (it == shadow.begin())
                    ans.push_back(-2147483647);
                else if (it == shadow.end())
                    ans.push_back(shadow.back());
                else
                    ans.push_back(*--it);
                break;
            }
        case 5:
            {
                mkshadow(data);
                auto it = shadow.begin();
                for (; it != shadow.end() && *it <= data.at<3>(); ++it)
                    ;
                if (it == shadow.end())
                    ans.push_back(2147483647);
                else
                    ans.push_back(*it);
                break;
            }
        }
    return to_string(ans, "\n");
}
// }}} End actual generation
}  // End namespace DataGen
// }}} DataGen

// {{{ Main
int main()
{
    srand(19260817);
    int count = 0;
    signal(SIGINT, printStat);
    while (true)
    {
        printf("\rData %d", ++count);
        fflush(stdout);
        if (!check())
        {
            printf("\nFailed\n");
            break;
        }
    }
    printStat();
    return 0;
}
// }}} End main

// vim: fdm=marker
