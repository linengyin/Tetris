#include "StdAfx.h"
#include "Game.h"
#include <iostream>
#include <sstream>
#include<vector>
#include<time.h>

using namespace std;
CGame::CGame(void) :m_hDC(NULL), m_nScore(0), m_nLevel(0), m_bIsPause(false),
m_vPanel(ROWS, vector<BYTE>(COLS, 0))
{
}

CGame::~CGame(void)
{
	if (m_nTimer)
	{
		KillTimer(m_hWnd, ID_TIMER);
		m_nTimer = 0;
	}
}

//返回界面的宽度
int CGame::GetUIWidth(void) const
{
	return CELL*COLS + 8 * 40;
}

//返回界面的高度
int CGame::GetUIHeight(void) const
{
	return CELL*ROWS + 32 * 2;
}

bool CGame::Init(HWND hWnd)
{
	Setwnd(hWnd);
	::MoveWindow(m_hWnd, 400, 10, GetUIWidth(), GetUIHeight(), FALSE);

	srand((unsigned int)time(NULL));//为rand函数提供不同的种子值

	CreateBlock();
	m_nTimer = ::SetTimer(m_hWnd, ID_TIMER, GetDownVal(), NULL);

	ShowScore();
	return true;
}

void CGame::Paint(HDC hdc)//界面更新
{
	SetHDC(hdc);
	DrawPanel();
	RefreshPanel();
}

void CGame::DrawPanel(void)//绘制游戏面板
{
	RECT rcRect;

	for (int y = 0; y<ROWS; y++)
	{
		for (int x = 0; x<COLS; x++)
		{
			//计算方块的边框范围
			rcRect.top = y*CELL + 1;
			rcRect.left = x*CELL + 1;
			rcRect.right = (x + 1)*CELL - 1;
			rcRect.bottom = (y + 1)*CELL - 1;
			::FrameRect(m_hDC, &rcRect, (HBRUSH)GetStockObject(GRAY_BRUSH));//GRAY_BRUSH是灰色
		}
	}
}

void CGame::RefreshPanel(void)   //刷新面板
{
	RECT rect;
	if (m_vBlock.size() <= 0)
	{
		return;
	}

	HBRUSH h_bSolid = (HBRUSH)GetStockObject(BLACK_BRUSH);
	HBRUSH h_bEmpty = (HBRUSH)GetStockObject(WHITE_BRUSH);

	//先刷背景屏和固定的方块
	for (int y = 0; y<(int)m_vPanel.size(); y++)
	{
		for (int x = 0; x<(int)m_vPanel[0].size(); x++)
		{
			//为避免刷掉方块的边框，rect范围必须比边框范围小1
			rect.top = y*CELL + 2;
			rect.bottom = (y + 1)*CELL - 2;
			rect.left = x*CELL + 2;
			rect.right = (x + 1)*CELL - 2;
			if (m_vPanel[y][x])
			{
				FillRect(m_hDC, &rect, h_bSolid);
			}
			else
			{
				FillRect(m_hDC, &rect, h_bEmpty);
			}
		}
	}

	//显示正在落下的方块
	for (int y = 0; y<m_nGoHeight; y++)
	{
		for (int x = 0; x<m_nGoWidth; x++)
		{
			if (m_vBlock[y*m_nGoWidth + x])  //实心
			{
				rect.top = (y + m_nCurTop)*CELL + 2;
				rect.bottom = (y + m_nCurTop + 1)*CELL - 2;
				rect.left = (x + m_nCurLeft)*CELL + 2;
				rect.right = (x + m_nCurLeft + 1)*CELL - 2;
				FillRect(m_hDC, &rect, h_bSolid);
			}

		}
	}

}

