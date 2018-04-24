#pragma once
//--------------------------------------------------------------
//head files
#include <stdio.h>
#include <string.h>
#include "stdafx.h"
#include "flash_api.h"
#include "FlashUpdateMain.h"
#include "afxdialogex.h"
#include "flash_apiDlg.h"
#include "ControlCAN.h"
//-----------------------------------------------------------------------------
//Macro definition
//sevice code denfine
//����
#define HANDS_COMM_SRVCODE 0x01
//оƬ����
#define CHIP_DECODE_SRVCODE 0x02
//API�汾��Ϣ
#define API_VERSION_SRVCODE 0x03
//��������
#define ERASE_SECTOR_SRVCODE 0x04
//��̿�ʼָ��
#define PROGRAM_SRVCODE 0x05
//����BLOCK�ļ�ͷ��Ϣ
#define BLOCK_HEAD_SRVCODE 0x06
//��ӦBLOCK������Ч����
#define BLOCK_DATA_SRVCODE 0x07
//������ӦBLOCKУ���--�ۼӺ�
#define BLOCK_CHECKSUM_SRVCODE 0x08
//�����ӦBLOCK��program״̬
#define BLOCK_PROMG_STATUS_SRVCODE 0x09
//FLASH У��
#define VERIFY_SRVCODE 0x0A

//-------------------------------------
//�������ȱ�־��غ궨��
//���ȸ�λ״̬
#define	PROGRESS_IN_RESET		0xff
//������ʼ
#define	PROGRESS_IN_START		0
//����OK
#define 	PROGRESS_IN_HAND_OK		1
//����OK
#define	PROGRESS_IN_DECODE_OK	2
//API�汾OK
#define	PROGRESS_IN_APIVERSION_OK	3
//��������OK
#define	PROGRESS_IN_ERASE_OK		4
//�������OK
#define	PROGRESS_IN_PROG_ENA_OK	5
//BLOCKͷ��ϢӦ��OK
#define	PROGRESS_IN_HEADRESPOND_OK	6
//BLOCKУ���Ӧ��OK
#define	PROGRESS_IN_BLOCKCHECK_OK	7
//FLASH ���OK
#define	PROGRESS_IN_FLASH_PROG_OK	8
//FLASH ���У��OK
#define	PROGRESS_IN_FLASH_VERIFY_OK	9

//------------------------------------


//״̬��Ϣ�궨��
//����,�·�
#define HAND_COMM_QUERY 0x62
//�����ϴ�
#define HAND_OK_RESPOND 0xaa

//���ܳɹ�
#define CHIP_DECODE_SUCCESS 0x00
#define CHIP_DECODE_FAIL	   10

//API�汾ƥ������
#define API_VESION_OK	0x6c
#define API_VERSION_FAIL 14




//�����������
//����ѡ��
#define ERASE_SECTOR_ALL 6
#define ERASE_SECTOR_A 1
#define ERASE_SECTOR_B 2
#define ERASE_SECTOR_C 3
#define ERASE_SECTOR_D 4
#define ERASE_SECTOR_BCD 5

//�������������붨��
//�����ɹ�
#define ERASE_SUCCESFULL 0
//δָ������
#define ERASE_NO_SPECIFIED_SECTOR 20
//��0ʧ��
#define ERASE_PRECONDITION_FAIL 21
//��������ʧ��
#define ERASE_FAIL 22
//����ʧ��
#define ERASE_COMPACT_FAIL 23
//Ԥ����ʧ��
#define ERASE_PRECOMPACT_FAIL 24

//PROGRAM
//�������
#define PROGRAM_ENABLE 0x6e
//������δ�������������
#define PROGRAM_DIABLE 0x70

//VERIFY
//У��ɹ�
#define VERIFY_OK	0
//У��ʧ��
#define VERIFY_FAIL 10

//���ݴ���
//BLOCK����ͷ��ȷ
#define BLOCK_HEAD_OK	0
//BLOCKͷ��ȷ���ղ��ļ��������
#define FILE_TRANS_END 5
//BLOCK SIZE >1024
#define BLOCK_SIZE_OVERLOW 10
//BLOCK ADD OVERLOW
#define BLOCK_ADD_OVERLOW 20
//BLOCK head and add are overlow
#define BLOCK_SEZE_ADD_OVERLOW	30

//one block check sum
#define CHECK_SUM_SUCCESFUL	0x82
#define CHECK_SUM_FAIL			0x83


//dsp �������յ�����
#define BLOCK_RECV_DATA_OK 0


//-----20100118:���Ӵ�����ͬ�������������Ĵ����������Ͷ���
//Ŀ������ʹ�����
#define TARGET_UPDATE_ENABLE 1
#define TARGET_UPDATE_DISABLE 0

