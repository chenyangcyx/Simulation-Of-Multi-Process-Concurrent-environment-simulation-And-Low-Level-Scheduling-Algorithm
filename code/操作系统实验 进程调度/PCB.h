#pragma once
#include "OverallData.h"

class PCB
{
private:
	int ProID;							//进程编号
	int Priority;						//进程优先数
	int InTimes;						//进程创建时间
	int ProState;						//进程状态（就绪、运行、等待）
	int RunTimes;						//进程需要运行时间
	int InstrucNum;						//进程中包含的指令数目
	int PSW;							//程序状态字，进程当前执行的指令编号
	PCB_Instruc Instruc[MAX_INSTRUC_NUM];		//指令内容

	bool if_used;						//该PCB在系统PCB表中是否被用
	bool if_end;						//PCB是否已经执行结束
	int start_time;						//进程的实际开始时间
	int end_time;						//进程的实际结束时间
	int current_use_time;				//进程已经运行的时间（已经占用CPU的时间）

	int p_pri;							//Unix调度算法需要变量
	int p_cpu;							//Unix调度算法需要变量

	int wait_state_time;				//已经处于阻塞队列的时长
public:
	PCB();					//默认构造函数
	void InitInstruc();		//初始化指令内容数组
	void InitPCB();			//初始化PCB内容
	int GetProID();			//获取进程编号
	int GetPriority();		//获取进程优先数
	int GetInTimes();		//获取进城创建时间
	int GetProState();		//获取进程状态
	int GetRunTimes();		//获取进程运行时间
	int GetInstrucNum();	//获取进程中包含的指令数目
	int GetPSW();			//获取程序状态字，进程当前执行的指令编号
	int GetInstrucID(int);			//获取指定的指令的编号
	int GetInstrucState(int);		//获取指定的指令的状态标志
	int GetInstrucTimes(int);		//获取指定的指令的运行时间
	int GetCurrentRunTime();		//获取已经运行的时间
	int Get_P_PRI();				//获取p_pri
	void SetProID(int);				//设置进程编号
	void SetPriority(int);			//设置进程优先数
	void SetInTimes(int);			//设置进城创建时间
	void SetProState(int);			//设置进程状态
	void SetRunTimes(int);			//设置进程运行时间
	void SetInstrucNum(int);		//设置进程中包含的指令数目
	void SetPSW(int);				//设置程序状态字，进程当前执行的指令编号
	void SetInstrucID(int, int);		//设置指定的指令的编号
	void SetInstrucState(int, int);		//设置指定的指令的状态标志
	void SetInstrucTimes(int, int);		//设置指定的指令的运行时间
	void SetCurrentTime(int);			//设置已经运行的时间

	void ProcessCreate();			//进程控制原语——进程创建
	void ProcessCancel(int);		//进程控制原语——进程撤销
	void ProcessBlocked();			//进程控制原语——进程阻塞
	void ProcessWakeUp();			//进程控制原语——进程唤醒

	bool GetIfUsed();				//返回PCB是否被用
	bool GetIfEnd();				//返回PCB是否已经结束
	int GetStartTime();				//返回PCB真正开始时间
	int GetEndTime();				//返回PCB结束的时间
	void SetStartTime(int);			//设置PCB真正开始的时间

	void P_CPU_Add();				//p_cpu+1
	void PCPU_DECAY();				//p_cpu参数的衰减
	void Cal_P_PRI();				//计算该PCB的p_pri参数

	void InitWaitTime();			//初始化等待状态时间
	void WaitTimeSelfAdd();			//等待时间自增
	int GetWaitTime();				//返回已经处于等待态的时间
};