#ifndef RELAYER_H__
#define RELAYER_H__

#define REL_JOBMAX 10000

enum{
	STATE_RUNNING = 1,
	STATE_CANCEL,
	STATE_OVER,
	STATE_T,
	STATE_Ex,
	STATE_R,
	STATE_W
};

struct rel_stat_st{
	int state;
	int fd1;
	int fd2;
	int64_t count12,count21;
};

/**
@function 
@param1
@param2
@return  >=0 success,return job id
         == -EINVAL fail,param is invalid
         == -ENOSPC fail, job array is full
         == -ENOMEM  fail,memory used out 
*/
int rel_addjob(int fd1, int fd2);



/**
@function 
@param1
@param2
@return  ==0 success,job is cancel
         == -EINVAL fail,param is invalid
         == -EBUSY fail,job is already canceled
*/
int rel_canceljob(int id);


/**
@function 
@param1
@param2
@return  ==0 success,job is terminate and return state
         == -EINVAL fail,param is invalid        
*/
int rel_waitjob(int id, struct rel_stat_st*);


/**
@function 
@param1
@param2
@return  ==0 success,job is  return state
         == -EINVAL fail,param is invalid        
*/

int rel_statjob(int id, struct rel_stat_st*);

#endif 