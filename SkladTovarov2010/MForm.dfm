object GMForm: TGMForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'SkladTovarov'
  ClientHeight = 488
  ClientWidth = 716
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 400
    Top = 439
    Width = 73
    Height = 13
    Caption = #1058#1077#1082#1091#1097#1080#1081' '#1076#1077#1085#1100
  end
  object BtnLoad: TButton
    Left = 8
    Top = 199
    Width = 75
    Height = 25
    Caption = 'Load Sklad'
    TabOrder = 0
    OnClick = BtnLoadClick
  end
  object DBGrid1: TDBGrid
    Left = 8
    Top = 8
    Width = 690
    Height = 185
    DataSource = DSTovar
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
  end
  object BtnSave: TButton
    Left = 89
    Top = 199
    Width = 75
    Height = 25
    Caption = 'Save Sklad'
    TabOrder = 2
    OnClick = BtnSaveClick
  end
  object BtnAdd: TButton
    Left = 462
    Top = 199
    Width = 75
    Height = 25
    Caption = 'Add tovar'
    TabOrder = 3
    OnClick = BtnAddClick
  end
  object BtnDel: TButton
    Left = 543
    Top = 199
    Width = 75
    Height = 25
    Caption = 'Del tovar'
    TabOrder = 4
    OnClick = BtnDelClick
  end
  object lbTovars: TListBox
    Left = 8
    Top = 230
    Width = 156
    Height = 203
    ItemHeight = 13
    TabOrder = 5
    OnClick = lbTovarsClick
  end
  object vleTovar: TValueListEditor
    Left = 170
    Top = 230
    Width = 528
    Height = 203
    TabOrder = 6
    TitleCaptions.Strings = (
      'ParamName'
      'ParamValue')
    ColWidths = (
      150
      372)
  end
  object BtnUpdate: TButton
    Left = 623
    Top = 199
    Width = 75
    Height = 25
    Caption = 'Update tovar'
    TabOrder = 7
    OnClick = BtnUpdateClick
  end
  object Edit1: TEdit
    Left = 497
    Top = 439
    Width = 121
    Height = 21
    NumbersOnly = True
    TabOrder = 8
    Text = '1'
  end
  object Button1: TButton
    Left = 624
    Top = 439
    Width = 75
    Height = 25
    Caption = #1055#1088#1086#1074#1077#1088#1082#1072
    TabOrder = 9
    OnClick = Button1Click
  end
  object DSTovar: TDataSource
    DataSet = QT
    Left = 640
    Top = 144
  end
  object ADOC1: TADOConnection
    Connected = True
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source=C:\To' +
      'vari.mdb;Mode=Share Deny None;Persist Security Info=False;Jet OL' +
      'EDB:System database="";Jet OLEDB:Registry Path="";Jet OLEDB:Data' +
      'base Password="";Jet OLEDB:Engine Type=5;Jet OLEDB:Database Lock' +
      'ing Mode=1;Jet OLEDB:Global Partial Bulk Ops=2;Jet OLEDB:Global ' +
      'Bulk Transactions=1;Jet OLEDB:New Database Password="";Jet OLEDB' +
      ':Create System Database=False;Jet OLEDB:Encrypt Database=False;J' +
      'et OLEDB:Don'#39't Copy Locale on Compact=False;Jet OLEDB:Compact Wi' +
      'thout Replica Repair=False;Jet OLEDB:SFP=False'
    Mode = cmShareDenyNone
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 496
    Top = 144
  end
  object QT: TADOQuery
    Active = True
    Connection = ADOC1
    CursorType = ctStatic
    Parameters = <>
    SQL.Strings = (
      'SELECT * FROM tovar')
    Left = 576
    Top = 96
  end
  object ADOUpd: TADOQuery
    Connection = ADOC1
    Parameters = <>
    Left = 568
    Top = 152
  end
end
