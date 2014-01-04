#define _WIN32_WINNT 0x0500

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream> 
#include <iomanip>
#include <conio.h>
#include <conio2.h>
#include <string>
#include <windows.h>
#include <ctime>

using namespace std;

// inisialisasi nama file database.
char* f_fasilitas = "fasilitas.dat";
char* f_tipekamar = "tipekamar.dat";
char* f_kamar = "kamar.dat";
char* f_makanan = "makanan.dat";
char* f_paketmakanan = "paketmakanan.dat";
char* f_pelanggan = "pelanggan.dat";

// define sleep
#define Delay Sleep
#define mydelay 0
#define LSDelay 0

// define view dan layout.
#define MAIN_FRAME_BGCOLOR LIGHTGRAY
#define MAIN_FRAME_X 36
#define MAIN_FRAME_Y 5
#define MAIN_FRAME_WIDTH 95
#define MAIN_FRAME_HEIGHT 35

#define MENU_FRAME_BGCOLOR RED
#define MENU_FRAME_X 1
#define MENU_FRAME_Y 5
#define MENU_FRAME_WIDTH 34
#define MENU_FRAME_HEIGHT 35

// define keyboard.
#define KEY_ENTER 13
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_ESC 27
#define KEY_BSP 8

// define maksimum data.
#define MAX_FASILITAS 20
#define MAX_TIPEKAMAR 10
#define MAX_KAMAR 100
#define MAX_FASILITAS_TIPEKAMAR 10
#define MAX_PELANGGAN 500
#define MAX_PAKETMAKANAN 15
#define MAX_MAKANAN 15

#define MAX_KAMAR_DISEWA 50
#define MAX_PAKETMAKANAN_DIPESAN 50

// define tanda pemisah tanggal.
#define TGL_TANDA '-'

// define status terisi kamar.
#define KMR_TERISI "TERISI"
#define KMR_KOSONG "TERSEDIA"

// define error.
#define FILE_FAULT "[Gagal] Kesalahan file. "
#define FILE_RETRIEVED "[Selesai] "

// define message
#define DATA_ADDED "Data baru ditambahkan. "
#define DATA_CHANGED "Data terubah. "

// buffer yg digunakan untuk fungsi atoi
char buffer[100];


// STRUKTUR DATA LAYOUT ========================================================

struct Layout {
    int w; // width
    int h; // height
    int x;
    int y;
    int edge;
    int fg1;
    int fg2;
    int bg1;
    int bg2;
};

// STRUKTUR DATA TANGGAL =======================================================
struct Tanggal {
    int wd, dd, mm, yy;
    char tanda;
};

// STRUKTUR DATA HOTEL. ========================================================
#define MAX_STRING 16
#define MAX_IDENTITAS 9
#define MAX_NMR_KAMAR 9

struct Fasilitas {
    int id;
    char nama_fasilitas[MAX_STRING];
} FASILITAS[MAX_FASILITAS];
int JML_FASILITAS = 0;

struct TipeKamar {
    int id;
    char nama_tipe[MAX_STRING];
    int fasilitas[MAX_FASILITAS_TIPEKAMAR][2]; // [0]:id fasilitas, [1]:jumlah unit fasilitas.
    int jml_fasilitas;
    long harga;
    float diskon;
    int kelipatan_hari;
} TIPE_KAMAR[MAX_TIPEKAMAR];
int JML_TIPEKAMAR = 0;

struct Kamar {
    int id;
    char nomor_kamar[MAX_NMR_KAMAR];
    int tipe_kamar;
    bool ditempati;
} KAMAR[MAX_KAMAR];
int JML_KAMAR = 0;

struct Makanan {
    int id;
    char nama_makanan[MAX_STRING];
} MAKANAN[MAX_MAKANAN];
int JML_MAKANAN = 0;

struct PaketMakanan {
    int id;
    char nama_paket[MAX_STRING];
    int makanan[MAX_MAKANAN][2];
    int jml_makanan;
    long harga;
} PAKET_MAKANAN[MAX_PAKETMAKANAN];
int JML_PKTMAKANAN = 0;

struct Pelanggan {
    int id;
    char nama[MAX_STRING];
    char no_identitas[MAX_IDENTITAS];
    char alamat[MAX_STRING];
    int lama_inap;
    
    Tanggal tgl_checkin;
    Tanggal tgl_checkout;

    int kamar[MAX_KAMAR_DISEWA];
    int jml_kamar;

    // pelanggan membeli 1 atau banyak paket makanan.
    int paket_makanan[MAX_PAKETMAKANAN_DIPESAN][2];
    int jml_pktmakanan;

} PELANGGAN[MAX_PELANGGAN];
int JML_PELANGGAN = 0;


// prototipe geje.
void SetWindow(int Width, int Height);
void coutxy(string s);
void coutxy(int x, int y, string s);
void logo();

// Protipe fungsi Tanggal.
Tanggal newtgl(int dd, int mm, int yy);
string weekdaytostr(int wd);
Tanggal curtgl();
string tgltostr(Tanggal t);
double tglDiff(Tanggal t1, Tanggal t2);



// VIEW AND LAYOUT =============================================================
void tampilHeader()
{
    int x = 1, y = 1, w = 130, h = MAIN_FRAME_Y-1;
    textbackground(LIGHTGRAY); textcolor(BLACK);
    char c = ' ';
    for (int i = 0; i < h; i++) {
        gotoxy(x, i + y);
        if (i >= h-2) {c = '_';}
        if (i >= h-1) {textbackground(DARKGRAY);}
        cout << setw(w) << setfill(c) << "";
    }
    
    cout << setw(0) << setfill(' ');
    gotoxy(1, 1);
}

void tampilFooter()
{
    int x = 1, y = MENU_FRAME_Y + MENU_FRAME_HEIGHT, w = 129, h = 1;
    textbackground(LIGHTGRAY); textcolor(BLACK);
    char c = ' ';
    for (int i = 0; i < h; i++) {
        gotoxy(x, i + y);
        if (i >= h-2) {c = '_';}
        if (i >= h-1) {textbackground(DARKGRAY);}
        cout << setw(w) << setfill(c) << "";
    }
    
    cout << setw(0) << setfill(' ');
    gotoxy(1, 1);
}

void clearMainFrame() {    
    int x = MAIN_FRAME_X, y = MAIN_FRAME_Y, h = MAIN_FRAME_HEIGHT, w = MAIN_FRAME_WIDTH;
    textbackground(MAIN_FRAME_BGCOLOR); textcolor(MAIN_FRAME_BGCOLOR);
    for (int i = 0; i < h; i++) {
        gotoxy(x, i + y);
        cout << setw(w) << setfill('_') << "";
    }
    
    cout << setw(0) << setfill(' ');
    gotoxy(1, 1);
}

void clearMenuFrame() {
    int x = MENU_FRAME_X, y = MENU_FRAME_Y, h = MENU_FRAME_HEIGHT, w = MENU_FRAME_WIDTH;
    textbackground(MENU_FRAME_BGCOLOR); textcolor(MENU_FRAME_BGCOLOR);
    for (int i = 0; i < h; i++) {
        gotoxy(x, i + y);
        cout << setw(w) << setfill('_') << "";
    }
    
    tampilHeader();
    tampilFooter();
    
    // tampil tanggal sekarang.
    textbackground(BLACK); textcolor(YELLOW);
    gotoxy(x,y+0); cout<<setw(MENU_FRAME_WIDTH)<<setfill(' ')<<left<<"         "+tgltostr(curtgl());
    gotoxy(x,y+1); cout<<setw(MENU_FRAME_WIDTH)<<setfill(' ')<<left<<"";
    
    cout << setw(0) << setfill(' ');
    gotoxy(1, 1);
}

void drawTableLayout(Layout l, string header) {
    int x = l.x, y = l.y;
	
	textbackground(l.edge); textcolor(l.fg2);
    gotoxy(x, y); cout << setw(l.w) << setfill(' ') << "";
    gotoxy(x, y); cout << header;

    if (l.h == 0) {
        textbackground(l.bg1); textcolor(l.bg1);
        gotoxy(x, ++y); cout << setw(l.w) << setfill('_') << "";
    }
    for (int i = 0; i < l.h; i++) {
        if (i % 2 == 0) {
            textbackground(l.bg1);
        } else {
            textbackground(l.bg2);
        }
        textcolor(BLACK);
        gotoxy(x, ++y); cout << setw(l.w-1) << setfill(' ') <<left<< "|" << "|";
    }
    textbackground(l.edge); textcolor(l.edge);
    gotoxy(x, ++y); cout << setw(l.w) << setfill('_') << "";

    gotoxy(1, 1);
}

void drawFormLayout(Layout l, string title) {
    int x = l.x, y = l.y;
    
    textbackground(l.edge); textcolor(l.fg2);
    gotoxy(x, y); cout << setw(l.w) << setfill(' ') << "";
    gotoxy(x, y); cout << "> " << title;
    gotoxy(x, ++y); cout << setw(l.w) << setfill('=') << "";

    textbackground(l.bg1); textcolor(l.fg1);
    for (int i = 0; i < l.h; i++) {
        gotoxy(x, ++y); cout << setw(l.w-1) << setfill(' ') <<left<< "|" << "|";
    }
    textbackground(l.edge); textcolor(l.fg2);
    gotoxy(x, ++y); cout << setw(l.w) << setfill('=') << "";

    gotoxy(1, 1);
}

void drawFormLayoutSimple(Layout l, string title) {
    textbackground(l.edge);
    textcolor(l.fg2);

    int x = l.x, y = l.y;
    gotoxy(x, y);
    cout << setw(l.w) << setfill(' ') << "";
    gotoxy(x, y);
    cout << ("> " + title);

    textbackground(l.bg1); textcolor(l.fg1);
    if (l.h == 0){
        gotoxy(x, ++y); cout <<setw(l.w)<<setfill(' ')<<"";
    }
    for (int i = 0; i < l.h; i++) {
        gotoxy(x, ++y); cout << setw(l.w-1) << setfill(' ') <<left<< "|" << "|";
    }
    textbackground(l.edge); textcolor(l.fg2);
    gotoxy(x, ++y); cout << setw(l.w) << setfill('=') << "";

    gotoxy(1, 1);
}

void clearLine(int x, int y, int w, int color)
{
    textbackground(color);
	gotoxy(x, y); cout << setw(w) << setfill(' ') << "";
    gotoxy(x, y);
}

void clearArea(int x, int y, int w, int h, int color) {
    for (int i = 0; i < h; i++) {
        clearLine(x, y + i, w, color);
    }
}

void loadingSlash(string msg) {
    cout << msg;
    for (int i = 0; i < 3; i++) {
        coutxy("|\b");
        Delay(LSDelay);
        cout << "/\b";
        Delay(LSDelay);
        cout << "-\b";
        Delay(LSDelay);
        cout << "\\\b";
        Delay(LSDelay);
    }
    cout << "\b\n";
}

// OTHER FUNCTION ==============================================================
bool isNumber(char text[]) {
    int len = strlen(text);
    bool valid;

    for (int i = 0; i < len; i++){
        if (i == 0 && (text[0] == '-' || text[0] == '+')) continue;
        if (i+1 < len && text[i] == '.' && isdigit(text[i+1])) continue;
        if (!isdigit(text[i])) {
            textcolor(RED);
            cout << "Tidak valid. ";
            return false;
        }
    }

    return true;
}

bool isNumberPosZ(char text[]) {
    if (!isNumber(text)) return false;
    if (atol(text) < 0) {
        textcolor(RED);
        cout << "Harus >= 0. ";
        return false;
    }

    return true;
}

bool isNumberPos(char text[]) {
    if (!isNumber(text)) return false;
    if (atol(text) <= 0) {
        textcolor(RED);
        cout << "Harus > 0. ";
        return false;
    }

    return true;
}

bool isEmptyText(char text[]) {
    int len = strlen(text);
    for (int i = 0; i < len; i++)
        if (text[i] != ' ') return false;

    textcolor(RED); cout << "Tidak valid. ";
    return true;
}

bool isOverDigit(char text[], int size)
{
    if (strlen(text) < size) return false;
    
    textcolor(RED); cout << "Over digit. ";
    return true;
}

bool isValidForm(string menus[], int size) {
    int score = 0;
    for (int i = 0; i < size; i++) {
        if (menus[i][0] == 'v') score++;
    }

    if (score != size) {
        textcolor(RED);
        cout << "Isi data dengan benar! Coba lagi. ";
        return false;
    }

    return true;
}

bool isEqual(char str1[], char str2[])
{
    return (strcmp(str1,str2)==0);    
}

string numtostr(long num)
{
    char buf[100];
    return itoa(num,buf,10);
}

string strrev(string s) {
    string result=""; 
    for (int i=0; i<s.length( ) ; i++) {
        result = s[ i ] + result;
    } 
    return result;
}

string strfill(string s, int n)
{
    string sfill;
    int len = s.length();
    
    for (int i=0;i<n;i++){
        if (i <= len) sfill += s[i];
        else sfill += " ";
    }
    
    return sfill;
}

string strlow(string s)
{
    string news = "";
    for (int i=0; i<s.length(); ++i)
        news += tolower(s[i]);
    return news;
}

string numtocurr(long num)
{
    string s = strrev(numtostr(num));
    string fulus = "Rp ";
    int nseparator = (s.length()-1)/3;
    for (int i=1;i<=nseparator;i++){
        s.insert(4*i-1,".");
    }
    
    s = strrev(s);
    for (int i=0;i<(11-s.length());i++){
        fulus += " ";
    }
    s = fulus + s;
    
    return s;
}

