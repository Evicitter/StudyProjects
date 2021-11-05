object FormTime: TFormTime
  Left = 0
  Top = 0
  Caption = 'FormTime'
  ClientHeight = 525
  ClientWidth = 328
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object VLEditor: TValueListEditor
    Left = 0
    Top = 0
    Width = 328
    Height = 496
    Align = alClient
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goAlwaysShowEditor, goThumbTracking]
    Strings.Strings = (
      '=')
    TabOrder = 0
    TitleCaptions.Strings = (
      'Parameters'
      'Value')
    ColWidths = (
      150
      172)
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 496
    Width = 328
    Height = 29
    Align = alBottom
    Caption = 'ToolBar1'
    DrawingStyle = dsGradient
    TabOrder = 1
  end
end
