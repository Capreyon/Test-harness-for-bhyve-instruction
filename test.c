/*
 * Test harness for bhyve instruction emulator
 */

#include <sys/types.h>
#include <sys/errno.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "vmm_stubs.h"

static uint64_t vm_regs[VM_REG_LAST];

struct mem_cell {
	uint64_t   addr;
	uint64_t   val;
};

int
vm_get_register(void *ctx, int vcpu, int reg, uint64_t *retval)
{

	if (reg >= VM_REG_GUEST_RAX &&
	    reg < VM_REG_LAST) {
		*retval = vm_regs[reg];
		return (0);
	}
		
	return (EINVAL);
}

int
vm_set_register(void *ctx, int vcpu, int reg, uint64_t val)
{

	if (reg >= VM_REG_GUEST_RAX &&
	    reg < VM_REG_LAST) {
		vm_regs[reg] = reg;
		return (0);
	}

	return (EINVAL);
}

/*
 * Memory r/w callback functions
 */
int
test_mread(void *vm, int cpuid, uint64_t gpa, uint64_t *rval, int rsize,
    void *arg)
{
	struct mem_cell *mc;

	mc = arg;

	if (mc->addr == gpa) {
		*rval = mc->val;
		return (0);
	}

	return (EINVAL);
}

int
test_mwrite(void *vm, int cpuid, uint64_t gpa, uint64_t wval,
    int wsize, void *arg)
{
	struct mem_cell *mc;

	mc = arg;

	if (mc->addr == gpa) {
		mc->val = wval;
		return (0);
	}
	
	return (EINVAL);
}

void
panic(char *str, ...)
{

	printf("panic: %s\n", str);
}


main()
{
	struct mem_cell mc;
	struct vie vie;
	uint64_t gla, gpa;
	int err;

	/*
	 * (rcx = lapic address, 0xff000000)
	 * and    0xf0(%rcx),%eax
	 * 0x23 0x81 0xf0 0x00 0x00 0x00
	 */
	memset(&vie, 0, sizeof(struct vie));
	vie.base_register = VM_REG_LAST;
	vie.index_register = VM_REG_LAST;

	vm_regs[VM_REG_GUEST_RAX] = 0x0000aabb;
	vm_regs[VM_REG_GUEST_RCX] = 0xff000000;
	vie.inst[0] = 0x23;
	vie.inst[1] = 0x81;
	vie.inst[2] = 0xf0;
	vie.inst[3] = 0x00;
	vie.inst[4] = 0x00;
	vie.inst[5] = 0x00;
	vie.num_valid = 6;

	gla = 0;
	err = vmm_decode_instruction(NULL, 0, gla, &vie);
	assert(err == 0);

	mc.addr = 0xff0000f0;
	mc.val  = 0x0000aa00;
	gpa = 0xff0000f0;
	err = vmm_emulate_instruction(NULL, 0, gpa, &vie,
				      test_mread, test_mwrite,
				      &mc);
	assert(err == 0);

	/*
	 * (rax = lapic address, 0xff000000)
         * andl   $0xfffffeff,0xf0(%rax)
         * 0x81 0xa0 0xf0 0x00 0x00 0x00 0xff 0xfe 0xff 0xff
         */
	memset(&vie, 0, sizeof(struct vie));
	vie.base_register = VM_REG_LAST;
	vie.index_register = VM_REG_LAST;

	vm_regs[VM_REG_GUEST_RAX] = 0xff000000;
	vie.inst[0] = 0x81;
	vie.inst[1] = 0xa0;
	vie.inst[2] = 0xf0;
	vie.inst[3] = 0x00;
	vie.inst[4] = 0x00;
	vie.inst[5] = 0x00;
	vie.inst[6] = 0xff;
	vie.inst[7] = 0xfe;
	vie.inst[8] = 0xff;
	vie.inst[9] = 0xff;
	vie.num_valid = 10;

	gla = 0;
	err = vmm_decode_instruction(NULL, 0, gla, &vie);
	assert(err == 0);

	mc.addr = 0xff0000f0;
	mc.val  = 0x0000a1aa;
	gpa = 0xff0000f0;
	err = vmm_emulate_instruction(NULL, 0, gpa, &vie,
				      test_mread, test_mwrite,
				      &mc);
	assert(err == 0);
	assert(mc.val == 0xa0aa);

	/*
	 * mov    %r8d,5827804(%rip)
	 * 44 89 05 dc ec 58 00
	 * rip -> 0xffffffff8046539d
	 * var -> 0xffffffff809f4080
	 */
	memset(&vie, 0, sizeof(struct vie));
	vie.base_register = VM_REG_LAST;
	vie.index_register = VM_REG_LAST;

	/* RIP-relative is from next instruction */
	vm_regs[VM_REG_GUEST_RIP] = 0xffffffff8046539d + 7;	
	vm_regs[VM_REG_GUEST_R8] = 0xa5a5a5a5deadbeefULL;
	vie.inst[0] = 0x44;
	vie.inst[1] = 0x89;
	vie.inst[2] = 0x05;
	vie.inst[3] = 0xdc;
	vie.inst[4] = 0xec;
	vie.inst[5] = 0x58;
	vie.inst[6] = 0x00;
	vie.num_valid = 7;

	gla = 0;
	err = vmm_decode_instruction(NULL, 0, gla, &vie);
	assert(err == 0);

	mc.addr = 0xff000080;
	mc.val  = 0;
	gpa = 0xff000080;
	err = vmm_emulate_instruction(NULL, 0, gpa, &vie,
				      test_mread, test_mwrite,
				      &mc);
	assert(err == 0);
	assert(mc.val == 0xdeadbeef);

	/*
	 * movl   $0x1ef,5872021(%rip)
	 * c7 05 95 99 59 00 ef
	 * rip -> ffffffff813d2751
	 * val -> ffffffff8196c0f0
	 * pa -> 0xfee000f0
	 */
	memset(&vie, 0, sizeof(struct vie));
	vie.base_register = VM_REG_LAST;
	vie.index_register = VM_REG_LAST;

	/* RIP-relative is from next instruction */
	vm_regs[VM_REG_GUEST_RIP] = 0xffffffff8046539d + 7;	
	vie.inst[0] = 0xc7;
	vie.inst[1] = 0x05;
	vie.inst[2] = 0x95;
	vie.inst[3] = 0x99;
	vie.inst[4] = 0x59;
	vie.inst[5] = 0x00;
	vie.inst[6] = 0xef;
	vie.num_valid = 7;

        gla = 0;
        err = vmm_decode_instruction(NULL, 0, gla, &vie);
        assert(err == 0);

}
