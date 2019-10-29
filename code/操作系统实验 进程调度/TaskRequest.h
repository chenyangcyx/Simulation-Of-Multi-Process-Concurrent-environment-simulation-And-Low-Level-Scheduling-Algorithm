#pragma once
#include "OverallData.h"
#include "PCB.h"
#include <time.h>
#include <cstdlib>
#include <Windows.h>

class TaskRequest
{
private:
	PCB system_PCB[SYSTEM_MAX_PCB_NUM];				//系统内可用的PCB表
	bool if_PCB_used[SYSTEM_MAX_PCB_NUM];			//10个PCB表是否被占用
	int next_PCB_no;				//下一个将要使用的PCB编号
	PCB last_one;					//上一个产生的PCB
	int PCB_num;					//生成的PCB的编号
public:
	TaskRequest();					//默认构造函数
	void ValueReset();				//重新设置初始化参数
	bool IfExistAvailablePCB();		//是否存在可用的PCB
	void RefreshPCB(PCB);			//更新PCB last_one
	PCB CreateNewPCB();				//生成新的PCB文件
	void InitPCBNum();				//重置PCB的编号
	bool IfAllOK();					//判断所有的PCB是否都已经完成
	PCB& GetOnePCB(int);			//获取某一个具体的PCB
	void SetOnePCB(int, PCB);		//设置某一个具体的PCB
	int CalMaxInTime();				//计算PCB中最大的InTime
	bool TaskJoinCheck(int,int&);	//作业进入的实时检查
	bool CheckIfJoin(int);			//检查某一个PCB是否被使用
	int CheckPCBState(int);			//检查某一个PCB的当前状态
};