#include "timer.h"


typedef struct tiemr_
{
    sl_uint8_t is_active; // 有效
    sl_uint8_t run;        
    sl_uint32_t period;
    sl_uint8_t  timer_mode;
    sl_uint8_t  urgeflag;
    sl_uint32_t tick;
    sl_uint8_t  timeoutflag;
    callback cb;
}sl_tiemr;

sl_tiemr timer_pool[TIMER_MAX_NUM];

/**
 * @brief 从定时器对象池中返回一个空闲的定时器对象，并设置定时器相关属性
 * 
 * @param timer_id  用于接收生成的定时器对象id
 * @param period    定时器定时周期， 单位ms 
 * @param timer_mode  定时器模式 0: 周期循环  1：单次
 * @param cb        定时时间到触发的回调函数
 * @return sl_err_t 
 */
sl_err_t timer_create(sl_uint8_t *timer_id, sl_uint32_t period, sl_uint8_t timer_mode, callback cb)
{
    int index = 0;
    while(timer_pool[index++].is_active && index <= TIMER_MAX_NUM);
    
    if(index > TIMER_MAX_NUM)
        return ERR_FULL;
    timer_pool[index-1].is_active = TRUE;
    timer_pool[index-1].run  = FALSE;
    timer_pool[index-1].period = period;
    timer_pool[index-1].tick = period;
    timer_pool[index-1].timer_mode = timer_mode;
    timer_pool[index-1].cb = cb;
    timer_pool[index-1].timeoutflag = 0;
    timer_pool[index-1].urgeflag = 0;
    *timer_id = index-1;
    return ERR_OK; 
}



sl_err_t timer_create_urge(sl_uint8_t *timer_id, sl_uint32_t period, sl_uint8_t timer_mode, callback cb)
{
    int index = 0;
    while(timer_pool[index++].is_active && index <= TIMER_MAX_NUM);
    
    if(index > TIMER_MAX_NUM)
        return ERR_FULL;
    timer_pool[index-1].is_active = TRUE;
    timer_pool[index-1].run  = FALSE;
    timer_pool[index-1].period = period/SL_SYS_TICK;
    timer_pool[index-1].tick = period/SL_SYS_TICK;
    timer_pool[index-1].timer_mode = timer_mode;
    timer_pool[index-1].cb = cb;
    timer_pool[index-1].timeoutflag = 0;
    timer_pool[index-1].urgeflag = TRUE;
    *timer_id = index-1;
    return ERR_OK; 
}
/**
 * @brief 删除指定id的定时器对象
 * 
 * @param timer_id 待删除的定时器对象id
 * @return sl_err_t 成功: ERR_OK  失败：ERR_NOT_EXIST --id 不存在
 */
sl_err_t  timer_delete(sl_uint8_t timer_id)
{
    timer_pool[timer_id].is_active = FALSE;
}


sl_err_t timer_start(sl_uint8_t timer_id)
{
    if (timer_id >= TIMER_MAX_NUM )
        return ERR_NOT_EXIST;

    timer_pool[timer_id].run = TRUE;
    return  ERR_OK;
}


sl_err_t timer_reset(sl_uint8_t timer_id)
{  
    if (timer_id >= TIMER_MAX_NUM )
    return ERR_NOT_EXIST; 
    timer_pool[timer_id].tick = timer_pool[timer_id].period;
    return ERR_OK;
}

sl_err_t timer_set_period(sl_uint8_t timer_id, sl_uint32_t  period)
{
    if (timer_id >= TIMER_MAX_NUM )
        return ERR_NOT_EXIST; 
    timer_pool[timer_id].period = period;
    return ERR_OK;
}

sl_err_t timer_stop(sl_uint8_t timer_id)
{
    if (timer_id >= TIMER_MAX_NUM )
        return ERR_NOT_EXIST;

    timer_pool[timer_id].run = FALSE;

    return ERR_OK;
}

/**
 * @brief 定时器对象当时刷新任务，需要放在系统定时中断中执行
 * 
 */
void  timer_tick()
{
    for(int i=0; i<TIMER_MAX_NUM; i++)
    {
        if(timer_pool[i].run)
        {
            timer_pool[i].tick--;
            if (timer_pool[i].tick == 0)
            {
                timer_pool[i].tick = timer_pool[i].period;
                if (timer_pool[i].urgeflag)
                {
                     timer_pool[i].cb();
                }
                else
                {
                      timer_pool[i].timeoutflag = TRUE;
                }            
                if (timer_pool[i].timer_mode == TIMER_MODE_ONE_SHOT)
                {
                    timer_pool[i].run = 0;
                }
            }
        }
    }
}

/**
 * @brief 定时器任务循环，放在主循环中调用，当定时器定时时间到，会自动触发对应的定时器回调函数
 * 
 */
void  timer_task_loop()
{
    int i; 

    for (i=0; i<TIMER_MAX_NUM; i++ )
    {
        if(timer_pool[i].is_active == FALSE)
            continue;

        if(timer_pool[i].timeoutflag == TRUE)
        {
            timer_pool[i].cb();
            timer_pool[i].timeoutflag = 0;
        }
    }
}

