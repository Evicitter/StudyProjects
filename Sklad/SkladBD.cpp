//Last update time : 06.09.2009 21:04:42
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("DM.cpp", DMMain); /* TDataModule: File Type */
USEFORM("List.cpp", fmList);
USEFORM("Nakl.cpp", fmNakl);
USEFORM("Report.cpp", fmReport);
USEFORM("SQL.cpp", fmSQL);
USEFORM("Tovari.cpp", fmTovari);
USEFORM("Unit1.cpp", fmMain);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TfmMain), &fmMain);
         Application->CreateForm(__classid(TDMMain), &DMMain);
         Application->CreateForm(__classid(TfmList), &fmList);
         Application->CreateForm(__classid(TfmNakl), &fmNakl);
         Application->CreateForm(__classid(TfmReport), &fmReport);
         Application->CreateForm(__classid(TfmSQL), &fmSQL);
         Application->CreateForm(__classid(TfmTovari), &fmTovari);
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