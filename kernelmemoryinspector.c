#include <ntddk.h>

NTSTATUS ReadKernelMemory(
    PVOID SourceAddress,
    PVOID DestinationBuffer,
    SIZE_T Size
) {
    __try {
        ProbeForRead(SourceAddress, Size, sizeof(UCHAR));
        RtlCopyMemory(DestinationBuffer, SourceAddress, Size);
        return STATUS_SUCCESS;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return GetExceptionCode();
    }
}

VOID UnloadDriver(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("KernelMemoryInspector: Unloaded\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = UnloadDriver;
    DbgPrint("KernelMemoryInspector: Loaded\n");
    return STATUS_SUCCESS;
}
