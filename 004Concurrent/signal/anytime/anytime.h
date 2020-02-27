

#ifndef ANYTIME_H__
#define ANYTIME_H__

#define JOB_MAX 1024
typedef void at_jobfunc_t(void*);

//like the fd
/**
@function at_addjob
@param1 seconds
@param2 function
@param3 arg
//use the standar Macro,path is /usr/include/asm-generic/errno-base.h 
@return  >=0  success,return job id
         == -EINVAL fail,param is invalid
         == -ENOSPC fail,arr is full
         == _ENOMEM fail,

*/
int at_addjob(int sec, at_jobfunc_t *jobp, void* arg);



/**
@function check the flag == running
@param job id
@return  == 0 success, the job is canceled
  		 ==-EINVAL fail,invalid param 
  		 ==-EBUSY  	fail,the job is finished
		 ==-ECANCELED  fail,the job is repeat cancel
  //wait()   parent should wait the child die      
*/
int at_cacnceljob(int id);


/**
@function 
@param job id
@return == 0 success,the job is free
        == -EINVAL fail,param is invalid
*/

int at_waitjob(int id);


at_pausejob();
at_resumejob();
#endif