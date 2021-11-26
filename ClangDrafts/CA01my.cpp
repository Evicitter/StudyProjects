//Last update time : 28.01.2016 20:08:34

//#define __STDC_LIMIT_MACROS
//#define __STDC_CONSTANT_MACROS
//#define __STDC_FORMAT_MACROS

#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

//INCs -------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
//------------------------

//NAMESPACES ---------------------------
using namespace llvm;
using namespace clang;
//using namespace clang::driver;
//using namespace clang::ast_matchers;
//using namespace cl;
using namespace clang::tooling;
using namespace std;
//--------------------------------------


//CONST --------------------------------------
const string mapsource = "magicfibicus.c";
//VAR ----------------------------------------
string filesource;
string findfunction;
string reptofunction;
string OldCode;
string NewCode;
Rewriter rewriter;
set<std::string> listFunctions;
//--------------------------------------------

class MainVisitor : public RecursiveASTVisitor<MainVisitor>
{
public:
	ASTContext *Context;

	explicit MainVisitor(ASTContext *Context) :
		Context(Context)
	{
		//Чистим список функций на всякий
		listFunctions.clear();
		//-----------------------
	}

	bool VisitNamedDecl(NamedDecl *ND)
	{
		//Здесь выведем AST-дерево для заданной функции

		auto name = ND->getQualifiedNameAsString();
		if (name == findfunction)
		{
			cout << "\n\tAST-Tree for function : \"" << findfunction << "\"\n";
			ND->dump();
			cout << "---------------------------------------------------\n\n";
		}
		return true;
	}

	virtual bool VisitFunctionDecl(FunctionDecl *FD) //Ищем все объявления функций
	{
		SourceManager& srcMgr = Context->getSourceManager();
		SourceLocation sl = FD->getLocation();

		//Узнаём источник происхождения функции
		if (srcMgr.getFilename(sl).str() == mapsource)
		{
			string funcName = FD->getNameInfo().getName().getAsString();
			
			listFunctions.insert(funcName); //+ " in " + func->getLocStart().printToString(srcMgr));

			if (funcName == findfunction)
			{
				//unsigned int re = sl.getRawEncoding();
				rewriter.ReplaceText(sl, funcName.length(), reptofunction);
			}
		}
		//----------------------------------------------------------------------
		return true;
	}

	virtual bool VisitStmt(Stmt *st) //Ищем все вызовы функций
	{
		if (CallExpr* call = dyn_cast<CallExpr>(st))
		{
			FunctionDecl* f = call->getDirectCallee();
			string funcName = f->getNameInfo().getName().getAsString();

			SourceManager& srcMgr = Context->getSourceManager();
			SourceLocation sl = call->getLocStart();
			
			if (srcMgr.getFilename(sl).str() == mapsource)
			{
				listFunctions.insert(funcName);
				if (funcName == findfunction)
				{
					rewriter.ReplaceText(call->getLocStart(), funcName.length(), reptofunction);
				}
			}
		}
		return true;
	}
};


class MainASTConsumer : public clang::ASTConsumer
{
public:
	MainASTConsumer(ASTContext *Context, CompilerInstance *Compiler) :	MV(Context)
	{
	}

	virtual void HandleTranslationUnit(clang::ASTContext &Context)
	{
		//Запуск разбора
		MV.TraverseDecl(Context.getTranslationUnitDecl());
		//--------------------------------------------------
	}
private:
	MainVisitor MV;
	//MainVisitor rVis;
};

class MainAction : public clang::ASTFrontendAction
{
public:

	explicit MainAction(const string fsource, const string ffuncfind, const string ffuncrepl)
	{
		filesource = fsource;
		findfunction = ffuncfind;
		reptofunction = ffuncrepl;
		NewCode = OldCode = "";
	}

	virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile)
	{
		//cout << "CreateASTConsumer" << endl;

		ASTContext* ac = &Compiler.getASTContext();

		rewriter.setSourceMgr(ac->getSourceManager(), ac->getLangOpts());

		//Собираем старый исходный код ------------------------------------------------------------------------------
		RewriteBuffer& rb = rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID());
		for (RopePieceBTreeIterator I = rb.begin(), E = rb.end(); I != E; I.MoveToNextPiece()) OldCode += I.piece();
		//-----------------------------------------------------------------------------------------------------------

		return std::unique_ptr<clang::ASTConsumer>(new MainASTConsumer(ac, &Compiler));
	}

	virtual bool BeginSourceFileAction(clang::CompilerInstance &Compiler, StringRef Filename)
	{
		//cout << "Fileref:" << string(Filename.begin(), Filename.end());
		return true;
	}

	virtual void EndSourceFileAction()
	{
		//cout << "EndSourceFileAction" << endl;

		//Собираем новый код -----------------------------------------------------
		RewriteBuffer& rb = rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID());
		for (RopePieceBTreeIterator I = rb.begin(), E = rb.end(); I != E; I.MoveToNextPiece()) NewCode += I.piece();
		//-------------------------------------------------------------------------

		//Вывод старого и нового кода ----------
		cout << "\n\nOld Source Code:\n";
		cout << OldCode << "\n\n";
		cout << "\n\nNew Source Code:\n";
		cout << NewCode << "\n\n";
		//---------------------------------------
		
		//rewriter.overwriteChangedFiles();

		//Вывод функций
		int numberFunc = 0;
		for (auto i : listFunctions)
		{
			cout << "find function #" << numberFunc << " \"" << i << "\"\n";
			++numberFunc;
		}
		//--------------------------
	}
};

int main(int argc, const char **argv)
{
	string source = "A:\\fib.c"; //default {
	string destiny = "";
	string sfunc = "Fibonacci";
	string rfunc = "Fibo";
	//----------------------------------------


	if (argc >= 2) source = argv[1];  //Если указан параметр - путь к файлу исходнику
	if (argc >= 3) destiny = argv[2]; //Если указан параметр - путь к обработаному файлу
	if (argc >= 4) sfunc = argv[3];   //"Кого ищем на замену"
	if (argc >= 5) rfunc = argv[4];	  //"На что заменяет"
	//-----------------------------------------------------------------------------------

	//READ SOURCE --------------------------
	ifstream in(source);
	if (!in.is_open())
	{
		cout << "Input source file : ";
		cin >> source;
		in.open(source);
		if (!in.is_open())
		{
			cout << "\nFile not found!\n";
			system("PAUSE");
			return 0;
		}
	}
	stringstream buffer;
	buffer << in.rdbuf();
	in.close();
	//---------------------------------------

	
	//PRE and START tool
	vector<string> ArgsInvoc;
	//ArgsInvoc.push_back("-w");							//Запрет всех предупреждений
	ArgsInvoc.push_back("-Wno-deprecated-declarations");	//Запрет предупреждений об устаревших (deprecated) объявлениях.

	//NOTE:
	//mapsource хранит название виртуального файла в который собирается исходный код.
	clang::tooling::runToolOnCodeWithArgs(new MainAction(source, sfunc, rfunc), buffer.str(), ArgsInvoc, mapsource);
	cout.flush();
	//------------------------------------------------------


	//SAVE Result -------------------------------------
	bool bSaveFile = !destiny.empty(); //if not empty path

	//REQ USER PATH ------------------------------------
	if (!bSaveFile)
	{
		cout << "\nSave changes to file (y/n): ";
		string req;
		cin >> req;
		bSaveFile = (req == "y") || (req == "Y"); //return true if presskey 'Y' or 'y'
	}
	//-----------------------------------------------------------------------------------

	if (bSaveFile)
	{
		if (destiny.empty())
		{
			cout << "\nInput filename for save : ";
			cin >> destiny;
		}
		ofstream out(destiny);
		if (!out.is_open())					//fail
		{
			cout << "\nSaving error!\n";
			//system("PAUSE");
			return 0;
		}
		out << NewCode;
		out.close();
	}
	//--------------------------------------------------------------------------
	
	//system("PAUSE");
	return 0;
}