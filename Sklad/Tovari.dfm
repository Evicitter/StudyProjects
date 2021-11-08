object fmTovari: TfmTovari
  Left = 491
  Top = 201
  Caption = #1056#1077#1076#1072#1082#1090#1086#1088' '#1090#1086#1074#1072#1088#1086#1074
  ClientHeight = 140
  ClientWidth = 313
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 77
    Height = 20
    Caption = #8470' '#1090#1086#1074#1072#1088#1072':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 40
    Width = 74
    Height = 20
    Caption = #1053#1072#1079#1074#1072#1085#1080#1077
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 72
    Width = 85
    Height = 20
    Caption = #1058#1080#1087' '#1090#1086#1074#1072#1088#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object DBEdit1: TDBEdit
    Left = 136
    Top = 40
    Width = 161
    Height = 21
    DataField = #1053#1072#1079#1074#1072#1085#1080#1077
    DataSource = dsTovari
    TabOrder = 0
  end
  object DBComboBox1: TDBComboBox
    Left = 136
    Top = 72
    Width = 161
    Height = 21
    DataField = #1058#1080#1087
    DataSource = dsTovari
    ItemHeight = 13
    Items.Strings = (
      #1052#1086#1083#1086#1095#1085#1086#1077
      #1061#1083#1077#1073
      #1050#1086#1085#1089#1077#1088#1074#1099)
    TabOrder = 1
  end
  object Button1: TButton
    Left = 136
    Top = 104
    Width = 75
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 216
    Top = 104
    Width = 75
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    ModalResult = 2
    TabOrder = 3
  end
  object DBEdit2: TDBEdit
    Left = 136
    Top = 8
    Width = 161
    Height = 21
    DataField = #8470'_'#1090#1086#1074#1072#1088#1072
    DataSource = dsTovari
    TabOrder = 4
  end
  object dsTovari: TDataSource
    DataSet = DMMain.ADO_Tovari
    Left = 64
    Top = 104
  end
  object ADOQueryTIP: TADOQuery
    Connection = DMMain.ADOConnection1
    Parameters = <>
    SQL.Strings = (
      'SELECT '#1058#1080#1087'_'#1090#1086#1074#1072#1088#1072' FROM '#1058#1086#1074#1072#1088
      'GROUP BY '#1058#1080#1087'_'#1090#1086#1074#1072#1088#1072)
    Left = 24
    Top = 104
  end
end
