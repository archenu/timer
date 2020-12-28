/**
 * @file test.c
 * @author your name (you@domain.com)
 * @brief  创建一个定时器，每秒运行一次print_hello函数，运行5次停止
 * @version 0.1
 * @date 2020-12-24
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "timer.h"
#include <windows.h>
#include <stdio.h>

sl_uint8_t timer_id;
sl_uint8_t run_flag = 1;

void print_hello(void)
{
    static sl_uint8_t cnt = 0;
    if( ++cnt == 5)
    {
        cnt = 0;
        timer_stop(timer_id);
        run_flag = 0;
    }
    printf("hello, world!\r\n");
}



int main(void)
{
    unsigned int tick = 0;
    /** 1. 创建定时器 **/
    timer_create(&timer_id, TIMER_MODE_CYCLE, print_hello);

    /** 2. 启动定时器 */
    timer_start(timer_id, 2000);
    while (run_flag)
    {

        if (++tick >= 80000)
        {
            tick = 0;

            /** 3.在tick中断中定时刷新 此处模拟系统滴答时钟中断*/
            timer_tick();
        }

        /** 4. 主循环中定时器消息循环 */
        timer_task_loop();
    }
    return 0;
}