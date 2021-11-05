object BrwMaterial: TBrwMaterial
  Left = 0
  Top = 0
  Caption = 'Material Browser'
  ClientHeight = 443
  ClientWidth = 599
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 599
    Height = 29
    Caption = 'ToolBar1'
    DrawingStyle = dsGradient
    Images = IL_ToolBar1
    TabOrder = 0
    ExplicitLeft = 128
    ExplicitTop = 48
    ExplicitWidth = 150
  end
  object Panel1: TPanel
    Left = 0
    Top = 29
    Width = 599
    Height = 414
    Align = alClient
    BorderStyle = bsSingle
    Caption = 'Panel1'
    Color = clBlack
    ParentBackground = False
    ShowCaption = False
    TabOrder = 1
    ExplicitLeft = 216
    ExplicitTop = 96
    ExplicitWidth = 185
    ExplicitHeight = 41
    object StatusBar1: TStatusBar
      Left = 1
      Top = 390
      Width = 593
      Height = 19
      Color = clBtnShadow
      Panels = <>
      SimplePanel = True
      SimpleText = 'Ready!!!'
    end
  end
  object IL_ToolBar1: TImageList
    Left = 544
    Top = 40
  end
end
