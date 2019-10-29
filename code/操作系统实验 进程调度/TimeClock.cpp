#include "TimeClock.h"

TimeClock::TimeClock()
{
	if_break = FALSE;
}

void TimeClock::BeginCreate()
{
	thread create_break(&TimeClock::CreateBreak, this);		//初始化多线程
	create_break.detach();									//并行执行t1
}

void TimeClock::CreateBreak()
{
	while (1)
	{
		Sleep(BREAK_TIME_LENGTH);		//等待时间
		if_break = TRUE;				//产生中断
	}
}

void TimeClock::ResetBreak()
{
	if_break = FALSE;
}

bool TimeClock::GetIfBreak()
{
	return if_break;
}