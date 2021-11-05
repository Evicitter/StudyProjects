object ProgressForm: TProgressForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Progress'
  ClientHeight = 77
  ClientWidth = 427
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.Enabled = True
  GlassFrame.SheetOfGlass = True
  OldCreateOrder = False
  Position = poDesktopCenter
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PBar: TProgressBar
    Left = 2
    Top = 24
    Width = 425
    Height = 17
    Max = 2147483647
    State = pbsError
    TabOrder = 0
  end
  object UpdateTimer: TTimer
    Enabled = False
    OnTimer = MainUpdateTime
    Left = 320
    Top = 32
  end
end
