//
// Created by MACHENIKE on 12/18/2020.
//

#ifndef OSFILE_CMD_H
#define OSFILE_CMD_H

//展示读取文件夹内容
void cmd_ls(char *content);

//创建文件夹
void cmd_mkdir(char *content);

//创建文件
void cmd_touch();

//复制文件
void cmd_cp();

//关闭系统
void cmd_shutdown();

void parseline(char *str, char *strSorted[], int strSortedLen);

int getcmd(char *buf, int nbuf);

void runcmd(char *buf);

void cmd_test(char *content);

#endif //OSFILE_CMD_H
