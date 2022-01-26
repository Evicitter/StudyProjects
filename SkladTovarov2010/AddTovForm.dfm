object FAddTov: TFAddTov
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = #1044#1086#1073#1072#1074#1083#1077#1085#1080#1077' '#1090#1086#1074#1072#1088#1072
  ClientHeight = 143
  ClientWidth = 404
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Класс: TLabel
    Left = 8
    Top = 8
    Width = 29
    Height = 13
    Caption = #1050#1083#1072#1089#1089
  end
  object vleT: TValueListEditor
    Left = 8
    Top = 32
    Width = 305
    Height = 105
    Strings.Strings = (
      'Name=PP'
      'Coast=1'
      'Count=1'
      'BadDays=1')
    TabOrder = 0
    ColWidths = (
      150
      149)
  end
  object cbClass: TComboBox
    Left = 43
    Top = 5
    Width = 105
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 1
    Text = 'Fruit'
    Items.Strings = (
      'Fruit'
      'Meat'
      'Concerv')
  end
  object Button1: TButton
    Left = 319
    Top = 79
    Width = 75
    Height = 25
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 319
    Top = 110
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
