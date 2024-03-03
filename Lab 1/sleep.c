#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    int i;
    if (argc != 2) // 如果在调用main函数时没有传递任何命令行参数，那么argc参数的值将是1。
                   // 这是因为在执行程序时，操作系统会自动传递一个参数给程序，这个参数是程序的名称。
                   // 因此，argc的值至少是1，即程序的名称本身。
    {
        fprintf(2, "usage: sleep pattern [file ...]\n");
        exit(1);
    }
    i=atoi(argv[1]);//获取时间 ,argv[0]为程序的名称
    sleep(i);
    exit(0);
}
