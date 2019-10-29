#include "TaskRequest.h"

TaskRequest::TaskRequest()
{
	ValueReset();
}

void TaskRequest::ValueReset()
{
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
		if_PCB_used[i] = false;
	next_PCB_no = 1;
	InitPCBNum();
}

bool TaskRequest::IfExistAvailablePCB()
{
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		if (system_PCB[i].GetIfUsed() == false)
			return true;
	}
	return false;
}

void TaskRequest::RefreshPCB(PCB data)
{
	last_one = data;
}

PCB TaskRequest::CreateNewPCB()
{
	PCB temp;
	temp.SetProID(PCB_num++);						//设置PCB编号
	srand((unsigned int)__rdtsc());					//设置随机数种子
	temp.SetPriority(rand() % MAX_PRIORITY_NUM);	//生成0-5的随即优先数
	if (PCB_num - 1 == 1)							//使用第一个PCB，没有上一个
		temp.SetInTimes(0);
	else											//使用的不是第一个PCB，则有上一个PCB可供参考
	{
		temp.SetInTimes(last_one.GetInTimes() + ((rand() % (MAX_INTIMES_INTERVAL - MIN_INTIMES_INTERVAL + 1)) + MIN_INTIMES_INTERVAL)*10);
	}
	temp.SetInstrucNum((rand() % (MAX_INSTRUC_NUM - MIN_INSTRUC_NUM + 1)) + MIN_INSTRUC_NUM);		//设置指令个数
	temp.SetPSW(1);
	int all_time = 0;
	for (int i = 0; i < temp.GetInstrucNum(); i++)
	{
		temp.SetInstrucID(i, i + 1);
		temp.SetInstrucState(i, rand() % 3);
		temp.SetInstrucTimes(i, ((rand() % (MAX_INSTRUC_TIMES - MIN_INSTRUC_TIMES + 1)) + MIN_INSTRUC_TIMES) * 10);
		if (temp.GetInstrucState(i) == 2)
			temp.SetInstrucTimes(i, PV_INSTRUCTION_TIME);
		all_time += temp.GetInstrucTimes(i);
	}
	temp.SetRunTimes(all_time);
	last_one = temp;
	return temp;
}

void TaskRequest::InitPCBNum()
{
	PCB_num = 1;
}

bool TaskRequest::IfAllOK()
{
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		if ((system_PCB[i].GetIfEnd() == false) && (system_PCB[i].GetIfUsed()==true))
			return false;
	}
	return true;
}

PCB & TaskRequest::GetOnePCB(int data)
{
	return system_PCB[data];
}

void TaskRequest::SetOnePCB(int i, PCB data)
{
	system_PCB[i] = data;
}

int TaskRequest::CalMaxInTime()
{
	int max_intime = -1;
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		if (system_PCB[i].GetInTimes() >= max_intime)
		{
			max_intime = system_PCB[i].GetInTimes();
		}
	}
	return max_intime;
}

bool TaskRequest::TaskJoinCheck(int time,int& num)
{
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		if (time == system_PCB[i].GetInTimes())
		{
			num = i;
			return true;
		}
	}
	return false;
}

bool TaskRequest::CheckIfJoin(int no)
{
	if (system_PCB[no].GetIfUsed()==true)
		return true;
	else return false;
}

int TaskRequest::CheckPCBState(int no)
{
	return system_PCB[no].GetProState();
}
