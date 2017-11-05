#include "include/config.h"

__fastcall TCfg::TCfg()
{
LastPl=new TStringList();
List=new TStringList();
Alpha=255;
Volume=50;
}

void __fastcall TCfg::Save()
{
if(LastPl->Count>0)
{
List->Add("<last>");
for(int i=0;i<LastPl->Count;i++)
{
List->Add(LastPl->Strings[i]);
}
List->Add("</last>");
}//if pl
List->Add("<volume>"+IntToStr(Volume)+"</volume>");
List->Add("<skin>"+Skin+"</skin>");
List->Add("<alpha>"+IntToStr(Alpha)+"</alpha>");
List->Add("<lang>"+Lang+"</lang>");
List->SaveToFile(File);
}
void __fastcall TCfg::Load()
{
AnsiString str;
List->LoadFromFile(File);
LastPl->Clear();
for(int i=0;i<List->Count;i++)
{
if(List->Strings[i].Pos("<alpha>")&&List->Strings[i].Pos("</alpha>"))
{
str=List->Strings[i].Delete(1,7);
str=str.Delete(str.Pos("</alpha>"),8);
Alpha=StrToInt(str);
}
if(List->Strings[i].Pos("<volume>")&&List->Strings[i].Pos("</volume>"))
{
str=List->Strings[i].Delete(1,8);
str=str.Delete(str.Pos("</volume>"),9);
Volume=StrToInt(str);
}           //if
if(!List->Strings[i].Pos("</last>")&&!List->Strings[i].Pos("<last>"))
{
LastPl->Add(List->Strings[i]);
} //if
if(List->Strings[i].Pos("<skin>")&&List->Strings[i].Pos("</skin>"))
{
str=List->Strings[i].Delete(1,6);
str=str.Delete(str.Pos("</skin>"),7);
Skin=str;
}
if(List->Strings[i].Pos("<lang>")&&List->Strings[i].Pos("</lang>"))
{
str=List->Strings[i].Delete(1,6);
str=str.Delete(str.Pos("</lang>"),7);
Lang=str;
}
}   //for


}
