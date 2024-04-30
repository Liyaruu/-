#pragma section all "cpu1_dsram"

#include "includes.h"

float Steer_Error[3] = {0.0};// �⻷-�ǶȻ�
float Speed_Error[4] = {0.0};// �ڻ�-���ٶȻ�

extern float ANGLE[3];//Ŀ��Ƕ�
extern uint16 Direction_Flag;
extern int16 center_flag ;
extern float gyro_x;
int32 Differ_PWM = 0;

PID_t Steer_PID;//�⻷
PID_t Speed_PID;//�ڻ�

//PID������ʼ��
void PID_Init(void)
{

    Steer_PID.LastData = Speed_PID.LastData = 0.0;
    Steer_PID.LastError = Speed_PID.LastError = 0.0;
    Steer_PID.PrevError = Speed_PID.PrevError = 0.0;
    Steer_PID.SumError = Speed_PID.SumError = 0.0;
    //�⻷
    Steer_PID.Kp = 1.5;
    Steer_PID.Ki = 2.5;
    Steer_PID.Kd = 0.5;

    //�ڻ�

    //�ٶ�PWM=4000ʱ 2.0  3.5  0.3

    Speed_PID.Kp = 2.0;//2.5
    Speed_PID.Ki = 3.5;//4.0
    Speed_PID.Kd = 0.3;//0.3
}

void PID_Refresh(PID_t p)
{
    p.LastData=0;
    p.LastError=0;
    p.PrevError=0;
    p.SumError=0;
}

//��charge.c��ʹ��
////λ��ʽPID
//float PID_Realize(PID_t  p, float NowData, float SetData)
//{
//    float iError,   // ��ǰ���
//          Realize;  // ���ó���ʵ������
//
//    iError = SetData - NowData; // ���㵱ǰ���
//    p.SumError += p.Ki * iError;   // ������
////    p->SumError = RANGE_PROTECT(p->SumError, p->T, -(p->T));
//
//    if(p.SumError >= 40 || p.SumError <= -40)
//    {
//        if(p.SumError >= 40)
//        {
//            p.SumError = 40;
//        }
//        if(p.SumError <= -40)
//        {
//            p.SumError = -40;
//        }
//    }
//    Realize =  (p.Kp) * iError + p.SumError + p.Kd * (iError - p.LastError) ;
//
//    p.PrevError = p.LastError;    // ����ǰ�����
//    p.LastError = iError;          // �����ϴ����
//    p.LastData  = NowData;         // �����ϴ�����
//
//    return Realize; // ����ʵ��ֵ
//}

//����ʽPID
int32 PID_Increase(PID_t  p, int32 NowData, int32 SetData)
{
    int32 iError, //��ǰ���
        Increase; //���ó���ʵ������

    iError = SetData - NowData; // ���㵱ǰ���

    Increase = (int32)(p.Kp * (iError - p.LastError) + p.Ki * iError + p.Kd * (iError - 2 * p.LastError + p.PrevError));

    p.PrevError = p.LastError; // ����ǰ�����
    p.LastError = (float)iError;       // �����ϴ����

    return Increase; // ��������
}



void DifferControl(void)
{
    if(ANGLE[0] <= 0.0)
    {
        Direction_Flag = 0;
    }
    else if(ANGLE[0] >= 0.0)
    {
        Direction_Flag = 1;
    }
    if(Direction_Flag == 1)
    {

       DirectError_Get();//��ȡƫ��


       Direction_out(); //ƫ��PWM��ȡ
    }

}


void DirectError_Get(void)  //�⻷,�ǶȻ�
{



      Steer_Error[0] = (float)PID_Increase(Steer_PID, ANGLE[0], 90.0);

//    b = Steer_Error[0] * 3.8 - 0.25/150;
//    Differ_PWM = (int32) b ;
//
//    if(Differ_PWM>=650)
//    {
//        Differ_PWM=650;
//    }
//    if(Differ_PWM<=-650)
//    {
//        Differ_PWM=-650;
//    }
//    Steer_Error[0]=0; //������仰����ת��ֻ���ڻ����⻷���Ϊ0
}


float error[3] = {0.0};

void Direction_out(void)//�ڻ�,���ٶȻ�
{

//   float a;
//  error[0] = Steer_Error[0]*1.0 - gyro_x*1.0; //�����ź�ƫ��+������10.3  0.19

  Speed_Error[0] = (float)PID_Increase(Speed_PID, gyro_x*1.0, Steer_Error[0]);
//  error[1] =  error[0];

//  Speed_Error[3] = Speed_Error[2];
//  Speed_Error[2] = Speed_Error[1];
//  Speed_Error[1] = Speed_Error[0];

  Differ_PWM=Speed_Error[0];
//  a = Speed_Error[0]*0.8+Speed_Error[1]*0.1+Speed_Error[2]*0.06+Speed_Error[3]*0.04;//�˲�
//  Differ_PWM = (int32)a;
  //ips114_show_int(0, 0, Differ_PWM, 5);

  if(Differ_PWM>=1200)
  {
      Differ_PWM=1200;
  }
  if(Differ_PWM<=-1200)
  {
      Differ_PWM=-1200;
  }
}
#pragma section all restore

