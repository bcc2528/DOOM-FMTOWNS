section .text
		align 4
global FixedMul
FixedMul:
	mov		eax, [esp+4]
	imul		dword [esp+8]
	shrd		eax, edx, 16
	ret

		align 4
global FixedDiv
FixedDiv:
;;fixed_t FixedDiv(fixed_t a, fixed_t b) --- a = edi register, b = esi register
	push		ebp
	mov		ebp,esp
	push		esi
	push		edi
	mov		edi,dword[ebp+8]
	mov		esi,dword[ebp+12]
;;(abs(a)>>14)
	mov		eax,edi
	cdq
	xor		eax,edx
	sub		eax,edx
	sar		eax,14
	mov		ecx,eax
;;(abs(b))
	mov		eax,esi
	cdq
	xor		eax,edx
	sub		eax,edx
;;if ( (_abs(a)>>14) >= _abs(b))
	cmp		eax,ecx
	jnle FixedDiv2
;;return (a^b)<0 ? MININT : MAXINT;
	mov 		eax,esi
	xor 		eax,edi
	inc 		eax
	jle 		div_min
	mov 		eax,2147483647
	jmp 		end_div
div_min:
	mov 		eax,-2147483648
	jmp 		end_div
FixedDiv2:
;;((long long)a<<16) / ((long long)b)
	mov		eax, edi
	cdq
	shld		edx,eax,16
	sal		eax,16
	idiv		esi
end_div:
	pop		edi
	pop		esi
	leave
	ret
