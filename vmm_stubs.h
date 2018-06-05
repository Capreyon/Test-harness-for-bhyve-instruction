/*
 * Identifiers for architecturally defined registers.
 */
enum vm_reg_name {
	VM_REG_GUEST_RAX,
	VM_REG_GUEST_RBX,
	VM_REG_GUEST_RCX,
	VM_REG_GUEST_RDX,
	VM_REG_GUEST_RSI,
	VM_REG_GUEST_RDI,
	VM_REG_GUEST_RBP,
	VM_REG_GUEST_R8,
	VM_REG_GUEST_R9,
	VM_REG_GUEST_R10,
	VM_REG_GUEST_R11,
	VM_REG_GUEST_R12,
	VM_REG_GUEST_R13,
	VM_REG_GUEST_R14,
	VM_REG_GUEST_R15,
	VM_REG_GUEST_CR0,
	VM_REG_GUEST_CR3,
	VM_REG_GUEST_CR4,
	VM_REG_GUEST_DR7,
	VM_REG_GUEST_RSP,
	VM_REG_GUEST_RIP,
	VM_REG_GUEST_RFLAGS,
	VM_REG_GUEST_ES,
	VM_REG_GUEST_CS,
	VM_REG_GUEST_SS,
	VM_REG_GUEST_DS,
	VM_REG_GUEST_FS,
	VM_REG_GUEST_GS,
	VM_REG_GUEST_LDTR,
	VM_REG_GUEST_TR,
	VM_REG_GUEST_IDTR,
	VM_REG_GUEST_GDTR,
	VM_REG_GUEST_EFER,
	VM_REG_LAST
};

void	panic(char *str, ...);

int	vm_get_register(void *ctx, int vcpu, int reg, uint64_t *retval);
int	vm_set_register(void *ctx, int vcpu, int reg, uint64_t val);

#include "vmm_instruction_emul.h"

