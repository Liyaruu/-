#pragma section all "cpu0_dsram"

#ifndef XCORR_H
#define XCORR_H

#include "SysSe/Math/Ifx_FftF32.h"
#include "isr.h"

#define MIC_NUM 3072    // ����ɼ���˷����
#define Sample_MAX 1024


extern float X1,X2;
extern int MAXi_14,MAXi_23;//��������ֵλ��
extern uint16  mic_flag;   //��һ����˷�ɼ���ɱ�־λ


extern cfloat32 mic_1[MIC_NUM]; // ������˷����ݵĻ���
extern cfloat32 mic_2[MIC_NUM];
extern cfloat32 mic_3[MIC_NUM];
extern cfloat32 mic_4[MIC_NUM];

float xcorr(cfloat32 *demo_1,cfloat32 *demo_2);
float Get_Angle(float X14,float X23,float err);
void XCORR(void);

#endif

#pragma section all restore
