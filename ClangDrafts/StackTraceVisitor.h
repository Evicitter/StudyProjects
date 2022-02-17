//Date: 11/07/2016 20:16:00
#include "stdafx.h"

#ifndef _H_STACKTRACEVISITOR_H_
#define _H_STACKTRACEVISITOR_H_

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "GlobalVariables.h"

#include <unordered_set>

using namespace llvm;
using namespace clang;

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class STI_ASTVisitor : public clang::RecursiveASTVisitor<STI_ASTVisitor>
{
public:

	STI_ASTVisitor(Rewriter& R, CommonGlobalVars& cgv);
	~STI_ASTVisitor();

	//New methods ----------------------------------------------
	void PrintParams(const char* specname = "");
	void showdump(Decl* d, Stmt* s, const char* Description = nullptr, const char* calefuncname = "", const char* fname = "", const int linenum = 0); //option -showdump
	//----------------------------------------------------------

	//Redefine -------------------------------------------------
	bool VisitStmt(Stmt* s);
	bool VisitCompoundStmt(CompoundStmt* cs);
	bool VisitReturnStmt(ReturnStmt* ds);
	bool VisitDecl(Decl* d);
	bool VisitFunctionDecl(FunctionDecl* f);

public:
	std::string UniTrans;	//Имя единицы трансляции - например: main.cpp

private:
	Rewriter& TheRewriter;
	CommonGlobalVars& cgv;

	bool bOpenCompoundToChange;
	bool IamInTemplateClass;
	bool IamInTemplateFunction;
	bool IamInMainFunction;		//true - если текущая функция main

	std::unordered_set<unsigned int> ChangePoses;
	std::unordered_set<unsigned int> PassFunctions;

	std::string ExcludeFileOfTool;
	std::string EnterFunctionName;
	std::string LeaveFunctionName;
	std::string MainEnterFunctionName;
	std::string MainLeaveFunctionName;
	std::string PreReturnFunctionName;
};


#endif