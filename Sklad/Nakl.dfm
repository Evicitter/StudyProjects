object fmNakl: TfmNakl
  Left = 244
  Top = 216
  Caption = #1053#1082#1083#1072#1076#1085#1072#1103' '#1085#1072' '#1090#1086#1074#1072#1088#1099
  ClientHeight = 464
  ClientWidth = 854
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridNakl: TDBGrid
    Left = 0
    Top = 0
    Width = 854
    Height = 120
    Align = alTop
    DataSource = dsNakl
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 120
    Width = 854
    Height = 344
    Align = alClient
    Caption = #1058#1086#1074#1072#1088' '#1085#1072' '#1089#1082#1083#1072#1076#1077
    TabOrder = 1
    object DBNavigatorNakl: TDBNavigator
      Left = 2
      Top = 15
      Width = 850
      Height = 25
      DataSource = dsNakl
      Align = alTop
      TabOrder = 0
    end
    object DBGrid1: TDBGrid
      Left = 2
      Top = 69
      Width = 850
      Height = 248
      Align = alClient
      DataSource = dsNaklToTovar
      TabOrder = 1
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -11
      TitleFont.Name = 'MS Sans Serif'
      TitleFont.Style = []
    end
    object DBNavigator1: TDBNavigator
      Left = 2
      Top = 317
      Width = 850
      Height = 25
      DataSource = dsNaklToTovar
      Align = alBottom
      TabOrder = 2
    end
    object ToolBar1: TToolBar
      Left = 2
      Top = 40
      Width = 850
      Height = 29
      Caption = 'ToolBar1'
      TabOrder = 3
      object BitAdd: TBitBtn
        Left = 0
        Top = 0
        Width = 105
        Height = 22
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1087#1086#1089#1090#1072#1074#1082#1091
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 0
      end
      object BitBtn1: TBitBtn
        Left = 105
        Top = 0
        Width = 75
        Height = 22
        Caption = 'BitBtn1'
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 1
      end
      object btnReportSale: TBitBtn
        Left = 180
        Top = 0
        Width = 75
        Height = 22
        Caption = #1054#1090#1095#1105#1090
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 2
        OnClick = btnReportSaleClick
      end
    end
  end
  object dsNakl: TDataSource
    DataSet = DMMain.ADO_Nakladnaya
    Left = 760
    Top = 256
  end
  object dsNaklToTovar: TDataSource
    DataSet = DMMain.ADO_NakToTovar
    Left = 760
    Top = 360
  end
end
