object FF: TFF
  Left = 429
  Top = 232
  Width = 687
  Height = 400
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'FF'
  Color = clMenu
  Enabled = False
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object LabelZoneId: TLabel
    Left = 8
    Top = 0
    Width = 150
    Height = 50
    AutoSize = False
    Caption = 'Zone n°'
    Color = clMenu
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Nirmala UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelHygrometrie: TLabel
    Left = 456
    Top = 48
    Width = 73
    Height = 33
    AutoSize = False
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Nirmala UI'
    Font.Style = []
    ParentFont = False
  end
  object LabelVolumeZone: TLabel
    Left = 8
    Top = 96
    Width = 353
    Height = 50
    AutoSize = False
    Caption = 'Volume : '
    Color = clMenu
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Nirmala UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelTauxHygrometrie: TLabel
    Left = 8
    Top = 48
    Width = 150
    Height = 33
    AutoSize = False
    Caption = 'Taux hygrometrie :'
    Color = clMenu
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Nirmala UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelZoneType: TLabel
    Left = 176
    Top = 0
    Width = 273
    Height = 41
    AutoSize = False
    Color = clMenu
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Nirmala UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelMotorisationEtat: TLabel
    Left = 464
    Top = 0
    Width = 143
    Height = 33
    Alignment = taCenter
    AutoSize = False
    Caption = 'VMC : Inactive'
    Color = clRed
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Nirmala UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelDm2: TLabel
    Left = 272
    Top = 96
    Width = 361
    Height = 50
    AutoSize = False
    Caption = 'Debit Volumique d'#39'extraction :'
    Color = clMenu
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Nirmala UI'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object LabelPlageDeFonctionnement: TLabel
    Left = 464
    Top = 152
    Width = 185
    Height = 49
    AutoSize = False
    Caption = 'Horaires de fonctionnement :'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object ProgressBarHygrometrie: TProgressBar
    Left = 168
    Top = 48
    Width = 281
    Height = 33
    Min = 0
    Max = 100
    Smooth = True
    Step = 2
    TabOrder = 0
  end
  object GridPlageDeFonctionnement: TStringGrid
    Left = 0
    Top = 152
    Width = 458
    Height = 55
    ColCount = 7
    FixedCols = 0
    RowCount = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssNone
    TabOrder = 1
  end
end
