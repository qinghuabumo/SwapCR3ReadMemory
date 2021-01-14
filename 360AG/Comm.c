#include "Comm.h"
#include "tools.h"
#include "Readingwriting.h"
#include "FacilitateDll.h"
Data data = {0};
ULONG64 cr3=0;
ULONG64 OldCr3=0;
char OldVadRot[0x45] = {0};
ULONG64 cr3win10=0;
ULONG64 OldCr3win10=0;
ULONG64 OldVadRotwin10=0;
ULONG64 OldCr3win102=0;
UNICODE_STRING symName;
PDEVICE_OBJECT DeviceObject;

//���̹رջص�win7
void PcreateProcessNotifyRoutineWin7(
	HANDLE ParentId,
	HANDLE ProcessId,
	BOOLEAN Create
	)
{

	if (Create == 0)
	{

		if (data.primarypid == ProcessId)
		{
			PEPROCESS processnew = NULL;
			//��ȡ���±�process
			PsLookupProcessByProcessId((HANDLE)data.newpid, &processnew);
			//cr3
			*(ULONG64*)((ULONG64)processnew + 0x28) = OldCr3;
			//����vad
			memcpy((void*)((ULONG64)processnew + 0x448), OldVadRot, 0x40);
			KernelSleep(1000, 0);
			PsResumeProcess(processnew);
		}

	}
}
//���̹رջص�win10
void PcreateProcessNotifyRoutineWin10(
	HANDLE ParentId,
	HANDLE ProcessId,
	BOOLEAN Create
	)
{

	if (Create == 0)
	{

		if (data.primarypid == ProcessId)
		{
			PEPROCESS processnew = NULL;
			PEPROCESS processprimary = NULL;
			//��ȡ��Ϸprocess
			PsLookupProcessByProcessId((HANDLE)data.primarypid, &processprimary);
			//��ȡ���±�process
			PsLookupProcessByProcessId((HANDLE)data.newpid, &processnew);
			//cr3
			*(ULONG64*)((ULONG64)processnew + 0x28) = OldCr3win10;
			//cr3�ں�
			*(ULONG64*)((ULONG64)processnew + 0x280) = OldCr3win102;
			//����vad
			*(ULONG64*)((ULONG64)processnew + 0x658) = OldVadRotwin10;
			PsResumeProcess(processnew);
		
		}

	}
}
//�����ص�
ULONG Createcallback()
{
	ULONG ddk = windowsDDK();
	if (ddk == 6)
	{
		NTSTATUS routine = PsSetCreateProcessNotifyRoutine(PcreateProcessNotifyRoutineWin7, 0);
		if (!NT_SUCCESS(routine))
		{
			KdPrint(("PsSetCreateProcessNotifyRoutine\r\n"));
		}
	}
	if (ddk == 10)
	{
		NTSTATUS routine = PsSetCreateProcessNotifyRoutine(PcreateProcessNotifyRoutineWin10, 0);
		if (!NT_SUCCESS(routine))
		{
			KdPrint(("PsSetCreateProcessNotifyRoutine\r\n"));
		}
	}
}
//�����豸
ULONG initialization(PDRIVER_OBJECT pDriver)
{
	

	//������
	UNICODE_STRING deviceName = { 0 };
	//�豸��
	
	RtlInitUnicodeString(&deviceName, DRIVER_NAME);
	DeviceObject = NULL;
	//�����豸����
	NTSTATUS status = IoCreateDevice(pDriver, NULL, &deviceName, FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	
	if (!NT_SUCCESS(status))
	{
		KdPrint(("��������ʧ��\r\n"));
		return 0;
	}
	RtlInitUnicodeString(&symName, SYM_NAME);
	//�������豸����
	status = IoCreateSymbolicLink(&symName, &deviceName);
	if (!NT_SUCCESS(status))
	{
		IoDeleteDevice(DeviceObject);
		KdPrint(("��������ʧ��\n"));
		return 0;
	}
	DeviceObject->Flags |= DO_BUFFERED_IO;
	//��һ��
	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		//�����豸����
		pDriver->MajorFunction[i] = Disaptch;
	}
	//�豸��д
	pDriver->MajorFunction[IRP_MJ_CREATE] = Disaptch;
	pDriver->MajorFunction[IRP_MJ_CLOSE] = Disaptch;
}
//ͨ�ź���
NTSTATUS Disaptch(
	_In_ struct _DEVICE_OBJECT *DeviceObject,
	_Inout_ struct _IRP *Irp
	)
{
	PIO_STACK_LOCATION stack;

	stack = IoGetCurrentIrpStackLocation(Irp);

	switch (stack->MajorFunction)
	{

	case IRP_MJ_CREATE:
	{
		DbgPrint("�豸�Ѵ�");
		break;
	}

	case IRP_MJ_CLOSE:
	{
		DbgPrint("�豸�ѹر�");
		break;
	}
	case IRP_MJ_DEVICE_CONTROL:
	{

		switch (stack->Parameters.DeviceIoControl.IoControlCode)
		{
		case READCODE:
		{
			
			//��ȡpid
			ReadMemoryPid(&data, Irp);
		      
			break;
		}
		case BASEDLL:
			//��ȡdll��ַ
			ReadMemoryNameDll(Irp);
			break;
		}
		break;
	}


	}

	return 1;
}

//��pid����
ULONG ReadMemoryPid(pData pdata, _Inout_ struct _IRP *Irp)
{
	PEPROCESS processnew= NULL;
	PEPROCESS processprimary = NULL;
	//��������
	memcpy(pdata, Irp->AssociatedIrp.SystemBuffer, sizeof(Data));
	//��ȡ��Ϸprocess
	PsLookupProcessByProcessId((HANDLE)pdata->primarypid ,&processprimary);
	//��ȡ���±�process
	PsLookupProcessByProcessId((HANDLE)pdata->newpid, &processnew);
	//�ж��Ƿ�win7-win10
	ULONG ddk = windowsDDK();
	//win7
	if (ddk==6)
	{
		Win7Cr3date(processnew, processprimary);
	}
	//win10
	if (ddk==10)
	{
	
		Win10Cr3date(processnew, processprimary);
	}
	// ָʾ��ɴ�IRP
	Irp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return 1;
}
ULONG ReadMemoryNameDll( _Inout_ struct _IRP *Irp)
{
	
	PEPROCESS processprimary = NULL;
	//��ȡ��Ϸprocess
	PsLookupProcessByProcessId(data.primarypid, &processprimary);
	wchar_t arr[20] = {0};
	memcpy(arr, Irp->AssociatedIrp.SystemBuffer, sizeof(arr));
	ULONG64 dllbese =ObtainDll(processprimary, arr);
	if (dllbese==0)
	{
		KdPrint(("��ȡģ���ַʧ��\r\n"));
	}
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = sizeof(ULONG64);
	memcpy(Irp->AssociatedIrp.SystemBuffer, &dllbese, sizeof(ULONG64));
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return 1;
}