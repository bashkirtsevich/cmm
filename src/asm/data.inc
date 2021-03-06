; ������ ������� �������
OnLoadFileProcPtr	    dd 0 ; ������� �������� ����������� �����
OnWriteEXEFileProcPtr	    dd 0 ; ������ ������������ ����
OnWriteMAPFileProcPtr	    dd 0 ; ������ MAP �����
OnWriteMessageProcPtr	    dd 0 ; ����� ���������
; ���������� ����������
; ���������� �� tokens
tok			    dd 0
type			    dd 0
src			    dd 0
post			    dd 0
number			    dd 0
string			    db STRLEN dup 0
tok2			    dd 0
type2			    dd 0
src2			    dd 0
post2			    dd 0
number2 		    dd 0
modline 		    dd 0
string2 		    db STRLEN dup 0
; ---- ������   ����������� �����
currmod 		    dd 0; ����� �������� �����
displaytokerrors	    dd 1; ���� ������ ������� � �� �� ���
error			    dd 0; �������� ����� ������� �����
maxerrors		    dd 16;������ �� ���������� ������
makemapfile		    dd 0; ���� ������� � MAP-�����
dbg			    dd 0; ���� ������� � TDS-����� ��� TD32
dbginfo 		    dd 0; ��������� �� ����� dbg ����������� (��.�������� � enum dbg_...)
dbgs			    dd 0; ������� ��������� � dbginfo
input			    dd 0; ��������� �� ������ ������������� ������ � ������� ������
inptr			    dd 0; ��������� �� ����� ������ � ������ input
inptr2			    dd 0; ����� inptr
endoffile		    dd 0; ���� ����� �����
totallines		    dd 1; ����� ���������� ����������������� �����
linenumber		    dd 0; ����� ������� ������
linenum2		    dd 0; ����� ��������� ������
list			    dd 0; ���� ������ ��������
label_			    dd 0; �������� ����� ��� ��������� �����
module			    dd 0; ������� ����������������� �������
outptr			    dd 0; ������ � output
output			    dd 0; ��������� �� ����� � �����
localsize		    dd 0; ������ ����� ��� ���������� �����������
posttype		    dd 0; ��������� �� ��� POST
postloc 		    dd 0; ��������� �� ��������� � output
postnum 		    dd 0; ��������� �� �������� �� postloc
posts			    dd 0; ����� ������� ������ � posttype, postloc � postnum
postsize		    dd 0; ��������� ������ ���� post-����������
poststrptr		    dd MAXDATA-1; ������ ��� ������ post-�����
procedure_start 	    dd 0; ����� ������ ���������
runfilesize		    dd 0;
startptr		    dd 0; ��������� �� main()
treestart		    dd 0; ��������� �� ������ ������ ���������������
treeptr 		    dd 0; ��������� �� ������  ������ � ������ ����� �������
locallist		    dd 0; ��������� �� ������ ������ ��������� ����������
localptr		    dd 0; ��������� �� ������� ������ � ������ ���������
DLLcount		    dd 0; ������� ��������������� DLL
APIcount		    dd 0; ������� ������������ API
importFlag		    dd 0; ���� ������� �� DLL
DLLlist 		    dd MAXDLLS dup 0
currentfilename 	    db FILENAMESIZE dup 0 ; ������
mapstr			    db 120 dup 0

if DEFINE_MAKE_EXE
; ---- DOS&PE headers
exeheader dw 0x5A4D,0x40,1,0,2,0,0xFFFF,0,\
	     0,0,0,0,0x40,0,0,0,\
	     0xC88C,0xD88E,0x0FBA,0xB400,0xCD09,0xB821,0x4C00,0x21CD,\
	     0x6957,0x336E,0x2032,0x6E6F,0x796C,0x2421,0x40,0,\
	     0x4550,0

