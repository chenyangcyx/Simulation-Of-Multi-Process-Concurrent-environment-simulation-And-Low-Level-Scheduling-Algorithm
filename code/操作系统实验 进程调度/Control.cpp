#include "Control.h"

Control::Control()
{
	ValueReset();
}

void Control::ValueReset()
{
	if_close_timeslice_break = false;
	last_priority_ID = -1;
	break_time_num = TIMESLICE_TIME_LENGTH / BREAK_TIME_LENGTH;
	time_slice_num = 0;
	receive_break = 0;
	cpu_current_time = 0;
}

void Control::MenuShow()
{
	system("cls");
	printf("*****多进程并发环境模拟以及低级调度算法的仿真实现*****\n");
	printf("*                                                    *\n");
	printf("*                                                    *\n");
	printf("*           1-开始模拟运行                           *\n");
	printf("*                                                    *\n");
	printf("*           2-读取文件PCB内容（pcb-input.txt）       *\n");
	printf("*                                                    *\n");
	printf("*           3-重新生成PCB文件（pcb-input.txt）       *\n");
	printf("*                                                    *\n");
	printf("*           4-查看运行结果（RunResults.txt）         *\n");
	printf("*                                                    *\n");
	printf("*           5-退出程序                               *\n");
	printf("*                                                    *\n");
	printf("*                                                    *\n");
	printf("******************************************************\n");
	printf("\n\n请输入需要执行的选项：  ");
	int choose_num = 0;
	scanf("%d", &choose_num);
	while (choose_num < 1 || choose_num > 5)
	{
		printf("\n输入错误！请重新输入：  ");
		scanf("%d", &choose_num);
	}
	ChoiceOperation(choose_num);
}

void Control::ChoiceOperation(int choice)
{
	switch (choice)
	{
	case 1:
		system("cls");
		UnixDispatch();
		LastShowAllInfo();
		ShowSavedFileInfo();
		AnyKeyToBackToMenu();
		break;
	case 2:
		system("cls");
		fo.ResetPCBHeadShowInfo();
		fo.ReadAndShowAll();
		AnyKeyToBackToMenu();
		break;
	case 3:
		system("cls");
		fo.ResetPCBHeadShowInfo();
		fo.ResetPCBHeadWriteInfo();
		ReCreateAllPCB();
		AnyKeyToBackToMenu();
		break;
	case 4:
		system("cls");
		fo.ShowRunResults();
		AnyKeyToBackToMenu();
		break;
	case 5:
		exit(0);
		break;
	default:
		break;
	}
}

void Control::AnyKeyToBackToMenu()
{
	char c[10];
	printf("\n\n请输入任意值以返回主菜单！  ");
	scanf("%s", c);
	MenuShow();
}

void Control::ReCreateAllPCB()
{
	int i = 0;
	printf("要生成多少个PCB任务请求（请输入5-10之间的整数）：  ");
	int num = 0;
	scanf("%d", &num);
	while (num < 5 || num>10)
	{
		printf("输入格式错误！请重新输入！\n");
		scanf("%d", &num);
	}
	PCB create_temp;
	fo.ClearPCBFile();
	tr.InitPCBNum();
	for (int i = 0; i < num; i++)
	{
		create_temp = tr.CreateNewPCB();
		fo.SetPCB(create_temp);
		fo.WritePCBToFile();
		fo.ShowOwnPCBDetail();
	}
	fo.ReOpenWriteFile();
}

void Control::UnixDispatch()
{
	PCB temp[SYSTEM_MAX_PCB_NUM], t;
	fo.SetAllSystemPCB(temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7], temp[8], temp[9]);
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		tr.SetOnePCB(i, temp[i]);
	}
	fo.ClearResultsFile();				//清空Results文件
	/*数值重新初始化*/
	ValueReset();
	cp.ValueReset();
	fo.ValueReset();
	ps.ValueReset();
	tr.ValueReset();
	/*数值重新初始化*/
	int join_task_num = 0, max_intime = -1;
	max_intime = tr.CalMaxInTime();
	thread task_request_thread(&Control::TaskRequestThread, this);		//任务请求调入
	thread time_slice_thread(&Control::TimeSliceThread, this);			//时间片调度
	thread PCB_run_thread(&Control::PCBRunThread, this);				//进程运行
	tc.BeginCreate();		//开始计时
	task_request_thread.detach();
	time_slice_thread.detach();
	PCB_run_thread.join();			//主线程等待执行，所以使用join
	LastTimeSliceDone();			//最后一个时间片的处理
}

