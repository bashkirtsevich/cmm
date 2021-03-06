; ��������� ����������
_END		= 1Bh
FILENAMESIZE	= 80
IDLENGTH	= 65		; ����� ����� ��������������, ������� NULL-����������
MAXDATA 	= 512000	; ������ ������� ��� ��� � ������
MAXDBGS 	= 50000 	; ������ ������� ��� ���������  ����������
MAXDLLS 	= 100
MAXINPUT	= 512000	; �����������  ������ �������� �����
MAXMDL		= 100		; ������������ ����� includes
MAXPOSTS	= 150000	; ������ ������� ��� POST ���������
SORTSIZE	= 20
STRLEN		= 1000		; ����� ������ ��� token

; ������������ ���������
; ���� ��������
e_ok			= 0
e_outofmemory		= 1
e_cannotopeninput	= 2
e_toomanyerrors 	= 3
e_internalerror 	= 4
e_noinputspecified	= 5
e_unknowncommandline	= 6
e_extract		= 7
e_cannotopenmapfile	= 8
e_someerrors		= 9

; ---- ���� tokens tk_xxxx
tk_eof			= 0
tk_number		= 1
tk_string		= 2
tk_var			= 3

tk_id			= 4
; ���� ����������
tk_db			= 5
tk_dw			= 6
tk_dd			= 7
tk_dq			= 8

tk_byte 		= 9
tk_char 		= 10
tk_word 		= 11
tk_short		= 12

tk_dword		= 13
tk_int			= 14

tk_enum 		= 15 ; ������������ ���������

tk_struc		= 16 ; ����������� ���������

tk_if			= 17
tk_else 		= 18
tk_endif		= 19 ; �������� ����������

tk_include		= 20 ; ��������� �����
tk_define		= 21 ; ����������� ���������

tk_import		= 22 ; ������ �� DLL

tk_map			= 23 ; ��������� MAP-�����

tk_debug		= 24 ; ��������� ���������� ����������

tk_lis			= 25 ; ������ ASM-��������

tk_dll			= 26 ; ��������� DLL-�����

tk_loop 		= 27
tk_return		= 28
tk_do			= 29

tk_while		= 30
tk_void 		= 31

tk_continue		= 32
tk_break		= 33

tk_docase		= 34
tk_case 		= 35
tk_default		= 36

tk_carryflag		= 37
tk_extract		= 38
tk_FALSE		= 39
tk_from 		= 40

tk_notcarryflag 	= 41
tk_notoverflow		= 42
tk_overflow		= 43
tk_TRUE 		= 44

tk_zeroflag		= 45
tk_notzeroflag		= 46

tk_assign		= 47
tk_swap 		= 48

tk_minus		= 49
tk_plus 		= 50

tk_minusminus		= 51
tk_plusplus		= 52
tk_mult 		= 53
tk_div			= 54
tk_mod			= 55

tk_multminus		= 56
tk_divminus		= 57
tk_modminus		= 58
tk_rr			= 59
tk_ll			= 60

tk_rrminus		= 61

tk_llminus		= 62

tk_minusequals		= 63
tk_plusequals		= 64
tk_rrequals		= 65
tk_llequals		= 66

tk_or			= 67
tk_and			= 68
tk_xor			= 69
tk_not			= 70

tk_orminus		= 71

tk_andminus		= 72
tk_xorminus		= 73

tk_orequals		= 74
tk_andequals		= 75
tk_xorequals		= 76

tk_equalto		= 77

tk_notequal		= 78
tk_greater		= 79
tk_greaterequal 	= 80
tk_less 		= 81

tk_lessequal		= 82

tk_oror 		= 83
tk_andand		= 84
tk_openbrace		= 85
tk_closebrace		= 86

tk_openbracket		= 87
tk_closebracket 	= 88
tk_openblock		= 89
tk_closeblock		= 90

tk_colon		= 91
tk_semicolon		= 92
tk_comma		= 93
tk_period		= 94

tk_dollar		= 95
tk_question		= 96
tk_tilda		= 97

tk_reg			= 98
tk_controlreg		= 99
tk_debugreg		= 100
tk_testreg		= 101
tk_seg			= 102

tk_undefproc		= 103
tk_proc 		= 104

tk_postnumber		= 105

tk_local		= 106
tk_locallabel		= 107
tk_param		= 108
tk_locnumber		= 109

tk_DLL			= 110

if DEFINE_MAKE_EXE
tk_API			= 111
end if

tk_directive		= 112
tk_command		= 113
tk_mnemonics		= 114

tokens			= 115

; ---- ��������� ���� ��������
cpt_near		= 0
cpt_far 		= 1
; ---- ��������� POST �������
POST_CALL	 = 1
POST_API	 = 2
POST_DATA	 = 3
POST_LOC	 = 4
POST_STR	 = 5
; ---- �������� ��������� � dbginfo: dbg_...
dbg_mod 	 = 0 ; ����� ������
dbg_line	 = 4 ; ����� ������ � ������
dbg_loc 	 = 8 ; ������� � EXE-�����
dbg_size	 = 12; ������ ���������
; ---- �������� ��������� idrec
left		 = 0	      ; ��������� �� idrec
right		 = 4	      ; ��������� �� idrec
recid		 = 8	      ; ��� ��������������
newid		 = recid+4    ; ��������� �� ����� ��� recid
rectok		 = newid+4    ; ����� token
rectype 	 = rectok+4   ; ��� token: void, byte, char, word, ...
recsrc		 = rectype+4  ; �������� �����
recpost 	 = recsrc+4   ; ���� POST ����������
recnumber	 = recpost+4  ; �������� ��� offset � recsrc
recmodline	 = recnumber+4; ����� ������ � ������
recsize 	 = recnumber+4; ������ ���������
; ---- �������� ��������� localrec
localnext	= 0		  ; ��������� �� ��������� localrec
localid 	= 4		  ; ��� ��������  ����������
localtok	= localid+IDLENGTH; �������� token
localtype	= localtok+4	  ; ��� ����������
localnumber	= localtype+4	  ; ������� � �����
local_size	= localnumber+4   ; ������ ���������