; PE Header
;unsigned PEsign  dd  0x4550;
PEmachine			dw 0x14C	; target machine  dd  Intel 386
PEnSections			dw 1		; XAC - only .text!!! number of sections in Sections table
PEDate				dw 0
PEtime				dw 0
PEpSymbolTable			dd 0		; Offset within COFF file of the symbol table
PEnSymbols			dd 0		; number of entries in the symbol table
PEOptHeaderSize 		dw 0xE0 	; Size of optional header
PECharacteristics		dw 0x30E	;0x30E 32-bit+...
; Optional header (only in EX image)
OptMagic			dw 0x10B	; normal executable
OptLmajor			db 2		; Linker major version number
OptLminor			db 0x37 	; Linker minor version number
OptCodeSize			dd 0		; Size of the code section
OptInitDataSize 		dd 0		; Size of the initialized data section
OptUninitDataSize		dd 0		; Size of the uninitialized data section (BSS)
OptEntryPointRVA		dd 0x1000	; Address of entry point, relative to image base
OptBaseOfCode			dd 0x1000	; Address realtive to image base
OptBaseOfData			dd 0		; Address realtive to image base
OptImageBase			dd 0x00400000	; Preferred address of first byte of image
OptSectionAlignment		dd 0x1000	;
OptFileAlignment		dd 0x200	;
OptOSmajor			dw 1		;
OptOSminor			dw 0		;
OptUserVersion			dd 0		;
OptSubSysMajor			dw 4		;
OptSubSysMinor			dw 0		;
OptReserved			dd 0		;
OptImageSize			dd 0x0		; Size of image, including all headers
OptHeaderSize			dd 0x200	; DOSheader+PEheader+ObjectTable
OptFileChecksum 		dd 0		; Image file checksum
OptSubSystem			dw 3		; 2-GUI; 3-console
OptDLLflag			dw 0		;
OptStackReserveSize		dd 0x100000	;
OptStackCommitSixe		dd 0x1000	;
OptHeapReserveSize		dd 0x100000	;
OptHeapCommitSize		dd 0x1000	;
OptLoaderSize			dd 0		;
OptNumOfDataDirectories 	dd 16		;
; Optional header Data Directories
OptExportTableAdr		dd 0		;
OptExportTableSize		dd 0		;
OptImportTableAdr		dd 0		;
OptImportTableSize		dd 0		;
OptResourceTableAdr		dd 0		;
OptResourceTablesize		dd 0		;
OptExceptionTableAdr		dd 0		;
OptExceptionTableSize		dd 0		;
OptSecurityTableAdr		dd 0		;
OptSecurityTableSize		dd 0		;
OptBaseRelocationTableAdr	dd 0		;
OptBaseRelocationTableSize	dd 0		;
OptDebugAdr			dd 0		;
OptDebugSize			dd 0		;
OptCopyrightAdr 		dd 0		;
OptCopyrightSize		dd 0		;
OptGlobalPtrAdr 		dd 0		;
OptGlobalPtrSize		dd 0		;
OptTLStableAdr			dd 0		;
OptTLStablesize 		dd 0		;
OptLoadConfigTableAdr		dd 0		;
OptLoadConfigTableSize		dd 0		;
OptReserved2			dd 0,0,0,0,0,0,0,0,0,0
; Sections Table
; TEXT section header
TxtSectionName			db '.text', 0, 0, 0
TxtVirtualSize			dd 0		;
TxtRVAoffset			dd 0x1000	;
TxtSizeOfRawData		dd 0		;
TxtPointerToRawData		dd 0x200	;
TxtPointerToRelocs		dd 0		;
TxtPointerToLinenumbers 	dd 0		;
TxtNumberOfRelocs		dw 0		;
TxtNumberOfLinenumbers		dw 0		;
TxtSectionFlags 		dd 0xE00000E0	; can be executed + contain executable code
else
; �� �� ���� ����?
OptBaseOfCode			dd ?		; Address realtive to image base
OptImageBase			dd ?		; Preferred address of first byte of image
end if
; ---- ������  ������ ������
cha				db ?
cha2				db ?
numberofids			dd ?
current_proc_type		dd ?		; ��� ������� ��������� (cpt_near, cpt_far)
returntype			dd ?		; ��� ������������� �������� (void, byte, word, ...)
paramsize			dd ?
relation			dd ?		; ���� ��������� � ���������
startlabel			dd ?
endlabel			dd ?
modules 			db MAXMDL*FILENAMESIZE dup ?

St_Mnemonics			dd 26 dup ?;
St_Registers			dd 26 dup ?;
St_Directives			dd 26 dup ?;
St_Sizes			dd 26 dup ?;
Buffer16			db 26 dup ?; ����� ��� ���������� �����

; GENERATE

ACTUALMNEMDESC	dd ?; ��������� �� �������� ������� ���������
ACTUALMNEMONIC	db ?; ��� ������� ���������
OPERANDSIZE	db ?; ������ ��������(B,W,D,BW,WB,DW)
OPDESC		db 3 dup ?; ���� ��������� (R,E,D,CC,SR,TR,DR,CR)
OPDATA		db 3 dup ?; ���������� �� �������� (RG,CO,EA)
OPCONST 	dd 3 dup ?; �������� ��������� � ��������
OPCONSTFLAG	db 3 dup ?; ������� ��������� � ��������
OPCONSTSIZE	db 3 dup ?; ����������� ��������� � ��������
OPPOST		db 3 dup ?;
OPPOSTREF	dd 3 dup ?;
PFLAG		db ?; �������� P-����� (����� ������).
RANDFLAG	db ?; TRUE: RAND: ��������� ������� 0x66
ADDRFLAG	db ?; TRUE: ADDR: ��������� ������� 0x67
WBIT		db ?; ��� WORD � OPCODE
DBIT		db ?; ��� DIRECTION � OPCODE
OPLENGTH	db ?; ���������� ��������� � ������� ����������
EA_M		db ?; ���� XRM
EA_X		db ?;
EA_R		db ?; R NIBBLE IN XRM BYTE.
EA_S		db ?; SEGMENT REGISTER
EA_R2		db ?; BASE REGISTER
EA_I		db ?; INDEX REGISTER
EADESC		db 2 dup ?; CONTAINS INFORMATION ABOUT AN PARSED EFFECTIVE ADRESS.
EALENGTH	db ?; ���������� ��������� � EA
EA_SIBFLAG	db ?; ���� ������� Sib � 32-������ EA
EA_SCALING	db ?; ������ � Sib
EA_SCALE	dd ?; �������� ��� Scale � Sib
EADescInd	dd ?; ������ � EADESC
SEGREGISTER	db ?; SEGMENT OVERRIDE PREFIX NEEDED OR 0.
SYSRNUM 	db ?; CONTROL/DEBUG/TASKREG INDEX
SYSRTYPE	db ?; SYSTEM REGISTER TYPE (CR,CR4,DR,TR)
SYSRCODE	db ?; ENCODED REGISTER TYPE
OVERRIDE	db ?; SEGMENT OVERRIDE PREFIX NEEDED OR 0.
opDescInd	dd ?;
prevTok 	dd ?;

SavedESPState	dd ?; ��������� �������� ESP � ������������ ���������� � ����� � ������� ������

CompileAll_str1 db 'main',0 ; ����� � ����������, ��� ��� �����-�� �������� ��� �������� ������������ � ��