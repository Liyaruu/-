#include "includes.h"



void ADC_init(void)
{
    adc_init(ADC1_CH2_A10, ADC_12BIT); //mic_1
    adc_init(ADC1_CH3_A11, ADC_12BIT); //mic_2
    adc_init(ADC1_CH4_A12, ADC_12BIT); //mic_3
    adc_init(ADC1_CH5_A13, ADC_12BIT); //mic_4
    //�����źŲɼ�

//    adc_init(ADC0_CH0_A0, ADC_12BIT); //��ż��A0��
//    adc_init(ADC0_CH1_A1, ADC_12BIT); //��ż��A1��
//    adc_init(ADC0_CH2_A2, ADC_12BIT); //��ż��A2��
//    adc_init(ADC0_CH3_A3, ADC_12BIT); //��ż��A3��
}

void GPIO_init(void)
{
//    gpio_init(P33_10, GPO, 0, GPO_PUSH_PULL);//bee
//    gpio_init(P20_8, GPO, 0, GPO_PUSH_PULL);//������

    //������ʼ��
    gpio_init(P22_0, GPI, 1, GPO_PUSH_PULL);//ע����Ҫ��Ϊ����ģʽ
    gpio_init(P22_1, GPI, 1, GPO_PUSH_PULL);
    gpio_init(P22_2, GPI, 1, GPO_PUSH_PULL);
    gpio_init(P22_3, GPI, 1, GPO_PUSH_PULL);

}

void PWM_init(void)
{

    pwm_init(MOTOR3_A, 17000, 0);
    pwm_init(MOTOR3_B, 17000, 0);
    //��������Ϊ�ҵ��
    pwm_init(MOTOR4_A, 17000, 0);
    pwm_init(MOTOR4_B, 17000, 0);
    //��������Ϊ����
    //A��ֵʱ��ת�����������������߶��Ӿ�Ϊ�Ϻ��ºڣ��ź�������->��(��->��)����Ϊ3A,3B,4A,4B
}


void init_cpu1(void)
{
//    encoder_quad_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);//��������ʼ��
//    encoder_quad_init(TIM6_ENCODER, TIM6_ENCODER_CH1_P20_3, TIM6_ENCODER_CH2_P20_0);

    ips114_set_dir(IPS114_CROSSWISE_180);//����ģʽ
    ips114_set_font(IPS114_8X16_FONT);
    ips114_set_color(RGB565_WHITE, RGB565_BLACK);
    ips114_init();

    PWM_init();

    GPIO_init();

    bluetooth_ch9141_init();

    icm20602_init();

    Charge_init();

    PID_Init();

    pit_ms_init(CCU60_CH1, 5);//�������жϳ�ʼ��
}

void init_cpu0(void)
{
//    ips114_set_dir(IPS114_CROSSWISE_180);//����ģʽ
//    ips114_set_font(IPS114_8X16_FONT);
//    ips114_set_color(RGB565_WHITE, RGB565_BLACK);
//    ips114_init();

    ADC_init();

    pit_us_init(CCU60_CH0, 100);//�����źŲɼ�10KHz
}

