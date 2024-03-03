#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    // dirent 目录中的一个目录项
    struct dirent de;
    // stat 存储文件（Linux一切皆文件）的相关信息
    struct stat st;
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    // fstat 读取文件状态信息，将其存入到st当中 成功返回0，失败返回-1
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // 根据文件的类型不同，输出不同的信息
    switch (st.type)
    {
    // 文件类型
    case T_FILE:
        fprintf(2, "find: you cannot find %s in a file\n", name);
        break;
    // 文件夹类型
    case T_DIR:
        //       目录   + / + 文件名 + 结束符号
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        // 将p指针移向路径缓冲区的末端并加上分隔符  strlen(buf): 这个函数返回字符串 buf的长度，即不包括 null 终止符的字符数。
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            // de.inum 为0 有特殊用途，一般不做使用
            //不在 . 与.. 中寻找
            if (de.inum == 0||strcmp(de.name, ".")==0||strcmp(de.name, "..")==0)
                continue;

            // 将de.name(一个地址),中DIRSIZ个字节的数据移动到p之后
            memmove(p, de.name, DIRSIZ);
            // 设定'\0'终止符为字符串的结尾
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            if (0 == fork())
            {
                if (st.type == T_DIR)
                {
                        find(buf, name);
                }
                exit(0);
            }
            else
            {
                if (st.type == T_FILE)
                {
                    if (0==strcmp(de.name,name))
                        fprintf(1,"%s\n", buf);
                }
                wait(0);
            }
        }
        break;
    }
    close(fd);
    exit(0);
}

int main(int argc, char *argv[])
{
    // find / some
    // 必须存在两个参数，加上默认传递的argv[0](程序本身的名称) 则argc应该为3
    if (argc != 3)
    {
        fprintf(2, "find: not enough arguments,need two\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}