//���������б���������������
#define TASK_HANDLED 1
#define TASK_HANDLE_NON 0


//�����򳤶�
#define CAN_DATA_FIELD_SIZE 8

//���ݳ���
#define CAN_NON_FRAG_DATA_LEN 6
#define CAN_FRAG_DATA_LEN		500


#define FRAG_MSG 1
#define NONFRAG_MSG 0

#define RS_MSG 1
#define RQ_MSG 0

typedef struct
{
	//������Ŀ���ַ
	UCHAR ucTargeAddr;
	//��Ŀ���ַ����ʹ��=1ʹ��
	UCHAR ucTargetEnable;
	//���������б������������=1�������
	UCHAR ucTaskHandled;
}_FLASHUPDATE_TARGET_T;


enum TIMER_OPTION
{
	TIMER_ONE_SHOT = 0, 	//ֻ����һ��
	TIMER_REPEAT = 1		//�ظ�ִ��
};


//CAN��Ϣ���ö��
enum CAN_MSG_CLASS_ENUM
{
	CAN_RESERVED_CLASS = 0,
	CAN_CFG_MSG_CLASS = 1,
	CAN_CTRL_MSG_CLASS = 2,
	CAN_ALRM_MSG_CLASS = 3,
	CAN_STATE_MSG_CLASS = 4,
	CAN_ANALOG_MSG_CLASS = 5,
	CAN_MSG_CLASS_EOL
};


enum CAN_MSG_HANGLE_ERR_ENUM
{
	CAN_MSG_HANDLE_OK = 0,
	CAN_MSG_HANDLE_INVALID_MAC_ID,
	CAN_MSG_HANDLE_INVALID_BATT_GRP,
	CAN_MSG_HANDLE_INVALID_OBJ,
	CAN_MSG_HANDLE_INVALID_MSG_CLASS,
	CAN_MSG_HANDLE_INVALID_SRVC_COD,


	CAN_MSG_HANDLE_EOL
};
// CAN ID allocated
enum _CAN_MAC_ID_ENUM
{
	MAC_ID_RSVDX00 = 0,
	MAC_ID_MON = 1,
	MAC_ID_RSVDX02,
	MAC_ID_RSVDX03,
	MAC_ID_RSVDX04,
	MAC_ID_RSVDX05,
	MAC_ID_RSVDX06,
	MAC_ID_RSVDX07,
	MAC_ID_RSVDX08,
	MAC_ID_RSVDX09,
	MAC_ID_RSVDX0A,
	MAC_ID_RSVDX0B,
	MAC_ID_RSVDX0C,
	MAC_ID_RSVDX0D,
	MAC_ID_RSVDX0E,
	MAC_ID_BYP = 0x0F,
	MAC_ID_MOD1_INV = 0x10,
	MAC_ID_MOD2_INV = 0x11,
	MAC_ID_MOD3_INV = 0x12,
	MAC_ID_MOD4_INV = 0x13,
	MAC_ID_MOD5_INV = 0x14,
	MAC_ID_MOD6_INV = 0x15,
	MAC_ID_MOD7_INV = 0x16,
	MAC_ID_MOD8_INV = 0x17,
	MAC_ID_MOD9_INV = 0x18,
	MAC_ID_MOD10_INV = 0x19,
	MAC_ID_RSVDX1A,
	MAC_ID_RSVDX1B,
	MAC_ID_RSVDX1C,
	MAC_ID_RSVDX1D,
	MAC_ID_RSVDX1E,
	MAC_ID_RSVDX1F,
	MAC_ID_MOD1_REC = 0x20,
	MAC_ID_MOD2_REC = 0x21,
	MAC_ID_MOD3_REC = 0x22,
	MAC_ID_MOD4_REC = 0x23,
	MAC_ID_MOD5_REC = 0x24,
	MAC_ID_MOD6_REC = 0x25,
	MAC_ID_MOD7_REC = 0x26,
	MAC_ID_MOD8_REC = 0x27,
	MAC_ID_MOD9_REC = 0x28,
	MAC_ID_MOD10_REC = 0x29,
	MAC_ID_EOL
};



