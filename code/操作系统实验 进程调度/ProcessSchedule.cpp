#include "ProcessSchedule.h"

ProcessSchedule::ProcessSchedule()
{
	ValueReset();
}

void ProcessSchedule::ValueReset()
{
	int s1 = (int)running_queue.size();
	int s2 = (int)ready_queue.size();
	int s3 = (int)wait_queue.size();
	for (int i = 1; i <= s1; i++)
		running_queue.pop();
	for (int i = 1; i <= s2; i++)
		ready_queue.pop();
	for (int i = 1; i <= s3; i++)
		wait_queue.pop();
}

void ProcessSchedule::JoinRunningQueue(PCB &data, FileOperation &fo, int cpu_time)
{
	bool if_exist = false;
	for (int i = 1; i <= running_queue.size(); i++)
	{
		PCB &temp = running_queue.front();
		if (temp.GetProID() == data.GetProID())
		{
			if_exist = true;
			break;
		}
		running_queue.pop();
		running_queue.push(temp);
	}
	if (if_exist == false)
	{
		running_queue.push(data);
		data.SetProState(PCB_STATE_RUNNING);
		string str = "";
		str = str + "◢cpu时间：" + to_string(cpu_time) + "，进程" + to_string(data.GetProID()) + "调入运行队列！\n";
		cout << str;
		fo.WriteResults(str);
	}
}

void ProcessSchedule::JoinReadyQueue(PCB &data, FileOperation &fo, int cpu_time)
{
	bool if_exist = false;
	for (int i = 1; i <= ready_queue.size(); i++)
	{
		PCB &temp = ready_queue.front();
		if (temp.GetProID() == data.GetProID())
		{
			if_exist = true;
			break;
		}
		ready_queue.pop();
		ready_queue.push(temp);
	}
	if (if_exist == false)
	{
		ready_queue.push(data);
		data.SetProState(PCB_STATE_READY);
		string str = "";
		str = str + "◣cpu时间：" + to_string(cpu_time) + "，进程" + to_string(data.GetProID()) + "调入就绪队列！\n";
		cout << str;
		fo.WriteResults(str);
	}
}

void ProcessSchedule::JoinWaitQueue(PCB &data, FileOperation &fo, int cpu_time)
{
	bool if_exist = false;
	for (int i = 1; i <= wait_queue.size(); i++)
	{
		PCB &temp = wait_queue.front();
		if (temp.GetProID() == data.GetProID())
		{
			if_exist = true;
			break;
		}
		wait_queue.pop();
		wait_queue.push(temp);
	}
	if (if_exist == false)
	{
		wait_queue.push(data);
		data.SetProState(PCB_STATE_WAIT);
		string str = "";
		str = str + "◥cpu时间：" + to_string(cpu_time) + "，进程" + to_string(data.GetProID()) + "调入等待队列！\n";
		cout << str;
		fo.WriteResults(str);
	}
}

void ProcessSchedule::OutReadyQueue(int out_id)
{
	for (int i = 1; i <= ready_queue.size(); i++)
	{
		PCB &temp = ready_queue.front();
		ready_queue.pop();
		if (temp.GetProID() != out_id + 1)
			ready_queue.push(temp);
	}
}

void ProcessSchedule::OutRunningQueue(int out_id)
{
	for (int i = 1; i <= running_queue.size(); i++)
	{
		PCB &temp = running_queue.front();
		running_queue.pop();
		if (temp.GetProID() != out_id + 1)
			running_queue.push(temp);
	}
}

void ProcessSchedule::OutWaitQueue(int out_id)
{
	for (int i = 1; i <= wait_queue.size(); i++)
	{
		PCB &temp = wait_queue.front();
		wait_queue.pop();
		if (temp.GetProID() != out_id + 1)
			wait_queue.push(temp);
	}
}

int ProcessSchedule::GetReadyQueueSize()
{
	return (int)ready_queue.size();
}

int ProcessSchedule::GetRunningQueueSize()
{
	return (int)running_queue.size();
}

int ProcessSchedule::GetWaitQueueSize()
{
	return (int)wait_queue.size();
}

PCB& ProcessSchedule::GetReadyQueueOnlyID()
{
	return ready_queue.front();
}

void ProcessSchedule::RunningQueuePop()
{
	running_queue.pop();
}

int ProcessSchedule::GetRunningQueueFirstID()
{
	if (running_queue.size() > 0)
	{
		PCB temp = running_queue.front();
		return temp.GetProID() - 1;
	}
	else return PCB_INIT_NUM;
}

string ProcessSchedule::ShowRunningQueue()
{
	string str = "";
	str += "运行队列中的进程序号：";
	for (int i = 1; i <= running_queue.size(); i++)
	{
		PCB &temp = running_queue.front();
		str += to_string(temp.GetProID()) + "  ";
		running_queue.pop();
		running_queue.push(temp);
	}
	str += "\n";
	cout << str;
	return str;
}

string ProcessSchedule::ShowReadyQueue()
{
	string str = "";
	str += "就绪队列中的进程序号：";
	for (int i = 1; i <= ready_queue.size(); i++)
	{
		PCB &temp = ready_queue.front();
		str += to_string(temp.GetProID()) + "  ";
		ready_queue.pop();
		ready_queue.push(temp);
	}
	str += "\n";
	cout << str;
	return str;
}

string ProcessSchedule::ShowWaitQueue()
{
	string str = "";
	str += "等待队列中的进程序号：";
	for (int i = 1; i <= wait_queue.size(); i++)
	{
		PCB &temp = wait_queue.front();
		str += to_string(temp.GetProID()) + "  ";
		wait_queue.pop();
		wait_queue.push(temp);
	}
	str += "\n";
	cout << str;
	return str;
}

string ProcessSchedule::GetAllRunningQueue()
{
	string str = "";
	for (int i = 1; i <= running_queue.size(); i++)
	{
		PCB &temp = running_queue.front();
		str += to_string(temp.GetProID()) + "  ";
		running_queue.pop();
		running_queue.push(temp);
	}
	str += "\n";
	return str;
}

string ProcessSchedule::GetAllReadyQueue()
{
	string str = "";
	for (int i = 1; i <= ready_queue.size(); i++)
	{
		PCB &temp = ready_queue.front();
		str += to_string(temp.GetProID()) + "  ";
		ready_queue.pop();
		ready_queue.push(temp);
	}
	str += "\n";
	return str;
}

string ProcessSchedule::GetAllWaitQueue()
{
	string str = "";
	for (int i = 1; i <= wait_queue.size(); i++)
	{
		PCB &temp = wait_queue.front();
		str += to_string(temp.GetProID()) + "  ";
		wait_queue.pop();
		wait_queue.push(temp);
	}
	str += "\n";
	return str;
}

PCB & ProcessSchedule::GetRunningPCB()
{
	return running_queue.front();
}

bool ProcessSchedule::IfExistRunningPCB()
{
	if (running_queue.size() != 0)
		return true;
	else return false;
}