char pressanykey()
{
    //cout<<"(tekan sembarang...)"; 
    return getch();
}

char confirm(int x, int y, string pesan, int bgcol) {
    string menus[] = {" YA  ", "TIDAK"};

    int peslen = pesan.length();
    int totlen = peslen + 2*menus[0].length();
	
	textbackground(bgcol); textcolor(BLACK);
    coutxy(x, y, string(pesan));

    int pilihan = 1;
    int maxpilihan = 2;
    while (true) {
        for (int i = 0; i < 2; i++) {
            textbackground(bgcol); textcolor(BLACK);
            if (i + 1 == pilihan) {
                textbackground(BLUE); textcolor(WHITE);
            }
            coutxy(x + peslen + i * 5, y, menus[i]);
        }
        char p = getch();
        if (int(p) == KEY_LEFT) {
            pilihan = (pilihan - 1 == 0) ? maxpilihan : pilihan - 1;
        } else if (int(p) == KEY_RIGHT) {
            pilihan = (pilihan + 1 > maxpilihan) ? 1 : pilihan + 1;
        } else if (int(p) == KEY_ENTER) break;
    }

    clearLine(x, y, totlen, bgcol);

    return (pilihan == 1 ? 'y' : 't');
}

// Fungsi Tanggal ===============================================================
Tanggal newtgl(int dd, int mm, int yy)
{    
    tm a = {0,0,0,dd,mm-1,yy-1900};
    time_t mytime = mktime(&a);
    tm *ltm = localtime(&mytime);
    
    Tanggal t = {
        ltm->tm_wday,
        ltm->tm_mday,
        ltm->tm_mon + 1,
        ltm->tm_year + 1900,
        TGL_TANDA};
    
    return t;
}

string weekdaytostr(int wd)
{
    string weekday[7] = {"Minggu","Senin","Selasa","Rabu","Kamis","Jumat","Sabtu"};
    return weekday[wd];
}

Tanggal curtgl()
{    
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    Tanggal t = {
        ltm->tm_wday,
        ltm->tm_mday,
        ltm->tm_mon + 1,
        ltm->tm_year + 1900,
        TGL_TANDA};
    
    return t;
}

string tgltostr(Tanggal t)
{
    string wd = numtostr(t.wd);
    string  d = numtostr(t.dd);
    string  m = numtostr(t.mm);
    string  y = numtostr(t.yy);
    char c[2] = {t.tanda,'\0'};
    string tnd = string(c);
    
    return (weekdaytostr(t.wd)+", "+d+tnd+m+tnd+y);
}

double tglDiff(Tanggal t1, Tanggal t2)
{
    tm a = {0,0,0,t1.dd,t1.mm-1,t1.yy-1900};
    time_t time_a = mktime(&a);    
    
    tm b = {0,0,0,t2.dd,t2.mm-1,t2.yy-1900};
    time_t time_b = mktime(&b);
    
    double diff = difftime(time_a,time_b) / (60.0 * 60.0 * 24.0);
    //tm *local_time = localtime(&sec);
    
    return diff;
}

Tanggal tglnext(Tanggal t, int days)
{
    tm a = {0,0,0,t.dd,t.mm-1,t.yy-1900};
    time_t time_a = mktime(&a);    
    
    time_a += (time_t)(days * 60.0 * 60.0 * 24.0);
    tm *ltm = localtime(&time_a);
    
    Tanggal newt = {
        ltm->tm_wday,
        ltm->tm_mday,
        ltm->tm_mon + 1,
        ltm->tm_year + 1900,
        TGL_TANDA};
    
    return newt;
}


int Menuselector_bg = WHITE;
int Menuselector_fg = BLACK;
int Menuselector_bghover = LIGHTBLUE;
int Menuselector_fghover = WHITE;
void setMenuSelectorColor(int bg, int fg, int bghover, int fghover)
{
    Menuselector_bg = bg;
	Menuselector_fg = fg;
	Menuselector_bghover = bghover;
	Menuselector_fghover = fghover;
}

void setMenuSelectorColor(int bg, int fg)
{
    Menuselector_bg = bg;
	Menuselector_fg = fg;
	Menuselector_bghover = CYAN;
	Menuselector_fghover = WHITE;
}

void menuSelector(string menus[], int *pilihan, int maxpilihan, int spacing, int maxshow) 
{
    if (maxpilihan == 0) return;
    maxshow = maxshow>maxpilihan? maxpilihan : maxshow;

	int x = wherex(), y = wherey();
 	int bg = Menuselector_bg, fg = Menuselector_fg, bghover = Menuselector_bghover, fghover = Menuselector_fghover;
 	
 	int start = 0;
    while (true) {
        int row=0;
        if (*pilihan == 1) start = 0;
        else if (*pilihan == maxpilihan) start = maxpilihan-maxshow;
        else if ( *pilihan > start+maxshow) start++;
        else if ( *pilihan-1 < start ) start--;
        
        for (int i = start; i < maxshow+start; i++) {
            if (i + 1 == *pilihan) {textbackground(bghover); textcolor(fghover);}
            else {textbackground(bg); textcolor(fg);
                if (menus[i][0] == '<' || menus[i][0] == '>') textbackground(LIGHTGRAY);
            }
            
			for (int j = 0; j < spacing; j++) {
                gotoxy(x, y + row * spacing + j);
                cout << setw(menus[i].length()) << setfill(' ') << "";
            }
						
            coutxy(x, y + row*spacing, menus[i]);
            row++;
        }
        
        gotoxy(1, 1);
        char p = getch();
        if (int(p) == KEY_UP) {
            *pilihan = (*pilihan - 1 == 0) ? maxpilihan : *pilihan - 1;
            while (menus[*pilihan - 1].length() == 0 || menus[*pilihan - 1][0] == '@')
                *pilihan = (*pilihan - 1 == 0) ? maxpilihan : *pilihan - 1;
        } else if (int(p) == KEY_DOWN) {
            *pilihan = (*pilihan + 1 > maxpilihan) ? 1 : *pilihan + 1;
            while (menus[*pilihan - 1].length() == 0 || menus[*pilihan - 1][0] == '@')
                *pilihan = (*pilihan + 1 > maxpilihan) ? 1 : *pilihan + 1;
        } else if (int(p) == 71){
            *pilihan = 1;
        } else if (int(p) == 79){
            *pilihan = maxpilihan;
        } else if (int(p) == KEY_ENTER) break;
    }
    
    // reset color.
    textbackground(bg); textcolor(fg);
}

void menuSelector(string menus[], int *pilihan, int maxpilihan, int spacing) 
{
    menuSelector(menus,pilihan,maxpilihan,spacing,maxpilihan);
}

// @FASILITAS ===================================================================

bool addFasilitas(Fasilitas f);
bool loadFasilitasFromFile();
bool saveFasilitasToFile();
void tampilFasilitas();

Fasilitas newFasilitas()
{
    int new_id = JML_FASILITAS == 0 ? 0 : FASILITAS[JML_FASILITAS - 1].id + 1; // last_id+1
	Fasilitas f;
	f.id = new_id;
	strcpy(f.nama_fasilitas,"");
	
	return f;
}

Fasilitas* getFasilitasByNama(string nama_fas) {
    for (int i = 0; i < JML_FASILITAS; i++) {
        if (FASILITAS[i].nama_fasilitas == nama_fas) return &FASILITAS[i];
    }

    return NULL;
}

Fasilitas* getFasilitasById(int id) {
    for (int i = 0; i < JML_FASILITAS; i++) {
        if (FASILITAS[i].id == id) return &FASILITAS[i];
    }

    return NULL;
}

bool addFasilitas(Fasilitas f) {
    // jika array FASILITAS sudah penuh, maka penambahan digagalkan.
    if (JML_FASILITAS == MAX_FASILITAS) {
        textcolor(RED);
        cout << "Gagal. Tabel penuh. ";
        textcolor(WHITE);
        return false;
    }

    // jika sudah ada fasilitas dg id yg sama, maka penambahan digagalkan.
    for (int i = 0; i < JML_FASILITAS; i++) {
        if (f.id == FASILITAS[i].id) {
            textcolor(RED);
            cout << "Gagal. Duplikasi data. zcx";
            textcolor(WHITE);
            return false;
        }
    }

    FASILITAS[JML_FASILITAS] = f;
    
    JML_FASILITAS++;
    
    textcolor(GREEN); cout << DATA_ADDED;
    textcolor(WHITE);
    return true;
}

bool editFasilitas(Fasilitas *f, Fasilitas buf) {
    *f = buf;
    
    textcolor(GREEN); cout << DATA_CHANGED;
    textcolor(WHITE);
    return true;
}

bool deleteFasilitas(int id_fas) {
    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        for (int j = 0; j < TIPE_KAMAR[i].jml_fasilitas; j++) {
            if (TIPE_KAMAR[i].fasilitas[j][0] == id_fas) {
                TipeKamar t = TIPE_KAMAR[i];
                textcolor(RED);
                cout << "Gagal. Data terpakai tipe kamar " << t.nama_tipe << ". ";
                return false;
            }
        }
    }

    bool found = false;
    Fasilitas *prev_fas;
    for (int i = 0; i < JML_FASILITAS; i++) {
        if (found) {
            *prev_fas = FASILITAS[i];
            prev_fas++;
        } else if (FASILITAS[i].id == id_fas) {
            found = true;
            prev_fas = &FASILITAS[i];
        }
    }
    if (found) {
        cout << "Terhapus. ";
        JML_FASILITAS--;
    } else {
        textcolor(RED);
        cout << "Data tidak ditemukan. ";
    }

    return found;
}

bool loadFasilitasFromFile() {
    // read from file.
    ifstream file(f_fasilitas, ios::binary);
    if (!file) {
        cout << FILE_FAULT;
        return false;
    }

    Fasilitas f;
    file.read((char*) &f, sizeof (Fasilitas));
    while (!file.eof()) {
        Delay(mydelay);
        addFasilitas(f);
        cout <<" : "<< f.id << " " << f.nama_fasilitas << endl;
        file.read((char*) &f, sizeof (Fasilitas));
    }

    file.close();

    cout << FILE_RETRIEVED;
    return true;
}

bool saveFasilitasToFile() {
    // write to file.
    ofstream file(f_fasilitas, ios::binary);
    if (!file) {
        textcolor(RED);
        cout << "Gagal. Kesalahan file. ";
        return false;
    }

    Fasilitas f;
    for (int i = 0; i < JML_FASILITAS; i++) {
        f = FASILITAS[i];
        file.write((char*) &f, sizeof (Fasilitas));
    }

    file.close();

    textcolor(GREEN);
    cout << "Tersimpan. ";
    return true;
}

void tampilFormInputFasilitas(int _x, int _y, Fasilitas *f) 
{ //			w  h  x  y  bg  fg0    fg1  bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, 3, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int headersize = 2;
    int padx = 2;
    int pady = headersize + 0;
    
    bool editing = f==NULL ? false : true;

	string menus[] = {
	    "o Nama Fasilitas    :",
        "<< Kembali           "};
    for (int i=0;editing && i<1;i++) menus[i][0] = 'v';
    
    int pilihan = 1;
    int maxpilihan = 2;
    
	drawFormLayout(l, 
        editing ? 
        "FORMULIR PERUBAHAN DATA FASILITAS ID = " + numtostr((*f).id) + ", BERNAMA = " + (*f).nama_fasilitas:
        "FORMULIR PENAMBAHAN DATA FASILITAS");

    bool valid_pilih;
    char simpan, kembali = 't';
    do {
        Fasilitas buf = editing? *f : newFasilitas();
        
        x = padx + l.x;
        y = pady + l.y;

        clearArea(MAIN_FRAME_X, MAIN_FRAME_Y + 5, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - (y + 5), MAIN_FRAME_BGCOLOR);
        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y + 5); tampilFasilitas();
        
        x+=22;
        
        // tampilkan data yg sudah diinput di formulir.
        textbackground(l.bg1); textcolor(l.fg1);
        if(menus[0][0]!='x') coutxy(x,y,buf.nama_fasilitas);
        
        gotoxy(x-22,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        // input user.
        int spc = l.w-(padx+22)-1;
		clearLine(_x,y+2,l.w,l.bg1); // clear message.
		textbackground(l.bg1); textcolor(l.fg1);
        switch(pilihan){
            case 1:
			    clearLine(x, y, spc, l.bg1);
				gotoxy(x, y); cout << setw(sizeof(buf.nama_fasilitas)-1) << setfill(' ') << "" << "|";
                gotoxy(x, y); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x, y, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || isOverDigit(buffer, sizeof(buf.nama_fasilitas))) {
                    menus[0][0] = 'x';
                    continue;
                }
                
                // salin hasil input dari buffer ke rekord.
				strcpy(buf.nama_fasilitas, buffer);
				menus[0][0] = 'v';
				
				// konfirmasi simpan.
				simpan = confirm(x, y, "Simpan '"+string(buf.nama_fasilitas)+"' ? ", l.bg1);
				if (simpan == 'y') {
                    gotoxy(x,y+2);
            	    if (!editing && addFasilitas(buf)) {
                        saveFasilitasToFile();
           		    } else if (editing && editFasilitas(f,buf)){
                        saveFasilitasToFile();
                    }
                    //getch();
       		    }
       		    break;
        	default: // batal?
                kembali = 'y';
                break;
		}
    } while (kembali == 't');
}