void Control::CloseTimeSliceBreak()
{
	if_close_timeslice_break = true;
}

void Control::OpenTimeSliceBreak()
{
	if_close_timeslice_break = false;
}

int Control::GetMaxPriorityProcessID()
{
	int min_no = -1;
	int min = 99999;
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		PCB &temp = tr.GetOnePCB(i);
		if(((temp.GetProState()==PCB_STATE_READY)||(temp.GetProState() == PCB_STATE_RUNNING))&&(temp.GetIfUsed()==true)&&(temp.GetIfEnd()!=true))
			if (temp.Get_P_PRI() <= min)
			{
				min = temp.Get_P_PRI();
				min_no = i;
			}
	}
	return min_no;
}

string Control::GetOnePCBInfo_TimeSlice(int no)
{
	PCB &temp = tr.GetOnePCB(no);
	string text = "";
	//进程编号输出
	text += "进程编号：" + to_string(no + 1) + "\n";
	//进程创建时间
	text += "进入时间：" + to_string(temp.GetInTimes()) + "；";
	//进程状态
	text += "当前状态：";
	switch (temp.GetProState())
	{
	case PCB_STATE_RUNNING:
		text += "Running；";
		break;
	case PCB_STATE_READY:
		text += "Ready；";
		break;
	case PCB_STATE_WAIT:
		text += "Wait；";
		break;
	case PCB_STATE_OVER:
		text += "Over；";
		break;
	default:
		break;
	}
	//进程运行需要时间
	text += "需要运行时间：" + to_string(temp.GetRunTimes()) + "；";
	//进程中包含的指令数目
	text += "包含指令数目：" + to_string(temp.GetInstrucNum()) + "；";
	//当前运行到哪一条指令
	text += "当前指令：" + to_string(temp.GetPSW()) + "；\n";
	//进程实际开始时间
	text += "开始运行时间：" + to_string(temp.GetStartTime()) + "；";
	//进程已经运行时间
	text += "已经运行时间：" + to_string(temp.GetCurrentRunTime()) + "；";
	//p-pri的值
	text += "P-PRI值：" + to_string(temp.Get_P_PRI()) + "；\n";
	if (temp.GetIfEnd() == true)
	{
		text += "该进程运行结束！  结束时间：" + to_string(temp.GetEndTime()) + "\n";
	}
	return text;
}

void Control::TaskRequestThread()
{
	int join_task_num = 0, max_intime = -1;
	max_intime = tr.CalMaxInTime();
	while (1)
	{
		//符合时间的任务请求处理
		if ((tr.TaskJoinCheck(cpu_current_time,join_task_num) == true) && (cpu_current_time < max_intime + BREAK_TIME_LENGTH * 2))
		{
			/*任务调度到就绪队列*/
			PCB &rj = tr.GetOnePCB(join_task_num);
			if (rj.GetIfUsed() == false)
			{
				rj.ProcessCreate();				//原语——进程创建
				ps.JoinReadyQueue(rj, fo, cpu_current_time);
				string str = "△cpu时间：" + to_string(cpu_current_time) + "，作业" + to_string(rj.GetProID()) + "调入内存，等待执行！\n";
				cout << str;
				fo.WriteResults(str);
			}
		}
		if (cpu_current_time >= max_intime + BREAK_TIME_LENGTH * 2)
			break;
	}
}

