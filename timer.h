#ifndef __TIMER_H__
#define __TIMER_H__

#include "type_def.h"


/*** user interface ***/


#define     SL_SYS_TICK         10   ///< 根据自己的系统时钟周期而定，默认为10ms
#define     TIMER_MAX_NUM       8    ///< 最大定时器个数
#define     TIMER_MODE_ONE_SHOT 1    ///< 单次触发，触发后定时器停止
#define     TIMER_MODE_CYCLE    0    ///< 周期触发


/*********end  interface ************/

typedef void (*callback)(void );


/**
 * @brief 从定时器对象池中返回一个空闲的定时器对象，并设置定时器相关属性
 * 
 * @param timer_id  用于接收生成的定时器对象id
 * @param period    定时器定时周期， 单位ms 
 * @param timer_mode  定时器模式 0: 周期循环  1：单次
 * @param cb        定时时间到触发的回调函数
 * @return sl_err_t 成功返回ERR_OK,  失败返回ERR_FULL, 即没有定时器资源，需要适当增加TIMER_MAX_NUM值
 * @note            用此接口创建的定时任务，回调函数会在主循环中执行，响应速度取决于主循环的实时性，紧急且简短的任务
 *                  建议使用 timer_create_urge 接口函数
 *              
 */
sl_err_t timer_create(sl_uint8_t *timer_id, sl_uint32_t period, sl_uint8_t timer_mode, callback cb);


/**
 * @brief 从定时器对象池中返回一个空闲的定时器对象，并设置定时器相关属性
 * 
 * @param timer_id  用于接收生成的定时器对象id
 * @param period    定时器定时周期， 单位ms 
 * @param timer_mode  定时器模式 0: 周期循环  1：单次
 * @param cb        定时时间到触发的回调函数
 * @return sl_err_t 成功返回ERR_OK,  失败返回ERR_FULL, 即没有定时器资源，需要适当增加TIMER_MAX_NUM值
 * @note  此接口和timer_create 接口函数的不同之处是：
 *        用此函数创建的定时器任务对象，回调函数会在中断中执行，所以回调函数应该简短，用于比较紧急的定时任务
 */
sl_err_t timer_create_urge(sl_uint8_t *timer_id, sl_uint32_t period, sl_uint8_t timer_mode, callback cb);


/**
 * @brief 删除制定id的定时器对象
 * 
 * @param timer_id 待删除的定时器对象id
 * @return sl_err_t 成功: ERR_OK  失败：ERR_NOT_EXIST --id 不存在
 */
sl_err_t  timer_delete(sl_uint8_t timer_id);


/**
 * @brief 启动指定id 的定时器对象
 * 
 * @param timer_id  待启动的定时器对象id
 * @return sl_err_t 成功: ERR_OK  失败：ERR_NOT_EXIST --id 不存在
 */
sl_err_t timer_start(sl_uint8_t timer_id);


/**
 * @brief  暂停指定id 的定时器对象
 * 
 * @param timer_id 待暂停的定时器对象id
 * @return sl_err_t 成功: ERR_OK  失败：ERR_NOT_EXIST --id 不存在
 */
sl_err_t timer_stop(sl_uint8_t timer_id);


/**
 * @brief   修改指定id 的定时器对象的定时周期
 * 
 * @param timer_id 待修改的定时器对象id
 * @param period   新设定的定时器周期值， 单位： ms
 * @return sl_err_t 成功: ERR_OK  失败：ERR_NOT_EXIST --id 不存在
 */
sl_err_t timer_set_period(sl_uint8_t timer_id, sl_uint32_t  period);


/**
 * @brief   复位指定id定时器对象的计数值，重新开始计时
 * 
 * @param timer_id 待复位的的定时器对象id
 * @return sl_err_t 成功: ERR_OK  失败：ERR_NOT_EXIST --id 不存在
 */
sl_err_t timer_reset(sl_uint8_t timer_id);


/**
 * @brief 定时器任务时间走时函数，需要放在系统定时器中断中调用
 * 
 */
void  timer_tick(void);

/**
 * @brief 定时器事件监测函数，需要放在主循环中调用，检测定时器超时事件，并
 *        执行响应回调函数
 */
void  timer_task_loop(void);


#endif

