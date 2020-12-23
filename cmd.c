//
// Created by MACHENIKE on 12/18/2020.
//
#include <mem.h>
#include <stdio.h>
#include <malloc.h>
#include "cmd.h"
#include "cmd_sub.h"
#include "ulib.h"

//展示读取文件夹内容
void cmd_ls(char *content, int ninode) {
    printf("run ls:\n");
    if (*content == '\0') {
        show_folder(ninode);
        return;
    }
    char *next = parse_content(content, "/");
    printf("after parse:\tcontent:%s\tnext:%s\n", content, next);
    int inode_next = find_next_inode(ninode, content);
    if (inode_next == -1) {
        printf("folder empty!\n");
        return;
    } else if (inode_next == 0) {
        printf("no such sub_folder!\n");
        return;
    }
    if (*next != '\0') {
        // 如果仍然有下一个待访问子文件夹则递归
        cmd_ls(next, inode_next);
        return;
    } else {
        // 展示文件夹
        printf("show folder:\n");
        show_folder(inode_next);
    }
}

//创建
int cmd_new(char *content, int ninode, uint16_t file_type) {
    printf("run new:\n");
    if (*content == '\0') {
        printf("need a name\n");
        return -1;
    }
    char *next = parse_content(content, "/");
    printf("after parse:\tcontent:%s\tnext:%s\n", content, next);
    int inode_next = find_next_inode(ninode, content);
    if ((inode_next <= 0) && (*next == '\0')) {//无文件夹，无待访问——冲
        printf("make new\n");
        int new_ninode = 0;
        int new_ndata = 0;
        set_sp_block(file_type, &new_ninode, &new_ndata);
        if (build_new(ninode, new_ninode, new_ndata, file_type, content) < 0) {
            printf("new fail.\n");
            return -1;
        }
        return 0;
    } else if ((inode_next > 0) && (*next != '\0')) {//有文件夹，有待访问——冲
        printf("cd %s:\n", content);
        return cmd_new(next, inode_next, file_type);
    } else {
        //无文件夹，有待访问——错
        //有文件夹，无待访问——错
        printf("wrong.inode_next = %d\t *next : %s\n", inode_next, next);
        return -1;
    }
}

/*
//创建文件夹
void cmd_mkdir(char *content, int ninode) {
    printf("run mkdir:\n");
    if (*content == '\0') {
        printf("need a folder name\n");
        return;
    }
    char *next = parse_content(content, "/");
    printf("after parse:\tcontent:%s\tnext:%s\n", content, next);
    int inode_next = find_next_inode(ninode, content);
    if ((inode_next <= 0) && (*next == '\0')) {//无文件夹，无待访问——冲
        printf("make new folder\n");
        int new_ninode = 0;
        int new_ndata = 0;
        set_sp_block(TYPE_FOLDER, &new_ninode, &new_ndata);
        int flag = build_new(ninode, new_ninode, new_ndata, TYPE_FOLDER, content);
        if (flag < 0) {
            printf("mkdir folder fail.\n");
            return;
        }
    } else if ((inode_next > 0) && (*next != '\0')) {//有文件夹，有待访问——冲
        printf("cd %s:\n", content);
        cmd_mkdir(next, inode_next);
        return;
    } else {
        //无文件夹，有待访问——错
        //有文件夹，无待访问——错
        printf("wrong.inode_next = %d\t *next : %s\n", inode_next, next);
    }
}

//创建文件
void cmd_touch(char *content, int ninode) {
    printf("run mkdir:\n");
    if (*content == '\0') {
        printf("need a folder name\n");
        return;
    }
    char *next = parse_content(content, "/");
    printf("after parse:\tcontent:%s\tnext:%s\n", content, next);
    int inode_next = find_next_inode(ninode, content);
    if ((inode_next <= 0) && (*next == '\0')) {//无文件夹，无待访问——冲
        printf("make new folder\n");
        int new_ninode = 0;
        int new_ndata = 0;
        set_sp_block(TYPE_FILE, &new_ninode, &new_ndata);
        int flag = build_new(ninode, new_ninode, new_ndata, TYPE_FILE, content);
        if (flag < 0) {
            printf("mkdir folder fail.\n");
            return;
        }
    } else if ((inode_next > 0) && (*next != '\0')) {//有文件夹，有待访问——冲
        printf("cd %s:\n", content);
        cmd_mkdir(next, inode_next);
        return;
    } else {
        //无文件夹，有待访问——错
        //有文件夹，无待访问——错
        printf("wrong.inode_next = %d\t *next : %s\n", inode_next, next);
    }
}

*/