void tampilFormDeleteEditFasilitas(int _x, int _y) 
{   //			w  h  x  y  bg  fg0    fg1  bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, 3, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int headersize = 2;
    int padx = 2;
    int pady = headersize + 0;

    string menus[] = {
	    "Ubah  By ID         :",
        "Hapus By ID         :",
        "<< Kembali           "};
    int pilihan = 1;
    int maxpilihan = 3;

    clearMainFrame();
    drawFormLayout(l, "Operasi Data Fasilitas. ");

    // kriteria data yg akan diedit atau dihapus.
    Fasilitas *f_edit;
    int id_fas;
    
    bool valid_pilih;
    char hapus, kembali = 't';
    do {
        x = padx + l.x;
        y = pady + l.y;

        clearArea(MAIN_FRAME_X, MAIN_FRAME_Y + 5, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - (y + 5), MAIN_FRAME_BGCOLOR);
        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y + 5); tampilFasilitas();

        gotoxy(x,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        x+=22;
        int spc = l.w-(padx+22)-1;
        
        // input user.
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1:
                clearLine(x, y, spc, l.bg1);
                gotoxy(x, y); cin.getline(buffer, sizeof (buffer)); fflush(stdin);
                clearLine(x, y, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }

                id_fas = atoi(buffer);
                f_edit = getFasilitasById(id_fas);
                
                if (f_edit==NULL){
                    coutxy(x,y,"Data tidak ditemukan. ");
                    continue;
                }
                
				tampilFormInputFasilitas(_x,_y, f_edit);
				
				clearMainFrame();
                drawFormLayout(l, "Operasi Data Fasilitas. ");
                break;
            case 2:
                clearLine(x, y+1, spc, l.bg1);
                gotoxy(x, y+1); cin.getline(buffer, sizeof (buffer)); fflush(stdin);
                clearLine(x, y+1, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }

                id_fas = atoi(buffer);
                
                // konfirmasi hapus.
                hapus = confirm(x, y+1, "Hapus ID "+numtostr(id_fas)+" ? ", l.bg1);
                if (hapus == 'y') {
                    if (deleteFasilitas(id_fas)) {
                        saveFasilitasToFile();
                    }
                }
                break;
            case 3:
                kembali = 'y';
                break;
        }
    } while (kembali == 't');
}

void tampilFasilitas() 
{
    int _x = wherex(), _y = wherey();
    
    Layout l = {MAIN_FRAME_WIDTH, 38-_y, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x = l.x, y = l.y;

    string header = "| ID    | JENIS FASILITAS        | JML FASILITAS = "+numtostr(JML_FASILITAS)+" UNIT.";
    drawTableLayout(l,	header);

    if (JML_FASILITAS == 0) {
        textbackground(l.bg1); textcolor(l.fg1);
        coutxy(x+2, ++y, 	" tidak ada data. ");
    }
    for (int i = 0; i < JML_FASILITAS; i++) {
        if (y >= (l.y + l.h)) {
            coutxy(x+2, y, "(...more...)");
            break;
        }
        
        if (i % 2 == 0) {textbackground(l.bg1); textcolor(l.fg1);}
        else {textbackground(l.bg2); textcolor(l.fg1);}
        
        coutxy(x + 2, ++y, numtostr(FASILITAS[i].id));
        coutxy(x + 10, y, FASILITAS[i].nama_fasilitas);
    }
}

// @TIPEKAMAR ==================================================================

bool addTipeKamar(TipeKamar f);
bool loadTipeKamarFromFile();
bool saveTipeKamarToFile();
void tampilTipeKamar();

bool addTipeKamar(TipeKamar t) {
    // jika array FASILITAS sudah penuh, maka penambahan digagalkan.
    if (JML_TIPEKAMAR == MAX_TIPEKAMAR) {
        textcolor(RED);
        cout << "Gagal. Tabel penuh. ";
        return false;
    }

    // jika sudah ada fasilitas dg id atau nama yg sama, makan penambahan digagalkan.
    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        if (t.id == TIPE_KAMAR[i].id) {
            textcolor(RED);
            cout << "Gagal. Duplikasi data. ";
            return false; // data tipe kamar baru batal ditambahkan karena sudah ada yg serupa.
        }
    }

    // data tipe kamar baru ditambahkan.
    TIPE_KAMAR[JML_TIPEKAMAR] = t;

    JML_TIPEKAMAR++;
    
    textcolor(GREEN); cout << DATA_ADDED;
    textcolor(WHITE);
    return true;
}

bool editTipeKamar(TipeKamar *t, TipeKamar buf) {
    *t = buf;
    
    textcolor(GREEN); cout << DATA_CHANGED;
    textcolor(WHITE);
    return true;
}

bool deleteTipeKamar(int id_tipe) {
    for (int i = 0; i < JML_PELANGGAN; i++) {
        for (int j = 0; j < PELANGGAN[i].jml_kamar; j++) {
            if (PELANGGAN[i].kamar[j] == id_tipe) {
                cout << "Gagal. Data dibutuhkan di tabel Pelanggan. ";
                return false;
            }
        }
    }
    
    for (int i = 0; i < JML_KAMAR; i++) {
        if (KAMAR[i].tipe_kamar == id_tipe) {
            cout << "Gagal. Data dibutuhkan di tabel Kamar. ";
            return false;
        }
    }

    bool found = false;
    TipeKamar *prev_tipe;
    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        if (found) {
            *prev_tipe = TIPE_KAMAR[i];
            prev_tipe++;
        } else if (TIPE_KAMAR[i].id == id_tipe) {
            found = true;
            prev_tipe = &TIPE_KAMAR[i];
            delete &TIPE_KAMAR[i];
        }
    }
    
    if (found) {
        textcolor(RED);
        cout << "Terhapus. ";
        JML_TIPEKAMAR--;
    } else {
        textcolor(RED);
        cout << "Data tidak ditemukan. ";
    }

    return found;
}

bool loadTipeKamarFromFile() {
    // read from file.
    ifstream file(f_tipekamar, ios::binary);
    if (!file) {
        cout << FILE_FAULT;
        return false;
    }

    TipeKamar t;
    file.read((char*) &t, sizeof (TipeKamar));
    while (!file.eof()) {
        Delay(mydelay);
        addTipeKamar(t);
        cout <<" : " << t.id << " " << t.nama_tipe << " " << t.harga << endl;
        file.read((char*) &t, sizeof (TipeKamar));
    }

    file.close();

    cout << FILE_RETRIEVED;
    return true;
}

bool saveTipeKamarToFile() {
    // write to file.
    ofstream file(f_tipekamar, ios::binary);
    if (!file) {
        textcolor(RED);
        cout << "Gagal tersimpan. Kesalahan file. ";
        return false;
    }

    TipeKamar t;
    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        t = TIPE_KAMAR[i];
        file.write((char*) &t, sizeof (TipeKamar));
    }

    file.close();

    textcolor(GREEN); cout << "Tersimpan. ";
    return true;
}

TipeKamar newTipeKamar() {
    // auto increment id.
    int new_id = JML_TIPEKAMAR == 0 ? 0 : TIPE_KAMAR[JML_TIPEKAMAR - 1].id + 1;

    TipeKamar t;
    t.id = new_id;
    strcpy(t.nama_tipe, "");
    
    t.harga = 0;
    t.diskon = 0.0f;
    t.kelipatan_hari = 0;
    
    t.jml_fasilitas = 0;
    for (int i = 0; i < MAX_FASILITAS_TIPEKAMAR; i++) {
        t.fasilitas[i][0] = -1;
        t.fasilitas[i][1] = -1;
    }

    return t;
}

TipeKamar *getTipeKamarById(int id) {
    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        if (TIPE_KAMAR[i].id == id)
            return &TIPE_KAMAR[i];
    }
    return NULL;
}

void tampilFasilitasTipeKamar(int x, int y, TipeKamar t) 
{   //			w  h               x y  bg  fg0   fg1  bgrow0  bgrow1
    Layout l = {35, t.jml_fasilitas, x, y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    string header = "Fasilitas                | Unit";
    drawFormLayoutSimple(l, header);
	
	textbackground(l.bg1); textcolor(l.fg1);
	if (t.jml_fasilitas == 0){
        gotoxy(x+2, ++y); cout << "tidak ada data.";
    }
    for (int i = 0; i < t.jml_fasilitas; i++) {
        Fasilitas *f = getFasilitasById(t.fasilitas[i][0]);
		gotoxy(x+2, ++y); cout << (*f).nama_fasilitas;
        gotoxy(x+30,  y); cout << t.fasilitas[i][1];
    }
}

void tampilFasilitasSelector(int x, int y, Fasilitas **f) {
    if (JML_FASILITAS == 0) {
        *f = NULL;
        return;
    }

    Layout l = {30, 10, x, y, GREEN, BLACK, WHITE, LIGHTGRAY, WHITE};
    drawFormLayoutSimple(l, "Pilih Fasilitas:");

    string menus[JML_FASILITAS];
    for (int i = 0; i < JML_FASILITAS; i++) {
        menus[i] = strfill(numtostr(FASILITAS[i].id) + " " + string(FASILITAS[i].nama_fasilitas), l.w-4);
    }

    int pilihan = 1;
    int maxpilihan = JML_FASILITAS;

    // tampilkan menu fasilitas selector.
    gotoxy(x+2,y+1);
    setMenuSelectorColor(l.bg1,l.fg1);
	menuSelector(menus, &pilihan, maxpilihan, 1, l.h);
	
    // bersihkan tampilan tabel.
    clearArea(x, y, l.w, l.h + 2, l.bg2);

    *f = &FASILITAS[pilihan - 1];
}

bool deleteFasilitasTipeKamar(TipeKamar *t, int id_fas) {
    bool found = false;
    int *prev_id, *prev_jml;
    for (int i = 0; i < (*t).jml_fasilitas; i++) {
        if (found) {
            *prev_id = (*t).fasilitas[i][0];
            *prev_jml = (*t).fasilitas[i][1];
            
            prev_id = &(*t).fasilitas[i][0];
            prev_jml = &(*t).fasilitas[i][1];
        } else if (!found && (*t).fasilitas[i][0] == id_fas) {
            found = true;
            prev_id = &(*t).fasilitas[i][0];
            prev_jml = &(*t).fasilitas[i][1];
        }
    }
    
    // kurangi 1, jumlah unit fasilitas.
    if (found) (*t).jml_fasilitas--;	

    return found;
}

void addFasilitasTipeKamar(TipeKamar *t, int id_fas, int jml_unit) 
{
    // jika 0. sama aja ngehapus si unit fasilitas tersebut.
	if (jml_unit <= 0) {
        deleteFasilitasTipeKamar(t, id_fas);
        return;
    }

    for (int i = 0; i < (*t).jml_fasilitas; i++) {
        if ((*t).fasilitas[i][0] == id_fas) {
            // jika sudah ada. update jumlah unit.
            (*t).fasilitas[i][1] = jml_unit;
            return;
        }
    }

    // jika belum ada. tambahkan fasilitas baru untuk tipe kamar yg dimaksud.
    (*t).fasilitas[(*t).jml_fasilitas][0] = id_fas;
    (*t).fasilitas[(*t).jml_fasilitas][1] = jml_unit;
    (*t).jml_fasilitas++;
}

string numtoperc(float num)
{
    float perc = num * 100.0f;
    stringstream s;
    s<<perc<<" %";
    return s.str();
}

void tampilFormInputTipeKamar(int _x, int _y, TipeKamar *t) 
{   //			w  h  x  y  bg  fg0    fg1  bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - 3, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int edgesize = 2;
    int padx = 3;
    int pady = edgesize + 1;
    
    bool editing = t==NULL ? false : true;
    
    string menus[] = {
	    "o Nama Tipe         :",
        "o Harga Sewa/Hari   :",
        "o Terapkan diskon   :",
        "o Kelipatan hari    :",
        "o PILIH FASILITAS   :",
        "",
        "<< BATAL             ",
        ">> SELESAI & SIMPAN  "};
    for (int i=0;editing && i<5;i++) menus[i][0] = 'v';
        
    int pilihan = 1;
    int maxpilihan = 8;
    
    clearMainFrame();
    drawFormLayout(l, 
        editing ?
        "FORMULIR PERUBAHAN TIPE KAMAR ID = " + numtostr((*t).id) + ", TIPE = " + (*t).nama_tipe:
        "FORMULIR PENAMBAHAN TIPE KAMAR");

    TipeKamar buf = editing ? *t : newTipeKamar();
    bool valid_pilih;
    char simpan, selesai = 't';
    do {
        x = padx + l.x;
        y = pady + l.y;

        clearArea(x, y+maxpilihan, 35, buf.jml_fasilitas + 3, l.bg1);
        tampilFasilitasTipeKamar(x, y+maxpilihan, buf);
        
        x+=22;    
        
        // tampilkan data formulir.
        textbackground(l.bg1); textcolor(l.fg1);
        if(menus[0][0] == 'v') coutxy(x,y,  buf.nama_tipe);
        if(menus[1][0] == 'v') coutxy(x,y+1,numtocurr(buf.harga));
        if(menus[2][0] == 'v') coutxy(x,y+2,numtoperc(buf.diskon));
        if(menus[3][0] == 'v') coutxy(x,y+3,numtostr(buf.kelipatan_hari));

        gotoxy(x-22,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        // memasukkan nilai oleh user sesuai atribut yg dipilih.	
        int spc = l.w-(padx+22)-1;        
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1: // nama tipekamar
				clearLine(x,y,spc,l.bg1);
				gotoxy(x, y); cout << setw(sizeof(buf.nama_tipe)-1) << setfill(' ') << "" << "|";
                gotoxy(x, y); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y,spc,l.bg1);

                //validasi.
                if (isEmptyText(buffer) || isOverDigit(buffer,sizeof(buf.nama_tipe))) {
                    menus[0][0] = 'x';
                    continue;
                }
            
                strncpy(buf.nama_tipe, buffer, sizeof(buf.nama_tipe));
                
				menus[0][0] = 'v';
                break;
            case 2: // harga
                clearLine(x,y+1,spc,l.bg1);
                gotoxy(x,y+1); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+1,spc,l.bg1);

                //validasi.
                if (!isNumberPos(buffer)) {
                    menus[1][0] = 'x';
                    continue;
                }
            
                buf.harga = atol(buffer);
                
                menus[1][0] = 'v';
                break;
            case 3: // diskon
                clearLine(x,y+2,spc,l.bg1);
                gotoxy(x,y+2); cout<<"%";
                gotoxy(x,y+2); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+2,spc,l.bg1);

                //validasi.
                if (!isNumberPosZ(buffer)) {
                    menus[2][0] = 'x';
                    continue;
                }
            
                buf.diskon = atof(buffer)/100.0f;
                
                menus[2][0] = 'v';
                break;
            case 4: // ketentuan hari utk diskon.
                clearLine(x,y+3,spc,l.bg1);
                gotoxy(x,y+3); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+3,spc,l.bg1);

                //validasi.
                if (!isNumberPosZ(buffer)) {
                    menus[3][0] = 'x';
                    continue;
                }
            
                buf.kelipatan_hari = atol(buffer);
                
                menus[3][0] = 'v';
                break;
            case 5: // fasilitas
                Fasilitas *f;
                
                clearLine(x,y+4,spc,l.bg1);
				tampilFasilitasSelector(x, y+4, &f);
				clearLine(x,y+4,spc,l.bg1);
                
                if (f == NULL) {
                    coutxy(x, y + 4, "Fasilitas kosong. ");
                    continue;
                }

                clearLine(x,y+4,spc,l.bg1);
				gotoxy(x,y+4); cout << (*f).nama_fasilitas;
                cout << ", sebanyak = "; cin.getline(buffer, sizeof (buffer)); fflush(stdin);
                clearLine(x,y+4,spc,l.bg1);
                
                //validasi.
                if (isEmptyText(buffer) || !isNumberPosZ(buffer)) {
                    clearLine(x,y+4,spc,l.bg1);
                    continue;
                }
					
				// tambahkan data fasilitas baru.
				addFasilitasTipeKamar(&buf, (*f).id, atoi(buffer));
				
				if (buf.jml_fasilitas == 0) menus[4][0] = 'x';
				else menus[4][0] = 'v';
				
                break;
            case 7:
                selesai = confirm(x, y+6, " ? ", l.bg1);
                break;
            case 8:
				clearLine(x,y+7,spc,l.bg1);
                simpan = confirm(x, y+7, " ? ", l.bg1);
                if (simpan == 'y' && isValidForm(menus, 3)) {
        			if (!editing && addTipeKamar(buf) && saveTipeKamarToFile()) {
            		    selesai = 'y';
            		    pressanykey();
       				} else if (editing && editTipeKamar(t,buf) && saveTipeKamarToFile()) {
            		    selesai = 'y';
            		    pressanykey();
       				}
                }
                break;
        }
    } while (selesai == 't');
}

