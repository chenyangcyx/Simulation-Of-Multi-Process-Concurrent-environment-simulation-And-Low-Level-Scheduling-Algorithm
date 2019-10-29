#pragma once
#include "OverallData.h"
#include "PCB.h"
#include "FileOperation.h"
#include <queue>
#include <iostream>
#include <string>
using namespace std;

class ProcessSchedule
{
private:
	queue<PCB>running_queue;		//运行队列
	queue<PCB>ready_queue;			//就绪队列
	queue<PCB>wait_queue;			//等待队列
public:
	ProcessSchedule();				//默认构造函数
	void ValueReset();				//重新设置初始化参数
	void JoinRunningQueue(PCB&, FileOperation &, int);	//加入运行队列
	void JoinReadyQueue(PCB&, FileOperation &, int);	//加入就绪队列
	void JoinWaitQueue(PCB&, FileOperation &, int);		//加入等待队列
	void OutReadyQueue(int);		//将特定序号的PCB从就绪队列中移除
	void OutRunningQueue(int);		//将特定序号的PCB从就绪队列中移除
	void OutWaitQueue(int);			//将特定序号的PCB从就绪队列中移除
	int GetReadyQueueSize();		//获取就绪态队列长度
	int GetRunningQueueSize();		//获取就绪态队列长度
	int GetWaitQueueSize();			//获取就绪态队列长度
	PCB& GetReadyQueueOnlyID();		//获取就绪态仅有的唯一的元素
	void RunningQueuePop();			//运行态序列弹出
	int GetRunningQueueFirstID();	//取得当前运行队列中的进程的ProID

	string ShowRunningQueue();		//展示运行队列的所有进程编号
	string ShowReadyQueue();		//展示就绪队列的所有进程编号
	string ShowWaitQueue();			//展示等待队列的所有进程编号
	string GetAllRunningQueue();	//获得运行队列的所有进程编号
	string GetAllReadyQueue();		//获得就绪队列的所有进程编号
	string GetAllWaitQueue();		//获得等待队列的所有进程编号

	PCB& GetRunningPCB();			//获取当前运行队列中的PCB
	bool IfExistRunningPCB();		//查看当前运行队列中是否有正在运行的进程
};