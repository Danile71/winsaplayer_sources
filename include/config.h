class TCfg
{
private:
TStringList *List;
AnsiString File;
int Volume;
AnsiString Skin,Lang;
int Alpha;
public:
TStringList *LastPl;
__fastcall TCfg();
__property AnsiString FileName={write=File};
__property TStringList *SaveLast={write=LastPl};
__property int SaveVol={write=Volume};
__property int LoadVol={read=Volume};
__property AnsiString LoadSkin={read=Skin};
__property AnsiString SaveSkin={write=Skin};
__property AnsiString LoadLang={read=Lang};
__property AnsiString SaveLang={write=Lang};
__property int LoadAlpha={read=Alpha};
__property int SaveAlpha={write=Alpha};
void __fastcall Save(),__fastcall Load();
};