void tampilFormDeleteEditTipeKamar(int _x, int _y) 
{   //			w  h  x  y  bg  fg0    fg1  bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, 3, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int edgesize = 2;
    int padx = 2;
    int pady = edgesize + 0;

    string menus[] = {
	    "Edit  By ID         :",
        "Hapus By ID         :",
        "<< KEMBALI           "};
    int pilihan = 1;
    int maxpilihan = 3;
    
    clearMainFrame();
    drawFormLayout(l, "Operasi Data Tipe Kamar. ");
    
    // kriteria data yg akan diedit atau dihapus.
    TipeKamar *t;
    int id_tpkmr;
    
    bool valid_pilih;
    char hapus, kembali = 't';
    do {
        x = padx + l.x;
        y = pady + l.y;

        clearArea(MAIN_FRAME_X, MAIN_FRAME_Y + 5, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - (y + 5), MAIN_FRAME_BGCOLOR);
        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y + 5); tampilTipeKamar();

        gotoxy(x,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        x+=22;
        int spc = l.w-(padx+22)-1;
        
        // field inputan user.        
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1:
                clearLine(x,y,spc,l.bg1);
                gotoxy(x, y); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y,spc,l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }

                id_tpkmr = atoi(buffer);
                t = getTipeKamarById(id_tpkmr);
                
                if (t==NULL){
                    coutxy(x,y,"Data tidak ditemukan. ");
                    continue;
                }
                
				tampilFormInputTipeKamar(_x,_y, t);
				
				clearMainFrame();
				drawFormLayout(l, "Operasi Data Tipe Kamar. ");
                break;
            case 2:                
				clearLine(x,y+1,spc,l.bg1);
                gotoxy(x, y+1); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+1,spc,l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }

                id_tpkmr = atoi(buffer);
                
                // konfirmasi hapus.
                hapus = confirm(x, y+1, "Hapus "+string(buffer)+" ? ", l.bg1);
                if (hapus == 'y') {
                    if (deleteTipeKamar(id_tpkmr)) {
                        saveTipeKamarToFile();
                    }
                }
                break;
            default:
                kembali = 'y';
                break;
        }
    } while (kembali == 't');
}

int getTipeKamarCount(int id_tipe)
{
 	int count = 0;
    for(int i=0;i<JML_TIPEKAMAR;i++){
	    if (TIPE_KAMAR[i].id == id_tipe) count++;
    } 	
    return count;
}

void tampilTipeKamar() 
{
    Layout l = {MAIN_FRAME_WIDTH, 38-wherey(), wherex(), wherey(), RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x = l.x, y = l.y;
    
    string header = "| ID   | TIPE KAMAR       | DISKON | SYRT HARI | HARGA/HARI      | FASILITAS        | JML UNIT  ";
    drawTableLayout(l, header);
    
    if (JML_TIPEKAMAR == 0) {
        textbackground(l.bg1); textcolor(l.fg1);
        coutxy(x+2, y + 1, "                                 tidak ada data                                   ");
    }

    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        l.h = TIPE_KAMAR[i].jml_fasilitas;
        l.x = x; l.y = y;
        l.bg2 = WHITE;
        
        drawTableLayout(l, header);
        if (i>0) clearLine(x+1,y,l.w-2,l.edge);

        if (i % 2 == 0) {
            textbackground(l.bg1); textcolor(l.fg1);
        } else {
            textbackground(l.bg1); textcolor(l.fg1);
        }

        gotoxy(x + 2, ++y); cout<<TIPE_KAMAR[i].id;
        gotoxy(x + 9, 	y); cout<<TIPE_KAMAR[i].nama_tipe;
        gotoxy(x + 30, 	y); cout<<numtoperc(TIPE_KAMAR[i].diskon);
        gotoxy(x + 40, 	y); cout<<TIPE_KAMAR[i].kelipatan_hari<<"";
        gotoxy(x + 49, 	y);	cout<<numtocurr(TIPE_KAMAR[i].harga);
        for (int j = 0; j < TIPE_KAMAR[i].jml_fasilitas; j++) {
            Fasilitas *f = getFasilitasById(TIPE_KAMAR[i].fasilitas[j][0]);
            
            gotoxy(x + 67, y); 		cout<<(*f).nama_fasilitas;
            gotoxy(x + 86, y++); 		cout<<TIPE_KAMAR[i].fasilitas[j][1];
        }
    }

    gotoxy(1,1);
}

// @KAMAR ======================================================================

void tampilKamar();

bool addKamar(Kamar t) {
    // jika array KAMAR sudah penuh, maka penambahan digagalkan.
    if (JML_KAMAR == MAX_KAMAR) {
        textcolor(RED);
        cout << "Gagal. Tabel penuh. ";
        return false;
    }

    // jika sudah ada kamar dg id atau nama yg sama, makan penambahan digagalkan.
    for (int i = 0; i < JML_KAMAR; i++) {
        if (t.id == KAMAR[i].id || isEqual(strupr(t.nomor_kamar),strupr(KAMAR[i].nomor_kamar))) {
            textcolor(RED);
            cout << "Gagal. Duplikasi data. ";
            return false; // data kamar baru batal ditambahkan karena sudah ada yg serupa.
        }
    }

    // data kamar baru ditambahkan.
    KAMAR[JML_KAMAR] = t;

    JML_KAMAR++;
    
    textcolor(GREEN); cout << DATA_ADDED;
    textcolor(WHITE);
    return true;
}

bool editKamar(Kamar *k, Kamar buf) {
    *k = buf;
    
    textcolor(GREEN); cout << DATA_CHANGED;
    textcolor(WHITE);
    return true;
}

bool deleteKamar(int id_tipe) 
{
 	for (int i=0;i<JML_PELANGGAN;i++){
	    for (int j=0;j<PELANGGAN[i].jml_kamar;j++){
		    if (PELANGGAN[i].kamar[j] == id_tipe){
				cout<<"Gagal. Data dibutuhkan di tabel pelanggan. ";
				return false;
			}	
		}
	}

    bool found = false;
    Kamar *prev_tipe;
    for (int i = 0; i < JML_KAMAR; i++) {
        if (found) {
            *prev_tipe = KAMAR[i];
            *prev_tipe++;
        } else if (KAMAR[i].id == id_tipe) {
            found = true;
            prev_tipe = &KAMAR[i];
            delete &KAMAR[i];
        }
    }
    if (found) {
        textcolor(GREEN);
        cout << "Terhapus. ";
        JML_KAMAR--;
    } else {
        textcolor(RED);
        cout << "Data tidak ditemukan. ";
    }

    return found;
}

bool loadKamarFromFile() {
    // read from file.
    ifstream file(f_kamar, ios::binary);
    if (!file) {
        cout << FILE_FAULT;
        return false;
    }

    Kamar t;
    file.read((char*) &t, sizeof (Kamar));
    while (!file.eof()) {
        Delay(mydelay);
        addKamar(t);
        cout <<" : " << t.id << " " << t.nomor_kamar << endl;
        file.read((char*) &t, sizeof (Kamar));
    }

    file.close();
    cout << FILE_RETRIEVED;
    return true;
}

bool saveKamarToFile() {
    // write to file.
    ofstream file(f_kamar, ios::binary);
    if (!file) {
        textcolor(RED);
        cout << "Gagal tersimpan. Kesalahan file. ";
        return false;
    }

    Kamar t;
    for (int i = 0; i < JML_KAMAR; i++) {
        t = KAMAR[i];
        file.write((char*) &t, sizeof (Kamar));
    }

    file.close();

    textcolor(GREEN); cout << "Tersimpan. ";
    return true;
}

Kamar newKamar() {
    // auto increment id.
    int new_id = JML_KAMAR == 0 ? 0 : KAMAR[JML_KAMAR - 1].id + 1;

    Kamar t;
    t.id = new_id;
    strcpy(t.nomor_kamar, "");
    t.ditempati = false;
    t.tipe_kamar = -1;

    return t;
}

Kamar *getKamarById(int id) {
    for (int i = 0; i < JML_KAMAR; i++) {
        if (KAMAR[i].id == id)
            return &KAMAR[i];
    }
    return NULL;
}

void tampilTipeKamarSelector(TipeKamar **t) 
{
    int x = wherex(), y = wherey();
    if (JML_TIPEKAMAR == 0) {
        *t = NULL;
        return;
    }
    
    //			w    h            x  y  bg  fg0   fg1  bgrow0  bgrow1
    Layout l = {30, JML_TIPEKAMAR, x, y, GREEN, BLACK, WHITE, LIGHTGRAY, WHITE};
    drawFormLayoutSimple(l, "Pilih Tipe Kamar:");

    string menus[JML_TIPEKAMAR];
    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        menus[i] = strfill("| " + numtostr(TIPE_KAMAR[i].id) + "  " + string(TIPE_KAMAR[i].nama_tipe), l.w);
    }

    int pilihan = 1;
    int maxpilihan = JML_TIPEKAMAR;

    gotoxy(x,y+1);
    setMenuSelectorColor(l.bg1,l.fg1);
	menuSelector(menus, &pilihan, maxpilihan, 1);

    // bersihkan tabel.
    clearArea(x, y, l.w, maxpilihan + 2, l.bg2);

    *t = &TIPE_KAMAR[pilihan - 1];
}

