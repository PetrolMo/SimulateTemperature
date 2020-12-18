#include <iostream>
#include <fstream>
#include <sstream>
#include<string>
#include<ctime>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#define random(a,b) (rand()%(b-a)+a)
#define LENGTH 2
#define WIDTH 2
#include<Windows.h>
#define SECONDS 500*2
using namespace std;

//日期结构体
struct date_time {
	date_time() {};
	date_time(int y, int m, int d, int h, int quarter) {
		this->year = y; this->month = m; this->day = d; this->hour = h; this->quarter = quarter;
	}
	int year;
	int month;
	int day;
	int hour;
	int quarter;
};

//传感器类
class sensor
{
public:
	sensor(double r) : R(0)
	{
		this->R = r;
	};
	float getTemp(double r);//阻值转温度
private:
	float temp;//温度
	double R;//阻值
};

//传感器矩阵类
class sensorMatrix
{
public:
	int length;
	int width;
	float averageTemp;
	sensor* s;
	sensorMatrix(int l, int w)
	{
		this->length = l; this->width = w;
	};
};

//日期类
class date {
public:
	date_time myTime;
	string DateStr;
	date() {};
	date(date_time d)
	{
		this->myTime = d;
	};
	bool isLeepYear();//判断是不是闰年
	void setDate(int offset, int days, int quarter);//得到对应的月份天数
	void setDateString();	//得到正确格式
	string getFileString(); //得到正确的文件夹格式
	string getTXTString();//得到正确的txt文件格式
	bool operator=(const date& d)//符号重载
	{
		this->myTime = d.myTime;
	};
};

//温度类
class Temp
{
public:
	date* pd;
	sensorMatrix* ps;
	float temp;
	float averageTemp();//平均温度
	float getQuarterTemp();//得到某一个quarter的温度
	string getTempString();//得到温度字符串
};

//传感器阻值转换为温度
float sensor::getTemp(double Rt)
{
	float t, ans;
	const float Rp = 10000.0; //10K
	const float T2 = (273.15 + 25.0);;//T2
	const float Bx = 3950.0;//B
	const float Ka = 273.15;
	//like this R=5000, T2=273.15+25,B=3470, RT=5000*EXP(3470*(1/T1-1/(273.15+25)),
	t = Rt / Rp;
	t = log(t);					//ln(Rt/Rp)
	t /= Bx;					//ln(Rt/Rp)/B
	t += (1 / T2);
	t = 1 / (t);
	t -= Ka;
	ans = int(t * 10) / 10.0;
	return ans;
}

//获取每一个月平均温度
float Temp::averageTemp()
{
	float ans = 0;
	int high, low;
	switch (pd->myTime.month) {
	case 1:high = 28700; low = 21200; break;
	case 2:high = 27300; low = 20200; break;
	case 3:high = 22300; low = 15100; break;
	case 4:high = 16600; low = 11450; break;
	case 5:high = 13120; low = 9150; break;
	case 6:high = 10930; low = 8390; break;
	case 7:high = 8760; low = 6505; break;
	case 8:high = 9150; low = 6780; break;
	case 9:high = 10930; low = 8390; break;
	case 10:high = 13750; low = 10450; break;
	case 11:high = 18300; low = 13750; break;
	case 12:high = 25900; low = 19200; break;
	default:cout << "ERROR!\n";
	}
	//srand((double)clock());
	//ans += ps->s->getTemp(random(low, high));
	ans = ps->s->getTemp(high) + ps->s->getTemp(low);
	ans = ans / 2;
	ps->averageTemp = ans;
	return ans;
}