void Control::TimeSliceThread()
{
	while (!tr.IfAllOK())
	{
		if (break_time_num == (TIMESLICE_TIME_LENGTH / BREAK_TIME_LENGTH))
		{
			string str = "";
			str += "\n\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓时间片信息↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓";
			str += "\n第" + to_string(time_slice_num) + "个时间片到来！\n";
			cout << str;
			//每次时间片信号到来时的处理
			if (!if_close_timeslice_break)		//有时间片信号且没有被关中断
			{
				fo.WriteResults(str);
				str.clear();
				//将每个进程的p-cpu进行衰减并计算每个进程的p-pri
				for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
				{
					PCB &ts = tr.GetOnePCB(i);
					ts.PCPU_DECAY();				//p-cpu值衰减（所有进程同时衰减）
					ts.Cal_P_PRI();					//计算所有进程的p-pri
				}
				int current_priority_id = GetMaxPriorityProcessID();
				if (current_priority_id != last_priority_ID)
				{
					if (ps.GetRunningQueueSize() > 0)			//上一次进程从运行态调入就绪态
					{
						PCB& temp = tr.GetOnePCB(last_priority_ID);		//取出运行队列中的元素
						ps.JoinReadyQueue(temp, fo, cpu_current_time);	//将该元素加入就绪队列
						ps.OutRunningQueue(last_priority_ID);
					}
					PCB& re = tr.GetOnePCB(current_priority_id);		//获取优先权最大的进程
					ps.JoinRunningQueue(re, fo, cpu_current_time);		//将该进程加入到运行队列
					ps.OutReadyQueue(current_priority_id);				//将该进程从就绪队列中删除
					last_priority_ID = current_priority_id;
				}
			}
			AllPCBInfoShow_TimeSlice();
			str += ps.ShowRunningQueue();
			str += ps.ShowReadyQueue();
			str += ps.ShowWaitQueue();
			str += "↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑时间片信息↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n";
			cout << "↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑时间片信息↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n";
			time_slice_num++;
			break_time_num = 0;		//时间片信号恢复
			fo.WriteResults(str);
		}
	}
}

void Control::PCBRunThread()
{
	while (!tr.IfAllOK())
	{
		//每次的中断处理
		if (tc.GetIfBreak())		//产生一次10ms中断
		{
			ShowEveryTimeDetail();			//每个时间点显示一次详细信息
			if (ps.GetRunningQueueFirstID() != PCB_INIT_NUM)		//运行队列中有元素
			{
				PCB &temp = tr.GetOnePCB(ps.GetRunningQueueFirstID());		//temp为当前运行队列中的元素
				if (temp.GetStartTime() == PCB_INIT_NUM)		//若进程从未被创建，则开始计时
				{
					temp.SetStartTime(cpu_current_time);
				}
				cp.RecoverSpot(temp);			//CPU恢复现场
				cp.CurrentRunTimeSelfAdd(cpu_current_time, fo);		//进程实际使用时间自增，更新CPU状态，更新时间片bool参数
				cp.RefreshOverRunTime();		//恢复超出的实际使用时间
				temp.P_CPU_Add();				//当前运行的进程的p-cpu自增
				cp.ProtectSpot(temp);			//CPU保护现场
				CheckIfCloseTimeSlice(temp);	//检测是否需要关闭时间片中断

				if (temp.GetInstrucState(temp.GetPSW() - 1) == INSTRUC_STATE_PV_OPERATION)		//PV操作的检测
					PVOperation(temp);
				PVOperationTimeAdd();			//处于等待态的进程的等待时间自增
				CheckIfPVOperationOK();			//检查是否有处于等待态的进程完成

				//temp引用的重新声明
				PCB &temp_new = tr.GetOnePCB(ps.GetRunningQueueFirstID());		//temp为当前运行队列中的元素
				if (temp_new.GetStartTime() == PCB_INIT_NUM)		//若进程从未被创建，则开始计时
				{
					temp_new.SetStartTime(cpu_current_time);
				}

				if (cp.CheckIfOK() == true)		//进程完成的检测
				{
					string str = "";
					str += "√cpu时间：" + to_string(cpu_current_time + BREAK_TIME_LENGTH) + "，进程" + to_string(cp.GetPC()) + "运行结束！\n";
					cout << str;
					fo.WriteResults(str);
					//该进程出运行队列，并且将if_end标志设置为true
					ps.OutRunningQueue(temp_new.GetProID() - 1);
					temp_new.ProcessCancel(cpu_current_time + BREAK_TIME_LENGTH);		//进程撤销原语
					//马上调入下一个就绪态的PCB
					int max = GetMaxPriorityProcessID();		//获取当前的最大优先权的ID
					if (ps.GetReadyQueueSize() > 0)
					{
						PCB& temp2 = tr.GetOnePCB(max);						//取出就绪态中适合的元素
						ps.JoinRunningQueue(temp2, fo, cpu_current_time + BREAK_TIME_LENGTH);	//将该进程加入到运行队列
						cp.RecoverSpot(temp2);								//恢复现场
					}
					ps.OutReadyQueue(max);						//将该进程从就绪队列中删除
					last_priority_ID = max;
				}
			}
			else			//运行队列中没有元素
			{
				cp.WaitTimeSelfAdd();
				PVOperationTimeAdd();			//处于等待态的进程的等待时间自增
				CheckIfPVOperationOK();			//检查是否有处于等待态的进程完成
				//极端情况解决——运行态为空，就绪态为1，进不去的情况
				if (ps.GetReadyQueueSize() !=0 && ps.GetRunningQueueSize() == 0)
				{
					int max = GetMaxPriorityProcessID();
					PCB& temp = tr.GetOnePCB(max);
					ps.OutReadyQueue(temp.GetProID() - 1);
					ps.JoinRunningQueue(temp, fo, cpu_current_time + BREAK_TIME_LENGTH);
				}
			}
			break_time_num++;
			receive_break++;
			tc.ResetBreak();		//中断信号恢复
			RefreshCpuTime();
		}
	}
}