bool CGame::CreateBlock()    //输出方块
{
	int nSel = rand() % 7;//7种形状
	switch (nSel)
	{
	case 0:   //水平条
		m_nGoWidth = 4;
		m_nGoHeight = 1;


		m_vBlock.resize(m_nGoWidth*m_nGoHeight);
		m_vBlock = { 1, 1, 1, 1 };



		//当前坐标位置
		m_nCurTop = 0 - m_nGoHeight;
		m_nCurLeft = (COLS - m_nGoWidth) / 2;
		break;
	case 1:   //三角
		m_nGoWidth = 3;
		m_nGoHeight = 2;

		m_vBlock.resize(m_nGoWidth*m_nGoHeight);
		m_vBlock = { 0,1,0,1,1,1 };

		//当前坐标位置
		m_nCurTop = 0 - m_nGoHeight;
		m_nCurLeft = (COLS - m_nGoWidth) / 2;
		break;
	case 2:   //左横折
		m_nGoWidth = 3;
		m_nGoHeight = 2;

		m_vBlock.resize(m_nGoWidth*m_nGoHeight);
		m_vBlock = { 1,0,0,1,1,1 };

		//当前坐标位置
		m_nCurTop = 0 - m_nGoHeight;
		m_nCurLeft = (COLS - m_nGoWidth) / 2;
		break;
	case 3:   //右横折
		m_nGoWidth = 3;
		m_nGoHeight = 2;

		m_vBlock.resize(m_nGoWidth*m_nGoHeight);
		m_vBlock = { 0,0,1,1,1,1 };

		//当前坐标位置
		m_nCurTop = 0 - m_nGoHeight;
		m_nCurLeft = (COLS - m_nGoWidth) / 2;
		break;
	case 4:   //左闪电
		m_nGoWidth = 3;
		m_nGoHeight = 2;

		m_vBlock.resize(m_nGoWidth*m_nGoHeight);
		m_vBlock = { 1,1,0,0,1,1 };

		//当前坐标位置
		m_nCurTop = 0 - m_nGoHeight;
		m_nCurLeft = (COLS - m_nGoWidth) / 2;
		break;
	case 5:   //右闪电
		m_nGoWidth = 3;
		m_nGoHeight = 2;

		m_vBlock.resize(m_nGoWidth*m_nGoHeight);
		m_vBlock = { 0,1,1,1,1,0 };

		//当前坐标位置
		m_nCurTop = 0 - m_nGoHeight;
		m_nCurLeft = (COLS - m_nGoWidth) / 2;
		break;
	case 6:   //正方形
		m_nGoWidth = 2;
		m_nGoHeight = 2;

		m_vBlock.resize(m_nGoWidth*m_nGoHeight);
		m_vBlock = { 1,1,1,1 };

		//当前坐标位置
		m_nCurTop = 0 - m_nGoHeight;
		m_nCurLeft = (COLS - m_nGoWidth) / 2;
		break;

	}
	return m_vBlock.size()>0;
}

bool CGame::IsTouchBottom()
{
	if (m_vBlock.size() <= 0)
	{
		return false;
	}

	//是否到达底部
	if (ROWS == m_nCurTop + m_nGoHeight)
	{
		//固定方块
		for (int i = 0; i<m_nGoHeight; i++)
		{
			for (int j = 0; j<m_nGoWidth; j++)
			{
				if (m_vBlock[i*m_nGoWidth + j])
				{
					m_vPanel[m_nCurTop + i][m_nCurLeft + j] = 1;
				}
			}
		}

		return true;
	}

	for (int y = m_nGoHeight - 1; y >= 0; y--)//从底行开始扫描
	{
		//判断第一个实心方块在面板上邻接的下方方格是否为实心，是就代表已经到达底部
		for (int x = 0; x<m_nGoWidth; x++)
		{
			if (!m_vBlock[y*m_nGoWidth + x])
			{
				//空的方块不用判断。
				continue;
			}
			if (m_nCurTop + y + 1<0)
			{
				return false;
			}
			if (!m_vPanel[m_nCurTop + y + 1][m_nCurLeft + x])
			{
				//底部为空的方块也不用判断
				continue;
			}

			//判断是否gameover
			if (m_nCurTop <= 0)
			{
				if (m_nTimer)
				{
					KillTimer(m_hWnd, ID_TIMER);
					m_nTimer = 0;
				}
				MessageBox(m_hWnd, TEXT("游戏结束"), TEXT("提示"), MB_OK | MB_ICONEXCLAMATION);
				SendMessage(m_hWnd, WM_CLOSE, 0, 0);
				return true;
			}

			//固定方块
			for (int i = 0; i<m_nGoHeight; i++)
			{
				for (int j = 0; j<m_nGoWidth; j++)
				{
					if (m_vBlock[i*m_nGoWidth + j])
					{
						m_vPanel[m_nCurTop + i][m_nCurLeft + j] = 1;
					}
				}
			}
			return true;
		}
	}
	return false;
}

