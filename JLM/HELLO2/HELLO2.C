/*
 * Another JLM "hello world", this time written in C.
 * Designed for Microsoft Visual C++ and standard-compliant C17 compilers.
 */

#include <jlm.h>

struct Client_Reg_Struc *pcl;

struct cb_s *Get_Cur_VM_Handle(void)
{
    VxDCall(Get_Cur_VM_Handle);
    __asm mov eax, ebx
}

ULONG Begin_Nest_Exec(void)
{
    __asm mov ebp, pcl
    VxDCall(Begin_Nest_Exec);
}

ULONG End_Nest_Exec(void)
{
    __asm mov ebp, pcl
    VxDCall(End_Nest_Exec);
}

ULONG Exec_Int(unsigned long intno)
{
    __asm mov eax, intno
    __asm mov ebp, pcl
    VxDCall(Exec_Int);
}

int main(void)
{
    struct cb_s *hVM;
    const char *psz = "Hello world\r\n";

    hVM = Get_Cur_VM_Handle();
    pcl = (struct Client_Reg_Struc *)hVM->CB_Client_Pointer;
    Begin_Nest_Exec();

    for (; *psz; psz++) {
        pcl->Client_EAX = 0x0200;
        pcl->Client_EDX = *psz;
        Exec_Int(0x21);
    }

    End_Nest_Exec();

    return 1;
}
