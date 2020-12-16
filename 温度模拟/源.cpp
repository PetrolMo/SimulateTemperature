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
		this->year = y;this->month = m;this->day = d;this->hour = h;this->quarter = quarter;
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
	string tempStr;
	date() {};
	date(date_time d)
	{
		this->myTime = d;
	};
	bool isLeepYear();//判断是不是闰年
	void setDate(int offset, int days, int quarter);//得到对应的月份天数
	void setTempString();	//得到正确格式
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

float sensor::getTemp(double Rt)//阻值转温度
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

float Temp::averageTemp() //每一个月平均温度
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
	for (int i = 0; i < ps->length; i++)
		for (int j = 0; j < ps->width; j++) {
			srand((double)clock());
			ans += ps->s->getTemp(random(low, high));
		}
	ans = (ans / LENGTH) / WIDTH;
	ps->averageTemp = ans;
	return ans;
}

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
			temp += 2;
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
		tHigh = temp + (quarter / 45)+ hour - 8;
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
	srand((double)clock());
	float k = random(int(10 * tLow), int(10 * tHigh));
	ans = k / 10.0;
	temp = ans;
	return ans;
}

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
	return s;
}

bool date::isLeepYear() {
	bool ans = false;
	if (myTime.year % 4 == 0 && myTime.year % 100 != 0 || myTime.year % 400 == 0)
		ans = true;
	return ans;
}

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
	myTime.year  = 2016+offset;
	myTime.month = tm.tm_mon+1;
	myTime.day = tm.tm_mday;
	myTime.hour = tm.tm_hour;
	myTime.quarter = tm.tm_min;
}

void date::setTempString()
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
	tempStr = ans;
}

int main()
{
	Temp t;
	sensorMatrix sm(LENGTH, WIDTH);
	date_time startTime(2016, 1, 1, 0, 0);
	date d(startTime);
	t.pd = &d;
	t.ps = &sm;
	for (int i = 1; i <= 3; i++)
	{
		for (int j = 0; j <= d.isLeepYear() ? 366 : 365; j++)
		{
			for (int m = 0; m < 96; m++)
			{
				d.setDate(i, j, m);
				d.setTempString();
				cout << t.getTempString() + "℃_" + d.tempStr << endl;
				Sleep(SECONDS);
			}
		}
	}
	return 0;
}