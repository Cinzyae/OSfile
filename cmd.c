//
// Created by MACHENIKE on 12/18/2020.
//
#include <mem.h>
#include <stdio.h>
#include <malloc.h>
#include "cmd.h"
#include "ulib.h"
#include "filesys.h"
#include "cmd_sub.h"

//展示读取文件夹内容
void cmd_ls(char *content, int ninode) {
    printf("run ls:\n");
    if (*content == '\0') {
        show_folder(ninode);
        return;
    }

    char *next = parse_content(content);
    printf("after parse:\n");
    printf("    content:%s\tnext:%s\n", content, next);
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
        printf("show folder:");
        show_folder(inode_next);
    }
}

//创建文件夹
void cmd_mkdir(char *content, int ninode) {
    printf("run mkdir:\n");
    char *next = parse_content(content);
    printf("\tafter parse:\tcontent:%s\tnext:%s\n", content, next);
    int inode_next = find_next_inode(ninode, content);
    if ((inode_next <= 0) && (*next == '\0')) {//无文件夹，无待访问——冲
        printf("make new folder\n");
        int new_ninode = 0;
        int new_ndata = 0;
        set_sp_block(TYPE_FILE, &new_ninode, &new_ndata);
        if (build_new(ninode, new_ninode, new_ndata, TYPE_FOLDER, content) < 0) {
            printf("mkdir folder fail.\n");
            return;
        }
    } else if ((inode_next > 0) && (*next != '\0')) {//有文件夹，有待访问——冲
        printf("run mkdir:\n");
        cmd_mkdir(next, inode_next);
        return;
    }
    // TODO : 分情况
    //无文件夹，有待访问——错
    //有文件夹，无待访问——错
}

//创建文件
void cmd_touch(char *content) {
    printf("run touch:\n");
    //读sp_block块,sp_block查空，修改sp_block块，返回可修改inode块序号
    int new_ninode = 0;
    int new_ndata = 0;
    set_sp_block(TYPE_FILE, &new_ninode, &new_ndata);
    //查看根目录
    if (build_new(0, new_ninode, new_ndata, TYPE_FILE, content) < 0) {
        printf("touch file fail.\n");
        return;
    } else {
        printf("here is free space in home dir.\n");
    }
}

//复制文件
void cmd_cp() {

    //读sp_block块,sp_block查空，修改sp_block块
    //修改inode块，修改对应data块为data_item
    //写data块
    //返回
}

//关闭系统
void cmd_shutdown() {
    shutdown_sys();
}

void cmd_test(char *content) {
    printf("run test\n");
    sp_block *q = NULL;
    q = malloc(MEM_BLOCK_SIZE);
    read_block(0, (uint32_t *) q);
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
    printf("\torder:%s\tcontent:%s\n", order, content);
    if (strcmp(order, "ls") == 0) {
        cmd_ls(content, 0);
    } else if (strcmp(order, "mkdir") == 0) {
        cmd_mkdir(content, 0);
    } else if (strcmp(order, "shutdown") == 0) {
        cmd_shutdown();
    } else if (strcmp(order, "touch") == 0) {
        cmd_touch(content);
    } else if (strcmp(order, "cp") == 0) {
        cmd_cp();
    } else if (strcmp(order, "test") == 0) {
        cmd_test(content);
    }
}

//传入命令，返回/后一位的指针
char *parse_content(char *content) {
    char whitespace[] = " /";
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

    ino_list *p = NULL;
    p = malloc(sizeof(ino_list));
    read_inode(0, (uint32_t *) p);
    p->inodes[0].file_type = TYPE_FOLDER;
    p->inodes[0].size = 0;
    p->inodes[0].link = 0;
    p->inodes[0].block_point[0] = new_ndata;
    write_inode(0, (uint32_t *) p);
    free(p);
}