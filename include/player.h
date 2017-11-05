class TSong
{
private:
HSAMPLE samp;
HSTREAM chan;
float fft[1024];
bool ret;
void __fastcall GetAbout(AnsiString FileName);
void __fastcall  SetVolume(float Volume);
void __fastcall  GetInfo();
AnsiString GetTime;
bool __fastcall ISActive();
void __fastcall  SetPos(double Pos);
AnsiString __fastcall GetPos();
float __fastcall  GetPercent();
AnsiString __fastcall GetTitle();
AnsiString __fastcall GetAlbum();
AnsiString __fastcall GetArtist();
AnsiString __fastcall GetYear();
AnsiString __fastcall GetGenre();
public:
float Y_Pos[28]; //—юда будет попадать сила спектра канала от 0.0 до 1.0
__fastcall TSong();
void __fastcall Clear();
__property AnsiString FileName={write=GetAbout};
__property AnsiString Title={read=GetTitle};
__property AnsiString Album={read=GetAlbum};
__property AnsiString Artist={read=GetArtist};
__property AnsiString Year={read=GetYear};
__property AnsiString Genre={read=GetGenre};
__property AnsiString MaxTime={read=GetTime};
__property bool IsActive={read=ISActive};
__property double SetPosition={write = SetPos};
__property AnsiString GetPosition={read=GetPos};
__property float Volume={write=SetVolume};
__property float GetPosPercent={read=GetPercent};
TStringList *List;
AnsiString __fastcall GetMyPos(double X,int MAX);
void __fastcall GetStreamMusic(TCanvas *Canvas,int X,int Y);
void __fastcall Play();
void __fastcall Pause();
void __fastcall Stop();

};
