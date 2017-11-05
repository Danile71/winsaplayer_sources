//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "mmsystem.h"
#define NUM 13
char *types[NUM+1]={".mp1",".mp2",".mp3",".wav",".mid",".midi",".ogg",".aiff",".wma",".flac",".wpl",".m3u"};
#include "Unit1.h"
TForm1 *Form1;
int X=300,Y=200;
AnsiString Pos;
AnsiString glName;
DWORD RestoreOldInstance;
Graphics::TBitmap * BM,*BMM;
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "playlist.cpp"
//#include "plugins.cpp"
TXml *xml;
#include "config.cpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvScrollBar"
#pragma link "Song"
#pragma link "siComp"
#pragma link "siLangRT"
#pragma link "trayicon"
#pragma link "acMagn"
#pragma link "sSkinManager"
#pragma link "WinSkinData"
#pragma link "sSkinManager"
#pragma link "sSkinProvider"
#pragma link "JvExExtCtrls"
#pragma link "JvImage"
#pragma resource "*.dfm"



void __fastcall TForm1::ParseMess(AnsiString str)
{
AnsiString FileName;
int X=0;
int start = str.Pos(".exe");
str=str.Delete(1,start+5);
for(int i=1;i<=str.Length();i++)
{
if(str[i]=='"')X++;
else
if(X==1)FileName=FileName+str[i];
if(X==2){ReadFile(FileName);FileName="";X=1;}

}
}
void __fastcall TForm1::WMCopyData(TMessage& Message)
{
DWORD DataCount;
char *PassedCommandLine;
PCOPYDATASTRUCT PassedCopyDataStruct;
PassedCopyDataStruct = reinterpret_cast<COPYDATASTRUCT*>(Message.LParam);
DataCount = PassedCopyDataStruct->cbData;
if (DataCount > 0)
   {
 PassedCommandLine = new char[DataCount];
    if (PassedCopyDataStruct->lpData != NULL)
        memmove(PassedCommandLine, PassedCopyDataStruct->lpData, DataCount);
        ParseMess(PassedCommandLine);
   }
Win32Check(ReplyMessage(true));
if(PassedCommandLine!=NULL)delete PassedCommandLine;
}

void __fastcall TForm1::WmDropFiles(TWMDropFiles& Message)
{
     	HDROP drop_handle=(HDROP)Message.Drop;
	char fName[MAX_PATH];
	int filenum=DragQueryFile(drop_handle,-1,NULL,NULL);
	for (int i=0;i<filenum;i++)
	{
		DragQueryFile(drop_handle,i,fName,MAX_PATH);
		ReadFile(fName);
	};
	DragFinish(drop_handle);

};

