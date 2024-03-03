#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

// 一行一行地读入数据
char *getline(int fd)
{
    static char buf[MAXARG]; //变量在程序运行的整个生命周期内都存在,变量的作用域将被限制在声明它的文件中
    char *p = buf;
    while (read(fd, p, sizeof(char)) > 0)
    {
        if (*p == '\n')
            break;
        ++p;
    }
    *p = '\0'; // 设置结束符
    return buf;
}

int main(int argc, char *argv[])
{
    char *p;
    if (argc <= 1)
    {
        fprintf(2, "xargs:not enough arguments\n");
        exit(1);
    }
    else
    {
        while (1)
        {
            // 管道通过标准输出（1）向xargs传递数据
            // xargs 应从 fd=0中读取数据
            p = getline(0);
            if (*p == '\0')
            {
                break; // 数据处理完毕
            }

            if (0 == fork())
            {
                int i=0;
                for(i=0;i<argc-1;++i){
                    argv[i]=argv[i+1];
                }
                argv[argc-1]=p;//将管道传来的参数添加入参数列表
                argv[argc]=0;
                exec(argv[0],argv);
                fprintf(2, "exec failed!\n");
                exit(1);
            }
            else
            {
                wait(0);
            }
        }
    }
    exit(0);
}