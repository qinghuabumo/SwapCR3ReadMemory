#include "Readingwriting.h"
#include "Comm.h"
//�л�cr3
ULONG Win7Cr3date(PEPROCESS processnew, PEPROCESS processprimary)
{
	//������±�����
	NTSTATUS arr = PsSuspendProcess(processnew);
	cr3 = (ULONG64)processnew + 0x28;
	if (NT_SUCCESS(arr))
	{
		//����ԭ�еı���
		OldCr3 = *(ULONG64*)((ULONG64)processnew + 0x28);
		memcpy(OldVadRot, (void*)((ULONG64)processnew + 0x448), 0x40);
		//���ܽ���
		//cr3
		*(ULONG64*)((ULONG64)processnew + 0x28) = *(ULONG64*)((ULONG64)processprimary + 0x28);
		//����vad
		memcpy((void*)((ULONG64)processnew + 0x448), (void*)((ULONG64)processprimary + 0x448), 0x40);
		return 1;
	}
	return 0;
}
ULONG Win10Cr3date(PEPROCESS processnew, PEPROCESS processprimary)
{
	//������±�����
	NTSTATUS arr = PsSuspendProcess(processnew);
	cr3win10 = (ULONG64)processnew + 0x28;
	if (NT_SUCCESS(arr))
	{
		//����ԭ�еı���
		OldCr3win10 = *(ULONG64*)((ULONG64)processnew + 0x28);
		OldCr3win102 = *(ULONG64*)((ULONG64)processnew + 0x280);
		OldVadRotwin10 = *(ULONG64*)((ULONG64)processnew + 0x658);
		//���ܽ���
		//cr3
		*(ULONG64*)((ULONG64)processnew + 0x28) = *(ULONG64*)((ULONG64)processprimary + 0x28);
		//�����ں˲�cr3
		*(ULONG64*)((ULONG64)processnew + 0x280) = *(ULONG64*)((ULONG64)processprimary + 0x280);
		//vad
		*(ULONG64*)((ULONG64)processnew + 0x658) = *(ULONG64*)((ULONG64)processprimary + 0x658);
		return 1;
	}
	return 0;
}