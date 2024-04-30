/*********************************************************************************************************************
* TC377 Opensourec Library ����TC377 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC377 ��Դ���һ����
*
* TC377 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          isr
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC377TP
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-11-03       pudding            first version
********************************************************************************************************************/

#include "isr_config.h"
#include "isr.h"
#include "includes.h"

#define Sample_MAX 1024           // ����ɼ���˷����
#define MIC_NUM 3072

uint16  mic_flag=0;                // ��˷�Ŀǰ�ɲɼ���־λ
uint16 mic_sample_num=0;          // ��ǰ�ɼ�����
float icm_add[2]={0};//angle[0]Ϊ�����ǻ���ֵ

cfloat32 mic_1[MIC_NUM]; // ������˷����ݵĻ���
cfloat32 mic_2[MIC_NUM];
cfloat32 mic_3[MIC_NUM];
cfloat32 mic_4[MIC_NUM];

uint16 L_flag=0;
uint16 L_count;
uint16 run_flag=0;
extern uint16 L1,L2,L3,L4;//��вɼ�

float first_flag=0;//��һ�βɼ���־λ
extern int num;//��xcorr.c������

extern int Charge_flag;//����־λ��������;û��ʱ���г��
extern int16 center_flag;
extern float Value_VB;
extern uint8 key1_state;
extern PID_t Steer_PID;//����pid�⻷
extern PID_t Speed_PID;//����pid�ڻ�


// **************************** PIT�жϺ��� ****************************
IFX_INTERRUPT(cc60_pit_ch0_isr, CCU6_0_CH0_INT_VECTAB_NUM, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH0);
//    tsl1401_collect_pit_handler();                  // ����CCD�ɼ�

    // �ɼ���˷�����
    // ��Ϊ����FFT�����ʱ��������Ҫ��ȥֱ��ƫ�����������̶���2000��ʵ��Ӧ���Լ�ͨ������û������ʱ����ֵ��ȷ������Ӧ�ü�ȥ����
    // ��û��������ʱ�򣬼�ȥƫ�ú�õ�����ֵӦ������0������˵������ƫ�þ�����ȷ��
    mic_1[mic_sample_num].real = adc_convert(ADC1_CH2_A10)-2000;
    mic_1[mic_sample_num].imag = 0;

    mic_2[mic_sample_num].real = adc_convert(ADC1_CH3_A11) - 2000;
    mic_2[mic_sample_num].imag = 0;

    mic_3[mic_sample_num].real = adc_convert(ADC1_CH4_A12) - 2000;
    mic_3[mic_sample_num].imag = 0;

    mic_4[mic_sample_num].real = adc_convert(ADC1_CH5_A13)-2000;
    mic_4[mic_sample_num].imag = 0;

    mic_sample_num++;

    //��һ�βɼ�����ʱ��Ҫ���⴦��
    if(mic_sample_num==2048&&first_flag==0)//�Ե�һ�βɼ����⴦��
    {
        first_flag=1;
        mic_flag=1;
    }

    //num=0ʱ�������д���
    //num=1ʱ������1��2
    //num=2ʱ������2��3
    //num=3ʱ������3��1
    if(mic_sample_num==2048&&first_flag==1)
    {
        num=1;
    }
    if(mic_sample_num==3072&&first_flag==1)
    {
        num=2;
        mic_sample_num = 0;
    }
    if(mic_sample_num==1024&&first_flag==1)
    {
       num=3;
    }


}

float gyro_x = 0.0;
extern int16 icm20602_gyro_x;

IFX_INTERRUPT(cc60_pit_ch1_isr, CCU6_0_CH1_INT_VECTAB_NUM, CCU6_0_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU60_CH1);

    icm20602_get_gyro();
    gyro_x = icm20602_gyro_transition(icm20602_gyro_x);
    //icm_add[0] += gyro_x * (-0.005)+0.00122;
    //Get_Speed();


    if(Charge_flag==1)
    {
        gpio_set_level(SD, 1);

        Charge_Start();
    }

//��в���
//        if(L_flag==1)
//        {
//            L_count++;
//        }
//        else
//        {
//            L1=adc_mean_filter_convert (ADC0_CH0_A0,4);
//            L2=adc_mean_filter_convert (ADC0_CH1_A1,4);
//            if(L1>500||L2>500)
//            {
//                 Motor_stop();//С��ֹͣ
//                 PID_Refresh(Steer_PID);//����pid����
//                 PID_Refresh(Steer_PID);
//                 run_flag=1;
//                 L_flag=1;
//             }
//        }
//
//        if(L_count==600)//���ֹͣ����ʱ��Ϊ 600*5ms
//        {
//            L_flag=0;
//            L_count=0;
//        }

}

IFX_INTERRUPT(cc61_pit_ch0_isr, CCU6_1_CH0_INT_VECTAB_NUM, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU61_CH0);




}

IFX_INTERRUPT(cc61_pit_ch1_isr, CCU6_1_CH1_INT_VECTAB_NUM, CCU6_1_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    pit_clear_flag(CCU61_CH1);





}
// **************************** PIT�жϺ��� ****************************


