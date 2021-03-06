byte Directives={
	"IF","ELSE","ENDIF",	// �������� ����������
	"INCLUDE","DEFINE", // ��������� �����/����������� ���������
	"IMPORT", 	// ������ �� DLL �� ����� API
	"IMPORTN",		// ������ �� DLL �� ������ API
	"MAP",			// ��������� MAP-�����
	"DEBUG",		// ��������� ���������� ����������
	"LIST", 		// ������ ASM-��������
	"DLL",			// ��������� DLL-�����
	"DB","DW","DD", 	// ���� ����������
	"BYTE","CHAR","WORD","SHORT","DWORD","INT",
	"ENUM", 		// ������������ ���������
	"STRUC",		// ����������� ���������
	"CYCLE","RETURN",
	"WHILE","DO","INLINE",
	"CONTINUE","BREAK",
	"DOCASE","CASE","DEFAULT",
	"CARRYFLAG","EXTRACT","FROM",
	"NOTCARRYFLAG","NOTOVERFLOW","OVERFLOW",
	"ZEROFLAG","NOTZEROFLAG",_END};
// ----- ��� tokens, �� �������������� ����� ������� ��������������
EMPTY()
{
	WRITESTR(#string);
	WRITESTR("-ToDo\n");
	NextTok();
}

// ---- ���������� ����� �� Jmp_....
dword GetDirAddr(dword table,num)
{
	EAX=num<<2+table;
	EAX=DSDWORD[EAX];
}

// ----- ��������� #define
DirDefine()
byte holdid[IDLENGTH];
dword next;
{
	next=1;
	NextTok();
	if(tok==tk_id){
		lstrcpyA(#holdid,#string);	// ��� ���������
		NextTok();
		IF(tok==tk_eof)  unexpectedeof();
		ELSE IF(tok==tk_number){
			AddConstToTree(#holdid,DoConstLongMath()); next = 0;
		}
		ELSE IF(tok==tk_minus){
			IF(tok2==tk_number) {
				AddConstToTree(#holdid,DoConstLongMath());
				next = 0;
			}
		}
		ELSE IF(tok==tk_undefproc){
			tok = tk_id; AddToTree(#holdid);
		}
		ELSE	AddToTree(#holdid);
	}
	ELSE idexpected();
	IF(next)NextTok();
}

// -- #enum
DirEnum()
dword counter;
byte holdid[IDLENGTH];
{
	counter=0;
	NextTok();
	IF(tok!=tk_openbrace)expected('{');
	for(;;){
		NextTok();
		IF(tok==tk_id){
			lstrcpyA(#holdid,#string);
			IF( tok2 == tk_assign ){
				NextTok(); NextTok();
				IF(tok==tk_number)counter=DoConstLongMath();
				ELSE numexpected();
			}
			AddConstToTree(#holdid,counter);
			counter++;
			CONTINUE;
		}
		IF(tok==tk_comma)CONTINUE;
		IF(tok==tk_semicolon)BREAK;
	}
	NextTok();
}

// ��������� #import
DirImport()
{
	NextTok();
	IF(tok==tk_string)GetImport(1); 	// import �� ����� API-�������
	ELSE stringexpected();
	NextTok();
}

// ��������� #importN
DirImportN()
{
	NextTok();
	IF(tok==tk_string)GetImport(0); 	// import �� ����� API-�������
	ELSE stringexpected();
	NextTok();
}

// ---- ������ �� DLL
GetImport(dword byName)
dword dll;
dword dllpos,base,export,fptr,i,nexports,nsect,delta;
byte path[80],name[120];
dword tok0,type0,src0,post0;
dword number0;
dword ord;
dword pname1,pname2,j;
{
	pname1 = 0; ord=0; importFlag=1;
	IF(DLLcount>=MAXDLLS)outofmemory2();
	IF(SearchTree(#tok0,#type0,#src0,#post0,#string,#number0))return; // DLL ��� ������������
	wsprintfA(#name,"%s",#string);
	dll=_lopen(#name,0);
	IF(dll== -1){
		GetSystemDirectoryA(#path,80);
		wsprintfA(#name,"%s\\%s",#path,#string);
		dll=_lopen(#name,0);
		IF(dll==-1) {
			unabletoopen(#string);
			return;
		}
	}
	nsect=0;
	_llseek(dll,0x3c,0); _lread(dll,#fptr,4);
	_llseek(dll,fptr+120,0); _lread(dll,#export,4); // Get export address
	IF(export==0) {
		wsprintfA(#mapstr,"ERROR: No export directory in file %s.\n",#string);
		preerror(#mapstr); return;
	}
	_llseek(dll,fptr+6,0); _lread(dll,#nsect,2);			 // Number of sections
	delta=export;
	i=1;
	while(i<=nsect){
		EAX=i; EAX--; EAX=EAX*40; EAX+=260; EAX+=fptr;	// fptr+260+40*(i-1)
		_llseek(dll,EAX,0); _lread(dll,#base,4); // RVA of section
		IF(base<=export){
			EAX=export-base;
			IF(EAX<delta){
				delta=export-base;
				EAX=i; EAX--; EAX=EAX*40; EAX+=268; EAX+=fptr;	// fptr+268+40*(i-1)
				_llseek(dll,EAX,0); _lread(dll,#dllpos,4);
			}
		}
		i++;
	}
	dllpos = dllpos + delta;	 // filepos for export directory table
	delta = dllpos - export;
	_llseek(dll,dllpos+24,0); _lread(dll,#nexports,4);		// number of entries for export
	_llseek(dll,dllpos+32,0); _lread(dll,#base,4);		// address of export name pointer table
	_llseek(dll,dllpos+36,0); _lread(dll,#fptr,4);// address of Ordinal Table
	base=base+delta; fptr=fptr+delta;
	tok0=tok; number0=number;src0=src;type0=type;post0=post;
	tok=tk_DLL; number=nexports;src=NULL; type=byName; post=0; modline=0;
	AddToTree(#string);
	EBX=DLLcount; EBX<<=2;
	DLLlist[EBX] = treeptr; 	// save ptr in tree
	tok=tk_API; type=treeptr;
	i=0;
	while(nexports-1>i){
		EAX=i; EAX<<=1; EAX+=fptr;	// fptr+2*i
		_llseek(dll,EAX,0); _lread(dll,#ord,2);// Ordinal number
		EAX=i; EAX<<=2; EAX+=base;	// base+4*i
		_llseek(dll,EAX,0); _lread(dll,#pname1,8);	// address of name
		_llseek(dll,pname1+delta,0); _lread(dll,#string,pname2-pname1);// address of Ordinal Table
		 number=ord+1;	// ��� �������� ������������ ����� �� 1 ������ ��������������� �� DLL
		 AddToTree(#string);
//	 SHOW(#string);SHOW("\n");
		 i++;
	}
	EAX=i; EAX<<=1; EAX+=fptr;	// fptr+2*i
	_llseek(dll,EAX,0); _lread(dll,#ord,2); // Ordinal number
	j=0;
	for(;;){
		_llseek(dll,pname2+delta+j,0); EAX=j;
		_lread(dll,#string[EAX],1); EAX=j;
		IF(string[EAX]==0)BREAK;
		j++;
	}
	number=ord+1;
	AddToTree(#string);
	tok=tok0; number=number0;src=src0;type=type0;post=post0;
	_lclose(dll);
	DLLcount++; importFlag=0;
}

// ----- ��������� #include
DirInclude()
byte s[STRLEN],s2[STRLEN];
{
	NextTok();
	if(tok==tk_string) {
		AL=cha2;
		$PUSH EAX,linenum2,inptr2,number,tok2,tok,input,inptr,endoffile,
				displaytokerrors,currmod;
		lstrcpyA(#s,#string); lstrcpyA(#s2,#string2);
		Preview(#s);
		lstrcpyA(#string,#s); lstrcpyA(#string2,#s2);
		$POP currmod,displaytokerrors,endoffile,inptr,input,tok,tok2,number,inptr2,
			linenum2,EAX;
		cha2=AL;
		NextTok();
	}
	ELSE stringexpected();
}

// ----- ��������� list
DirList()
{
	IF(mapfile==0){
		makemapfile=1;
		StartMapfile();
	}
	list^=1;		// ������������ ������ ��������
	NextTok();
}

// ----- ��������� map
DirMap()
{
	makemapfile = 1; StartMapfile();
	NextTok();
}

// ---- ��������� ���������� ���������� ��� ��������� � �����
GetProc(dword vartype)
dword src0,beg,count;
byte var_name[IDLENGTH];
{
	lstrcpyA(#var_name,#string);	// ��� ���������
	beg=inptr2; 	// ������� ������ ��������
	count=0; EAX=0; 	// ���� ������ ����� ���������
	modline=currmod<<16+linenum2;
	for(;;){
		ESI><inptr2;
		$LODSB;
		ESI><inptr2;
		cha2=AL;
		IF(AL==0){
			unexpectedeof();
			return;
		}
		IF(AL==13){ 	// CR
			linenum2++; // ��������� ����� ������
			totallines++;
			CONTINUE;
		}
		IF(AL=='{'){	// ������ �������������
			count++;
			BREAK;
		}
	}
	for(;;){
		ESI><inptr2;
		$LODSB;
		ESI><inptr2;
		cha2=AL;
		IF(AL==0){
			unexpectedeof();
			break;
		}
		IF(AL==13){ 	// CR
			linenum2++; // ��������� ����� ������
			totallines++;
		}
		else if(AL=='}'){	// ���� ������
			count--;
			IF(count==0){ // ����� ���������
				ESI><inptr2;
				$LODSB;
				ESI><inptr2;
				cha2=AL;	// ���������� }
				src0=LocalAlloc(0x40,inptr2-beg+2); // �������� ���.�����
				EAX=src0;
				DSBYTE[EAX]='(';
				lstrcpynA(src0+1,beg,inptr2-beg);
				tok=tk_proc;
				type=vartype;
				src=src0;
				number=0;
				post=1;
				AddToTree(#var_name);
				BREAK;
			}
		}
		ELSE IF(AL=='{'){	// ������ �������������
			count++;
		}
	}
	NextTok();
}

// ---- ��������� ���������� ���������� ��� ��������� � �����
GetVar(dword vartype)
dword src0,beg,end,count,size;
byte var_name[IDLENGTH];
{
	beg=inptr;
	modline=0;		// ������� ������ ��������
	NextTok();
	IF(tok2==tk_openbracket){ // ���������� �������: type FunctionName(...)
		GetProc(vartype);
		return;
	}
	for(;;){		// ���������� ����������
		IF(tok==tk_semicolon){	// ����� ����������� ����������
			tok=tk_var;
			type=vartype;
			src=src0;
			number=0;
			post=1;
			AddToTree(#var_name);
			break;
		}
		IF(tok==tk_comma){		// ������ ����������
			tok=tk_var;
			type=vartype;
			src=src0;
			number=0;
			post=1;
			AddToTree(#var_name);
			NextTok();
		}
		else IF(tok==tk_id){ 			//	tk_id
			src0=NULL;
			beg=inptr2;
			size=0;
			lstrcpyA(#var_name,#string);	// ��� ����������
			number=0;
			tok=tk_var;
			type=vartype;
			post=1;
			NextTok();
		}
		else if(tok==tk_assign)||(tok==tk_openblock){
			inptr2--;
			count=0;
			EAX=0;
			for(;;){
				ESI><inptr2;
				$LODSB;
				ESI><inptr2;
				cha2=AL;
				IF(AL==0){
					unexpectedeof();
					break;
				}
				IF(AL=='"'){
					ESI><inptr2;
					do{
						$LODSB;
					}while(AL!='"');
					ESI><inptr2;
					cha2=AL;
				}
				else IF(AL==',')||(AL==';'){
					IF(count==0){
						end=inptr2;
						src0 = LocalAlloc(0x40,end-beg+2);
						IF(size){
							EAX=src0;
							DSBYTE[EAX]='[';
							lstrcpynA(src0+1,beg,end-beg);
						}
						ELSE lstrcpynA(src0,beg,end-beg);
						modline=currmod<<16+linenumber;
						BREAK;
					}
				}
				ELSE IF(AL=='}'){	// ������ ��������
					count--;
				}
				ELSE IF(AL=='{'){	// ������ �������������
					count++;
				}
				IF(AL==']'){	// �����������
					size++;
				}
			}
			NextTok();
		}
	}
	NextTok();
}

// ---- ���������� ���� ������
CmdByte()
{
	GetVar(tk_byte);
}

CmdChar()
{
	GetVar(tk_char);
}

CmdWord()
{
	GetVar(tk_word);
}

CmdShort()
{
	GetVar(tk_short);
}

CmdDword()
{
	GetVar(tk_dword);
}

CmdInt()
{
	GetVar(tk_int);
}

// ----  break;
CmdBreak()
{
	wsprintfA(#mapstr,"jmp @L%d",endlabel);
	Asm(#mapstr);
	NextSemiNext();
}

// ---- case(Cond) ...
CmdCase()
dword loclabel;
{
	NextTok();
	expecting(tk_openbracket);
	loclabel=label;
	label++;
	relation=0;
	if(tok==tk_command){
		GetDirAddr(#Jmp_Commands,number);
		IF(EAX==#CmdCarryFlag)wsprintfA(#mapstr,"jnc @L%d",loclabel);
		else IF(EAX==#CmdNotCarryFlag)wsprintfA(#mapstr,"jc @L%d",loclabel);
		ELSE IF(EAX==#CmdZeroFlag)wsprintfA(#mapstr,"jnz @L%d",loclabel);
		ELSE IF(EAX==#CmdNotZeroFlag)wsprintfA(#mapstr,"jz @L%d",loclabel);
		ELSE IF(EAX==#CmdOverflow)wsprintfA(#mapstr,"jno @L%d",loclabel);
		ELSE IF(EAX==#CmdNotOverflow)wsprintfA(#mapstr,"jo @L%d",loclabel);
		NextTok();
	}
	ELSE{
		IF(Expression("eax",tk_reg,tk_dword))wsprintfA(loclabel,"test eax,eax;jnz @L%d",#mapstr);
		ELSE wsprintfA(#mapstr,"test eax,eax;jz @L%d",loclabel);
	}
	Asm(#mapstr);
	expecting(tk_closebracket);
	DoCommand();
	wsprintfA(#mapstr,"jmp @L%d",endlabel);
	Asm(#mapstr);
	wsprintfA(#mapstr,"@L%d:",loclabel);
	Asm(#mapstr);
}

// ---- continue;
CmdContinue()
{
	wsprintfA(#mapstr,"jmp @L%d",startlabel);
	Asm(#mapstr); NextSemiNext();
}

// ---- cycle(Var) ...
CmdCycle()
byte varName[2*IDLENGTH];
{
	NextTok();
	expecting(tk_openbracket);
	$PUSH startlabel,endlabel;
	startlabel=label;
	label++;
	endlabel=label;
	label++;
	relation=0;
	wsprintfA(#mapstr,"@L%d:",startlabel);
	Asm(#mapstr);
	GetVarname(#varName);
	NextTok();
	expecting(tk_closebracket);
	DoCommand();
	IF(varName[0]==0){	// ������� ����� ����������� - ����������� ����
		wsprintfA(#mapstr,"jmp @L%d",startlabel);
	}
	 ELSE{
		wsprintfA(#mapstr,"dec %s;jnz @L%d",#varName,startlabel);
	}
	Asm(#mapstr);
	wsprintfA(#mapstr,"@L%d:",endlabel);
	Asm(#mapstr);
	$POP endlabel,startlabel;
}

// ---- ����� ������� � if
CmdCarryFlag()
{
	CmdNotCarryFlag:
	CmdZeroFlag:
	CmdNotZeroFlag:
	CmdOverflow:
	CmdNotOverflow:
}
// ---- ... else ...
CmdElse()
{
}

// ---- ���������� ������������ ��������
CmdEnum()
dword counter;
byte holdid[IDLENGTH];
{
	counter=0;
	NextTok();
	expecting(tk_openbrace);
	for(;;){
		IF(tok==tk_eof)unexpectedeof();
		ELSE IF(tok==tk_comma)NextTok();
		ELSE IF(tok==tk_closebrace)BREAK;
		ELSE IF(tok==tk_id){
			lstrcpyA(#holdid,#string);
			IF(tok2==tk_assign ){
				NextTok();
				NextTok();
				IF(tok==tk_number)counter=DoConstLongMath();
				ELSE numexpected();
			}
			AddConstToTree(#holdid,counter);
			counter++;
			NextTok();
		}
		ELSE{
			idexpected();
			NextTok();
		}
	}
	expecting(tk_closebrace);
	SemiNext();
}

// ---- while(Cond) ...
CmdWhile()
{
	NextTok();
	expecting(tk_openbracket);
	$PUSH startlabel,endlabel;
	startlabel=label;
	label++;
	endlabel=label;
	label++;
	relation=0;
	wsprintfA(#mapstr,"@L%d:",startlabel);
	Asm(#mapstr);
	if(tok==tk_command){
		GetDirAddr(#Jmp_Commands,number);
		IF(EAX==#CmdCarryFlag)wsprintfA(#mapstr,"jnc @L%d",endlabel);
		else IF(EAX==#CmdNotCarryFlag)wsprintfA(#mapstr,"jc @L%d",endlabel);
		else IF(EAX==#CmdZeroFlag)wsprintfA(#mapstr,"jnz @L%d",endlabel);
		ELSE IF(EAX==#CmdNotZeroFlag)wsprintfA(#mapstr,"jz @L%d",endlabel);
		ELSE IF(EAX==#CmdOverflow)wsprintfA(#mapstr,"jno @L%d",endlabel);
		ELSE IF(EAX==#CmdNotOverflow)wsprintfA(#mapstr,"jo @L%d",endlabel);
		NextTok();
	}
	ELSE{
		IF(Expression("eax",tk_reg,tk_dword))wsprintfA(#mapstr,"test eax,eax;jnz @L%d",endlabel);
		ELSE wsprintfA(#mapstr,"test eax,eax;jz @L%d",endlabel);
	}
	Asm(#mapstr);
	expecting(tk_closebracket);
	DoCommand();
	wsprintfA(#mapstr,"jmp @L%d",startlabel);
	Asm(#mapstr);
	wsprintfA(#mapstr,"@L%d:",endlabel);
	Asm(#mapstr);
	$POP endlabel,startlabel;
}

// ---- default
CmdDefault()
{
	NextTok();
	DoCommand();
}

CmdDb()
{
	NextTok();
	for(;;){
		IF(tok==tk_number)OP(byte DoConstLongMath());
		ELSE IF(tok==tk_string ){
			ECX=number;
			EDX=#string;
			loop(ECX){
				OP(byte DSBYTE[EDX]);
				EDX++;
			}
			NextTok();
		}
		ELSE IF(tok==tk_comma)NextTok();
		ELSE IF(tok==tk_semicolon)BREAK;
		ELSE{
			numexpected();
			NextTok();
		}
	}
}

CmdDd()
{
	NextTok();
	for(;;){
		IF(tok==tk_number)OUTDWORD(DoConstDwordMath());
		ELSE IF(tok==tk_comma)NextTok();
		ELSE IF(tok==tk_semicolon)BREAK;
		ELSE{
			numexpected();
			NextTok();
		}
	}
}

CmdDw()
{
	NextTok();
	for(;;){
		IF(tok==tk_number)OUTWORD(DoConstDwordMath());
		ELSE IF(tok==tk_comma)NextTok();
		ELSE IF(tok==tk_semicolon)BREAK;
		ELSE{
			numexpected();
			NextTok();
		}
	}
}

// ---- do ... while(Cond)
CmdDo()
{
	NextTok();
	$PUSH startlabel,endlabel;
	startlabel=label;
	label++;
	endlabel=label;
	label++;
	relation=0;
	wsprintfA(#mapstr,"@L%d:",startlabel);
	Asm(#mapstr);
	DoCommand();
	if(tok==tk_command){
		if(GetDirAddr(#Jmp_Commands,number)==#CmdWhile){
			NextTok();
			expecting(tk_openbracket);
			if(tok==tk_command){
				GetDirAddr(#Jmp_Commands,number);
				IF(EAX==#CmdCarryFlag)wsprintfA(#mapstr,"jc @L%d",startlabel);
				else IF(EAX==#CmdNotCarryFlag)wsprintfA(#mapstr,"jnc @L%d",startlabel);
				else IF(EAX==#CmdZeroFlag)wsprintfA(#mapstr,"jz @L%d",startlabel);
				ELSE IF(EAX==#CmdNotZeroFlag)wsprintfA(#mapstr,"jnz @L%d",startlabel);
				ELSE IF(EAX==#CmdOverflow)wsprintfA(#mapstr,"jo @L%d",startlabel);
				ELSE IF(EAX==#CmdNotOverflow)wsprintfA(#mapstr,"jno @L%d",startlabel);
				NextTok();
			}
			ELSE{
				IF(Expression("eax",tk_reg,tk_dword))wsprintfA(#mapstr,"test eax,eax;jz @L%d",startlabel);
				ELSE wsprintfA(#mapstr,"test eax,eax;jnz @L%d",startlabel);
			}
			Asm(#mapstr);
			expecting(tk_closebracket);
		}
		ELSE{
ER:
			preerror("'while' expected following 'do'");
		}
	}
	ELSE GOTO ER;
	wsprintfA(#mapstr,"@L%d:",endlabel);
	Asm(#mapstr);
	$POP endlabel,startlabel;
}

// ---- docase ...
CmdDoCase()
{
	NextTok();
	$PUSH startlabel,endlabel;
	startlabel=label;
	label++;
	endlabel=label;
	label++;
	wsprintfA(#mapstr,"@L%d:",startlabel);
	Asm(#mapstr);
	DoCommand();
	wsprintfA(#mapstr,"@L%d:",endlabel);
	Asm(#mapstr);
	$POP endlabel,startlabel;
}

// ---- if(Cond) ...
CmdIf()
dword loclabel;
{
	NextTok();
	expecting(tk_openbracket);
	loclabel=label;
	label++;
	relation=0;
	if(tok==tk_command){
		GetDirAddr(#Jmp_Commands,number);
		IF(EAX==#CmdCarryFlag)wsprintfA(#mapstr,"jnc @L%d",loclabel);
		else IF(EAX==#CmdNotCarryFlag)wsprintfA(#mapstr,"jc @L%d",loclabel);
		ELSE IF(EAX==#CmdZeroFlag)wsprintfA(#mapstr,"jnz @L%d",loclabel);
		ELSE IF(EAX==#CmdNotZeroFlag)wsprintfA(#mapstr,"jz @L%d",loclabel);
		ELSE IF(EAX==#CmdOverflow)wsprintfA(#mapstr,"jno @L%d",loclabel);
		ELSE IF(EAX==#CmdNotOverflow)wsprintfA(#mapstr,"jo @L%d",loclabel);
		NextTok();
	}
	ELSE{
		IF(Expression("eax",tk_reg,tk_dword))wsprintfA(#mapstr,"test eax,eax;jnz @L%d",loclabel);
		ELSE wsprintfA(#mapstr,"test eax,eax;jz @L%d",loclabel);
	}
	Asm(#mapstr);
	expecting(tk_closebracket);
	DoCommand();
	IF(tok==tk_command){
		IF(GetDirAddr(#Jmp_Commands,number)==#CmdElse){
			wsprintfA(#mapstr,"jmp @L%d;\n@L%d:",label,loclabel);
			Asm(#mapstr);
			loclabel=label;
			label++;
			NextTok();
			DoCommand();
		}
	}
	wsprintfA(#mapstr,"@L%d:",loclabel);
	Asm(#mapstr);
}

// ---- return(Expr)
CmdReturn()
{
	NextTok();
	IF(tok==tk_openbracket){
		NextTok();
		IF(tok!=tk_closebracket){
			IF(returntype!=tk_void)Expression("eax",tk_reg,returntype);
		}
		expecting(tk_closebracket);
	}
	LeaveProc();
	SemiNext();
}

// ---- ���������� ���������� ����������
GlobalVar(dword vartype)	// both initialized and unitialized combined
dword size,elements,ptr;
long i,count;
{
	ptr=treeptr;
	elements=1;
	size = TypeSize(vartype);
	NextTok();
	for(;;){
		IF(tok==tk_eof)goto G04;	//break;
		ELSE IF(tok==tk_semicolon)NextTok();
		else IF(tok==tk_assign)NextTok();
		else IF(tok==tk_plus)NextTok();
		else IF(tok==tk_openblock){ 	// type VarName[...]
			NextTok();
			elements = DoConstLongMath();
			expecting(tk_closeblock);
		}
		else if(tok==tk_number){	// type VarName=initvalue
			ESI=ptr;	// �������������� ����� ��������� ������
			DSDWORD[ESI+recnumber] = outptr-output+OptImageBase+OptBaseOfCode;
			DSDWORD[ESI+recpost] = 0;
G01:
			IF(vartype==tk_byte)i=DoConstDwordMath();
			ELSE IF(vartype==tk_word)i=DoConstDwordMath();
			ELSE IF(vartype==tk_dword)i=DoConstDwordMath();
			ELSE i=DoConstLongMath();
			count=elements;
			loop(count){
				IF(size==1)OP(byte i);
				ELSE IF(size==2)OUTWORD(i);
				ELSE IF(size==4)OUTDWORD(i);
			}
		}
		else IF(tok==tk_minus){
			NextTok();
			number=-number;
			goto G01;
		}
		else IF(tok==tk_string){
			ESI=ptr;	// �������������� ����� ��������� ������
			DSDWORD[ESI+recnumber] = outptr-output+OptImageBase+OptBaseOfCode;
			DSDWORD[ESI+recpost] = 0;
			count = 1;
			do{
				i=number;
				EDX=#string;
				loop(i){
					OP(byte DSBYTE[EDX]);
					EDX++;
					count++;
				}
				NextTok();
			}while(tok==tk_string);
			OP(byte 0);// count++;
			i=elements;
			i-=count;
			IF(i>0)loop(i)OP(byte 0);
		}
		else IF(tok==tk_from){
			NextTok();
			SHOW("count = DoFrom(1);\n");
			i=size*elements;
			i-=count;
			loop(i)OP(byte 0);
			NextTok();
		}
		else IF(tok==tk_extract){
			NextTok();
			SHOW("count = DoExtract(1);\n");
			i=size*elements;
			i-=count;
			loop(i)OP(byte 0);
		}
		else if(tok==tk_openbrace){ // varname={...};
			ESI=ptr;	// �������������� ����� ��������� ������
			DSDWORD[ESI+recnumber] = outptr-output+OptImageBase+OptBaseOfCode;
			DSDWORD[ESI+recpost] = 0;
			count = 0;
			NextTok();
			for(;;){
				IF(tok==tk_closebrace)break;
				ELSE IF(tok==tk_comma)NextTok();
				else IF(tok==tk_plus)NextTok();
				else IF(tok==tk_string){
					i=number;
					EDX=#string;
					loop(i){
						OP(DSBYTE[EDX]);
						EDX++;
						count++;
					}
					IF(tok2!=tk_plus){
						OP(byte 0);
						count++;
					}
					NextTok();
				}
				else IF(tok==tk_postnumber){
					SetPost(treeptr,POST_DATA);
					OUTDWORD(number);
					NextTok();
				}
				else IF(tok==tk_number){
G02:
					IF(vartype==tk_byte)OP(byte DoConstDwordMath());
					ELSE IF(vartype==tk_word)OUTWORD(DoConstDwordMath());
					ELSE IF(vartype==tk_char)OP(byte DoConstLongMath());
					ELSE IF(vartype==tk_short) OUTWORD(DoConstLongMath());
					ELSE IF(vartype==tk_dword) OUTDWORD(DoConstDwordMath());
					ELSE IF(vartype==tk_int) OUTDWORD(DoConstLongMath());
					count++;
				}
				ELSE IF(tok==tk_minus){
					NextTok();
					number=-number;
					goto G02;
				}
				ELSE{
					numexpected();
					NextTok();
				}
			}
			count=elements-count;
			IF(count>0){
				loop(count){
					IF(size==1)OP(byte 0);
					ELSE IF(size==2)OUTWORD(0);
					ELSE IF(size==4)OUTDWORD(0);
				}
			}
		}
		ELSE{
G04:
			ESI=ptr;
			DSDWORD[ESI+recnumber] = postsize;
			DSDWORD[ESI+recpost] = 1;
			postsize = elements * size + postsize;
			BREAK;
		}
	}
}

//===== ������� �������������� ��������
dword Jmp_Directives={
//	"if","else","endif",	// �������� ����������
	#EMPTY,#EMPTY,#EMPTY,
//	"include","define", // ��������� �� ��
	#DirInclude,#DirDefine,
//	"import", 	// ������ �� DLL
	#DirImport,
//	"importN",		// ������ �� DLL
	#DirImportN,
//	"map",			// ��������� MAP-�����
	#DirMap,
//	"debug",		// ��������� ���������� ����������
	#EMPTY,
//	"list", 		// ������ ASM-��������
	#DirList,
//	"dll",			// ��������� DLL-�����
	#EMPTY,
//	"db","dw","dd", 	// ���� ����������
	#EMPTY,#EMPTY,#EMPTY,
//	"byte","char","word","short","dword","int",
	#EMPTY,#EMPTY,#EMPTY,#EMPTY,#EMPTY,#EMPTY,
//	"enum", 		// ������������ ���������
	#DirEnum,
//	"struc",		// ����������� ���������
	#EMPTY,
//	"cycle","return",
	#EMPTY,#EMPTY,
//	"while","do","inline",
	#EMPTY,#EMPTY,#EMPTY,
//	"continue","break",
	#EMPTY,#EMPTY,
//	"docase","case","default",
	#EMPTY,#EMPTY,#EMPTY,
//	"CARRYFLAG","extract","from",
	#EMPTY,#EMPTY,#EMPTY,
//	"NOTCARRYFLAG","NOTOVERFLOW","OVERFLOW",
	#EMPTY,#EMPTY,#EMPTY,
//	"ZEROFLAG","NOTZEROFLAG"
	#EMPTY,#EMPTY
};
//===== ������� �������������� ������
dword Jmp_Commands={
//	"if","else","endif",	// �������� ����������
	#CmdIf,#CmdElse,#EMPTY,
//	"include","define", // ��������� �� ��
	#EMPTY,#EMPTY,
//	"import","importN", // ������ �� DLL
	#EMPTY,#EMPTY,
//	"map",			// ��������� MAP-�����
	#EMPTY,
//	"debug",		// ��������� ���������	����������
	#EMPTY,
//	"list", 		// ������ ASM-� ������
	#EMPTY,
//	"dll",			// ��������� DLL-�����
	#EMPTY,
//	"db","dw","dd", 	// ���� ����������
	#CmdDb,#CmdDw,#CmdDd,
//	"byte","char","word","short","dword","int",
	#CmdByte,#CmdChar,#CmdWord,#CmdShort,#CmdDword,#CmdInt,
//	"enum", 		// ������������ ���������
	#CmdEnum,
//	"struc",		// ����������� ���������
	#EMPTY,
//	"cycle","return",
	#CmdCycle,#CmdReturn,
//	"while","do","inline",
	#CmdWhile,#CmdDo,#EMPTY,
//	"continue","break",
	#CmdContinue,#CmdBreak,
//	"docase","case","default",
	#CmdDoCase,#CmdCase,#CmdDefault,
//	"CARRYFLAG","extract","from",
	#CmdCarryFlag,#EMPTY,#EMPTY,
//	"NOTCARRYFLAG","NOTOVERFLOW","OVERFLOW",
	#CmdNotCarryFlag,#CmdNotOverflow,#CmdOverflow,
//	"ZEROFLAG","NOTZEROFLAG"
	#CmdZeroFlag,#CmdNotZeroFlag
};

