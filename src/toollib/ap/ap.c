#include <toollib/ap/ap.h>

#include <string.h>

const char* ap_chop(int* argc, const char*** argv){
	if(!(*argc))
		return 0;

	const char* arg = **argv;
	(*argv)++;
	(*argc)--;
	return arg;
}

ap_Arg ap_next(size_t descc, ap_Desc* descv, int* argc, const char*** argv){
	ap_Arg arg = {0};
	
	const char* a = ap_chop(argc, argv);
	if(!a)
		return arg;
	
	arg.m_Id = -1;

	ap_ArgType type;
	size_t len = strlen(a);
	if(a[0] == '-'){
		arg.m_Status = AP_ARG_STATUS_ERR_SHORT;

		if(len < 2){
			arg.m_Value = a;
			return arg;
		}

		if(a[1] == '-'){
			arg.m_Status = AP_ARG_STATUS_ERR_LONG;

			if(len < 3){
				// It is actually allowed (--)
				arg.m_Status = AP_ARG_STATUS_OK;
				arg.m_Value = a;
				arg.m_ValueLen = 2;
				return arg;
			}

			a += 2;
			for(size_t i = 0; i < descc; i++){
				if(!descv[i].m_Long) 
					continue;
				
				size_t dlen = strlen(descv[i].m_Long);
				if(strncmp(descv[i].m_Long, a, dlen) == 0 && (a[dlen] == '=' || a[dlen] == ':' || a[dlen] == 0)){
					type = descv[i].m_Type;
					arg.m_Id = i;
					a += dlen;
					len -= dlen + 2;
					break;
				}
			}

			if(arg.m_Id < 0){
				arg.m_Value = a - 2;
				return arg;
			}
		} else{
			char c = a[1];
			for(size_t i = 0; i < descc; i++){
				if(descv[i].m_Short == c){
					type = descv[i].m_Type;
					arg.m_Id = i;
					break;
				}
			}

			if(arg.m_Id < 0){
				arg.m_Value = a;
				return arg;
			}

			a += 2;
			len -= 2;
		}
	} else {
		arg.m_Value = a;
		arg.m_ValueLen = len;
		arg.m_Status = AP_ARG_STATUS_OK;
		return arg;
	}

	if(type == AP_ARG_TYPE_FLAG){
		if(len != 0)
			arg.m_Status = AP_ARG_STATUS_ERR_VALUE;
		else
			arg.m_Status = AP_ARG_STATUS_OK;
		
		return arg;
	}

	switch(type){
		case AP_ARG_TYPE_FLAG: {
			if(len != 0)
				arg.m_Status = AP_ARG_STATUS_ERR_VALUE;
			else
				arg.m_Status = AP_ARG_STATUS_OK;
			return arg;
		} break;
		case AP_ARG_TYPE_VALUE: {
			if(len == 0){
				arg.m_Status = AP_ARG_STATUS_ERR_VALUE;
				break;
			}

			arg.m_Status = AP_ARG_STATUS_OK;
			if(a[0] == '=' || a[0] == ':')
				a++;

			arg.m_Value = a;
			arg.m_ValueLen = strlen(a);
		} break;
		case AP_ARG_TYPE_VALUE_OPTIONAL: {
			arg.m_Status = AP_ARG_STATUS_OK;
			if(len != 0){
				if(a[0] == '=' || a[0] == ':')
					a++;

				arg.m_Value = a;
				arg.m_ValueLen = strlen(a);
			}
		} break;
		case AP_ARG_TYPE_VALUE2: {
			if(len == 0){
				arg.m_Status = AP_ARG_STATUS_ERR_VALUE;
				break;
			}

			if(a[0] == '=' || a[0] == ':')
				a++;
			arg.m_Value = a;

			while(*a && a[0] != '=' && a[0] != ':')
				a++;

			if(*a){
				arg.m_ValueLen = a - arg.m_Value;
				a++;
				arg.m_Value2 = a;
				arg.m_Value2Len = strlen(a);
				arg.m_Status = AP_ARG_STATUS_OK;
			} else{
				arg.m_ValueLen = strlen(arg.m_Value);
				arg.m_Status = AP_ARG_STATUS_ERR_VALUE2;
				break;
			}
		} break;
		case AP_ARG_TYPE_VALUE2_OPTIONAL: {
			if(len == 0){
				arg.m_Status = AP_ARG_STATUS_ERR_VALUE;
				break;
			}

			if(a[0] == '=' || a[0] == ':')
				a++;

			arg.m_Value = a;
			while(*a && a[0] != '=' && a[0] != ':')
				a++;

			arg.m_ValueLen = a - arg.m_Value;

			arg.m_Status = AP_ARG_STATUS_OK;
			if(*a){
				a++;
				arg.m_Value2 = a;
				arg.m_Value2Len = strlen(a);
			}
		} break;
		default: break;
	}

	if(arg.m_Status != AP_ARG_STATUS_OK){
		// Resolve errors
		
		a = ap_chop(argc, argv);
		if(!a)
			return arg;
	
		len = strlen(a);

		if(arg.m_Status == AP_ARG_STATUS_ERR_VALUE){
			arg.m_Value = a;
			arg.m_ValueLen = len;
			arg.m_Status = AP_ARG_STATUS_OK;

			if(type == AP_ARG_TYPE_VALUE2 || type == AP_ARG_TYPE_VALUE2_OPTIONAL){
				while(*a && *a != '=' && *a != ':'){
					a++;
					len--;
				}

				if(*a){
					a++;

					arg.m_Value2 = a;
					arg.m_Value2Len = len - 1;
					arg.m_ValueLen -= len;
				} else if(type != AP_ARG_TYPE_VALUE2_OPTIONAL){
					a = ap_chop(argc, argv);
					if(!a){
						arg.m_Status = AP_ARG_STATUS_ERR_VALUE2;
						return arg;
					}

					arg.m_Value2 = a;
					arg.m_Value2Len = strlen(a);
				}
			}
		} else if(arg.m_Status == AP_ARG_STATUS_ERR_VALUE2){
			arg.m_Value2 = a;
			arg.m_Value2Len = len;
			arg.m_Status = AP_ARG_STATUS_OK;
		}
	}
	
	return arg;
}

