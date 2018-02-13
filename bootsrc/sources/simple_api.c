
/* 
	Copyright (c) 2017-2018 Azeez Adewale <azeezadewale98@gmail.com"> 
	MIT License Copyright (c) 2017 simple 

*/

/* 
 * File:   simple.h
 * Author: thecarisma
 *
 * Created on July 10, 2017, 1:10 PM
 */

#include "../includes/simple.h"
/* Support for C Functions */

SIMPLE_API void register_block_t ( SimpleState *sState,const char *cStr, void (*pFunc)(void *) )
{
	List *list  ;
	if ( sState->c_blocks == NULL ) {
		sState->c_blocks = simple_list_new_gc(sState,0);
	}
	list = simple_list_newlist_gc(sState,sState->c_blocks);
	simple_list_addstring_gc(sState,list,cStr);
	simple_list_addfuncpointer_gc(sState,list,pFunc);
}

SIMPLE_API void loadcfunctions ( SimpleState *sState )
{
	/* General */
	register_block("lengthOf",simple_vmlib_len);
	register_block("add",simple_vmlib_add);
	register_block("del",simple_vmlib_del);
	register_block("clock",simple_vmlib_clock);
	register_block("input",simple_vmlib_input);
	register_block("ascii",simple_vmlib_ascii);
	register_block("char",simple_vmlib_char);
	register_block("date",simple_vmlib_date);
	register_block("time",simple_vmlib_time);
	register_block("getchar",simple_vmlib_getchar);
	register_block("random",simple_vmlib_random);
	register_block("timelist",simple_vmlib_timelist);
	register_block("adddays",simple_vmlib_adddays);
	register_block("diffdays",simple_vmlib_diffdays);
	register_block("getSimpleVersion",simple_vmlib_version);
	register_block("clockspersecond",simple_vmlib_clockspersecond);
	register_block("swap",simple_vmlib_swap);
	/* Check Data Type */
	register_block("isstring",simple_vmlib_isstring);
	register_block("isnumber",simple_vmlib_isnumber);
	register_block("islist",simple_vmlib_islist);
	register_block("type",simple_vmlib_type);
	register_block("isnull",simple_vmlib_isnull);
	register_block("isobject",simple_vmlib_isobject);
	/* Conversion */
	register_block("hex",simple_vmlib_hex);
	register_block("dec",simple_vmlib_dec);
	register_block("number",simple_vmlib_number);
	register_block("string",simple_vmlib_string);
	register_block("str2hex",simple_vmlib_str2hex);
	register_block("hex2str",simple_vmlib_hex2str);
	register_block("str2list",simple_vmlib_str2list);
	register_block("list2str",simple_vmlib_list2str);
	register_block("str2hexcstyle",simple_vmlib_str2hexcstyle);
	/* Functional */
	register_block("executeCode",simple_vmlib_eval);
	/* Error Handling */
	register_block("throw",simple_vmlib_raise);
	register_block("assert",simple_vmlib_assert);
	/* Check Characters */
	register_block("isalnum",simple_vmlib_isalnum);
	register_block("isalpha",simple_vmlib_isalpha);
	register_block("iscntrl",simple_vmlib_iscntrl);
	register_block("isdigit",simple_vmlib_isdigit);
	register_block("isgraph",simple_vmlib_isgraph);
	register_block("islower",simple_vmlib_islower);
	register_block("isprint",simple_vmlib_isprint);
	register_block("ispunct",simple_vmlib_ispunct);
	register_block("isspace",simple_vmlib_isspace);
	register_block("isupper",simple_vmlib_isupper);
	register_block("isxdigit",simple_vmlib_isxdigit);
	/* Garbage Collector & Variable Pointer */
        simple_vm_dll_loadfunctions(sState);
	register_block("callgc",simple_vmlib_callgc);
	register_block("varptr",simple_vmlib_varptr);
	register_block("intvalue",simple_vmlib_intvalue);
	register_block("object2pointer",simple_vmlib_object2pointer);
	register_block("pointer2object",simple_vmlib_pointer2object);
	register_block("nullpointer",simple_vmlib_nullpointer);
	register_block("space",simple_vmlib_space);
	register_block("ptrcmp",simple_vmlib_ptrcmp);
	/* Simple State */
	register_block("init_simple_state",init_simple_state_block);
	register_block("execute_simple_code",execute_simple_code_block);
	register_block("free_simple_state",free_simple_state_block);
	register_block("execute_simple_file",execute_simple_file_block);
	register_block("find_simple_variable",find_simple_variable_block);
	register_block("init_simple_variable",simple_vmlib_state_newvar);
	register_block("main_simple_state",main_simple_state_block);
	register_block("simple_state_setvar",simple_vmlib_state_setvar);
	register_block("simple_state_new",simple_vmlib_state_new);
	register_block("simple_state_mainfile",simple_vmlib_state_mainfile);
	/*
	**  Simple See and Give 
	**  Also we add the display() and read() block to the api
	*/
	register_block("display",display_string);
	register_block("read",read_string);
}

int api_is_list ( void *pointer,int x )
{
	int nType  ;
	if ( SIMPLE_API_ISPOINTER(x) ) {
		nType = SIMPLE_API_GETPOINTERTYPE(x);
		if ( nType == SIMPLE_OBJTYPE_VARIABLE || nType == SIMPLE_OBJTYPE_LISTITEM ) {
			return 1 ;
		}
	}
	return 0 ;
}

SIMPLE_API List * api_get_list ( void *pointer,int x )
{
	int nType  ;
	Item *pItem  ;
	List *list  ;
	if ( SIMPLE_API_ISPOINTER(x) ) {
		nType = SIMPLE_API_GETPOINTERTYPE(x);
		if ( nType == SIMPLE_OBJTYPE_VARIABLE ) {
			list = (List *) SIMPLE_API_GETPOINTER(x) ;
			return simple_list_getlist(list,3) ;
		}
		else if ( nType == SIMPLE_OBJTYPE_LISTITEM ) {
			pItem = (Item *) SIMPLE_API_GETPOINTER(x) ;
			return simple_item_getlist(pItem) ;
		}
	}
	return NULL ;
}

SIMPLE_API void api_ret_list ( void *pointer,List *list )
{
	List *list2,*list3  ;
	VM *vm  ;
	vm = (VM *) pointer ;
	list2 = simple_list_getlist(vm->pMem,simple_list_getsize(vm->pMem)-1);
	list3 = simple_vm_newvar2(vm,SIMPLE_TEMP_VARIABLE,list2);
	simple_list_setint_gc(((VM *) pointer)->sState,list3,SIMPLE_VAR_TYPE,SIMPLE_VM_LIST);
	simple_list_setlist_gc(((VM *) pointer)->sState,list3,SIMPLE_VAR_VALUE);
	list2 = simple_list_getlist(list3,SIMPLE_VAR_VALUE);
	/* Copy the list */
	simple_list_copy(list2,list);
	SIMPLE_API_PUSHPVALUE(list3);
	SIMPLE_API_OBJTYPE = SIMPLE_OBJTYPE_VARIABLE ;
}

