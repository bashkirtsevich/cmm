; ��������������� �������
; ---------------------------------------------------------------------------
; �������������� ��������� � ����� � ��������� ���������
; ���������, ��� ��� ����� ��������
; ---------------------------------------------------------------------------
  proc	RestoreState
	mov	esp, [SavedESPState]
	popad
	ret	; �� �������� ����� ������ ������ ����� CMM32CompileEnd
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	fprint
  lpBuffer	  = 8
	push	ebp
	mov	ebp, esp

	; ����� OnWriteMAPFile
;        invoke  lstrlenA, [ebp+lpBuffer]

;        push    eax
	push	dword [ebp+lpBuffer]
	call	OnWriteMAPFile

	pop	ebp
	retn	4
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	WRITESTR

	push	eax
	call	OnWriteMessage

	retn
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc errmsg
       mov     eax, errmsg_msg
       call    WRITESTR

       retn
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc outofmemory2
       call    errmsg

       mov     eax, outofmemory2_str
       call    WRITESTR

;       invoke  ExitProcess, e_outofmemory
	call	RestoreState

       retn
  endp
; ---------------------------------------------------------------------------
; ����� �������� CODE&DATA
; ---------------------------------------------------------------------------

  proc	PrintMemsizes
  handle	  = 8

	push	ebp
	mov	ebp, esp

	mov	eax, [outptr]
	sub	eax, [output]

	invoke	wsprintfA, mapstr, PrintMemsizes_str1, eax, [postsize]
	add	esp, 10h

	push	mapstr
	call	fprint

	pop	ebp
	ret	4
  endp
; ---------------------------------------------------------------------------
; ������ �������� ����� � �����
; ---------------------------------------------------------------------------
  proc	LoadInputfile
  lpPathName	  = 8

	push	ebp
	mov	ebp, esp
; ��� ������ ������ � ������/������� �������������� ������ ������� �-��
	push	dword [ebp+lpPathName] ; ��������� �� ��� �����
	push	input		       ; ��������� �� ����������
	call	OnLoadFile

	mov	eax, [input]
	mov	[inptr], eax

	mov	eax, [input]
	mov	[inptr2], eax

	mov	[endoffile], 0 ; �� ������ �����

;        xor     eax, eax ; �� ����� ������, ������� ����� ����������

LoadInputfile_end:
	pop	ebp
	ret	4
  endp
; ---------------------------------------------------------------------------
; ������ �������� �����
; ---------------------------------------------------------------------------
  proc	unabletoopen
  hstr		  = -50h
  str_		  = 8

	enter	50h, 0		; byte hstr[80];

	lea	eax, [ebp+hstr]
	invoke	wsprintfA, eax, unabletoopen_str1, [ebp+str_]
	add	esp, 0Ch

	lea	eax, [ebp+hstr]
	push	eax
	call	preerror

	leave
	ret	4
  endp
; ---------------------------------------------------------------------------
; ����� ������ ��������� ������
; ---------------------------------------------------------------------------
  proc	ShowSrcLine
  str_		  = -3E8h

	enter	3E8h, 0

	cmp	[list], 0	  ; if1
	jz	ShowSrcLine_end

	xchg	esi, [inptr]

	push	edi
	push	esi

	inc	esi

	lea	edi, [ebp+str_]

ShowSrcLine_loop1start:
	lodsb

	cmp	al, 13
	jz	ShowSrcLine_loop1end

	test	al, al
	jz	ShowSrcLine_loop1end

	stosb

	jmp	ShowSrcLine_loop1start

ShowSrcLine_loop1end:
	mov	al, 0

	stosb

	pop	esi
	pop	edi

	xchg	esi, [inptr]

	cmp	[displaytokerrors], 0 ; if2
	jz	ShowSrcLine_end

	lea	eax, [ebp+str_]
	push	eax
	call	fprint

	push	(DisplayTree_str1+1Ch) ; ������ ������ "\n"
	call	fprint

ShowSrcLine_end:
	leave
	ret
  endp
; ---------------------------------------------------------------------------
; ������ ���������� ������� �� �������� ������
; ---------------------------------------------------------------------------
  proc	NextChar
	xchg	esi, [inptr]

	lodsb

	mov	[cha], al

	xchg	esi, [inptr]	  ; �������� ������� ������� � ������

	test	al, al		; if1
	jz	NextChar_if1start

	cmp	al, 26
	jnz	NextChar_if1end

