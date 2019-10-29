#include "FileOperation.h"

FileOperation::FileOperation()
{
	ValueReset();
}

void FileOperation::ValueReset()
{
	write_txt = fopen("19316117-pcbs-input.txt", "a+");
	read_txt = fopen("19316117-pcbs-input.txt", "r");
	if_firsttime_read = true;
	if_show_pcb_head = false;
	if_write_pcb_head = false;
}

PCB FileOperation::ReadOnePCBFromFile()
{
	int data[6], instruc[MAX_INSTRUC_NUM][3];
	//如果是第一次读取，则将表头给读取
	if (if_firsttime_read == true)
	{
		fscanf(read_txt, "ProID\tPriority\tInTimes\tRunTimes\tInstrucNum\tPSW\tInstruc_ID\tInstruc_State\tInstruc_Times\n");
		if_firsttime_read = false;
	}
	fscanf(read_txt, "%d\t\t%d\t\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t%d\t\t\t%d\t\t\t\t%d\n", &data[0], &data[1], &data[2], &data[3], &data[4], &data[5],&instruc[0][0],&instruc[0][1],&instruc[0][2]);
	read_from_file.SetProID(data[0]);
	read_from_file.SetPriority(data[1]);
	read_from_file.SetInTimes(data[2]);
	read_from_file.SetRunTimes(data[3]);
	read_from_file.SetInstrucNum(data[4]);
	read_from_file.SetPSW(data[5]);
	for (int i = 1; i < read_from_file.GetInstrucNum(); i++)
	{
		fscanf(read_txt, "\t\t\t\t\t\t\t\t\t\t\t\t%d\t\t\t%d\t\t\t\t%d\n", &instruc[i][0], &instruc[i][1], &instruc[i][2]);
	}
	for (int i = 0; i < read_from_file.GetInstrucNum(); i++)
	{
		read_from_file.SetInstrucID(i, instruc[i][0]);
		read_from_file.SetInstrucState(i, instruc[i][1]);
		read_from_file.SetInstrucTimes(i, instruc[i][2]);
	}
	return read_from_file;
}

string FileOperation::ReadAndShowAll()
{
	ReOpenReadFile();
	if_firsttime_read = true;
	string str = "";
	while (!feof(read_txt))
	{
		ReadOnePCBFromFile();
		str += ShowOwnPCBDetail();
		str += "----------------------------------------------------------------------------------------------------------------------\n";
		printf("----------------------------------------------------------------------------------------------------------------------\n");
	}
	return str;
}

void FileOperation::SetSystemPCB(PCB &sys)
{
	sys = read_from_file;
}

void FileOperation::SetAllSystemPCB(PCB &s0, PCB &s1, PCB &s2, PCB &s3, PCB &s4, PCB &s5, PCB &s6, PCB &s7, PCB &s8, PCB &s9)
{
	PCB temp[10];
	ReOpenReadFile();
	if_firsttime_read = true;
	int i = 0;
	while (!feof(read_txt))
	{
		temp[i++] = ReadOnePCBFromFile();
	}
	s0 = temp[0];
	s1 = temp[1];
	s2 = temp[2];
	s3 = temp[3];
	s4 = temp[4];
	s5 = temp[5];
	s6 = temp[6];
	s7 = temp[7];
	s8 = temp[8];
	s9 = temp[9];
}

string FileOperation::ShowPCBHead()
{
	if_show_pcb_head = true;
	string str = "ProID\tPriority\tInTimes\tRunTimes\tInstrucNum\tPSW\tInstruc_ID\tInstruc_State\tInstruc_Times\n";
	cout << str;
	return str;
}

void FileOperation::ResetPCBHeadShowInfo()
{
	if_show_pcb_head = false;
}

void FileOperation::ResetPCBHeadWriteInfo()
{
	if_write_pcb_head = false;
}

