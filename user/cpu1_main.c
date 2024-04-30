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
* �ļ�����          cpu1_main
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

#include "zf_common_headfile.h"
#pragma section all "cpu1_dsram"
#include "includes.h"

float ANGLE[3]={0.0};//cpu1-����ؽǶ�ֵ
extern float xcorr_angle;//cpu0-�������нǶȽ��
extern int MAXi_14,MAXi_23;//mic14��mic23�������������±�ֵ
extern int angle_flag;//ANGLE[0]�Ƿ��ڱ�cpu1ʹ��

uint16 L1,L2,L3,L4;//ADC�ɼ����ֵ
extern uint16 L_flag;//�򿪵�м���־λ
extern uint16 run_flag;

extern uint8   bluetooth_ch9141_data;//������������

extern int16 center_flag;//�򿪵����־λ

extern float gyro_x;
extern int32 Differ_PWM;//����pid���ֵ


extern uint8 key1_state;//key1״̬
extern int Charge_flag;//����־λ
//extern int16 N_L, N_R;     //��������ȡֵ
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� enableInterrupts(); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� disableInterrupts(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� enableInterrupts(); �������жϵ���Ӧ��


// **************************** �������� ****************************
void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������
    //char txt[256];

//    //��һ��ʹ��ʱ
//    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))                      // �ж��Ƿ�������
//    {
//        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);                // ������һҳ
//    }

//    //��ȡFlash��������
//    flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);           // �����ݴ� flash ��ȡ��������
//    if(flash_union_buffer[0].int8_type==1)
//    {
//
//        Charge_flag=0;//��ʼ���ɼ�����Ѱ���ű�
//    }
//    flash_buffer_clear();
//
    init_cpu1();


    // �˴���д�û����� ���������ʼ�������
    //cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        // �˴���д��Ҫѭ��ִ�еĴ���
        //����������
        //        Motor_PWM();
//                ips114_show_int(0, 0,MAXi_14, 5);
//                ips114_show_int(0, 20,MAXi_23, 5);


//        //����1����Flash�޸�
//        //����2���¡���С��ֱ������
//        //����3���¡����ٰ���λ���������������³����12V
//            key1_state=gpio_get_level(P22_0);
//            while(!gpio_get_level(P22_0));
//            if(key1_state==0)
//            {
//                 ips114_show_string(0, 0, "test");
//                 test();
//                 ips114_show_string(0, 0, "exit");
//            }

        if(angle_flag==0)
        {
            ANGLE[0]=xcorr_angle;

        }

        //ips114_show_float(20,0,ANGLE[0], 4, 3);

        if(center_flag==1)
        {
            DifferControl();

            Motor_PWM();

        }


//���ӵ�м��
//            if(run_flag==1)
//            {
//                system_delay_ms(250);
//                run_flag=0;
//            }

//��������
//                sprintf(txt, "%d,%f\n",Differ_PWM,ANGLE[0]);
//                uart_write_string(UART_2,txt);
//                system_delay_ms(100);


        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}
#pragma section all restore
