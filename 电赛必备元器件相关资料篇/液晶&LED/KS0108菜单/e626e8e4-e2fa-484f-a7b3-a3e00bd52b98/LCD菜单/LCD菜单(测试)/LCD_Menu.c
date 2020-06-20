/*--------------------------------------------------------------*/
//菜单配置
#define Menu_arrow_mode	 0		//菜单指针显示模式
//						=0		//箭头
//						=1		//反色

#define Menu_roll_repeat 1		//是否重复滚动
//						=0		//不重复
//						=1		//重复

#define Menu_roll_Bar	 0		//是否显示滚动条(未完成)
//						=0		//不显示
//						=1		//显示

#define Menu_index		 1		//是否显示序号(数字输出,占用资源)
//						=0		//不显示
//						=1		//显示


/*--------------------------------------------------------------*/
//按键功能键宏定义
#define UP		0xf7	//P3_3
#define Down	0xfb	//P3_2
#define Esc		0xdf	//P3_5
#define Enter	0xef	//P3_4


/*--------------------------------------------------------------*/
//调用子函数区
void NullSubs(void)
{
}


void TimeSet(void)
{
	LCD_printss(7, 2, " TimeSet over!");
}

void DateSet(void)
{
	LCD_printss(7, 2, " DateSet over!");
}

void AlertSet (void)
{
	LCD_printss(7, 2, "AlertSet over!");
}

void LED1 (void)
{
	P1_7 = ~P1_7;
}

void LED2 (void)
{
	P1_6 = ~P1_6;
}

void LED3 (void)
{
	P1_5 = ~P1_5;
}

void LED4 (void)
{
	P1 = ~P1;
}

void delay50ms(void)
{
    unsigned char i,j,k;
    for(i=165;i>0;i--)
    for(j=60;j>0;j--)
    for(k=1;k>0;k--);
}

void about(void)
{
	unsigned char i, j;
	LCD_clr_scr(ASCR);		//液晶清屏
	i = 0; j = 0;
	while(i < 101)		//ASCII字符测试
	{ 
		LCD_printc(j & 0x07, i & 0x0f, i + 32); 
		delay50ms(); i++;
		if(!(i % 16)) j++;
	}
	LCD_printss(7, 2, "www.Alexi2008.com");
	while(P3 == 0xff);
	LCD_clr_scr(ASCR);		//液晶清屏 
}

void help(void)
{
	LCD_clr_scr(ASCR);		//液晶清屏
	LCD_printss(0, 0, "In this demo,you can");
	LCD_printss(1, 0, "press arrow key to mo");
	LCD_printss(2, 0, "ve the arrow or black");
	LCD_printss(3, 0, "to the right position");
	LCD_printss(4, 0, "and then press the En");
	LCD_printss(5, 0, "ter key to perform th");
	LCD_printss(6, 0, "e related function.");
	LCD_printss(7, 2, "^_^ [come on] ^_^");
	while(P3 == 0xff);
	LCD_clr_scr(ASCR);		//液晶清屏
}

void info(void)
{
	LCD_clr_scr(ASCR);		//液晶清屏
	LCD_printss(2, 2, "Nokia 5500 Sport");
	LCD_printss(3, 7, "V 04.60");
	LCD_printss(4, 6, "14-09-07");
	LCD_printss(5, 7, "RM-86");
	delay50ms();delay50ms();
	while(P3 == 0xff);
	LCD_clr_scr(ASCR);		//液晶清屏
}

/*--------------------------------------------------------------*/
//宏参数定义
#define Null 		0


/*--------------------------------------------------------------*/
//目录结构体定义
struct MenuItem
{
	unsigned char MenuCount;		//当前层节点数
	unsigned char *DisplayString;	//菜单标题
	void (*Subs)();					//节点函数
	struct MenuItem *ChildrenMenus; //子节点
	struct MenuItem *ParentMenus;	//父节点
};