NextChar_if1start:
	mov	[endoffile], 1

NextChar_if1end:
	cmp	al, 13		; if2 (CR)
	jnz	NextChar_if2end

	inc	[linenumber]	  ; ��������� ����� ������
	inc	[totallines]

	call	ShowSrcLine

	call	NextChar

NextChar_if2end:
	ret
  endp
; ---------------------------------------------------------------------------
; ��������� ���������� token
; ---------------------------------------------------------------------------
  proc	NextTok
	mov	eax, [inptr2]
	mov	[inptr], eax

	mov	eax, [linenum2]
	mov	[linenumber], eax

	mov	al, [cha2]
	mov	[cha], al

	mov	[displaytokerrors], 1

	push	number	 ; int
	push	string	 ; lpString2
	push	post	 ; int
	push	src	 ; int
	push	type	 ; int
	push	tok	 ; int
	call	TokScan

	mov	eax, [linenum2]
	cmp	[linenumber], eax ; if1 (����� ������?)
	jz	short NextTok_if1end

	cmp	[dbg], 0	  ; if2 (��������� �������?)
	jz	short NextTok_if1end

	push	esi
	mov	esi, [dbgs]
	imul	esi, dbg_size
	add	esi, [dbginfo]

	mov	eax, [currmod]

	cmp	eax, [esi+dbg_mod]
	jnz	NextTok_DIFF ; ��������� ��������� ���������� �� ����� ������

	mov	eax, [linenumber]

	cmp	eax, [esi+dbg_line]
	jnz	NextTok_DIFF ; ��������� ��������� ���������� �� ����� ������

	mov	eax, [linenumber]
	mov	[esi+dbg_line], eax ; �������� ����� ������

	jmp	NextTok_ALL

NextTok_DIFF:
	inc	[dbgs]

	add	esi, dbg_size

	mov	eax, [currmod]
	mov	[esi+dbg_mod], eax

	mov	eax, [linenumber]
	mov	[esi+dbg_line], eax

	mov	eax, [outptr]
	mov	[esi+dbg_loc], eax

NextTok_ALL:
	pop	esi

NextTok_if1end:
	mov	eax, [inptr]
	mov	[inptr2], eax

	mov	eax, [linenumber]
	mov	[linenum2], eax

	mov	al, [cha]
	mov	[cha2], al

	mov	[displaytokerrors ], 0 ; ��� ������ ��������� �� �������

	push	number2  ; int
	push	string2  ; lpString2
	push	post2	 ; int
	push	src2	 ; int
	push	type2	 ; int
	push	tok2	 ; int
	call	TokScan

	mov	eax, [linenum2]
	mov	[linenumber], eax
	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	preerror
  str_		  = 8

	push	ebp
	mov	ebp, esp

	mov	eax, [maxerrors]
	cmp	[error], eax	  ; if1
	jnb	preerror_if1else

	inc	[error]

	invoke	wsprintfA, mapstr, preerror_str, currentfilename, [linenumber], [error], [ebp+str_]
	add	esp, 18h

	mov	eax, mapstr
	call	WRITESTR

	cmp	[makemapfile], 0
	jz	preerror_if2end ; if2

	push	mapstr	 ; lpBuffer
	call	fprint

preerror_if2end:
	jmp	preerror_end

preerror_if1else:
	call	toomanyerrors

preerror_end:
	pop	ebp
	ret	4
  endp
; ---------------------------------------------------------------------------
; ������ �������� �����
; ---------------------------------------------------------------------------
  proc	GetDword
  ptr_		   = 8

	push	ebp
	mov	ebp, esp

	xchg	esi, [ebp+ptr_]
	lodsd
	xchg	esi, [ebp+ptr_]

	pop	ebp
	ret	4
  endp
; ---------------------------------------------------------------------------
; ������ �������� ����� �� ������
; ---------------------------------------------------------------------------
  proc	SetDword
  ptr_		  = 8
  value 	  = 0Ch

	push	ebp
	mov	ebp, esp

	xchg	edi, [ebp+ptr_]
	xchg	eax, [ebp+value]
	stosd
	xchg	edi, [ebp+ptr_]
	xchg	eax, [ebp+value]

	pop	ebp
	ret	8
  endp
