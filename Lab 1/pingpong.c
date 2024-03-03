#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p1[2];
    int p2[2];
    int pid;
    char *str[5];
    pipe(p1); // 创造管道1,父向子发
    pipe(p2); // 创造管道2,子向父发
    if (0 == fork())
    {                            // 子进程
        pid = getpid();          // 得到自身的pid号
        close(p1[1]);            // 关闭父向子管道的写入端
        close(p2[0]);            // 关闭子向夫管道的读入段

        read(p1[0],str,sizeof(str));
        close(p1[0]);   //读入数据后就关闭读入端
        fprintf(1, "%d: received %s\n", pid,str); // 向标准输出端1 ，输出结果
        write(p2[1], "pong\0", 5); // 给父管道传递数据,考虑到结束符的存在
        close(p2[1]);            // 关闭子向父管道的写入端
        exit(0);
    }
    else
    {                   // 父进程
        pid = getpid(); // 得到自身的pid号
        close(p1[0]);            // 关闭父向子管道的读入端
        close(p2[1]);            // 关闭子向夫管道的写入段

        write(p1[1], "ping\0", 5); //给子管道传递数据
        close(p1[1]);//关闭父向子的写入端

        read(p2[0],str,sizeof(str));
        close(p2[0]);   //读入数据后就关闭读入端

        fprintf(1, "%d: received %s\n", pid,str); // 向标准输出端1 ，输出结果
    }
    exit(0);
}