#define DASH_BOARD      0
#define MAIN_MENU       1
#define VIEW_LOG        2
#define DOWNLOAD_LOG    3
#define CLEAR_LOG       4
#define SET_TIME        5    
int x = 0,x1 = 0;
int delay = 0, speed = 0;
static int count = 0;
int m_flag = 0, m_flag1 = 0,field_flag = 0,flag = 0;
unsigned char key, e_flag = 0;
unsigned char events[8][3] = {"CR", "GN", "G1", "G2", "G3", "G4", "G5", "GR"};
unsigned char menu[4][11] = {"view log", "down log", "clear log", "set time"};
unsigned char clock_reg[3];
unsigned char time[9];
unsigned char screen_flag = 0, event_count = 0,view_count = 0;
unsigned char h1, h2, m1, m2, s1, s2, data1, data2, data3;
char view[14];
void dashboard();
void mainmenu();
void display_time();
void get_time();
void store_event(int,char*);
void view_log();
void download_log();
void clear_log();
void set_time();
void display_speed(int);
void display_menu(int, int);
