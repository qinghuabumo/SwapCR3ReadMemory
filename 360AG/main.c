#include <ntifs.h>
#include "Comm.h"
UNICODE_STRING symName;
PDEVICE_OBJECT DeviceObject ;

 Data data;
ULONG64 cr3;
ULONG64 OldCr3;
char OldVadRot[0x45];
ULONG64 cr3win10;
ULONG64 OldCr3win10;
ULONG64 OldVadRotwin10;
ULONG64 OldCr3win102;

void PcreateProcessNotifyRoutineWin7(
	HANDLE ParentId,
	HANDLE ProcessId,
	BOOLEAN Create
	);

void Function(PDRIVER_OBJECT pDriver)
{
	
	initialization(pDriver);
	Createcallback();

}
VOID DriverUpload(PDRIVER_OBJECT pDriver)
{
// ��Ϸ��������Ⱥ��301016862
	
	PsSetCreateProcessNotifyRoutine(PcreateProcessNotifyRoutineWin7, 1);
	IoDeleteSymbolicLink(&symName);
	IoDeleteDevice(pDriver->DeviceObject);
	KdPrint(("DriverUpload fuck over\r\n"));
}
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING pReg)
{
// ��Ϸ��������Ⱥ��301016862
	Function( pDriver);

// ��Ϸ��������Ⱥ��301016862
	pDriver->DriverUnload = DriverUpload;
	KdPrint(("���سɹ�\r\n"));
	return STATUS_SUCCESS;
}