//获取每一时刻的温度
float Temp::getQuarterTemp() //得到某一个quarter的温度
{
	float tHigh, tLow;
	float ans;
	int month = pd->myTime.month;
	int day = pd->myTime.day;
	int hour = pd->myTime.hour;
	int quarter = pd->myTime.quarter;
	temp = this->averageTemp();
	switch (month) {
	case 1:
		if (day <= 10 && day >= 1)
			temp += 0.7;
		else if (day <= 31 && day >= 20)
			temp += 0.3;
		break;
	case 2:
		if (day <= 10 && day >= 1)
			temp -= 0.3;
		else if (day <= 31 && day >= 20)
			temp += 2;
		break;
	case 3:
		if (day <= 10 && day >= 1)
			temp -= 2;
		else if (day <= 31 && day >= 20)
			temp += 3;
		break;
	case 4:
		if (day <= 10 && day >= 1)
			temp -= 2;
		else if (day <= 31 && day >= 20)
			temp += 1.7;
		break;
	case 5:
		if (day <= 10 && day >= 1)
			temp -= 1.7;
		else if (day <= 31 && day >= 20)
			temp += 1;
		break;
	case 6:
		if (day <= 10 && day >= 1)
			temp -= 1;
		else if (day <= 31 && day >= 20)
			temp += 1.8;
		break;
	case 7:
		if (day <= 10 && day >= 1)
			temp -= 1.8;
		else if (day <= 31 && day >= 20)
			temp -= 0.3;
		break;
	case 8:
		if (day <= 10 && day >= 1)
			temp += 0.3;
		else if (day <= 31 && day >= 20)
			temp -= 1.5;
		break;
	case 9:
		if (day <= 10 && day >= 1)
			temp += 1.5;
		else if (day <= 31 && day >= 20)
			temp -= 1.7;
		break;
	case 10:
		if (day <= 10 && day >= 1)
			temp += 1.7;
		else if (day <= 31 && day >= 20)
			temp -= 2;
		break;
	case 11:
		if (day <= 10 && day >= 1)
			temp += 2.5;
		else if (day <= 31 && day >= 20)
			temp -= 2;
		break;
	case 12:
		if (day <= 10 && day >= 1)
			temp += 2.1;
		else if (day <= 31 && day >= 20)
			temp -= 0.7;
		break;
	default:cout << "ERROR!\n";
	}
	if (hour >= 0 && hour <= 5 || hour == 23) {
		tHigh = temp - 4;
		tLow = temp - 5;
	}
	else if (hour == 6 || hour == 7) {
		tHigh = temp + (quarter / 30) + hour * 2 - 16;
		tLow = temp + (quarter / 30) + hour * 2 - 17;
	}
	else if (hour >= 8 && hour <= 13) {
		tHigh = temp + (quarter / 45) + hour - 8;
		tLow = temp + (quarter / 45) + hour - 9;
	}
	else if (hour >= 14 && hour <= 17) {
		tHigh = temp - (quarter / 45) - hour + 20;
		tLow = temp - (quarter / 45) - hour + 19;
	}
	else if (hour == 18) {
		tHigh = temp - (quarter / 30) - hour + 20;
		tLow = temp - (quarter / 30) - hour + 19;
	}
	else if (hour >= 19 && hour <= 22) {
		tHigh = temp - (quarter / 45) - hour + 19;
		tLow = temp - (quarter / 45) - hour + 18;
	}
	else cerr << "ERROR!\n";
	float k = 0;
	for (int i = 0; i < ps->length; i++)
		for (int j = 0; j < ps->width; j++)
		{
			srand((double)clock());
			k += random(int(10 * tLow), int(10 * tHigh));
		}
	k = k / (ps->length) / (ps->width);
	ans = k / 10.0;
	temp = ans;
	return ans;
}

//获取温度字符串
string Temp::getTempString() {
	string* ans;
	string s = to_string(getQuarterTemp());
	if (s.substr(0, 1) != "-") {
		s.insert(0, "+");
	}
	if (s.find(".") == 2) {
		s.insert(1, "0");
	}
	s.erase(s.end() - 5, s.end());
	return s + "℃";
}

