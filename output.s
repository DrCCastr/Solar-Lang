	.text
	.def	@feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 0
	.file	"Main"
	.def	idk;
	.scl	2;
	.type	32;
	.endef
	.globl	idk                             # -- Begin function idk
	.p2align	4, 0x90
idk:                                    # @idk
# %bb.0:                                # %entry
                                        # kill: def $edx killed $edx def $rdx
                                        # kill: def $ecx killed $ecx def $rcx
	leal	(%rcx,%rdx), %eax
	retq
                                        # -- End function
	.def	main;
	.scl	2;
	.type	32;
	.endef
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
main:                                   # @main
.seh_proc main
# %bb.0:                                # %entry
	pushq	%rbp
	.seh_pushreg %rbp
	subq	$48, %rsp
	.seh_stackalloc 48
	leaq	48(%rsp), %rbp
	.seh_setframe %rbp, 48
	.seh_endprologue
	callq	__main
	movl	$1, %ecx
	movl	$2, %edx
	callq	idk
	movl	%eax, -4(%rbp)
	addq	$48, %rsp
	popq	%rbp
	retq
	.seh_endproc
                                        # -- End function
