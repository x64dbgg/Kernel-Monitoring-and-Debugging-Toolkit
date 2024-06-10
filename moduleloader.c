#include <ntddk.h>

NTSTATUS LoadKernelModule(
    PCWSTR ModulePath
) {
    UNICODE_STRING usModulePath;
    RtlInitUnicodeString(&usModulePath, ModulePath);
    return ZwLoadDriver(&usModulePath);
}

VOID UnloadDriver(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("ModuleLoader: Unloaded\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = UnloadDriver;
    DbgPrint("ModuleLoader: Loaded\n");
    return STATUS_SUCCESS;
}
