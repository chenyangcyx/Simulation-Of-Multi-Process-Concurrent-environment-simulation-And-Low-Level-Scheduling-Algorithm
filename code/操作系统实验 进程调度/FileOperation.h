#pragma once
#include <stdio.h>
#include "PCB.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class FileOperation
{
private:
	PCB read_from_file;		//读入的数据的暂存
	FILE *read_txt;			//要读入的文件指针  19316117-pcbs-input.txt
	FILE *write_txt;		//要写入的文件指针  19316117-pcbs-input.txt
	FILE *run_results;		//运行结果 19316117-RunResults.txt
	bool if_firsttime_read;	//是否第一次读取文件
	bool if_show_pcb_head;	//是否已经输出PCB文件头
	bool if_write_pcb_head;	//是否已经写入PCB文件头
public:
	FileOperation();
	void ValueReset();				//重新设置初始化参数
	PCB ReadOnePCBFromFile();		//读取一条PCB的信息
	string ReadAndShowAll();		//读取并显示全部的PCB信息
	void SetSystemPCB(PCB&);		//设置传入的系统的PCB
	void SetAllSystemPCB(PCB&, PCB&, PCB&, PCB&, PCB&, PCB&, PCB&, PCB&, PCB&, PCB&);
	string ShowPCBHead();			//显示PCB的表目信息
	void ResetPCBHeadShowInfo();	//重置PCB的表目的信号  if_show_pcb_head
	void ResetPCBHeadWriteInfo();	//重置PCB的表目的信号  if_write_pcb_head
	string ShowOwnPCBDetail();		//显示PCB的信息
	string ShowPCBDetail(PCB);		//显示传入的PCB的信息
	void SetPCB(PCB data);			//设置当前的PCB
	PCB GetPCB();					//获取当前的PCB
	void WriteHeadToFile();			//将PCB的头写入文件
	void WritePCBToFile();			//将PCB写入文件
	void ShowRunResults();			//显示RunResults文件
	void ClearPCBFile();			//清空PCB文件
	void ClearResultsFile();		//清空Results文件
	void ReOpenWriteFile();			//重新打开写入文件
	void ReOpenReadFile();			//重新打开读取文件
	void WriteResults(string);		//向结果文件中写入数据
};