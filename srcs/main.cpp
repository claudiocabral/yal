#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <string_view>
#include <lexer.h>

void func()
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
}

bool process_file(std::string_view filename, llvm::LLVMContext & context, llvm::Module & module, llvm::IRBuilder<> & builder)
{
    auto funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    auto mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);
    auto entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);

    llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world!\n");

    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

    llvm::FunctionType *putsType = 
        llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
    decltype(auto) putsFunc = module.getOrInsertFunction("puts", putsType);

    builder.CreateCall(putsFunc, helloWorld);
    builder.CreateRetVoid();

    module.print(llvm::errs(), nullptr);
    return true;
}

int scripting_mode() {
    printf("One day we will be able invoke this as a script\n");
    return 0;
}

int app(int argc, char **argv)
{
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    if (!Target) {
        llvm::errs() << Error;
        return 1;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Reloc::Model::PIC_;
    auto TheTargetMachine =
        Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);


    auto Filename = "output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return 1;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;

    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    if (argc == 1)
        return scripting_mode();
    while(--argc)
    {
        ++argv;
        llvm::LLVMContext context;
        llvm::Module module(*argv, context);
        llvm::IRBuilder<> builder(context); 
        module.setDataLayout(TheTargetMachine->createDataLayout());
        module.setTargetTriple(TargetTriple);

        if (!process_file(*argv, context, module, builder))
            return 1;
        pass.run(module);
        llvm::outs() << "Wrote " << Filename << "\n";
        dest.flush();
    }
    return 0;
}

int main(int argc, char **argv)
{
    return app(argc, argv);
}
