#pragma section all "cpu1_dsram"
#include "includes.h"

uint8 key1_state=1,key2_state=1,key3_state=1,key4_state=1;//����״̬��ȡ
uint8 parameter=1;//��������ĸ�����
uint8 send_flag=0;//������������ֻ��ֵһ��
extern uint8   bluetooth_ch9141_data;
extern PID_t Steer_PID;//�⻷
extern PID_t Speed_PID;//�ڻ�

extern int Charge_flag;//����־λ
extern int Charge_Flag_start;//������ʼ����־λ
extern int32 Charge_Time;//������ʱ�䡪����λ ms
extern int32 second;//������ʱ�䡪����λ s
extern int32 time;//������ʱ�䡪����λ s

float p1,i1,d1,p2,i2,d2;



void test(void)
{
    while(1)
    {
        //��ʾ��ǰ����ֵ
       // ips114_show_uint(0, 20, parameter, 3);

        //ѡ����Ҫ���ڵĲ���
        key2_state=gpio_get_level(P22_1);
        while(!gpio_get_level(P22_1));
        if(key2_state==0)
        {
            ips114_show_uint(0, 20, 1 , 3);


//            gpio_set_level(SD, 0);
//            pwm_set_duty(Buck_PWM,0);
//
//            //��־λ�ı�״̬
//            Charge_flag=0;
//            //center_flag=1;
//            Charge_Flag_start=0;
//
//            //���ʱ�临λ
//            Charge_Time=0;
//            second=0;
//            time=0;
//
//            parameter++;
//            if(parameter==7)
//            {
//                parameter=1;
//            }
//            ips114_show_uint(0, 20, parameter, 3);
//            send_flag=0;
        }

        //ȷ��ѡ�����
        key3_state=gpio_get_level(P22_2);
        while(!gpio_get_level(P22_2));

        if(key3_state==0)
        {

            ips114_show_uint(0, 20, 2, 3);
//          switch(parameter)
//          {
//            case 1:
//                if(send_flag==0)
//                {
//                    Speed_PID.Kp=(float)bluetooth_ch9141_data*0.1;
//                    ips114_show_uint(0, 40, bluetooth_ch9141_data, 3);
//                    send_flag=1;
//                }
//                ips114_show_uint(0, 60, 22, 3);
//                break;
//            case 2:
//                if(send_flag==0)
//                {
//                    Speed_PID.Ki=(float)bluetooth_ch9141_data*0.1;
//                    ips114_show_uint(0, 40, bluetooth_ch9141_data, 3);
//                    send_flag=1;
//                }
//                 break;
//            case 3:
//                if(send_flag==0)
//                {
//                    Speed_PID.Kd=(float)bluetooth_ch9141_data*0.1;
//                    ips114_show_uint(0, 40, bluetooth_ch9141_data, 3);
//                    send_flag=1;
//                }
//                 break;
//            case 4:
//                if(send_flag==0)
//                {
//                    Steer_PID.Kp=(float)bluetooth_ch9141_data*0.1;
//                    ips114_show_uint(0, 40, bluetooth_ch9141_data, 3);
//                    send_flag=1;
//                }
//                 break;
//            case 5:
//                if(send_flag==0)
//                {
//                    Steer_PID.Ki=(float)bluetooth_ch9141_data*0.1;
//                    ips114_show_uint(0, 40, bluetooth_ch9141_data, 3);
//                    send_flag=1;
//                }
//                 break;
//            case 6:
//                if(send_flag==0)
//                {
//                    Steer_PID.Kd=(float)bluetooth_ch9141_data*0.1;
//                    ips114_show_uint(0, 40, bluetooth_ch9141_data, 3);
//                    send_flag=1;
//                }
//                 break;
//          }//switch
        }//if



        //�˳�����
        key4_state=gpio_get_level(P22_3);
        while(!gpio_get_level(P22_3));
        if(key4_state==0)
        {
            break;
        }
    }
}
#pragma section all restore
