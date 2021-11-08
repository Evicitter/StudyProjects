object fmSQL: TfmSQL
  Left = 226
  Top = 145
  Caption = 'SQL '#1047#1072#1087#1088#1086#1089#1099
  ClientHeight = 468
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 854
    Height = 468
    ActivePage = TabSheetDiagram
    Align = alClient
    TabOrder = 0
    object TabSheetPrimers: TTabSheet
      Caption = #1060#1086#1088#1084#1080#1088#1091#1077#1084#1099#1077' '#1079#1072#1087#1088#1086#1089#1099
      object RadioGroupPrimers: TRadioGroup
        Left = 0
        Top = 0
        Width = 846
        Height = 105
        Align = alTop
        Caption = #1042#1099#1073#1088#1072#1090#1100' '#1076#1072#1085#1085#1099#1077
        Items.Strings = (
          #1057#1082#1083#1072#1076
          #1058#1086#1074#1072#1088
          #1053#1072#1082#1083#1072#1076#1085#1072#1103' '#1085#1072' '#1090#1086#1074#1072#1088)
        TabOrder = 0
        OnClick = RadioGroupPrimersClick
      end
      object DBGridPrimers: TDBGrid
        Left = 0
        Top = 105
        Width = 846
        Height = 335
        Align = alClient
        DataSource = dsPrimers
        TabOrder = 1
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
      end
    end
    object TabSheetParams: TTabSheet
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1080#1095#1077#1089#1082#1080#1077' '#1079#1072#1087#1088#1086#1089#1099
      ImageIndex = 1
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 846
        Height = 361
        Align = alTop
        Caption = #1055#1086#1089#1090#1072#1074#1086#1082' '#1079#1072' '#1087#1077#1088#1080#1086#1076
        TabOrder = 0
        object Label1: TLabel
          Left = 2
          Top = 15
          Width = 150
          Height = 344
          Align = alLeft
          Caption = #1044#1072#1090#1072' '#1085#1072#1095#1072#1083#1072' '#1089#1088#1086#1082#1072' '#1087#1086#1089#1090#1072#1074#1082#1080':'
          ExplicitHeight = 13
        end
        object Label2: TLabel
          Left = 673
          Top = 15
          Width = 171
          Height = 344
          Align = alRight
          Caption = #1044#1072#1090#1072'  '#1086#1082#1086#1085#1095#1072#1085#1080#1103' '#1089#1088#1086#1082#1072' '#1087#1086#1089#1090#1072#1074#1082#1080':'
          ExplicitHeight = 13
        end
        object DateTimePickerDate1: TDateTimePicker
          Left = 0
          Top = 32
          Width = 153
          Height = 21
          Date = 39882.672601088000000000
          Time = 39882.672601088000000000
          TabOrder = 0
        end
        object DateTimePickerDate2: TDateTimePicker
          Left = 672
          Top = 32
          Width = 169
          Height = 21
          Date = 39893.675857407400000000
          Time = 39893.675857407400000000
          TabOrder = 1
        end
        object ButtonResult: TButton
          Left = 16
          Top = 80
          Width = 113
          Height = 33
          BiDiMode = bdLeftToRight
          Caption = #1055#1086#1089#1090#1072#1074#1082#1080'  '#1079#1072' '#1087#1077#1088#1080#1086#1076
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          OnClick = ButtonResultClick
        end
        object DBGrid1: TDBGrid
          Left = 152
          Top = 15
          Width = 521
          Height = 344
          Align = alClient
          DataSource = DataSource1
          TabOrder = 3
          TitleFont.Charset = DEFAULT_CHARSET
          TitleFont.Color = clWindowText
          TitleFont.Height = -11
          TitleFont.Name = 'MS Sans Serif'
          TitleFont.Style = []
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = #1050#1086#1085#1090#1088#1086#1083#1100#1085#1072#1103' '#1088#1072#1073#1086#1090#1072
      ImageIndex = 2
      object DBGrid2: TDBGrid
        Left = 0
        Top = 116
        Width = 846
        Height = 324
        Align = alBottom
        DataSource = dsKontrol
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
      end
      object Button1: TButton
        Left = 8
        Top = 8
        Width = 75
        Height = 25
        Caption = #1042#1099#1074#1077#1089#1090#1080
        TabOrder = 1
        OnClick = Button1Click
      end
      object Edit1: TEdit
        Left = 88
        Top = 8
        Width = 209
        Height = 21
        TabOrder = 2
        Text = '1'
        OnChange = Edit1Change
      end
    end
    object TabSheetModify: TTabSheet
      Caption = #1047#1072#1087#1088#1086#1089#1099' '#1080#1079#1084#1077#1085#1077#1085#1080#1103
      ImageIndex = 3
      object LabelName: TLabel
        Left = 16
        Top = 48
        Width = 88
        Height = 13
        Caption = #1053#1072#1079#1074#1072#1085#1080#1077' '#1090#1086#1074#1072#1088#1072
      end
      object LabelType: TLabel
        Left = 16
        Top = 80
        Width = 57
        Height = 13
        Caption = #1058#1080#1087' '#1090#1086#1074#1072#1088#1072
      end
      object LabelNum: TLabel
        Left = 16
        Top = 16
        Width = 51
        Height = 13
        Caption = #8470' '#1058#1086#1074#1072#1088#1072
      end
      object DBGridModify: TDBGrid
        Left = 320
        Top = 0
        Width = 526
        Height = 433
        DataSource = fmList.dsTovari
        TabOrder = 0
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
      end
      object EditName: TEdit
        Left = 192
        Top = 48
        Width = 121
        Height = 21
        TabOrder = 1
        Text = #1086#1095#1080#1089#1090#1080#1090#1077
      end
      object EditType: TEdit
        Left = 192
        Top = 80
        Width = 121
        Height = 21
        TabOrder = 2
        Text = #1086#1095#1080#1089#1090#1080#1090#1077
      end
      object ButtonInsert: TButton
        Left = 8
        Top = 104
        Width = 75
        Height = 25
        Caption = #1044#1086#1073#1072#1074#1080#1090#1100
        TabOrder = 3
        OnClick = ButtonInsertClick
      end
      object ButtonUpdate: TButton
        Left = 240
        Top = 104
        Width = 75
        Height = 25
        Caption = #1048#1079#1084#1077#1085#1080#1090#1100
        TabOrder = 4
        OnClick = ButtonUpdateClick
      end
      object ButtonDelete1: TButton
        Left = 0
        Top = 144
        Width = 313
        Height = 25
        Hint = #1059#1076#1072#1083#1080#1090#1100' '#1087#1086' ('#1053#1072#1079#1074#1072#1085#1080#1102') '#1080#1083#1080' ('#1058#1080#1087#1091') '#1080#1083#1080' ('#1090#1086#1084#1091' '#1080' '#1076#1088')'
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1087#1086' ('#1053#1072#1079#1074#1072#1085#1080#1102') '#1080#1083#1080' ('#1058#1080#1087#1091') '#1080#1083#1080' ('#1055#1086' '#1085#1086#1084#1077#1088#1091')'
        ParentShowHint = False
        ShowHint = True
        TabOrder = 5
        OnClick = ButtonDelete1Click
      end
      object ButtonDelete2: TButton
        Left = -8
        Top = 176
        Width = 321
        Height = 25
        Hint = #1059#1076#1072#1083#1080#1090#1100' '#1087#1086' '#1053#1072#1079#1074#1072#1085#1080#1102' '#1080' '#1058#1080#1087#1091' '#1089#1086#1074#1084#1077#1089#1090#1085#1086
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1087#1086' ('#1053#1072#1079#1074#1072#1085#1080#1102' '#1080' '#1058#1080#1087#1091' '#1080' '#1053#1086#1084#1077#1088#1091') '#1089#1086#1074#1084#1077#1089#1090#1085#1086
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        OnClick = ButtonDelete2Click
      end
      object EditNum: TEdit
        Left = 192
        Top = 16
        Width = 121
        Height = 21
        TabOrder = 7
        Text = '1'
        OnChange = EditNumChange
      end
    end
    object TabSheetDiagram: TTabSheet
      Caption = #1055#1086#1089#1090#1072#1074#1082#1080' '#1090#1086#1074#1072#1088#1086#1074
      ImageIndex = 4
      OnShow = TabSheetDiagramShow
      object DBChartDiagram: TDBChart
        Left = 0
        Top = 41
        Width = 846
        Height = 399
        BackWall.Brush.Color = clWhite
        Title.Text.Strings = (
          #1057#1091#1084#1084#1072' '#1090#1086#1074#1072#1088#1086#1074' '#1074' '#1085#1072#1082#1083#1072#1076#1085#1099#1093)
        View3DOptions.Elevation = 315
        View3DOptions.Orthogonal = False
        View3DOptions.Perspective = 0
        View3DOptions.Rotation = 360
        Align = alClient
        TabOrder = 0
        object Series1: TBarSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Style = smsValue
          Marks.Visible = True
          DataSource = ADOQueryDiagram
          Title = 'SeriesBar'
          Gradient.Direction = gdTopBottom
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
          YValues.ValueSource = #1057#1091#1084#1084#1072#1058#1086#1074#1072#1088#1086#1074
        end
        object Series2: TPieSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.Style = smsLabelPercent
          Marks.Visible = True
          DataSource = ADOQueryDiagram
          Title = 'SeriesPie'
          Active = False
          Gradient.Direction = gdRadial
          OtherSlice.Legend.Visible = False
          OtherSlice.Text = 'Other'
          PieValues.Name = 'Pie'
          PieValues.Order = loNone
          PieValues.ValueSource = #1057#1091#1084#1084#1072#1058#1086#1074#1072#1088#1086#1074
        end
      end
      object RadioGroupDiagram: TRadioGroup
        Left = 0
        Top = 0
        Width = 846
        Height = 41
        Align = alTop
        Caption = 'RadioGroupDiagram'
        Columns = 2
        ItemIndex = 0
        Items.Strings = (
          #1041#1083#1086#1095#1085#1072#1103
          #1050#1088#1091#1075#1086#1074#1072#1103)
        TabOrder = 1
        OnClick = RadioGroupDiagramClick
      end
    end
  end
  object ADOQueryPrimers: TADOQuery
    Connection = DMMain.ADOConnection1
    Parameters = <>
    Left = 632
    Top = 400
  end
  object dsPrimers: TDataSource
    DataSet = ADOQueryPrimers
    Left = 528
    Top = 368
  end
  object ADOQueryResult: TADOQuery
    Connection = DMMain.ADOConnection1
    CursorType = ctStatic
    Parameters = <
      item
        Name = 'd1'
        Attributes = [paNullable]
        DataType = ftFixedChar
        NumericScale = 255
        Precision = 255
        Size = 510
        Value = Null
      end
      item
        Name = 'd2'
        Attributes = [paNullable]
        DataType = ftFixedChar
        NumericScale = 255
        Precision = 255
        Size = 510
        Value = Null
      end>
    SQL.Strings = (
      
        'SELECT t.'#1053#1072#1079#1074#1072#1085#1080#1077' AS '#1053#1072#1079#1074#1072#1085#1080#1077'_'#1090#1086#1074#1072#1088#1072', NT.'#8470'_'#1053#1072#1082#1083#1072#1076#1085#1086#1081' As '#8470'_'#1053#1072#1082#1083#1072#1076 +
        #1085#1086#1081'_'#1082'_'#1090#1086#1074#1072#1088#1091', NT.'#1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' AS '#1050#1086#1083#1080#1095#1077#1089#1090#1074#1086#1058#1086#1074#1072#1088#1086#1074', N.'#1044#1072#1090#1072' As '#1044#1072#1090#1072
      'FROM '#1058#1086#1074#1072#1088' t, ['#1053#1072#1082#1083#1072#1076#1085#1072#1103' '#1082' '#1090#1086#1074#1072#1088#1091'] NT, '#1053#1072#1082#1083#1072#1076#1085#1072#1103' N'
      
        'WHERE (N.'#1044#1072#1090#1072' Between :d1 And :d2) AND (N.'#8470'_'#1053#1072#1082#1083#1072#1076#1085#1086#1081'=NT.'#8470'_'#1053#1072#1082#1083#1072 +
        #1076#1085#1086#1081') AND (NT.'#8470'_'#1058#1086#1074#1072#1088#1072'=t.'#8470'_'#1058#1086#1074#1072#1088#1072')')
    Left = 96
    Top = 400
    object ADOQueryResult___: TIntegerField
      FieldName = #8470'_'#1053#1072#1082#1083#1072#1076#1085#1086#1081'_'#1082'_'#1090#1086#1074#1072#1088#1091
    end
    object ADOQueryResultDSDesigner2: TDateTimeField
      FieldName = #1044#1072#1090#1072
    end
    object ADOQueryResult_: TWideStringField
      DisplayWidth = 20
      FieldName = #1053#1072#1079#1074#1072#1085#1080#1077'_'#1090#1086#1074#1072#1088#1072
      Size = 100
    end
    object ADOQueryResultDSDesigner: TIntegerField
      FieldName = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086#1058#1086#1074#1072#1088#1086#1074
    end
  end
  object DataSource1: TDataSource
    DataSet = ADOQueryResult
    Left = 24
    Top = 392
  end
  object dsKontrol: TDataSource
    DataSet = ADOQueryKontrol
    Left = 200
    Top = 408
  end
  object ADOQueryKontrol: TADOQuery
    Connection = DMMain.ADOConnection1
    CursorType = ctStatic
    Parameters = <
      item
        Name = 'd1'
        Attributes = [paNullable]
        DataType = ftFixedChar
        NumericScale = 255
        Precision = 255
        Size = 510
        Value = Null
      end>
    SQL.Strings = (
      
        'SELECT t.'#1053#1072#1079#1074#1072#1085#1080#1077' AS '#1053#1072#1079#1074#1072#1085#1080#1077#1058#1086#1074#1072#1088#1072', t.'#1058#1080#1087' AS '#1058#1080#1087#1058#1086#1074#1072#1088#1072', nt.'#8470'_'#1058#1086 +
        #1074#1072#1088#1072', COUNT(nt.'#8470'_'#1053#1072#1082#1083#1072#1076#1085#1086#1081') AS '#1050#1086#1083#1080#1095#1077#1089#1090#1074#1086'_'#1085#1072#1082#1083#1072#1076#1085#1099#1093', SUM(nt.'#1050#1086#1083#1080 +
        #1095#1077#1089#1090#1074#1086') AS '#1057#1091#1084#1084#1072#1058#1086#1074#1072#1088#1086#1074
      'FROM ['#1053#1072#1082#1083#1072#1076#1085#1072#1103' '#1082' '#1090#1086#1074#1072#1088#1091'] nt, '#1058#1086#1074#1072#1088' t'
      'WHERE (nt.'#8470'_'#1058#1086#1074#1072#1088#1072'=t.'#8470'_'#1058#1086#1074#1072#1088#1072')'
      'GROUP BY nt.'#8470'_'#1058#1086#1074#1072#1088#1072', t.'#1053#1072#1079#1074#1072#1085#1080#1077', t.'#1058#1080#1087
      'HAVING SUM(nt.'#1050#1086#1083#1080#1095#1077#1089#1090#1074#1086')>=:d1'
      'ORDER BY t.'#1058#1080#1087' desc')
    Left = 248
    Top = 424
    object ADOQueryKontrolDSDesigner: TWideStringField
      DisplayWidth = 20
      FieldName = #1053#1072#1079#1074#1072#1085#1080#1077#1058#1086#1074#1072#1088#1072
      Size = 100
    end
    object ADOQueryKontrolDSDesigner2: TWideStringField
      DisplayWidth = 20
      FieldName = #1058#1080#1087#1058#1086#1074#1072#1088#1072
      Size = 50
    end
    object ADOQueryKontrol_: TIntegerField
      FieldName = #8470'_'#1058#1086#1074#1072#1088#1072
    end
    object ADOQueryKontrol_2: TIntegerField
      FieldName = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086'_'#1085#1072#1082#1083#1072#1076#1085#1099#1093
    end
    object ADOQueryKontrolDSDesigner3: TFloatField
      FieldName = #1057#1091#1084#1084#1072#1058#1086#1074#1072#1088#1086#1074
    end
  end
  object ADOQueryModify: TADOQuery
    Connection = DMMain.ADOConnection1
    DataSource = fmList.dsTovari
    Parameters = <>
    Left = 480
    Top = 384
  end
  object ADOQueryDiagram: TADOQuery
    Connection = DMMain.ADOConnection1
    CursorType = ctStatic
    OnCalcFields = ADOQueryDiagramCalcFields
    Parameters = <>
    SQL.Strings = (
      'SELECT nt.'#8470'_'#1053#1072#1082#1083#1072#1076#1085#1086#1081', SUM(nt.'#1050#1086#1083#1080#1095#1077#1089#1090#1074#1086') AS '#1057#1091#1084#1084#1072#1058#1086#1074#1072#1088#1086#1074
      'FROM ['#1053#1072#1082#1083#1072#1076#1085#1072#1103' '#1082' '#1090#1086#1074#1072#1088#1091'] nt'
      'GROUP BY nt.'#8470'_'#1053#1072#1082#1083#1072#1076#1085#1086#1081)
    Left = 360
    Top = 392
    object ADOQueryDiagramDSDesigner: TFloatField
      FieldName = #1057#1091#1084#1084#1072#1058#1086#1074#1072#1088#1086#1074
    end
    object ADOQueryDiagramField: TStringField
      FieldKind = fkCalculated
      FieldName = #1053#1072#1082#1083#1072#1076#1085#1072#1103
      Size = 30
      Calculated = True
    end
    object ADOQueryDiagram_: TIntegerField
      FieldName = #8470'_'#1053#1072#1082#1083#1072#1076#1085#1086#1081
    end
  end
end
