//Last update time : 03.10.2015 18:50:12

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("MainForm.cpp", MForm);
USEFORM("GraphicForm.cpp", GraphicsForm);
USEFORM("ConnectionOpt.cpp", ConnectOptions);
USEFORM("WaitForm.cpp", WaitProgress);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "Calibration (Тарировка) (с)";
		Application->CreateForm(__classid(TMForm), &MForm);
		Application->CreateForm(__classid(TConnectOptions), &ConnectOptions);
		Application->CreateForm(__classid(TGraphicsForm), &GraphicsForm);
		Application->CreateForm(__classid(TWaitProgress), &WaitProgress);
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