// **************************** �ⲿ�жϺ��� ****************************
IFX_INTERRUPT(exti_ch0_ch4_isr, EXTI_CH0_CH4_INT_VECTAB_NUM, EXTI_CH0_CH4_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    if(exti_flag_get(ERU_CH0_REQ0_P15_4))           // ͨ��0�ж�
    {
        exti_flag_clear(ERU_CH0_REQ0_P15_4);
        wireless_module_uart_handler();                 // ����ģ��ͳһ�ص�����



    }

    if(exti_flag_get(ERU_CH4_REQ13_P15_5))          // ͨ��4�ж�
    {
        exti_flag_clear(ERU_CH4_REQ13_P15_5);




    }
}

IFX_INTERRUPT(exti_ch1_ch5_isr, EXTI_CH1_CH5_INT_VECTAB_NUM, EXTI_CH1_CH5_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    if(exti_flag_get(ERU_CH1_REQ10_P14_3))          // ͨ��1�ж�
    {
        exti_flag_clear(ERU_CH1_REQ10_P14_3);



    }

    if(exti_flag_get(ERU_CH5_REQ1_P15_8))           // ͨ��5�ж�
    {
        exti_flag_clear(ERU_CH5_REQ1_P15_8);




    }
}

// ��������ͷpclk����Ĭ��ռ���� 2ͨ�������ڴ���DMA��������ﲻ�ٶ����жϺ���
// IFX_INTERRUPT(exti_ch2_ch6_isr, EXTI_CH2_CH6_INT_VECTAB_NUM, EXTI_CH2_CH6_INT_PRIO)
// {
//  interrupt_global_enable(0);                     // �����ж�Ƕ��
//  if(exti_flag_get(ERU_CH2_REQ7_P00_4))           // ͨ��2�ж�
//  {
//      exti_flag_clear(ERU_CH2_REQ7_P00_4);
//  }
//  if(exti_flag_get(ERU_CH6_REQ9_P20_0))           // ͨ��6�ж�
//  {
//      exti_flag_clear(ERU_CH6_REQ9_P20_0);
//  }
// }

IFX_INTERRUPT(exti_ch3_ch7_isr, EXTI_CH3_CH7_INT_VECTAB_NUM, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    if(exti_flag_get(ERU_CH3_REQ6_P02_0))           // ͨ��3�ж�
    {
        exti_flag_clear(ERU_CH3_REQ6_P02_0);
        camera_vsync_handler();                     // ����ͷ�����ɼ�ͳһ�ص�����
    }
    if(exti_flag_get(ERU_CH7_REQ16_P15_1))          // ͨ��7�ж�
    {
        exti_flag_clear(ERU_CH7_REQ16_P15_1);




    }
}
// **************************** �ⲿ�жϺ��� ****************************


// **************************** DMA�жϺ��� ****************************
IFX_INTERRUPT(dma_ch5_isr, DMA_INT_VECTAB_NUM, DMA_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    camera_dma_handler();                           // ����ͷ�ɼ����ͳһ�ص�����
}
// **************************** DMA�жϺ��� ****************************


// **************************** �����жϺ��� ****************************
IFX_INTERRUPT(uart0_tx_isr, UART0_INT_VECTAB_NUM, UART0_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart0_handle);


}
IFX_INTERRUPT(uart0_rx_isr, UART0_INT_VECTAB_NUM, UART0_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart0_handle);

#if DEBUG_UART_USE_INTERRUPT                        // ������� debug �����ж�
        debug_interrupr_handler();                  // ���� debug ���ڽ��մ����� ���ݻᱻ debug ���λ�������ȡ
#endif                                              // ����޸��� DEBUG_UART_INDEX ����δ�����Ҫ�ŵ���Ӧ�Ĵ����ж�ȥ

}
IFX_INTERRUPT(uart0_er_isr, UART0_INT_VECTAB_NUM, UART0_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart0_handle);



}

// ����1Ĭ�����ӵ�����ͷ���ô���
IFX_INTERRUPT(uart1_tx_isr, UART1_INT_VECTAB_NUM, UART1_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart1_handle);




}
IFX_INTERRUPT(uart1_rx_isr, UART1_INT_VECTAB_NUM, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart1_handle);
    camera_uart_handler();                          // ����ͷ��������ͳһ�ص�����
}
IFX_INTERRUPT(uart1_er_isr, UART1_INT_VECTAB_NUM, UART1_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart1_handle);




}


// ����2Ĭ�����ӵ�����ת����ģ��
IFX_INTERRUPT(uart2_tx_isr, UART2_INT_VECTAB_NUM, UART2_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart2_handle);





}
IFX_INTERRUPT(uart2_rx_isr, UART2_INT_VECTAB_NUM, UART2_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart2_handle);
    wireless_module_uart_handler();                 // ����ģ��ͳһ�ص�����

}
IFX_INTERRUPT(uart2_er_isr, UART2_INT_VECTAB_NUM, UART2_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart2_handle);

}



IFX_INTERRUPT(uart3_tx_isr, UART3_INT_VECTAB_NUM, UART3_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrTransmit(&uart3_handle);




}
IFX_INTERRUPT(uart3_rx_isr, UART3_INT_VECTAB_NUM, UART3_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrReceive(&uart3_handle);





}
IFX_INTERRUPT(uart3_er_isr, UART3_INT_VECTAB_NUM, UART3_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // �����ж�Ƕ��
    IfxAsclin_Asc_isrError(&uart3_handle);





}
// **************************** �����жϺ��� ****************************