SIMPLE_API List * api_new_list ( VM *vm )
{
	List *list  ;
	list = simple_list_newlist_gc(vm->sState,vm->pActiveMem);
	return list ;
}

SIMPLE_API void api_ret_cpointer ( void *pointer,void *pGeneral,const char *cType )
{
	List *list  ;
	/* Create the list */
	list = SIMPLE_API_NEWLIST ;
	/* The variable value will be a list contains the pointer */
	simple_list_addpointer_gc(((VM *) pointer)->sState,list,pGeneral);
	/* Add the pointer type */
	simple_list_addstring_gc(((VM *) pointer)->sState,list,cType);
	/* Add the status number ( 0 = Not Copied ,1 = Copied  2 = Not Assigned yet) */
	simple_list_addint_gc(((VM *) pointer)->sState,list,2);
	SIMPLE_API_RETLIST(list);
}

SIMPLE_API void * api_get_cpointer ( void *pointer,int x,const char *cType )
{
	List *list, *list2  ;
	int y  ;
	if ( SIMPLE_API_ISLIST(x) ) {
		list = SIMPLE_API_GETLIST(x) ;
		if ( simple_list_ispointer(list,1) ) {
			if ( simple_list_getpointer(list,1) != NULL ) {
				if ( (strcmp(simple_list_getstring(list,2),cType) == 0) || (((VM *) pointer)->nIgnoreCPointerTypeCheck==1) ) {
					/*
					**  Check if the pointer is copied or not 
					**  We check for 2 (not assigned) also, happens when f1 ( x , f2() ) and f2 return C pointer 
					*/
					if ( (simple_list_getint(list,3) == 0) || (simple_list_getint(list,3) == 2) ) {
						return simple_list_getpointer(list,1) ;
					}
					list2 = ((VM *) pointer)->aCPointers ;
					if ( simple_list_getsize(list2) > 0 ) {
						for ( y = 1 ; y <= simple_list_getsize(list2) ; y++ ) {
							if ( simple_list_getpointer(list,1) == simple_list_getpointer(list2,y) ) {
								return simple_list_getpointer(list,1) ;
							}
						}
					}
					simple_list_setpointer_gc(((VM *) pointer)->sState,list,1,NULL);
					SIMPLE_API_ERROR(SIMPLE_API_NULLPOINTER);
					return NULL ;
				}
				SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
				return NULL ;
			}
			else {
				if ( strcmp(simple_list_getstring(list,2),"NULLPOINTER") == 0 ) {
					return NULL ;
				}
			}
			SIMPLE_API_ERROR(SIMPLE_API_NULLPOINTER);
			return NULL ;
		}
	}
	SIMPLE_API_ERROR(SIMPLE_API_NOTPOINTER);
	return NULL ;
}

SIMPLE_API void api_set_cpointer_null ( void *pointer,int x )
{
	List *list, *list2  ;
	int y  ;
	list = (List *) SIMPLE_API_GETLIST(x) ;
	/* Check pointer status ( 0 = copied , 1 = Not copied ) */
	if ( simple_list_getint(list,3) == 0 ) {
		simple_list_setpointer_gc(((VM *) pointer)->sState,list,1,NULL);
		return ;
	}
	list2 = ((VM *) pointer)->aCPointers ;
	if ( simple_list_getsize(list2) > 0 ) {
		for ( y = simple_list_getsize(list2) ; y >= 1 ; y-- ) {
			if ( simple_list_getpointer(list,1) == simple_list_getpointer(list2,y) ) {
				simple_list_deleteitem_gc(((VM *) pointer)->sState,list2,y);
				simple_list_setpointer_gc(((VM *) pointer)->sState,list,1,NULL);
			}
		}
	}
}

SIMPLE_API void * api_var_ptr ( void *pointer,const char  *cStr,const char *cStr2 )
{
	VM *vm  ;
	List *list, *pActiveMem  ;
	Item *pItem  ;
	/*
	**  Usage 
	**  To get pointer to a ring variable value (pointer to double or int) i.e. int * or double * 
	**  We need this because some C Functions get int * or double * as parameter 
	*/
	vm = (VM *) pointer ;
	/* Set the Active Scope */
	pActiveMem = vm->pActiveMem ;
	vm->pActiveMem = simple_list_getlist(vm->pMem,simple_list_getsize(vm->pMem)-1);
	if ( simple_vm_findvar(vm, cStr ) == 0 ) {
		/* Restore the Active Scope */
		vm->pActiveMem = pActiveMem ;
		SIMPLE_API_ERROR(SIMPLE_VM_ERROR_NOTVARIABLE);
		return NULL ;
	}
	/* Restore the Active Scope */
	vm->pActiveMem = pActiveMem ;
	list = (List *) SIMPLE_VM_STACK_READP ;
	SIMPLE_VM_STACK_POP ;
	if ( simple_list_getint(list,SIMPLE_VAR_TYPE) == SIMPLE_VM_NUMBER ) {
		pItem = simple_list_getitem(list,SIMPLE_VAR_VALUE);
		if ( strcmp(cStr2,"double") == 0 ) {
			return &(pItem->data.dNumber) ;
		}
		else if ( strcmp(cStr2,"int") == 0 ) {
			return &(pItem->data.iNumber) ;
		}
	}
	else if ( simple_list_getint(list,SIMPLE_VAR_TYPE) == SIMPLE_VM_STRING ) {
		pItem = simple_list_getitem(list,SIMPLE_VAR_VALUE);
		return pItem->data.pString->cStr ;
	}
	return NULL ;
}

SIMPLE_API void api_int_value ( void *pointer,const char  *cStr )
{
	VM *vm  ;
	List *list  ;
	Item *pItem  ;
	/*
	**  Usage 
	**  Convert the variable value from int to double 
	**  When we pass int * for Simple Variable Value to a C Function 
	**  We need to convert again from int to double, because Simple uses double 
	*/
	vm = (VM *) pointer ;
	if ( simple_vm_findvar(vm, cStr ) == 0 ) {
		SIMPLE_API_ERROR(SIMPLE_VM_ERROR_NOTVARIABLE);
		return ;
	}
	list = (List *) SIMPLE_VM_STACK_READP ;
	SIMPLE_VM_STACK_POP ;
	if ( simple_list_getint(list,SIMPLE_VAR_TYPE) == SIMPLE_VM_NUMBER ) {
		pItem = simple_list_getitem(list,SIMPLE_VAR_VALUE);
		pItem->data.dNumber = (double) pItem->data.iNumber ;
	}
}

SIMPLE_API void list_add_cpointer ( List *list,void *pGeneral,const char *cType )
{
	List *list2  ;
	/* create sub list */
	list2 = simple_list_newlist(list);
	/* The variable value will be a list contains the pointer */
	simple_list_addpointer(list2,pGeneral);
	/* Add the pointer type */
	simple_list_addstring(list2,cType);
	/* Add the status number ( 0 = Not Copied ,1 = Copied  2 = Not Assigned yet) */
	simple_list_addint(list2,2);
}

