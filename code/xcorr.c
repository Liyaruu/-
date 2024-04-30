#pragma section all "cpu0_dsram"
#include "xcorr.h"
#include<stdio.h>
#include<math.h>
#include<complex.h>

//#define MIC_NUM 3072
//#define Sample_MAX 1024
//�ź�Ϊ250Hz~2000Hz

#define FFT_MAX     410//Ƶ������
#define FFT_MIN     51//Ƶ������
#define FFT_NUM     2048//����ش������ݳ���

float X1,X2;//�Խ����ϵĹ�������Դ����֮��
float angle_i=45/34.6;//45/34.6 //�Ƕ�ϵ����//��С�Ƕȼ��Ϊ4.5��
float xcorr_angle=0.0;//��������ýǶ�ֵ
int angle_flag=0;
int num=0;//�жϴ��������źŵ�����������
int MAXi_14,MAXi_23,flag=0;//mic14��mic23�������������±�ֵ

extern float ANGLE[3];//cpu1-����ؽǶ�ֵ
extern int16 center_flag;

/*������ģ*/
float32 value(cfloat32 a)
{
    float32 c;
    c =sqrt( a.real * a.real+ a.imag * a.imag);
    return c;
}


/*����ظ������*/
cfloat32 xcorr_mul(cfloat32 a, cfloat32 b)
{
    cfloat32 c;
    c.real = a.real * b.real + a.imag * b.imag;
    c.imag = a.imag * b.real-a.real * b.imag;
    return c;
}

/*�������*/
cfloat32 mul(cfloat32 a, cfloat32 b)
{
    cfloat32 c;
    c.real = a.real * b.real - a.imag * b.imag;
    c.imag = a.real * b.imag + a.imag * b.real;
    return c;
}

/*�������*/
cfloat32 add(cfloat32 a, cfloat32 b)
{
    cfloat32 c;
    c.real = a.real + b.real;
    c.imag = a.imag + b.imag;
    return c;
}

/*�������*/
cfloat32 minus(cfloat32 a, cfloat32 b)
{
    cfloat32 c;
    c.real = a.real - b.real;
    c.imag = a.imag - b.imag;
    return c;
}

/*���������ֵ*/
float Absolute_Value(cfloat32 a)
{
    float i,j;
    float value;
    i=a.real*a.real;
    j=a.imag*a.imag;
    value=i+j;
    return value;
}

/*���ٸ���Ҷ�任*/
void FFT(cfloat32* output, cfloat32* input, int n)
{
    int i, j, k=0,f=11, l = 0,m=n;
    cfloat32 wn, w, x1, x2;

    for (i = 0; i < n; i++)
    {
        k = f;
        for (m = i, j = 0; k > 0; m >>= 1)
        {
            j = (j << 1) | (m & 1);
            k--;
        }
        if(num==3)
        {
           if(j>=1024)
           {
               output[i].real = input[j-Sample_MAX].real;
               output[i].imag = 0.0;
           }
           else
           {
               output[i].real = input[j+2*Sample_MAX].real;
               output[i].imag = 0.0;
            }//if

         }
         else
         {
             output[i].real = input[j].real;
             output[i].imag = 0.0;
         }//if
    }//for
    for (l = 1; l < n; l <<= 1)
    {

        wn.real = (float)cos(PI / l);
        wn.imag = (float)-sin(PI / l);
        for (j = 0; j < n; j += l << 1)
        {
            w.real = 1.0;
            w.imag = 0.0;
            for (k = 0; k < l; k++, w = mul(w, wn))
            {
                 x1 = output[j + k];
                 x2 = mul(w, output[j + k + l]);
                 output[j + k] = add(x1, x2);
                 output[j + k + l] = minus(x1, x2);
             }
         }
     }

}

/*���ٸ���Ҷ��任*/
void IFFT(cfloat32* output, cfloat32* input, int n)
{
    int i, j, k = 0, f =11, l = 0,m=n;
    cfloat32 wn, w, x1, x2;

    for (i = 0; i < n; i++)
    {
        k = f;
        for (m = i, j = 0; k > 0; m >>= 1)
        {
            j = (j << 1) | (m & 1);
            k--;
        }
        output[i] = input[j];//��FFT��֮ͬ��
    }
    for (l = 1; l < n; l <<= 1)
    {
        wn.real = (float)cos(PI / l);
        wn.imag = (float)sin(PI / l);
        for (j = 0; j < n; j += l << 1)
        {
            w.real = 1.0;
            w.imag = 0.0;
            for (k = 0; k < l; k++, w = mul(w, wn))
            {
                x1 = output[j + k];
                x2 = mul(w, output[j + k + l]);
                output[j + k] = add(x1, x2);
                output[j + k + l] = minus(x1, x2);
            }
        }
    }

}


