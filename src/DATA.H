//===== ���������� ����������
// ---- ���������� �� tokens
dword tok=0;
dword type=0;
dword src=0;
dword post=0;
dword number=0;
byte string[STRLEN]=0;
dword tok2=0;
dword type2=0;
dword src2=0;
dword post2=0;
dword number2=0;
dword modline=0;
byte string2[STRLEN]=0;
// ---- ������	 ����������� �����
dword currmod=0;	// ����� �������� �����
dword displaytokerrors=1;// ���� ������ ������� � �� �� ���
dword error=0;		// �������� ����� ������� �����
dword maxerrors = 16; // ������ �� ���������� ������
dword  makemapfile=0; // ���� ������� � MAP-�����
dword  dbg=0; 	// ���� ������� � TDS-����� ��� TD32
dword dbginfo=0;	// ��������� �� ����� dbg ����������� (��.�������� � enum dbg_...)
dword dbgs=0; 	// ������� ��������� � dbginfo
dword input=0;		// ��������� �� ������ ������������� ������ � ������� ������
dword inptr=0;		// ��������� �� ����� ������ � ������ input
dword inptr2=0; 	// ����� inptr
dword endoffile=0;	// ���� ����� �����
dword totallines=1; // ����� ���������� ����������������� �����
dword linenumber=0; // ����� ������� ������
dword linenum2=0; // ����� ��������� ������
dword list=0; 	// ���� ������ ��������
dword label=0;		// �������� ����� ��� ��������� �����
dword mapfile=0;	// Handle ��� MAP �����
dword module=0; 	// ������� ����������������� �������
dword outptr=0; 	// ������ � output
dword output=0; 	// ��������� �� ����� � �����
dword localsize=0;	// ������ ����� ��� ���������� �����������
dword posttype=0; // ��������� �� ��� POST
dword postloc=0;	// ��������� �� ��������� � output
dword postnum=0;	// ��������� �� �������� �� postloc
dword posts=0;		// ����� ������� ������ � posttype, postloc � postnum
dword postsize=0; // ��������� ������ ���� post-����������
dword poststrptr=MAXDATA-1; // ������ ��� ������ post-�����
dword procedure_start=0;// ����� ������ ���������
dword runfilesize=0;
dword startptr=0; // ��������� �� main()
dword treestart=0;	// ��������� �� ������ ������ ���������������
dword treeptr=0;	// ��������� �� ������	������ � ������ ����� �������
dword locallist = NULL; // ��������� �� ������ ������ ��������� ����������
dword localptr=NULL;	// ��������� �� ������� ������ � ������ ���������
dword DLLcount=0; // ������� ��������������� DLL
dword APIcount=0; // ������� ������������ API
dword importFlag=0; // ���� ������� �� DLL
dword DLLlist[MAXDLLS];
byte currentfilename[FILENAMESIZE] = 0;
byte inputfile[FILENAMESIZE]=0;
byte rawfilename[FILENAMESIZE]=0;
byte mapstr[120]=0;
// ---- DOS&PE headers
word exeheader[34]={
	 0x5A4D,0x40,1,0,2,0,0xFFFF,0,
	 0,0,0,0,0x40,0,0,0,
	 0xC88C,0xD88E,0x0FBA,0xB400,0xCD09,0xB821,0x4C00,0x21CD,
	 0x6957,0x336E,0x2032,0x6E6F,0x796C,0x2421,0x40,0,
	 0x4550,0};
// PE Header
//unsigned dword PEsign = 0x4550;
word PEmachine=0x14C; // target machine = Intel 386
word PEnSections=1; 	// XAC - only .text!!! number of sections in Sections table
word PEDate=0;
word PEtime=0;
dword PEpSymbolTable=0;  // Offset within COFF file of the symbol table
dword PEnSymbols=0;  // number of entries in the symbol table
word PEOptHeaderSize=0xE0; // Size of optional header
word PECharacteristics=0x30E; //0x30E 32-bit+...
// Optional header (only in EX image)
word OptMagic=0x10B;	// normal executable
byte OptLmajor=2; 		 // Linker major version number
byte OptLminor=0x37;		 // Linker minor version number
dword OptCodeSize=0;	 // Size of the code section
dword OptInitDataSize=0; // Size of the initialized data section
dword OptUninitDataSize=0;// Size of the uninitialized data section (BSS)
dword OptEntryPointRVA=0x1000;// Address of entry point, relative to image base
dword OptBaseOfCode=0x1000;// Address realtive to image base
dword OptBaseOfData=0;//Address realtive to image base
dword OptImageBase=0x00400000;// Preferred address of first byte of image
dword OptSectionAlignment=0x1000;
dword OptFileAlignment=0x200;
word OptOSmajor=1;
word OptOSminor=0;
dword OptUserVersion=0;
word OptSubSysMajor=4;
word OptSubSysMinor=0;
dword OptReserved=0;
dword OptImageSize=0x0;  // Size of image, including all headers
dword OptHeaderSize=0x200;	 //DOSheader+PEheader+ObjectTable
dword OptFileChecksum=0; // Image file checksum
word OptSubSystem=3;	// 2-GUI; 3-console
word OptDLLflag=0;
dword OptStackReserveSize=0x100000;
dword OptStackCommitSixe=0x1000;
dword OptHeapReserveSize=0x100000;
dword OptHeapCommitSize=0x1000;
dword OptLoaderSize=0;
dword OptNumOfDataDirectories=16;
// Optional header Data Directories
dword OptExportTableAdr=0;
dword OptExportTableSize=0;
dword OptImportTableAdr=0;
dword OptImportTableSize=0;
dword OptResourceTableAdr=0;
dword OptResourceTablesize=0;
dword OptExceptionTableAdr=0;
dword OptExceptionTableSize=0;
dword OptSecurityTableAdr=0;
dword OptSecurityTableSize=0;
dword OptBaseRelocationTableAdr=0;
dword OptBaseRelocationTableSize=0;
dword OptDebugAdr=0;
dword OptDebugSize=0;
dword OptCopyrightAdr=0;
dword OptCopyrightSize=0;
dword OptGlobalPtrAdr=0;
dword OptGlobalPtrSize=0;
dword OptTLStableAdr=0;
dword OptTLStablesize=0;
dword OptLoadConfigTableAdr=0;
dword OptLoadConfigTableSize=0;
dword OptReserved2[10]={0,0,0,0,0,0,0,0,0,0};
// Sections Table
// TEXT section header
byte TxtSectionName[8]=".text";
dword TxtVirtualSize=0;
dword TxtRVAoffset=0x1000;
dword TxtSizeOfRawData=0;
dword TxtPointerToRawData=0x200;
dword TxtPointerToRelocs=0;
dword TxtPointerToLinenumbers=0;
word TxtNumberOfRelocs=0;
word TxtNumberOfLinenumbers=0;
dword TxtSectionFlags=0xE00000E0; 	 // can be executed + contain executable code
// ---- ������	������ ������
dword stdout; 	// Handle to stdout
byte cha,cha2;
dword numberofids;
dword current_proc_type;	// ��� ������� ��������� (cpt_near, cpt_far)
dword returntype; 	// ��� ������������� �������� (void, byte, word, ...)
dword paramsize;
dword relation;
dword startlabel,endlabel;
byte modules[MAXMDL*FILENAMESIZE];
dword St_Mnemonics[26];
dword St_Registers[26];
dword St_Directives[26];
dword St_Sizes[26];
byte Buffer16[64]; // ����� ��� ���������	�����

