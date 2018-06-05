PROG=itest

SRCS= test.c vmm_instruction_emul.c

CFLAGS+= -D_VERIFICATION

NO_MAN=

.include <bsd.prog.mk>
