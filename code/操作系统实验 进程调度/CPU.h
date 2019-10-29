#pragma once
#include "OverallData.h"
#include "PCB.h"
#include "FileOperation.h"
#include <iostream>
#include <string>
using namespace std;

class CPU
{
private:
	int PC;							//程序计数器，当前正在运行的进程序号
	PCB_Instruc IR;					//指令寄存器，当前正在运行的指令状态
	int PSW;						//状态寄存器，当前正在运行的指令编号
	int current_time;				//指令已经运行的时间
	int cpu_state;					//CPU的状态
	int wait_time;					//CPU的等待时间
	PCB store;						//传入到CPU运行的PCB

	bool if_could_go;				//CPU是否可以自增的标识
public:
	CPU();							//默认构造函数
	void ValueReset();				//重新设置初始化参数
	int GetPC();					//获取PC程序计数器
	PCB_Instruc GetIR();			//获取IR指令寄存器
	int GetPSW();					//获取PSW状态寄存器
	int GetCpuState();				//获取CPU的状态
	PCB& GetStorePCB();				//获取当前存储的PCB
	int GetWaitTime();				//获取等待时间
	void SetPC(int);				//设置PC程序计数器
	void SetIR(PCB_Instruc);		//设置IR指令寄存器
	void SetPSW(int);				//设置PSW状态寄存器
	void SetCpuState(int);			//设置CPU的状态
	void SetStorePCB(PCB&);			//设置当前存储的PCB
	void SetWaitTime(int);			//设置等待时间
	void WaitTimeSelfAdd();			//等待时间自增
	void RefreshPSW();				//刷新PSW状态

	void RecoverSpot(PCB&);			//恢复现场
	void ProtectSpot(PCB&);			//保护现场

	void SetUserState();			//CPU切换成用户态
	void SetCoreState();			//CPU切换成核心态

	void CurrentRunTimeSelfAdd(int, FileOperation&);	//已经运行时间自增
	void RefreshOverRunTime();			//恢复已经超出的实际运行时间
	bool CheckIfOK();					//查看进程是否已经运行完成
	bool CheckIfBorder();				//进程是否运行到了时间边界
	void CheckIfCouldGo(int);			//检查是否可以运行下一条指令
	int UpdateCPUMode();				//更新CPU的当前模式，用户态、核心态
};