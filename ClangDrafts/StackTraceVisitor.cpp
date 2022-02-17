//Date: 06/10/2016 19:22:24
#include "stdafx.h"

#include "StackTraceVisitor.h"
#include "CFile_ini.h"

//{
#include "A:/bingos.h"

#ifdef appCheckPro
#undef appCheckPro
#undef appCheckClose
#undef appWriteLine
#define appCheckPro(x)
#define appCheckClose(x)
#define appWriteLine(x)
#endif
//}

void STI_ASTVisitor::showdump(Decl* d, Stmt* s, const char* Description, const char* calefuncname, const char* fname, const int linenum)
{
	//if (!TransVars.bValidUnitFile) return;

	//pFuncStack _pfs_loc_stack(calefuncname, fname, linenum, 0);
	//_pfs_loc_stack.writeline(Description);

	if (cgv.bShowDump)
	{
		if (d != nullptr)
		{
#if 1
			llvmout("> " << calefuncname << " : " << d->Decl::getDeclKindName() << '\n' << Description);
#endif
			d->dumpColor();
			//system("PAUSE");
		}
		if (s != nullptr)
		{
#if 1
			llvmout("> " << calefuncname << " : " << s->getStmtClassName() << '\n' << Description << '\n');
#endif
			s->dumpColor();
			//system("PAUSE");
		}
	}
}

