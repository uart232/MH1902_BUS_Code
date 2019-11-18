#include "mh523.h"
#include "iso14443_4.h"
#include "iso14443a.h"
#include "iso14443b.h"
#include "mifare.h"//��Ƶ�����ͷ�ļ�

#include "string.h"//C�⺯�� ͷ�ļ�

#include "gpio.h"

#include "ContactlessAPI.h"

//////////////////////////////////////////////////////////////////////
//��ȡ����  �ӿ���ȡ���������ݴ����MH523�ṹ����
//pCardSn�����Ŵ���׵�ַ
//���أ�����״̬��  0=�ɹ�  ����=ʧ��
///////////////////////////////////////////////////////////////////////
unsigned char pro_GetCardID(unsigned char *pCardSn) // no indenped file
{
    signed char status = -1;

    status = pcd_request( 0x26, MH523.CardTypebuf);
	  //printf("req = %d\r\n",status);
    if(MI_OK==status)
    {
        status=pcd_cascaded_anticoll(PICC_ANTICOLL1, MH523.coll_position, MH523.UIDbuf);
        if(status == MI_OK)
        {
            status=pcd_cascaded_select(PICC_ANTICOLL1, MH523.UIDbuf,&MH523.SAK);
            if(status == MI_OK) //ѡ���ɹ�
            {
                memcpy(pCardSn,MH523.UIDbuf,sizeof(MH523.UIDbuf));//��������
                if(MH523.SAK == 0x08)//�����M1 ֱ�ӷ���
                {
                    beep();
                    pcd_hlta();
                    return status;
                }
                else
                {
                    
                    status = pcd_rats_a(0, MH523.ATS,&MH523.ATSLength);	//����RATS
                    if(status == MI_OK)
                    {
                        beep();
                        return status;
                    }

                    return status;
                }
            }
        }
    }
    return status;
}


//////////////////////////////////////////////////////////////////////
//��ȡ������  �ӿ���ȡ���������ݴ����MH523�ṹ����
//���ݿ����ص�SAKΪ�����жϿ�����
//���أ�����
///////////////////////////////////////////////////////////////////////
unsigned char CaptureCardType(void) //���ؿ���
{
    unsigned char status;
    unsigned char CardType = 0xFF;
    //memset(&MH523,0,sizeof(MH523));//��ջ���
    //status = pro_GetCardID(MH523.Block);
    if(1)
    {
        //printf("SAK:%02X\r\n",MH523.SAK);
        switch(MH523.SAK)
        {
        case 0x08:
            CardType = M1_CARD;
            break;
        case 0x25:
            CardType = CPUAPP;
            break;
        case 0x20:
            CardType = UNION_PAY;
            break;
        case 0x28:
            CardType = UNIONPAYCPU;
            break;
        default:
            CardType = 0xFF;
        }
    }
    return CardType;
}

//////////////////////////////////////////////////////////////////////
//APDU�����ӿں���
//�ǽӿ�APDU�����ӿ�
//
//���أ�ͨ��״̬��
///////////////////////////////////////////////////////////////////////
unsigned char pro_APDU(unsigned char *sendbuf,unsigned short sendlen,unsigned char *recebuf,unsigned short *recelen)
{
    int status;

    status = ISO14443_4_HalfDuplexExchange(&g_pcd_module_info, sendbuf, sendlen, recebuf, (unsigned int *)recelen);

    //printf("sta= %d\r\n",status);
    return status;
}





