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

#define MAXARGS 10

//展示读取文件夹内容
void cmd_ls(char *content, int ninode) {
    printf("run ls:\n");
    char *next = parse_content(content);
    memset(next++, 0, 1);
    printf("after parse:\n");
    printf("    content:%s\tnext:%s\n", content, next);
    int inode_next = ls_sub(ninode, content);
    if (inode_next == -1) {
        printf("folder empty!\n");
        return;
    } else if (inode_next == 0) {
        printf("no such sub_folder!\n");
        return;
    }

    if (*next != '\0') {
        // 如果仍然有下一个带访问子文件夹则递归
        cmd_ls(next, inode_next);
        return;
    } else {
        //TODO : 展示文件夹
        printf("show folder");
    }
}

//创建文件夹
void cmd_mkdir(char *content) {
    printf("run mkdir:\n");
    int block = 0;
    int inode = 0;
    //set_sp_block(TYPE_FOLDER, &block, &inode);
    //set_inode_block(TYPE_FOLDER, &block, &inode);

    //修改inode块，修改对应data块为dir_item
    //写data块
    //返回

}

//创建文件
void cmd_touch(char *content) {
    printf("run touch:\n");
    //读sp_block块,sp_block查空，修改sp_block块，返回可修改inode块序号
    int ninode = 0;
    int ndata = 0;
    set_sp_block(TYPE_FILE, &ninode, &ndata);
    //查看根目录
    if (touch_sub(ninode, ndata, TYPE_FILE, content) < 0) {
        printf("touch file fail.\n");
        return;
    } else {
        printf("here is free space in home dir.\n");
    }
    //修改inode块，修改对应data块为data_item
    //写data块
    //返回
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
    inode *p = NULL;
    p = malloc(MEM_BLOCK_SIZE);
    p->size = 1;
    p->file_type = TYPE_FILE;
    p->link = 1;
    write_block(33, (uint32_t *) p);
    free(p);

    inode *q = NULL;
    q = malloc(MEM_BLOCK_SIZE);
    read_block(33, (uint32_t *) q);
    printf("block size:%d\n", q->size);
    printf("file type:%d\n", q->file_type);
    printf("link:%d\n", q->link);
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
        cmd_mkdir(content);
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

char *parse_content(char *content) {
    char whitespace[] = " /";
    char *p = content;
    char *next = NULL;
    while (strchr(whitespace, *p++) == 0) {
        next = p;
    }
    // TODO : fix next
    return next;
}
//void parseline(char *str, char *strSorted[], int strSortedLen) {
//    char *p;
//    int len = 0;
//    char whitespace[] = " \t\r\n\v";
//    for (p = str; strchr(whitespace, *p); p++);
//    while (*p && len <= strSortedLen) {
//        strSorted[len++] = p;
//        while (*p && !strchr(whitespace, *p))
//            p++;
//        while (strchr(whitespace, *p))
//            *p++ = '\0';
//    }
//    strSorted[len] = 0;
//}
