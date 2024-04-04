program SuivreX;

{$R *.dres}

uses
  Vcl.Forms,
  WEBLib.Forms,
  UnitMain in 'UnitMain.pas' {FormMain: TWebForm} {*.html};

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TFormMain, FormMain);
  Application.Run;
end.