void Control::AllPCBInfoShow_TimeSlice()
{
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		if (tr.CheckIfJoin(i) == true)
		{
			string text = GetOnePCBInfo_TimeSlice(i);
			cout << text;
			fo.WriteResults(text);
		}
	}
	ShowCurrentTimeSlice();
}

void Control::ShowCurrentTimeSlice()
{
	string str = "";
	str = str + "此时cpu时间：" + to_string(time_slice_num * TIMESLICE_TIME_LENGTH) + "\n";
	cout << str;
	fo.WriteResults(str);
}

void Control::RefreshCpuTime()
{
	cpu_current_time = receive_break * BREAK_TIME_LENGTH;
}

void Control::LastTimeSliceDone()
{
	/*最后一个输出的善后处理*/
	string str = "";
	str += "\n\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓时间片信息↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓";
	str += "\n第" + to_string(time_slice_num) + "个时间片到来！\n";
	cout << str;
	fo.WriteResults(str);
	str.clear();
	AllPCBInfoShow_TimeSlice();
	str += ps.ShowRunningQueue();
	str += ps.ShowReadyQueue();
	str += ps.ShowWaitQueue();
	str += "↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑时间片信息↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n";
	cout << "↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑时间片信息↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n";
	time_slice_num++;
	break_time_num = 0;
	fo.WriteResults(str);
	/*最后一个输出的善后处理*/
}