void STI_ASTVisitor::PrintParams(const char* specname)
{
	llvmdebug("[ (%s)\tbOpenChangeCompound,IamInMainFunction=(%d,%d)]\n", specname, bOpenCompoundToChange, IamInMainFunction);
	system("PAUSE");
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
STI_ASTVisitor::STI_ASTVisitor(Rewriter& R, CommonGlobalVars& _cgv) : TheRewriter(R), cgv(_cgv)
{
	appCheckPro(0);
	bOpenCompoundToChange = false;
	IamInTemplateClass = false;
	IamInTemplateFunction = false;
	IamInMainFunction = false;

	CIniReader inir;

	ExcludeFileOfTool = inir.ReadString("Main", "IncludeNameSupportFile", "bingos.h");
	EnterFunctionName = inir.ReadString("f0","EnterFunctionName","appCheckPro");
	LeaveFunctionName = inir.ReadString("f0", "LeaveFunctionName", "appCheckClose");
	MainEnterFunctionName = inir.ReadString("f0", "MainEnterFunctionName", "appCheckPro");
	MainLeaveFunctionName = inir.ReadString("f0", "MainLeaveFunctionName", "appCheckClose");
	PreReturnFunctionName = inir.ReadString("f0", "PreReturnFunctionName", "appCheckClose");
}

STI_ASTVisitor::~STI_ASTVisitor()
{
	appCheckPro(0);
	appCheckClose(0);
}

bool STI_ASTVisitor::VisitStmt(Stmt* s)
{
	//if (!TransVars.bValidUnitFile) return false;

	/*std::string desc = s->getStmtClassName();
	desc.append("\n");
	desc.append(TheRewriter.getRewrittenText(s->getSourceRange()).data());
	desc.append("\n");
	showdump(nullptr, s, desc.c_str(), __FUNCTION__, __FILE__, __LINE__);*/

	//showdump(nullptr, s, __FUNCTION__);

	// Only care about If statements.
	/*if (isa<IfStmt>(s)) {
	IfStmt *IfStatement = cast<IfStmt>(s);
	Stmt *Then = IfStatement->getThen();

	TheRewriter.InsertText(Then->getLocStart(), "// the 'if' part\n", true,	true);

	Stmt *Else = IfStatement->getElse();
	if (Else)
	TheRewriter.InsertText(Else->getLocStart(), "// the 'else' part\n",
	true, true);
	}*/
	/*for (auto xxx = s->child_begin(); xxx != s->child_end(); ++xxx)
	{
	printf_s("Stmt=%s\n", (*xxx)->getStmtClassName());
	}*/

	//if (isa<StringLiteral>(s))
	//{
	//StringLiteral* xx = cast<StringLiteral>(s);
	//printf_s("this lit %d %s\n", xx->getByteLength(), xx->getBytes().data());
	//}

	return true;
}

bool STI_ASTVisitor::VisitCompoundStmt(CompoundStmt* cs)
{
	if (bOpenCompoundToChange)
	{
		bOpenCompoundToChange = false;

		unsigned int x = cs->getLBracLoc().getRawEncoding();

		if (ChangePoses.find(x) != ChangePoses.end()) return true;
		
		ChangePoses.insert(x);

		std::string insertText;
		
		//Если функция - main
		if (IamInMainFunction)
		{
			//Вставляем функцию в начало
			insertText.append("\n");
			insertText.append(MainEnterFunctionName);
			//TODO: use rand integer argument
			insertText.append("(0);\n");
			TheRewriter.InsertText(cs->getLBracLoc().getLocWithOffset(1), insertText, true, true);

			//Вставляем функцию завершения в конец
			insertText.clear();
			insertText.append("\n");
			insertText.append(MainLeaveFunctionName);
			//TODO: use rand integer argument
			insertText.append("(0);\n");
			TheRewriter.InsertText(cs->getRBracLoc(), insertText, true, true);
		} 
		else //Если любая другая (не main) функция
		{
			//Вставляем функцию в начало
			insertText.append("\n");
			insertText.append(EnterFunctionName);
			//TODO: use rand integer argument
			insertText.append("(0);\n");
			TheRewriter.InsertText(cs->getLBracLoc().getLocWithOffset(1), insertText, true, true);
		}		

		++cgv.nChanges;
	}
	return true;
}

bool STI_ASTVisitor::VisitReturnStmt(ReturnStmt* ds)
{
	/*std::string desc = ds->getStmtClassName();
	desc.append("\n");
	desc.append(TheRewriter.getRewrittenText(ds->getSourceRange()).data());
	desc.append("\n");
	showdump(nullptr, ds, desc.c_str(), __FUNCTION__, __FILE__, __LINE__);
	*/

	if (IamInMainFunction)
	{
		if (ds->getSourceRange().isValid())
		{
			std::string retstr = TheRewriter.getRewrittenText(ds->getSourceRange());
			std::string cins;
			cins.append("{");
			cins.append(PreReturnFunctionName);
			cins.append("(0);");	//TODO: use rand integer argument for function
			cins.append(retstr);
			cins.append(";}");
			TheRewriter.RemoveText(ds->getSourceRange().getEnd(), 1);
			TheRewriter.ReplaceText(ds->getSourceRange(), cins);
			++cgv.nChanges;
		}
	}

	/*
	std::string retstr = TheRewriter.getRewrittenText(ds->getSourceRange());
	std::string cins("{appReturn(__FUNCTION__);");
	cins.append(retstr);
	cins.append(";}");

	TheRewriter.RemoveText(ds->getSourceRange().getEnd(), 1);
	TheRewriter.ReplaceText(ds->getSourceRange(), cins);
	*/
	return true;
}

bool STI_ASTVisitor::VisitDecl(Decl* d)
{
	if (!TransVars.bValidUnitFile) return false;

	/*std::string desc = d->clang::Decl::getDeclKindName();
	desc.append("\n");
	desc.append(TheRewriter.getRewrittenText(d->getSourceRange()).data());
	desc.append("\n");
	showdump(d, nullptr, desc.c_str(), __FUNCTION__, __FILE__, __LINE__);*/

	return true;
}

//In function
bool STI_ASTVisitor::VisitFunctionDecl(FunctionDecl* f)
{
	/*std::string desc = f->clang::Decl::getDeclKindName();
	desc.append("\n");
	desc.append(TheRewriter.getRewrittenText(f->getSourceRange()).data());
	desc.append("\n");
	showdump(f, nullptr, desc.c_str(), __FUNCTION__, __FILE__, __LINE__);
	*/

	// Only function definitions (with bodies), not declarations.
	if (f->hasBody())
	{
		IamInMainFunction = f->isMain();
		bOpenCompoundToChange = true;
		//bOpenCompoundToChange = !f->isFunctionTemplateSpecialization();

		unsigned int x = f->getSourceRange().getBegin().getRawEncoding();

		if (PassFunctions.find(x) != PassFunctions.end())
			return true;

		PassFunctions.insert(x);
			
		//nameFunctions.Insert(f->getNameInfo().getName().getAsString().c_str());

		//for (auto xxx = f->getBody()->child_begin(); xxx != f->getBody()->child_end(); ++xxx)
		//printf_s("xx=%s %s\n", (*xxx)->getStmtClassName(), f->getNameInfo().getName().getAsString().c_str());

		//Stmt* FuncBody = f->getBody();

		// Type name as string
		//QualType QT = f->getReturnType();
		//std::string TypeStr = QT.getAsString();

		// Function name
		//DeclarationName DeclName = f->getNameInfo().getName();
		//std::string FuncName = DeclName.getAsString();

		// Add comment before
		//std::stringstream SSBefore;
		//SSBefore << "// Begin function " << FuncName << " returning " << TypeStr << "\n";

		// And after
		//std::stringstream SSAfter;
		//SSAfter << "\n// End function " << FuncName;
		//ST = FuncBody->getLocEnd().getLocWithOffset(1);
		//TheRewriter.InsertText(ST, SSAfter.str(), true, true);
	}

	return true;
}