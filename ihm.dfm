object FP: TFP
  Left = 338
  Top = 147
  Width = 1305
  Height = 727
  Caption = 'Application Exploitation Sur Site'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = Menu
  OldCreateOrder = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Menu: TMainMenu
    Left = 1224
    object Application1: TMenuItem
      Caption = 'Application'
      object ConnectAutomate: TMenuItem
        Caption = 'Activer Connexion Automate'
        OnClick = ConnectAutomateClick
      end
      object ConnecterBDD: TMenuItem
        Caption = 'Connecter BDD'
        OnClick = ConnecterBDDClick
      end
      object ConnexionMoteur: TMenuItem
        Caption = 'Connecter Moteurs'
        OnClick = ConnexionMoteurClick
      end
      object ChargerFichierConfiguration1: TMenuItem
        Caption = 'Charger Fichier Configuration'
        OnClick = ChargerFichierConfiguration1Click
      end
      object Quitter1: TMenuItem
        Caption = 'Quitter'
        OnClick = Quitter1Click
      end
    end
    object Affichage1: TMenuItem
      Caption = 'Affichage'
      Enabled = False
      object Cascade1: TMenuItem
        Caption = 'Cascade'
        OnClick = Cascade1Click
      end
      object Mosaique1: TMenuItem
        Caption = 'Mosaique'
        OnClick = Mosaique1Click
      end
    end
  end
  object ClientSocketHygrometrie: TClientSocket
    Active = False
    Address = '172.16.10.202'
    ClientType = ctNonBlocking
    Port = 502
    OnConnect = ClientSocketHygrometrieConnect
    OnDisconnect = ClientSocketHygrometrieDisconnect
    OnRead = ClientSocketHygrometrieRead
    OnError = ClientSocketHygrometrieError
    Left = 1192
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 1256
  end
  object ServerSocket: TServerSocket
    Active = False
    Port = 1905
    ServerType = stNonBlocking
    OnClientRead = ServerSocketClientRead
    Left = 1160
  end
end