void __fastcall TForm1::ReadFile(AnsiString FileName)
{
	String str=ExtractFileExt(FileName);
        if(FileName.Pos("http://"))
        {
         song->List->Add(FileName);
         ListBox1->Items->Add(FileName);

        }
        else
       for(int i=0;i<NUM;i++)
        {
        if(str==types[i])
        {
         if(types[i]==types[11])
         {
         ListBox1->Clear();
         song->Clear();
         TStringList *pl=new TStringList();
         pl->LoadFromFile(FileName);
         for(int g=0;g<pl->Count;g++)
         {
           ReadFile(pl->Strings[g]);
         }
          delete pl;
         }
         else
         if(types[i]!=types[10])
         {
         song->List->Add(FileName);
         ListBox1->Items->Add(ExtractFileName(FileName).SubString(1,ExtractFileName(FileName).Pos(str)-1));
         }
         else
         {
         ListBox1->Clear();
         song->Clear();
         xml->LoadFromFile(FileName);
         for(int i=0;i<xml->GetPl->Count;i++)
         {
         ReadFile(xml->GetPl->Strings[i]);
         }
         }
        return;
        }
        }
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{}
//--------------------------------------------------------------------------
void __fastcall TForm1::Play()
{
if(ListBox1->Items->Count>0)
{
ImageList1->GetBitmap(0, PlayBtn->Glyph);
if(ListBox1->ItemIndex==-1){ListBox1->ItemIndex++;}
song->FileName=song->List->Strings[ListBox1->ItemIndex];
glName=ListBox1->Items->Strings[ListBox1->ItemIndex];
song->Play();
song->Volume=TrackBar1->Position;
PlayBtn->Tag=1;
Label6->Caption=song->MaxTime;
Label1->Caption=song->Title;
Label2->Caption=song->Album;
Label3->Caption=song->Artist;
Label4->Caption=song->Year;
Label8->Caption=song->Genre;
Timer1->Enabled=true;
}
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
sSkinProvider1->ShowAppIcon=false;
xml=new TXml();
LockWindowUpdate(Form1->Handle);
Canvas->Brush->Color=(TColor)2323232;
Label1->Caption="";
Label2->Caption="";
Label3->Caption="";
Label4->Caption="";
Label8->Caption="";
TCfg *cfg=new TCfg();
BM = new Graphics::TBitmap;
BM->Width=200;
BM->Height=130;
BM->Transparent=true;
BM->TransparentColor=clWhite;
BMM = new Graphics::TBitmap;
BMM->Width=Form1->Width;
BMM->Height=Form1->Height;
BMM->Transparent=true;
BMM->TransparentColor=clWhite;
AnsiString str2=ExtractFilePath(ParamStr(0));
if(FileExists(str2+"\\config.dat"))
{
cfg->FileName=str2+"\\config.dat";
cfg->Load();
Form1->AlphaBlendValue=cfg->LoadAlpha;
if(cfg->LoadLang!=NULL)siLangRT1->Language=cfg->LoadLang;
else siLangRT1->ActiveLanguage=1;
TrackBar1->Position=cfg->LoadVol;
if(ParamStr(1)=="")
{
for(int i=0;i<cfg->LastPl->Count;i++)
{
ReadFile(cfg->LastPl->Strings[i]);
}
Play();
}
}
sSkinManager1->SkinDirectory=ExtractFilePath(ParamStr(0))+"\\skins";
sSkinManager1->SkinName=cfg->LoadSkin;
delete  cfg;
AnsiString str=Label9->Caption+'|';
for(int i=0;i<NUM;i++)
{
str=str+'*'+types[i]+';';
}
OpenDialog1->Filter=str;
DoubleBuffered=true;
DragAcceptFiles(ListBox1->Handle,true);
if(ParamStr(1)!="")
{
for(int i=1;i<=ParamCount();i++)
{
ReadFile(ParamStr(i));
}
}
}

void __fastcall TForm1::OpenDialog1CanClose(TObject *Sender,
      bool &CanClose)
{

for(int i=0;i<OpenDialog1->Files->Count;i++)
{
ReadFile(OpenDialog1->Files->Strings[i]);
}
}
int px=7;
int py=130;
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
if(song->GetPosPercent==500)
{
Timer1->Enabled=false;
if(ListBox1->Items->Count>1&&ListBox1->ItemIndex+1<ListBox1->Items->Count)ListBox1->ItemIndex+=1;
else if(ListBox1->ItemIndex+1==ListBox1->Items->Count)ListBox1->ItemIndex=0;
Play();
}
else
{
ProgressBar1->Position=song->GetPosPercent;
Label5->Caption=song->GetPosition;
if(song->IsActive)
   {
    song->GetStreamMusic();
    PaintBox1->Canvas->CopyRect(TRect(0,0,PaintBox1->Width,PaintBox1->Height),BMM->Canvas,TRect(X,Y,X+PaintBox1->Width,Y+PaintBox1->Height));
    BM->Canvas->CopyRect(TRect(0,0,BM->Width,BM->Height),BMM->Canvas,TRect(X,Y,X+BM->Width,Y+BM->Height));
    for(int x=0;x<27;x++)BM->Canvas->Rectangle(x*px,py,(x+1)*px,py-py*song->Y_Pos[x]);
    PaintBox1->Canvas->Draw(0,0,BM);
   }  //song  avticve
}//else
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox1DblClick(TObject *Sender)
{
Play();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46)DeleteSong(ListBox1->ItemIndex);
if(Key==13)Play();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ProgressBar1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if(song->IsActive&&Button==mbLeft)
{
song->SetPosition=(X+2)*100/ProgressBar1->Width;
ProgressBar1->Refresh();
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ProgressBar1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
if(song->IsActive)
{
Label7->Left=X;
Label7->Top=Y+ProgressBar1->Top+20;
Label7->Visible=true;
Label7->Caption=song->GetMyPos(X,ProgressBar1->Width);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
Label7->Visible=false;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::ListBox1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if(Button==mbRight&&ListBox1->ItemAtPos(TPoint(X, Y), true) != -1)
{
ListBox1->ItemIndex=ListBox1->Items->IndexOf(Pos);
PopupMenu1->Popup(X+Form1->Left,Y+Form1->Top);
}
else
{
PopupMenu2->Popup(X+Form1->Left,Y+Form1->Top);
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
Play();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2Click(TObject *Sender)
{
DeleteSong(ListBox1->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteSong(int Index)
{
if(ListBox1->ItemIndex>-1)
{//if 1
if(ListBox1->ItemIndex==ListBox1->Items->IndexOf(glName))
{//if 2
Timer1->Enabled=false;
song->Stop();
Refresh();
ListBox1->Items->Delete(Index);
song->List->Delete(Index);
//
ProgressBar1->Position=0;
Label5->Caption="00:00:00";
Label6->Caption="00:00:00";
//
if(ListBox1->Items->Count>0)
{
ListBox1->ItemIndex=Index;
Play();
}
} //if 2
else
{
ListBox1->Items->Delete(Index);
song->List->Delete(Index);
}
}//if 1
}
void __fastcall TForm1::TrackBar1PosChange(TObject *Sender)
{
song->Volume=TrackBar1->Position;
}
//---------------------------------------------------------------------------















void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
TCfg *cfg=new TCfg();
AnsiString str=ExtractFilePath(ParamStr(0));
cfg->FileName=str+"\\config.dat";
cfg->LastPl=song->List;
cfg->SaveLang=siLangRT1->Language;
cfg->SaveVol=TrackBar1->Position;
cfg->SaveSkin=sSkinManager1->SkinName;
cfg->SaveAlpha=AlphaBlendValue;
cfg->Save();
delete xml;
delete BM,BMM;
delete  cfg;
}
//---------------------------------------------------------------------------


int mX,mY;


void __fastcall TForm1::ListBox1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
if(ListBox1->ItemAtPos(TPoint(X, Y), true) != -1)
Pos = ListBox1->Items->Strings[ListBox1->ItemAtPos(TPoint(X, Y), true)];
if(Shift.Contains(ssLeft))//если нажата левая кнопка мыши
  {
  if(ListBox1->ItemAtPos(TPoint(X, Y), true) != -1)
      ListBox1->BeginDrag(true,0);//включаем Drag&Drop
      mX=X;
mY=Y;
  }

}
//---------------------------------------------------------------------------






void __fastcall TForm1::ListBox1DragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
Accept = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1EndDrag(TObject *Sender, TObject *Target,
      int X, int Y)
{
if(ListBox1->ItemAtPos(TPoint(X, Y), true) != -1){
ListBox1->Items->Move(ListBox1->ItemAtPos(TPoint(mX, mY), true),ListBox1->ItemAtPos(TPoint(X, Y), true));
song->List->Move(ListBox1->ItemAtPos(TPoint(mX, mY), true),ListBox1->ItemAtPos(TPoint(X, Y), true));
}
}
//---------------------------------------------------------------------------







void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
if(Button==mbRight){MainMenu->Popup(Form1->Left+X,Form1->Top+Y);}        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
Form2->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrayIcon1Minimize(TObject *Sender)
{
Hide();
}
//---------------------------------------------------------------------------










void __fastcall TForm1::siLangRT1ChangeLanguage(TObject *Sender)
{
AnsiString str=Label9->Caption+'|';
for(int i=0;i<NUM;i++)
{
str=str+'*'+types[i]+';';
}
OpenDialog1->Filter=str;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N5Click(TObject *Sender)
{
OpenDialog1->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::url1Click(TObject *Sender)
{
Form3->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N6Click(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N9Click(TObject *Sender)
{
Application->Minimize();
}
//---------------------------------------------------------------------------






void __fastcall TForm1::sSkinManager1AfterChange(TObject *Sender)
{
BMM=Form1->GetFormImage();
BM->Canvas->Pen->Color=clGreen;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::ApplicationEvents1Minimize(TObject *Sender)
{
//Form4->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ApplicationEvents1Message(tagMSG &Msg,
      bool &Handled)
{
 if (Msg.message == (WM_APP+201))
   {
    Handled = true;
   }
}
//---------------------------------------------------------------------------





void __fastcall TForm1::StopBtnClick(TObject *Sender)
{
PlayBtn->Tag=0;
ImageList1->GetBitmap(1, PlayBtn->Glyph);
PlayBtn->Repaint();
ProgressBar1->Position=0;
Label5->Caption="00:00:00";
Label6->Caption="00:00:00";
Label1->Caption="";
Label2->Caption="";
Label3->Caption="";
Label4->Caption="";
Label8->Caption="";
Timer1->Enabled=false;
song->Stop();
Refresh();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddBtnClick(TObject *Sender)
{
OpenDialog1->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
if(ListBox1->Items->Count>1)
{
if(ListBox1->ItemIndex==ListBox1->Items->Count-1)ListBox1->ItemIndex=0;
else  ListBox1->ItemIndex++;
ListBox1->ItemIndex=ListBox1->ItemIndex;
}
Play();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
if(ListBox1->Items->Count>1)
{
 if(ListBox1->ItemIndex==0)ListBox1->ItemIndex=ListBox1->Items->Count-1;
 else ListBox1->ItemIndex--;
}
Play();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::PlayBtnClick(TObject *Sender)
{
PlayBtn->Glyph = NULL;
if(PlayBtn->Tag==0)Play();
else
if(PlayBtn->Tag==1)
{
ImageList1->GetBitmap(0, PlayBtn->Glyph);
song->Play();
PlayBtn->Tag=2;
Timer1->Enabled=true;
}
else
{
Timer1->Enabled=false;
ImageList1->GetBitmap(1, PlayBtn->Glyph);
song->Pause();
PlayBtn->Tag=1;
}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N8Click(TObject *Sender)
{
OpenDialog1->Execute();
}
//---------------------------------------------------------------------------


