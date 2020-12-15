#ifndef __TIMER_H__
#define __TIMER_H__

#include "type_def.h"


/*** user interface ***/


#define     SL_SYS_TICK         10   ///< �����Լ���ϵͳʱ�����ڶ�����Ĭ��Ϊ10ms
#define     TIMER_MAX_NUM       8    ///< ���ʱ������
#define     TIMER_MODE_ONE_SHOT 1    ///< ���δ�����������ʱ��ֹͣ
#define     TIMER_MODE_CYCLE    0    ///< ���ڴ���


/*********end  interface ************/

typedef void (*callback)(void );


/**
 * @brief �Ӷ�ʱ��������з���һ�����еĶ�ʱ�����󣬲����ö�ʱ���������
 * 
 * @param timer_id  ���ڽ������ɵĶ�ʱ������id
 * @param period    ��ʱ����ʱ���ڣ� ��λms 
 * @param timer_mode  ��ʱ��ģʽ 0: ����ѭ��  1������
 * @param cb        ��ʱʱ�䵽�����Ļص�����
 * @return sl_err_t �ɹ�����ERR_OK,  ʧ�ܷ���ERR_FULL, ��û�ж�ʱ����Դ����Ҫ�ʵ�����TIMER_MAX_NUMֵ
 * @note            �ô˽ӿڴ����Ķ�ʱ���񣬻ص�����������ѭ����ִ�У���Ӧ�ٶ�ȡ������ѭ����ʵʱ�ԣ������Ҽ�̵�����
 *                  ����ʹ�� timer_create_urge �ӿں���
 *              
 */
sl_err_t timer_create(sl_uint8_t *timer_id, sl_uint32_t period, sl_uint8_t timer_mode, callback cb);


/**
 * @brief �Ӷ�ʱ��������з���һ�����еĶ�ʱ�����󣬲����ö�ʱ���������
 * 
 * @param timer_id  ���ڽ������ɵĶ�ʱ������id
 * @param period    ��ʱ����ʱ���ڣ� ��λms 
 * @param timer_mode  ��ʱ��ģʽ 0: ����ѭ��  1������
 * @param cb        ��ʱʱ�䵽�����Ļص�����
 * @return sl_err_t �ɹ�����ERR_OK,  ʧ�ܷ���ERR_FULL, ��û�ж�ʱ����Դ����Ҫ�ʵ�����TIMER_MAX_NUMֵ
 * @note  �˽ӿں�timer_create �ӿں����Ĳ�֮ͬ���ǣ�
 *        �ô˺��������Ķ�ʱ��������󣬻ص����������ж���ִ�У����Իص�����Ӧ�ü�̣����ڱȽϽ����Ķ�ʱ����
 */
sl_err_t timer_create_urge(sl_uint8_t *timer_id, sl_uint32_t period, sl_uint8_t timer_mode, callback cb);


/**
 * @brief ɾ���ƶ�id�Ķ�ʱ������
 * 
 * @param timer_id ��ɾ���Ķ�ʱ������id
 * @return sl_err_t �ɹ�: ERR_OK  ʧ�ܣ�ERR_NOT_EXIST --id ������
 */
sl_err_t  timer_delete(sl_uint8_t timer_id);


/**
 * @brief ����ָ��id �Ķ�ʱ������
 * 
 * @param timer_id  �������Ķ�ʱ������id
 * @return sl_err_t �ɹ�: ERR_OK  ʧ�ܣ�ERR_NOT_EXIST --id ������
 */
sl_err_t timer_start(sl_uint8_t timer_id);


/**
 * @brief  ��ָͣ��id �Ķ�ʱ������
 * 
 * @param timer_id ����ͣ�Ķ�ʱ������id
 * @return sl_err_t �ɹ�: ERR_OK  ʧ�ܣ�ERR_NOT_EXIST --id ������
 */
sl_err_t timer_stop(sl_uint8_t timer_id);


/**
 * @brief   �޸�ָ��id �Ķ�ʱ������Ķ�ʱ����
 * 
 * @param timer_id ���޸ĵĶ�ʱ������id
 * @param period   ���趨�Ķ�ʱ������ֵ�� ��λ�� ms
 * @return sl_err_t �ɹ�: ERR_OK  ʧ�ܣ�ERR_NOT_EXIST --id ������
 */
sl_err_t timer_set_period(sl_uint8_t timer_id, sl_uint32_t  period);


/**
 * @brief   ��λָ��id��ʱ������ļ���ֵ�����¿�ʼ��ʱ
 * 
 * @param timer_id ����λ�ĵĶ�ʱ������id
 * @return sl_err_t �ɹ�: ERR_OK  ʧ�ܣ�ERR_NOT_EXIST --id ������
 */
sl_err_t timer_reset(sl_uint8_t timer_id);


/**
 * @brief ��ʱ������ʱ����ʱ��������Ҫ����ϵͳ��ʱ���ж��е���
 * 
 */
void  timer_tick(void);

/**
 * @brief ��ʱ���¼���⺯������Ҫ������ѭ���е��ã���ⶨʱ����ʱ�¼�����
 *        ִ����Ӧ�ص�����
 */
void  timer_task_loop(void);


#endif

