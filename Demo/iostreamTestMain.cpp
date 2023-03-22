//
// Created by wjc on 2023/3/23.
//
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <iomanip>
using namespace std;

/* ostream cout;
 * istream cin;
 * istream是basic_istream模板的一个特化
 * typedef basic_istream<char， char_traits<char>> istream;
 * typedef basic_ostream<char， char_traits<char>> ostream;
 * class basic_istream : virtual public basic_ios<Elem, _Traits>
 * basic_istream继承自basic_ios
 * ios是basic_ios模板的一个特化
 * typedef basic_ios<char， char_traits<char>> ios;
 *
 * cin为istream流类的对象 代表标准输入设备键盘
 * 后三个为ostream流类的对象  cout 代表标准输出设备显示器 cerr clog 代表错误信息输出设备显示器
 */

// 宽度控制
// 对齐控制
// 填充控制
// 精度控制
// 禁止输出
int main(void)
{
//    system("chcp 936");
    int n = 64;
    double d = 123.45;
    double d2 = 0.0187;

    cout<<"=================宽度控制===================="<<endl;
    cout<<n<<'#'<<endl;
    cout.width(10);
    cout<<n<<'#'<<endl;
//    cout<<setw(10)<<n<<'#'<<n<<endl; // 宽度控制不会影响下一个输出

    cout<<"=================对齐控制===================="<<endl;
    cout.width(10);
    cout.setf(ios::left);
    cout<<n<<'#'<<endl;
    cout.width(10);
    cout<<n<<'#'<<endl;     // 对齐控制会影响下一个输出
//    cout.width(10);
//    cout.setf(ios::right);
//    cout<<n<<'#'<<endl;

    cout.width(10);
    cout.unsetf(ios::left);
    cout<<n<<'#'<<endl;

//    cout<<setw(10)<<setiosflags(ios::left)<<n<<"#"<<endl;
//    cout<<setw(10)<<n<<"#"<<endl;   // 对齐控制会影响下一个输出
//    cout<<setw(10)<<setiosflags(ios::right)<<n<<"#"<<endl;
//    cout<<setw(10)<<resetiosflags(ios::left)<<n<<"#"<<endl;

    cout<<"=================填充控制===================="<<endl;
    cout<<setw(10)<<setfill('?')<<n<<'#'<<endl;
    cout<<setw(10)<<n<<"#"<<endl;    // 填充控制会影响下一个输出
    cout<<setw(10)<<setfill(' ')<<n<<"#"<<endl;

    cout<<"=================精度控制===================="<<endl;
    cout<<setprecision(4)<<d<<endl;
    cout<<setprecision(2)<<d2<<endl;

    cout<<setiosflags(ios::fixed);
    cout<<setprecision(4)<<d<<endl;
    cout<<setprecision(2)<<d2<<endl;

    cout<<"=================进度输出===================="<<endl;
    cout<<n<<endl;
//    cout<<ios::oct<<n<<endl;
//    cout<<ios::dec<<n<<endl;
//    cout<<oct<<n<<endl;
//    cout<<resetiosflags(ios::dec);
//    cout<<setiosflags(ios::oct)<<n<<endl;
////    cout<<resetiosflags(ios::dec);
//    cout<<resetiosflags(ios::oct);
//    cout<<setiosflags(ios::hex)<<n<<endl;
//    cout<<hex<<n<<endl;

    cout.setf(ios::showbase);
    cout<<n<<endl;
    cout.unsetf(ios::dec);
    cout.setf(ios::oct);
    cout<<n<<endl;

    cout.unsetf(ios::showbase);
    cout<<n<<endl;

//    cout<<oct<<n<<endl;
//    cout<<hex<<n<<endl;
//    cout<<endl;
//
//    cout<<setiosflags(ios::showbase);
//    cout<<dec<<n<<endl;
//    cout<<oct<<n<<endl;
//    cout<<hex<<n<<endl;
//
//    cout<<endl;
//    cout<<setbase(10)<<n<<endl;
//    cout<<setbase(8)<<n<<endl;
//    cout<<setbase(16)<<n<<endl;


    return 0;
}

