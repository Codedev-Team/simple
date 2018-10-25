
/* 
	Copyright (c) 2017-2018 Azeez Adewale <azeezadewale98@gmail.com> 
	MIT License Copyright (c) 2017 simple 

*/

/* 
 * File:   simple.h
 * Author: thecarisma
 *
 * Created on July 10, 2017, 1:10 PM
 */


#include "../includes/simple.h"
#include "../includes/simple_vmdll.h"

/* Blocks */

void simple_vm_dll_loadblocks ( SimpleState *sState )
{
	register_block("loadDynamicLibrary",simple_vm_dll_loadlib);
	register_block("callDynamicFunction",simple_vm_dll_calllib_function);
	
	register_block("callDynamicModule",simple_vm_dll_calllib);
	register_block("closeDynamicModule",simple_vm_dll_closelib);
}

void simple_vm_dll_loadlib ( void *pointer )
{
	LpHandleType handle  ;
	char library_path[200]  ; 
	if ( SIMPLE_API_PARACOUNT != 1 ) {
            SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
            return ;
    }
    if ( SIMPLE_API_ISSTRING(1) ) {
		strcpy(library_path,SIMPLE_API_GETSTRING(1));
		handle = LoadDLL(library_path);
		if ( handle == NULL ) {
                printf( "\nCannot load dynamic library : %s",library_path) ;
                SIMPLE_API_ERROR(SIMPLE_VM_ERROR_LIBLOADERROR);
                return ;
        } else {
			SIMPLE_API_RETCPOINTER(handle,"SIMPLE_DYNAMIC_LIBRARY_");
		}		
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void* call_func(void* address, List* parameters) {
	int size ;
	size = simple_list_getsize(parameters);
	printf("The list size is %i \n",size);
}

void simple_vm_dll_calllib_function ( void *pointer )
{
	LpHandleType handle  ;
	lp address ;
	char library_path[200]  ; 
	if ( SIMPLE_API_PARACOUNT != 3 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS3PARA);
		return ;
	}
	if ( SIMPLE_API_ISPOINTER(1) && SIMPLE_API_ISSTRING(2) && SIMPLE_API_ISLIST(3) ) {
		strcpy(library_path,SIMPLE_API_GETSTRING(2));
		handle = SIMPLE_API_GETCPOINTER(1,"SIMPLE_DYNAMIC_LIBRARY_") ;
		address = (lp) GetDLLBlock(handle, library_path) ;
        if ( address == NULL ) {
			printf( "\nCannot call the function : %s", library_path ) ;
			SIMPLE_API_ERROR("Error occur while calling the a function");
			return ;
		} else {
			void* returnValue = call_func(address,SIMPLE_API_GETLIST(3));
			//SIMPLE_API_RETNUMBER(returnValue);
		}
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

void simple_vm_dll_calllib ( void *pointer )
{
    LpHandleType handle  ;
    const char *cDLL  ;
    char library_path[200]  ; 
	char __library_path[SIMPLE_PATHSIZE]  ;
	char simple_folder[100] ;
    loadlibblockptr pBlock  ;
    VM *vm  ;
    SimpleState *sState  ;
    vm = (VM *) pointer ;
    sState = vm->sState ;
	strcpy(library_path,SIMPLE_API_GETSTRING(1));
    if ( SIMPLE_API_PARACOUNT != 1 ) {
            SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
            return ;
    }
    if ( SIMPLE_API_ISSTRING(1) ) {
        if (simple_fexists(library_path)) {
            strcpy(library_path,library_path);
        } else { 
			snprintf(__library_path, sizeof(__library_path), "%s/modules/dynamic_modules/%s", simple_file_initial_dir,library_path);
			if (simple_fexists(__library_path)) {
				strcpy(library_path,__library_path);
			} else {
				snprintf(__library_path, sizeof(__library_path), "./modules/dynamic_modules/%s", library_path);
				if (simple_fexists(__library_path)) {
					strcpy(library_path,__library_path);
				} else {
					simple_distro_folder(simple_folder); 
					snprintf(__library_path, sizeof(__library_path), "%s/modules/dynamic_modules/%s", simple_folder,library_path);
					if (simple_fexists(__library_path)) {
							strcpy(library_path,__library_path);
					} else {
						//checking using environment variable if SIMPLE_PATH and SIMPLE_MODULE_PATH are set
						char* simple_env_path = getenv("SIMPLE_PATH");  snprintf(__library_path, sizeof(__library_path), "%s/s%s/modules/dynamic_modules/%s", simple_env_path, SIMPLE_VERSION, library_path); 
						if (simple_fexists(__library_path)) { strcpy(library_path,__library_path); }
						else {
							char* simple_env_path = getenv("SIMPLE_MODULE_PATH"); snprintf(__library_path, sizeof(__library_path), "%s/dynamic_modules/%s", simple_env_path, library_path);
							if (simple_fexists(__library_path)) { strcpy(library_path,__library_path);}
							else {
								//find the module in relative to run folder (UNDONE) //this is last
								#ifdef _WIN32
									snprintf(__library_path, sizeof(__library_path), "C:/Simple/s%s/modules/dynamic_modules/%s",SIMPLE_VERSION,library_path);
								#else
									snprintf(__library_path, sizeof(__library_path), "/var/lib/simple/s%s/modules/dynamic_modules/%s", SIMPLE_VERSION,library_path);
								#endif
								if (simple_fexists(__library_path)) { strcpy(library_path,__library_path);}
								else {
									/* Now we assume it is executed in a folder bin and the modules is in parent folder 
										like
										simple/bin/
										simple/modules/
										simple/includes/
									so we go parent directory *simple* and check for modules. Think execute simple from zip extract
									*/
									snprintf(__library_path, sizeof(__library_path), "../modules/dynamic_modules/%s", library_path);
									if (simple_fexists(__library_path)) { strcpy(library_path,__library_path);}
									else {
                                        /* We dig deep for android and ios we first check the assets folder then the storage*/
										#ifdef __ANDROID__
											//check android asset first
											snprintf(__library_path, sizeof(__library_path), "%s/%s", external_data_path, library_path);
											if (simple_fexists(__library_path)) { strcpy(library_path,__library_path);}
											else {
												snprintf(__library_path, sizeof(__library_path), "%s/dynamic_modules/%s", external_data_path, library_path);
												if (simple_fexists(__library_path)) { strcpy(library_path,__library_path);}
												else {
													//now check the sdcard (External Storage)
													simple_env_path = getenv("EXTERNAL_STORAGE");
													snprintf(__library_path, sizeof(__library_path), "%s/simple/s%s/modules/dynamic_modules/%s", simple_env_path, SIMPLE_VERSION,library_path);
													if (simple_fexists(__library_path)) { strcpy(library_path,__library_path);}
													else {
														snprintf(__library_path, sizeof(__library_path), "%s/simple/modules/dynamic_modules/%s", simple_env_path, library_path);
														if (simple_fexists(__library_path)) { strcpy(library_path,__library_path);}
														else {

														}
													}
												}
											}
                                        #endif
									}
								}
							}
						} 
					}
				}
			}
        }
        cDLL = library_path;
        handle = LoadDLL(cDLL);
        if ( handle == NULL ) {
                printf( "\nLibrary File : %s",library_path ) ;
                SIMPLE_API_ERROR(SIMPLE_VM_ERROR_LIBLOADERROR);
                return ;
        } 
        pBlock = (loadlibblockptr) GetDLLBlock(handle, "init_simple_module") ;
        if ( pBlock == NULL ) {
                printf( "\nLibrary File : %s", file_real_name(library_path) ) ;
                SIMPLE_API_ERROR("The dynamic library doesn't contain the init_simple_module() block!");
                return ;
        } 
        simple_list_deletearray_gc(sState,sState->c_blocks);
        (*pBlock)(sState) ;
        simple_list_genarray_gc(sState,sState->c_blocks);
        simple_list_genhashtable2_gc(sState,sState->c_blocks);
        SIMPLE_API_RETCPOINTER(handle,"SIMPLE_DYNAMIC_LIBRARY_");
    } else {
        SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
    }
}

void simple_vm_dll_closelib ( void *pointer )
{
	LpHandleType handle  ;
	if ( SIMPLE_API_PARACOUNT != 1 ) {
		SIMPLE_API_ERROR(SIMPLE_API_MISS1PARA);
		return ;
	}
	if ( SIMPLE_API_ISPOINTER(1) ) {
		handle = SIMPLE_API_GETCPOINTER(1,"SIMPLE_DYNAMIC_LIBRARY_") ;
		CloseDLL(handle);
	} else {
		SIMPLE_API_ERROR(SIMPLE_API_BADPARATYPE);
	}
}

/* User Interface - Commands Implementation (Faster) - Because we don't have blocks call */

void simple_vm_display ( VM *vm )
{
	Item *pItem  ;
	char cStr[100]  ;
	List *list  ;
	char *cString  ;
	int x  ;
	if ( vm->nBlockExecute > 0 ) {
		vm->nBlockExecute-- ;
	}
	if ( SIMPLE_VM_STACK_ISSTRING ) {
		cString = SIMPLE_VM_STACK_READC ;
		if ( strlen(cString) != (unsigned int) SIMPLE_VM_STACK_STRINGSIZE ) {
			for ( x = 0 ; x < SIMPLE_VM_STACK_STRINGSIZE ; x++ ) {
				printf( "%c",cString[x] ) ;
			}
		} else {
			printf( "%s",cString ) ;
		}
	}
	else if ( SIMPLE_VM_STACK_ISPOINTER ) {
		if ( SIMPLE_VM_STACK_OBJTYPE == SIMPLE_OBJTYPE_VARIABLE ) {
			list = simple_list_getlist((List *) SIMPLE_VM_STACK_READP,SIMPLE_VAR_VALUE);
			if ( simple_vm_oop_isobject(list) ) {
				simple_vm_oop_printobj(vm,list);
			} else {
				simple_list_print(list);
			}
		}
		else if ( SIMPLE_VM_STACK_OBJTYPE ==SIMPLE_OBJTYPE_LISTITEM ) {
			pItem = (Item *) SIMPLE_VM_STACK_READP ;
			list = simple_item_getlist(pItem) ;
			if ( simple_vm_oop_isobject(list) ) {
				simple_vm_oop_printobj(vm,list);
			} else {
				simple_list_print(list);
			}
		}
	}
	else if ( SIMPLE_VM_STACK_ISNUMBER ) {
		simple_vm_numtostring(vm,SIMPLE_VM_STACK_READN,cStr);
		printf( "%s",cStr ) ;
	}
	SIMPLE_VM_STACK_POP ;
	simple_vm_freestack(vm);
	fflush(stdout);
}

void simple_vm_read ( VM *vm )
{
	int x  ;
	char cLine[256]  ;
	List *var  ;
	Item *pItem  ;
	/* Get Input From the User and save it in the variable */
	fgets(cLine , 256 , stdin );
	/* Remove New Line */
	for ( x = 0 ; x <= 255 ; x++ ) {
		if ( cLine[x] == '\n' ) {
			cLine[x] = '\0' ;
			break ;
		}
	}
	if ( SIMPLE_VM_STACK_ISPOINTER ) {
		if ( SIMPLE_VM_STACK_OBJTYPE == SIMPLE_OBJTYPE_VARIABLE ) {
			var = (List *) SIMPLE_VM_STACK_READP ;
			SIMPLE_VM_STACK_POP ;
			simple_list_setint_gc(vm->sState,var, SIMPLE_VAR_TYPE ,SIMPLE_VM_STRING);
			simple_list_setstsimple_gc(vm->sState,var, SIMPLE_VAR_VALUE, cLine);
		}
		else if ( SIMPLE_VM_STACK_OBJTYPE ==SIMPLE_OBJTYPE_LISTITEM ) {
			pItem = (Item *) SIMPLE_VM_STACK_READP ;
			SIMPLE_VM_STACK_POP ;
			simple_item_settype_gc(vm->sState,pItem,ITEMTYPE_STRING);
			simple_string_set_gc(vm->sState,simple_item_getstring(pItem),cLine);
		}
	}
}

/* User Interface Blocks (Another implementation) - Flexibile (We can replace blocks in Simple Code) */

void display_string ( void *pointer )
{
	char *cString  ;
	int x  ;
	char cStr[100]  ;
	List *list  ;
	VM *vm  ;
	vm = (VM *) pointer ;
	if ( SIMPLE_API_ISSTRING(1) ) {
		cString = SIMPLE_API_GETSTRING(1) ;
		if ( strlen(cString) != (unsigned int) SIMPLE_API_GETSTRINGSIZE(1) ) {
			for ( x = 0 ; x < SIMPLE_API_GETSTRINGSIZE(1) ; x++ ) {
				printf( "%c",cString[x] ) ;
			}
		} else {
			printf( "%s",cString ) ;
		}
	}
	else if ( SIMPLE_API_ISNUMBER(1) ) {
		simple_vm_numtostring(vm,SIMPLE_API_GETNUMBER(1),cStr);
		printf( "%s",cStr ) ;
	}
	else if ( SIMPLE_API_ISLIST(1) ) {
		list = SIMPLE_API_GETLIST(1);
		if ( simple_vm_oop_isobject(list) ) {
			simple_vm_oop_printobj(vm,list);
		} else {
			simple_list_print(list);
		}
	}
	fflush(stdout);
}

void read_string ( void *pointer )
{
	int x  ;
	char cLine[256]  ;
	/* Get Input From the User and save it in the variable */
	fgets(cLine , 256 , stdin );
	/* Remove New Line */
	for ( x = 0 ; x <= 255 ; x++ ) {
		if ( cLine[x] == '\n' ) {
			cLine[x] = '\0' ;
			break ;
		}
	}
	SIMPLE_API_RETSTRING(cLine);
}