; ---------------------------------------------------------------------------
; �������� �������� token �� �������� ���
; ---------------------------------------------------------------------------
  proc	expecting
  want		  = 8

	push	ebp
	mov	ebp, esp

	mov	eax, [tok]

	cmp	[ebp+want], eax ; if1
	jz	expecting_if1end

	cmp	dword [ebp+want], tk_closebracket ; if2
	jnz	expecting_if2else

	push	')'
	call	expected ; ����� ������� ��������� ���� ���

	jmp	expecting_if1end

expecting_if2else:
	cmp	dword [ebp+want], tk_openbracket
	jnz	expecting_if3else

	push	'('
	call	expected

	jmp	expecting_if1end

expecting_if3else:
	cmp	dword [ebp+want], tk_semicolon ; if3
	jnz	expecting_if4else

	push	';'
	call	expected

	jmp	expecting_if1end

expecting_if4else:
	cmp	dword [ebp+want], tk_colon ; if4
	jnz	expecting_if5else

	push	':'
	call	expected

	jmp	expecting_if1end

expecting_if5else:
	cmp	dword [ebp+want], tk_openblock ; if5
	jnz	expecting_if6else

	push	'['
	call	expected

	jmp	expecting_if1end

expecting_if6else:
	cmp	dword [ebp+want], tk_closeblock ; if6
	jnz	expecting_if7else

	push	']'
	call	expected

	jmp	expecting_if1end

expecting_if7else:
	cmp	dword [ebp+want], tk_openbrace ; if7
	jnz	expecting_if8else

	push	'{'
	call	expected

	jmp	expecting_if1end

expecting_if8else:
	cmp	dword [ebp+want], tk_closebrace ; if8
	jnz	expecting_if9else

	push	'}'
	call	expected

	jmp	expecting_if1end

expecting_if9else:
	cmp	dword [ebp+want], tk_comma ; if9
	jnz	expecting_if10else

	push	','
	call	expected

	jmp	expecting_if1end

expecting_if10else:
	push	expecting_msg ; "expecting a different token"
	call	preerror

expecting_if1end:
	call	NextTok

	pop	ebp
	ret	4
  endp
; ---------------------------------------------------------------------------
; ���������� ������ �����������
; ---------------------------------------------------------------------------
  proc	internalerror
  str_		  = 8

	push	ebp
	mov	ebp, esp

	inc	[error]

	invoke	wsprintfA, mapstr, internalerror_str1, currentfilename, [linenumber], [error], [ebp+str_]
	add	esp, 18h

	mov	eax, mapstr
	call	WRITESTR

	invoke	wsprintfA, mapstr, internalerror_str2, string
	add	esp, 0Ch

	mov	eax, mapstr
	call	WRITESTR

	invoke	wsprintfA, mapstr, internalerror_str3, [tok], [post], [number]
	add	esp, 14h

	mov	eax, mapstr
	call	WRITESTR

	invoke	wsprintfA, mapstr, internalerror_str4, string2
	add	esp, 0Ch

	mov	eax, mapstr
	call	WRITESTR

	invoke	wsprintfA, mapstr, internalerror_str5, [tok2], [post2], [number2]
	add	esp, 14h

	mov	eax, mapstr
	call	WRITESTR

	cmp	[makemapfile], 0  ; if1
	jz	internalerror_if1end

internalerror_if1end:
;        invoke  ExitProcess, e_internalerror
	call	RestoreState

	pop	ebp
	ret    4
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	datatype_expected
	push	datatype_expected_msg ; "byte, word, short, char, dword or int e"...
	call	preerror

	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	idexpected
	push	idexpected_str1 ; "undefined 'identifier' expected"
	call	preerror

	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	unexpectedeof
	push	unexpectedeof_str1 ; "unexpected END OF FILE"
	call	preerror

	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	localunresolved
  holdstr	  = -50h
  str_		  = 8

	enter	50h, 0		; byte holdstr[80];

	lea	eax, [ebp+holdstr]
	invoke	wsprintfA, eax, localunresolved_msg, [ebp+str_]
	add	esp, 0Ch

	lea	eax, [ebp+holdstr]
	push	eax
	call	preerror

	leave
	ret	4
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	illegaloperand
	push	illegaloperand_str ; "ASM: Illegal operand\r\n"
	call	preerror
	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	toomuchoperands
	push	toomuchoperands_str
	call	preerror
	ret
  endp
