#include "PCB.h"

PCB::PCB()
{
	InitPCB();
	if_used = false;
	if_end = false;
	start_time = -1;
	end_time = -1;
	current_use_time = 0;
	p_pri = 0;
	p_cpu = 0;
	InitWaitTime();
}

void PCB::InitInstruc()
{
	for (int i = 0; i < MAX_INSTRUC_NUM; i++)
	{
		Instruc->Instruc_ID = INSTRUC_INIT_NUM;
		Instruc->Instruc_State = INSTRUC_INIT_NUM;
		Instruc->Instruc_Times = INSTRUC_INIT_NUM;
	}
}

void PCB::InitPCB()
{
	ProID = PCB_INIT_NUM;
	Priority = PCB_INIT_NUM;
	InTimes = PCB_INIT_NUM;
	ProState = PCB_INIT_NUM;
	RunTimes = PCB_INIT_NUM;
	InstrucNum = INSTRUC_STATE_PV_OPERATION;
	PSW = PCB_INIT_NUM;
	InitInstruc();
}

int PCB::GetProID()
{
	return this->ProID;
}

int PCB::GetPriority()
{
	return this->Priority;
}

int PCB::GetInTimes()
{
	return this->InTimes;
}

int PCB::GetProState()
{
	return this->ProState;
}

int PCB::GetRunTimes()
{
	return this->RunTimes;
}

int PCB::GetInstrucNum()
{
	return this->InstrucNum;
}

int PCB::GetPSW()
{
	return this->PSW;
}

int PCB::GetInstrucID(int no)
{
	return this->Instruc[no].Instruc_ID;
}

int PCB::GetInstrucState(int no)
{
	return this->Instruc[no].Instruc_State;
}

int PCB::GetInstrucTimes(int no)
{
	return this->Instruc[no].Instruc_Times;
}

int PCB::GetCurrentRunTime()
{
	return this->current_use_time;
}

int PCB::Get_P_PRI()
{
	return p_pri;
}

void PCB::SetProID(int data)
{
	this->ProID = data;
}

void PCB::SetPriority(int data)
{
	this->Priority = data;
}

void PCB::SetInTimes(int data)
{
	this->InTimes = data;
}

void PCB::SetProState(int data)
{
	this->ProState = data;
}

void PCB::SetRunTimes(int data)
{
	this->RunTimes = data;
}

void PCB::SetInstrucNum(int data)
{
	this->InstrucNum = data;
}

void PCB::SetPSW(int data)
{
	this->PSW = data;
}

void PCB::SetInstrucID(int no, int data)
{
	this->Instruc[no].Instruc_ID = data;
}

void PCB::SetInstrucState(int no, int data)
{
	this->Instruc[no].Instruc_State = data;
}

void PCB::SetInstrucTimes(int no, int data)
{
	this->Instruc[no].Instruc_Times = data;
}

void PCB::SetCurrentTime(int data)
{
	this->current_use_time = data;
}

void PCB::ProcessCreate()
{
	if_used = true;
	if_end = false;
}

void PCB::ProcessCancel(int time)
{
	if_used = true;
	if_end = true;
	this->end_time = time;
	this->SetProState(PCB_STATE_OVER);
}

void PCB::ProcessBlocked()
{
	this->ProState = PCB_STATE_WAIT;		//阻塞，变为等待态
}

void PCB::ProcessWakeUp()
{
	this->ProState = PCB_STATE_READY;	//唤醒，变为就绪态
}

bool PCB::GetIfUsed()
{
	return this->if_used;
}

bool PCB::GetIfEnd()
{
	return this->if_end;
}

int PCB::GetStartTime()
{
	return this->start_time;
}

int PCB::GetEndTime()
{
	return this->end_time;
}

void PCB::SetStartTime(int data)
{
	this->start_time = data;
}

void PCB::P_CPU_Add()
{
	if (GetIfUsed() == true && GetIfEnd() != true)
		p_cpu++;
}

void PCB::PCPU_DECAY()
{
	if (GetIfUsed() == true && GetIfEnd() != true)
		p_cpu = p_cpu / 2;
}

void PCB::Cal_P_PRI()
{
	p_pri = (p_cpu / 2) + GetPriority();
}

void PCB::InitWaitTime()
{
	wait_state_time = 0;
}

void PCB::WaitTimeSelfAdd()
{
	wait_state_time += BREAK_TIME_LENGTH;
}

int PCB::GetWaitTime()
{
	return wait_state_time;
}