/*--------------------------------------------------------------*/
//结构体区
struct MenuItem code TimeMenu[];
struct MenuItem code GalleryMenu[];
struct MenuItem code OrganiserMenu[];
struct MenuItem code MediaMenu[];
struct MenuItem code LedMenu[];
struct MenuItem code SMSMenu[];
struct MenuItem code OWNMenu[];
struct MenuItem code MainMenu[];


/*--------------------------------------------------------------*/
//结构体成员
#define  TimeMenuCount	4
struct MenuItem code TimeMenu[]=
{//MenuCount, 		 DisplayString, 	Subs, 		ChildrenMenus, ParentMenus
	{TimeMenuCount,   "1.Time Set",   TimeSet,   Null,   	MainMenu},
	{TimeMenuCount,   "2.Date Set",   DateSet,   Null,   	MainMenu},
	{TimeMenuCount,   "3.AlertSet",   AlertSet,  Null,   	MainMenu},
	{TimeMenuCount,   "4.Back",       NullSubs,  MainMenu,  MainMenu},
}; 

#define  OWNMenuCount	14
struct MenuItem code OWNMenu[]=
{//MenuCount, 		 DisplayString, 	Subs, 		ChildrenMenus, ParentMenus
	{OWNMenuCount,   "1.Navigator",		NullSubs,   Null,   	GalleryMenu},
	{OWNMenuCount,   "2.Landmarks",		NullSubs,   Null,   	GalleryMenu},
	{OWNMenuCount,   "3.Position",		NullSubs,  	Null,   	GalleryMenu},
	{OWNMenuCount,   "4.Themes",		NullSubs,   Null,   	GalleryMenu},
	{OWNMenuCount,   "5.Dictionary",	NullSubs,   Null,   	GalleryMenu},
	{OWNMenuCount,   "6.Adobe PDF",		NullSubs,  	Null,   	GalleryMenu},
	{OWNMenuCount,   "7.X-plore",		NullSubs,   Null,   	GalleryMenu},
	{OWNMenuCount,   "8.A4 Input",		NullSubs,   Null,   	GalleryMenu},
	{OWNMenuCount,   "9.Quickoffice",	NullSubs,  	Null,   	GalleryMenu},
	{OWNMenuCount,   "10.QReader",		NullSubs,   Null,   	GalleryMenu},
	{OWNMenuCount,   "11.MOTO-TXT",		NullSubs,  	Null,   	GalleryMenu},
	{OWNMenuCount,   "12.About",		about,   	Null,   	GalleryMenu},
	{OWNMenuCount,   "13.Help",			help,  		Null,   	GalleryMenu},
	{OWNMenuCount,   "14.Back",			NullSubs,   GalleryMenu,GalleryMenu},
};

#define  GalleryMenuCount	6
struct MenuItem code GalleryMenu[]=
{//MenuCount, 		 DisplayString, 	Subs, 		ChildrenMenus, ParentMenus
	{GalleryMenuCount,   "1.Images",   		NullSubs,   Null,   	MainMenu},
	{GalleryMenuCount,   "2.Video clips",   NullSubs,   Null,   	MainMenu},
	{GalleryMenuCount,   "3.Music",     	NullSubs,   Null,   	MainMenu},
	{GalleryMenuCount,   "4.Tracks",   		NullSubs,   Null,   	MainMenu},
	{GalleryMenuCount,   "5.All apps",   	NullSubs,   OWNMenu,   	MainMenu},
	{GalleryMenuCount,   "6.Back",   		NullSubs,   MainMenu,   MainMenu},
}; 