void tampilFormInputKamar(int _x, int _y, Kamar *k) 
{   //			w  h  x  y  bg  fg0    fg1  bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - 3, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int edgesize = 2;
    int padx = 3;
    int pady = edgesize + 0;
    
    bool editing = k==NULL? false : true;

    string menus[] = {
		"o Nomor Kamar      :",
        "o PILIH TIPE KAMAR :",
        "<< BATAL            ",
        ">> SELESAI & SIMPAN "};
    for (int i=0;editing && i<2;i++) menus[i][0] = 'v';
    
    int pilihan = 1;
    int maxpilihan = 4;

    clearMainFrame();
    drawFormLayout(l, 
        editing?
        "FORMULIR PENGUBAHAN DATA KAMAR ID = " + numtostr((*k).id) + ", TIPE = " + (*k).nomor_kamar:
        "FORMULIR PENAMBAHAN DATA KAMAR");

    Kamar buf = editing? *k : newKamar();
    buf.ditempati = false;
    bool valid_pilih;
    char simpan = 't', selesai = 't';
    do {
        x = padx + l.x;
        y = pady + l.y;
        
        clearArea(MAIN_FRAME_X, MAIN_FRAME_Y + 6, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - (y + 6), MAIN_FRAME_BGCOLOR);
        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y + 6); tampilKamar();
        
        x+=22;

        // tampil data formulir.
        textbackground(l.bg1); textcolor(l.fg1);
        if(menus[0][0]=='v') coutxy(x,y,  strupr(buf.nomor_kamar));
        if(menus[1][0]=='v') coutxy(x,y+1,(*getTipeKamarById(buf.tipe_kamar)).nama_tipe);
        
        gotoxy(x-22,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        int spc = l.w-(padx+22)-1;

        // inputan user.
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1: // nomor kamar
				clearLine(x, y, spc, l.bg1);
				gotoxy(x, y); cout << setw(sizeof(buf.nomor_kamar)-1) << setfill(' ') << "" << "|";
                gotoxy(x, y); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x, y, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || isOverDigit(buffer, sizeof(buf.nomor_kamar))) {
                    menus[0][0] = 'x';
                    continue;
                }

                strncpy(buf.nomor_kamar, buffer, sizeof(buf.nomor_kamar));
                
				menus[0][0] = 'v';
                break;
            case 2: // tipe kamar
                TipeKamar *t;
                
                clearLine(x, y+1, spc, l.bg1);
                gotoxy(x, y+1); tampilTipeKamarSelector(&t);
                clearLine(x, y+1, spc, l.bg1);

                if (t == NULL) {
                    coutxy(x, y+1, "Tidak ada data. ");
                    menus[1][0] = 'x';
                    continue;
                }

                buf.tipe_kamar = (*t).id;
                
                menus[1][0] = 'v';
                break;
            case 3:
				selesai = confirm(x, y+2, " ? ", l.bg1);
                break;
            case 4:
                clearLine(x, y+3, spc, l.bg1);
				simpan = confirm(x, y+3, " ? ", l.bg1);
                if (simpan == 'y' && isValidForm(menus, 2)) {
					if (!editing && addKamar(buf) && saveKamarToFile()) {
            		    selesai = 'y';
        			} else if (editing && editKamar(k,buf) && saveTipeKamarToFile()) {
            		    selesai = 'y';
       				}
       				pressanykey();
                }
                break;
        }
    } while (selesai == 't');
}

void tampilFormDeleteEditKamar(int _x, int _y) 
{   //			w  h  x  y  bg  fg0    fg1  bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, 3, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int edgesize = 2;
    int padx = 2;
    int pady = edgesize + 0;

    string menus[] = {
		"Edit  By ID         :",
        "Hapus By ID         :",
        "<< KEMBALI           "};
    int pilihan = 1;
    int maxpilihan = 3;
    
    clearMainFrame();
    drawFormLayout(l, "Edit data kamar.");
        
    // kriteria data yg akan diedit atau dihapus.
    Kamar *k;
    int id_kmr;

    bool valid_pilih;
    char hapus = 't', kembali = 't';
    do {        
        x = padx + l.x;
        y = pady + l.y;

        clearArea(MAIN_FRAME_X, MAIN_FRAME_Y + 5, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - (y + 5), MAIN_FRAME_BGCOLOR);
        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y + 5); tampilKamar();
        
        gotoxy(x,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        x+=22;
        int spc = l.w-(padx+22)-1;
        
        // memasukkan nilai oleh user.        
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1:
                clearLine(x, y, spc, l.bg1);
                gotoxy(x, y); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x, y, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }
                
                id_kmr = atoi(buffer);
                k = getKamarById(id_kmr);
                
                if (k==NULL){
                    coutxy(x,y,"Data tidak ditemukan. ");
                    continue;
                }
                
				tampilFormInputKamar(_x,_y, k);
				
				clearMainFrame();
                drawFormLayout(l, "Edit data kamar.");
                break;
            case 2:
                clearLine(x,y+1, spc, l.bg1);
                gotoxy(x,y+1); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+1, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }
                
                id_kmr = atoi(buffer);
				hapus = confirm(x,y+1, "Hapus ID " + string(buffer) + " ? ", l.bg1);
                if (hapus == 'y') {
                    if (deleteKamar(id_kmr)) {
                        saveKamarToFile();
                    }
                }
                break;
            case 3: // keluar?
				kembali = 'y';
                break;
        }
    } while (kembali == 't');
}

