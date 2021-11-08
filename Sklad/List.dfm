object fmList: TfmList
  Left = 273
  Top = 219
  Caption = 'fmList'
  ClientHeight = 464
  ClientWidth = 854
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter2: TSplitter
    Left = 570
    Top = 83
    Height = 381
    Align = alRight
  end
  object DBGrid1: TDBGrid
    Left = 0
    Top = 83
    Width = 570
    Height = 381
    Align = alClient
    DataSource = DataSource1
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'MS Sans Serif'
    TitleFont.Style = []
  end
  object DBNavigator1: TDBNavigator
    Left = 0
    Top = 0
    Width = 854
    Height = 25
    DataSource = DataSource1
    Align = alTop
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 573
    Top = 83
    Width = 281
    Height = 381
    Align = alRight
    Caption = #1048#1085#1092#1086#1088#1084#1072#1094#1080#1103' '#1086' '#1090#1086#1074#1072#1088#1077
    TabOrder = 2
    object DBText1: TDBText
      Left = 2
      Top = 44
      Width = 277
      Height = 17
      Align = alTop
      DataField = #1053#1072#1079#1074#1072#1085#1080#1077
      DataSource = dsTovari
    end
    object Splitter1: TSplitter
      Left = 2
      Top = 166
      Width = 277
      Height = 3
      Cursor = crVSplit
      Align = alTop
    end
    object Label1: TLabel
      Left = 2
      Top = 169
      Width = 277
      Height = 13
      Align = alTop
      Caption = #1054#1087#1080#1089#1072#1085#1080#1077' '#1090#1086#1074#1072#1088#1072
      ExplicitWidth = 88
    end
    object DBImage1: TDBImage
      Left = 2
      Top = 61
      Width = 277
      Height = 105
      Align = alTop
      DataField = #1060#1086#1090#1086
      DataSource = dsTovari
      TabOrder = 0
    end
    object DBMemo1: TDBMemo
      Left = 2
      Top = 211
      Width = 277
      Height = 168
      Align = alClient
      DataField = #1054#1087#1080#1089#1072#1085#1080#1077
      DataSource = dsTovari
      TabOrder = 1
    end
    object ToolBar2: TToolBar
      Left = 2
      Top = 182
      Width = 277
      Height = 29
      ButtonHeight = 21
      ButtonWidth = 67
      Caption = 'ToolBar2'
      ParentShowHint = False
      ShowCaptions = True
      ShowHint = True
      TabOrder = 2
      object ToolButtonCopyMemo: TToolButton
        Left = 0
        Top = 2
        Hint = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
        Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
        ImageIndex = 0
        OnClick = ToolButtonCopyMemoClick
      end
      object ToolButtonPasteMemo: TToolButton
        Left = 67
        Top = 2
        Hint = #1042#1089#1090#1072#1074#1080#1090#1100
        Caption = #1042#1089#1090#1072#1074#1080#1090#1100
        ImageIndex = 1
        OnClick = ToolButtonPasteMemoClick
      end
      object ToolButtonOpenMemo: TToolButton
        Left = 134
        Top = 2
        Hint = #1054#1090#1082#1088#1099#1090#1100
        Caption = #1054#1090#1082#1088#1099#1090#1100
        ImageIndex = 2
        OnClick = ToolButtonOpenMemoClick
      end
      object ToolButtonSaveMemo: TToolButton
        Left = 201
        Top = 2
        Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        ImageIndex = 3
        OnClick = ToolButtonSaveMemoClick
      end
    end
    object ToolBar3: TToolBar
      Left = 2
      Top = 15
      Width = 277
      Height = 29
      ButtonHeight = 21
      ButtonWidth = 67
      Caption = 'ToolBar3'
      ParentShowHint = False
      ShowCaptions = True
      ShowHint = True
      TabOrder = 3
      object ToolButtonCopyImage: TToolButton
        Left = 0
        Top = 2
        Hint = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
        Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
        ImageIndex = 0
        OnClick = ToolButtonCopyImageClick
      end
      object ToolButtonPasteImage: TToolButton
        Left = 67
        Top = 2
        Hint = #1042#1089#1090#1072#1074#1080#1090#1100
        Caption = #1042#1089#1090#1072#1074#1080#1090#1100
        ImageIndex = 1
        OnClick = ToolButtonPasteImageClick
      end
      object ToolButtonOpenImage: TToolButton
        Left = 134
        Top = 2
        Caption = #1054#1090#1082#1088#1099#1090#1100
        ImageIndex = 2
        OnClick = ToolButtonOpenImageClick
      end
      object ToolButtonSaveImage: TToolButton
        Left = 201
        Top = 2
        Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        ImageIndex = 3
        OnClick = ToolButtonSaveImageClick
      end
    end
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 25
    Width = 854
    Height = 29
    ButtonHeight = 21
    ButtonWidth = 60
    Caption = 'ToolBar1'
    ShowCaptions = True
    TabOrder = 3
    object ToolButton1: TToolButton
      Left = 0
      Top = 2
      Caption = #1042' '#1085#1072#1095#1072#1083#1086
      ImageIndex = 0
      OnClick = ToolButton1Click
    end
    object ToolButton2: TToolButton
      Left = 60
      Top = 2
      Caption = #1053#1072#1079#1072#1076
      ImageIndex = 1
      OnClick = ToolButton2Click
    end
    object ToolButton3: TToolButton
      Left = 120
      Top = 2
      Caption = #1042#1087#1077#1088#1105#1076
      ImageIndex = 2
      OnClick = ToolButton3Click
    end
    object ToolButton4: TToolButton
      Left = 180
      Top = 2
      Caption = #1042' '#1082#1086#1085#1077#1094
      ImageIndex = 3
      OnClick = ToolButton4Click
    end
    object ToolButton5: TToolButton
      Left = 240
      Top = 2
      Width = 8
      Caption = 'ToolButton5'
      ImageIndex = 4
      Style = tbsSeparator
    end
    object ToolButton6: TToolButton
      Left = 248
      Top = 2
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100
      ImageIndex = 4
      OnClick = ToolButton6Click
    end
    object ToolButton7: TToolButton
      Left = 308
      Top = 2
      Caption = #1059#1076#1072#1083#1080#1090#1100
      ImageIndex = 5
      OnClick = ToolButton7Click
    end
    object ToolButton8: TToolButton
      Left = 368
      Top = 2
      Caption = #1048#1079#1084#1077#1085#1080#1090#1100
      ImageIndex = 6
      OnClick = ToolButton8Click
    end
    object ToolButton9: TToolButton
      Left = 428
      Top = 2
      Width = 8
      Caption = 'ToolButton9'
      ImageIndex = 7
      Style = tbsSeparator
    end
    object ToolButton10: TToolButton
      Left = 436
      Top = 2
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      ImageIndex = 7
      OnClick = ToolButton10Click
    end
    object ToolButton11: TToolButton
      Left = 496
      Top = 2
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100
      ImageIndex = 8
      OnClick = ToolButton11Click
    end
  end
  object ToolBarFind: TToolBar
    Left = 0
    Top = 54
    Width = 854
    Height = 29
    Caption = 'ToolBarFind'
    TabOrder = 4
    object CheckBoxFilter: TCheckBox
      Left = 0
      Top = 0
      Width = 97
      Height = 22
      Caption = #1060#1080#1083#1100#1090#1088#1086#1074#1072#1090#1100
      TabOrder = 0
      OnClick = CheckBoxFilterClick
    end
    object EditFilter: TEdit
      Left = 97
      Top = 0
      Width = 121
      Height = 22
      TabOrder = 1
      Text = 'EditFilter'
    end
    object btnFind: TBitBtn
      Left = 218
      Top = 0
      Width = 75
      Height = 22
      Caption = #1055#1086#1080#1089#1082
      Default = True
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 2
      OnClick = btnFindClick
    end
  end
  object DataSource1: TDataSource
    DataSet = DMMain.ADO_Nakladnaya
    Left = 24
    Top = 400
  end
  object dsTovari: TDataSource
    DataSet = DMMain.ADO_Tovari
    Left = 512
    Top = 408
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = '*.txt'
    Filter = 'Text File ( *.txt )|*.txt'
    Title = #1054#1090#1082#1088#1099#1090#1100' '#1090#1077#1082#1089#1090#1086#1074#1086#1081' '#1092#1072#1081#1083
    Left = 488
    Top = 296
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Text File ( *.txt )|*.txt'
    Title = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1090#1077#1082#1089#1090#1086#1074#1086#1081' '#1092#1072#1081#1083
    Left = 520
    Top = 296
  end
  object OpenPictureDialog1: TOpenPictureDialog
    Title = #1054#1090#1082#1088#1099#1090#1100' '#1088#1080#1089#1091#1085#1086#1082
    Left = 488
    Top = 192
  end
  object SavePictureDialog1: TSavePictureDialog
    Title = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1088#1080#1089#1091#1085#1086#1082
    Left = 520
    Top = 192
  end
end
