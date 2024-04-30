#pragma section all "cpu1_dsram"

#include "includes.h"

float Value_RV;//VCC��ѹֵ��������ѹ
float Value_VB;//�������ݵ�ѹֵ
float Value_RI;//������

PID_t Charge_PID;//���PID
float Charge_P=34.0;//Ŀ��㹦��ֵ
float Now_P=0.0;//��ǰ����ֵ
int32 Charge_Time=0;//������ʱ�䡪����λ ms
int32 second=0;//������ʱ�䡪����λ s
int32 time=0;//������ʱ�䡪����λ s
int32 time_flag=0;
int32 ci=0;

int32 Charge_PWM=5000;//buck��·��ʼPWMֵ

int Charge_flag=1;//����־λ
int Charge_Flag_start=0;//������ʼ����־λ
int Charge_Protect=0;//��籣��

extern int16 center_flag;

void Charge_init(void)
{
    adc_init(RV, ADC_12BIT); //RV
    adc_init(VB, ADC_12BIT); //VB
    adc_init(RI, ADC_12BIT); //RI

    pwm_init(Buck_PWM, 100000, 0);//Buck��·����Ƶ��250KHz
    gpio_init(SD, GPO, 0, GPO_PUSH_PULL);


    //��ʼ��PID
    Charge_PID.LastData = 0.0;
    Charge_PID.LastError = 0.0;
    Charge_PID.PrevError = 0.0;
    Charge_PID.SumError = 0.0;
    Charge_PID.Kp = 2.0;
    Charge_PID.Ki = 0.2;
    Charge_PID.Kd = 0;

}

//λ��ʽPID
float PID_Realize(PID_t  p, float NowData, float SetData)
{
    float iError,   // ��ǰ���
          Realize;  // ���ó���ʵ������

    iError = SetData - NowData; // ���㵱ǰ���
    p.SumError += p.Ki * iError;

    if(p.SumError >= 20 || p.SumError <= -20)
    {
        if(p.SumError >= 20)
        {
             p.SumError = 20;
        }
        if(p.SumError <= -20)
        {
             p.SumError = -20;
        }
    }

    // ����������ʱ�����ֹ����³����������� CHARGE_P * 0.8ʱIΪ0������50wʱ��50 * 0.8=40������40���ϣ�I��������
    if(fabs(iError)>SetData*0.7)
    {
        Realize =  (p.Kp) * iError + p.Kd * (iError - p.LastError) ;
        p.SumError=0;
    }
    else
    {
        Realize =  (p.Kp) * iError + p.SumError + p.Kd * (iError - p.LastError) ;
    }

    p.PrevError = p.LastError;    // ����ǰ�����
    p.LastError = iError;          // �����ϴ����
    p.LastData  = NowData;         // �����ϴ�����


    return Realize; // ����ʵ��ֵ
}

//Buck��·����PWM
void Charge_out(float pwm)
{
    pwm=184*pwm+80;//��ѹ��ռ�ձȳ����Թ�ϵ

    //��籣��
    if(pwm>9000)
    {
        pwm=9000;
        //�ﲻ������ʱ�����ʽ���10W�Ա�������·
        if(Charge_Protect>50)
        {
            Charge_Protect=0;
            Charge_P-=2;
        }
        else
        {
            Charge_Protect++;
        }
    }
    if(pwm<4000)
    {
        pwm=4000;
    }
    pwm_set_duty(Buck_PWM, (int)pwm);
}

//�㹦�ʳ�����
void Charge_Ctrl(float Goal,float Actual)
{
    float Charge_Err=0.0;
    float Charge_Value=0.0;

    Charge_Err=PID_Realize(Charge_PID, Now_P, Charge_P);//PID�������
    Charge_Value=Charge_Err+Actual;//����

    Charge_Value=sqrt(Charge_Value*(float)time);//��ѹ

    Charge_out(Charge_Value);//���PWM

}

//���ʼ���·
void Charge_Check(void)
{

    if(Value_RV<2.0)//����VCCֵ��Ҫ����
    {
        Charge_Flag_start=0;
    }
    else
    {
        Charge_Flag_start=1;
    }

    //����ѹ�䵽11.5Vʱ����������
    if(Value_VB > 12.0)
    {
        ci++;
        if(ci==3)
        {

//            flash_buffer_clear();                                                       // ��ջ�����
//            flash_union_buffer[0].int8_type  = 1;
//            flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);        // ��ָ�� Flash ������ҳ��д�뻺��������

            gpio_set_level(SD, 0);
            pwm_set_duty(Buck_PWM,0);

            //��־λ�ı�״̬
            Charge_flag=0;
            Charge_Flag_start=0;
            center_flag=1;

            //���ʱ�临λ
            Charge_Time=0;
            second=0;
            time=0;
        }

    }


}

void Charge_Regular(void)
{
    if(Charge_Time==200)
    {
        Charge_Time=0;
        time++;
        second++;
    }

    if(second==1)
    {
        second=0;
        if(time_flag==1)
        {
            Charge_PWM+=100;
            if(Charge_PWM>=8000)
            {
                Charge_PWM=8000;
            }
        }
    }
    if(time==20)
    {
        time_flag=1;
    }

    pwm_set_duty(Buck_PWM,Charge_PWM);
}

//��纯��
void Charge_Start(void)
{
    //��ȡVCC������ѹ�������������������ݵ�ѹ
    Value_RV = adc_mean_filter_convert(RV,5)*0.0195;//*3.3*24.45/4096
    //Value_RI = adc_mean_filter_convert(RI,5)*0.002;//*3.3*2.5/4096
    Value_VB = adc_mean_filter_convert(VB,5)*0.0195;//*3.3*24.45/4096
    //Now_P=Value_RI*Value_RV;

    Charge_Check();

    //���RV��RI��VB��ADC����
//    ips114_show_float(0, 0, Value_RV, 4, 3);
//    ips114_show_float(0, 20,Value_RI, 4, 3);
//    ips114_show_float(0, 40,Value_VB, 4, 3);


    //�����Ĵ���
    if(Charge_Flag_start==1)
    {
        gpio_set_level(SD, 1);

        //��Ϊ���Ƴ��
        Charge_Regular();

        //PID+ǰ�����Ƴ��
        //Charge_Ctrl(Charge_P,Now_P);

        Charge_Time+=1;
    }
}




#pragma section all restore