SIMPLE_API int api_iscpointer_list ( List *list )
{
	if ( simple_list_getsize(list) != 3 ) {
		return 0 ;
	}
	if ( simple_list_ispointer(list,1) && simple_list_isstring(list,2) && simple_list_isnumber(list,3) ) {
		return 1 ;
	}
	return 0 ;
}

SIMPLE_API int api_is_cpointer ( void *pointer,int x )
{
	if ( SIMPLE_API_ISLIST(x) ) {
		return api_iscpointer_list(SIMPLE_API_GETLIST(x)) ;
	}
	return 0 ;
}

SIMPLE_API int api_is_object ( void *pointer,int x )
{
	if ( SIMPLE_API_ISLIST(x) ) {
		return simple_vm_oop_isobject(SIMPLE_API_GETLIST(x)) ;
	}
	return 0 ;
}

SIMPLE_API int api_cpointer_cmp ( List *list,List *list2 )
{
	if ( simple_list_getpointer(list,SIMPLE_CPOINTER_POINTER) == simple_list_getpointer(list2,SIMPLE_CPOINTER_POINTER) ) {
		return 1 ;
	}
	else {
		return 0 ;
	}
}

SIMPLE_API int api_is_pointer ( void *pointer,int x )
{
	List *list, *list2  ;
	VM *vm  ;
	Item *pItem  ;
	vm = (VM *) pointer ;
	list = simple_list_getlist(SIMPLE_API_PARALIST,x) ;
	if ( simple_list_ispointer(list,SIMPLE_VAR_VALUE) ) {
		return 1 ;
	}
	else if ( simple_list_isstring(list,SIMPLE_VAR_VALUE) ) {
		/* Treat NULL Strings as NULL Pointers - so we can use NULL instead of NULLPOINTER() */
		if ( strcmp(simple_list_getstring(list,SIMPLE_VAR_VALUE),"") == 0 ) {
			/* Create the list for the NULL Pointer */
			simple_list_setint_gc(((VM *) pointer)->sState,list,SIMPLE_VAR_TYPE,SIMPLE_VM_POINTER);
			list2 = SIMPLE_API_NEWLIST ;
			pItem = simple_list_getitem(vm->pActiveMem,simple_list_getsize(vm->pActiveMem));
			/* Increase the References count for the item */
			simple_vm_gc_newitemreference(pItem);
			simple_list_setpointer_gc(((VM *) pointer)->sState,list,SIMPLE_VAR_VALUE,pItem);
			simple_list_setint_gc(((VM *) pointer)->sState,list,SIMPLE_VAR_PVALUETYPE,SIMPLE_OBJTYPE_LISTITEM);
			/* The variable value will be a list contains the pointer */
			simple_list_addpointer_gc(((VM *) pointer)->sState,list2,NULL);
			/* Add the pointer type */
			simple_list_addstring_gc(((VM *) pointer)->sState,list2,"NULLPOINTER");
			/* Add the status number ( 0 = Not Copied ,1 = Copied  2 = Not Assigned yet) */
			simple_list_addint_gc(((VM *) pointer)->sState,list2,2);
			return 1 ;
		}
	}
	return 0 ;
}

SIMPLE_API void * api_get_cpointer2pointer ( void *pointer,int x,const char *cType )
{
	List *list, *list2  ;
	int y  ;
	Item *pItem  ;
	if ( SIMPLE_API_ISLIST(x) ) {
		list = SIMPLE_API_GETLIST(x) ;
		if ( simple_list_ispointer(list,1) ) {
			if ( simple_list_getpointer(list,1) != NULL ) {
				if ( (strcmp(simple_list_getstring(list,2),cType) == 0) || (((VM *) pointer)->nIgnoreCPointerTypeCheck==1) ) {
					/*
					**  Check if the pointer is copied or not 
					**  We check for 2 (not assigned) also, happens when f1 ( x , f2() ) and f2 return C pointer 
					*/
					if ( (simple_list_getint(list,3) == 0) || (simple_list_getint(list,3) == 2) ) {
						pItem = simple_list_getitem(list,1);
						return & (pItem->data.pointer) ;
					}
					list2 = ((VM *) pointer)->aCPointers ;
					if ( simple_list_getsize(list2) > 0 ) {
						for ( y = 1 ; y <= simple_list_getsize(list2) ; y++ ) {
							if ( simple_list_getpointer(list,1) == simple_list_getpointer(list2,y) ) {
								pItem = simple_list_getitem(list,1);
								return & (pItem->data.pointer) ;
							}
						}
					}
					simple_list_setpointer_gc(((VM *) pointer)->sState,list,1,NULL);
					SIMPLE_API_ERROR(SIMPLE_API_NULLPOINTER);
					return NULL ;
				}
				SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
				return NULL ;
			}
			else {
				if ( strcmp(simple_list_getstring(list,2),"NULLPOINTER") == 0 ) {
					return NULL ;
				}
			}
			SIMPLE_API_ERROR(SIMPLE_API_NULLPOINTER);
			return NULL ;
		}
	}
	SIMPLE_API_ERROR(SIMPLE_API_NOTPOINTER);
	return NULL ;
}

SIMPLE_API void list_add_cpointer_gc ( void *pState,List *list,void *pGeneral,const char *cType )
{
	List *list2  ;
	/* create sub list */
	list2 = simple_list_newlist_gc(pState,list);
	/* The variable value will be a list contains the pointer */
	simple_list_addpointer_gc(pState,list2,pGeneral);
	/* Add the pointer type */
	simple_list_addstring_gc(pState,list2,cType);
	/* Add the status number ( 0 = Not Copied ,1 = Copied  2 = Not Assigned yet) */
	simple_list_addint_gc(pState,list2,2);
}
/*
**  Library 
**  General 
*/