void Control::LastShowAllInfo()
{
	string str = "";
	printf("\n所有指令已经执行完毕，输入任意键开始输出每个进程的详细信息：  ");
	str += "\n所有指令已经执行完毕，开始输出每个进程的详细信息：\n";
	int tn;
	scanf("%d", &tn);
	printf("\n");
	/*输出每条进程的详细情况*/
	str += fo.ShowPCBHead();
	str += fo.ReadAndShowAll();
	printf("\n");
	str += "\n";
	/*输出每条进程的详细情况*/

	//计算CPU结束时间
	int cpu_endtime = -1;
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		PCB &temp = tr.GetOnePCB(i);
		if (temp.GetIfUsed() == true)
		{
			if (temp.GetEndTime() > cpu_endtime)
				cpu_endtime = temp.GetEndTime();
		}
	}

	string str2 = "";
	/*输出详细统计信息*/
	for (int i = 0; i < SYSTEM_MAX_PCB_NUM; i++)
	{
		PCB &temp = tr.GetOnePCB(i);
		if (temp.GetIfUsed() == true)
		{
			//进程编号
			str2 += "进程" + to_string(temp.GetProID()) + "：\n";
			//进程进入时间
			str2 += "提交时间：" + to_string(temp.GetInTimes()) + "；";
			//进程需要运行时间
			str2 += "需要运行时间：" + to_string(temp.GetRunTimes()) + "；";
			//进程包含指令数
			str2 += "包含指令数：" + to_string(temp.GetInstrucNum()) + "；";
			//进程开始运行时间
			str2 += "开始运行时间：" + to_string(temp.GetStartTime()) + "；";
			//进程结束运行时间
			str2 += "结束运行时间：" + to_string(temp.GetEndTime()) + "；\n";
			//周转时间
			str2 += "周转时间：" + to_string(temp.GetEndTime() - temp.GetInTimes()) + "；";
			//带权周转时间
			str2 += "带权周转时间：" + to_string((float)(((temp.GetEndTime() - temp.GetInTimes())*1.0) / (temp.GetRunTimes()*1.0))) + "；\n";
		}
	}
	//CPU利用率
	str2 += "\nCPU总共运行时间：" + to_string(cpu_endtime) + "；有效工作时间：" + to_string(cpu_endtime - cp.GetWaitTime()) + "；等待时间：" + to_string(cp.GetWaitTime()) + "；\n";
	str2 += "CPU利用率：" + to_string(100.0*(float)((1.0*(cpu_endtime - cp.GetWaitTime())) / (cpu_endtime*1.0))) + "%；\n\n";
	/*输出详细统计信息*/
	cout << str2;
	fo.WriteResults(str);
	fo.WriteResults(str2);
}

void Control::ShowSavedFileInfo()
{
	//结束提示信息
	cout << "\n\n进程已经全部运行结束！\n";
	char directory[256] = { '\0' };
	GetCurrentDirectoryA(_MAX_DIR, directory);
	cout << "进程PCB信息存储在：" << directory << "\\19316117-pcbs-input.txt中！" << endl;
	GetCurrentDirectoryA(_MAX_DIR, directory);
	cout << "进程运行结果存储在：" << directory << "\\19316117_RunResults.txt中！" << endl << endl;
}

void Control::CheckIfCloseTimeSlice(PCB &data)
{
	switch (data.GetInstrucState(data.GetPSW()))
	{
	case INSTRUC_STATE_SYSTEMCALL:
		if_close_timeslice_break = true;
		break;
	case INSTRUC_STATE_USERMOD_OPERATION:
		if_close_timeslice_break = false;
		break;
	case INSTRUC_STATE_PV_OPERATION:
		if_close_timeslice_break = true;
		break;
	default:
		break;
	}
}

void Control::PVOperation(PCB &pcb)
{
	string str = "";
	str = str + "\n◇cpu时间：" + to_string(cpu_current_time) + "，进程" + to_string(pcb.GetProID()) + "进入阻塞态！\n";
	cout << str;
	fo.WriteResults(str);
	pcb.SetCurrentTime(pcb.GetCurrentRunTime() - BREAK_TIME_LENGTH);		//原来的时间-10
	pcb.SetPSW(pcb.GetPSW() - 1);
	if (pcb.GetPSW() == 0)
		pcb.SetPSW(1);
	ps.OutRunningQueue(pcb.GetProID() - 1);			//将该进程从运行队列移出
	ps.JoinWaitQueue(pcb, fo, cpu_current_time);	//将该进程移入等待队列
	//找出最高优先级的PCB进程
	int max = GetMaxPriorityProcessID();		//获取当前的最大优先权的ID
	if (ps.GetReadyQueueSize() > 0)
	{
		PCB& temp2 = tr.GetOnePCB(max);							//取出就绪态中适合的元素
		ps.JoinRunningQueue(temp2, fo, cpu_current_time);		//将该进程加入到运行队列
		cp.RecoverSpot(temp2);									//恢复现场
	}
	ps.OutReadyQueue(max);						//将该进程从就绪队列中删除
	pcb.InitWaitTime();							//开始等待时间计时
}

void Control::PVOperationTimeAdd()
{
	for (int i = 1; i <= SYSTEM_MAX_PCB_NUM; i++)
	{
		PCB &temp = tr.GetOnePCB(i - 1);
		if (temp.GetProState()==PCB_STATE_WAIT)
		{
			temp.WaitTimeSelfAdd();		//等待时间自增
		}
	}
}

