#include "CPU.h"

CPU::CPU()
{
	ValueReset();
}

void CPU::ValueReset()
{
	PC = 0;
	IR.Instruc_ID = 0;
	IR.Instruc_State = 0;
	IR.Instruc_Times = 0;
	PSW = 0;
	current_time = 0;
	wait_time = 0;
	cpu_state = USER_MODE;
	if_could_go = true;
}

int CPU::GetPC()
{
	return this->PC;
}

PCB_Instruc CPU::GetIR()
{
	return this->IR;
}

int CPU::GetPSW()
{
	return this->PSW;
}

int CPU::GetCpuState()
{
	return this->cpu_state;
}

PCB & CPU::GetStorePCB()
{
	return this->store;
}

int CPU::GetWaitTime()
{
	return wait_time;
}

void CPU::SetPC(int data)
{
	this->PC = data;
}

void CPU::SetIR(PCB_Instruc data)
{
	this->IR = data;
}

void CPU::SetPSW(int data)
{
	this->PSW = data;
}

void CPU::SetCpuState(int data)
{
	this->cpu_state = data;
}

void CPU::SetStorePCB(PCB &data)
{
	this->store = data;
}

void CPU::SetWaitTime(int data)
{
	wait_time = data;
}

void CPU::WaitTimeSelfAdd()
{
	wait_time += BREAK_TIME_LENGTH;
}

void CPU::RefreshPSW()
{
	int i =  0, sum = 0;
	while (sum < current_time)
	{
		sum += store.GetInstrucTimes(i);
		i++;
	}
	PSW = (i == 0 ? 1 : i);
}

void CPU::RecoverSpot(PCB& data)
{
	SetStorePCB(data);
	this->PC = store.GetProID();
	this->PSW = store.GetPSW();
	PCB_Instruc temp;
	temp.Instruc_ID = store.GetInstrucID(PSW);
	temp.Instruc_State = store.GetInstrucState(PSW);
	temp.Instruc_Times = store.GetInstrucTimes(PSW);
	this->IR = temp;
	this->current_time = store.GetCurrentRunTime();
}

void CPU::ProtectSpot(PCB& data)
{
	data.SetPSW(this->PSW);
	data.SetCurrentTime(this->current_time);
}

void CPU::SetUserState()
{
	this->cpu_state = USER_MODE;
}

void CPU::SetCoreState()
{
	this->cpu_state = CORE_MODE;
}

void CPU::CurrentRunTimeSelfAdd(int cpu_time,FileOperation &fo)
{
	string str = "";
	CheckIfCouldGo(cpu_time);			//检查是否可以继续前进
	if (if_could_go == true)
	{
		current_time += BREAK_TIME_LENGTH;
		RefreshPSW();
		UpdateCPUMode();
	}
	else
	{
		wait_time += BREAK_TIME_LENGTH;		//CPU等待时间增加
		str = str + "○当前cpu时间：" + to_string(cpu_time) + "；距离下一个时间片的剩余时间不足！进程" + to_string(PC) + "无法继续执行下一条指令！\n";
		cout << str;
		fo.WriteResults(str);
	}
}

void CPU::RefreshOverRunTime()
{
	if (current_time >= store.GetRunTimes())
		current_time = store.GetRunTimes();
}

bool CPU::CheckIfOK()
{
	if (current_time >= store.GetRunTimes())
		return true;
	else return false;
}

bool CPU::CheckIfBorder()
{
	if (current_time == 0 && PSW == 1)
		return true;
	for (int i = 1, sum = 0; i <= store.GetInstrucNum(); i++)
	{
		sum += store.GetInstrucTimes(i - 1);
		if (current_time == sum)
			return true;
	}
	return false;
}

void CPU::CheckIfCouldGo(int cpu_time)
{
	int next_timeslice = (cpu_time / 1000 + 1) * 1000;		//下个时间片到来的时间
	int remain_time = next_timeslice - cpu_time;		//现在时间到下个时间片到来时间的剩余时间
	if (CheckIfBorder() == true)
	{
		if (current_time == 0 && PSW == 1)					//第一条指令的特殊情况
		{
			if (store.GetInstrucTimes(0) > remain_time)
				if_could_go = false;
			else
				if_could_go = true;
		}
		if (current_time != 0)
		{
			if (store.GetInstrucTimes(PSW) > remain_time)
				if_could_go = false;
			else
				if_could_go = true;
		}
	}
}

int CPU::UpdateCPUMode()
{
	switch (store.GetInstrucState(PSW))
	{
	case INSTRUC_STATE_SYSTEMCALL:
		this->cpu_state = CORE_MODE;		//核心态
		break;
	case INSTRUC_STATE_USERMOD_OPERATION:
		this->cpu_state = USER_MODE;		//用户态
		break;
	case INSTRUC_STATE_PV_OPERATION:
		this->cpu_state = CORE_MODE;		//核心态
		break;
	default:
		break;
	}
	return this->cpu_state;
}
