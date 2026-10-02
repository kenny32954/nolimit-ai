.text

.global _Serial_Open
.global _Serial_Close
.global _Serial_IsOpen
.global _Serial_ReadSingle
.global _Serial_Write
.global _Serial_PollRX
.global _Serial_PollTX
.global _Serial_ClearRX
.global _Serial_ClearTX

#define syscall_(name, id) \
name: ;\
    mov.l syscall_table, r2 ;\
    mov.l 1f, r0 ;\
    jmp @r2 ;\
    nop ;\
    .align 4 ;\
1:  .long id

syscall_(_Serial_Open,      0x1BB7)
syscall_(_Serial_Close,     0x1BB8)
syscall_(_Serial_ReadSingle,0x1BB9)
syscall_(_Serial_Write,     0x1BBE)
syscall_(_Serial_PollRX,    0x1BBF)
syscall_(_Serial_PollTX,    0x1BC0)
syscall_(_Serial_ClearRX,   0x1BC1)
syscall_(_Serial_ClearTX,   0x1BC2)
syscall_(_Serial_IsOpen,    0x1BC6)

.align 4
syscall_table:
    .long 0x80020070