string dtostr(double val)
{
    ostringstream oss;
    oss<<val;

    return oss.str();
}

double strtodouble(const string& str)
{
    istringstream iss(str);
    double val;
    iss>>val;
    return val;
}

int istringstream02main(void)
{
    double val = 55.55;

    string str = dtostr(val);
    cout<<str<<endl;

    str = "123.123";
    val = strtodouble(str);
    cout<<val<<endl;


    //192,168,0,100,   sscanf
    //sprintf
    istringstream iss("192,168,0,100");
    int v1;
    int v2;
    int v3;
    int v4;
    char ch;
    iss>>v1>>ch>>v2>>ch>>v3>>ch>>v4;

    ch = '.';
    ostringstream oss;
    oss<<v1<<ch<<v2<<ch<<v3<<ch<<v4;

    cout<<oss.str()<<endl;

    return 0;
}

int istringstreammain(void)
{
    string line;
    string word;

    while (getline(cin, line))
    {
        istringstream is(line);
//        is>>word;
//        cout<<word<<"#";
        while (is>>word)
            cout<<word<<"#";
    }
    cout<<endl;
    return 0;
}

int seekgmain(void)
{
    ifstream fin("test4.txt");
//    assert(fin);
    fin.seekg(2);

    char ch;
    fin.get(ch);
    cout<<ch<<endl;

    fin.seekg(-1, ios::end);
    fin.get(ch);
    cout<<ch<<endl;

    fin.seekg(0, ios::end);
    streampos pos = fin.tellg();
    cout<<pos<<endl;

    return 0;
}

struct Test
{
    int a;
    int b;
};

struct Test2
{
    int a;
    string b;
    string c;
};

int iofstream05main(void)
{
    Test2 t1;
    t1.a = 100;
    t1.b = "abcxxxssssdddddddddddddddsssssssssssssssssssssssssssssssssssssssssssssssssssssss";
    t1.c = "yyyeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";

    ofstream fout("test3.txt", ios::out | ios::binary);
//    fout.write((char*)&t1, sizeof(t1));
    fout.write((char*)&t1.a, sizeof(int));
    int len;
    len = t1.b.length();
    fout.write((char*)&len, sizeof(int));   // 写入的数据的指针  写入数据的字节大小
    fout.write(t1.b.data(), t1.b.length());
    len = t1.c.length();
    fout.write((char*)&len, sizeof(int));
    fout.write(t1.c.data(), t1.c.length());
    fout.close();

    ifstream fin("test3.txt", ios::in | ios::binary);
    Test2 t2;
    fin.read((char*)&t2.a, sizeof(int));   //第一个参数用来接收数据的指针 第二个参数 读取的字节数的大小
    fin.read((char*)&len, sizeof(int));
    t2.b.resize(len);
    fin.read(&t2.b[0], len);
    fin.read((char*)&len, sizeof(int));
    t2.b.resize(len);
    fin.read(&t2.b[0], len);

    cout<<t2.a<<" "<<t2.b<<" "<<t2.c<<endl;
    fin.close();

//    cout<< sizeof(t2.a)<<" "<< sizeof(t2.b)<<" "<< sizeof(t2.c)<<endl;
//
//    cout<< sizeof(Test2)<<endl;
//    cout<< sizeof(t2)<<endl;
//    string a = "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
//    cout<< sizeof(string)<<endl;
//    cout<< sizeof(a)<<endl;

    return 0;
}

int iofstream04main(void)
{
    Test2 t1;
    t1.a = 100;
    t1.b = "abcxxxssssdddddddddddddddsssssssssssssssssssssssssssssssssssssssssssssssssssssss";
    t1.c = "yyyeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";

    ofstream fout("test3.txt", ios::out | ios::binary);
    fout.write((char*)&t1, sizeof(t1));
    fout.close();

    ifstream fin("test3.txt", ios::in | ios::binary);
    Test2 t2;
    fin.read((char*)&t2, sizeof(Test2));
    cout<<t2.a<<" "<<t2.b<<" "<<t2.c<<endl;
    fin.close();

    cout<< sizeof(t2.a)<<" "<< sizeof(t2.b)<<" "<< sizeof(t2.c)<<endl;

    cout<< sizeof(Test2)<<endl;
    cout<< sizeof(t2)<<endl;
//    string a = "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd";
//    cout<< sizeof(string)<<endl;
//    cout<< sizeof(a)<<endl;

    return 0;
}

