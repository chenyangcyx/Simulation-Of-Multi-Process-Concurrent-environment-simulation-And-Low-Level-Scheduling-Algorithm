#pragma once
#include "OverallData.h"
#include "FileOperation.h"
#include "PCB.h"
#include "ProcessSchedule.h"
#include "TaskRequest.h"
#include "TimeClock.h"
#include "CPU.h"
#include <iostream>
#include <string>
#include <tchar.h>
using namespace std;

class Control
{
private:
	FileOperation fo;
	TaskRequest tr;
	ProcessSchedule ps;
	TimeClock tc;
	CPU cp;
	int cpu_current_time;
	bool if_close_timeslice_break;	//是否关闭时间片中断
	int last_priority_ID;			//上一次位于运行态的进程的序号
	int break_time_num;				//中断——时间片计数
	int time_slice_num;				//时间片计数
	int receive_break;				//实际收到的中断计数
public:
	Control();						//默认构造函数
	void ValueReset();				//重新设置初始化参数
	void MenuShow();				//显示菜单选项
	void ChoiceOperation(int);		//对应的选项操作
	void AnyKeyToBackToMenu();		//询问是否返回主菜单
	void ReCreateAllPCB();			//重新生成全部的PCB文件并写入到文件
	void UnixDispatch();			//Unix新进程调度算法实现
	void CloseTimeSliceBreak();		//关时间片中断
	void OpenTimeSliceBreak();		//开时间片中断
	int GetMaxPriorityProcessID();	//通过p-pri比较，求得p-pri最小，即优先权最高的进程ID
	string GetOnePCBInfo_TimeSlice(int);	//一次时间片下，获取某一个PCB进程的详细信息
	void TaskRequestThread();				//任务请求模块 线程
	void TimeSliceThread();					//时间片调度模块 线程
	void PCBRunThread();					//进程运行模块 线程
	void AllPCBInfoShow_TimeSlice();		//一次时间片下，各进程状态输出
	void ShowCurrentTimeSlice();			//显示当前时间片时间
	void RefreshCpuTime();					//刷新当前cpu时间
	void LastTimeSliceDone();				//最后一个时间片的善后处理
	void LastShowAllInfo();					//当进程运行完成时，输出全部的详细信息
	void ShowSavedFileInfo();				//显示保存文件的信息
	void CheckIfCloseTimeSlice(PCB&);		//检测是否需要关闭
	void PVOperation(PCB&);					//2型指令的操作——进入阻塞队列、重新导入、50ms的操作
	void PVOperationTimeAdd();				//处于等待态的PCB进程当前等待时间自增
	void CheckIfPVOperationOK();			//检查所有的PCB中，等待时间是否已经结束
	void PVProcessToReady(PCB&);			//将等待态的PCB进程放入就绪态
	void ShowEveryTimeDetail();				//显示每个时间点的详细信息
};