; ---------------------------------------------------------------------------
; ��������� token �������� ;
; ---------------------------------------------------------------------------
  proc	NextSemiNext
	call	NextTok

	call	SemiNext

	ret
  endp
; ---------------------------------------------------------------------------
; ����������� ; � token �� ���
; ---------------------------------------------------------------------------
  proc	SemiNext
	cmp	[tok], tk_semicolon ; if1
	jz	SemiNext_if1end

	push	';'
	call	expected

SemiNext_if1end:
	call	NextTok

	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	undefinederror
  holdstr	  = -50h

	enter	50h, 0		; byte holdstr[80];

	lea	eax, [ebp+holdstr]
	invoke	wsprintfA, eax, undefinederror_str1, string
	add	esp, 0Ch

	lea	eax, [ebp+holdstr]
	push	eax
	call	preerror

	leave
	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	swaperror
	push	swaperror_str1 ; ---- ������������ ������� ��� swap
	call	preerror
	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	operatorexpected
	push	operatorexpected_str1 ; "operator identifier expected"
	call	preerror
	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	outofmemory
	push	outofmemory_msg ; "Compiler out of memory"
	call	preerror

	cmp	[makemapfile], 0
	jz	outofmemory_ifend

outofmemory_ifend:
;        invoke  ExitProcess, e_outofmemory
	call	RestoreState

	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	numexpected
	push	numexpected_msg ; "'number' expected"
	call	preerror
	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	expected
hstr		= -50h
dw_ch		= 8

	enter	50h, 0

	lea	eax, [ebp+hstr]
	invoke	wsprintfA, eax, expected_msg, [ebp+dw_ch]
	add	esp, 0Ch

	lea	eax, [ebp+hstr]
	push	eax
	call	preerror

	leave
	ret	4
  endp
; ---------------------------------------------------------------------------
; ��������� ������
; ---------------------------------------------------------------------------
  proc	stringexpected
	push	stringexpected_str1
	call	preerror

	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	maxwordpostserror
	push	maxwordpostserror_msg ; "maximum number of word post location re"...
	call	preerror
	ret
  endp
; ---------------------------------------------------------------------------
;
; ---------------------------------------------------------------------------
  proc	expectederror
  hstr		  = -50h
  str_		  = 8

	enter	50h, 0

	cmp	[displaytokerrors], 0 ; if
	jz	short expectederror_ifend

	lea	eax, [ebp+hstr]
	invoke	wsprintfA, eax, expectederror_msg, dword [ebp+str_]
	add	esp, 0Ch

	lea	eax, [ebp+hstr]
	push	eax
	call	preerror

expectederror_ifend:
	leave
	ret	4
  endp
; ---------------------------------------------------------------------------
; �������� �� �����
; ---------------------------------------------------------------------------
  proc	IsNumber
dw_ch		= 8

	push	ebp
	mov	ebp, esp

	cmp	byte [ebp+dw_ch], '0' ; if1
	jb	IsNumber_if1else

	cmp	byte [ebp+dw_ch], '9'
	jbe	IsNumber_if1end

IsNumber_if1else:
	xor	eax, eax

	jmp	IsNumber_end

IsNumber_if1end:
	mov	eax, 1

IsNumber_end:
	pop	ebp
	ret	4
  endp
; ---------------------------------------------------------------------------
; ���������� ����� ������ - �����
; ---------------------------------------------------------------------------
  proc	toomanyerrors
	call	RestoreState
;        invoke  ExitProcess, e_toomanyerrors
  endp
; ---------------------------------------------------------------------------
; ��������� ������ ���� ����� � �������� ����
; ---------------------------------------------------------------------------
  proc	WriteZero
  c		  = -4
  size		  = 8

	enter	4, 0

	mov	byte [ebp+c], 0

WriteZero_loopstart:
	lea	eax, [ebp+c]
	push	1
	push	eax
	call	OnWriteEXEFile

	dec	dword [ebp+size]
	jnz	WriteZero_loopstart ; condition

	leave
	ret	4
  endp