//复制文件
int cmd_cp(char *content, int ninode) {
    int inode2copy;
    char *next = parse_content(content, " ");
    printf("content:%s\tnext:%s\n", content, next);
    cmd_new(next, 0, TYPE_FILE);
}

//关闭系统
void cmd_shutdown() {
    shutdown_sys();
}

void cmd_test(char *content) {
    printf("run test\n");
    sp_block *q = NULL;
    q = malloc(MEM_BLOCK_SIZE);
    read_block(0, q);
    printf("magic_num:%d\t", q->magic_num);
    printf("free_block_count:%d\t", q->free_block_count);
    printf("free_inode_count:%d\t", q->free_inode_count);
    printf("dir_inode_count:%d\t\n", q->dir_inode_count);
    free(q);
}

int getcmd(char *buf, int nbuf) {
    printf("*");
    memset(buf, 0, nbuf);
    gets_ulib(buf, nbuf);
    if (buf[0] == 0)
        return -1;
    return 0;
}

void runcmd(char *buf) {
    char whitespace[] = " \t\r\n\v";
    char *p = buf;
    char *order = p;
    char *content;
    while (strchr(whitespace, *p++) == 0) {
        content = p;
    }
    memset(content++, 0, 1);
    printf("order:%s\tcontent:%s\n-------\n", order, content);
    if (strcmp(order, "ls") == 0) {
        cmd_ls(content, 0);
    } else if (strcmp(order, "mkdir") == 0) {
//        cmd_mkdir(content, 0);
        cmd_new(content, 0, TYPE_FOLDER);
    } else if (strcmp(order, "touch") == 0) {
//        cmd_touch(content, 0);
        cmd_new(content, 0, TYPE_FILE);
    } else if (strcmp(order, "cp") == 0) {
        cmd_cp(content, 0);
    } else if (strcmp(order, "shutdown") == 0) {
        cmd_shutdown();
    } else if (strcmp(order, "new") == 0) {
        cmd_new(content, 0, TYPE_FOLDER);
    } else if (strcmp(order, "test") == 0) {
        cmd_test(content);
    }
    printf("-------\n");
}

//传入命令，返回/后一位的指针
char *parse_content(char *content, char *whitespace) {
//    char whitespace[] = " /";
    char *p = content;
    char *next = NULL;
    while (strchr(whitespace, *p++) == 0) {
        next = p;
    }
    memset(next++, 0, 1);
    return next;
}

void root() {
    int new_ninode = 0;
    int new_ndata = 0;
    set_sp_block(TYPE_FILE, &new_ninode, &new_ndata);

    ino_list p;
    read_inode(0, &p);
    p.inodes[0].file_type = TYPE_FOLDER;
    p.inodes[0].size = 0;
    p.inodes[0].link = 0;
    p.inodes[0].block_point[0] = new_ndata;
    p.inodes[0].block_point[1] = new_ndata - 1;
    p.inodes[0].block_point[2] = new_ndata - 2;
    p.inodes[0].block_point[3] = new_ndata - 3;
    p.inodes[0].block_point[4] = new_ndata - 4;
    p.inodes[0].block_point[5] = new_ndata - 5;
    write_inode(0, &p);
//    free(p);
}