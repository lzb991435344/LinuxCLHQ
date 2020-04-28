#ifndef  MYSEM_H__
#define MYSEM_H__

typedef void mysem_t; //重命名

/**
 @function mysem_init 初始化资源
 @param int initval 初始的资源值
 @return mysem_t* 指向该结构的指针
*/
mysem_t* mysem_init(int initval);

/**
 @function mysem_init 归还资源
 @param mysem_t* ptr 当前归还的位置
 @param int n 归还的资源值
 @return int  成功与否
*/
int mysem_add(mysem_t* ptr, int n);

int mysem_sub(mysem_t* ptr, int n);

int mysem_destroy(mysem_t* ptr);

#endif