string FileOperation::ShowOwnPCBDetail()
{
	string str, str1, str2;
	if (if_show_pcb_head == false)
		str += ShowPCBHead();
	str1 += to_string(read_from_file.GetProID()) + "\t" + to_string(read_from_file.GetPriority()) + "\t" + "\t"
		+ to_string(read_from_file.GetInTimes()) + "\t" + to_string(read_from_file.GetRunTimes()) + "\t" + "\t"
		+ to_string(read_from_file.GetInstrucNum()) + "\t" + "\t" + to_string(read_from_file.GetPSW()) + "\t"
		+ to_string(read_from_file.GetInstrucID(0)) + "\t" + "\t" + to_string(read_from_file.GetInstrucState(0))
		+ "\t" + "\t" + "\t" + to_string(read_from_file.GetInstrucTimes(0)) + "\n";

	cout << str1;
	for (int i = 1; i < read_from_file.GetInstrucNum(); i++)
	{
		str2 = str2 + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + to_string(read_from_file.GetInstrucID(i)) + "\t"
			+ "\t" + to_string(read_from_file.GetInstrucState(i))
			+ "\t" + "\t" + "\t" + to_string(read_from_file.GetInstrucTimes(i)) + "\n";
	}
	cout << str2;
	return str + str1 + str2;
}

string FileOperation::ShowPCBDetail(PCB data)
{
	string str1, str2;
	str1 = str1 + to_string(data.GetProID()) + "\t" + to_string(data.GetPriority()) + "\t" + "\t" + to_string(data.GetInTimes()) + "\t"
		+ to_string(data.GetRunTimes()) + "\t" + "\t" + to_string(data.GetInstrucNum()) + "\t" + "\t" + to_string(data.GetPSW()) + "\t"
		+ to_string(data.GetInstrucID(0)) + "\t" + "\t" + to_string(data.GetInstrucState(0)) + "\t" + "\t" + "\t" + to_string(data.GetInstrucTimes(0)) + "\n";
	cout << str1;
	for (int i = 1; i < data.GetInstrucNum(); i++)
	{
		str2 = str2 + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t" + "\t"
			+ to_string(data.GetInstrucID(i)) + "\t" + "\t" + to_string(data.GetInstrucState(i))
			+ "\t" + "\t" + "\t" + to_string(data.GetInstrucTimes(i)) + "\n";
	}
	cout << str2;
	return str1 + str2;
}

void FileOperation::SetPCB(PCB data)
{
	this->read_from_file = data;
}

PCB FileOperation::GetPCB()
{
	return this->read_from_file;
}

void FileOperation::WriteHeadToFile()
{
	if_write_pcb_head = true;
	fprintf(write_txt, "ProID\tPriority\tInTimes\tRunTimes\tInstrucNum\tPSW\tInstruc_ID\tInstruc_State\tInstruc_Times\n");
}

void FileOperation::WritePCBToFile()
{
	if (if_write_pcb_head == false)
		WriteHeadToFile();
	fprintf(write_txt, "%d\t\t%d\t\t\t%d\t\t%d\t\t\t%d\t\t\t%d\t%d\t\t\t%d\t\t\t\t%d\n",
		read_from_file.GetProID(), read_from_file.GetPriority(), read_from_file.GetInTimes(),
		read_from_file.GetRunTimes(), read_from_file.GetInstrucNum(), read_from_file.GetPSW(),
		read_from_file.GetInstrucID(0), read_from_file.GetInstrucState(0), read_from_file.GetInstrucTimes(0));
	for (int i = 1; i < read_from_file.GetInstrucNum(); i++)
	{
		fprintf(write_txt, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t%d\t\t\t%d\t\t\t\t%d\n",
			read_from_file.GetInstrucID(i), read_from_file.GetInstrucState(i), read_from_file.GetInstrucTimes(i));
	}
}

void FileOperation::ShowRunResults()
{
	run_results = fopen("19316117_RunResults.txt", "a+");
	char s[1000], ch;
	int i = 0;
	while ((ch = fgetc(run_results)) != EOF)
		s[i++] = ch;
	s[i] = '\0';
	printf("%s\n", s);
	fclose(run_results);
}

void FileOperation::ClearPCBFile()
{
	fclose(write_txt);
	write_txt = fopen("19316117-pcbs-input.txt", "w+");
}

void FileOperation::ClearResultsFile()
{
	run_results = fopen("19316117_RunResults.txt", "w+");
	fclose(run_results);
}

void FileOperation::ReOpenWriteFile()
{
	fclose(write_txt);
	write_txt = fopen("19316117-pcbs-input.txt", "a+");
}

void FileOperation::ReOpenReadFile()
{
	fclose(read_txt);
	read_txt = fopen("19316117-pcbs-input.txt", "r");
	fseek(read_txt, 0L, SEEK_SET);
}

void FileOperation::WriteResults(string data)
{
	ofstream out("19316117_RunResults.txt", ios::app);
	out << data;
	out.close();
}
