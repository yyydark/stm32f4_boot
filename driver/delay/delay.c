#include "delay.h"




/******************************************************************************
 * @brief     ͨ�ö�ʱ��3�жϳ�ʼ��
 *            ��ʱ�����ʱ����㷽��:Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *            Ft = ��ʱ������Ƶ��, ��λ: Mhz
 *
 * @param[in]  arr   :  �Զ���װֵ
 * @param[in]  psc   :  ʱ��Ԥ��Ƶ��
 *
 * @return     ��
 *
 ******************************************************************************/
void TIM3_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                  //ʹ��TIM3ʱ��

    TIM_TimeBaseInitStructure.TIM_Prescaler      = psc;                   //��ʱ����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode    = TIM_CounterMode_Down;  //���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_Period         = arr;                   //�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_ClockDivision  = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);                 //��ʼ����ʱ��3
}




/******************************************************************************
 * @brief      ��ʼ����ʱ����ʱ��������ʱ1us
 *
 * @param[in]  ��
 *
 * @return     ��
 *
 ******************************************************************************/
int Delay_Init(void)
{
    TIM3_Init(100 - 1, 84 - 1); //��ʱ1us
    return 0;
}


/******************************************************************************
 * @brief      ΢�뼶��ʱ
 *
 * @param[in]  us    :  ��ʱ΢����
 *
 * @return     ��
 *
 ******************************************************************************/
void Delay_Us(uint32_t us)
{
    TIM3->ARR = us;           //�Զ�������
    TIM3->CNT = TIM3->ARR;    //������
    TIM_Cmd(TIM3, ENABLE);    //������ʱ��
    while(TIM3->CNT) {;}      //�ȴ���ʱʱ�䵽
    TIM_Cmd(TIM3, DISABLE);   //�رն�ʱ��
}




/******************************************************************************
 * @brief      ���뼶��ʱ,��������OS��ʹ��
 *
 * @param[in]  us    :  ��ʱ������
 *
 * @return     ��
 *
 ******************************************************************************/
void Delay_Ms(uint32_t ms)
{
    for(int i = 0; i < ms; i++){
        Delay_Us(1000);
    }
}
