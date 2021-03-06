/*
 * =============================================================================
 *
 *       Filename:  form_setting_timer.c
 *
 *    Description:  时间设置界面
 *
 *        Version:  1.0
 *        Created:  2018-03-01 23:32:41
 *       Revision:  none
 *
 *         Author:  xubin
 *        Company:  Taichuan
 *
 * =============================================================================
 */
/* ---------------------------------------------------------------------------*
 *                      include head files
 *----------------------------------------------------------------------------*/
#include "externfunc.h"
#include "screen.h"
#include "config.h"
#include "my_ntp.h"

#include "my_button.h"
#include "my_scroll.h"
#include "my_title.h"

#include "form_base.h"

/* ---------------------------------------------------------------------------*
 *                  extern variables declare
 *----------------------------------------------------------------------------*/
extern int createFormSettingDate(HWND hMainWnd,void (*callback)(void));
extern int createFormSettingTimeReal(HWND hMainWnd,void (*callback)(void));

/* ---------------------------------------------------------------------------*
 *                  internal functions declare
 *----------------------------------------------------------------------------*/
static int formSettingTimerProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam);

static void buttonTitleNotify(HWND hwnd, int id, int nc, DWORD add_data);
static int buttonSwitchNotify(HWND hwnd,void (*callback)(void));
static void reloadTimer(void);

/* ---------------------------------------------------------------------------*
 *                        macro define
 *----------------------------------------------------------------------------*/
#if DBG_FORM_SET_LOCAL > 0
	#define DBG_P( x... ) printf( x )
#else
	#define DBG_P( x... )
#endif

#define BMP_LOCAL_PATH "setting/"
enum {
	IDC_TIMER_1S = IDC_FORM_SETTING_TIEMR,
	IDC_BUTTON_SWITCH,
	IDC_SCROLLVIEW,
	IDC_TITLE,
};


struct ScrollviewItem {
	char title[32]; // 左边标题
	char text[32];  // 右边文字
	int (*callback)(HWND,void (*callback)(void)); // 点击回调函数
	int index;  // 元素位置
	int item_type; // 0标准 1开关
	int switch_state; // 当item_type为1时，此变量有效,0关闭，1开启
	int text_color_type; // 文字颜色,0暗，1亮
};

/* ---------------------------------------------------------------------------*
 *                      variables define
 *----------------------------------------------------------------------------*/
static BITMAP bmp_enter; // 进入
static BITMAP image_swich_off;// 关闭状态图片
static BITMAP image_swich_on;	// 打开状态图片
static HWND hScrollView;
static int bmp_load_finished = 0;
static int flag_timer_stop = 0;

static struct ScrollviewItem locoal_list[] = {
	{"自动获取","",buttonSwitchNotify},
	{"设置日期","",createFormSettingDate},
	{"设置时间","",createFormSettingTimeReal},
	{0},
};

static BmpLocation bmp_load[] = {
    {&bmp_enter,	BMP_LOCAL_PATH"ico_返回_1.png"},
	{&image_swich_off,BMP_LOCAL_PATH"Switch Off_小.png"},
	{&image_swich_on, BMP_LOCAL_PATH"Switch On_小.png"},
    {NULL},
};

static MY_CTRLDATA ChildCtrls [] = {
    SCROLLVIEW(0,40,1024,580,IDC_SCROLLVIEW),
};

static MyCtrlButton ctrls_button[] = {
	{0},
};

static MY_DLGTEMPLATE DlgInitParam =
{
    WS_NONE,
    // WS_EX_AUTOSECONDARYDC,
	WS_EX_NONE,
    0,0,SCR_WIDTH,SCR_HEIGHT,
    "",
    0, 0,       //menu and icon is null
    sizeof(ChildCtrls)/sizeof(MY_CTRLDATA),
    ChildCtrls, //pointer to control array
    0           //additional data,must be zero
};

static FormBasePriv form_base_priv= {
	.name = "FsetTimer",
	.idc_timer = IDC_TIMER_1S,
	.dlgProc = formSettingTimerProc,
	.dlgInitParam = &DlgInitParam,
	.initPara =  initPara,
	.auto_close_time_set = 30,
};