void simple_vmlib_len ( void *pointer )
{
	VM *vm  ;
	vm = (VM *) pointer ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		SIMPLE_API_RETNUMBER(SIMPLE_API_GETSTRINGSIZE(1) - 1);
	}
	else if ( SIMPLE_API_ISLIST(1) ) {
		if ( simple_vm_oop_isobject(SIMPLE_API_GETLIST(1)) == 0 ) {
			SIMPLE_API_RETNUMBER(simple_list_getsize(SIMPLE_API_GETLIST(1)) - 1);
		}
		else {
			SIMPLE_VM_STACK_PUSHPVALUE(SIMPLE_API_GETPOINTER(1));
			SIMPLE_VM_STACK_OBJTYPE = SIMPLE_API_GETPOINTERTYPE(1) ;
			simple_vm_expr_npoo(vm,"lengthOf",0);
			vm->nIgnoreNULL = 1 ;
		}
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_add ( void *pointer )
{
	List *list,*list2  ;
	VM *vm  ;
	vm = (VM *) pointer ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	if ( SIMPLE_API_ISLIST(1) ) {
		list = SIMPLE_API_GETLIST(1) ;
		if ( SIMPLE_API_ISSTRING(2) ) {
			simple_list_addstring2_gc(vm->sState,list,SIMPLE_API_GETSTRING(2),SIMPLE_API_GETSTRINGSIZE(2));
			SIMPLE_API_RETSTRING2(SIMPLE_API_GETSTRING(2),SIMPLE_API_GETSTRINGSIZE(2));
		}
		else if ( SIMPLE_API_ISNUMBER(2) ) {
			simple_list_adddouble_gc(vm->sState,list,SIMPLE_API_GETNUMBER(2));
			SIMPLE_API_RETNUMBER(SIMPLE_API_GETNUMBER(2));
		}
		else if ( SIMPLE_API_ISLIST(2) ) {
			list2 = SIMPLE_API_GETLIST(2) ;
			simple_vm_addlisttolist(vm,list2,list);
		}
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_del ( void *pointer )
{
	List *list  ;
	double nNum1  ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	if ( SIMPLE_API_ISLIST(1) ) {
		list = SIMPLE_API_GETLIST(1) ;
		if ( SIMPLE_API_ISNUMBER(2) ) {
			nNum1 = SIMPLE_API_GETNUMBER(2) ;
			if ( ( nNum1 < 1 ) || ( nNum1 > simple_list_getsize(list) ) ) {
				SIMPLE_API_ERROR("Error in second parameter, item number outside the list size range!");
				return ;
			}
			simple_list_deleteitem_gc(((VM *) pointer)->sState,list,nNum1);
		} else {
			SIMPLE_API_ERROR("Error in second parameter, Function requires number!");
			return ;
		}
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_clock ( void *pointer )
{
	clock_t nNum1  ;
	nNum1 = clock();
	SIMPLE_API_RETNUMBER((double) nNum1);
}

void simple_vmlib_input ( void *pointer )
{
	char *cLine  ;
	int nSize  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISNUMBER(1) ) {
		nSize = SIMPLE_API_GETNUMBER(1);
	}
	else if ( SIMPLE_API_ISSTRING(1) ) {
		nSize = simple_vm_stringtonum((VM *) pointer,SIMPLE_API_GETSTRING(1));
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
		return ;
	}
	if ( nSize > 0 ) {
		cLine = (char *) simple_state_malloc(((VM *) pointer)->sState,nSize);
		if ( cLine == NULL ) {
			SIMPLE_API_ERROR(SIMPLE_OOM);
			return ;
		}
		/* Get Input From the User and save it in the variable */
		SIMPLE_SETBINARY ;
		fread( cLine , sizeof(char) , nSize , stdin );
		/* Return String */
		SIMPLE_API_RETSTRING2(cLine,nSize);
		simple_state_free(((VM *) pointer)->sState,cLine);
	} else {
		SIMPLE_API_ERROR("Error in first parameter,  input size < 1 !");
	}
}

void simple_vmlib_ascii ( void *pointer )
{
	int x  ;
	unsigned char *cString  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		if ( SIMPLE_API_GETSTRINGSIZE(1) == 1 ) {
			cString = (unsigned char *) SIMPLE_API_GETSTRING(1) ;
			x = (int) cString[0] ;
			SIMPLE_API_RETNUMBER(x);
		} else {
			SIMPLE_API_ERROR("Error in first parameter, the string size is not one letter !");
			return ;
		}
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_char ( void *pointer )
{
	int x  ;
	char cStr[2]  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISNUMBER(1) ) {
		x = SIMPLE_API_GETNUMBER(1) ;
		cStr[0] = (char) x ;
		cStr[1] = '\0' ;
		SIMPLE_API_RETSTRING2(cStr,1);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_date ( void *pointer )
{
	time_t timer  ;
	char buffer[25]  ;
	struct tm*tm_info  ;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer,25,"%d/%m/%Y", tm_info);
	SIMPLE_API_RETSTRING(buffer);
}

void simple_vmlib_time ( void *pointer )
{
	time_t timer  ;
	char buffer[25]  ;
	struct tm*tm_info  ;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer,25,"%H:%M:%S", tm_info);
	SIMPLE_API_RETSTRING(buffer);
}

void simple_vmlib_getchar ( void *pointer )
{
	char cStr[2]  ;
	SIMPLE_SETBINARY ;
	cStr[0] = getchar() ;
	SIMPLE_API_RETSTRING2(cStr,1);
}

void simple_vmlib_random ( void *pointer )
{
	int nNum1,nNum2  ;
	nNum1 = rand() ;
	if ( SIMPLE_API_PARACOUNT == 0 ) {
		SIMPLE_API_RETNUMBER(nNum1);
	}
	else if ( SIMPLE_API_PARACOUNT == 1 ) {
		if ( SIMPLE_API_ISNUMBER(1) ) {
			nNum2 = SIMPLE_API_GETNUMBER(1) ;
			if ( nNum2 > 0 ) {
				SIMPLE_API_RETNUMBER(nNum1 % ++nNum2);
			}
		}
		else {
			SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
		}
	}
	else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARACOUNT);
	}
}

void simple_vmlib_timelist ( void *pointer )
{
	time_t timer  ;
	char buffer[25]  ;
	struct tm*tm_info  ;
	List *list  ;
	list = SIMPLE_API_NEWLIST ;
	time(&timer);
	tm_info = localtime(&timer);
	/*
	**  Add List Items 
	**  abbreviated weekday name 
	*/
	strftime(buffer,25,"%a", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* full weekday name */
	strftime(buffer,25,"%A", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* abbreviated month name */
	strftime(buffer,25,"%b", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* full month name */
	strftime(buffer,25,"%B", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Date & Time */
	strftime(buffer,25,"%c", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Day of the month */
	strftime(buffer,25,"%d", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Hour (24) */
	strftime(buffer,25,"%H", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Hour (12) */
	strftime(buffer,25,"%I", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Day of the year */
	strftime(buffer,25,"%j", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Month of the year */
	strftime(buffer,25,"%m", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Minutes after hour */
	strftime(buffer,25,"%M", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* AM or PM */
	strftime(buffer,25,"%p", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Seconds after the hour */
	strftime(buffer,25,"%S", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* Week of the year (sun-sat) */
	strftime(buffer,25,"%U", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* day of the week */
	strftime(buffer,25,"%w", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* date */
	strftime(buffer,25,"%x", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* time */
	strftime(buffer,25,"%X", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* year of the century */
	strftime(buffer,25,"%y", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* year */
	strftime(buffer,25,"%Y", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* time zone */
	strftime(buffer,25,"%Z", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	/* percent sign */
	strftime(buffer,25,"%%", tm_info);
	simple_list_addstring_gc(((VM *) pointer)->sState,list,buffer);
	SIMPLE_API_RETLIST(list);
}

void simple_vmlib_adddays ( void *pointer )
{
	const char *cStr  ;
	char buffer[25]  ;
	int x,nDay,nMonth,nYear,nDaysInMonth  ;
	int aDaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARACOUNT);
		return ;
	}
	if ( ! (SIMPLE_API_ISSTRING(1) && SIMPLE_API_ISNUMBER(2)) ) {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
		return ;
	}
	cStr = SIMPLE_API_GETSTRING(1);
	if ( (SIMPLE_API_GETSTRINGSIZE(1) == 10) ) {
		if ( isalnum(cStr[0]) && isalnum(cStr[1]) && isalnum(cStr[3]) && isalnum(cStr[4]) && isalnum(cStr[6]) && isalnum(cStr[7]) && isalnum(cStr[8]) && isalnum(cStr[9]) ) {
			sprintf( buffer , "%c%c" , cStr[0],cStr[1] ) ;
			nDay = atoi(buffer) + ((int) SIMPLE_API_GETNUMBER(2)) ;
			sprintf( buffer , "%c%c" , cStr[3],cStr[4] ) ;
			nMonth = atoi(buffer) ;
			sprintf( buffer , "%c%c%c%c" , cStr[6],cStr[7],cStr[8],cStr[9] ) ;
			nYear = atoi(buffer) ;
			/* Fix Day Number */
			nDaysInMonth = aDaysInMonth[nMonth-1] ;
			/* Fix Leap Year */
			if ( (nMonth == 2) && (simple_vmlib_adddays_isleapyear(nYear)) ) {
				nDaysInMonth = 29 ;
			}
			while ( nDay > nDaysInMonth ) {
				nDay = nDay - nDaysInMonth ;
				nMonth++ ;
				if ( nMonth == 13 ) {
					nMonth = 1 ;
					nYear++ ;
				}
				nDaysInMonth = aDaysInMonth[nMonth-1] ;
				/* Fix Leap Year */
				if ( (nMonth == 2) && (simple_vmlib_adddays_isleapyear(nYear)) ) {
					nDaysInMonth = 29 ;
				}
			}
			while ( nDay < 1 ) {
				nMonth-- ;
				if ( nMonth == 0 ) {
					nMonth = 12 ;
					nYear-- ;
				}
				nDaysInMonth = aDaysInMonth[nMonth-1] ;
				/* Fix Leap Year */
				if ( (nMonth == 2) && (simple_vmlib_adddays_isleapyear(nYear)) ) {
					nDaysInMonth = 29 ;
				}
				nDay = nDaysInMonth - abs(nDay) ;
			}
			sprintf(buffer,"%2d/%2d/%4d", nDay,nMonth,nYear);
			for ( x = 0 ; x <= 9 ; x++ ) {
				if ( buffer[x] == ' ' ) {
					buffer[x] = '0' ;
				}
			}
			SIMPLE_API_RETSTRING(buffer);
			return ;
		}
	}
	SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	return ;
}

void simple_vmlib_diffdays ( void *pointer )
{
	const char *cStr, *cStr2  ;
	struct tm tm_info,tm_info2  ;
	time_t timer,timer2  ;
	char buffer[5]  ;
	double nResult  ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARACOUNT);
		return ;
	}
	if ( ! (SIMPLE_API_ISSTRING(1) && SIMPLE_API_ISSTRING(2)) ) {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
		return ;
	}
	cStr = SIMPLE_API_GETSTRING(1);
	cStr2 = SIMPLE_API_GETSTRING(2);
	if ( (SIMPLE_API_GETSTRINGSIZE(1) == 10) && (SIMPLE_API_GETSTRINGSIZE(2) == 10) ) {
		if ( isalnum(cStr[0]) && isalnum(cStr[1]) && isalnum(cStr[3]) && isalnum(cStr[4]) && isalnum(cStr[6]) && isalnum(cStr[7]) && isalnum(cStr[8]) && isalnum(cStr[9]) ) {
			tm_info.tm_hour = 0 ;
			tm_info.tm_min = 0 ;
			tm_info.tm_sec = 0 ;
			sprintf( buffer , "%c%c" , cStr[0],cStr[1] ) ;
			tm_info.tm_mday = atoi(buffer) ;
			sprintf( buffer , "%c%c" , cStr[3],cStr[4] ) ;
			tm_info.tm_mon = atoi(buffer)-1 ;
			sprintf( buffer , "%c%c%c%c" , cStr[6],cStr[7],cStr[8],cStr[9] ) ;
			tm_info.tm_year = atoi(buffer) - 1900 ;
			timer = mktime(&tm_info);
			if ( tm_info.tm_year > 1097 ) {
				/*
				**  1097 + 1900 = 2997 
				**  Values over limit may cause crash 
				*/
				SIMPLE_API_ERROR(SIMPLE_API_BADPARARANGE);
				return ;
			}
			if ( isalnum(cStr2[0]) && isalnum(cStr2[1]) && isalnum(cStr2[3]) && isalnum(cStr2[4]) && isalnum(cStr2[6]) && isalnum(cStr2[7]) && isalnum(cStr2[8]) && isalnum(cStr2[9]) ) {
				tm_info2.tm_hour = 0 ;
				tm_info2.tm_min = 0 ;
				tm_info2.tm_sec = 0 ;
				sprintf( buffer , "%c%c" , cStr2[0],cStr2[1] ) ;
				tm_info2.tm_mday = atoi(buffer) ;
				sprintf( buffer , "%c%c" , cStr2[3],cStr2[4] ) ;
				tm_info2.tm_mon = atoi(buffer)-1 ;
				sprintf( buffer , "%c%c%c%c" , cStr2[6],cStr2[7],cStr2[8],cStr2[9] ) ;
				tm_info2.tm_year = atoi(buffer) - 1900 ;
				timer2 = mktime(&tm_info2);
				if ( tm_info2.tm_year > 1097 ) {
					/*
					**  1097 + 1900 = 2997 
					**  Values over limit may cause crash 
					*/
					SIMPLE_API_ERROR(SIMPLE_API_BADPARARANGE);
					return ;
				}
				nResult = difftime(timer,timer2);
				nResult  = ceil(ceil(nResult) / 86400 ) ;
				SIMPLE_API_RETNUMBER(nResult);
				return ;
			}
		}
	}
	SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	return ;
}

void simple_vmlib_version ( void *pointer )
{
	SIMPLE_API_RETSTRING(SIMPLE_VERSION);
}

void simple_vmlib_clockspersecond ( void *pointer )
{
	SIMPLE_API_RETNUMBER(CLOCKS_PER_SEC);
}

int simple_vmlib_adddays_isleapyear ( int nYear )
{
	if ( nYear%400 == 0 ) {
		return 1 ;
	}
	if ( nYear%100 == 0 ) {
		return 0 ;
	}
	return nYear % 4 == 0 ;
}

void simple_vmlib_swap ( void *pointer )
{
	List *list  ;
	int nNum1,nNum2,nSize  ;
	if ( SIMPLE_API_PARACOUNT != 3 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS3PARA);
		return ;
	}
	if ( SIMPLE_API_ISLIST(1) ) {
		list = SIMPLE_API_GETLIST(1) ;
		if ( SIMPLE_API_ISNUMBER(2)  && SIMPLE_API_ISNUMBER(3) ) {
			nNum1 = (int) SIMPLE_API_GETNUMBER(2) ;
			nNum2 = (int) SIMPLE_API_GETNUMBER(3) ;
			nSize = simple_list_getsize(list);
			if ( (nNum1 > 0) && (nNum2 > 0) && (nNum1!= nNum2) && (nNum1<= nSize) && (nNum2 <= nSize) ) {
				simple_list_swap(list,nNum1, nNum2);
			} else {
				SIMPLE_API_ERROR(SIMPLE_API_BADPARARANGE);
			}
		} else {
			SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
		}
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

/* Check Data Type */

void simple_vmlib_isstring ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		SIMPLE_API_RETNUMBER(1);
	} else {
		SIMPLE_API_RETNUMBER(0);
	}
}

void simple_vmlib_isnumber ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISNUMBER(1) ) {
		SIMPLE_API_RETNUMBER(1);
	} else {
		SIMPLE_API_RETNUMBER(0);
	}
}

void simple_vmlib_islist ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISLIST(1) ) {
		if ( simple_vm_oop_isobject(SIMPLE_API_GETLIST(1) ) == 0 ) {
			SIMPLE_API_RETNUMBER(1);
			return ;
		}
	}
	SIMPLE_API_RETNUMBER(0);
}

void simple_vmlib_type ( void *pointer )
{
	List *list  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	/* The order of checking C Pointer and OBJECT before List is important because the list can be both of them */
	if ( SIMPLE_API_ISSTRING(1) ) {
		SIMPLE_API_RETSTRING("STRING");
	}
	else if ( SIMPLE_API_ISNUMBER(1) ) {
		SIMPLE_API_RETSTRING("NUMBER");
	}
	else if ( SIMPLE_API_ISCPOINTER(1) ) {
		list = SIMPLE_API_GETLIST(1) ;
		SIMPLE_API_RETSTRING(simple_list_getstring(list,SIMPLE_CPOINTER_TYPE));
	}
	else if ( SIMPLE_API_ISOBJECT(1) ) {
		SIMPLE_API_RETSTRING("OBJECT");
	}
	else if ( SIMPLE_API_ISLIST(1) ) {
		SIMPLE_API_RETSTRING("LIST");
	} else {
		SIMPLE_API_RETSTRING("UNKNOWN");
	}
}

void simple_vmlib_isnull ( void *pointer )
{
	char *cStr  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		if ( strcmp(SIMPLE_API_GETSTRING(1),"") == 0 ) {
			SIMPLE_API_RETNUMBER(1);
			return ;
		}
		else if ( SIMPLE_API_GETSTRINGSIZE(1) == 4 ) {
			cStr = SIMPLE_API_GETSTRING(1) ;
			if ( (cStr[0] == 'n' || cStr[0] == 'N') && (cStr[1] == 'u' || cStr[1] == 'U') && (cStr[2] == 'l' || cStr[2] == 'L') && (cStr[3] == 'l' || cStr[3] == 'L') ) {
				SIMPLE_API_RETNUMBER(1);
				return ;
			}
		}
	}
	else if ( SIMPLE_API_ISPOINTER(1) ) {
		if ( SIMPLE_API_GETPOINTER(1) == NULL ) {
			SIMPLE_API_RETNUMBER(1);
			return ;
		}
	}
	SIMPLE_API_RETNUMBER(0);
}

void simple_vmlib_isobject ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISLIST(1) ) {
		if ( simple_vm_oop_isobject(SIMPLE_API_GETLIST(1) ) == 1 ) {
			SIMPLE_API_RETNUMBER(1);
			return ;
		}
	}
	SIMPLE_API_RETNUMBER(0);
}
/* Conversion */

void simple_vmlib_hex ( void *pointer )
{
	char cStr[100]  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISNUMBER(1) ) {
		sprintf( cStr , "%x" , (int) SIMPLE_API_GETNUMBER(1) ) ;
		SIMPLE_API_RETSTRING(cStr);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_dec ( void *pointer )
{
	unsigned int x  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		sscanf(SIMPLE_API_GETSTRING(1),"%x",&x);
		SIMPLE_API_RETNUMBER(x);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_number ( void *pointer )
{
	double x  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		x = simple_vm_stringtonum((VM *) pointer,SIMPLE_API_GETSTRING(1));
		SIMPLE_API_RETNUMBER(x);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_string ( void *pointer )
{
	double nNum1  ;
	char cStr[100]  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISNUMBER(1) ) {
		nNum1 = SIMPLE_API_GETNUMBER(1);
		simple_vm_numtostring((VM *) pointer, nNum1,cStr);
		SIMPLE_API_RETSTRING(cStr);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_str2hex ( void *pointer )
{
	char cStr[3]  ;
	unsigned char *cString  ;
	int x,nMax  ;
	char *cString2  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		cString = (unsigned char *) SIMPLE_API_GETSTRING(1) ;
		nMax = SIMPLE_API_GETSTRINGSIZE(1) ;
		cString2 = (char *) simple_state_malloc(((VM *) pointer)->sState,nMax*2);
		if ( cString2 == NULL ) {
			SIMPLE_API_ERROR(SIMPLE_OOM);
			return ;
		}
		for ( x = 1 ; x <= nMax ; x++ ) {
			sprintf( cStr , "%x" , (unsigned int) cString[x-1] ) ;
			cString2[(x-1)*2] = cStr[0] ;
			if ( cStr[1] != '\0' ) {
				cString2[((x-1)*2)+1] = cStr[1] ;
			} else {
				cString2[((x-1)*2)+1] = ' ' ;
			}
		}
		SIMPLE_API_RETSTRING2(cString2,nMax*2);
		simple_state_free(((VM *) pointer)->sState,cString2);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_hex2str ( void *pointer )
{
	char cStr[3]  ;
	const char *cString  ;
	char *cString2  ;
	int x,i,nMax  ;
	unsigned int y  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		cString = SIMPLE_API_GETSTRING(1) ;
		nMax = SIMPLE_API_GETSTRINGSIZE(1) ;
		cString2 = (char *) simple_state_malloc(((VM *) pointer)->sState,(nMax/2)+1);
		if ( cString2 == NULL ) {
			SIMPLE_API_ERROR(SIMPLE_OOM);
			return ;
		}
		i = 0 ;
		for ( x = 0 ; x < nMax ; x+=2 ) {
			cStr[0] = cString[x] ;
			if ( cString[x+1]   != ' ' ) {
				cStr[1] = cString[x+1] ;
				cStr[2] = '\0' ;
			} else {
				cStr[1] = '\0' ;
			}
			sscanf(cStr,"%x",&y);
			cString2[i] = y ;
			i++ ;
		}
		SIMPLE_API_RETSTRING2(cString2,nMax/2);
		simple_state_free(((VM *) pointer)->sState,cString2);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_str2list ( void *pointer )
{
	char *cStr  ;
	int x,nSize,nStart  ;
	List *list  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		cStr = SIMPLE_API_GETSTRING(1) ;
		nSize = SIMPLE_API_GETSTRINGSIZE(1) ;
		nStart = 0 ;
		list = SIMPLE_API_NEWLIST ;
		for ( x = 0 ; x < nSize ; x++ ) {
			if ( cStr[x] == '\n' ) {
				if ( x > nStart ) {
					if ( cStr[x-1] == '\r' ) {
						simple_list_addstring2_gc(((VM *) pointer)->sState,list,cStr+nStart,x-nStart-1);
					}
					else {
						simple_list_addstring2_gc(((VM *) pointer)->sState,list,cStr+nStart,x-nStart);
					}
				} else {
					simple_list_addstring_gc(((VM *) pointer)->sState,list,"");
				}
				nStart = x+1 ;
			}
		}
		if ( nSize > nStart ) {
			simple_list_addstring2_gc(((VM *) pointer)->sState,list,cStr+nStart,nSize-nStart);
		}
		SIMPLE_API_RETLIST(list);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_list2str ( void *pointer )
{
	List *list  ;
	String *pString  ;
	int x  ;
	char cStr[100]  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISLIST(1) ) {
		list = SIMPLE_API_GETLIST(1) ;
		pString = simple_string_new_gc(((VM *) pointer)->sState,"");
		for ( x = 1 ; x <= simple_list_getsize(list) ; x++ ) {
			if ( simple_list_isstring(list,x) ) {
				if ( x != 1 ) {
					simple_string_add_gc(((VM *) pointer)->sState,pString,"\n");
				}
				simple_string_add_gc(((VM *) pointer)->sState,pString,simple_list_getstring(list,x));
			}
			else if ( simple_list_isnumber(list,x) ) {
				if ( x != 1 ) {
					simple_string_add_gc(((VM *) pointer)->sState,pString,"\n");
				}
				simple_vm_numtostring((VM *) pointer,simple_list_getdouble(list,x) ,cStr);
				simple_string_add_gc(((VM *) pointer)->sState,pString,cStr);
			}
		}
		SIMPLE_API_RETSTRING(simple_string_get(pString));
		simple_string_delete_gc(((VM *) pointer)->sState,pString);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_str2hexcstyle ( void *pointer )
{
	char cStr[3]  ;
	unsigned char *cString  ;
	int x,nMax  ;
	char *cString2  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		cString = (unsigned char *) SIMPLE_API_GETSTRING(1) ;
		nMax = SIMPLE_API_GETSTRINGSIZE(1) ;
		cString2 = (char *) simple_state_malloc(((VM *) pointer)->sState,nMax*5);
		if ( cString2 == NULL ) {
			SIMPLE_API_ERROR(SIMPLE_OOM);
			return ;
		}
		for ( x = 1 ; x <= nMax ; x++ ) {
			sprintf( cStr , "%x" , (unsigned int) cString[x-1] ) ;
			/* Separator */
			cString2[(x-1)*5] = ',' ;
			cString2[(x-1)*5+1] = '0' ;
			cString2[(x-1)*5+2] = 'x' ;
			cString2[(x-1)*5+3] = cStr[0] ;
			if ( cStr[1] != '\0' ) {
				cString2[((x-1)*5)+4] = cStr[1] ;
			} else {
				cString2[((x-1)*5)+4] = ' ' ;
			}
		}
		/* Pass the first letter to avoid the first comma */
		cString2++ ;
		SIMPLE_API_RETSTRING2(cString2,nMax*5-1);
		/* When we call free() we use the original pointer */
		cString2-- ;
		simple_state_free(((VM *) pointer)->sState,cString2);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

/* Functional */

void simple_vmlib_eval ( void *pointer )
{
	const char *cStr  ;
	VM *vm  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		cStr = SIMPLE_API_GETSTRING(1);
		vm = (VM *) pointer ;
		vm->nEvalCalledFromSimpleCode = 1 ;
		if ( simple_vm_eval(vm,cStr) == 0 ) {
			vm->nEvalCalledFromSimpleCode = 0 ;
		}
		/*
		**  The CALL instruction will check nEvalCalledFromSimpleCode to execute the main loop again 
		**  Before executing the main loop again, The CALL instruction will set nEvalCalledFromSimpleCode to 0 
		*/
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}
/* Error Handling */

void simple_vmlib_raise ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		SIMPLE_API_ERROR(SIMPLE_API_GETSTRING(1));
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_assert ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISNUMBER(1) ) {
		if ( SIMPLE_API_GETNUMBER(1) != 1 ) {
			SIMPLE_API_ERROR("Assertion Failed!");
			return ;
		}
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}
/* Check Characters */

void simple_vmlib_isfunc ( void *pointer,int (*pFunc)(int) )
{
	char *cStr  ;
	int nSize,x  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARACOUNT);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		cStr = SIMPLE_API_GETSTRING(1) ;
		nSize = SIMPLE_API_GETSTRINGSIZE(1) ;
		for ( x = 0 ; x < nSize ; x++ ) {
			if ( ! (*pFunc)(cStr[x]) ) {
				SIMPLE_API_RETNUMBER(0);
				return ;
			}
		}
		SIMPLE_API_RETNUMBER(1);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_isalnum ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isalnum);
}

void simple_vmlib_isalpha ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isalpha);
}

void simple_vmlib_iscntrl ( void *pointer )
{
	simple_vmlib_isfunc(pointer,iscntrl);
}

void simple_vmlib_isdigit ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isdigit);
}

void simple_vmlib_isgraph ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isgraph);
}

void simple_vmlib_islower ( void *pointer )
{
	simple_vmlib_isfunc(pointer,islower);
}

void simple_vmlib_isprint ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isprint);
}

void simple_vmlib_ispunct ( void *pointer )
{
	simple_vmlib_isfunc(pointer,ispunct);
}

void simple_vmlib_isspace ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isspace);
}

void simple_vmlib_isupper ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isupper);
}

void simple_vmlib_isxdigit ( void *pointer )
{
	simple_vmlib_isfunc(pointer,isxdigit);
}
/* Garbage Collector & Variable Pointer */

void simple_vmlib_callgc ( void *pointer )
{
	simple_vm_gc_deletetemlists((VM *) pointer);
}

void simple_vmlib_varptr ( void *pointer )
{
	const char *cStr , *cStr2  ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) && SIMPLE_API_ISSTRING(2) ) {
		cStr = SIMPLE_API_GETSTRING(1);
		cStr2 = SIMPLE_API_GETSTRING(2);
		SIMPLE_API_RETCPOINTER(api_var_ptr(pointer,cStr,cStr2),cStr2);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_intvalue ( void *pointer )
{
	const char *cStr  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISSTRING(1) ) {
		cStr = SIMPLE_API_GETSTRING(1);
		api_int_value(pointer,cStr);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_object2pointer ( void *pointer )
{
	List *list  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( ! SIMPLE_API_ISLIST(1) ) {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
	list = SIMPLE_API_GETLIST(1) ;
	SIMPLE_API_RETCPOINTER((void *) list,"OBJECTPOINTER");
}

void simple_vmlib_pointer2object ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( ! SIMPLE_API_ISPOINTER(1) ) {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
	SIMPLE_API_RETLIST((List *) SIMPLE_API_GETCPOINTER(1,"OBJECTPOINTER"));
}

void simple_vmlib_nullpointer ( void *pointer )
{
	SIMPLE_API_RETCPOINTER(NULL,"NULLPOINTER");
}

void simple_vmlib_space ( void *pointer )
{
	String *pString  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISNUMBER(1) ) {
		pString = simple_string_new2_gc(((VM *) pointer)->sState,"",SIMPLE_API_GETNUMBER(1));
		SIMPLE_API_RETSTRING2(simple_string_get(pString),SIMPLE_API_GETNUMBER(1));
		simple_string_delete_gc(((VM *) pointer)->sState,pString);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vmlib_ptrcmp ( void *pointer )
{
	List *list, *list2  ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	if ( SIMPLE_API_ISCPOINTER(1) && SIMPLE_API_ISCPOINTER(2) ) {
		list = SIMPLE_API_GETLIST(1) ;
		list2 = SIMPLE_API_GETLIST(2) ;
		SIMPLE_API_RETNUMBER(api_cpointer_cmp(list,list2));
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}
/* Simple State */

void init_simple_state_block ( void *pointer )
{
	SIMPLE_API_RETCPOINTER(init_simple_state(),"SIMPLESTATE");
}

void execute_simple_code_block ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	execute_simple_code((SimpleState *) SIMPLE_API_GETCPOINTER(1,"SIMPLESTATE"),SIMPLE_API_GETSTRING(2));
}

void free_simple_state_block ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	free_simple_state((SimpleState *) SIMPLE_API_GETCPOINTER(1,"SIMPLESTATE"));
}

void execute_simple_file_block ( void *pointer )
{
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	execute_simple_file((SimpleState *) SIMPLE_API_GETCPOINTER(1,"SIMPLESTATE"),SIMPLE_API_GETSTRING(2));
}

void find_simple_variable_block ( void *pointer )
{
	List *list  ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	list = simple_state_findvar((SimpleState *) SIMPLE_API_GETCPOINTER(1,"SIMPLESTATE"),SIMPLE_API_GETSTRING(2));
	SIMPLE_API_RETLIST(list);
}

void simple_vmlib_state_newvar ( void *pointer )
{
	List *list  ;
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	list = simple_state_newvar((SimpleState *) SIMPLE_API_GETCPOINTER(1,"SIMPLESTATE"),SIMPLE_API_GETSTRING(2));
	SIMPLE_API_RETLIST(list);
}

void main_simple_state_block ( void *pointer )
{
	char *cStr  ;
	int argc  ;
	char *argv[2]  ;
	argv[0] = (char *) simple_state_malloc(((VM *) pointer)->sState,100);
	argv[1] = (char *) simple_state_malloc(((VM *) pointer)->sState,100);
	cStr = SIMPLE_API_GETSTRING(1);
	argc = 2 ;
	strcpy(argv[0],"ring");
	strcpy(argv[1],cStr);
	simple_execute(cStr,0,1,0,0,0,0,0,0,0,argc,argv);
	simple_state_free(((VM *) pointer)->sState,argv[0]);
	simple_state_free(((VM *) pointer)->sState,argv[1]);
}

void simple_vmlib_state_setvar ( void *pointer )
{
	List *list, *list2, *list3  ;
	VM *vm  ;
	vm = (VM *) pointer ;
	if ( SIMPLE_API_PARACOUNT != 3 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS3PARA);
		return ;
	}
	list = simple_state_findvar((SimpleState *) SIMPLE_API_GETCPOINTER(1,"SIMPLESTATE"),SIMPLE_API_GETSTRING(2));
	/* Check Variable before usage */
	if ( list==NULL ) {
		SIMPLE_API_ERROR("Variable doesn't exist!");
		return ;
	}
	if ( SIMPLE_API_ISSTRING(3) ) {
		simple_list_setint_gc(vm->sState,list, SIMPLE_VAR_TYPE ,SIMPLE_VM_STRING);
		simple_list_setstring2_gc(vm->sState,list, SIMPLE_VAR_VALUE , SIMPLE_API_GETSTRING(3),SIMPLE_API_GETSTRINGSIZE(3));
	}
	else if ( SIMPLE_API_ISNUMBER(3) ) {
		simple_list_setint_gc(vm->sState,list, SIMPLE_VAR_TYPE ,SIMPLE_VM_NUMBER);
		simple_list_setdouble_gc(vm->sState,list, SIMPLE_VAR_VALUE ,SIMPLE_API_GETNUMBER(3));
	}
	else if ( SIMPLE_API_ISLIST(3) ) {
		list2 = SIMPLE_API_GETLIST(3) ;
		simple_list_setint_gc(vm->sState,list, SIMPLE_VAR_TYPE ,SIMPLE_VM_LIST);
		simple_list_setlist_gc(vm->sState,list, SIMPLE_VAR_VALUE);
		list3 = simple_list_getlist(list,SIMPLE_VAR_VALUE);
		simple_list_copy(list3,list2);
	}
}

void simple_vmlib_state_new ( void *pointer )
{
	SIMPLE_API_RETCPOINTER(simple_state_new(),"SIMPLESTATE");
}

void simple_vmlib_state_mainfile ( void *pointer )
{
	SimpleState *sState  ;
	char *cStr  ;
	int argc  ;
	char *argv[2]  ;
	argv[0] = (char *) simple_state_malloc(((VM *) pointer)->sState,100);
	argv[1] = (char *) simple_state_malloc(((VM *) pointer)->sState,100);
	if ( SIMPLE_API_PARACOUNT != 2 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS2PARA);
		return ;
	}
	sState = (SimpleState *) SIMPLE_API_GETCPOINTER(1,"SIMPLESTATE") ;
	cStr = SIMPLE_API_GETSTRING(2);
	argc = 2 ;
	strcpy(argv[0],"ring");
	strcpy(argv[1],cStr);
	sState->argc = argc ;
	sState->argv = argv ;
	/*
	**  Don't Delete the VM after execution 
	**  In this case the caller already called qApp.Exec() 
	**  Deleting the VM in sub program after execution 
	**  Will lead to crash when we execute events (like button click) in the sub program 
	**  So we keep the VM to avoid the Crash 
	*/
	sState->nDontDeleteTheVM = 1 ;
	simple_scanner_readfile(sState,cStr);
}


