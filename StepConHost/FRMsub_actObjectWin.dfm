object actObjectWin: TactObjectWin
  Left = 0
  Top = 0
  Width = 95
  Height = 64
  TabOrder = 0
  object Shape1: TShape
    Left = 0
    Top = 0
    Width = 95
    Height = 64
    Align = alClient
    Brush.Color = clGray
    Pen.Color = clLime
    Pen.Width = 3
    Shape = stRoundRect
    OnMouseDown = MainMouseDown
    ExplicitWidth = 105
  end
  object ImgIcon: TImage
    Left = 3
    Top = 3
    Width = 32
    Height = 32
    Transparent = True
    OnClick = ImgIconClick
  end
  object LabelName: TLabel
    Left = 3
    Top = 41
    Width = 27
    Height = 13
    Caption = 'Name'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindow
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object PB_LoadImm: TProgressBar
    Left = 72
    Top = 3
    Width = 17
    Height = 51
    Orientation = pbVertical
    State = pbsError
    TabOrder = 0
  end
end
