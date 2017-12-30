﻿#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;
struct student_infomation
{
	int id;//编号
	int b[10];//你所借书的的数量，上限10
	int borrowsum;//已借书,默认0
	string name;//读者姓名
	int book_num;//书数，默认为0
	string borrowday;//上次借阅时间
	
};
struct book
{
	int book_id;//图书检索号
	int BorrowCount;//图书借阅量,初始化为0
	string name;//书名
	string kind;//图书种类
	double price;//图书价格
	int sum;//图书总库存存量
	int nowsum;//库存量
	string author;//作者
	int appointment;//图书预约量
	bool ok;//是否可借,初始为可以
	string borrowdate;//图书最近一次借出时间
	string returndate;//图书最近一次归还时间
	string room;//馆藏地
};
bool cmpByidnum(book a, book b);

bool cmpByCount(book a, book b);

bool cmpByBorrowsum(student_infomation a, student_infomation b);

bool cmpByid(student_infomation a, student_infomation b);

class Library
{
private:
	int borro_book;//每月借出书数量
	int return_book;//每月还书数量
	vector<book> data;
	vector<student_infomation> data_1;
	vector<int> betoli;//预约书到馆，储存其编号
public:
	Library();
	void AddBook(book NewBook);  //增加图书
	void DeleteBook(string bookname, string author);//删除图书
	void BorrowBook(string name, string author);//借阅图书
	void BackBook(string name, string author, int k);//归还图书
	void ShowAllBook(); //输出系统所有图书
	void  SearchBookPosWithname(string thebook); //按书名查询
	void  SearchBookPosWithAuthor(string theauthor);//按作者查询
	void  SearchBookPosWithKind(string kind);//按种类查询
	int    SearchBookPosWithAB(string theauthor, string thebook);//按作者和书名查询
	int    SearchBookPosWithid(int id); //按检索号搜寻
	void SortBook(int ca);  //排序
	void SortStudent(int ca);//按读者借阅量排序
	void Save();  //存入图书馆文件
	void Save_student();//存入学生文件
	void Appointment(string bookname, string author);//预约图书
	void printbook(book a);//输出某本书的所有信息
	void revisebook(string name, string author);//修改某本书的信息
	int SerchStudent(int id);//查询某个读者
							 //int SerchStudent1(int id);//查询某个读者
	void AddStudent(student_infomation a);//增加一个读者
	void PrintStudent(int kid);//输出读者信息
	int GetStudent();//返回读者总数
	int read_num(int id);//获取某个读者的借阅量
	int getborrownum();//获取本月外借量
	int getreturnnum();//获取本月还书量
	void PrintToLi();//输出到馆预约书
};
Library::Library()
{
	borro_book = 0;
	return_book = 0;
	int AllBook, AllStudent;
	ifstream fin("book.txt");
	if (fin)
	{
		fin >> AllBook;
		for (int i = 0; i<AllBook; i++)
		{
			book temporary;
			fin >> temporary.book_id >> temporary.name >> temporary.author >> temporary.price >> temporary.kind >> temporary.room >> temporary.sum >> temporary.nowsum >> temporary.BorrowCount >> temporary.ok >> temporary.appointment >> temporary.borrowdate >> temporary.returndate;
			data.push_back(temporary);
		}
		fin.close();
	}
	//cin.clear();
	//cin.sync();
	ifstream tfin("student_infomation.txt");
	if (tfin)
	{
		tfin >> AllStudent;
		for (int i = 0; i<AllStudent; i++)
		{
			student_infomation temporary;
			tfin >> temporary.id >> temporary.name >> temporary.borrowsum >> temporary.book_num >> temporary.borrowday;
			for (int j = 0; j<10; j++)
			{
				tfin >> temporary.b[j];
			}
			data_1.push_back(temporary);
		}
		tfin.close();
	}
}
int Library::read_num(int a)
{
	//SortStudent(1);
	return data_1[a - 1].borrowsum;
}
int Library::getborrownum()//获取本月外借量
{
	return borro_book;
}
int Library::getreturnnum()//获取本月还书量
{
	return return_book;
}
int Library::GetStudent()
{
	int k = (int)data_1.size();
	return k + 1;
}
void Library::AddBook(book NewBook)
{
	data.push_back(NewBook);
}
void Library::AddStudent(student_infomation newstudent)
{
	data_1.push_back(newstudent);
}
void Library::DeleteBook(string bookname, string author)
{
	int pos = SearchBookPosWithAB(author, bookname);
	if (pos != -1)
	{
		data.erase(data.begin() + pos);
		return;
	}
	else
		cout << "没有此书！\n";
}
void Library::BorrowBook(string name, string author)
{
	string BorrowDate;
	string BackDate;
	char c;
	int flag = 0;
	SortStudent(1);
	int sid = -1;
	for (int i = 0; i <(int)data.size(); i++)
	{
		if (data[i].name == name&&data[i].author == author)
		{
			if (data[i].nowsum)
			{
				cout << "借书读者的读者号是：";
				cin >> sid;
				if (data_1[sid - 1].book_num>10)
				{
					cout << "现你同时借了6本书！不可再借！" << endl;
					break;
				}
				flag = 1;
				data[i].nowsum = data[i].nowsum - 1;
				data[i].BorrowCount = data[i].BorrowCount + 1;
				cout << "输入借阅日期" << endl;
				cin >> BorrowDate;
				data[i].borrowdate = BorrowDate;
				cout << "输入归还日期(最多借一月)" << endl;
				cin >> BackDate;
				data[i].returndate = BackDate;
				data[i].ok = bool(data[i].nowsum);
				borro_book++;
				data_1[sid - 1].book_num++;
				for (int j = 0; j<10; j++)
				{
					if (data_1[sid - 1].b[j] == 0)
						data_1[sid - 1].b[j] = data[i].book_id;
					Save();
					Save_student();
				}
			}
			else
			{
				cout << "这书被借走了！是否预约？Y/N" << endl;
				cin >> c;
				c = toupper(c);
				if (c == 'Y')
					data[i].appointment++;
			}
		}
	}
	if (!flag)
		cout << "抱歉，未找到您要找的书。" << endl;
}
void Library::BackBook(string name, string author, int k)
{
	int c = -1;
	SortStudent(1);
	if (k != -1)
	{
		c = k - 1;
	}
	else
	{
		cout << "请输入你的读者号：";
		cin >> c;
		c = c - 1;
	}
	for (int i = 0; i <(int)data.size(); i++)
	{
		if (data[i].name == name&&data[i].author == author)
		{
			data[i].nowsum = data[i].nowsum + 1;
			data[i].ok = bool(data[i].nowsum);
			return_book++;
			if (data[i].appointment != 0)
			{
				data[i].appointment--;
				betoli.push_back(data[i].book_id);
			}
			for (int j = 0; j<10; j++)
			{
				if (data_1[c].b[j] == data[i].book_id)
					data_1[c].b[j] = 0;
			}
			data_1[c].book_num--;
			break;
		}
	}
	Save();
	Save_student();
}
void Library::printbook(book a)
{
	cout << setw(8) << a.book_id;
	cout << setw(14) << a.name;
	cout << setw(14) << a.author;
	cout << setw(14) << fixed << setprecision(2) << a.price;
	cout << setw(14) << a.kind;
	cout << setw(14) << a.room;
	cout << setw(14) << a.sum;
	cout << setw(14) << a.nowsum;
	cout << setw(14) << a.BorrowCount;
	cout << setw(10) << (a.ok == 0 ? "不可借" : "可借");
	cout << setw(14) << a.appointment;
	cout << setw(14) << a.borrowdate;
	cout << setw(14) << a.returndate << endl;
	// cout<<endl;
}
void Library::PrintToLi()
{
	SortBook(1);
	int k = (int)betoli.size();
	if (!k) cout << "     暂时无预约书到馆！" << endl;
	else
	{
		cout << "        到馆的预约书有：\n";
		for (int i = 0; i<k; i++)
		{
			printbook(data[betoli[i] - 1]);
		}
	}
}
void Library::PrintStudent(int kid)
{
	int id = kid - 1;
	//id=SerchStudent(id);
	SortStudent(1);
	SortBook(1);
	//SortStudent(1);
	cout << setw(8) << data_1[id].id;
	cout << setw(8) << data_1[id].name;
	cout << setw(14) << data_1[id].borrowsum;
	cout << setw(18) << data_1[id].book_num;
	cout << setw(14) << data_1[id].borrowday << endl;
	if (data_1[id].book_num)
	{
		cout << "你当前借了这些书：\n";
		cout << setw(16) << "检索号" << setw(16) << "书名" << setw(16) << "作者" << endl;
		for (int i = 0; i<10; i++)
		{
			if (data_1[id].b[i] != 0)
				cout << setw(16) << data[data_1[id].b[i] - 1].book_id << setw(16) << data[data_1[id].b[i] - 1].name << setw(16) << data[data_1[id].b[i] - 1].author << endl;
		}
	}
	else
		cout << "你当前并未借任何书,快去借本书看看吧！\n";
}
void Library::ShowAllBook()
{
	//system("cls");
	cout << setw(8) << "检索号" << setw(14) << "书名" << setw(14) << "作者" << setw(14) << "价格" << setw(14) << "种类" << setw(14) << "馆藏地" << setw(14) << "总库存量" << setw(14) << "在馆量" << setw(14) << "借阅量" << setw(10) << "是否可借" << setw(14) << "预约量";
	cout << setw(14) << "借出时间" << setw(14) << "归还时间" << endl;
	for (int i = 0; i <(int)data.size(); i++)
	{
		printbook(data[i]);
	}
}
int Library::SerchStudent(int id)
{
	int m = -1;
	for (int i = 0; i <(int)data_1.size(); i++)
	{
		if (data_1[i].id == id)
		{
			return i;
		}
	}
	return m;
}
void Library::SearchBookPosWithname(string thebook)//按书名查询
{
	int flag = 0;
	for (int i = 0; i <(int)data.size(); i++)
	{
		if (data[i].name == thebook)
		{
			printbook(data[i]);
			flag = 1;
		}
	}
	if (!flag) cout << "查无此书！\n";
}
void Library::SearchBookPosWithAuthor(string theauthor)//按作者查询
{
	bool flag = false;
	for (int i = 0; i <(int)data.size(); i++)
	{
		if (data[i].author == theauthor)
		{
			flag = true;
			printbook(data[i]);
		}
	}
	if (!flag) cout << "查无此作者的书！";
}
void Library::SearchBookPosWithKind(string kind)//按种类查询
{
	bool flag = false;
	for (int i = 0; i <(int)data.size(); ++i)
	{
		if (data[i].kind == kind)
		{
			flag = true;
			printbook(data[i]);
		}
	}
	if (!flag) cout << "查无此类书！";
}
int Library::SearchBookPosWithAB(string theauthor, string thebook)//按作者和书名查询
{
	for (int i = 0; i <(int)data.size(); ++i)
	{
		if (data[i].author == theauthor&&data[i].name == thebook)
		{
			printbook(data[i]);
			return i;
		}
	}
	cout << "查无此书！";
	return -1;
}
int Library::SearchBookPosWithid(int id)
{
	for (int i = 0; i <(int)data.size(); i++)
	{
		if (data[i].book_id == id)
		{
			return i;
		}
	}
	return -1;
}
void Library::SortBook(int ca)  //排序
{
	if (ca == 0)   //按借阅量排序
	{
		sort(data.begin(), data.end(), cmpByCount);
	}
	else    //按检索号排序
	{
		sort(data.begin(), data.end(), cmpByidnum);
	}
}
void Library::SortStudent(int ca)
{
	if (ca == 0)//按借阅量排序
		sort(data_1.begin(), data_1.end(), cmpByBorrowsum);
	else
		sort(data_1.begin(), data_1.end(), cmpByid);//按读者号排序
}
void Library::Save() //书籍文件
{
	ofstream fout("book.txt");
	if (fout)
	{
		fout << data.size() << endl;
		//book p;
		for (int i = 0; i <(int)data.size(); i++)
		{
			fout << data[i].book_id << " " << data[i].name << " " << data[i].author << " " << data[i].price << " " << data[i].kind << " " << data[i].room << " " << data[i].sum << " " << data[i].nowsum << " " << data[i].BorrowCount << " " << data[i].ok << " " << data[i].appointment << " " << data[i].borrowdate << " " << data[i].returndate << " " << endl;
		}
		fout.close();
	}
}
void Library::Save_student() //学生信息
{
	ofstream fout("student_infomation.txt");
	if (fout)
	{
		fout << data_1.size() << endl;
		//student_infomation p;
		for (int i = 0; i <(int)data_1.size(); i++)
		{
			fout << data_1[i].id << " " << data_1[i].name << " " << data_1[i].borrowsum << " " << data_1[i].book_num << " " << data_1[i].borrowday;
			for (int j = 0; j<10; j++)
			{
				fout << " " << data_1[i].b[j];
			}
			fout << endl;
		}
		fout.close();
	}
}
void Library::revisebook(string name, string author)//修改
{
	string Room, Kind;
	int num, k = 0;
	printf("你要修改的内容是：\n");
	printf("                 1.馆藏地修改\n");
	printf("                 2.库存量修改\n");
	printf("                 3.所属种类修改\n");
	printf("                 4.退出\n");
	for (int i = 0; i <(int)data.size(); i++)
	{
		if (data[i].author == author&&data[i].name == name)
		{
			k = i;
			break;
		}
	}
	int cho;
	do
	{
		cin >> cho;
		switch (cho)
		{
		case 1:
		{
			cout << "请输入新的馆藏地：\n";
			cin >> Room;
			data[k].room = Room;
			break;
		}
		case 2:
		{
			cout << "请输入新的总库存量：\n";
			cin >> num;
			data[k].sum = num;
			break;
		}
		case 3:
		{
			cout << "请输入图书所属新种类：\n";
			cin >> Kind;
			data[k].kind = Kind;
			break;
		}
		}
	} while (cho<4);
}
void Library::Appointment(string bookname, string author)//预约图书
{
	for (int i = 0; i <(int)data.size(); i++)
	{
		if (data[i].author == author&&data[i].name == bookname)
		{
			if (data[i].nowsum>0) printf("馆内有书，你无须预约！\n");
			else
			{
				data[i].appointment++;
				printf("预约成功！");
			}
		}
	}
}
int main()
{
	cout.setf(ios::left);//左对齐
	Library mybook;
	string mm;//密码数组
			  // mybook.PrintStudent(0);
	char *p = "1960593106";
	cout << "                                                   欢迎使用图书管理系统" << endl;
	cout << "请选择你的登录选项：\n";
	cout << "                  1.管理员登录" << endl;
	cout << "                  2.读者登录" << endl;
	int cho, start, kk = 1;
	cin >> start;
	cin.get();
	switch (start)
	{
	case 1:
	{

		// string passwdInput;
		cout << "请输入管理员口令:";
		while (1)
		{
			string paaword = "1960593106";
			getline(cin, mm);
			if (!mm.compare(paaword))
			{
				cout << "密码正确,正在进入系统....." << endl;
				break;
			}
			else
			{
				cout << "密码不正确,请重新输入" << endl;

			}
		}
		do
		{
			// cout<<"                                  欢迎使用图书管理系统"<<endl;
			cin.clear();
			cin.sync();
			//system("cls");
			cout << "              1.所有图书目录 " << endl;
			cout << "              2.查询图书 " << endl;
			cout << "              3.增加图书 " << endl;
			cout << "              4.删除图书 " << endl;
			cout << "              5.借阅图书 " << endl;
			cout << "              6.归还图书 " << endl;
			cout << "              7.修改图书信息 " << endl;
			cout << "              8.本月借还统计 " << endl;
			cout << "              9.预约书到馆情况 " << endl;
			cout << "              10.退出 " << endl;
			cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "              请选择功能,输入指令 " << endl;
			cin >> cho;
			switch (cho)
			{
			case 1:
			{
				int cho2;
				do
				{
					mybook.ShowAllBook();
					cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
					cout << "0:按借阅量排序 , 1:按索引号排序 ,3:退出" << endl;
					cin >> cho2;
					switch (cho2)
					{
					case 0:
						mybook.SortBook(0);
						break;
					case 1:
						mybook.SortBook(1);
						break;
					}
				} while (cho2<2);
				break;
			}
			case 2:
			{
				cout << "         1.按书名查询 " << endl;
				cout << "         2.按作者查询 " << endl;
				cout << "         3.按种类查询 " << endl;
				cout << "         4.按书名和作者查询 " << endl;
				cout << "         5.退出 " << endl;
				cout << "         请选择功能,输入指令 " << endl;
				int select;
				do
				{
					string Name, AutHor, Kind;
					cin >> select;
					switch (select)
					{
					case 1:
						cout << "请输入书名！" << endl;
						cin >> Name;
						mybook.SearchBookPosWithname(Name); //按书名查询
						break;
					case 2:
						cout << "请输入作者！" << endl;
						cin >> AutHor;
						mybook.SearchBookPosWithAuthor(AutHor);//按作者查询
						break;
					case 3:
						cout << "请输入种类！" << endl;
						cin >> Kind;
						mybook.SearchBookPosWithKind(Kind);//按种类查询
						break;
					case 4:
						cout << "请输入作者和书名！" << endl;
						cin >> AutHor >> Name;
						mybook.SearchBookPosWithAB(AutHor, Name);//按作者和书名查询
						break;
					}
				} while (select <= 4 && select >= 1);
				break;
			}
			case 3:         //增加图书
			{
				book temp;
				cout << "请输入检索号:";
				cin >> temp.book_id;
				while (mybook.SearchBookPosWithid(temp.book_id)>-1)
				{
					cout << "检索号重复!" << endl;
					cin >> temp.book_id;
				}
				cin.clear();
				cin.sync();
				cout << "书名:";
				cin >> temp.name;
				cout << "作者：";
				cin >> temp.author;
				cout << "价格:";
				cin >> temp.price;
				cout << "种类:";
				cin >> temp.kind;
				cout << "馆藏地址:";
				cin >> temp.room;
				cout << "数量:";
				cin >> temp.sum;
				temp.nowsum = temp.sum;
				temp.BorrowCount = 0;
				temp.ok = true;
				temp.appointment = 0;
				temp.borrowdate = "0000.00.00";
				temp.returndate = "0000.00.00";
				mybook.AddBook(temp);
				mybook.Save();
				cout << "信息保存成功" << endl;
				break;
			}
			case 4:         //删除图书
			{
				string bookname, bookauthor;
				cout << "请输入书名和作者:" << endl;
				cin >> bookname >> bookauthor;
				mybook.DeleteBook(bookname, bookauthor);
				break;
			}
			case 5:         //借书
			{
				string bookname, bookauthor;
				cout << "请输入要借的书名和作者：" << endl;
				cin >> bookname >> bookauthor;
				mybook.BorrowBook(bookname, bookauthor);
				mybook.Save();
				break;
			}
			case 6:         //还书
			{
				string bookname, bookauthor;
				cout << "请输入要还的书名和作者：" << endl;
				cin >> bookname >> bookauthor;
				mybook.BackBook(bookname, bookauthor, -1);
				mybook.Save();
				break;
			}
			case 7:
			{
				string name, author;
				cout << "请输入要修改的书名和作者：" << endl;
				cin >> name >> author;
				mybook.revisebook(name, author);
				break;
			}
			case 8:
			{
				printf("                     本月借书%d次\n", mybook.getborrownum());
				printf("                     本月还书%d次\n", mybook.getreturnnum());
				break;
			}
			case 9:
			{
				mybook.PrintToLi();
				cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
				break;
			}
			}
		} while (cho<10);
		break;
		// mybook.Save();
	}
	case 2:
	{
		int bh, cho, k;
		cout << "请输入你的读者编号：\n";
		cin >> bh;
		if (mybook.SerchStudent(bh) == -1)
		{
			int n;
			cout << "你不是此系统读者，是否注册？\n";
			cout << "                    1.注册\n";
			cout << "                    2.退出\n";
			cin >> n;
			student_infomation temp;
			if (n == 1)
			{
				cout << "请输入你的姓名:";
				cin >> temp.name;
				cin.clear();
				cin.sync();
				temp.id = mybook.GetStudent();
				temp.borrowsum = 0;
				temp.book_num = 0;
				temp.borrowday = "0000.00.00";
				for (int i = 0; i<10; i++)
				{
					temp.b[i] = 0;
				}
				mybook.AddStudent(temp);
				mybook.Save_student();
				//cout<<"信息保存成功"<<endl;
				cout << "            注册成功！请记住你的读书号\n";
				cout << "            姓名:" << temp.name << endl;
				cout << "            读书号：" << temp.id << endl;
				k = temp.id;
			}
			else
				break;
		}
		else
		{
			k = bh;
		}
		do
		{	
			cin.clear();
			cin.sync();//清除缓冲区
			cout << "              1.所有图书目录 " << endl;
			cout << "              2.查询图书 " << endl;
			cout << "              3.归还图书 " << endl;
			cout << "              4.预约图书" << endl;
			cout << "              5.查看我的借阅信息 " << endl;
			cout << "              6.退出 " << endl;
			cout << "              请选择功能,输入指令 " << endl;
			cin >> cho;
			switch (cho)
			{
			case 1:
			{
				int cho2;
				do
				{
					mybook.ShowAllBook();
					cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
					cout << "0:按借阅量排序 , 1:按索引号排序 ,3:退出" << endl;
					cin >> cho2;
					switch (cho2)
					{
					case 0:
						mybook.SortBook(0);
						break;
					case 1:
						mybook.SortBook(1);
						break;
					}
				} while (cho2<2);
				break;
			}
			case 2:
			{
				cout << "              1.按书名查询 " << endl;
				cout << "              2.按作者查询 " << endl;
				cout << "              3.按种类查询 " << endl;
				cout << "              4.按书名和作者查询 " << endl;
				cout << "              5.退出 " << endl;
				cout << "              请选择功能,输入指令 " << endl;
				int select;
				do
				{
					string Name, AutHor, Kind;
					cin >> select;
					switch (select)
					{
					case 1:
						cout << "请输入书名！" << endl;
						cin >> Name;
						mybook.SearchBookPosWithname(Name); //按书名查询
						break;
					case 2:
						cout << "请输入作者！" << endl;
						cin >> AutHor;
						mybook.SearchBookPosWithAuthor(AutHor);//按作者查询
						break;
					case 3:
						cout << "请输入种类！" << endl;
						cin >> Kind;
						mybook.SearchBookPosWithKind(Kind);//按种类查询
						break;
					case 4:
						cout << "请输入作者和书名！" << endl;
						cin >> AutHor >> Name;
						mybook.SearchBookPosWithAB(AutHor, Name);//按作者和书名查询
						break;
					}
				} while (select <= 4 && select >= 1);
				break;
			}
			case 3:         //还书
			{
				string bookname, bookauthor;
				cout << "请输入要还的书名和作者：" << endl;
				cin >> bookname >> bookauthor;
				mybook.BackBook(bookname, bookauthor, k);
				break;
			}
			case 4://预约图书
			{
				string bookname, author;
				cout << "请输入你要预约书的书名和作者:" << endl;
				cin >> bookname >> author;
				mybook.Appointment(bookname, author);//预约图书
				break;
			}
			case 5://查阅个人信息
			{
				//mybook.SortStudent(1);
				cout << setw(8) << "读者号" << setw(8) << "姓名" << setw(14) << "历史借阅次数" << setw(18) << "现在借阅书个数" << setw(14) << "上次还书时间" << endl;
				mybook.PrintStudent(k);
				mybook.SortStudent(0);
				printf("你在所有读者中借阅量排名第 %d 位\n", mybook.SerchStudent(k) + 1);
				printf("所有读者中的最高借阅量是：%d次\n", mybook.read_num(1));
				mybook.SortStudent(1);
				cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
				break;
			}
			}
		} while (cho<6);
		break;
	}
	}
	mybook.Save();
	mybook.Save_student();
	return 0;
}



bool cmpByidnum(book a, book b)
{
	return a.book_id<b.book_id;
}
bool cmpByCount(book a, book b)
{
	return a.BorrowCount>b.BorrowCount;
}
bool cmpByBorrowsum(student_infomation a, student_infomation b)
{
	return a.borrowsum>b.borrowsum;
}
bool cmpByid(student_infomation a, student_infomation b)
{
	return a.id<b.id;
}
