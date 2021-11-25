object GraphicsForm: TGraphicsForm
  Left = 0
  Top = 0
  Caption = 'Graphic'
  ClientHeight = 477
  ClientWidth = 516
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 478
    Height = 451
    Caption = #1043#1088#1072#1092#1080#1082
    TabOrder = 0
    object MChart: TChart
      Left = 2
      Top = 15
      Width = 474
      Height = 226
      AllowPanning = pmNone
      Gradient.EndColor = 13556735
      Gradient.MidColor = 14739177
      Gradient.StartColor = 16774122
      Legend.Alignment = laLeft
      Legend.ColorWidth = 10
      Legend.CurrentPage = False
      Legend.Font.Shadow.Visible = False
      Legend.Frame.Visible = False
      Legend.Inverted = True
      Legend.LegendStyle = lsValues
      Legend.ShapeCallout.Side = csRight
      Legend.ShapeStyle = fosRoundRectangle
      Legend.Symbol.Height = 60
      Legend.Symbol.Shadow.Visible = False
      Legend.Symbol.Width = 10
      Legend.Title.Text.Strings = (
        'ret')
      Legend.Title.Visible = False
      Legend.TopPos = 0
      Legend.Visible = False
      Title.Text.Strings = (
        'General')
      Title.Visible = False
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.LabelsFormat.Font.Color = clGreen
      LeftAxis.LabelsFormat.Font.Style = [fsBold]
      LeftAxis.Maximum = 2000.000000000000000000
      LeftAxis.Minimum = 30.000000000000000000
      LeftAxis.Title.Font.Color = clGreen
      Pages.ScaleLastPage = False
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      RightAxis.LabelsFormat.Font.Color = clRed
      RightAxis.LabelsFormat.Font.Style = [fsBold]
      RightAxis.Maximum = 4095.000000000000000000
      Shadow.Smooth = False
      Shadow.Visible = False
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      TopAxis.Visible = False
      View3D = False
      Zoom.Allow = False
      Align = alTop
      TabOrder = 0
      DefaultCanvas = 'TGDIPlusCanvas'
      PrintMargins = (
        15
        16
        15
        16)
      ColorPaletteIndex = 0
      object Series2: TFastLineSeries
        Selected.Hover.Visible = True
        Marks.Font.Height = -9
        Marks.Font.Name = 'Calibri'
        Marks.Font.Style = [fsBold]
        Marks.Visible = True
        Marks.Arrow.Color = 14561061
        Marks.Arrow.Visible = False
        Marks.BackColor = 5299290
        Marks.Callout.Arrow.Color = 14561061
        Marks.Callout.Arrow.Visible = False
        Marks.Callout.Length = 20
        Marks.Symbol.Frame.Width = 3
        Marks.Symbol.Pen.Width = 3
        Marks.Color = 5299290
        SeriesColor = 27648
        Title = 'Freq'
        ValueFormat = '#,##0.##'
        LinePen.Color = 27648
        LinePen.Width = 2
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
      object Series1: TFastLineSeries
        Marks.Visible = True
        Title = 'Digits'
        ValueFormat = '#,##0'
        VertAxis = aRightAxis
        LinePen.Color = clRed
        LinePen.Width = 2
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Y'
        YValues.Order = loNone
      end
    end
    object Chart1: TChart
      Left = 2
      Top = 241
      Width = 474
      Height = 207
      Legend.Visible = False
      Title.Text.Strings = (
        'TChart')
      Title.Visible = False
      Align = alTop
      TabOrder = 1
      DefaultCanvas = 'TGDIPlusCanvas'
      ColorPaletteIndex = 13
    end
  end
end