#define  OrganiserMenuCount	8
struct MenuItem code OrganiserMenu[]=
{//MenuCount, 		 DisplayString, 	Subs, 		ChildrenMenus, ParentMenus
	{OrganiserMenuCount,   "1.Calculator", 	NullSubs,   Null,   	MainMenu},
	{OrganiserMenuCount,   "2.Clock",     	NullSubs,   Null,   	MainMenu},
	{OrganiserMenuCount,   "3.Converter",   NullSubs,   Null,   	MainMenu},
	{OrganiserMenuCount,   "4.Note book",   NullSubs,   Null,   	MainMenu},
	{OrganiserMenuCount,   "5.Calendar",   	NullSubs,   Null,   	MainMenu},
	{OrganiserMenuCount,   "6.File mgr.",   NullSubs,   Null,   	MainMenu},
	{OrganiserMenuCount,   "7.Memory",   	NullSubs,   Null,   	MainMenu},
	{OrganiserMenuCount,   "8.Back",   		NullSubs,   MainMenu,   MainMenu},
}; 

#define  MediaMenuCount	6
struct MenuItem code MediaMenu[]=
{//MenuCount, 		 DisplayString, 	Subs, 		ChildrenMenus, ParentMenus
	{MediaMenuCount,   "1.Camera",   	NullSubs,   Null,   	MainMenu},
	{MediaMenuCount,   "2.RealPlayer",  NullSubs,   Null,   	MainMenu},
	{MediaMenuCount,   "3.Recorder",   	NullSubs,   Null,   	MainMenu},
	{MediaMenuCount,   "4.FlashPlayer", NullSubs,   Null,   	MainMenu},
	{MediaMenuCount,   "5.CorePlayer",  NullSubs,   Null,   	MainMenu},
	{MediaMenuCount,   "6.Back",   		NullSubs,   MainMenu,   MainMenu},
}; 

#define  LedMenuCount	5
struct MenuItem code LedMenu[]=
{//MenuCount, 		 DisplayString, 	Subs, 		ChildrenMenus, ParentMenus
	{LedMenuCount,   "1.Led1 ON/OFF",   LED1,   	Null,   	MainMenu},
	{LedMenuCount,   "2.Led2 ON/OFF",   LED2,   	Null,   	MainMenu},
	{LedMenuCount,   "3.Led3 ON/OFF",   LED3,   	Null,   	MainMenu},
	{LedMenuCount,   "4.All LED ON/OFF",LED4,   	Null,   	MainMenu},
	{LedMenuCount,   "5.Back",   		NullSubs,   MainMenu,   MainMenu},
}; 

#define  SMSMenuCount	13
struct MenuItem code SMSMenu[]=
{//MenuCount, 		 DisplayString, 	Subs, 		ChildrenMenus, ParentMenus
	{SMSMenuCount,   "1.New message",   NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "2.Inbox",   		NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "3.Outbox",   		NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "4.Mailbox",  		NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "5.My folders",   	NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "6.Drafts",   		NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "7.Sent",   		NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "8.Reports",   	NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "9.SIM message",   NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "10.Setting",  	NullSubs,   Null,   	MainMenu},
	{SMSMenuCount,   "11.Cell info",	info,   	Null,   	MainMenu},
	{SMSMenuCount,   "12.Help",  		help,   	Null,   	MainMenu},
	{SMSMenuCount,   "13.Back",   		NullSubs,   MainMenu,   MainMenu},
}; 

#define  MainMenuCount	7
struct MenuItem code MainMenu[]=
{//MenuCount, 		  DisplayString, 	 Subs, 		 ChildrenMenus, ParentMenus
	{MainMenuCount,   "1.Time Set" ,   	 NullSubs,   TimeMenu,   	Null},
	{MainMenuCount,   "2.Gallery",  	 NullSubs,   GalleryMenu,	Null},
	{MainMenuCount,   "3.Media", 		 NullSubs,   MediaMenu,  	Null},
	{MainMenuCount,   "4.Organiser",  	 NullSubs,   OrganiserMenu, Null},
	{MainMenuCount,   "5.LED Display",   NullSubs,   LedMenu,    	Null},
	{MainMenuCount,   "6.Messaging",  	 NullSubs,   SMSMenu,    	Null},
	{MainMenuCount,   "7.Back",   		 NullSubs,   MainMenu,   	MainMenu},
};