/*�������黥���*/
float xcorr(cfloat32 *mic_1,cfloat32 *mic_2)
{
    cfloat32 mic1_out[FFT_NUM];
    cfloat32 mic2_out[FFT_NUM];
    cfloat32 xcorr_out[FFT_NUM];

    int32 MAX_i=0;//��������ֵ������
    float32 Delay_12=0.0;//�����
    float32 MIC_MAX=0.0;//����ֵ
    float32 MAX_value=0.0;


    //fft

     FFT(mic1_out,mic_1,FFT_NUM);
     FFT(mic2_out,mic_2,FFT_NUM);


    //Ƶ�����
    for(int i=0; i<FFT_NUM; i++)
    {
        if(i>=FFT_MIN && i<=FFT_MAX)
        {
            xcorr_out[i]=xcorr_mul(mic1_out[i],mic2_out[i]);//�����ź�Ƶ��ĳ˻�
            MAX_value = value(xcorr_out[i]);
            xcorr_out[i].real = xcorr_out[i].real /MAX_value;
            xcorr_out[i].imag = xcorr_out[i].imag /MAX_value;
        }
        else
        {
            xcorr_out[i].real=xcorr_out[i].imag=0.0;
        }
    }

    IFFT(mic2_out,xcorr_out,FFT_NUM);//ifft

    //ȡ���ֵ��ֻ��ʵ����
    MIC_MAX=mic2_out[0].real;
    MAX_i=0;
    for(int i=1; i<FFT_NUM; i++)
    {
        if(mic2_out[i].real>MIC_MAX)
         {
           MIC_MAX=mic2_out[i].real;
           MAX_i=i;
         }
    }

//�鿴14��23����������ֵ����
    if(flag==0)
    {
        MAXi_14=MAX_i;
        flag=1;
    }
    else
    {
        MAXi_23=MAX_i;
        flag=0;
    }

    //�ҵ����ֵ��Ӧ�±�MAX_i��Delay_12��λΪcm
    if(MAX_i>1024)
    {
        Delay_12=(float32)(FFT_NUM-MAX_i)*(-3.46);
    }
    else
    {
        Delay_12=MAX_i*(3.46);
    }
    return Delay_12;
}

/*�Ƕ�Ϊ-180~180���Ƕ���err����Ϊ���Թ�ϵ*/
float Get_Angle(float X14,float X23,float err)
{
    float angle;
    if(X14<0&&X23<=0)//�Ƕȷ�Χ(45,135]
    {
        angle=90.0+angle_i*err;
    }
    else if(X14<=0&&X23>0)//�Ƕȷ�Χ(135,180]U[-180,-135]
    {
        if(err>=0)
        {
            angle=180.0-angle_i*err;
        }
        else
        {
            angle=-180.0-angle_i*err;
        }
    }
    else if(X14>0&&X23>=0)//�Ƕȷ�Χ(-135,-45]
    {
        angle=-90.0+angle_i*err;
    }
    else//�Ƕȷ�Χ(-45,45]
    {
        angle=-angle_i*err;
    }
    return angle;
}

/*����ش���*/
void XCORR(void)
{
    float err=0.0;
    //������Ҫ50ms���ң����ɼ�1024��������Ҫ100ms����

    if(mic_flag == 1&&num!=0)
    {
       if(num==2)
       {
           X1=xcorr(mic_1+Sample_MAX,mic_4+Sample_MAX);//x1��x4����Դ����Զ����
           X2=xcorr(mic_2+Sample_MAX,mic_3+Sample_MAX);
       }
       else
       {
           X1=xcorr(mic_1,mic_4);//x1��x4����Դ����Զ����
           X2=xcorr(mic_2,mic_3);
       }

       err=fabs(X1)-fabs(X2);

       if(fabs(err)<50&&fabs(X1)<50&&fabs(X2)<50)//��������
       {
           angle_flag=1;
           xcorr_angle=Get_Angle(X1,X2,err);
           angle_flag=0;
       }
//       gpio_set_level(P21_5, 0);
//       system_delay_ms(500);
//       gpio_set_level(P21_5, 1);

        //center_flag=1;//���ʹ��

        num=0;


    }
}

#pragma section all restore