void tampilKamar() 
{   
    int _x = wherex(), _y = wherey();
    
    Layout l = {MAIN_FRAME_WIDTH, 38-_y, _x, _y, RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    string header = "| ID   | NMR KAMAR | TIPE_KAMAR         | STATUS    ";
    
    int x = _x, y = _y;
    
	drawTableLayout(l, header);
        
	if (JML_KAMAR == 0) {
        textbackground(l.bg1); textcolor(l.fg1);
      	coutxy(x, ++y, 	"                tidak ada data                       ");
	}	
    for (int i = 0; i < JML_KAMAR; i++) {
        if (y >= (l.y + l.h)) {
            coutxy(x+2, y, "(...more...)");
            break;
        }
        
        if (i % 2 == 0) {
            textbackground(l.bg1); textcolor(l.fg1);
        } else {
            textbackground(l.bg2); textcolor(l.fg1);
        }

        TipeKamar *t = getTipeKamarById(KAMAR[i].tipe_kamar);
        coutxy(x + 2, ++y, numtostr(KAMAR[i].id));
        coutxy(x + 9, y, KAMAR[i].nomor_kamar);
        coutxy(x + 21, y, (t!=NULL?(*t).nama_tipe : "n/a"));
        coutxy(x + 42, y, KAMAR[i].ditempati ? KMR_TERISI : KMR_KOSONG);
    }
}

// @PAKETMAKANAN ===============================================================
PaketMakanan *getPaketMakananById(int id) {
    for (int i = 0; i < JML_PKTMAKANAN; i++) {
        if (PAKET_MAKANAN[i].id == id)
            return &PAKET_MAKANAN[i];
    }
    return NULL;
}

// @PELANGGAN dan @PEMESANAN ===================================================

void tampilPelanggan();
void tampilPelanggan(string kid, string knama, string knoid, 
                     string kalmt, string knokmr, string ktpkmr, string ktglin, 
                     string ktglout);

Pelanggan newPelanggan() {
    // auto increment id.
    int new_id = JML_PELANGGAN == 0 ? 0 : PELANGGAN[JML_PELANGGAN - 1].id + 1; // last_id+1

    Pelanggan p;

    p.id = new_id;
    strcpy(p.nama, "");
    strcpy(p.no_identitas, "");
    strcpy(p.alamat, "");

    p.lama_inap = 0;
    
    p.tgl_checkin = curtgl();
    p.tgl_checkout = curtgl();

    p.jml_kamar = 0;
    for (int i = 0; i < MAX_KAMAR_DISEWA; i++) {
        p.kamar[i] = -1;
    }

    p.jml_pktmakanan = 0;
    for (int i = 0; i < MAX_PAKETMAKANAN_DIPESAN; i++) {
        p.paket_makanan[i][0] = -1;
        p.paket_makanan[i][1] = -1;
    }

    return p;
}

bool addPelanggan(Pelanggan p) {
    // jika array pelanggan sudah penuh, maka penambahan digagalkan.
    if (JML_PELANGGAN == MAX_PELANGGAN) {
        textcolor(RED);
        cout << "Gagal. Tabel penuh. ";
        return false;
    }

    // jika sudah ada pelanggan dg id atau nama yg sama, maka penambahan digagalkan.
    for (int i = 0; i < JML_PELANGGAN; i++) {
        if (p.id == PELANGGAN[i].id) {
            textcolor(RED);
            cout << "Gagal. Duplikasi data. ";
            return false; // data pelanggan baru batal ditambahkan karena sudah ada.
        }
    }

    // data kamar baru ditambahkan.
    PELANGGAN[JML_PELANGGAN] = p;

    JML_PELANGGAN++;
    
    textcolor(GREEN); cout << DATA_ADDED;
    textcolor(WHITE);
    return true;
}

bool deletePelanggan(int id) 
{
    bool found = false;
    Pelanggan *prev_plg;
    for (int i = 0; i < JML_PELANGGAN; i++) {
        if (found) {
            *prev_plg = PELANGGAN[i];
            *prev_plg++;
        } else if (PELANGGAN[i].id == id) {
            found = true;
            prev_plg = &PELANGGAN[i];
            delete &PELANGGAN[i];
        }
    }
    if (found) {
        textcolor(RED);
        cout << "Terhapus. ";
        JML_PELANGGAN--;
    } else {
        textcolor(RED);
        cout << "Data tidak ditemukan. ";
    }

    return found;
}

bool editPelanggan(Pelanggan *p, Pelanggan buf) {
    *p = buf;
    
    textcolor(GREEN); cout << DATA_CHANGED;
    textcolor(WHITE);
    return true;
}

Pelanggan *getPelangganById(int id) {
    for (int i = 0; i < JML_PELANGGAN; i++) {
        if (PELANGGAN[i].id == id)
            return &PELANGGAN[i];
    }
    return NULL;
}

bool isCheckedOut(Pelanggan p)
{
    int diff = (int)tglDiff(p.tgl_checkout,p.tgl_checkin);
    if (diff == 0) return false;
    
    return true;
}

void setKamarDitempati(Kamar *k)
{
    (*k).ditempati = true;
}

void setKamarDitinggalkan(Kamar *k)
{
    (*k).ditempati = false;
}

void tempatiKamar(Pelanggan p)
{
    for (int i=0;i<p.jml_kamar;i++){
        Kamar *k = getKamarById(p.kamar[i]);
        setKamarDitempati(k);
    }
    saveKamarToFile();
}

void keluarKamar(Pelanggan p)
{    
    for (int i=0;i<p.jml_kamar;i++){
        Kamar *k = getKamarById(p.kamar[i]);
        setKamarDitinggalkan(k);
    }
    
    cout<<"Pelanggan berhasil checkout. ";
    saveKamarToFile();
}

bool loadPelangganFromFile() {
    // read from file.
    ifstream file(f_pelanggan, ios::binary);
    if (!file) {
        cout << FILE_FAULT;
        return false;
    }

    Pelanggan p;
    file.read((char*) &p, sizeof (Pelanggan));
    while (!file.eof()) {
        Delay(mydelay);
        addPelanggan(p);
        cout <<" : " << p.id << " " << p.nama << " " << endl;
        file.read((char*) &p, sizeof (Pelanggan));
    }

    file.close();
    cout << FILE_RETRIEVED;
    return true;
}

bool savePelangganToFile() {
    // write to file.
    ofstream file(f_pelanggan, ios::binary);
    if (!file) {
        textcolor(RED);
        cout << "Gagal tersimpan. Kesalahan file. ";
        return false;
    }

    Pelanggan p;
    for (int i = 0; i < JML_PELANGGAN; i++) {
        p = PELANGGAN[i];
        file.write((char*) &p, sizeof (Pelanggan));
    }

    file.close();

    textcolor(GREEN); cout << "Tersimpan. ";
    return true;
}

void tampilPesananKamar(Pelanggan p) 
{   //			w  h               x y  bg  fg0   fg1  bgrow0  bgrow1
    Layout l = {35, p.jml_kamar, wherex(), wherey(), BLUE, BLACK, WHITE, LIGHTGRAY, LIGHTGRAY};
    int x = l.x, y=l.y;
    
    string header = "Kamar Yang Dipesan   | Nmr Kamar ";
    drawFormLayoutSimple(l, header);
	
	textbackground(l.bg1); textcolor(l.fg1);
	if (p.jml_kamar == 0){
        gotoxy(x, ++y); cout <<setw(l.w)<<setfill(' ')<<left<<" tidak ada data.";
    }
    for (int i = 0; i < p.jml_kamar; i++) {
        Kamar k = *getKamarById(p.kamar[i]);
        TipeKamar t = *getTipeKamarById(k.tipe_kamar);
        
		gotoxy(x+2, ++y); cout << t.nama_tipe;
        gotoxy(x+25,  y); cout << k.nomor_kamar;
    }
}

bool deletePesananKamar(Pelanggan *p, int id_kmr) {
    bool found = false;
    int *prev_id;
    for (int i = 0; i < (*p).jml_kamar; i++) {
        if (found) {
            *prev_id = (*p).kamar[i];            
            prev_id = &(*p).kamar[i];
        } else if (!found && (*p).kamar[i] == id_kmr) {
            found = true;
            prev_id = &(*p).kamar[i];
        }
    }
    
    // kurangi 1, jumlah unit fasilitas.
    if (found) (*p).jml_kamar--;

    return found;
}

void addPesananKamar(Pelanggan *p, int id_kmr) 
{
    for (int i = 0; i < (*p).jml_kamar; i++) {
        if ((*p).kamar[i] == id_kmr) {
            // jika sudah ada. biarkan.
            cout<<"Kamar tersebut sudah dipilih. ";
            return;
        }
    }

    // jika belum ada. tambahkan kamar baru.
    (*p).kamar[(*p).jml_kamar] = id_kmr;
    (*p).jml_kamar++;
}

Kamar *getKamarTersedia(int id_tpkmr)
{
    for (int i=0;i<JML_KAMAR;i++){
        if(id_tpkmr == KAMAR[i].tipe_kamar && !KAMAR[i].ditempati ){
            return &KAMAR[i];
        }
    }
    return NULL;
}

Kamar *getKamarTersedia(char* nmr_kmr)
{
    for (int i=0;i<JML_KAMAR;i++){
        if(isEqual(KAMAR[i].nomor_kamar,nmr_kmr) && !KAMAR[i].ditempati){
            return &KAMAR[i];
        }
    }
    return NULL;
}

int getTipeKamarTersediaCount(int id_tpkmr)
{
    int jml = 0;
    for (int i=0;i<JML_KAMAR;i++){
        if(id_tpkmr == KAMAR[i].tipe_kamar && !KAMAR[i].ditempati ){
            jml++;    
        }
    }
    return jml;
}

int getTipeKamarDipesanCount(Pelanggan p, int id_tpkmr)
{
    int jml = 0;
    for (int i=0;i<p.jml_kamar;i++){
        Kamar k = *getKamarById(p.kamar[i]);
        if (k.tipe_kamar == id_tpkmr) jml++;
    }
    return jml;
}

bool isTipeKamarTersedia(TipeKamar t, int jml_psn)
{
    int jml = getTipeKamarTersediaCount(t.id);
    
    if (jml < jml_psn){
        textcolor(RED); cout<<"Unit tidak mencukupi. ";
        return false;
    }
    
    return true;
}

void tampilKamarTersediaSelector(TipeKamar **t, int *jml_unit) 
{
    if (JML_TIPEKAMAR == 0) {
        *t == NULL;
        return;
    }
    
    Layout l = {30, JML_TIPEKAMAR, wherex(), wherey(), GREEN, BLACK, WHITE, LIGHTGRAY, WHITE};
    drawFormLayoutSimple(l, "Kelas Kamar.   (tersedia)");

    int jml_tersedia[JML_TIPEKAMAR];
    string menus[JML_TIPEKAMAR];
    for (int i = 0; i < JML_TIPEKAMAR; i++) {
        jml_tersedia[i] = getTipeKamarTersediaCount(TIPE_KAMAR[i].id);
        menus[i] =  numtostr(TIPE_KAMAR[i].id) + "\t" + 
                    strfill(TIPE_KAMAR[i].nama_tipe, 15) + " " + 
                    "("+numtostr(jml_tersedia[i])+" unit)";
    }

    int pilihan = 1;
    int maxpilihan = JML_TIPEKAMAR;

    gotoxy(l.x+2,l.y+1);
    setMenuSelectorColor(l.bg1,l.fg1);
	menuSelector(menus, &pilihan, maxpilihan, 1);

    // bersihkan tabel.
    clearArea(l.x, l.y, l.w, maxpilihan + 2, l.bg2);

    *t = &TIPE_KAMAR[pilihan-1];
    *jml_unit = jml_tersedia[pilihan - 1];
}

long hitungHargaKamar(TipeKamar t, int jml_unit)
{
    return (t.harga * jml_unit);
}

long hitungHargaKamarDiskon(TipeKamar t, long subtotal, int lama_inap)
{
    return  (long)(t.diskon * subtotal) * int(lama_inap/t.kelipatan_hari);
}

long hitungHargaPakan(PaketMakanan m, int jml_pkt)
{
    return (m.harga * jml_pkt);
}

void tampilTagihan(Pelanggan p)
{   //	        w     h         x   y  bg   fg0    fg1    bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, 38-wherey(), wherex(), wherey(), RED, BLACK, WHITE, WHITE, WHITE};
    int edgesize = 1;
    int padx = 2;
    int pady = edgesize + 0;
    
    int x = padx + l.x;
    int y = pady + l.y;
    
    string header = " > INFORMASI TAGIHAN                | BIAYA KOTOR     | DISKON          | BIAYA BERSIH ";
	drawTableLayout(l, header);
    
    // TAGIHAN SEWA KAMAR
    long subt_kamar = 0;
    textcolor(l.fg1);
    int c=1;
    for (int i=0;i<JML_TIPEKAMAR;i++){
        TipeKamar t = TIPE_KAMAR[i];
        int qty = getTipeKamarDipesanCount(p, t.id);
        
        // tipe kamar yg tidak dipesan, tidak ditampilkan.
        if (qty == 0) continue;
        
        if (i%2 == 0) textbackground(l.bg1); 
        else textbackground(l.bg2); 
        
        long kotor = hitungHargaKamar(t, qty);
        long diskon = hitungHargaKamarDiskon(t, kotor, p.lama_inap);
        long bersih = kotor-diskon;
        
        gotoxy(x,y++);  cout<<setw(20)<<setfill(' ')<<left<<(numtostr(c)+". Kelas "+string(t.nama_tipe))
                            <<setw(9)<<right<<("("+numtostr(qty)+" unit)");
                        cout<<" : "<<setw(18)<<right<<numtocurr(kotor);
                        cout<<setw(18)<<right<<numtocurr(diskon)<<setw(18)<<numtocurr(bersih);
                       
        subt_kamar += bersih;
        c++;
    }
    
    // perhitungan total biaya sewa kamar. (tapi belum sama biaya kelebihan inap)
    int total = subt_kamar * p.lama_inap;
    
    // tampilin subtotal biaya semua kamar yg disewa.
    textbackground(LIGHTGRAY);
    gotoxy(x,y++); cout <<setw(68)<<setfill(' ')<<left<<"Subtot kamar : "
                        <<setw(18)<<right<<numtocurr(subt_kamar);
    
    // tampilin total biaya sewa kamar. (tapi belum sama biaya tambahan kelebihan inap)
    textbackground(DARKGRAY);
    gotoxy(x,y++); cout <<setw(38)<<setfill(' ')<<left<<"Lama Inap : "
                        <<setw(33)<<right<<"("+numtostr(p.lama_inap)+" hari) x ("+numtocurr(subt_kamar)+") ="
                        <<setw(15)<<right<<numtocurr(total);
    
    // perhitungan buat kelebihan inap.
    Tanggal tgl_hrs_out = tglnext(p.tgl_checkin, p.lama_inap);
    
        // jika pelanggan cek-out sebelum tgl seharusnya (diff<0), maka kelebihan inap dianggap 0.
        // diff>0 artinya ada kelebihan waktu inap.
    int diff = (int)tglDiff(p.tgl_checkout, tgl_hrs_out);
        diff = diff >= 0 ? diff : 0; 
    
    // perhitungan kelebihan hari inap dilakukan jika plg sudah cek-out, jika belum maka masih dianggap 0.
    int inap_lebih = isCheckedOut(p) ? diff : 0;
    long biaya_inap_lebih = subt_kamar * inap_lebih;
    
    // totalin dari biaya sewa kamar sebelumnya + biaya kelebihan inap.
    total = total + biaya_inap_lebih;
    
    // tampilin biaya kelebihan inap
    gotoxy(x,y++); cout <<setw(38)<<setfill(' ')<<left<<"Biaya Kelebihan Inap : "
                        <<setw(33)<<right<<"("+numtostr(inap_lebih)+" hari) x ("+numtocurr(subt_kamar)+") ="
                        <<setw(15)<<right<<numtocurr(biaya_inap_lebih);
    
    // total tagihan kamar final.
    textbackground(GREEN); 
    gotoxy(x,y++); cout <<setw(68)<<setfill(' ')<<left<<"Total Tagihan Kamar : "
                        <<setw(18)<<right<<numtocurr(total);
    
    // garis pemisah.
    textbackground(l.bg1);
    gotoxy(l.x+1,y++); cout<<setw(l.w-2)<<setfill('-')<<"";
    
    // TAGIHAN PAKET MAKANAN
    long subt_mkn = 0;
    textbackground(l.bg1); textcolor(l.fg1);
    if (p.jml_pktmakanan==0){
        gotoxy(x,y++); cout<<"-(tidak ada paket makanan dipesan)";
    }
    for (int i=0;i<p.jml_pktmakanan;i++){
        if (i%2 == 0) textbackground(l.bg1); 
        else textbackground(l.bg2); 
        
        PaketMakanan m = *getPaketMakananById(p.paket_makanan[i][0]);
        int qty = p.paket_makanan[i][1];
        
        long kotor = hitungHargaPakan(m,p.paket_makanan[i][1]);
        long diskon = 0;
        long bersih = kotor-diskon;
        
        gotoxy(x,y++);  cout<<setw(26)<<setfill(' ')<<left<<("- "+string(m.nama_paket))
                            <<("("+numtostr(qty)+")");
                        cout<<" : "<<setw(18)<<right<<numtocurr(kotor);
                        cout<<setw(18)<<right<<numtocurr(diskon)<<setw(18)<<numtocurr(bersih);
                       
        subt_mkn += bersih;
    }
    
    // subtotal tagihan paket makanan
    textbackground(LIGHTGRAY);
    gotoxy(x,y++); cout <<setw(68)<<setfill(' ')<<left<<"Subtot paket makanan : "
                        <<setw(18)<<right<<numtocurr(subt_mkn);
}

void tampilDetailPelanggan(Pelanggan p)
{   //	        w     h         x   y  bg   fg0    fg1    bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, 38-wherey(), wherex(), wherey(), RED, BLACK, WHITE, WHITE, LIGHTGRAY};    
    int edgesize = 1;
    int padx = 2;
    int pady = edgesize + 1;
    
    int x = padx + l.x;
    int y = pady + l.y;
    
    clearMainFrame();
    
    string header = " > INFORMASI DETAIL PELANGGAN, ID : "+numtostr(p.id);
	drawTableLayout(l, header);
	
	double diff = tglDiff(p.tgl_checkin,p.tgl_checkout);
    string data[] = {
		"Nama Pemesan      : " + string(p.nama),
        "No Identitas      : " + string(p.no_identitas),
        "Alamat            : " + string(p.alamat),
        "Lama Inap         : " + numtostr(p.lama_inap),
        "",
        "Tgl Check-In      : " + tgltostr(p.tgl_checkin),
        "Tgl Harus Checkout: " + tgltostr(tglnext(p.tgl_checkin, p.lama_inap)),
        "Tgl Check-Out     : " + (diff==0?"Belum Checkout":tgltostr(p.tgl_checkout)),
        "end"};
        
    textcolor(l.fg1);
    string *s=data;
    int col = 0;
    for (int i=0; *s!="end" ; i++){
        if (*s == "") {
            col = 40; i = -1;
        }
        
        if (i%2 == 1) textbackground(l.bg1); 
        else textbackground(l.bg2); 
        
        coutxy(x + col,y+i,*s);
        s++;
    }
    
    // menu lihat kamar
    string menu[] = {
		"Lihat Kamar",
        "<< KEMBALI "};        
    int pilihan = 1;
    int maxpilihan = 2;
	
	bool selesai=false;
	do{
    gotoxy(l.x, y+5); tampilTagihan(p);
    
    gotoxy(x+79,y);
    setMenuSelectorColor(LIGHTGRAY, BLACK);
	menuSelector(menu, &pilihan, maxpilihan, 1);
	switch(pilihan){	    
        case 1:
            clearArea(x+56, y+5, 37, 27, DARKGRAY);
            gotoxy(x+57,y+6); tampilPesananKamar(p);
            getch();
            clearArea(x+57, y+6, 35, p.jml_kamar+2, l.bg1);
            break;
        default:
            selesai=true;
            break;
    }
    } while(!selesai);
    //while (confirm(x+65,y+4,"Kembali ? ",l.bg2) == 'y') break;
}

void pilihkanKamar(Pelanggan *p, TipeKamar t, int jml)
{
    // validasi jml kmr dipesan.
    if ( !isTipeKamarTersedia(t,jml) ){
        return;
    }
    
    // reset kamar2 yg dipesan.
    Kamar *k_del[(*p).jml_kamar];
    int jml_del = 0;
    for (int i=0;i<(*p).jml_kamar;i++){
        Kamar *k = getKamarById((*p).kamar[i]);
        if ((*k).tipe_kamar == t.id){
            k_del[jml_del] = k;
            jml_del++;
        }
    }
    for(int i=0;i<jml_del;i++){
        Kamar *k = k_del[i];
        deletePesananKamar(p, (*k).id);
    }
    
    // jika jml unit mencukupi, pilihkan kamar yg kosong utk pelanggan.
    Kamar *k_add[jml];
    for (int i=0;i<jml;i++){
        Kamar *k = getKamarTersedia(t.id);
        k_add[i] = k;
        
        setKamarDitempati(k); // tandai sementara.
        addPesananKamar(p, (*k).id);
    }
    for(int i=0;i<jml;i++){
        Kamar *k = k_add[i];
        setKamarDitinggalkan(k); // reset penandaan.
    }
}

bool checkoutPelangganProses(Pelanggan *p)
{
    if (isCheckedOut(*p)) {
        cout<<"Gagal. Pelanggan sudah checkout. ";
        return false;
    }
    
    (*p).tgl_checkout = curtgl();
    keluarKamar(*p);
    
    return true;
}

void checkinPelangganProses(Pelanggan *p)
{
    (*p).tgl_checkin = curtgl();
    tempatiKamar(*p);
}

void tampilFormInputPelanggan(Pelanggan *p) 
{   //			w  h  x  y  bg  fg0    fg1  bgrow0  bgrow1
    Layout l = {MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - 3, wherex(), wherey(), RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int edgesize = 2;
    int padx = 3;
    int pady = edgesize + 1;
    
    bool editing = p==NULL ? false : true;
    
    string menus[] = {
		"o Nama Pemesan      :",
        "o No Identitas      :",
        "o Alamat            :",
        "o Lama Inap         :",
        "o PILIH KAMAR       :",
        "",
        "@ Tgl Check-In      : " + (editing? tgltostr((*p).tgl_checkin) : tgltostr(curtgl())),
        "",
        "<< BATAL             ",
        ">> SELESAI & SIMPAN  "};
    for (int i=0;editing && i<5;i++) menus[i][0] = 'v';
        
    int pilihan = 1;
    int maxpilihan = 10;

    clearMainFrame();
    drawFormLayout(l, "FORMULIR PEMESANAN KAMAR PELANGGAN");

    Pelanggan buf = editing ? *p : newPelanggan();
    Kamar *k;
    
    bool valid_pilih;
    char simpan, selesai = 't';
    do {
        x = padx + l.x;
        y = pady + l.y;

        clearArea(x+55, y-1, 35, 36-l.y, l.bg1);
        gotoxy(x+55, y-1); tampilPesananKamar(buf);
        
        x+=22;
        
        // tampil data formulir pemesanan.
        textbackground(l.bg1); textcolor(l.fg1);
        if (menus[0][0] == 'v') {gotoxy(x,y);   cout<<buf.nama;}
        if (menus[1][0] == 'v') {gotoxy(x,y+1); cout<<buf.no_identitas;}
        if (menus[2][0] == 'v') {gotoxy(x,y+2); cout<<buf.alamat;}
        if (menus[3][0] == 'v') {gotoxy(x,y+3); cout<<buf.lama_inap;}
                
        gotoxy(x-22,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        int spc = l.w-(padx+22)-1;
        int wclr = 33;
        
        // inputan user.
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1: // nama pemesan
                clearLine(x,y,wclr,l.bg1);
                gotoxy(x, y); cout << setw(sizeof(buf.nama)-1) << setfill(' ') << "" << "|";
                gotoxy(x, y); cin.getline(buffer, sizeof (buffer)); fflush(stdin);
                clearLine(x,y,wclr,l.bg1);
                
                //validasi.
                if (isEmptyText(buffer) || isOverDigit(buffer, sizeof(buf.nama))) {
                    menus[0][0] = 'x';
                    continue;
                }

                strncpy(buf.nama, buffer, sizeof(buf.nama));
                
				menus[0][0] = 'v';
                break;
            case 2: // no identitas
                clearLine(x,y+1,wclr,l.bg1);
                gotoxy(x, y+1); cout << setw(sizeof(buf.no_identitas)-1) << setfill(' ') << "" << "|";
                gotoxy(x, y+1); cin.getline(buffer, sizeof (buffer)); fflush(stdin);
                clearLine(x,y+1,wclr,l.bg1);
                
                //validasi.
                if (isEmptyText(buffer) || isOverDigit(buffer, sizeof(buf.no_identitas))) {
                    menus[1][0] = 'x';
                    continue;
                }

                strncpy(buf.no_identitas, buffer, sizeof(buf.no_identitas));
                
				menus[1][0] = 'v';
                break;
            case 3: // alamat
                clearLine(x,y+2,wclr,l.bg1);
                gotoxy(x, y+2); cout << setw(sizeof(buf.alamat)-1) << setfill(' ') << "" << "|";
                gotoxy(x, y+2); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+2,wclr,l.bg1);
                
                //validasi.
                if (isEmptyText(buffer) || isOverDigit(buffer, sizeof(buf.alamat))) {
                    menus[2][0] = 'x';
                    continue;
                }

                strncpy(buf.alamat, buffer, sizeof(buf.alamat));
                
				menus[2][0] = 'v';
                break;
            case 4: // lama inap
                clearLine(x,y+3,wclr, l.bg1);
                gotoxy(x,y+3); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+3,wclr, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumberPos(buffer)) {
                    menus[3][0] = 'x';
                    continue;
                }
                
                buf.lama_inap = atoi(buffer);
                
                menus[3][0] = 'v';
                break;
            case 5: // pilih kamar
                TipeKamar *t;
                int jml;
                
				clearLine(x,y+4,wclr,l.bg1);
				gotoxy(x,y+4); tampilKamarTersediaSelector(&t, &jml);
				clearLine(x,y+4,wclr,l.bg1);
                
                if (t == NULL) {
                    coutxy(x, y + 4, "Tidak ada data. ");
                    continue;
                }

                clearLine(x,y+4,wclr,l.bg1);
				gotoxy(x, y+4); cout << (*t).nama_tipe;
                                cout << ", jml = "; 
                                cin.getline(buffer, sizeof (buffer)); fflush(stdin);
                clearLine(x,y+4,wclr,l.bg1);
                
                //validasi.
                if (isEmptyText(buffer) || !isNumberPosZ(buffer)) {
                    continue;
                }
                				
				// pilihkan kamar utk pelanggan seketemunya, sebanyak n.
				pilihkanKamar(&buf, *t, atoi(buffer));
				
				if (buf.jml_kamar == 0) menus[4][0] = 'x';
				else menus[4][0] = 'v';				
				
                break;
            case 9:
                clearLine(x,y+8,spc,l.bg1);
                selesai = confirm(x, y+8, " ? ", l.bg1);
                break;
            case 10:
                clearLine(x,y+9,spc,l.bg1);
                simpan = confirm(x, y+9, " ? ", l.bg1);
                if (simpan == 'y' && isValidForm(menus,5)){
                    if (!editing && addPelanggan(buf)){
                        
                        // checkin();
                        checkinPelangganProses(&buf);
                        if (!savePelangganToFile()) continue;
                        
                        selesai='y';
                        getch();    
                        
                        // tampilkan detail pelanggan di main frame.
                        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); 
                        tampilDetailPelanggan(buf);
                    } else if (editing && editPelanggan(p,buf) && savePelangganToFile()){
                        selesai='y';
                        getch();    
                    }
                }
            break;
        }
    } while (selesai == 't');
}