/*--------------------------------------------------------------*/
//全局变量声明区
struct MenuItem (*MenuPoint) = MainMenu;	//结构体指针,指向结构体后由内部函数指针指向功能函数
unsigned char DisplayStart = 0;				//显示时的第一个菜单项
unsigned char UserChoose = 0;				//用户所选菜单项
unsigned char DisplayPoint = 0;				//显示指针
unsigned char MaxItems;						//同级最大菜单数
unsigned char temp_n;						//临时变量
#define ShowPage	8						//同屏允许显示菜单数


/*--------------------------------------------------------------*/
//函数声明
void Show_Menu(void);
void Menu_Refresh(unsigned char KeyNum);


/*--------------------------------------------------------------*/
//显示函数区
void Show_Menu(void) 
{
	unsigned char n;
	
	MaxItems = MenuPoint[0].MenuCount;	//取得最大同级菜单
	DisplayPoint = DisplayStart;		//显示时的第一个菜单项

	for(n = 0; DisplayPoint < MaxItems && n < ShowPage; n++) 
	{
		LCD_clr_row(n);
	#if	Menu_arrow_mode			//反色显示
		LCD_printss(n, 0, MenuPoint[DisplayPoint].DisplayString);
		if(DisplayPoint == UserChoose) LCD_inverses(n, 0, 12);
	#else						//箭头显示
		LCD_printss(n, 2, MenuPoint[DisplayPoint].DisplayString);
		if(DisplayPoint == UserChoose) LCD_printss(n, 0, "->");
	#endif
		DisplayPoint++;
	#if	Menu_index				//显示序号
		if(!n)
		{
			if(UserChoose < 9) {LCD_printcs(0, 16, ' '); LCD_printn(0, 17, UserChoose+1, 1, 0);}
			else			   LCD_printn(0, 16, UserChoose+1, 2, 0);
			LCD_printcs(0, 18, '/');
			LCD_printn(0, 19, MaxItems, 2, 0);
		}
	#endif	
	}
}


/*--------------------------------------------------------------*/
//显示刷新
void Menu_Refresh(unsigned char KeyNum)
{
	unsigned char temp;

	switch(KeyNum) 
	{     
		case UP: UserChoose--;
			if (UserChoose == 255)		//上翻截至
			{
				#if	Menu_roll_repeat
				UserChoose = MaxItems - 1;
				#else
				UserChoose = 0;
				#endif
			}
			break;
		case Esc:
			if (MenuPoint[UserChoose].ParentMenus != Null)
			{
				MenuPoint = MenuPoint[UserChoose].ParentMenus;
				UserChoose = 0;
				DisplayStart = 0;
				LCD_clr_scr(ASCR);		//液晶清屏
			}
			break;
		case Down:
			UserChoose++;
			if (UserChoose == MaxItems) //下翻截至
			{
				#if	Menu_roll_repeat			
				UserChoose = 0;
				#else
				UserChoose = MaxItems - 1;
				#endif
			}
			break; 
		case Enter: 
			if (MenuPoint[UserChoose].Subs != NullSubs) 
			{ 
				(*MenuPoint[UserChoose].Subs)();	//执行函数功能
			} 
			else if (MenuPoint[UserChoose].ChildrenMenus != Null) 
			{ 
				MenuPoint = MenuPoint[UserChoose].ChildrenMenus; 
				UserChoose = 0;
				DisplayStart = 0;
				LCD_clr_scr(ASCR);			//液晶清屏
			} 
			break; 
		default: break;
	}

	DisplayStart = UserChoose / ShowPage;	//翻页
	DisplayStart = DisplayStart * ShowPage;
	if(temp != DisplayStart)
	{
		LCD_clr_scr(ASCR);					//液晶清屏
		temp = DisplayStart;
	}
	Show_Menu();							//显示菜单项
}
