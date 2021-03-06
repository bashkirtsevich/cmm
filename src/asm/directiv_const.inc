Directives db 'IF', 0, 'ELSE', 0, 'ENDIF', 0	 ; �������� ����������
	   db 'INCLUDE', 0, 'DEFINE', 0 	 ; ��������� �����/����������� ���������
if DEFINE_MAKE_EXE
	   db 'IMPORT', 0			 ; ������ �� DLL �� ����� API
	   db 'IMPORTN', 0			 ; ������ �� DLL �� ������ API
end if
	   db 'MAP', 0				 ; ��������� MAP-�����
	   db 'DEBUG', 0			 ; ��������� ���������� ����������
	   db 'LIST', 0 			 ; ������ ASM-��������
	   db 'DLL', 0				 ; ��������� DLL-�����
	   db 'DB', 0, 'DW', 0, 'DD', 0 	 ; ���� ����������
	   db 'BYTE', 0, 'CHAR', 0, 'WORD', 0, 'SHORT', 0, 'DWORD', 0, 'INT', 0
	   db 'ENUM', 0 			 ; ������������ ���������
	   db 'STRUC', 0			 ; ����������� ���������
	   db 'CYCLE', 0, 'RETURN', 0
	   db 'WHILE', 0, 'DO', 0, 'INLINE', 0
	   db 'CONTINUE', 0, 'BREAK', 0
	   db 'DOCASE', 0, 'CASE', 0, 'DEFAULT', 0
	   db 'CARRYFLAG', 0, 'EXTRACT', 0, 'FROM', 0
	   db 'NOTCARRYFLAG', 0, 'NOTOVERFLOW', 0, 'OVERFLOW', 0
	   db 'ZEROFLAG', 0, 'NOTZEROFLAG', 0, _END, 0

;===== ������� �������������� ��������
; �������� ����������
Jmp_Directives dd EMPTY 	; if
	       dd EMPTY 	; else
	       dd EMPTY 	; endif
; ��������� �� ��
	       dd DirInclude	; include
	       dd DirDefine	; define
; ������ �� DLL
if DEFINE_MAKE_EXE
	       dd DirImport	; import
	       dd DirImportN	; importN
end if
; ��������� MAP-�����
	       dd DirMap	; map
; ��������� ���������� ����������
	       dd EMPTY 	; debug
; ������ ASM-��������
	       dd DirList	; list

; ��������� DLL-�����
	       dd EMPTY 	; dll
; ���� ����������
	       dd EMPTY 	; db
	       dd EMPTY 	; dw
	       dd EMPTY 	; dd

	       dd EMPTY 	; byte
	       dd EMPTY 	; char
	       dd EMPTY 	; word
	       dd EMPTY 	; short
	       dd EMPTY 	; dword
	       dd EMPTY 	; int
; ������������ ���������
	       dd DirEnum	; enum
; ����������� ���������
	       dd EMPTY 	; struc

	       dd EMPTY 	; cycle
	       dd EMPTY 	; return

	       dd EMPTY 	; while
	       dd EMPTY 	; do
	       dd EMPTY 	; inline

	       dd EMPTY 	; continue
	       dd EMPTY 	; break

	       dd EMPTY 	; docase
	       dd EMPTY 	; case
	       dd EMPTY 	; default

	       dd EMPTY 	; CARRYFLAG
	       dd EMPTY 	; extract
	       dd EMPTY 	; from

	       dd EMPTY 	; NOTCARRYFLAG
	       dd EMPTY 	; NOTOVERFLOW
	       dd EMPTY 	; OVERFLOW

	       dd EMPTY 	; ZEROFLAG
	       dd EMPTY 	; NOTZEROFLAG

;===== ������� �������������� ������
; �������� ����������
Jmp_Commands dd CmdIf		; if
	     dd CmdElse 	; else
	     dd EMPTY		; endif
; ��������� �� ��
	     dd EMPTY		; include
	     dd EMPTY		; define
; ������ �� DLL
if DEFINE_MAKE_EXE
	     dd EMPTY		; import
	     dd EMPTY		; importN
end if
; ��������� MAP-�����
	     dd EMPTY		; map
; ��������� ���������  ����������
	     dd EMPTY		; debug
; ������ ASM-� ������
	     dd EMPTY		; list
; ��������� DLL-�����
	     dd EMPTY		; dll
; ���� ����������
	     dd CmdDb		; db
	     dd CmdDw		; dw
	     dd CmdDd		; dd

	     dd CmdByte 	; byte
	     dd CmdChar 	; char
	     dd CmdWord 	; word
	     dd CmdShort	; short
	     dd CmdDword	; dword
	     dd CmdInt		; int
; ������������ ���������
	     dd CmdEnum 	; enum
; ����������� ���������
	     dd EMPTY		; struc

	     dd CmdCycle	; cycle
	     dd CmdReturn	; return

	     dd CmdWhile	; while
	     dd CmdDo		; do
	     dd EMPTY		; inline

	     dd CmdContinue	; continue
	     dd CmdBreak	; break

	     dd CmdDoCase	; docase
	     dd CmdCase 	; case
	     dd CmdDefault	; default

	     dd CmdCarryFlag	; CARRYFLAG
	     dd EMPTY		; extract
	     dd EMPTY		; from

	     dd CmdNotCarryFlag ; NOTCARRYFLAG
	     dd CmdNotOverflow	; NOTOVERFLOW
	     dd CmdOverflow	; OVERFLOW

	     dd CmdZeroFlag	; ZEROFLAG
	     dd CmdNotZeroFlag	; NOTZEROFLAG

