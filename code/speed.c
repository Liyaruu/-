#pragma section all "cpu1_dsram"

#include "includes.h"

int16 N_L=0, N_R = 0;     //��������ȡֵ
float V_L, V_R = 0.0;   //ʵ���ٶ�

int32 OutPWM_L, OutPWM_R = 0;

int32 Goal_PWM_L = 5500;//5500
int32 Goal_PWM_R = 5500;
extern int32 Differ_PWM;
extern float ANGLE[3];

int16 center_flag = 0,Specil=0;

uint16 Direction_Flag = 1;  //ǰ�������־λ��1��ǰ����0������

//void Get_Speed(void)
//{
//    N_L= encoder_get_count(TIM5_ENCODER);                           // �ɼ���Ӧ����������
//    encoder_clear_count(TIM5_ENCODER);                              // �����Ӧ����
//    N_R =encoder_get_count(TIM6_ENCODER);                          // �ɼ���Ӧ����������
//    encoder_clear_count(TIM6_ENCODER);
//
////    V_R= N_R*21.0/(2321.1*0.005);//�����ٶ�=������*�ܳ�/2368*�ж�����;
////    V_L= N_L*21.0/(2321.1*0.005);//�����ٶ�=������*�ܳ�/2368*�ж�����;
//
//}

void Motor_PWM(void)
{
//        OutPWM_L = 6000;   //����
//        OutPWM_R = 6000;
//        pwm_set_duty(MOTOR3_A, 0);
//        pwm_set_duty(MOTOR3_B, OutPWM_R);
//
//        pwm_set_duty(MOTOR4_A, 0);//ע�������ֻ���
//        pwm_set_duty(MOTOR4_B, OutPWM_L);

    //0���180�����⴦��
     if(ANGLE[0]>150)//150��~180��
     {
         pwm_set_duty(MOTOR3_A, 0);
         pwm_set_duty(MOTOR3_B, 0);

         pwm_set_duty(MOTOR4_A, 4000);//ע�������ֻ���
         pwm_set_duty(MOTOR4_B, 0);
         Specil=1;
     }

     if(ANGLE[0]>=0&&ANGLE[0]<30)//0��~30��
     {
         pwm_set_duty(MOTOR3_A, 4000);
         pwm_set_duty(MOTOR3_B, 0);

         pwm_set_duty(MOTOR4_A, 0);//ע�������ֻ���
         pwm_set_duty(MOTOR4_B, 0);
         Specil=1;
     }

    if(Specil== 0)
    {
        OutPWM_R = Goal_PWM_R + Differ_PWM;
        OutPWM_L = Goal_PWM_L - Differ_PWM;

        if(Direction_Flag == 1)//30��~150��
        {
            pwm_set_duty(MOTOR3_A, OutPWM_R);
            pwm_set_duty(MOTOR3_B, 0);

            pwm_set_duty(MOTOR4_A, OutPWM_L);//ע�������ֻ���
            pwm_set_duty(MOTOR4_B, 0);
        }
        else if(Direction_Flag == 0)//-150��~-30��
        {
            if(ANGLE[0]<-90)
            {
                pwm_set_duty(MOTOR3_A, 0);
                pwm_set_duty(MOTOR3_B, 4000);

                pwm_set_duty(MOTOR4_A, 0);
                pwm_set_duty(MOTOR4_B, 0);//�����
            }
            else
            {
                pwm_set_duty(MOTOR3_A, 0);
                pwm_set_duty(MOTOR3_B, 0);

                pwm_set_duty(MOTOR4_A, 0);
                pwm_set_duty(MOTOR4_B, 4000);//�Һ���
            }
        }
    }
    Specil=0;


}

void Motor_stop(void)//�������������ͣ��
{
    pwm_set_duty(MOTOR3_A, 0);
    pwm_set_duty(MOTOR3_B, 0);
    pwm_set_duty(MOTOR4_A, 0);
    pwm_set_duty(MOTOR4_B, 0);
}

#pragma section all restore
