extern "C" {
	#include <trs/cg.h>
	#include <trs/error.h>
}

#include <memory>

#include <llvm/LinkAllIR.h>
#include <llvm/IR/IRBuilder.h>

namespace cg{
	bool compile(horn_Obj* obj);
};

extern "C" int trs_cgCompile(FILE* out, horn_Obj* obj){
	(void) out;
	(void) obj;
	assert(0 && "This function is not supported.");
	return 0;
}

extern "C" int trs_cgCompileCmd(FILE* out, horn_Obj* obj){
	std::unique_ptr<llvm::LLVMContext> context = std::make_unique<llvm::LLVMContext>();
	std::unique_ptr<llvm::Module> mod = std::make_unique<llvm::Module>("TRSGlobal", *context);
	std::unique_ptr<llvm::IRBuilder<>> builder = std::make_unique<llvm::IRBuilder<>>(*context);

	while(obj){
		if(cg::compile(obj)) return -1;
		obj = obj->next;
	}

	return 0;
}

namespace cg{
	bool compile(horn_Obj* obj){
		switch(obj->cmd){
			case HORN_CMD_VAR:
			{
				//
			} break;
			default:
				LOGE({}, EIDX_HORN_UNKNOWN_CMD, horn_CmdToString(obj->cmd));
				return true;
		}

		return false;
	}
}
