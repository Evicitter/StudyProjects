//Last update time : 07.05.2010 02:54:00

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("TimeForm.cpp", FormTime);
USEFORM("StepCon_PF.cpp", ProgressForm);
USEFORM("LogForm.cpp", FormLog);
USEFORM("FRMsub_actObjectWin.cpp", actObjectWin); /* TFrame: File Type */
USEFORM("FRM_FieldSpaces.cpp", frmFieldSpaces); /* TFrame: File Type */
USEFORM("GB_WaveBrowser.cpp", BrwWave);
USEFORM("GB_GroupBrowser.cpp", BrwGroup);
USEFORM("GB_MaterialBrowser.cpp", BrwMaterial);
USEFORM("StepCon_MF.cpp", MainForm);
USEFORM("GB_ActionBrowser.cpp", BrwAction);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "StepConHost";
		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TFormLog), &FormLog);
		Application->CreateForm(__classid(TFormTime), &FormTime);
		Application->CreateForm(__classid(TBrwAction), &BrwAction);
		Application->CreateForm(__classid(TBrwGroup), &BrwGroup);
		Application->CreateForm(__classid(TBrwWave), &BrwWave);
		Application->CreateForm(__classid(TBrwMaterial), &BrwMaterial);
		Application->CreateForm(__classid(TProgressForm), &ProgressForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
