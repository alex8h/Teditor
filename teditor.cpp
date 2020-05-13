#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

static string text;
static int MAX = 200;
static int MIN = 20;
static string buff;
static int * buff_num = new int[MAX];
static string * command = new string[MIN];

int find(string str,int i)
{
    return text.find(str,i);
}
int * find_all(string s)
{
    int *index = new int[MAX];
    for(int i = 0; i < MAX; i++)
        index[i] = 0;
    int k = 0, position = 0;
    while(true)
    {
        index[k] = find(s, position);
        if(index[k] == -1)
            return index;
        position = index[k] + 1;
        k++;
    }
    return index;
}

void copy(int line, int column, int number)
{
    buff = '\0';
    buff_num = NULL;
    int position = 0;
    for( int  k = 0; k != line; position++)
        if(text[position] == '\n')
            k++;
    buff = text.substr(position + column,number);
}

void paste(int line, int column)
{
    int position = 0;
    for( int  k = 0; position < buff.size() && k != line; position++)
        if(text[position] == '\n')
            k++;
    text.insert(position + column,buff);
}

void cut(int line, int column, int number)
{
    buff = '\0';
    buff_num = NULL;
    int position = 0;
    for( int  k = 0; k != line; position++)
        if(text[position] == '\n')
            k++;
    buff = text.substr(position + column,number);
    text.erase(position + column,number);
}

void copy_all(string s)
{
    buff = s;
    buff_num = NULL;
    buff_num = find_all(s);
}

void paste_all(bool sign,int number)
{//sign = 1, если был copy_all
    if(sign)
        for(int i = 0,k = 1; buff_num[i] != -1; i++, k++)
            text.insert(buff_num[i] + buff.size() * k + number,buff);
    else
        for(int i = 0,k = 0; buff_num[i] != -1; i++, k++)
            text.insert(buff_num[i] - buff.size() * k + number,buff);
}

void cut_all(string s)
{
    buff = s;
    buff_num = NULL;
    buff_num = find_all(s);
    for(int i = 0,k = 0; buff_num[i] != -1; i++, k++)
    {
        text.erase(buff_num[i] - buff.size() * k ,buff.size());
    }
}


void print(string s)
{
    for(int i = 0; i < s.size(); i++)
    {
        if(s[i] == '\n')
            cout<<endl;
        cout<<s[i];
    }
    cout<<endl;

}

int main()
{
    cout<<"****************************************************************"<<endl<<endl;

    fstream f;
    f.open("text.txt");
    getline(f,text,'\0');
    f.close();
    int *index = new int[MAX];
    print(text);




    buff = '\0';

    fstream ff;
    ff.open("command.txt");
    getline(ff,buff,'\0');
    ff.close();
    int j = 0;
    for(int i = 0; i < buff.size(); i++)
    {
        if( ! (buff[i] == '\n') )
        {
            command[j] += buff[i];
        }
        else
            j++;
    }
    int n = j;
    buff = '\0';


    for(int i = 0; i < n; i++)
    {
        string s = command[i].substr(0,3);

        if(s ==  "cop")
        {
            if(command[i].substr(5,3) == "all")
                copy_all(command[i].substr(9));
            else
            {
                int line, column, number;
                line = atoi(command[i].substr(5,1).c_str());
                column = atoi(command[i].substr(7,1).c_str());
                number = atoi(command[i].substr(9,1).c_str());
                copy(line, column, number);
            }
            }
        if(s == "pas")
        {
            if(command[i].substr(6,3) == "all")
            {   int number = atoi(command[i].substr(9).c_str());
                if(command[i-1].substr(0,3) == "cut")
                    paste_all(0,number);
                if(command[i-1].substr(0,3) == "cop")
                    paste_all(1,number);
            }
            else
            {
                int line, column;
                line = atoi(command[i].substr(6,1).c_str());
                column = atoi(command[i].substr(8,1).c_str());
                paste(line, column);
            }
        }
        if(s == "cut")
        {
            if(command[i].substr(4,3) == "all")
                cut_all(command[i].substr(8));
            else
            {
                int line, column, number;
                line = atoi(command[i].substr(4,1).c_str());
                column = atoi(command[i].substr(6,1).c_str());
                number = atoi(command[i].substr(8,1).c_str());
                cut(line, column, number);
            }

        }
        if (s =="und")
        {
            if(command[i-1].substr(0,3) == "cop")
            {
                buff = '\0';
                buff_num = NULL;
            }
            if(command[i-1].substr(0,3) == "pas")
            {
                if(command[i-1].substr(3,3) == "all")
                {
                    buff_num = NULL;
                    buff_num = find_all(buff);
                    for(int i = 0,k = 0; buff_num[i] != -1; i++, k++)
                    {
                        if(i & 1)
                            text.erase(buff_num[i] - buff.size() * k ,buff.size());
                    }
                    buff_num = NULL;
                }
                else
                {
                    int line, column, number;
                    line = atoi(command[i-1].substr(4,1).c_str());
                    column = atoi(command[i-1].substr(6,1).c_str());
                    number = buff.size();
                    cut(line, column, number);
                }

            }
            if(command[i-1].substr(0,3) == "cut")
            {
                if(command[i-1].substr(5,3) == "all")
                    paste_all(0,0);
                else
                {
                    int line, column;
                    line = atoi(command[i-1].substr(6,1).c_str());
                    column = atoi(command[i-1].substr(8,1).c_str());
                    paste(line, column);
                }
            }
            if(command[i-1].substr(0,3) == "und")
            {
                cout<<"ERR"<<endl;

            }
            //break;
        }
    }


    print(text);



    cout<<endl<<"****************************************************************"<<endl;
    return 0;
}
