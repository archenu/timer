#include "timer.h"


typedef struct tiemr_
{
    sl_uint8_t is_active; // ��Ч
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
 * @brief �Ӷ�ʱ��������з���һ�����еĶ�ʱ�����󣬲����ö�ʱ���������
 * 
 * @param timer_id  ���ڽ������ɵĶ�ʱ������id
 * @param period    ��ʱ����ʱ���ڣ� ��λms 
 * @param timer_mode  ��ʱ��ģʽ 0: ����ѭ��  1������
 * @param cb        ��ʱʱ�䵽�����Ļص�����
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
 * @brief ɾ��ָ��id�Ķ�ʱ������
 * 
 * @param timer_id ��ɾ���Ķ�ʱ������id
 * @return sl_err_t �ɹ�: ERR_OK  ʧ�ܣ�ERR_NOT_EXIST --id ������
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
 * @brief ��ʱ������ʱˢ��������Ҫ����ϵͳ��ʱ�ж���ִ��
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
 * @brief ��ʱ������ѭ����������ѭ���е��ã�����ʱ����ʱʱ�䵽�����Զ�������Ӧ�Ķ�ʱ���ص�����
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