//判断是否是闰年
bool date::isLeepYear() {
	bool ans = false;
	if (myTime.year % 4 == 0 && myTime.year % 100 != 0 || myTime.year % 400 == 0)
		ans = true;
	return ans;
}

//设置模拟日期 
void date::setDate(int offset, int days, int quarter)
{
	auto isLeap = [](int _year)->bool { return (_year % 4 == 0) && (_year % 100 != 0) || (_year
		% 400 == 0); };
	int day = days;
	for (int i = 1970; i < myTime.year; ++i)
	{
		day += isLeap(i) ? 366 : 365;
	}
	time_t temp = day * 24 * 60 * 60 + quarter * 15 * 60 - 8 * 60 * 60;
	struct tm tm;
	localtime_s(&tm, &temp);
	myTime.year = 2016 + offset;
	myTime.month = tm.tm_mon + 1;
	myTime.day = tm.tm_mday;
	myTime.hour = tm.tm_hour;
	myTime.quarter = tm.tm_min;
}

//设置输出的日期字符串 如：2017/06/01_08:00
void date::setDateString()
{
	string ans, syear, smonth, sday, shour;
	syear = to_string(myTime.year);
	smonth = to_string(myTime.month);
	sday = to_string(myTime.day);
	shour = to_string(myTime.hour);
	if (smonth.length() == 1)
	{
		smonth.insert(0, "0");
	}
	if (sday.length() == 1)
	{
		sday.insert(0, "0");
	}
	if (shour.length() == 1)
	{
		shour.insert(0, "0");
	}
	ans = syear + '/' + smonth + "/" + sday + '_' + shour + ':';
	int k = myTime.quarter;
	if (k == 0) {
		ans = ans + "00";
	}
	else {
		ans = ans + to_string(k);
	}
	DateStr = ans;
}

//根据日期获取正确的文件夹路径日期 如：D:\\Temps\\2017_01
string date::getFileString()
{
	string ans, syear, smonth;
	syear = to_string(myTime.year);
	smonth = to_string(myTime.month);
	if (smonth.length() == 1)
	{
		smonth.insert(0, "0");
	}
	ans = "D:\\Temps\\" + syear + '_' + smonth;
	return ans;
}

//根据日期获取正确的txt文件 如：2017_03_05
string date::getTXTString()
{
	string ans, syear, smonth, sday;
	syear = to_string(myTime.year);
	smonth = to_string(myTime.month);
	sday = to_string(myTime.day);
	if (smonth.length() == 1)
	{
		smonth.insert(0, "0");
	}
	if (sday.length() == 1)
	{
		sday.insert(0, "0");
	}
	ans = syear + '_' + smonth + "_" + sday + ".txt";
	return ans;
}

//创建文件夹 路径
void mkDir(string path) {
	string p = "mkdir " + path;  // 注意斜杠是两个。
	system(p.c_str());  //产生以时间为名字的文件夹
}

int main()
{
	string fileName;
	string fileRoute;
	Temp t;
	sensorMatrix sm(LENGTH, WIDTH);
	date_time startTime(2016, 1, 1, 0, 0);
	date d(startTime);
	t.pd = &d;
	t.ps = &sm;
	for (int i = 1; i <= 3; i++)
	{
		d.setDate(i, 0, 0);
		for (int j = 0; j <= (d.isLeepYear() ? 366 : 365); j++)
		{
			d.setDate(i, j, 0);
			//以月为单位创建文件夹
			if (d.myTime.day == 1) {
				mkDir(d.getFileString());
			}
			//以天为单位创建txt文件
			ofstream file;
			fileName = d.getTXTString();
			fileRoute = d.getFileString() + "\\" + fileName;
			file.open(fileRoute, ios::in | ios::app);
			for (int m = 0; m < 96; m++)
			{
				d.setDate(i, j, m);
				d.setDateString();
				cout << t.getTempString() + "_" + d.DateStr << endl;
				file << t.getTempString() + "_" + d.DateStr << endl;
			}
			file.close();
		}
	}
	return 0;
}