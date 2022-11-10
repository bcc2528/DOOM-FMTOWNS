section .text
		align 4
global SlopeDiv
SlopeDiv:
	mov		ecx, [esp+8]
	cmp		ecx, 512
	jnb		ans_SlopeDiv
	mov		eax, 2048
	jmp		end_SlopeDiv
ans_SlopeDiv:
	shr		ecx, 8
	mov		eax, [esp+4]
	shl		eax, 3
	sub		edx, edx
	div		ecx
	cmp		eax, 2048
	jbe		end_SlopeDiv
	mov		eax, 2048
end_SlopeDiv:
	ret
