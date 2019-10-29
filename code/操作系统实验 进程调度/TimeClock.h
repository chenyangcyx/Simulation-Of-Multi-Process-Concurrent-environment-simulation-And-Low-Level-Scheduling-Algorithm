#pragma once
#include <Windows.h>
#include <thread>
#include "OverallData.h"
using namespace std;

class TimeClock
{
private:
	bool if_break;				//是否产生中断
public:
	TimeClock();				//默认构造函数
	void BeginCreate();			//开始产生中断信号与时间片信号
	void CreateBreak();			//循环产生中断
	void ResetBreak();			//重置中断信号
	bool GetIfBreak();			//获取当前中断状态
};