typedef enum
{
	//��ʼ��Ч״̬
	STATUS_FLASH_UPDATE_INVALID = 0,
	//��ʼѡ��ڵ�����
	STATUS_FLASH_START,
	//��ָ���ڵ㿪ʼ��������
	STATUS_SELCET_NODE,

	//�ȴ������ź�״̬
	STATUS_WAITING_HANDS_RESPOND,
	//�ȴ�оƬ����Ӧ���ź�
	STATUS_WAITING_CHIP_DECODE,
	//�ȴ�API�汾ȷ����Ϣ
	STATUS_WAITING_API_VERSION,
	//API ok
	STATUS_API_OK,

	//������״̬
	STATUS_FLASH_ERASE_GOING,
	//��������
	STATUS_FLASH_ERASED,
	//�ȴ��������״̬
	STATUS_PROGRAM_PERMIT_WAITING,
	//�������
	STATUS_PROGRAM_ENABLE,
	//����BLOCKͷ
	STATUS_BLOCK_HEAD_WAITING,
	//����BLOCKͷ����
	STATUS_BLOCK_HEAD_OK,

	//������һ��BLOCK
	STATUS_NEXT_BLOCK_HEAD,

	//�ȴ���������BLOCK����
	STATUS_BLOCK_DATATRANS_WAITING,
	//BLOCK���ݴ������
	STATUS_BLOCK_DATATRANS_END,
	//�ȴ�BLOCKУ���Ӧ��
	STATUS_BLOCK_CHECKSUM_WAITING,
	//BLOCKУ��OK
	STATUS_BLOCK_CHECKSUM_OK,
	//���״̬�ȴ�
	STATUS_BLOCK_PROGRAM_WAITING,
	//BLOCK������
	STATUS_BLOCK_PROGRAM_COMPLETE,
	//�ļ��������
	STATUS_FILE_TRANS_END,
	//FLASH У����
	STATUS_FLASH_VERIFYING,
	//FLASH У�����
	STATUS_FLASH_VERIFY,
	//FLASH UPDATE ���
	STATUS_FLASH_UPDATE_OVER


}_FLASHUPDATE_STATUS;



//��ʼ״̬
#define	STATUS_HOST_UPDATE_INI  0

//��δ��ʼ����
#define	STATUS_HOST_HAVENOT_START 1

//����������
#define	STATUS_HOST_UPDAT_ING 2

//�����ɹ�
#define	STATUS_HOST_UPDATE_SUCCESFULL 3

//����ʧ��
#define	STATUS_HOST_UPDATE_FAIL 4

//ָ�������ڵ㲻����
#define	STATUS_UPDATE_NODE_OFF_LINE 5

typedef struct
{

	//FlashUpdate��־
	UCHAR sFlashUpdateFlag;//������������·

						   //��������̨������״̬
						   //[0]--BYPASS,1~10 Module,11--Monitor
						   //	_FLASHUPDATE_HOST_STATUS u16UpdateStatus[12];
	UINT16 u16UpdateStatus[12];


	//��������ģ���
	UINT16 u16ModIdx;

	//�Ƿ���Ҫ������Ӧkernel
	//=0x95��ʾ������kernel,����������kernel
	UINT16 u16FlashUpdateKernelFlag;

	//Flash update ״̬��
	_FLASHUPDATE_STATUS u16FlashupdateStatus;



	//.....
}_HOST_MODULE_ITC_T;




typedef union CAN_PACKED_PROTOCOL_STRUCT
{
	VCI_CAN_OBJ Frame;
	struct
	{
		UINT16	b6SourceMacId : 6;
		UINT16	b10MsgClass : 4;
		UINT16	NotUsed1 : 6;
		UINT16	NotUsed2;
		UINT	TimeStamp;
		BYTE	TimeFlag;
		BYTE	SendType;
		BYTE	RemoteFlag;//�Ƿ���Զ��֡
		BYTE	ExternFlag;//�Ƿ�����չ֡
		BYTE	DataLen;				//Data field all len,bytes
		BYTE	b6DestinationMacId : 6;
		BYTE	b1NotUsed : 1;
		BYTE	b1Fragment : 1;
		BYTE	b7ServiceCode : 7;
		BYTE	b1RsRq : 1;
		BYTE	MsgData[6];
		BYTE	Reserved[3];
	}PackedMsg;
}CAN_PACKED_PROTOCOL_U;


class CAN_FlashupdateMsgHandle //:public FlashUpdateMain
{
public:
	CAN_FlashupdateMsgHandle(VOID);
	virtual ~CAN_FlashupdateMsgHandle(VOID);

	UCHAR GetMsgClass(VOID);