void CGame::ClearRow()      //消行
{
	int nCount = 0;//消行次数
	bool isFilled = false;
	//消行处理
	for (int i = ROWS - 1; i >= 0; i--)
	{
		isFilled = true;
		for (int j = 0; j<COLS; j++)
		{
			if (!m_vPanel[i][j])
			{
				isFilled = false;
				break;
			}
		}
		if (isFilled)
		{
			for (int j = 0; j<COLS; j++)
			{
				m_vPanel[i][j] = 0;
			}
			//所有方块往下移
			for (int k = i - 1; k >= 0; k--)
			{
				for (int j = 0; j<COLS; j++)
				{
					m_vPanel[k + 1][j] = m_vPanel[k][j];
				}
			}
			i = i + 1;
			nCount++;
		}
	}

	//最高级别为9级，所以分数为（9+1）*score_level_inc-1
	if (m_nScore >= 10 * SCORE_LEVEL_INC - 1)
	{
		return;
	}

	//加分规则：消除行数，1行加10分，2行加15分，3行加20分，4行加30分
	switch (nCount)
	{
	case 1:
		m_nScore += 10;
		break;
	case 2:
		m_nScore += 15;
		break;
	case 3:
		m_nScore += 20;
	case 4:
		m_nScore += 30;
		break;
	}
	int nTemp_level = m_nScore / SCORE_LEVEL_INC;
	if (nTemp_level>m_nLevel)
	{
		m_nLevel = nTemp_level;
		//撤销当前计时器，然后重设
		if (m_nTimer)
		{
			KillTimer(m_hWnd, ID_TIMER);
		}
		m_nTimer = ::SetTimer(m_hWnd, ID_TIMER, GetDownVal(), NULL);
	}
	std::ostringstream oss;
	oss << "Score:" << m_nScore << ",Level:" << m_nLevel << std::endl;
	OutputDebugStringA(oss.str().c_str());

	ShowScore();
}

void CGame::ShowScore(void)
{
	std::wostringstream woss;
	woss << L"Score:" << m_nScore << L",Level:" << m_nLevel << std::ends;
	SetWindowText(m_hWndEdit, woss.str().c_str());
}

void CGame::DoDownShift(void)//下移
{
	if (m_vBlock.size() <= 0)
	{
		return;
	}
	//判断是否到达底部
	if (IsTouchBottom())
	{
		//消行处理
		ClearRow();
		CreateBlock();//输出下一个方块
	}
	m_nCurTop++;
	RefreshPanel();
}

void CGame::DoLeftShift(void) //左移
{
	if (m_bIsPause)
	{
		return;
	}
	if (m_vBlock.size() <= 0)
	{
		return;
	}
	//已经到达最左边，不能左移
	if (m_nCurLeft == 0)
	{
		return;
	}
	if (m_nCurTop<0)
	{
		return;  //方块没有完整显示前，不能左移
	}

	//检测落下的方块与已经固定的方块是否有冲突
	for (int y = 0; y<m_nGoHeight; y++)
	{
		for (int x = 0; x<m_nGoWidth; x++)//从左边开始扫描，获取该行最左边实心方块
		{
			if (m_vBlock[y*m_nGoWidth + x])
			{
				//判断当前方格在面板上面左边一个方格是否为实心，是就代表不能左移
				if (m_vPanel[m_nCurTop + y][m_nCurLeft + x - 1])
				{
					return;
				}
				break; //只判断最左边的一个实心方格，之后直接扫描下一行
			}
		}
	}

	m_nCurLeft--;
	RefreshPanel();

}

