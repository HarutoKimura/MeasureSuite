// fiat_curve25519_carry_square
#include "helper.h"
#include <measuresuite.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char symbol[] = {"fiat_curve25519_carry_mul"};
const int arg_width = 5;
const int arg_num_in = 2;
// those are the bounds from the JSON file
const uint64_t bounds[] = {
    0x18000000000000 - 1, 0x18000000000000 - 1, 0x18000000000000 - 1,
    0x18000000000000 - 1, 0x18000000000000 - 1,
};

int main() {
  SIGILL_SETUP();

  char fa[] = {
      "SECTION .text\n"
      "	GLOBAL mul_curve25519\n"
      "mul_curve25519:\n"
      "sub rsp, 0x90 ; last 0x30 (6) for Caller - save regs \n"
      "mov [ rsp + 0x60 ], rbx; saving to stack\n"
      "mov [ rsp + 0x68 ], rbp; saving to stack\n"
      "mov [ rsp + 0x70 ], r12; saving to stack\n"
      "mov [ rsp + 0x78 ], r13; saving to stack\n"
      "mov [ rsp + 0x80 ], r14; saving to stack\n"
      "mov [ rsp + 0x88 ], r15; saving to stack\n"
      "mov r8, rdx; preserving value of arg2 into a new reg\n"
      "mov rdx, [ rsi + 0x0 ]; saving arg1[0] in rdx.\n"
      "mulx r9, rax, [ r8 + 0x0 ]; x50, x49<- arg1[0] * arg2[0]\n"
      "imul r10, [ r8 + 0x20 ], 0x13; x10000 <- arg2[4] * 0x13\n"
      "mov rdx, [ rsi + 0x8 ]; arg1[1] to rdx\n"
      "mulx rcx, rbx, r10; x20, x19<- arg1[1] * x10000\n"
      "imul r15, [ r8 + 0x18 ], 0x13; x10001 <- arg2[3] * 0x13\n"
      "mov rdx, [ rsi + 0x10 ]; arg1[2] to rdx\n"
      "mulx r11, r13, r15; x18, x17<- arg1[2] * x10001\n"
      "imul r12, [ r8 + 0x10 ], 0x13; x10002 <- arg2[2] * 0x13\n"
      "mov rdx, r12; x10002 to rdx\n"
      "mulx rbp, r12, [ rsi + 0x18 ]; x14, x13<- arg1[3] * x10002\n"
      "imul r14, [ r8 + 0x8 ], 0x13; x10003 <- arg2[1] * 0x13\n"
      "mov [ rsp + 0x0 ], r8; spilling arg2 to mem\n"
      "mov r8, rdx; preserving value of x10002 into a new reg\n"
      "mov rdx, [ rsi + 0x20 ]; saving arg1[4] in rdx.\n"
      "mov [ rsp + 0x8 ], r15; spilling x10001 to mem\n"
      "mulx r14, r15, r14; x8, x7<- arg1[4] * x10003\n"
      "mov [ rsp + 0x10 ], rsi; spilling arg1 to mem\n"
      "xor rsi, rsi\n"
      "adox r15, r12\n"
      "adcx r15, r13\n"
      "setc r13b; spill CF x56 to reg (r13)\n"
      "clc;\n"
      "adcx r15, rbx\n"
      "adox rbp, r14\n"
      "mov rbx, -0x2 ; moving imm to reg\n"
      "inc rbx; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r15, rax\n"
      "movzx r13, r13b\n"
      "lea r11, [ r11 + rbp ]\n"
      "lea r11, [ r11 + r13 ]\n"
      "adcx rcx, r11\n"
      "adox r9, rcx\n"
      "mov rbx, r15; x67, copying x63 here, cause x63 is needed in a reg for "
      "other than x67, namely all: , x67, x68, size: 2\n"
      "shrd rbx, r9, 51; x67 <- x65||x63 >> 51\n"
      "mov rax, [ rsp + 0x10 ]; load m64 arg1 to register64\n"
      "mov r12, [ rsp + 0x0 ]; load m64 arg2 to register64\n"
      "mov rdx, [ rax + 0x0 ]; arg1[0] to rdx\n"
      "mulx r14, r13, [ r12 + 0x8 ]; x48, x47<- arg1[0] * arg2[1]\n"
      "mov rdx, [ rax + 0x10 ]; arg1[2] to rdx\n"
      "mulx rbp, r11, r10; x16, x15<- arg1[2] * x10000\n"
      "mov rdx, [ r12 + 0x0 ]; arg2[0] to rdx\n"
      "mulx rcx, r9, [ rax + 0x8 ]; x40, x39<- arg1[1] * arg2[0]\n"
      "mov rdx, [ rax + 0x18 ]; arg1[3] to rdx\n"
      "mov [ rsp + 0x0 ], r12; spilling arg2 to mem\n"
      "mulx rsi, r12, [ rsp + 0x8 ]; x12, x11<- arg1[3] * x10001\n"
      "mov rdx, [ rax + 0x20 ]; arg1[4] to rdx\n"
      "mov [ rsp + 0x18 ], r11; spilling x15 to mem\n"
      "mulx r8, r11, r8; x6, x5<- arg1[4] * x10002\n"
      "add r11, r12; could be done better, if r0 has been u8 as well\n"
      "mov r12, -0x2 ; moving imm to reg\n"
      "inc r12; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r11, [ rsp + 0x18 ]\n"
      "seto r12b; spill OF x122 to reg (r12)\n"
      "mov [ rsp + 0x20 ], r8; spilling x6 to mem\n"
      "mov r8, -0x2 ; moving imm to reg\n"
      "inc r8; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r11, r9\n"
      "setc r8b; spill CF x118 to reg (r8)\n"
      "clc;\n"
      "adcx r11, r13\n"
      "seto r13b; spill OF x126 to reg (r13)\n"
      "mov r9, -0x2 ; moving imm to reg\n"
      "inc r9; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r11, rbx\n"
      "movzx r8, r8b\n"
      "lea rsi, [ r8 + rsi ]\n"
      "mov r8, [ rsp + 0x20 ]\n"
      "lea rsi, [r8+rsi]\n"
      "movzx r12, r12b\n"
      "lea rbp, [ rbp + rsi ]\n"
      "lea rbp, [ rbp + r12 ]\n"
      "movzx r13, r13b\n"
      "lea rcx, [ rcx + rbp ]\n"
      "lea rcx, [ rcx + r13 ]\n"
      "adcx r14, rcx\n"
      "mov r9, 0x0 ; moving imm to reg\n"
      "adox r14, r9\n"
      "mov rbx, [ rsp + 0x0 ]; load m64 arg2 to register64\n"
      "mov rdx, [ rbx + 0x10 ]; arg2[2] to rdx\n"
      "mulx r8, r12, [ rax + 0x0 ]; x46, x45<- arg1[0] * arg2[2]\n"
      "mov r13, r11; x136, copying x133 here, cause x133 is needed in a reg "
      "for "
      "other than x136, namely all: , x136, x137, size: 2\n"
      "shrd r13, r14, 51; x136 <- x135||x133 >> 51\n"
      "mov rdx, [ rbx + 0x8 ]; arg2[1] to rdx\n"
      "mulx rsi, rbp, [ rax + 0x8 ]; x38, x37<- arg1[1] * arg2[1]\n"
      "mov rdx, [ rax + 0x10 ]; arg1[2] to rdx\n"
      "mulx rcx, r14, [ rbx + 0x0 ]; x32, x31<- arg1[2] * arg2[0]\n"
      "mov rdx, [ rsp + 0x8 ]; x10001 to rdx\n"
      "mulx rdx, r9, [ rax + 0x20 ]; x4, x3<- arg1[4] * x10001\n"
      "mov [ rsp + 0x0 ], rbx; spilling arg2 to mem\n"
      "mov rbx, rdx; preserving value of x4 into a new reg\n"
      "mov rdx, [ rax + 0x18 ]; saving arg1[3] in rdx.\n"
      "mov [ rsp + 0x28 ], r9; spilling x3 to mem\n"
      "mov [ rsp + 0x30 ], r11; spilling x133 to mem\n"
      "mulx r9, r11, r10; x10, x9<- arg1[3] * x10000\n"
      "test al, al\n"
      "adox r11, [ rsp + 0x28 ]\n"
      "adcx r11, r14\n"
      "setc r14b; spill CF x106 to reg (r14)\n"
      "clc;\n"
      "adcx r11, rbp\n"
      "setc bpl; spill CF x110 to reg (rbp)\n"
      "clc;\n"
      "adcx r11, r12\n"
      "setc r12b; spill CF x114 to reg (r12)\n"
      "clc;\n"
      "adcx r11, r13\n"
      "adox r9, rbx\n"
      "movzx r14, r14b\n"
      "lea rcx, [ rcx + r9 ]\n"
      "lea rcx, [ rcx + r14 ]\n"
      "movzx rbp, bpl\n"
      "lea rsi, [ rsi + rcx ]\n"
      "lea rsi, [ rsi + rbp ]\n"
      "movzx r12, r12b\n"
      "lea r8, [ r8 + rsi ]\n"
      "lea r8, [ r8 + r12 ]\n"
      "adc r8, 0x0\n"
      "mov r13, [ rsp + 0x0 ]; load m64 arg2 to register64\n"
      "mov rdx, [ rax + 0x8 ]; arg1[1] to rdx\n"
      "mulx rbx, r14, [ r13 + 0x10 ]; x36, x35<- arg1[1] * arg2[2]\n"
      "mov rbp, r11; x141, copying x138 here, cause x138 is needed in a reg "
      "for "
      "other than x141, namely all: , x141, x142, size: 2\n"
      "shrd rbp, r8, 51; x141 <- x140||x138 >> 51\n"
      "mov rdx, [ rax + 0x0 ]; arg1[0] to rdx\n"
      "mulx r12, r9, [ r13 + 0x18 ]; x44, x43<- arg1[0] * arg2[3]\n"
      "mov rdx, [ rax + 0x10 ]; arg1[2] to rdx\n"
      "mulx rcx, rsi, [ r13 + 0x8 ]; x30, x29<- arg1[2] * arg2[1]\n"
      "mov rdx, [ r13 + 0x0 ]; arg2[0] to rdx\n"
      "mov [ rsp + 0x38 ], r9; spilling x43 to mem\n"
      "mulx r8, r9, [ rax + 0x18 ]; x26, x25<- arg1[3] * arg2[0]\n"
      "mov rdx, r10; x10000 to rdx\n"
      "mulx rdx, r10, [ rax + 0x20 ]; x2, x1<- arg1[4] * x10000\n"
      "add r10, r9; could be done better, if r0 has been u8 as well\n"
      "mov r9, -0x2 ; moving imm to reg\n"
      "inc r9; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r10, rsi\n"
      "seto r9b; spill OF x90 to reg (r9)\n"
      "mov rsi, -0x2 ; moving imm to reg\n"
      "inc rsi; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r10, r14\n"
      "seto r14b; spill OF x94 to reg (r14)\n"
      "mov [ rsp + 0x40 ], r11; spilling x138 to mem\n"
      "mov r11, -0x2 ; moving imm to reg\n"
      "inc r11; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r10, [ rsp + 0x38 ]\n"
      "setc r11b; spill CF x86 to reg (r11)\n"
      "clc;\n"
      "adcx r10, rbp\n"
      "mov rbp, 0x7ffffffffffff ; moving imm to reg\n"
      "mov [ rsp + 0x0 ], r13; spilling arg2 to mem\n"
      "setc r13b; spill CF x144 to reg (r13)\n"
      "mov [ rsp + 0x48 ], rdx; spilling x2 to mem\n"
      "seto dl; spill OF x98 to reg (rdx)\n"
      "mov [ rsp + 0x10 ], rax; spilling arg1 to mem\n"
      "mov rax, r10; x147, copying x143 here, cause x143 is needed in a reg "
      "for "
      "other than x147, namely all: , x147, x146, size: 2\n"
      "and rax, rbp; x147 <- x143& 0x7ffffffffffff\n"
      "mov [ rdi + 0x18 ], rax; out1[3] = x147\n"
      "and r15, rbp; x68 <- x63& 0x7ffffffffffff\n"
      "sar  r11b, 1\n"
      "adcx r8, [ rsp + 0x48 ]\n"
      "sar  r9b, 1\n"
      "adcx rcx, r8\n"
      "sar  r14b, 1\n"
      "adcx rbx, rcx\n"
      "sar  dl, 1\n"
      "adcx r12, rbx\n"
      "movzx r11, r13b; x145, copying x144 here, cause x144 is needed in a reg "
      "for other than x145, namely all: , x145, size: 1\n"
      "lea r11, [ r11 + r12 ]\n"
      "shrd r10, r11, 51; x146 <- x145||x143 >> 51\n"
      "mov r9, [ rsp + 0x10 ]; load m64 arg1 to register64\n"
      "mov r14, [ rsp + 0x0 ]; load m64 arg2 to register64\n"
      "mov rdx, [ r9 + 0x0 ]; arg1[0] to rdx\n"
      "mulx r13, rax, [ r14 + 0x20 ]; x42, x41<- arg1[0] * arg2[4]\n"
      "mov rdx, [ r9 + 0x8 ]; arg1[1] to rdx\n"
      "mulx r8, rcx, [ r14 + 0x18 ]; x34, x33<- arg1[1] * arg2[3]\n"
      "mov rdx, [ r9 + 0x10 ]; arg1[2] to rdx\n"
      "mulx rbx, r12, [ r14 + 0x10 ]; x28, x27<- arg1[2] * arg2[2]\n"
      "mov rdx, [ r14 + 0x0 ]; arg2[0] to rdx\n"
      "mulx r11, rbp, [ r9 + 0x20 ]; x22, x21<- arg1[4] * arg2[0]\n"
      "mov rdx, [ r14 + 0x8 ]; arg2[1] to rdx\n"
      "mov [ rsp + 0x50 ], r10; spilling x146 to mem\n"
      "mov [ rsp + 0x58 ], r13; spilling x42 to mem\n"
      "mulx r10, r13, [ r9 + 0x18 ]; x24, x23<- arg1[3] * arg2[1]\n"
      "mov [ rsp + 0x0 ], r14; spilling arg2 to mem\n"
      "xor r14, r14\n"
      "adox rbp, r13\n"
      "adcx rbp, r12\n"
      "seto r12b; spill OF x70 to reg (r12)\n"
      "mov r13, -0x2 ; moving imm to reg\n"
      "inc r13; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rbp, rcx\n"
      "seto r13b; spill OF x78 to reg (r13)\n"
      "mov rcx, -0x2 ; moving imm to reg\n"
      "inc rcx; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rbp, rax\n"
      "seto cl; spill OF x82 to reg (rcx)\n"
      "mov rax, -0x2 ; moving imm to reg\n"
      "inc rax; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rbp, [ rsp + 0x50 ]\n"
      "movzx r12, r12b\n"
      "lea r10, [ r10 + r11 ]\n"
      "lea r10, [ r10 + r12 ]\n"
      "adcx rbx, r10\n"
      "movzx r13, r13b\n"
      "lea r8, [ r8 + rbx ]\n"
      "lea r8, [ r8 + r13 ]\n"
      "movzx rcx, cl\n"
      "lea r8, [ rcx + r8 ]\n"
      "mov rcx, [ rsp + 0x58 ]\n"
      "lea r8, [rcx+r8]\n"
      "adox r8, r14\n"
      "mov rax, rbp; x151, copying x148 here, cause x148 is needed in a reg "
      "for "
      "other than x151, namely all: , x151, x152, size: 2\n"
      "shrd rax, r8, 51; x151 <- x150||x148 >> 51\n"
      "imul rax, rax, 0x13; x153 <- x151 * 0x13\n"
      "lea r15, [ r15 + rax ]\n"
      "mov r11, r15; x155, copying x154 here, cause x154 is needed in a reg "
      "for "
      "other than x155, namely all: , x155, x156, size: 2\n"
      "shr r11, 51; x155 <- x154>> 51\n"
      "mov r12, 0x7ffffffffffff ; moving imm to reg\n"
      "mov r13, [ rsp + 0x30 ]; x137, copying x133 here, cause x133 is needed "
      "in "
      "a reg for other than x137, namely all: , x137, size: 1\n"
      "and r13, r12; x137 <- x133& 0x7ffffffffffff\n"
      "lea r11, [ r11 + r13 ]\n"
      "mov rcx, r11; x159, copying x157 here, cause x157 is needed in a reg "
      "for "
      "other than x159, namely all: , x158, x159, size: 2\n"
      "and rcx, r12; x159 <- x157& 0x7ffffffffffff\n"
      "mov [ rdi + 0x8 ], rcx; out1[1] = x159\n"
      "and rbp, r12; x152 <- x148& 0x7ffffffffffff\n"
      "mov [ rdi + 0x20 ], rbp; out1[4] = x152\n"
      "shr r11, 51; x158 <- x157>> 51\n"
      "mov r10, [ rsp + 0x40 ]; x142, copying x138 here, cause x138 is needed "
      "in "
      "a reg for other than x142, namely all: , x142, size: 1\n"
      "and r10, r12; x142 <- x138& 0x7ffffffffffff\n"
      "lea r11, [ r11 + r10 ]\n"
      "mov [ rdi + 0x10 ], r11; out1[2] = x160\n"
      "and r15, r12; x156 <- x154& 0x7ffffffffffff\n"
      "mov [ rdi + 0x0 ], r15; out1[0] = x156\n"
      "mov rbx, [ rsp + 0x60 ]; restoring from stack\n"
      "mov rbp, [ rsp + 0x68 ]; restoring from stack\n"
      "mov r12, [ rsp + 0x70 ]; restoring from stack\n"
      "mov r13, [ rsp + 0x78 ]; restoring from stack\n"
      "mov r14, [ rsp + 0x80 ]; restoring from stack\n"
      "mov r15, [ rsp + 0x88 ]; restoring from stack\n"
      "add rsp, 0x90 \n"
      "ret\n"
      "; cpu Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz\n"
      "; clocked at 1800 MHz\n"
      "; first cyclecount 4218, last 3918\n"
      "; seed 1 \n"
      "; time needed: 15526 ms / 1006 runs=> 15.433399602385686ms/run\n"
      "; Time spent for assembling and measureing (r=200,e=100): 3566 ms\n"
      "; Ratio (time for assembling + measure)/(total runtime for 1006runs): "
      "0.2296792477135128\n"
      "; number reverted permutation/ tried permutation: 436 / 443 "
      "=0.9841986455981941\n"
      "; number reverted spill/ tried spill: 266 / 272 =0.9779411764705882\n"
      "; number reverted decision/ tried decision: 285 / 290 "
      "=0.9827586206896551\n"
      "; FailedEvals (cannot swap because of edge to the next node): 783\n"};

  char fb[] = {
      "SECTION .text\n"
      "GLOBAL mul_curve25519\n"
      "mul_curve25519:\n"
      "sub rsp, 0x78 ; last 0x30 (6) for Caller - save regs \n"
      "mov [ rsp + 0x48 ], rbx; saving to stack\n"
      "mov [ rsp + 0x50 ], rbp; saving to stack\n"
      "mov [ rsp + 0x58 ], r12; saving to stack\n"
      "mov [ rsp + 0x60 ], r13; saving to stack\n"
      "mov [ rsp + 0x68 ], r14; saving to stack\n"
      "mov [ rsp + 0x70 ], r15; saving to stack\n"
      "mov r13, rdx; preserving value of arg2 into a new reg\n"
      "mov rdx, [ rsi + 0x0 ]; saving arg1[0] in rdx.\n"
      "mulx rcx, r15, [ r13 + 0x0 ]; x50, x49<- arg1[0] * arg2[0]\n"
      "imul r8, [ r13 + 0x20 ], 0x13; x10000 <- arg2[4] * 0x13\n"
      "mov rdx, r8; x10000 to rdx\n"
      "mulx r10, r9, [ rsi + 0x8 ]; x20, x19<- arg1[1] * x10000\n"
      "imul rax, [ r13 + 0x18 ], 0x13; x10001 <- arg2[3] * 0x13\n"
      "mov rbx, rdx; preserving value of x10000 into a new reg\n"
      "mov rdx, [ rsi + 0x10 ]; saving arg1[2] in rdx.\n"
      "mulx r14, rbp, rax; x18, x17<- arg1[2] * x10001\n"
      "imul r8, [ r13 + 0x10 ], 0x13; x10002 <- arg2[2] * 0x13\n"
      "mov rdx, r8; x10002 to rdx\n"
      "mulx r12, r11, [ rsi + 0x18 ]; x14, x13<- arg1[3] * x10002\n"
      "imul r8, [ r13 + 0x8 ], 0x13; x10003 <- arg2[1] * 0x13\n"
      "mov [ rsp + 0x0 ], r13; spilling arg2 to mem\n"
      "mov r13, rdx; preserving value of x10002 into a new reg\n"
      "mov rdx, [ rsi + 0x20 ]; saving arg1[4] in rdx.\n"
      "mov [ rsp + 0x8 ], rdi; spilling out1 to mem\n"
      "mulx r8, rdi, r8; x8, x7<- arg1[4] * x10003\n"
      "test al, al\n"
      "adox rdi, r11\n"
      "adcx rdi, rbp\n"
      "seto bpl; spill OF x52 to reg (rbp)\n"
      "mov r11, -0x2 ; moving imm to reg\n"
      "inc r11; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rdi, r9\n"
      "setc r11b; spill CF x56 to reg (r11)\n"
      "clc;\n"
      "adcx rdi, r15\n"
      "movzx rbp, bpl\n"
      "lea r12, [ r12 + r8 ]\n"
      "lea r12, [ r12 + rbp ]\n"
      "movzx r11, r11b\n"
      "lea r14, [ r14 + r12 ]\n"
      "lea r14, [ r14 + r11 ]\n"
      "adox r10, r14\n"
      "adcx rcx, r10\n"
      "mov r15, rdi; x67, copying x63 here, cause x63 is needed in a reg for "
      "other than x67, namely all: , x67, x68, size: 2\n"
      "shrd r15, rcx, 51; x67 <- x65||x63 >> 51\n"
      "mov r9, [ rsp + 0x0 ]; load m64 arg2 to register64\n"
      "mov rdx, [ r9 + 0x8 ]; arg2[1] to rdx\n"
      "mulx r8, rbp, [ rsi + 0x0 ]; x48, x47<- arg1[0] * arg2[1]\n"
      "mov rdx, [ r9 + 0x0 ]; arg2[0] to rdx\n"
      "mulx r11, r12, [ rsi + 0x8 ]; x40, x39<- arg1[1] * arg2[0]\n"
      "mov rdx, [ rsi + 0x10 ]; arg1[2] to rdx\n"
      "mulx r14, r10, rbx; x16, x15<- arg1[2] * x10000\n"
      "mov rdx, [ rsi + 0x18 ]; arg1[3] to rdx\n"
      "mov [ rsp + 0x0 ], r9; spilling arg2 to mem\n"
      "mulx rcx, r9, rax; x12, x11<- arg1[3] * x10001\n"
      "mov rdx, [ rsi + 0x20 ]; arg1[4] to rdx\n"
      "mov [ rsp + 0x10 ], r11; spilling x40 to mem\n"
      "mulx r13, r11, r13; x6, x5<- arg1[4] * x10002\n"
      "test al, al\n"
      "adox r11, r9\n"
      "adcx r11, r10\n"
      "setc r10b; spill CF x122 to reg (r10)\n"
      "clc;\n"
      "adcx r11, r12\n"
      "seto r12b; spill OF x118 to reg (r12)\n"
      "mov r9, -0x2 ; moving imm to reg\n"
      "inc r9; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r11, rbp\n"
      "seto r9b; spill OF x130 to reg (r9)\n"
      "mov rbp, -0x2 ; moving imm to reg\n"
      "inc rbp; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r11, r15\n"
      "movzx r12, r12b\n"
      "lea rcx, [ rcx + r13 ]\n"
      "lea rcx, [ rcx + r12 ]\n"
      "movzx r10, r10b\n"
      "lea r14, [ r14 + rcx ]\n"
      "lea r14, [ r14 + r10 ]\n"
      "adcx r14, [ rsp + 0x10 ]\n"
      "movzx r9, r9b\n"
      "lea r8, [ r8 + r14 ]\n"
      "lea r8, [ r8 + r9 ]\n"
      "mov rbp, 0x0 ; moving imm to reg\n"
      "adox r8, rbp\n"
      "mov r15, r11; x136, copying x133 here, cause x133 is needed in a reg "
      "for "
      "other than x136, namely all: , x136, x137, size: 2\n"
      "shrd r15, r8, 51; x136 <- x135||x133 >> 51\n"
      "mov rdx, [ rsi + 0x18 ]; arg1[3] to rdx\n"
      "mulx r13, r12, rbx; x10, x9<- arg1[3] * x10000\n"
      "mov r10, [ rsp + 0x0 ]; load m64 arg2 to register64\n"
      "mov rdx, [ rsi + 0x0 ]; arg1[0] to rdx\n"
      "mulx r9, rcx, [ r10 + 0x10 ]; x46, x45<- arg1[0] * arg2[2]\n"
      "mov rdx, [ rsi + 0x8 ]; arg1[1] to rdx\n"
      "mulx r14, r8, [ r10 + 0x8 ]; x38, x37<- arg1[1] * arg2[1]\n"
      "mov rdx, [ r10 + 0x0 ]; arg2[0] to rdx\n"
      "mov [ rsp + 0x18 ], r15; spilling x136 to mem\n"
      "mulx rbp, r15, [ rsi + 0x10 ]; x32, x31<- arg1[2] * arg2[0]\n"
      "mov rdx, rax; x10001 to rdx\n"
      "mulx rdx, rax, [ rsi + 0x20 ]; x4, x3<- arg1[4] * x10001\n"
      "add rax, r12; could be done better, if r0 has been u8 as well\n"
      "mov r12, -0x2 ; moving imm to reg\n"
      "inc r12; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rax, r15\n"
      "seto r12b; spill OF x106 to reg (r12)\n"
      "mov r15, -0x2 ; moving imm to reg\n"
      "inc r15; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rax, r8\n"
      "seto r15b; spill OF x110 to reg (r15)\n"
      "mov r8, -0x2 ; moving imm to reg\n"
      "inc r8; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rax, rcx\n"
      "setc r8b; spill CF x102 to reg (r8)\n"
      "clc;\n"
      "adcx rax, [ rsp + 0x18 ]\n"
      "movzx r8, r8b\n"
      "lea r13, [ r13 + rdx ]\n"
      "lea r13, [ r13 + r8 ]\n"
      "movzx r12, r12b\n"
      "lea rbp, [ rbp + r13 ]\n"
      "lea rbp, [ rbp + r12 ]\n"
      "movzx r15, r15b\n"
      "lea r14, [ r14 + rbp ]\n"
      "lea r14, [ r14 + r15 ]\n"
      "adox r9, r14\n"
      "mov rdx, [ r10 + 0x18 ]; arg2[3] to rdx\n"
      "mulx rcx, r8, [ rsi + 0x0 ]; x44, x43<- arg1[0] * arg2[3]\n"
      "adc r9, 0x0\n"
      "mov r12, rax; x141, copying x138 here, cause x138 is needed in a reg "
      "for "
      "other than x141, namely all: , x141, x142, size: 2\n"
      "shrd r12, r9, 51; x141 <- x140||x138 >> 51\n"
      "mov rdx, [ r10 + 0x10 ]; arg2[2] to rdx\n"
      "mulx r15, r13, [ rsi + 0x8 ]; x36, x35<- arg1[1] * arg2[2]\n"
      "mov rdx, [ r10 + 0x8 ]; arg2[1] to rdx\n"
      "mulx rbp, r14, [ rsi + 0x10 ]; x30, x29<- arg1[2] * arg2[1]\n"
      "mov rdx, [ r10 + 0x0 ]; arg2[0] to rdx\n"
      "mov [ rsp + 0x20 ], rbp; spilling x30 to mem\n"
      "mulx r9, rbp, [ rsi + 0x18 ]; x26, x25<- arg1[3] * arg2[0]\n"
      "mov rdx, [ rsi + 0x20 ]; arg1[4] to rdx\n"
      "mov [ rsp + 0x0 ], r10; spilling arg2 to mem\n"
      "mulx rbx, r10, rbx; x2, x1<- arg1[4] * x10000\n"
      "mov [ rsp + 0x28 ], r15; spilling x36 to mem\n"
      "xor r15, r15\n"
      "adox r10, rbp\n"
      "adcx r10, r14\n"
      "seto r14b; spill OF x86 to reg (r14)\n"
      "mov rbp, -0x2 ; moving imm to reg\n"
      "inc rbp; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r10, r13\n"
      "setc bpl; spill CF x90 to reg (rbp)\n"
      "clc;\n"
      "adcx r10, r8\n"
      "seto r8b; spill OF x94 to reg (r8)\n"
      "mov r13, -0x2 ; moving imm to reg\n"
      "inc r13; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox r10, r12\n"
      "mov r13, 0x7ffffffffffff ; moving imm to reg\n"
      "setc r12b; spill CF x98 to reg (r12)\n"
      "seto r15b; spill OF x144 to reg (r15)\n"
      "mov byte [ rsp + 0x30 ], r8b; spilling byte x94 to mem\n"
      "mov r8, r10; x147, copying x143 here, cause x143 is needed in a reg for "
      "other than x147, namely all: , x147, x146, size: 2\n"
      "and r8, r13; x147 <- x143& 0x7ffffffffffff\n"
      "and rdi, r13; x68 <- x63& 0x7ffffffffffff\n"
      "mov r13, [ rsp + 0x8 ]; load m64 out1 to register64\n"
      "mov [ r13 + 0x18 ], r8; out1[3] = x147\n"
      "sar  r14b, 1\n"
      "adcx r9, rbx\n"
      "sar  bpl, 1\n"
      "adcx r9, [ rsp + 0x20 ]\n"
      "sar byte [ rsp + 0x30 ], 1\n"
      "adcx r9, [ rsp + 0x28 ]\n"
      "sar  r12b, 1\n"
      "adcx rcx, r9\n"
      "movzx rbx, r15b; x145, copying x144 here, cause x144 is needed in a reg "
      "for other than x145, namely all: , x145, size: 1\n"
      "lea rbx, [ rbx + rcx ]\n"
      "shrd r10, rbx, 51; x146 <- x145||x143 >> 51\n"
      "mov r14, [ rsp + 0x0 ]; load m64 arg2 to register64\n"
      "mov rdx, [ r14 + 0x20 ]; arg2[4] to rdx\n"
      "mulx rdx, rbp, [ rsi + 0x0 ]; x42, x41<- arg1[0] * arg2[4]\n"
      "mov r12, rdx; preserving value of x42 into a new reg\n"
      "mov rdx, [ r14 + 0x18 ]; saving arg2[3] in rdx.\n"
      "mulx r15, r8, [ rsi + 0x8 ]; x34, x33<- arg1[1] * arg2[3]\n"
      "mov rdx, [ r14 + 0x10 ]; arg2[2] to rdx\n"
      "mulx r9, rcx, [ rsi + 0x10 ]; x28, x27<- arg1[2] * arg2[2]\n"
      "mov rdx, [ r14 + 0x8 ]; arg2[1] to rdx\n"
      "mov [ rsp + 0x8 ], r13; spilling out1 to mem\n"
      "mulx rbx, r13, [ rsi + 0x18 ]; x24, x23<- arg1[3] * arg2[1]\n"
      "mov rdx, [ r14 + 0x0 ]; arg2[0] to rdx\n"
      "mov [ rsp + 0x38 ], rbx; spilling x24 to mem\n"
      "mov [ rsp + 0x40 ], rdi; spilling x68 to mem\n"
      "mulx rbx, rdi, [ rsi + 0x20 ]; x22, x21<- arg1[4] * arg2[0]\n"
      "add rdi, r13; could be done better, if r0 has been u8 as well\n"
      "mov r13, -0x2 ; moving imm to reg\n"
      "inc r13; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rdi, rcx\n"
      "setc r13b; spill CF x70 to reg (r13)\n"
      "clc;\n"
      "adcx rdi, r8\n"
      "seto r8b; spill OF x74 to reg (r8)\n"
      "mov rcx, -0x2 ; moving imm to reg\n"
      "inc rcx; OF<-0x0, preserve CF (workaround, load -2, increa it, and dont "
      "save it. I this a hack?)\n"
      "adox rdi, rbp\n"
      "setc cl; spill CF x78 to reg (rcx)\n"
      "clc;\n"
      "adcx rdi, r10\n"
      "movzx r13, r13b\n"
      "lea rbx, [ r13 + rbx ]\n"
      "mov r13, [ rsp + 0x38 ]\n"
      "lea rbx, [r13+rbx]\n"
      "movzx r8, r8b\n"
      "lea r9, [ r9 + rbx ]\n"
      "lea r9, [ r9 + r8 ]\n"
      "movzx rcx, cl\n"
      "lea r15, [ r15 + r9 ]\n"
      "lea r15, [ r15 + rcx ]\n"
      "adox r12, r15\n"
      "adc r12, 0x0\n"
      "mov r10, rdi; x151, copying x148 here, cause x148 is needed in a reg "
      "for "
      "other than x151, namely all: , x151, x152, size: 2\n"
      "shrd r10, r12, 51; x151 <- x150||x148 >> 51\n"
      "imul r10, r10, 0x13; x153 <- x151 * 0x13\n"
      "add r10, [ rsp + 0x40 ]\n"
      "mov rbp, r10; x155, copying x154 here, cause x154 is needed in a reg "
      "for "
      "other than x155, namely all: , x155, x156, size: 2\n"
      "shr rbp, 51; x155 <- x154>> 51\n"
      "mov r13, 0x7ffffffffffff ; moving imm to reg\n"
      "and r11, r13; x137 <- x133& 0x7ffffffffffff\n"
      "lea rbp, [ rbp + r11 ]\n"
      "mov r8, rbp; x159, copying x157 here, cause x157 is needed in a reg for "
      "other than x159, namely all: , x158, x159, size: 2\n"
      "and r8, r13; x159 <- x157& 0x7ffffffffffff\n"
      "mov rcx, [ rsp + 0x8 ]; load m64 out1 to register64\n"
      "mov [ rcx + 0x8 ], r8; out1[1] = x159\n"
      "and rdi, r13; x152 <- x148& 0x7ffffffffffff\n"
      "shr rbp, 51; x158 <- x157>> 51\n"
      "mov [ rcx + 0x20 ], rdi; out1[4] = x152\n"
      "and rax, r13; x142 <- x138& 0x7ffffffffffff\n"
      "lea rbp, [ rbp + rax ]\n"
      "mov [ rcx + 0x10 ], rbp; out1[2] = x160\n"
      "and r10, r13; x156 <- x154& 0x7ffffffffffff\n"
      "mov [ rcx + 0x0 ], r10; out1[0] = x156\n"
      "mov rbx, [ rsp + 0x48 ]; restoring from stack\n"
      "mov rbp, [ rsp + 0x50 ]; restoring from stack\n"
      "mov r12, [ rsp + 0x58 ]; restoring from stack\n"
      "mov r13, [ rsp + 0x60 ]; restoring from stack\n"
      "mov r14, [ rsp + 0x68 ]; restoring from stack\n"
      "mov r15, [ rsp + 0x70 ]; restoring from stack\n"
      "add rsp, 0x78 \n"
      "ret\n"
      "; cpu Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz\n"
      "; clocked at 2699 MHz\n"
      "; first cyclecount 4025, last 3601\n"
      "; seed 1 \n"
      "; time needed: 18685 ms / 1006 runs=> 18.573558648111334ms/run\n"
      "; Time spent for assembling and measureing (r=200,e=100): 4107 ms\n"
      "; Ratio (time for assembling + measure)/(total runtime for 1006runs): "
      "0.2198019801980198\n"
      "; number reverted permutation/ tried permutation: 385 / 394 "
      "=0.9771573604060914\n"
      "; number reverted spill/ tried spill: 305 / 309 =0.9870550161812298\n"
      "; number reverted decision/ tried decision: 301 / 302 "
      "=0.9966887417218543\n"
      "; FailedEvals (cannot swap because of edge to the next node): 638\n"};
  // convenience pointer
  void (*err)(measuresuite_t, const char *) =
      error_handling_helper_template_str;

  const int num_batches = 10;
  const int batch_size = 20;

  // INIT
  int arg_num_out = 1;
  int chunksize = 0;
  measuresuite_t ms = NULL;
  printf("strlen fa / fb %lu, %lu\n", strlen(fa), strlen(fb));
  if (ms_measure_init(&ms, arg_width, arg_num_in, arg_num_out, chunksize,
                      bounds, "./liball_fiat.so", symbol)) {
    err(ms, "Failed to measure_init. Reason: %s.");
    return 1;
  }

  // measure
  if (ms_measure(ms, fa, fa, batch_size, num_batches)) {
    err(ms, "Failed to measure. Reason: %s.");
    return 1;
  }
  const char *output = NULL;
  size_t jsonlen = 0;
  ms_getJson(ms, &output, &jsonlen);
  printf("%s\n", output);

  // Require to have true in the string but not false
  if (strstr(output, "true") == NULL && strstr(output, "false") != NULL) {
    fprintf(stderr, "should have been a correct result\n");
    ms_measure_end(ms);
    return 1;
  }

  // END
  if (ms_measure_end(ms)) {
    err(ms, "Failed to measure_end. Reason: %s.");
    return 1;
  }
  return 0;
}