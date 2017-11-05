class TXml
{
private:
TStringList *List;
TStringList *PList;
AnsiString __fastcall GetAutor(AnsiString str);
void __fastcall AddSong(AnsiString str);
AnsiString Autor;
public:
__fastcall TXml();
__property AnsiString autor={read=Autor};
__property TStringList *GetPl={read=PList};
void __fastcall LoadFromFile(AnsiString FileName);
};