static MyCtrlTitle ctrls_title[] = {
	{
        IDC_TITLE, 
        MYTITLE_LEFT_EXIT,
        MYTITLE_RIGHT_NULL,
        0,0,1024,40,
        "日期时间",
        "",
        0xffffff, 0x333333FF,
        buttonTitleNotify,
    },
	{0},
};

static FormBase* form_base = NULL;

static void enableAutoClose(void)
{
	reloadTimer();
	Screen.setCurrent(form_base_priv.name);
	flag_timer_stop = 0;	
}
static void reloadTimer(void)
{
	int i;
	SVITEMINFO svii;
	struct ScrollviewItem *plist = locoal_list;
    SendMessage (hScrollView, SVM_RESETCONTENT, 0, 0);
	struct tm *tm = getTime();
	for (i=0; plist->title[0] != 0; i++) {
		plist->index = i;
		plist->item_type = 0;
		plist->text_color_type = 1;
		svii.nItemHeight = 60;
		svii.addData = (DWORD)plist;
		svii.nItem = i;
		if (strcmp("设置日期",plist->title) == 0) {
			sprintf(plist->text,"%d年%d月%d日",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday);
			if (g_config.auto_sync_time)
				plist->text_color_type = 0;
		} else if (strcmp("设置时间",plist->title) == 0) {
			sprintf(plist->text,"%d:%02d",tm->tm_hour,tm->tm_min);
			if (g_config.auto_sync_time)
				plist->text_color_type = 0;
		} else if (strcmp("自动获取",plist->title) == 0) {
			plist->item_type = 1;
			plist->switch_state = g_config.auto_sync_time;
		}
		SendMessage (hScrollView, SVM_ADDITEM, 0, (LPARAM)&svii);
		SendMessage (hScrollView, SVM_SETITEMADDDATA, i, (DWORD)plist);
		plist++;
	}
}

/* ----------------------------------------------------------------*/
/**
 * @brief buttonTitleNotify 标题按钮
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ----------------------------------------------------------------*/
static void buttonTitleNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	ShowWindow(GetParent(hwnd),SW_HIDE);
}

static int buttonSwitchNotify(HWND hwnd,void (*callback)(void))
{
	g_config.auto_sync_time = hwnd;	
	ntpEnable(g_config.auto_sync_time);
	ConfigSavePublic();
	reloadTimer();
}

void formSettingTimerLoadBmp(void)
{
    if (bmp_load_finished == 1)
        return;

	printf("[%s]\n", __FUNCTION__);
    bmpsLoad(bmp_load);
    bmp_load_finished = 1;
}
/* ---------------------------------------------------------------------------*/
/**
 * @brief scrollviewNotify 
 *
 * @param hwnd
 * @param id
 * @param nc
 * @param add_data
 */
/* ---------------------------------------------------------------------------*/
static void scrollviewNotify(HWND hwnd, int id, int nc, DWORD add_data)
{
	if (nc != SVN_CLICKED)
		return;
	int idx = SendMessage (hScrollView, SVM_GETCURSEL, 0, 0);
	struct ScrollviewItem *plist;
	plist = (struct ScrollviewItem *)SendMessage (hScrollView, SVM_GETITEMADDDATA, idx, 0);

	if (!plist)
		return;
	if (!plist->callback)
		return;
	if (plist->item_type == 0 && g_config.auto_sync_time == 0) {
		flag_timer_stop = 1;
		plist->callback(hwnd,enableAutoClose);
	} else if (plist->item_type == 1){
		plist->switch_state ^= 1;
		plist->callback(plist->switch_state,NULL);
		InvalidateRect (hwnd, NULL, TRUE);
	}
}

