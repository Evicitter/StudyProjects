object BrwGroup: TBrwGroup
  Left = 0
  Top = 0
  Caption = 'Group Browser'
  ClientHeight = 394
  ClientWidth = 402
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 402
    Height = 29
    Caption = 'ToolBar1'
    DrawingStyle = dsGradient
    TabOrder = 0
  end
  object ListBox1: TListBox
    Left = 0
    Top = 29
    Width = 402
    Height = 365
    Align = alClient
    ItemHeight = 13
    TabOrder = 1
    ExplicitTop = 21
  end
  object FlowPanel1: TFlowPanel
    Left = 104
    Top = 128
    Width = 225
    Height = 161
    Caption = 'FlowPanel1'
    TabOrder = 2
    OnMouseDown = FlowPanel1MouseDown
  end
  object IL_ToolBar1: TImageList
    Left = 352
    Top = 56
  end
end
