#include <trsap/trsap.h>

#include <string.h>

const char* trsap_chop(int* argc, const char*** argv){
	if(!(*argc))
		return 0;

	const char* arg = **argv;
	(*argv)++;
	(*argc)--;
	return arg;
}

trsap_Arg trsap_next(size_t descc, trsap_Desc* descv, int* argc, const char*** argv){
	trsap_Arg arg = {0};
	
	char* a = (char*) trsap_chop(argc, argv);
	if(!a)
		return arg;
	
	arg.m_Id = -1;

	trsap_ArgType type;
	size_t len = strlen(a);
	if(a[0] == '-'){
		arg.m_Status = TRSAP_ARG_STATUS_ERR_SHORT;

		if(len < 2){
			arg.m_Value = a;
			return arg;
		}

		if(a[1] == '-'){
			arg.m_Status = TRSAP_ARG_STATUS_ERR_LONG;

			if(len < 3){
				// It is actually allowed (--)
				arg.m_Status = TRSAP_ARG_STATUS_OK;
				arg.m_Value = a;
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
		arg.m_Status = TRSAP_ARG_STATUS_OK;
		return arg;
	}

	if(type == TRSAP_ARG_TYPE_FLAG){
		if(len != 0)
			arg.m_Status = TRSAP_ARG_STATUS_ERR_VALUE;
		else
			arg.m_Status = TRSAP_ARG_STATUS_OK;
		
		return arg;
	}

	switch(type){
		case TRSAP_ARG_TYPE_FLAG: {
			if(len != 0)
				arg.m_Status = TRSAP_ARG_STATUS_ERR_VALUE;
			else
				arg.m_Status = TRSAP_ARG_STATUS_OK;
			return arg;
		} break;
		case TRSAP_ARG_TYPE_VALUE: {
			if(len == 0){
				arg.m_Status = TRSAP_ARG_STATUS_ERR_VALUE;
				break;
			}

			arg.m_Status = TRSAP_ARG_STATUS_OK;
			if(a[0] == '=' || a[0] == ':')
				a++;

			arg.m_Value = a;
		} break;
		case TRSAP_ARG_TYPE_VALUE_OPTIONAL: {
			arg.m_Status = TRSAP_ARG_STATUS_OK;
			if(len != 0){
				if(a[0] == '=' || a[0] == ':')
					a++;

				arg.m_Value = a;
			}
		} break;
		case TRSAP_ARG_TYPE_VALUE2: {
			if(len == 0){
				arg.m_Status = TRSAP_ARG_STATUS_ERR_VALUE;
				break;
			}

			if(a[0] == '=' || a[0] == ':')
				a++;
			arg.m_Value = a;

			while(*a && a[0] != '=' && a[0] != ':')
				a++;

			if(*a){
				a[0] = 0;
				a++;
				arg.m_Value2 = a;
				arg.m_Status = TRSAP_ARG_STATUS_OK;
			} else{
				arg.m_Status = TRSAP_ARG_STATUS_ERR_VALUE2;
				break;
			}
		} break;
		case TRSAP_ARG_TYPE_VALUE2_OPTIONAL: {
			if(len == 0){
				arg.m_Status = TRSAP_ARG_STATUS_ERR_VALUE;
				break;
			}

			if(a[0] == '=' || a[0] == ':')
				a++;

			arg.m_Value = a;
			while(*a && a[0] != '=' && a[0] != ':')
				a++;

			arg.m_Status = TRSAP_ARG_STATUS_OK;
			if(*a){
				a[0] = 0;
				a++;
				arg.m_Value2 = a;
			}
		} break;
		default: break;
	}

	// TODO: Get values from other arguments if needed

	return arg;
}

