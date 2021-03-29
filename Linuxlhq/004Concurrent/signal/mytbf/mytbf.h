
//token：cps = 1:1
//array数据结构 1024 数组元素是结构体的指针
//cps,burst,token
#ifndef MY_TBF_H_
#define MY_TBF_H_

#define MYTBF_MAX 1024
typedef void mytbf_t;

struct mytbf_st* mytbf_init(int cps, int burst);
int mytbf_fetchtoken(mytbf_t*, int);
int mytbf_returntoken(mytbf_t*, int);
int mytbf_destory(mytbf_t*);
#endif
