object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Form2'
  ClientHeight = 476
  ClientWidth = 723
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 723
    Height = 476
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'main'
      ExplicitHeight = 391
      object GroupBox1: TGroupBox
        Left = 8
        Top = 8
        Width = 145
        Height = 105
        Caption = #1048#1085#1080#1094#1080#1072#1083#1080#1079#1072#1094#1080#1103' '#1089#1080#1076#1072
        TabOrder = 0
        object Label14: TLabel
          Left = 3
          Top = 66
          Width = 135
          Height = 13
          Caption = #1044#1086#1074#1077#1088#1080#1090#1077#1083#1100#1085#1072#1103' '#1074#1077#1088#1086#1103#1090'-'#1090#1100
        end
        object CheckBox1: TCheckBox
          Left = 3
          Top = 16
          Width = 134
          Height = 17
          Caption = #1041#1088#1072#1090#1100' '#1090#1077#1082#1091#1097#1077#1077' '#1074#1088#1077#1084#1103
          Checked = True
          State = cbChecked
          TabOrder = 0
          OnClick = CheckBox1Click
        end
        object Edit1: TEdit
          Left = 3
          Top = 39
          Width = 134
          Height = 21
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
        object Edit12: TEdit
          Left = 3
          Top = 81
          Width = 139
          Height = 21
          NumbersOnly = True
          TabOrder = 2
          Text = '50'
        end
      end
      object GroupBox2: TGroupBox
        Left = 159
        Top = 8
        Width = 202
        Height = 105
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1075#1077#1085#1077#1088#1072#1090#1086#1088#1072
        TabOrder = 1
        object Label1: TLabel
          Left = 3
          Top = 17
          Width = 39
          Height = 13
          Caption = #1052#1086#1076#1091#1083#1100
        end
        object Label2: TLabel
          Left = 3
          Top = 42
          Width = 64
          Height = 13
          Caption = #1055#1088#1080#1088#1072#1097#1077#1085#1080#1077
        end
        object Label3: TLabel
          Left = 3
          Top = 72
          Width = 58
          Height = 13
          Caption = #1052#1085#1086#1078#1080#1090#1077#1083#1100
        end
        object Edit2: TEdit
          Left = 78
          Top = 15
          Width = 121
          Height = 21
          TabOrder = 0
          Text = '2147483648'
        end
        object Edit3: TEdit
          Left = 78
          Top = 42
          Width = 121
          Height = 21
          TabOrder = 1
          Text = '16777215'
        end
        object Edit4: TEdit
          Left = 80
          Top = 69
          Width = 119
          Height = 21
          TabOrder = 2
          Text = '32767'
        end
      end
      object GroupBox3: TGroupBox
        Left = 367
        Top = 8
        Width = 345
        Height = 105
        Caption = #1058#1077#1089#1090
        TabOrder = 2
        object Label5: TLabel
          Left = 3
          Top = 17
          Width = 75
          Height = 13
          Caption = #1044#1083#1080#1085#1072' '#1084#1072#1089#1089#1080#1074#1072
        end
        object Label4: TLabel
          Left = 3
          Top = 69
          Width = 76
          Height = 13
          Caption = #1042#1099#1073#1086#1088' '#1084#1077#1090#1086#1076#1072'.'
        end
        object Button1: TButton
          Left = 288
          Top = 69
          Width = 54
          Height = 25
          Caption = #1047#1072#1087#1091#1089#1082
          TabOrder = 0
          OnClick = Button1Click
        end
        object TrackBar2: TTrackBar
          Left = 84
          Top = 15
          Width = 250
          Height = 26
          Max = 100
          Min = 1
          ParentShowHint = False
          Position = 100
          ShowHint = True
          ShowSelRange = False
          TabOrder = 1
        end
        object ComboBox1: TComboBox
          Left = 85
          Top = 69
          Width = 197
          Height = 21
          Style = csDropDownList
          ItemIndex = 0
          TabOrder = 2
          Text = #1051#1080#1085#1077#1081#1085#1099#1077' '#1082#1086#1085#1075#1088#1091#1101#1085#1090#1085#1099#1081
          Items.Strings = (
            #1051#1080#1085#1077#1081#1085#1099#1077' '#1082#1086#1085#1075#1088#1091#1101#1085#1090#1085#1099#1081
            #1050#1074#1072#1076#1088#1072#1090#1080#1095#1085#1099#1081' '#1082#1086#1085#1075#1088#1091#1101#1085#1090#1085#1099#1081' '
            #1050#1086#1074#1101#1102
            #1060#1080#1073#1086#1085#1072#1095#1095#1080
            #1043#1088#1080#1085)
        end
      end
      object GroupBox4: TGroupBox
        Left = 11
        Top = 119
        Width = 318
        Height = 138
        Caption = #1093#1072#1088#1072#1082#1090#1077#1088#1080#1089#1090#1080#1082#1072' '#1076#1083#1103' '#1088#1072#1074#1085#1086#1084#1077#1088#1085#1086#1075#1086' '#1089#1083#1091#1095'. '#1088#1072#1089#1087#1088#1077#1076#1077#1083#1077#1085#1080#1103
        TabOrder = 3
        object Label6: TLabel
          Left = 3
          Top = 20
          Width = 139
          Height = 13
          AutoSize = False
          Caption = #1052#1072#1090#1077#1084#1072#1090#1080#1095#1077#1089#1082#1086#1077' '#1086#1078#1080#1076#1072#1085#1080#1077
        end
        object Label7: TLabel
          Left = 3
          Top = 49
          Width = 139
          Height = 13
          AutoSize = False
          Caption = #1044#1080#1089#1087#1077#1088#1089#1080#1103
        end
        object Label8: TLabel
          Left = 3
          Top = 84
          Width = 171
          Height = 13
          AutoSize = False
          Caption = #1089#1088#1077#1076#1085#1077#1082#1074#1072#1076#1088#1072#1090#1080#1095#1085#1086#1077' '#1086#1090#1082#1083#1086#1085#1077#1085#1080#1077
        end
        object Label13: TLabel
          Left = 3
          Top = 116
          Width = 171
          Height = 13
          AutoSize = False
          Caption = #1050#1088#1080#1090#1077#1088#1080#1081' '#1093#1080'-'#1082#1074#1072#1076#1088#1072#1090' (exp)'
        end
        object Edit5: TEdit
          Left = 185
          Top = 12
          Width = 121
          Height = 21
          Enabled = False
          TabOrder = 0
          Text = '0'
        end
        object Edit6: TEdit
          Left = 185
          Top = 39
          Width = 121
          Height = 21
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
        object Edit7: TEdit
          Left = 187
          Top = 76
          Width = 122
          Height = 21
          Enabled = False
          TabOrder = 2
          Text = '0'
        end
        object Edit11: TEdit
          Left = 187
          Top = 113
          Width = 122
          Height = 21
          Enabled = False
          TabOrder = 3
          Text = '0'
        end
      end
      object GroupBox5: TGroupBox
        Left = 367
        Top = 127
        Width = 345
        Height = 130
        Caption = #1063#1072#1089#1090#1086#1090#1085#1099#1081' '#1090#1077#1089#1090
        TabOrder = 4
        object Label9: TLabel
          Left = 16
          Top = 24
          Width = 45
          Height = 13
          Caption = 'mr '#8211' '#963'r ='
        end
        object Label10: TLabel
          Left = 16
          Top = 43
          Width = 47
          Height = 13
          Caption = 'mr + '#963'r ='
        end
        object Label11: TLabel
          Left = 16
          Top = 75
          Width = 86
          Height = 13
          Caption = 'mr + '#963'r - mr - '#963'r='
        end
        object Label12: TLabel
          Left = 223
          Top = 80
          Width = 110
          Height = 13
          Caption = #1074#1099#1087#1072#1074#1096#1080#1093' '#1089#1095'=57.7% '
        end
        object Edit8: TEdit
          Left = 69
          Top = 16
          Width = 148
          Height = 21
          Enabled = False
          TabOrder = 0
          Text = '0'
        end
        object Edit9: TEdit
          Left = 69
          Top = 43
          Width = 148
          Height = 21
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
        object Edit10: TEdit
          Left = 108
          Top = 70
          Width = 109
          Height = 21
          Enabled = False
          TabOrder = 2
          Text = '0'
        end
      end
      object GroupBox6: TGroupBox
        Left = 14
        Top = 263
        Width = 315
        Height = 162
        Caption = #1055#1088#1086#1074#1077#1088#1082#1080' '#1085#1072' '#1089#1090#1072#1090#1080#1089#1090#1080#1095#1077#1089#1082#1091#1102' '#1085#1077#1079#1072#1074#1080#1089#1080#1084#1086#1089#1090#1100
        TabOrder = 5
        object Label15: TLabel
          Left = 3
          Top = 28
          Width = 171
          Height = 13
          AutoSize = False
          Caption = #1094#1080#1092#1088#1099' '#1074' '#1087#1086#1089#1083#1077#1076#1086#1074#1072#1090#1077#1083#1100#1085#1086#1089#1090#1080' '
        end
        object Label16: TLabel
          Left = 3
          Top = 68
          Width = 171
          Height = 13
          AutoSize = False
          Caption = #1057#1077#1088#1080#1103' '#1080#1079' '#1086#1076#1080#1085#1072#1082#1086#1074#1099#1093' '#1094#1080#1092#1088' n=2'
        end
        object Label17: TLabel
          Left = 3
          Top = 100
          Width = 171
          Height = 13
          AutoSize = False
          Caption = #1057#1077#1088#1080#1103' '#1080#1079' '#1086#1076#1080#1085#1072#1082#1086#1074#1099#1093' '#1094#1080#1092#1088' n=3'
        end
        object Edit13: TEdit
          Left = 180
          Top = 25
          Width = 122
          Height = 21
          Enabled = False
          TabOrder = 0
          Text = '0'
        end
        object Edit14: TEdit
          Left = 180
          Top = 65
          Width = 122
          Height = 21
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
        object Edit15: TEdit
          Left = 180
          Top = 100
          Width = 122
          Height = 21
          Enabled = False
          TabOrder = 2
          Text = '0'
        end
      end
    end
  end
end
