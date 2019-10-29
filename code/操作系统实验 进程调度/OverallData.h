#pragma once

constexpr int BREAK_TIME_LENGTH = 10;			//中断产生时间
constexpr int TIMESLICE_TIME_LENGTH = 1000;	//时间片产生时间

//PCB中的指令结构体
struct PCB_Instruc
{
	int Instruc_ID;			//指令编号
	int Instruc_State;		//指令类型
	int Instruc_Times;		//指令运行时间
};

//指令状态枚举体
enum INSTRUC_STATE
{
	INSTRUC_STATE_SYSTEMCALL,			//指令类型——系统调用
	INSTRUC_STATE_USERMOD_OPERATION,	//指令类型——用户态计算操作
	INSTRUC_STATE_PV_OPERATION			//指令类型——PV操作
};

//PCB状态枚举体
enum PCB_STATE
{
	PCB_STATE_READY,		//PCB状态——就绪态
	PCB_STATE_RUNNING,		//PCB状态——运行态
	PCB_STATE_WAIT,			//PCB状态——等待态
	PCB_STATE_OVER			//PCB状态——结束
};

constexpr int PCB_INIT_NUM = -1;		//PCB的初始化数值

constexpr int INSTRUC_INIT_NUM = -1;	//指令的初始化数值

constexpr int SYSTEM_MAX_PCB_NUM = 10;		//系统最大的PCB表的数量
constexpr int MAX_PRIORITY_NUM = 127;		//系统最大的优先数
constexpr int MAX_INTIMES_INTERVAL = 20;	//两个PCB产生之间的最大时间间隔
constexpr int MIN_INTIMES_INTERVAL = 10;	//两个PCB产生之间的最小时间间隔
constexpr int MAX_INSTRUC_NUM = 20;			//一个PCB中包含的最大的指令数目
constexpr int MIN_INSTRUC_NUM = 5;			//一个PCB中包含的最小的指令数目
constexpr int MAX_INSTRUC_TIMES = 4;		//一条指令最多需要运行的时间
constexpr int MIN_INSTRUC_TIMES = 1;		//一条指令最少需要运行的时间

enum CPU_STATE
{
	USER_MODE,		//用户态
	CORE_MODE		//核心态
};

constexpr int PV_INSTRUCTION_TIME = 50;		//PV操作指令的时间