//===== ���������� ���������
#define SHOW "WRITESTR"
// ----
#define STD_INPUT_HANDLE	-10
#define STD_OUTPUT_HANDLE	-11
#define STD_ERROR_HANDLE	-12
#define CREATE_NEW		1
#define CREATE_ALWAYS 		2
#define OPEN_EXISTING 		3
#define OPEN_ALWAYS 		4
#define TRUNCATE_EXISTING 	5
// ----
#define GENERIC_READ	0x80000000
#define GENERIC_WRITE	0x40000000
#define GENERIC_EXECUTE 0x20000000
#define GENERIC_ALL	0x10000000
// ---- ��������� �����������
#define _END	0x1B
#define FILENAMESIZE 80
#define IDLENGTH 65 // ����� ����� ��������������, ������� NULL-����������
#define MAXDATA 512000	// ������ ������� ��� ��� � ������
#define MAXDBGS 50000 // ������ ������� ��� ���������  ����������
#define MAXDLLS 100
#define MAXINPUT 512000 // �����������	������ �������� �����
#define MAXMDL	100	// ������������ ����� includes
#define MAXPOSTS 150000 // ������ ������� ��� POST ���������
#define NULL	0
#define SORTSIZE 20
#define STRLEN 1000 // ����� ������ ��� token
// ------------------------------------------
//===== ������������ ���������
// ---- ���� ��������
enum{ e_ok, e_outofmemory, e_cannotopeninput, e_toomanyerrors,
	e_internalerror, e_noinputspecified, e_unknowncommandline,
	e_extract, e_cannotopenmapfile, e_someerrors };
// ---- ���� tokens tk_xxxx
enum{
tk_eof,tk_number,tk_string,tk_var,
tk_id,
tk_db,tk_dw,tk_dd,tk_dq,	// ���� ����������
tk_byte,tk_char,tk_word,tk_short,
tk_dword,tk_int,
tk_enum,			// ������������ ���������
tk_struc, 	// ����������� ���������
tk_if,tk_else,tk_endif, // �������� ����������
tk_include,tk_define, // ��������� �� ��//����������� ���������
tk_import,		// ������ �� DLL
tk_map, 		// ��������� MAP-�����
tk_debug, 	// ��������� ���������� ����������
tk_list,		// ������ ASM-��������
tk_dll, 		// ��������� DLL-�����
tk_loop,tk_return,tk_do,
tk_while,tk_void,
tk_continue,tk_break,
tk_docase,tk_case,tk_default,
tk_carryflag,tk_extract,tk_FALSE,tk_from,
tk_notcarryflag,tk_notoverflow,tk_overflow,tk_TRUE,
tk_zeroflag,tk_notzeroflag,
tk_assign,tk_swap,
tk_minus,tk_plus,
tk_minusminus,tk_plusplus,tk_mult,tk_div,tk_mod,
tk_multminus,tk_divminus,tk_modminus,tk_rr,tk_ll,
tk_rrminus,
tk_llminus,
tk_minusequals,tk_plusequals,tk_rrequals,tk_llequals,
tk_or,tk_and,tk_xor,tk_not,
tk_orminus,
tk_andminus,tk_xorminus,
tk_orequals,tk_andequals,tk_xorequals,
tk_equalto,
tk_notequal,tk_greater,tk_greaterequal,tk_less,
tk_lessequal,
tk_oror,tk_andand,tk_openbrace,tk_closebrace,
tk_openbracket,tk_closebracket,tk_openblock,tk_closeblock,
tk_colon,tk_semicolon,tk_comma,tk_period,
tk_dollar,tk_question,tk_tilda,
tk_reg,tk_controlreg,tk_debugreg,tk_testreg,tk_seg,
tk_undefproc,tk_proc,
tk_postnumber,
tk_local,tk_locallabel,tk_param,tk_locnumber,
tk_DLL,tk_API,tk_directive,tk_command,tk_mnemonics,
tokens
};
// ---- ��������� ���� ��������
enum{ cpt_near, cpt_far };
// ---- ��������� POST �������
enum { POST_CALL=1, POST_API, POST_DATA, POST_LOC, POST_STR };
// ---- �������� ��������� � dbginfo: dbg_...
enum{ dbg_mod=0,	// ����� ������
	dbg_line=4, // ����� �����	� ������
	dbg_loc=8,	// ������� � EXE-�����
	dbg_size=12}; // ������ ���������
// ---- �������� ��������� idrec
enum{ left=0, 	// ��������� �� idrec
	right=4,		// ��������� �� idrec
	recid=8,		// ��� ��������������
	newid=recid+4,	// ��������� �� ����� ��� recid
	rectok=newid+4, // ����� token
	rectype=rectok+4, // ��� token: void, byte, char, word, ...
	recsrc=rectype+4, // �������� �����
	recpost=recsrc+4, // ���� POST ����������
	recnumber=recpost+4, // �������� ��� offset � recsrc
	recmodline=recnumber+4,// ����� ������ � ������
	recsize=recnumber+4};// ������ ���������
// ---- �������� ��������� localrec
enum{ localnext=0,		// ��������� �� ��������� localrec
	localid=4,		// ��� ��������  ����������
	localtok=localid+IDLENGTH,// �������� token
	localtype=localtok+4, // ��� ����������
	localnumber=localtype+4,	// ������� � �����
	local_size=localnumber+4};	// ������ ���������

