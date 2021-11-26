//Last update time : 30.01.2016 00:14:31

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
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

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
using namespace clang::ast_matchers;
//using namespace cl;
using namespace clang::tooling;
using namespace std;
//--------------------------------------


//CONST --------------------------------------
const static string mapsource = "magicfibicus.c";
//VAR ----------------------------------------
static string filesource;
static string findfunction;
static string reptofunction;
static string OldCode;
static string NewCode;
static Rewriter rewriter;
static set<std::string> listFunctions;
//--------------------------------------------

string getRandString()
{
	const size_t l = 8;
	string retres;
	for (size_t i = 0; i < l; ++i)
	{
		register size_t rsym;
		rsym = (rand() & 1) ? 'a' + (rand() * 26 / RAND_MAX) : 'A' + (rand() * 26 / RAND_MAX);
		retres += (char)rsym;
	}
	return retres;
}

void __fastcall initrand(int aargc, const char** aargv)
{
	//char* ps = aargv[0];
	int seed = time(NULL) + aargc;
	//while (*ps != '\0') seed += *ps;
	srand(seed);
}


class DeclHandler : public MatchFinder::MatchCallback
{
	Rewriter		&m_Rewriter;
	map<string, string>	&m_Vars;

public:
	DeclHandler(Rewriter &rewrite, map<string, string> &variables) : m_Rewriter(rewrite), m_Vars(variables) {}
	virtual void run(const MatchFinder::MatchResult &result)
	{
		string varOrig;
		string varNew;
		string filename;
		const VarDecl *pVarDecl = NULL;
		SourceManager &srcMgr = m_Rewriter.getSourceMgr();
		pVarDecl = result.Nodes.getNodeAs<clang::VarDecl>("decl");
		if (pVarDecl)
		{
			//getFileNameFromPath(srcMgr.getFilename(pVarDecl->getLocation()).str(), filename);
			//outs() << " PrintStats for file: " <<filename << "\n";
			//srcMgr.PrintStats();

			if (filename == mapsource)
			{
				//numDeclarations++;
				varOrig = pVarDecl->getDeclName().getAsString();
				varNew = varOrig;
				varNew += "_";
				varNew += getRandString();
				m_Rewriter.ReplaceText(pVarDecl->getLocation(), varOrig.length(), varNew);
				m_Vars[varOrig] = varNew;
				// выведем название всех объявленных переменных и их позицию;
				outs() << "[MATCHER] variable decl is: '" << varOrig;
				outs() << "' \ttype is: '" << pVarDecl->getType().getAsString();
				outs() << "' \tposition is: '" << pVarDecl->getLocation().printToString(srcMgr);
				outs() << "' \treplaced by: '" << varNew << "'\n";
			}
		}
	}
};

class DeclRefHandler : public MatchFinder::MatchCallback
{
	Rewriter		&m_Rewriter;
	map<string, string>	&m_Vars;
	typedef map<string, string>::const_iterator iterVars;
	iterVars		m_IterVars = m_Vars.begin();
public:
	DeclRefHandler(Rewriter &rewrite, map<string, string> &variables) : m_Rewriter(rewrite), m_Vars(variables) {}
	virtual void run(const MatchFinder::MatchResult &result)
	{
		string varOrig;
		string varNew;
		string filename;
		const DeclRefExpr *pDeclRefExpr = NULL;
		SourceManager &srcMgr = m_Rewriter.getSourceMgr();
		pDeclRefExpr = result.Nodes.getNodeAs<clang::DeclRefExpr>("declRef");
		if (pDeclRefExpr)
		{
			//getFileNameFromPath(srcMgr.getFilename(pDeclRefExpr->getLocation()).str(), filename);

			if (filename == mapsource)
			{
				varOrig = pDeclRefExpr->getNameInfo().getAsString();
				m_IterVars = m_Vars.begin();

				for (; m_IterVars != m_Vars.end(); m_IterVars++)
				{
					if (m_IterVars->first == varOrig)
						m_Rewriter.ReplaceText(pDeclRefExpr->getLocation(), varOrig.length(), m_IterVars->second);
				}
			}
		}
	}
};



class RandVarASTConsumer : public clang::ASTConsumer
{
public:
	MatchFinder			m_Matcher;
	DeclHandler			m_DeclHandler;
	DeclRefHandler		m_DeclRefHandler;
	map<string, string> mVars;

	RandVarASTConsumer(ASTContext *Context, CompilerInstance *Compiler) : m_Matcher(), m_DeclHandler(rewriter, mVars), m_DeclRefHandler(rewriter, mVars)
	{
		m_Matcher.addMatcher(decl().bind("decl"), &m_DeclHandler);
		m_Matcher.addMatcher(declRefExpr().bind("declRef"), &m_DeclRefHandler);
	}

	virtual void HandleTranslationUnit(clang::ASTContext &Context)
	{
		//Запуск разбора
		m_Matcher.matchAST(Context);
		//--------------------------------------------------
	}
};

class MainAction : public clang::ASTFrontendAction {
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

		//rewriter.setSourceMgr(ac->getSourceManager(), ac->getLangOpts());

		//Собираем старый исходный код ------------------------------------------------------------------------------
		//RewriteBuffer& rb = rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID());
		//for (RopePieceBTreeIterator I = rb.begin(), E = rb.end(); I != E; I.MoveToNextPiece()) OldCode += I.piece();
		//-----------------------------------------------------------------------------------------------------------

		return std::unique_ptr<clang::ASTConsumer>(new RandVarASTConsumer(ac, &Compiler));
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
		//RewriteBuffer& rb = rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID());
		//for (RopePieceBTreeIterator I = rb.begin(), E = rb.end(); I != E; I.MoveToNextPiece()) NewCode += I.piece();
		//-------------------------------------------------------------------------

		//Вывод старого и нового кода ----------
		//cout << "\n\nOld Source Code:\n";
		//cout << OldCode << "\n\n";
		//cout << "\n\nNew Source Code:\n";
		//cout << NewCode << "\n\n";
		//---------------------------------------

		//rewriter.overwriteChangedFiles();

		//Вывод функций
		//int numberFunc = 0;
		//for (auto i : listFunctions)
		//{
		//	cout << "find function #" << numberFunc << " \"" << i << "\"\n";
		//	++numberFunc;
		//}
		//--------------------------
		rewriter.getSourceMgr().PrintStats();
	}
};

int main(int argc, const char **argv)
{
	//initrand(argc, argv);

	string source = "A:\\ReadFile.cc"; //default {
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
	ArgsInvoc.push_back("-ferror-limit=1");

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
		//string req;
		//cin >> req;
		//bSaveFile = (req == "y") || (req == "Y"); //return true if presskey 'Y' or 'y'
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