	VOID FlashUpdateRoutine(VOID);
	UCHAR FlashupdateNodeGet(VOID);
	//---------------------------------
	INT32 HandCommXmitFcb(VOID);
	INT32 HandCommRecvChipDecodeXmit(VOID);
	INT32 ChipDecodeXmitFcb(VOID);
	INT32 ChipDecodeRecvFcb(VOID);
	INT32 ApiVersionXmitFcb(VOID);
	INT32 ApiVersionRecvFcb(VOID);
	INT32 EraseSectorXmitFcb(VOID);
	INT32 EraseSectorRecvFcb(VOID);
	INT32 ProgramXmitFcb(VOID);
	INT32 ProgramRecvFcb(VOID);
	INT32 BlockHeadXmitFcb(VOID);
	INT32 BlockHeadRecvFcb(VOID);
	INT32 BlockDataXmitFcb(VOID);
	INT32 BlockDataRecvFcb(VOID);
	INT32 BlockChecksumXmitFcb(VOID);
	INT32 BlockChecksumRecvFcb(VOID);
	INT32 BlockProgStatusXmitFcb(VOID);
	INT32 BlockProgStatusRecvFcb(VOID);
	INT32 VerifyXmitFcb(VOID);
	INT32 VerifyRecvFcb(VOID);

	VOID FlashupdateTaskReset(VOID);
	UCHAR FlashupdateTaskHandle(UCHAR ucRecvAddr);

	VOID ResetFlsUpdateMoudele(VOID);

	

private:



	//��ǰ���������Ľڵ��ַ
	UINT16 m_u16UpdaingNodeAdd;

	//��ǰ������ģ���
	UINT16 m_u16UpdatingModuleNo;

	//BLOCK�ļ�У���
	UINT16 m_u16BlockChecksum;

	//�ļ��������
	//ÿһ��SECTION �ֳ�N��BLOCK
	UINT16	m_u16BlockNumOfSection;
	//N��BLOCK�е�ǰBLOCK��
	UINT16	m_u16CurrentBlockNum;
	//��ǰSECTION�в���1024��(һ��BLOCK),ʣ������
	UINT16 	m_u16RemainWordOfSection;
	//ʣ��BLOCK(����1024��)���õ�CAN֡��
	UINT16	m_u16FrameNumOfRemainBlock;
	//ʣ��BLOCK(����1024��)����֡�����һ֡���ֵĸ���
	UINT16	m_u16LastFrameWordNumOfRemainBlock;
	//��ӦSECTION�ĳ���
	WORD m_wSectionLen;
	//��ӦSECTION��ַ
	UINT m_uSectionAddress;

	//�ܵ�֡��-1
	UINT16 m_u16FrameNumOneBlock;
	//���һ֡���ֵĸ���
	UINT16 m_u16LastFrameWordNumOneBlock;
	//�������֡��
	UINT16 m_u16CurrentFrameNum;
	//�������BLOCK��
	UINT16 m_u16BlockNum;

	//����BLOCK��У���
	UINT32 m_u32CheckSumOneBlock;

	//�������������BLOCK�еĵ�ַ
	DWORD m_ulPos;

	//	BYTE m_byBuff[1024*96];
	//	BYTE m_ucSectionBuff[1024*48];
	//	BYTE m_byBuff[1024*360];
	BYTE m_ucSectionBuff[1024 * 300];

	//��ӦBLOCK��ַ
	UINT m_uBlockAddress;

	//��ӦBLOCK�ĳ���
	WORD m_wBlockLen;

	//BLOCK�ط�����
	UINT16 m_u16ResendCnt;



	//������Ŀ�����
	//=0,��ʾ������·��=1~10����ģ��
	_FLASHUPDATE_TARGET_T m_tFlashupdateTarged[11];

	//���ݺ�ָ̨��������ģ�鿴�Ƿ�������Ӧ���ź�
	//ȡֵΪ...
	UINT16	m_u16RespondModuleFlag;


	//��ȡHEX�ļ�������־
	//��ֵ=0x6789 ��ʾ����
	UINT16 	m_u16ReadHexFileEnd;

	

	UCHAR m_ucMsgClass;


	int device_type = 4;	// CANalyst-II
	int device_ind = 0;		// first device
	int can_ind = 0;		// CAN channel 0
	CAN_PACKED_PROTOCOL_U	*tx_msg;
	CAN_PACKED_PROTOCOL_U	*rx_msg;

	
	UINT32	BlockMessageProcess_Packaged(void);

	UINT16  BlockCount;

	public:
	UINT16	BlockAmount;
	UINT16	EveryBlockDataNum[500];
	UINT16	BlockData[500][1024];
	UINT32	BlockCheckSum[500];
	UINT32	BlockAddress[500];


	// 0x01: moninter	0x0F:BYP	0x10 - 0x1F: REC    0x20 - 0x2F  INV
	char	FlashUpdateProgress[0x3F];		
	UINT16  FlashUpdateErrorCnt[0x3F];
	_HOST_MODULE_ITC_T *m_pHostModuleItc;
};