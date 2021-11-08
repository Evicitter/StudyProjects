object DMMain: TDMMain
  OldCreateOrder = False
  Height = 218
  Width = 505
  object ADOConnection1: TADOConnection
    Connected = True
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\Admin\Docu' +
      'ments\RAD Studio\Projects\Sklad\SkladTovarov.mdb;Persist Securit' +
      'y Info=False'
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 40
    Top = 8
  end
  object ADO_Nakladnaya: TADOTable
    Active = True
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = #1053#1072#1082#1083#1072#1076#1085#1072#1103
    Left = 40
    Top = 72
    object ADO_NakladnayaDSDesigner3: TDateTimeField
      FieldName = #1044#1072#1090#1072
    end
    object ADO_NakladnayaDSDesigner4: TWideStringField
      FieldName = #1055#1086#1076#1087#1080#1089#1100
      Size = 30
    end
    object ADO_NakladnayaDSDesigner5: TWideStringField
      FieldName = #1048#1085#1080#1094#1080#1072#1083#1099
      Size = 30
    end
    object ADO_Nakladnaya_: TIntegerField
      FieldName = #8470'_'#1085#1072#1082#1083#1072#1076#1085#1086#1081
    end
    object ADO_NakladnayaDSDesigner: TIntegerField
      FieldName = #1050#1086#1076
    end
  end
  object ADO_Sklad: TADOTable
    Active = True
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = #1057#1082#1083#1072#1076
    Left = 128
    Top = 72
    object ADO_SkladDSDesigner2: TWideStringField
      FieldName = #1040#1076#1088#1077#1089
      Size = 50
    end
    object ADO_SkladDSDesigner: TIntegerField
      FieldName = #1050#1086#1076
    end
  end
  object ADO_NakToTovar: TADOTable
    Active = True
    Connection = ADOConnection1
    CursorType = ctStatic
    IndexFieldNames = #8470'_'#1085#1072#1082#1083#1072#1076#1085#1086#1081
    MasterFields = #8470'_'#1085#1072#1082#1083#1072#1076#1085#1086#1081
    MasterSource = fmNakl.dsNakl
    TableName = #1053#1072#1082#1083#1072#1076#1085#1072#1103' '#1082' '#1090#1086#1074#1072#1088#1091
    Left = 288
    Top = 72
    object ADO_NakToTovarDSDesigner3: TIntegerField
      FieldName = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086
    end
    object ADO_NakToTovarField: TStringField
      FieldKind = fkLookup
      FieldName = #1058#1086#1074#1072#1088
      LookupDataSet = ADO_Tovari
      LookupKeyFields = #8470'_'#1090#1086#1074#1072#1088#1072
      LookupResultField = #1058#1086#1074#1072#1088'INFO'
      KeyFields = #8470'_'#1090#1086#1074#1072#1088#1072
      Lookup = True
    end
    object ADO_NakToTovar_: TIntegerField
      FieldName = #8470'_'#1085#1072#1082#1083#1072#1076#1085#1086#1081
    end
    object ADO_NakToTovar_2: TIntegerField
      FieldName = #8470'_'#1090#1086#1074#1072#1088#1072
    end
  end
  object ADO_Tovari: TADOTable
    Active = True
    Connection = ADOConnection1
    CursorType = ctStatic
    OnCalcFields = ADO_TovariCalcFields
    OnDeleteError = ADO_TovariDeleteError
    OnPostError = ADO_TovariPostError
    TableName = #1058#1086#1074#1072#1088
    Left = 200
    Top = 72
    object ADO_TovariDSDesigner4: TBlobField
      FieldName = #1060#1086#1090#1086
      Visible = False
    end
    object ADO_TovariDSDesigner5: TMemoField
      FieldName = #1054#1087#1080#1089#1072#1085#1080#1077
      Visible = False
      BlobType = ftMemo
    end
    object ADO_TovariDSDesigner: TWideStringField
      DisplayWidth = 20
      FieldName = #1053#1072#1079#1074#1072#1085#1080#1077
      Size = 100
    end
    object ADO_TovariDSDesigner2: TWideStringField
      DisplayWidth = 20
      FieldName = #1058#1080#1087
      Size = 50
    end
    object ADO_Tovari_2: TIntegerField
      DisplayWidth = 5
      FieldName = #8470'_'#1090#1086#1074#1072#1088#1072
    end
    object ADO_TovariINFO: TStringField
      FieldKind = fkCalculated
      FieldName = #1058#1086#1074#1072#1088'INFO'
      Calculated = True
    end
  end
end