void CGame::DoRightShift() //右移
{
	if (m_bIsPause)
	{
		return;
	}

	if (m_vBlock.size() <= 0)
	{
		return;
	}
	if (COLS - m_nGoWidth == m_nCurLeft)
	{
		return;
	}
	if (m_nCurTop < 0)
	{
		return;//方块完整显示前不能右移
	}
	for (int y = 0; y<m_nGoHeight; y++)
	{
		for (int x = m_nGoWidth - 1; x >= 0; x--)//从右边开始扫描，获取该行最右边实心方块
		{
			if (m_vBlock[y*m_nGoWidth + x])
			{
				//判断当前方格在面板上面右边一个方格是否为实心，是就代表不能左移
				if (m_vPanel[m_nCurTop + y][m_nCurLeft + x - 1])
				{
					return;
				}
				break; //只判断最右边的一个实心方格，之后直接扫描下一行
			}
		}
	}

	m_nCurLeft++;
	RefreshPanel();


}

void CGame::DoAccelerate()   //加速
{
	if (m_bIsPause)
	{
		return;
	}

	if (m_vBlock.size() <= 0)
	{
		return;
	}
	if (IsTouchBottom())
	{
		//消行处理
		ClearRow();
		CreateBlock();
	}
	m_nCurTop++;
	RefreshPanel();
}

void CGame::DoRedirection()//改变方向
{
	if (m_bIsPause)
	{
		return;
	}

	if (m_vBlock.size() <= 0)
	{
		return;
	}

	if (m_nCurTop<0)
	{
		return;//方块完整显示前不能转向
	}
	vector<BYTE> vTemp(m_nGoWidth*m_nGoHeight, 0);
	//旋转方块
	for (int i = 0; i<m_nGoWidth; i++)
	{
		for (int j = 0; j<m_nGoHeight; j++)
		{
			vTemp[i*m_nGoHeight + j] = m_vBlock[(m_nGoHeight - j - 1)*m_nGoWidth + i];
		}
	}
	//给方块重新定位
	int incHeight = m_nGoWidth - m_nGoHeight;
	int incWidth = m_nGoHeight - m_nGoWidth;

	//方块拟放置新的坐标
	int iTemp_cur_top = m_nCurTop - incHeight / 2;
	int iTemp_cur_left = m_nCurLeft - incWidth / 2;

	//判断当前空间是否足够让方块改变方向
	int max_len = max(m_nGoWidth, m_nGoHeight);
	//防止下标访问越界
	if (iTemp_cur_top + max_len - 1 >= ROWS || iTemp_cur_left<0 ||
		iTemp_cur_left + max_len - 1 >= COLS)
	{
		return;
	}
	//判断新的坐标下，是否可以放置方块
	for (int i = 0; i<max_len; i++)
	{
		for (int j = 0; j<max_len; j++)
		{
			//转向所需的空间内有已被占用的实心方格所在，即转向失败
			if (m_vPanel[iTemp_cur_top + i][iTemp_cur_left + j])
			{
				//不能放置
				return;
			}
		}
	}

	//把临时变量的值赋给block，只能赋值，而不能赋指针值
	for (int i = 0; i<m_nGoHeight; i++)
	{
		for (int j = 0; j<m_nGoWidth; j++)
		{
			m_vBlock[i*m_nGoWidth + j] = vTemp[i*m_nGoWidth + j];
		}
	}
	//方块的新坐标重新被赋值
	m_nCurTop = iTemp_cur_top;
	m_nCurLeft = iTemp_cur_left;

	//交换
	int iTemp = m_nGoWidth;
	m_nGoWidth = m_nGoHeight;
	m_nGoHeight = iTemp;

	RefreshPanel();
}

void CGame::SetPause(void)
{
	m_bIsPause = !m_bIsPause;
	if (m_bIsPause)
	{
		if (m_nTimer)
		{
			::KillTimer(m_hWnd, ID_TIMER);
		}
		m_nTimer = 0;
	}
	else
	{
		m_nTimer = ::SetTimer(m_hWnd, ID_TIMER, GetDownVal(), FALSE);
	}
}

int CGame::GetDownVal(void) const
{
	return INTERVAL_BASE - m_nLevel*INTERVAL_UNIT;
}

void CGame::SetShowEdit(HWND hWnd)
{
	m_hWndEdit = hWnd;
}