void Control::CheckIfPVOperationOK()
{
	for (int i = 1; i <= SYSTEM_MAX_PCB_NUM; i++)
	{
		PCB &temp = tr.GetOnePCB(i - 1);
		if ((temp.GetWaitTime() == PV_INSTRUCTION_TIME) && (temp.GetProState() == PCB_STATE_WAIT))
		//等待时间已经到
		{
			string str = "";
			PVProcessToReady(temp);
			str = str + "◆cpu时间：" + to_string(cpu_current_time + BREAK_TIME_LENGTH) + "，进程" + to_string(temp.GetProID()) + "的I/O操作完成！从阻塞态进入就绪态！\n";
			cout << str;
			fo.WriteResults(str);
		}
	}
}

void Control::PVProcessToReady(PCB &pcb)
{
	ps.OutWaitQueue(pcb.GetProID() - 1);			//移出等待队列
	ps.JoinReadyQueue(pcb, fo, cpu_current_time + BREAK_TIME_LENGTH);	//加入就绪队列
	pcb.InitWaitTime();					//等待时间复位
	pcb.ProcessWakeUp();				//进程唤醒
	pcb.SetCurrentTime(pcb.GetCurrentRunTime() + PV_INSTRUCTION_TIME);
	if(pcb.GetPSW()!=1)
		pcb.SetPSW(pcb.GetPSW() + 1);
}

void Control::ShowEveryTimeDetail()
{
	string str = "";
	int current_id = ps.GetRunningQueueFirstID();
	if (current_id != PCB_INIT_NUM)
	{
		PCB &temp = tr.GetOnePCB(current_id);
		str = str + "\n|- cpu时间：" + to_string(cpu_current_time) + "\n";
		str = str + "|- 当前运行进程：" + to_string(temp.GetProID()) + "，当前指令：" + to_string(temp.GetPSW()) + "，类型：";
		switch (temp.GetInstrucState(temp.GetPSW()))
		{
		case INSTRUC_STATE_SYSTEMCALL:
			str = str + "0-系统调用";
			break;
		case INSTRUC_STATE_USERMOD_OPERATION:
			str = str + "1-用户态计算操作";
			break;
		case INSTRUC_STATE_PV_OPERATION:
			str = str + "2-PV操作";
			break;
		default:
			break;
		}
		str = str + "，已经运行时间：" + to_string(temp.GetCurrentRunTime()) + "\n";
		str = str + "|- 运行队列：" + ps.GetAllRunningQueue();
		str = str + "|- 就绪队列：" + ps.GetAllReadyQueue();
		str = str + "|- 等待队列：" + ps.GetAllWaitQueue();
		if (cp.GetCpuState() == USER_MODE)
			str = str + "|- CPU状态：" + "USER_MODE";
		else if(cp.GetCpuState() == CORE_MODE)
			str = str + "|- CPU状态：" + "CORE_MODE";
		if (if_close_timeslice_break == true)
			str = str + "，时间片信号：被屏蔽\n";
		else
			str = str + "，时间片信号：未屏蔽\n";
		str += "\n";
		cout << str;
		fo.WriteResults(str);
	}
	else
	{
		str = str + "\n|- cpu时间：" + to_string(cpu_current_time) + "\n";
		str = str + "|- 当前运行队列中没有进程，CPU空等待！\n";
		str = str + "|- 运行队列：" + ps.GetAllRunningQueue();
		str = str + "|- 就绪队列：" + ps.GetAllReadyQueue();
		str = str + "|- 等待队列：" + ps.GetAllWaitQueue();
		if (cp.GetCpuState() == USER_MODE)
			str = str + "|- CPU状态：" + "USER_MODE";
		else if (cp.GetCpuState() == CORE_MODE)
			str = str + "|- CPU状态：" + "CORE_MODE";
		if (if_close_timeslice_break == true)
			str = str + "，时间片信号：被屏蔽\n";
		else
			str = str + "，时间片信号：未屏蔽\n";
		str += "\n";
		cout << str;
		fo.WriteResults(str);
	}
}