void tampilFormCheckoutPelanggan() 
{
    Layout l = {MAIN_FRAME_WIDTH, 3, wherex(), wherey(), RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int headersize = 2;
    int padx = 2;
    int pady = headersize + 0;
    
	string menus[] = {
	    "o Checkout By ID    :",
        "<< Kembali           "};
    
    int pilihan = 1;
    int maxpilihan = 2;
    
	drawFormLayout(l, "CHECKOUT PELANGGAN");
	
	Pelanggan *p;

    bool valid_pilih;
    char ckout, kembali = 't';
    do {        
        x = padx + l.x;
        y = pady + l.y;

        clearArea(MAIN_FRAME_X, MAIN_FRAME_Y + 5, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - (y + 5), MAIN_FRAME_BGCOLOR);
        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y + 5); tampilPelanggan();
        
        x+=22;
        
        gotoxy(x-22,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        // input user.
        int spc = l.w-(padx+22)-1;
		textbackground(l.bg1); textcolor(l.fg1);
        switch(pilihan){
            case 1:
			    clearLine(x, y, spc, l.bg1);
                gotoxy(x, y); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x, y, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer) ) {
                    continue;
                }
				
				// konfirmasi simpan.
				ckout = confirm(x, y, "Checkout ID Pelanggan '"+string(buffer)+"' ? ", l.bg1);
				if (ckout == 'y') {
                    int id_to_ckout = atoi(buffer);
                    p = getPelangganById(id_to_ckout);
                    
                    if (p==NULL){
                        cout<<"Data tidak ditemukan. ";
                        continue;
                    }
                    
                    // checkout.
                    if (checkoutPelangganProses(p)){
                        savePelangganToFile();
                        
                        // tampil tagihan.
                        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y);
                        tampilDetailPelanggan(*p);
                    }
                    
       		    }
       		    break;
        	default: // batal?
                kembali = 'y';
                break;
		}
    } while (kembali == 't');
}

void tampilFormCariPelanggan() 
{
    Layout l = {MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - 3, wherex(), wherey(), RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int edgesize = 2;
    int padx = 3;
    int pady = edgesize + 0;
    
    string menus[] = {
		">> RESET             ",
        "o ID Pelanggan      :",
        "o Nama Pelanggan    :",
        "o No. Identitas     :",
        "o Alamat            :",
        "o Nomor Kamar       :",
        "o Tipe Kamar        :",
        "o Tgl Check-In      :",
        "o Tgl Check-Out     :",
        "<< KEMBALI           "};
        
    int pilihan = 1;
    int maxpilihan = 10;

    clearMainFrame();
    drawFormLayout(l, "ISI KRITERIA UNTUK PENCARIAN DATA PELANGGAN (BOLEH KOSONG)");
    
    string kid, knama, knoid, kalmt, knokmr, ktpkmr, ktglin, ktglout;
    
    bool valid_pilih;
    char cari, selesai = 't';
    do {
        x = padx + l.x;
        y = pady + l.y;

        clearArea(l.x, y+10, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT-(y+10), l.bg1);
        gotoxy(l.x, y+10); tampilPelanggan(kid,knama,knoid,kalmt,knokmr,ktpkmr,ktglin,ktglout);
        
        x+=22;
        
        // tampil kriteria.
        textbackground(l.bg1); textcolor(l.fg1);
        gotoxy(x, y+1);cout<<kid;
        gotoxy(x, y+2);cout<<knama;
        gotoxy(x, y+3);cout<<knoid;
        gotoxy(x, y+4);cout<<kalmt;
        gotoxy(x, y+5);cout<<knokmr;
        gotoxy(x, y+6);cout<<ktpkmr;
        gotoxy(x, y+7);cout<<ktglin;
        gotoxy(x, y+8);cout<<ktglout;
        
        gotoxy(x-22,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        int spc = l.w-(padx+22)-1;
        
        // inputan user.
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1:
                clearLine(x,y+1,spc,l.bg1);
                clearLine(x,y+2,spc,l.bg1);
                clearLine(x,y+3,spc,l.bg1);
                clearLine(x,y+4,spc,l.bg1);
                clearLine(x,y+5,spc,l.bg1);
                clearLine(x,y+6,spc,l.bg1);
                clearLine(x,y+7,spc,l.bg1);
                clearLine(x,y+8,spc,l.bg1);
                kid = knama = knoid = kalmt = knokmr = ktpkmr = ktglin = ktglout = "";
                break;
            case 2: // id
                clearLine(x,y+1,spc,l.bg1);
                gotoxy(x, y+1); getline(cin, kid); cin.sync();
                clearLine(x,y+1,spc,l.bg1);
                
                break;
            case 3: // nama
                clearLine(x,y+2,spc,l.bg1);
                gotoxy(x, y+2); getline(cin, knama); cin.sync();
                clearLine(x,y+2,spc,l.bg1);
                
                break;
            case 4: // no identitas
                clearLine(x,y+3,spc,l.bg1);
                gotoxy(x, y+3); getline(cin, knoid); cin.sync();
                clearLine(x,y+3,spc,l.bg1);
                
                break;
            case 5: // alamat
                clearLine(x,y+4,spc,l.bg1);
                gotoxy(x, y+4); getline(cin, kalmt); cin.sync();
                clearLine(x,y+4,spc,l.bg1);
                
                break;
            case 6: // nmr kmr
                clearLine(x,y+5,spc,l.bg1);
                gotoxy(x+15, y+5); cout<<"(hanya untuk pelanggan yg belum check-out)";
                gotoxy(x, y+5); getline(cin, knokmr); cin.sync();
                clearLine(x,y+5,spc,l.bg1);
                
                break;
            case 7: // tipe kamar
				clearLine(x,y+6,spc,l.bg1);
				gotoxy(x,y+6); getline(cin, ktpkmr); cin.sync();
				clearLine(x,y+6,spc,l.bg1);

                break;
            case 8: //tgl cekin
                clearLine(x,y+7,spc,l.bg1);
                gotoxy(x+15, y+7); cout<<"format:(DD-MM-YYYY)";
                gotoxy(x, y+7); getline(cin, ktglin); cin.sync();
                clearLine(x,y+7,spc,l.bg1);
                
                break;
            case 9: //tgl cekout
                clearLine(x,y+8,spc,l.bg1);
                gotoxy(x+15, y+8); cout<<"format:(DD-MM-YYYY)";
                gotoxy(x, y+8); getline(cin, ktglout); cin.sync();
                clearLine(x,y+8,spc,l.bg1);
                
                break;
            default:
                selesai = 'y';
                break;
        }
    } while (selesai == 't');
}

void tampilFormOperasiPelanggan() 
{
    Layout l = {MAIN_FRAME_WIDTH, 5, wherex(), wherey(), RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    int x, y;
    int edgesize = 2;
    int padx = 3;
    int pady = edgesize + 0;

    string menus[] = {
		"Detail Plg By ID    :",
        "Edit   By ID        :",
        "Hapus  By ID        :",
        "Pencarian Data Plg  :",
        "<< KEMBALI           "};
    int pilihan = 1;
    int maxpilihan = 5;
    
    clearMainFrame();
    drawFormLayout(l, "DATA PELANGGAN. ");
        
    // kriteria data yg akan diedit atau dihapus.
    Pelanggan *p;
    int id_plg;

    bool valid_pilih;
    char hapus = 't', kembali = 't';
    do {        
        x = padx + l.x;
        y = pady + l.y;

        clearArea(MAIN_FRAME_X, MAIN_FRAME_Y + 7, MAIN_FRAME_WIDTH, MAIN_FRAME_HEIGHT - (y + 7), MAIN_FRAME_BGCOLOR);
        gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y + 7); tampilPelanggan();
        
        gotoxy(x,y);
        setMenuSelectorColor(l.bg1,l.fg1);
		menuSelector(menus, &pilihan, maxpilihan, 1);

        x+=22;
        int spc = l.w-(padx+22)-1;
        
        // memasukkan nilai oleh user.        
        textbackground(l.bg1); textcolor(l.fg1);
        switch (pilihan) {
            case 1:
                clearLine(x, y, spc, l.bg1);
                gotoxy(x, y); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x, y, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }
                
                id_plg = atoi(buffer);
                p = getPelangganById(id_plg);
                
                if (p==NULL){
                    coutxy(x,y,"Data tidak ditemukan. ");
                    continue;
                }
                
                // menampilkan detail pelanggan di main frame.
                gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y);
                tampilDetailPelanggan(*p);
                
                clearMainFrame();
                drawFormLayout(l, "DATA PELANGGAN. ");
                break;
            case 2:
                clearLine(x, y+1, spc, l.bg1);
                gotoxy(x, y+1); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x, y+1, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }
                
                id_plg = atoi(buffer);
                p = getPelangganById(id_plg);
                
                if (p==NULL){
                    coutxy(x,y+1,"Data tidak ditemukan. ");
                    continue;
                }
                
				gotoxy(l.x,l.y); tampilFormInputPelanggan(p);
				
				clearMainFrame();
                drawFormLayout(l, "DATA PELANGGAN. ");
                break;
            case 3:
                clearLine(x,y+2, spc, l.bg1);
                gotoxy(x,y+2); cin.getline(buffer, sizeof(buffer)); fflush(stdin);
                clearLine(x,y+2, spc, l.bg1);

                //validasi.
                if (isEmptyText(buffer) || !isNumber(buffer)) {
                    continue;
                }
                
                id_plg = atoi(buffer);
				hapus = confirm(x,y+2, "Hapus ID " + string(buffer) + " ? ", l.bg1);
                if (hapus == 'y') {
                    Pelanggan delp = *getPelangganById(id_plg);
                    if (isCheckedOut(delp) && deletePelanggan(id_plg)) {
                        keluarKamar(delp);
                        savePelangganToFile();
                    }
                }
                break;
            case 4:
                gotoxy(l.x,l.y); tampilFormCariPelanggan();
                
                clearMainFrame();
                drawFormLayout(l, "DATA PELANGGAN. ");
                break;
            case 5: // keluar?
				kembali = 'y';
                break;
        }
    } while (kembali == 't');
}

