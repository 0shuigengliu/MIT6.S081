#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// p = get a number from left neighbor
// print p
// loop:
//     n = get a number from left neighbor
//     if (p does not divide n)
//         send n to right neighbor
void Prime(int LeftNei[]) // 递归子程序,筛选素数
{
    int p, n;
    close(LeftNei[1]);
    // 从左邻居中获取一个数
    read(LeftNei[0], &p, sizeof(int));
    if(0==p){
        exit(0);//当管道的write端关闭时，read返回零
    }
    fprintf(1,"prime %d\n", p);

    int rp[2]; // 右邻居管道
    pipe(rp);

    while (read(LeftNei[0], &n, sizeof(int)))
    {
        if (0 != n % p)
        {
            write(rp[1], &n, sizeof(int)); //  send n to right neighbor
        }
    }

    if (0 == fork())
    {
        Prime(rp);
    }
    else
    {
        close(LeftNei[0]);
        close(rp[1]);
        wait(0);
    }
    exit(0);
}

int main()
{
    int p[2]; // 第一个进程的输入管道
    pipe(p);  // 创建管道
    for (int i = 2; i != 35; ++i)
    {
        write(p[1], &i, sizeof(i)); // 依次向管道中写入数据
    }
    if (0 == fork()) // 子程序
    {
        Prime(p);
        exit(0);
    }
    else
    {
        // 关闭进程不需要的文件描述符
        close(p[0]);
        close(p[1]);
        wait(0); // 等待程序的结束
    }
    
    exit(0);
}