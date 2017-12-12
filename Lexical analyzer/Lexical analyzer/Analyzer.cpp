#include<iostream>
#include<string>
#include<fstream>
#include<istream>
using namespace std;

bool IsDigit(char ch)
{
	if (ch >= '0'&&ch <= '9')
		return true;
	return false;
}//判断是否为数字


bool IsId(char ch)
{
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		return true;
	return false;
}//判断是否为标识符



int IsKeyword(string str,string keyword[28])
{
	for (int i = 0; i<28; i++)
	{
		if (str == keyword[i])
		{
			return i+1;
		}
	}
	return 0;
}//判断是否为关键字


int IsDelimiter(char ch,char de[8])
{
	for (int i = 0; i<8; i++)
	{
		if (ch == de[i])
			return i+1;
	}
	return 0;
}//判断是否为界符

void process(string phrase, string keyword[28], string op[23], char de[8])
{
	int state = 0;//状态
	phrase = phrase + ' ';
	int count = phrase.length();//字符串长度
	int i = 0;
	char ch;
	string str;
	fstream out("out.txt");
	out.seekp(0, ios::end);
	while (i < count)
	{
		switch (state)
		{
		case 0:
			ch = phrase[i];
			str = str + ch;
			i++;
			if (ch == ' ')
			{
				state = 0;
				str = "";
			}
			else if (IsId(ch))//是字母
			{
				state = 1;
			}
			else if (IsDigit(ch))//是数字
			{
				state = 3;
			}
			else if (IsDelimiter(ch,de))//是界符
			{
				if (ch == ';'||ch==','||ch==')'||ch==']'||ch=='}')
				{
					state = 10;
					out << "(界符，  " << str << "  )" << endl;
					str ="";
				}
				else if (ch == '('||ch=='['||ch=='{')
				{ 
					state = 14;
					out << "(界符，  " << str << "  )" << endl;
					str = "";
				}

			}
			else if (ch == '+'||ch=='-')//运算符
			{
				state = 29;
			}
			else if (ch == '*'||ch=='/'||ch=='%'||ch=='^'||ch=='~')
			{
				state = 10;
				out << "(运算符，  " << str << " )" << endl;
				 str ="";
			}
			else if (ch == '='||ch=='&'||ch=='<'||ch=='>'||ch=='|'||ch=='!')
			{
				state = 62;
			}
			break;
		case 1:
			ch = phrase[i];
			if (IsDigit(ch) || IsId(ch))
			{
				str = str + ch;
				state = 1;
				i++;
			}
			else
			{
				state = 0;
				if (IsKeyword(str, keyword))
				{
					out << "(关键字， " << str << " )" << endl;
				}
				else
				{
					out << "(变量名，  " << str << " )" << endl;
				}
				str = "";
			}
			break;
		case 3:
			ch = phrase[i];
			if (IsDigit(ch))
			{
				str = str + ch;
				state = 3;
				i++;
			}
			else if (ch == 'e')
			{
				str = str + ch;
				state = 6;
				i++;
			}
			else if (ch == '.')
			{
				str = str + ch;
				state = 4;
				i++;
			}
			else
			{
				state = 0;
				out << "(数字， " << str << " )" << endl;
				 str ="";
			}
			break;
		case 4:
			ch = phrase[i];
			if (IsDigit(ch))
			{
				str = str + ch;
				state = 5;
				i++;
			}
			else
				out << "error" << endl;
			break;
		case 5:
			ch = phrase[i];
			if (IsDigit(ch))
			{
				str = str + ch;
				state = 5;
				i++;
			}
			else if (ch == 'e')
			{
				str = str + ch;
				state = 6;
				i++;
			}
			else
			{
				state = 0;
				out << "(数字， " << str << " )" << endl;
				 str ="";
			}
			break;
		case 6:
			ch = phrase[i];
			if (IsDigit(ch))
			{
				str = str + ch;
				state = 8;
				i++;
			}
			else if (ch == '+' || ch == '-')
			{
					str = str + ch;
					state = 7;
					i++;
			}
			else
			{
				out << "error" << endl;
			}
			break;
		case 7:
			ch = phrase[i];
			if (IsDigit(ch))
			{
				str = str + ch;
				state = 8;
				i++;
			}
			else
			{
				out << "error" << endl;
			}
			break;
		case 8:
			ch = phrase[i];
			if (IsDigit(ch))
			{
				str = str + ch;
				state = 8;
				i++;
			}

			else
			{
				state = 0;
				out << "(数字， " << str << " )" << endl;
				 str ="";
			}
			break;
		case 10:
			state = 0;
			break;
		case 14:
			ch = phrase[i];
			state = 0;
			if (ch == ')'||ch==']'||ch=='}')
			{
				str = str + ch;
				i++;
				out << "(界符， " << str << "  )" << endl;
				str ="";
			}
			break;
		case 29:
			ch = phrase[i];
			if (ch == '+'||ch=='='||ch=='-')
			{
				state = 0;
				str = str + ch;
				i++;
				out << "(运算符， " << str << "  )" << endl;
				 str ="";
			}
			else if (IsDigit(phrase[i - 2]) || IsId(phrase[i - 2]))
			{
				state = 0;
				out << "(运算符， " << str << "  )" << endl;
				 str ="";	
			}
			else
			{
				state = 7;
			}
			break;
		case 62:
			ch = phrase[i];
			state = 0;
			if(ch=='='||ch=='|'||ch=='&')
			{
				str = str + ch;
				i++;
				out << "(运算符， " << str << "  )" << endl;
			}
			else
			{
				out << "(运算符， " << str << "  )" << endl;
			}
			str = "";
			break;
		}
	}
}//词法处理函数

int main()
{
	string keyword[28] = { "bool","break","case","catch","char","const","continue","default","delete","do","double","else","enum","false","float","for","if","int","long","new","return","short","struct","switch","true","void","while","main" };
	string op[23] = { "+","-","*","/","%","^","&","~","!","=","-=","+=","==","!=","<=",">=","&&","<",">","|","||","++","--" };
	char de[8] = { ';',',','(',')','[',']','{','}' };
	ifstream file("test.txt");
	string line;
	while (getline(file, line))
	{
		cout << line;
		process(line, keyword, op, de);//调用词法分析函数，每次分析一句
	}
	return 0;
}
