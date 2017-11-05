#include "include/playlist.h"
#define null ""
__fastcall TXml::TXml()
{
List=new TStringList();
PList=new TStringList();
}

AnsiString __fastcall TXml::GetAutor(AnsiString str)
{
AnsiString ret,strt="no name";
if(str.Pos("<title>"))
   {
ret="";
for(int i=str.Pos("<title>")+7;i<str.Pos("</title>");i++)ret=ret+str[i];
return ret;
}
else
return strt;
}
void __fastcall TXml::AddSong(AnsiString str)
{
AnsiString newstr;
if(str.Pos("media src="))
{
newstr=null;
for(int a=str.Pos("media src=")+11;a<str.Pos("/>")-1;a++)
{
newstr=newstr+str[a];
}//for media
if(newstr.Pos("&apos;")) {newstr.Insert("'",newstr.Pos("&apos;"));newstr.Delete(newstr.Pos("&apos;"),6);}
PList->Add(newstr);
}//if src
//PList->Add();
}
void __fastcall TXml::LoadFromFile(AnsiString FileName)
{
AnsiString str;
List->LoadFromFile(FileName);
for(int i=0;i<List->Count;i++)
{
//Autor=GetAutor(List->Strings[i]);
AddSong(List->Strings[i]);
}
}

