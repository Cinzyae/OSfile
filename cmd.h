//
// Created by MACHENIKE on 12/18/2020.
//

#ifndef OSFILE_CMD_H
#define OSFILE_CMD_H

#include <stdint.h>

//展示读取文件夹内容
void cmd_ls(char *content, int ninode);

//创建
int cmd_new(char *content, int ninode, uint16_t file_type);

/*
//创建文件夹
void cmd_mkdir(char *content, int ninode);

//创建文件
void cmd_touch(char *content, int ninode);
*/

//复制文件
int cmd_cp(char *content, int ninode);

//关闭系统
void cmd_shutdown();

int getcmd(char *buf, int nbuf);

void runcmd(char *buf);

void cmd_test(char *content);

void root();

#endif //OSFILE_CMD_H