void tampilPelanggan()
{
    tampilPelanggan("","","","","","","","");
}

void tampilPelanggan(string kid, string knama, string knoid, 
                     string kalmt, string knokmr, string ktpkmr, 
                     string ktglin, string ktglout) 
{
    Layout l = {MAIN_FRAME_WIDTH, 38-wherey(), wherex(), wherey(), RED, BLACK, WHITE, WHITE, LIGHTGRAY};
    string header = "| ID   | NAMA PELANGGAN      | ALAMAT          | TGL CEK-IN        | INAP | CKOUT | DETAIL  ";
    
    int x = l.x, y = l.y;
    
	drawTableLayout(l, header);
        
	if (JML_PELANGGAN == 0) {
        textbackground(l.bg1); textcolor(l.fg1);
      	coutxy(x+2, ++y, 	"  tidak ada data. ");
	}	
	int counter = 0;
    for (int i = 0; i < JML_PELANGGAN; i++) {
        if (y >= (l.y + l.h)) {
            coutxy(x+2, y, "(...more...)");
            break;
        }
        
        if (counter % 2 == 0) { textbackground(l.bg1); textcolor(l.fg1); } 
        else { textbackground(l.bg2); textcolor(l.fg1); }
        
        int pid, pnama, pnoid, pnokmr, palmt, ptpkmr, ptglin, ptglout;
        pid=pnama=pnoid=palmt=pnokmr=ptpkmr=ptglin=ptglout = -1;
        
        pid = numtostr(PELANGGAN[i].id).find(strlow(kid));
        pnama = strlow(PELANGGAN[i].nama).find(strlow(knama));
        pnoid = strlow(PELANGGAN[i].no_identitas).find(strlow(knoid));
        palmt = strlow(PELANGGAN[i].alamat).find(strlow(kalmt));
        ptglin = strlow(tgltostr(PELANGGAN[i].tgl_checkin)).find(strlow(ktglin));
        ptglout = strlow(tgltostr(PELANGGAN[i].tgl_checkin)).find(strlow(ktglout));
        
        for (int j=0;j<PELANGGAN[i].jml_kamar;j++){
            Kamar k = *getKamarById(PELANGGAN[i].kamar[j]);
            TipeKamar t = *getTipeKamarById(k.tipe_kamar);
            if (pnokmr == -1) pnokmr = strlow(k.nomor_kamar).find(strlow(knokmr));
            if (ptpkmr == -1) ptpkmr = strlow(t.nama_tipe).find(strlow(ktpkmr));
        }
        
        pnokmr = (knokmr!="" && isCheckedOut(PELANGGAN[i])) ? -1 : pnokmr;
        
        if (pid == -1 || pnama==-1 || pnoid==-1 || palmt==-1 || pnokmr==-1 || ptpkmr==-1 || ptglin==-1 || ptglout==-1) continue;

        coutxy(x + 2, ++y, numtostr(PELANGGAN[i].id));
        coutxy(x + 9, y, PELANGGAN[i].nama);
        coutxy(x + 31, y, PELANGGAN[i].alamat);
        coutxy(x + 49, y, tgltostr(PELANGGAN[i].tgl_checkin));
        coutxy(x + 70, y, numtostr(PELANGGAN[i].lama_inap));
        coutxy(x + 76, y, isCheckedOut(PELANGGAN[i])?"sudah":"belum");
        coutxy(x + 84, y, "Detail ->");
        
        counter++;
    }
}

void tampilFormCheckin()
{
    gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); tampilFormInputPelanggan(NULL);
}

void tampilFormCheckout()
{
    gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); tampilFormCheckoutPelanggan();
}

// @MAKANAN & @PAKET MAKANAN ===================================================

// belum diimplemen.

// @HALAMAN APLIKASI ===========================================================

void tampilHalamanKamar() {
    string menus[] = {
        " >> TAMPILKAN     DATA KAMAR      ",
        " >> TAMBAH        DATA KAMAR      ",
        " >> UBAH & HAPUS  DATA KAMAR      ",
        "",
        " << KEMBALI                       "
    };

    int pilihan = 1;
    int maxpilihan = 5;
    char kembali = 't';
    do {
        clearMenuFrame();
		gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); tampilKamar();

        gotoxy(1,10);
        setMenuSelectorColor(MENU_FRAME_BGCOLOR,WHITE);
		menuSelector(menus, &pilihan, maxpilihan, 2);

        clearMainFrame();
        switch (pilihan) {
            case 1:
                gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); tampilKamar();
                break;
            case 2:
                tampilFormInputKamar(MAIN_FRAME_X, MAIN_FRAME_Y,NULL);
                clearMainFrame();
                break;
            case 3:
                tampilFormDeleteEditKamar(MAIN_FRAME_X, MAIN_FRAME_Y);
                clearMainFrame();
                break;
            default:
                kembali = 'y';
                clearMainFrame();
                break;
        }
    } while (kembali == 't');
}

void tampilHalamanTipeKamar() {
	string menus[] = {
        " >> TAMPILKAN     DATA TIPE KAMAR ",
        " >> TAMBAH        DATA TIPE KAMAR ",
        " >> UBAH & HAPUS  DATA TIPE KAMAR ",
        "",
        " >> TAMPILKAN     DATA FASILITAS  ",
        " >> TAMBAH        DATA FASILITAS  ",
        " >> UBAH & HAPUS  DATA FASILITAS  ",
        "",
        " << KEMBALI                       "
    };

    int pilihan = 1;
    int maxpilihan = 9;
    char kembali = 't';
    do {
		clearMenuFrame();
		//tampilTipeKamar(MAIN_FRAME_X, MAIN_FRAME_Y);

        gotoxy(1,10);
        setMenuSelectorColor(MENU_FRAME_BGCOLOR,WHITE);
		menuSelector(menus, &pilihan, maxpilihan, 2);
		
		clearMainFrame();
		switch (pilihan) {
            case 1:
                gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); tampilTipeKamar();
                break;
            case 2:
                tampilFormInputTipeKamar(MAIN_FRAME_X, MAIN_FRAME_Y,NULL);
                clearMainFrame();
                break;
            case 3:
                tampilFormDeleteEditTipeKamar(MAIN_FRAME_X, MAIN_FRAME_Y);
                clearMainFrame();
                break;
            case 5:
                gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); 
                tampilFasilitas();
                break;
            case 6:
                tampilFormInputFasilitas(MAIN_FRAME_X, MAIN_FRAME_Y, NULL);
                clearMainFrame();
                break;
            case 7:
                tampilFormDeleteEditFasilitas(MAIN_FRAME_X, MAIN_FRAME_Y);
                clearMainFrame();
                break;
            default:
                kembali = 'y';
                clearMainFrame();
                break;
        }
    } while (kembali == 't');
}

void tampilHalamanUtama() {		
	clearMenuFrame();
	clearMainFrame();
	
	string menus[] = {
        " >> CHECKIN  PELANGGAN BARU       ",
        " >> CHECKOUT PELANGGAN            ",
        "",
        " >> LAYANAN  PESAN-ANTAR          ",
        "",
        " >> DATA  PELANGGAN               ",
        " >> DATA  KAMAR HOTEL             ",
        " >> DATA  KELAS KAMAR             ",
        "",
        " X  EXIT                          "
    };

    int pilihan = 1;
    int maxpilihan = 10;
    char exit = 't';
    do {
	   	clearMenuFrame();
	   	        
        gotoxy(1,10);
        setMenuSelectorColor(MENU_FRAME_BGCOLOR,WHITE);
		menuSelector(menus, &pilihan, maxpilihan, 2);
		
		clearMainFrame();
		switch (pilihan) {
            case 1:
                tampilFormCheckin();
                clearMainFrame();
                break;
            case 2:
                tampilFormCheckout();
                clearMainFrame();
                break;
            case 4:
                // layanan pesan antar
                clearMainFrame();
                break;
            case 6:
                gotoxy(MAIN_FRAME_X, MAIN_FRAME_Y); 
                tampilFormOperasiPelanggan();
                
                clearMainFrame();
                break;
            case 7:
                tampilHalamanKamar();
                clearMainFrame();
                break;
            case 8:
                tampilHalamanTipeKamar();
                clearMainFrame();
                break;
            default:
                exit = confirm(10, 28, " ? ", CYAN);
                clearMainFrame();
                break;
        }
    } while (exit == 't');
}

void loadDataMaster() {
    gotoxy(1, 1);

    loadingSlash("Mengambil data master " + string(f_fasilitas) + ". ");
    loadFasilitasFromFile();
    cout << endl << endl;

    loadingSlash("Mengambil data master " + string(f_tipekamar) + ". ");
    loadTipeKamarFromFile();
    cout << endl << endl;

    loadingSlash("Mengambil data master " + string(f_kamar) + ". ");
    loadKamarFromFile();
    cout << endl << endl;
    
    loadingSlash("Mengambil data master " + string(f_pelanggan) + ". ");
    loadPelangganFromFile();
    cout << endl << endl;

    getch();
    clrscr();
}

int main(int argc, char *argv[]) {
    SetConsoleTitle("Super Hotel");
    SetWindow(130, 40);
    
    gotoxy(17,1); logo();
    getch();
    
    // here we go.
    loadDataMaster();
    
    tampilHalamanUtama();

    //system("PAUSE");
    return EXIT_SUCCESS;
}

void coutxy(int x, int y, string s) {
    gotoxy(x, y);
    cout << s;
}

void coutxy(string s) {
    coutxy(wherex(), wherey(), s);
}

void SetWindow(int Width, int Height) {
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height;
    Rect.Right = Width;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE); // Get Handle 
    SetConsoleScreenBufferSize(Handle, coord); // Set Buffer Size 
    SetConsoleWindowInfo(Handle, TRUE, &Rect); // Set Window Size 
    
    HWND console = GetConsoleWindow();
 	RECT r;
    GetWindowRect(console, &r);
  	MoveWindow(console, Width, Height, 1068, 682, TRUE);
}

void logo()
{
string logo[]={
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                               |                                              ",
"                                              |||                                             ",
"                                            |||||||                                           ",
"                                           |||||||||                                          ",
"                                           |||||| ||||                                        ",
"                                           ||| || ||||       ||                               ",
"                                          ||||||| ||||     ||||||                             ",
"                                          ||||||| ||||    |||| |||                            ",
"                                         |||||||| |||| ||||||| ||||                           ",
"                               ||||||    |||||||| |||| ||||||| ||||                           ",
"                            |||||||||||| ||||| || |||| |||| || || |                           ",
"                            |||| ||| ||| |||||||| |||| ||||||| || |                           ",
"                            |||||||| ||| ||||| || || | |||| || || |                           ",
"                            |||| ||| ||| |||||||| || | ||||||| || |                           ",
"                            |||||||| ||| ||||| || || | ||||||| || |                           ",
"                            |||| ||| ||| |||||||| || | ||||||| || |                           ",
"                            |||||||| ||| |||||||| || | |||| || || |                           ",
"                            |||| ||| ||| |||||||| || | ||||||| || |                           ",
"                            |||||||||||||||||||||||| | |||| || || |                           ",
"                            |||||||||||||||||||||||||| ||||||||||||                           ",
"                      |||||||||||                              ||||||||||                     ",
"                      |||                                            ||||                     ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"                                                                                              ",
"END"
};

textbackground(BLACK);
string *s = &logo[0];
int x = wherex(), y = wherey();
for (int i=0;*(s+2)!="END";i++){
    if (i<15) textcolor(LIGHTRED);
    else if (i<25) textcolor(YELLOW);
    else textcolor(LIGHTGREEN);
    
    gotoxy(x,y+i);
    for (int j=0;j<93;j++){
        if ((*s)[j] != ' ') Delay(0);
        
        if (j<40) textcolor(LIGHTRED);
        else if (j<55) textcolor(LIGHTCYAN);
        else textcolor(LIGHTGREEN);
        
        cout<<(*s)[j];
    }
    
    s++;
}
textcolor(WHITE);
gotoxy(1,1);
}
