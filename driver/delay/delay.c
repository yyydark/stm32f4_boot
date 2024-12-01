#include "delay.h"




/******************************************************************************
 * @brief     通用定时器3中断初始化
 *            定时器溢出时间计算方法:Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *            Ft = 定时器工作频率, 单位: Mhz
 *
 * @param[in]  arr   :  自动重装值
 * @param[in]  psc   :  时钟预分频数
 *
 * @return     无
 *
 ******************************************************************************/
void TIM3_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                  //使能TIM3时钟

    TIM_TimeBaseInitStructure.TIM_Prescaler      = psc;                   //定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode    = TIM_CounterMode_Down;  //向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period         = arr;                   //自动重装载值
    TIM_TimeBaseInitStructure.TIM_ClockDivision  = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);                 //初始化定时器3
}




/******************************************************************************
 * @brief      初始化定时器延时函数，定时1us
 *
 * @param[in]  无
 *
 * @return     无
 *
 ******************************************************************************/
int Delay_Init(void)
{
    TIM3_Init(100 - 1, 84 - 1); //定时1us
    return 0;
}


/******************************************************************************
 * @brief      微秒级延时
 *
 * @param[in]  us    :  延时微秒数
 *
 * @return     无
 *
 ******************************************************************************/
void Delay_Us(uint32_t us)
{
    TIM3->ARR = us;           //自动重载器
    TIM3->CNT = TIM3->ARR;    //计数器
    TIM_Cmd(TIM3, ENABLE);    //启动定时器
    while(TIM3->CNT) {;}      //等待延时时间到
    TIM_Cmd(TIM3, DISABLE);   //关闭定时器
}




/******************************************************************************
 * @brief      毫秒级延时,不建议在OS中使用
 *
 * @param[in]  us    :  延时毫秒数
 *
 * @return     无
 *
 ******************************************************************************/
void Delay_Ms(uint32_t ms)
{
    for(int i = 0; i < ms; i++){
        Delay_Us(1000);
    }
}
