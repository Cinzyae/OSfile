#include <mem.h>
#include <stdio.h>
#include "cmd.h"
#include "filesys.h"

#define BUFSIZE 128
/*
 * main:        运行程序
 * cmd:         命令相关
 * cmd_sub:     实现命令相关
 * disk:        底层设备块相关
 * filesys:     磁盘块相关
 * ulib:        自定义函数
 */
int main() {
    printf("ex2 starts\n");
    init_sys();
    static char buf[BUFSIZE];
    while (getcmd(buf, BUFSIZE) >= 0) {
        buf[strlen(buf) - 1] = 0;
        runcmd(buf);
    }
    return 0;
}

// TODO : 检查返回值