int iofstream03main(void)
{
    Test test = { 100, 200 };
    ofstream fout("test2.txt");
//    ofstream fout("test2.txt", ios::out | ios::binary);
    fout.write(reinterpret_cast<char*>(&test), sizeof(Test));
    fout.close();

    Test test2;
    ifstream fin("test2.txt");
//    ifstream fin("test2.txt", ios::in || ios::binary);
    fin.read(reinterpret_cast<char*>(&test2), sizeof(Test));
    cout<<test2.a<<" "<<test2.b<<endl;

    return 0;
}

int iofstream2main(void)
{
    ofstream fout("test.txt");
    assert(fout);
    char ch;

    for (int i = 0; i < 26; ++i) {
        ch = 'A' + i;
        fout.put(ch);
    }
    fout.close();

    ifstream fin("test.txt");
    while (fin.get(ch))
    {
        cout<<ch;
    }
    cout<<endl;

    return 0;
}

int iofstreammain(void) {
    ofstream fout("test.txt");
    fout<<"abc"<<" "<<200;
    fout.close();

    ifstream fin("test.txt");
    string s;
    int n;
    fin>>s>>n;
    cout<<s<<" "<<n<<endl;

    return 0;
}

int ofstream1main(void)
{
//    ofstream fout;
//    fout.open("test.txt");
      ofstream fout("test.txt");

//    if (fout.is_open())
//    {
//        cout<<"succ"<<endl;
//    }
//    else
//    {
//        cout<<"failed"<<endl;
//    }

//    if (fout.good())
//    {
//        cout<<"succ"<<endl;
//    }
//    else {
//        cout<<"failed"<<endl;
//    }

//    if (fout)
//    {
//        cout<<"succ"<<endl;
//    }
//    else {
//        cout<<"failed"<<endl;
//    }

//    if (!fout)
//    {
//        cout<<"failed"<<endl;
//    }
//    else {
//        cout<<"succ"<<endl;
//    }

    assert(fout);

    fout.close();
    return 0;
}

int cinmain(void)
{
//    int n;
//    char ch;
//
//    cin>>n>>ch;
//    cout<<"n="<<n<<" "<<"ch="<<ch<<endl;

//    int ch = cin.get();  // 读取单个字符  返回一个整数  字符的ASCII吗？ get对回车换行的处理
//    cout<<ch<<endl;

//    char ch1;
//    char ch2;
//    cin.get(ch1).get(ch2);  // 读取单个字符 返回一个istream对象的引用
//    cout<<ch1<<" "<<ch2<<endl;

//    char buf[10] = {0};
//    cin.getline(buf, 9);
//    cout<<buf<<endl;

//    char buf[10] = {0};
//    cin>>buf;
//    cout<<buf<<endl;

//    char buf[10] = {0};
//    cin.read(buf, 5);  // 返回一个istream对象的引用
//    cout<<buf<<endl;

    char c[10], c2, c3;

    c2 = cin.get();
    c3 = cin.get();
    cin.putback(c2);  // 将一个字符添加到流
    cin.getline(&c[0], 9); // 读取一行 遇到回车键  返回istream对象的引用
    cout<<c<<endl;

    return 0;
}

int coutmain(void)
{
    int n = 100;
    int n2 = 200;
    cout<<n<<" "<<n2<<endl;

    cout.put('H');   // 输出单个字符  返回一个ostream对象的引用
    cout.put('i');
    cout.put(' ');
    cout.put('H').put('i').put('\n');

    char buf[] = "test!!!!!!";
    cout.write(buf, 5);   // 返回一个ostream对象的引用

    return 0;
}