#pragma once

#include<vector> 
using namespace std;

class CGame
{
	const int CELL = 20;  //方块的边长
	const int ROWS = 25;  //方块的行数
	const int COLS = 15;//方块的列数
						//升级所需分数值
	const int SCORE_LEVEL_INC = 80;
	//定时器ID
	const int ID_TIMER = 1;
	const int INTERVAL_UNIT = 25;
	const int INTERVAL_BASE = 300;
public:
	CGame(void);
	virtual ~CGame(void);

	//初始化游戏
	bool Init(HWND hWnd);
	//界面更新
	void Paint(HDC hdc);

	void DoDownShift(void);    //下移
	void DoLeftShift(void);    //左移
	void DoRightShift();       //右移
	void DoAccelerate();       //加速
	void DoRedirection();      //改变方向

	void SetPause(void);       //设置暂停
	void SetShowEdit(HWND hWnd);






protected:
	void DrawPanel(void);  //绘制游戏面板
	void ClearRow();       //消行
	bool CreateBlock();    //输出方块
	bool IsTouchBottom();
	void RefreshPanel(void);//刷新面板

							//返回界面的宽度
	int GetUIWidth(void) const;
	//返回界面的高度
	int GetUIHeight(void) const;

	void SetTimer(UINT  nTimer)
	{
		m_nTimer = nTimer;
	}
	UINT GetTimer(void) const
	{
		return m_nTimer;
	}
	void SetHDC(HDC hDC)
	{
		m_hDC = hDC;
	}
	void Setwnd(HWND hWnd)
	{
		m_hWnd = hWnd;
	}
	int GetDownVal(void) const;
	void ShowScore(void);

private:
	HDC m_hDC;             //窗口设备句柄
	HWND m_hWnd;
	HWND m_hWndEdit;       //显示分数和级别
	UINT  m_nTimer;         //保存计时器ID
	int  m_nScore;         //分数
	int m_nLevel;          //级数

	int m_nCurLeft;        //左端坐标 记录方块当前的位置
	int m_nCurTop;         //顶端坐标

	int m_nGoWidth;        //正在下落的方块的宽带和高度
	int m_nGoHeight;
	bool m_bIsPause;       //暂停标识

	vector< vector<BYTE> > m_vPanel;//每一格的状态
	vector<BYTE> m_vBlock;//大方块，方块为随机大小
};