#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct state
{
   int nextlevel[10];
}State;

State s[100];
int index1=0,hash=0;;
int flag=0;

void addtrans1(int i,int j)
{
   if(s[i].nextlevel[0]==0)
    {
        s[i].nextlevel[0]=j;
    }
}

void addtrans2(int i,int j)
{
   if(s[i].nextlevel[1]==0)
    {
        s[i].nextlevel[1]=j;
    }
}

void addtrans3(int i,int j)
{
    if(s[i].nextlevel[2]==0)
    {
        s[i].nextlevel[2]=j;
    }
    else
         s[i].nextlevel[3]=j;
}

int nfa2(char str[])
{
    flag=1;
    int i=0,prev=0;
    while(str[i]!=')')
    {
       if(str[i]=='(')
        {
            char temp[1000];
            strcpy(temp,&str[i]);
            i+=nfa2(temp);
        }
        else if(strncmp(&str[i],"letter",6)==0)
        {
            prev=index1;
            addtrans3(index1,index1+1);

            index1++;
            addtrans1(index1,index1+1);
            i=i+6;
            index1++;
            addtrans3(index1,index1+1);
            index1++;
        }
       else if(strncmp(&str[i],"digit",5)==0)
        {
            prev=index1;
            addtrans2(index1,index1+1);
            i=i+5;
            index1++;
            addtrans2(index1,index1+1);
            index1++;
        }
       else if(str[i]=='|')
        {
            addtrans3(prev,index1+1);
            i++;
            prev=index1+1;
            if(strncmp(&str[i],"letter",6)==0)
            {
                addtrans1(prev,prev+1);
                prev++;
                i=i+6;
            }
            else if(strncmp(&str[i],"digit",5)==0)
            {
                 addtrans2(prev,prev+1);
                 prev++;
                 i=i+5;
            }
            addtrans3(prev,index1);
            s[prev].nextlevel[2]=index1;
            addtrans3(index1,prev+1);
            index1=prev;
        }
        else if(str[i]=='*')
        {
            hash=1;
            if(flag==1)
            {
                 addtrans3(s[index1].nextlevel[2],prev);
            }
            else
            {
                 index1++;
                 addtrans3(index1,prev);
            }
            addtrans3(prev-1,index1+1);
            i++;
        }
        else
            i++;


    }
    return i+1;
}
void epsnfa(char exp[])
{
    char str[100];
    strcpy(str,exp);
    int prev1=0,i=0;

    while(i<strlen(str))
    {
        if(str[i]=='(')
        {
            if(!hash)
               index1++;

            addtrans3(index1,index1+1);
            index1++;
            prev1=index1;
            char temp[1000];
            i++;
            strcpy(temp,&str[i]);
            i+=nfa2(temp);
        }
        else if(strncmp(&str[i],"letter",6)==0)
        {
            if(!hash)
               index1++;
            prev1=index1;
            addtrans1(index1,index1+1);
            i=i+6;
            index1++;
            addtrans3(index1,index1+1);
        }
        else if(strncmp(&str[i],"digit",5)==0)
        {
             index1++;
             prev1=index1;
             addtrans2(index1,index1+1);
             s[index1].nextlevel[1]=index1+1;
             i=i+5;
             index1++;
             addtrans3(index1,index1+1);
        }
        else if(str[i]=='|')
        {
             i++;
             prev1;
            if(strncmp(&str[i],"letter",6)==0)
            {
                 addtrans1(prev1,index1+1);
                 s[prev1].nextlevel[0]=index1+1;
                 prev1++;
                 i=i+6;
            }
            else if(strncmp(&str[i],"digit",5)==0)
            {
                 addtrans2(prev1,index1+1);
                 s[prev1].nextlevel[1]=index1+1;
                 prev1++;
                 i=i+5;
            }
            else
            {
                 addtrans3(prev1,index1);
                 s[prev1].nextlevel[2]=index1;
            }
        }
        else if(str[i]=='*')
        {
            hash=1;

            if(flag==1)
            {
                 addtrans3(s[index1].nextlevel[2],prev1);
            }
            else
            {
                 index1++;
                 addtrans3(index1,prev1);
                 addtrans3(index1,index1+1);
            }
            addtrans3(prev1-1,index1+1);
            i++;
        }
        else
            i++;
    }

}
int main()
{
    char exp[100];
    printf("Enter the Regular Expression\n");
    scanf("%[^\n]",exp);
    for(int i=0;i<20;i++)
    {
        s[i].nextlevel[0]=0;
        s[i].nextlevel[1]=0;
        s[i].nextlevel[2]=0;
    }
    epsnfa(exp);
    index1++;

    printf("\n********Transition table******\n");
    printf("States\t|letter\t|digit\t|%c\t|\n_________________________________\n",238);
    for(int i=1;i<index1;i++)
    {
       printf("%d\t|%d\t|%d\t|%2d",i,s[i].nextlevel[0],s[i].nextlevel[1],s[i].nextlevel[2]);
       s[i].nextlevel[3]? printf(",%2d  |\n",s[i].nextlevel[3] ): printf("     |\n");
    }
    printf("%d ---> Accepting state\n",index1);
    printf("\nNOTE: 0 state means NO Transitions!!!!\n");

    return 0;
}
