
#ifndef _TEST_H_    //����1
#define _TEST_H_    //����2
//������ֱ�������������
#include <ntifs.h>

#define READCODE CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ALL_ACCESS)
#define BASEDLL CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ALL_ACCESS)
//������
#define DRIVER_NAME L"\\Device\\fuck"
//�豸��
#define SYM_NAME L"\\??\\fuck"
//���ݴ���ṹ
typedef struct DATA
{
	ULONG newpid;//���±�pid
	ULONG primarypid;//��Ϸpid
	ULONG size;//��С
}Data, *pData;
extern Data data ;
extern PDEVICE_OBJECT DeviceObject ;
extern UNICODE_STRING symName ;
extern ULONG64 cr3;
extern ULONG64 OldCr3;
extern char OldVadRot[0x45];
extern ULONG64 cr3win10;
extern ULONG64 OldCr3win10;
extern ULONG64 OldVadRotwin10;
extern ULONG64 OldCr3win102;
//��ʼ���豸
extern ULONG initialization(PDRIVER_OBJECT pDriver);
extern ULONG Createcallback();
extern ULONG ReadMemoryPid(pData pdata, _Inout_ struct _IRP *Irp);
extern ULONG ReadMemoryNameDll( _Inout_ struct _IRP *Irp);
extern ULONG PsResumeProcess(PEPROCESS Process);
extern NTSTATUS Disaptch(
	_In_ struct _DEVICE_OBJECT *DeviceObject,
	_Inout_ struct _IRP *Irp
	);

#endif              //����3