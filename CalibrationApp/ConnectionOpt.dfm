object ConnectOptions: TConnectOptions
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1089#1086#1077#1076#1080#1085#1077#1085#1080#1103
  ClientHeight = 312
  ClientWidth = 382
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object butok: TButton
    Left = 289
    Top = 8
    Width = 75
    Height = 25
    Caption = 'Ok'
    ModalResult = 1
    TabOrder = 0
  end
  object bitcancel: TButton
    Left = 289
    Top = 39
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 113
    Height = 312
    Align = alLeft
    TabOrder = 2
    object LPortName: TLabel
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 103
      Height = 13
      Margins.Top = 5
      Margins.Bottom = 10
      Align = alTop
      Caption = #1055#1086#1088#1090
      ExplicitWidth = 25
    end
    object LSpeed: TLabel
      AlignWithMargins = True
      Left = 5
      Top = 53
      Width = 103
      Height = 13
      Margins.Top = 10
      Margins.Bottom = 10
      Align = alTop
      Caption = #1057#1082#1086#1088#1086#1089#1090#1100
      ExplicitWidth = 48
    end
    object LParity: TLabel
      AlignWithMargins = True
      Left = 5
      Top = 86
      Width = 103
      Height = 13
      Margins.Top = 10
      Margins.Bottom = 10
      Align = alTop
      Caption = #1063#1105#1090#1085#1086#1089#1090#1100
      ExplicitWidth = 48
    end
    object LTypeSensor: TLabel
      AlignWithMargins = True
      Left = 5
      Top = 119
      Width = 103
      Height = 13
      Margins.Top = 10
      Margins.Bottom = 10
      Align = alTop
      Caption = #1058#1080#1087' '#1076#1072#1090#1095#1080#1082#1072
      ExplicitWidth = 64
    end
  end
  object GroupBox2: TGroupBox
    Left = 113
    Top = 0
    Width = 170
    Height = 312
    Align = alLeft
    TabOrder = 3
    object portCB: TComboBox
      AlignWithMargins = True
      Left = 5
      Top = 20
      Width = 160
      Height = 21
      Margins.Top = 5
      Margins.Bottom = 5
      Align = alTop
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 0
      Text = 'COM1'
      Items.Strings = (
        'COM1'
        'COM2'
        'COM3'
        'COM4'
        'COM5'
        'COM6'
        'COM7'
        'COM8'
        'COM9')
    end
    object speedCB: TComboBox
      AlignWithMargins = True
      Left = 5
      Top = 51
      Width = 160
      Height = 21
      Margins.Top = 5
      Margins.Bottom = 5
      Align = alTop
      Style = csDropDownList
      ItemIndex = 5
      TabOrder = 1
      Text = '19200'
      Items.Strings = (
        '1200'
        '2400'
        '4800'
        '9600'
        '14400'
        '19200'
        '115200'
        '128000'
        '256000')
    end
    object parityCB: TComboBox
      AlignWithMargins = True
      Left = 5
      Top = 82
      Width = 160
      Height = 21
      Margins.Top = 5
      Margins.Bottom = 5
      Align = alTop
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 2
      Text = 'NOPARITY'
      Items.Strings = (
        'NOPARITY'
        'ODDPARITY'
        'EVENPARITY'
        'MARKPARITY'
        'SPACEPARITY')
    end
    object cbTypeSensor: TComboBox
      AlignWithMargins = True
      Left = 5
      Top = 113
      Width = 160
      Height = 21
      Margins.Top = 5
      Margins.Bottom = 5
      Align = alTop
      Style = csDropDownList
      ItemIndex = 3
      TabOrder = 3
      Text = 'Italon'
      Items.Strings = (
        'Omni (Digit)'
        'Omni (Freq)'
        'ArNavi'
        'Italon'
        #1058#1088#1072#1085#1089#1054#1087#1090#1080#1084#1072)
    end
  end
end