static void myDrawItem (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
#define FILL_BMP_STRUCT(left,top,img)  \
	FillBoxWithBitmap(hdc,left, top,img.bmWidth,img.bmHeight,&img)

#define DRAW_TABLE(rc,offset,color)  \
	do { \
		SetPenColor (hdc, color); \
		if (p_item->index) { \
			MoveTo (hdc, rc->left + offset, rc->top); \
			LineTo (hdc, rc->right,rc->top); \
		} \
		MoveTo (hdc, rc->left + offset, rc->bottom); \
		LineTo (hdc, rc->right,rc->bottom); \
	} while (0)

	struct ScrollviewItem *p_item = (struct ScrollviewItem *)scrollview_get_item_adddata (hsvi);
	SetBkMode (hdc, BM_TRANSPARENT);
	SetTextColor (hdc, PIXEL_lightwhite);
	SelectFont (hdc, font20);
	if (p_item->callback && p_item->item_type == 0)
		FILL_BMP_STRUCT(rcDraw->left + 968,rcDraw->top + 15,bmp_enter);
	if (p_item->item_type == 1) {
		if (p_item->switch_state)
			FILL_BMP_STRUCT(rcDraw->left + 968,rcDraw->top + 15,image_swich_on);
		else
			FILL_BMP_STRUCT(rcDraw->left + 968,rcDraw->top + 15,image_swich_off);
	}
	// 绘制表格
	DRAW_TABLE(rcDraw,0,0xCCCCCC);
	if (p_item->text_color_type)
		SetTextColor (hdc, RGBA2Pixel (hdc, 0xCC, 0xCC, 0xCC, 0xFF));
	else
		SetTextColor (hdc, RGBA2Pixel (hdc, 0x99, 0x99, 0x99, 0xFF));
	// 输出文字
	TextOut (hdc, rcDraw->left + 30, rcDraw->top + 15, p_item->title);
	RECT rc;
	memcpy(&rc,rcDraw,sizeof(RECT));
	rc.left += 512;
	rc.right -= 70;
	DrawText (hdc,p_item->text, -1, &rc,
			DT_VCENTER | DT_RIGHT | DT_WORDBREAK  | DT_SINGLELINE);
}
/* ----------------------------------------------------------------*/
/**
 * @brief initPara 初始化参数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 */
/* ----------------------------------------------------------------*/
static void initPara(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	int i;
    for (i=0; ctrls_title[i].idc != 0; i++) {
        ctrls_title[i].font = font20;
        createMyTitle(hDlg,&ctrls_title[i]);
    }
    for (i=0; ctrls_button[i].idc != 0; i++) {
        ctrls_button[i].font = font22;
        createMyButton(hDlg,&ctrls_button[i]);
    }
	hScrollView = GetDlgItem (hDlg, IDC_SCROLLVIEW);
	SendMessage (hScrollView, SVM_SETITEMDRAW, 0, (LPARAM)myDrawItem);
	reloadTimer();
}

/* ----------------------------------------------------------------*/
/**
 * @brief formSettingTimerProc 窗口回调函数
 *
 * @param hDlg
 * @param message
 * @param wParam
 * @param lParam
 *
 * @return
 */
/* ----------------------------------------------------------------*/
static int formSettingTimerProc(HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch(message) // 自定义消息
    {
		case MSG_TIMER:
			{
				if (flag_timer_stop)
					return 0;
			} break;

		case MSG_COMMAND:
			{
				int id = LOWORD (wParam);
				int code = HIWORD (wParam);
				scrollviewNotify(hDlg,id,code,0);
				break;
			}
		case MSG_ENABLE_WINDOW:
			enableAutoClose();
			break;
		case MSG_DISABLE_WINDOW:
			flag_timer_stop = 1;
			break;
        default:
            break;
    }
	if (form_base->baseProc(form_base,hDlg, message, wParam, lParam) == FORM_STOP)
		return 0;
    return DefaultDialogProc(hDlg, message, wParam, lParam);
}

int createFormSettingTimer(HWND hMainWnd,void (*callback)(void))
{
	HWND Form = Screen.Find(form_base_priv.name);
	if(Form) {
		Screen.setCurrent(form_base_priv.name);
		reloadTimer();
		ShowWindow(Form,SW_SHOWNORMAL);
	} else {
        if (bmp_load_finished == 0) {
            return 0;
        }
		form_base_priv.callBack = callback;
		form_base = formBaseCreate(&form_base_priv);
		return CreateMyWindowIndirectParam(form_base->priv->dlgInitParam,
				hMainWnd, form_base->priv->dlgProc, 0);
	}

	return 0;
}

