#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>

#define MAXINCODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
    int TotalInodes;
    int FreeInode;
}SUPERBLOCK,*PSUPERBLOCK;

typedef struct inode
{
    char Filename[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int RefernceCount;
    int permission;
    struct inode *next;
}INODE,*PINODE,**PPINODE;

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode;
    PINODE ptrinode;
}FILETABLE,*PFILETABLE;

typedef struct ufdt
{
    PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head=NULL;

void man(char *name)
{
    if(name ==NULL )return;

    if(strcmp(name,"create")==0)
    {
        printf("Description:Used to create new regular file\n");
        printf("Usage :create File_name Ppermission\n");
    }
    else if(strcmp(name,"read")==0)
    {
        printf("Description:Used to read data from reagular file\n");
        printf("Usage:create File_name No_Of_Bytes_Read\n");
    }
    else if(strcmp(name,"write")==0)
    {
        printf("Description:Used to write data from reagular file\n");
        printf("Usage:write File_name\n  After this enter the data that you want to write\n");
    }
    else if(strcmp(name,"ls")==0)
    {
        printf("Description:Used to list all information of file\n");
        printf("Usage:ls\n");
    }
    else if(strcmp(name,"stat")==0)
    {
        printf("Description:Used to display information file\n");
        printf("Usage:stat File_name\n");
    }
    else if(strcmp(name,"fstat")==0)
    {
        printf("Description:Used to remove data from file\n");
        printf("Usage:stat File_name\n");
    }
    else if(strcmp(name,"truncate")==0)
    {
        printf("Description:Used to remove data from file\n");
        printf("Usage:truncate File_name \n");
    }
    else if(strcmp(name,"open")==0)
    {
        printf("Description:Used to open existing file\n");
        printf("Usage:open File_name\n");
    }
    else if(strcmp(name,"close")==0)
    {
        printf("Description:Used to close opened file\n");
        printf("Usage:close File_name\n");
    }
    else if(strcmp(name,"closeall")==0)
    {
        printf("Description:Used to close opened file\n");
        printf("Usage:close File_name\n");
    }
    else if(strcmp(name,"lseek")==0)
    {
        printf("Description:Used to close file offset\n");
        printf("Usage:lseek File_name ChangeInOffset StartPoint\n");
    }
    else if(strcmp(name,"rm")==0)
    {
        printf("Description:Used to delete file\n");
        printf("Usage:rm File_name\n");
    }
    else
    {
        printf("ERROR:No manual entry available\n");
    }
}

void DisplayHelp()
{
    printf("ls:To List out all file\n");
    printf("clear:To clear console\n");
    printf("open:To open file \n");
    printf("close:To close the file");
    printf("closeall:To close all opende file\n");
    printf("read:To Read the contents from file\n");
    printf("write:To write contents from file\n");
    printf("exit:To teminate file system\n");
    printf("stat:To Display infromation of file using name\n");
    printf("fstat:To Display information of file using name\n");
    printf("truncate:To Remove all data from file\n");
    printf("rm:To Delete the file\n");

}

int GetFDFromName(char *name)
{
    int i=0;

    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable  !=NULL)
            if(strcmp((UFDTArr[i].ptrfiletable->ptrinode->Filename),name)==0)
            break;
            i++;        
    }

    if(i==50)   return -1;
    else        return i;
}

PINODE Get_Inode(char *name)
{
    PINODE temp=head;
    int i=0;

    if(name==NULL)
        return NULL;

        while(temp!=NULL)
        {
            if(strcmp(name,temp->Filename)==0)
            break;
            temp=temp->next;
        }
        return temp;   
}

void CreateDILB()
{
    int i=1;
    PINODE newn =NULL;
    PINODE temp=head;

    while(i<=MAXINCODE)
    {
        newn =(PINODE)malloc(sizeof(INODE));

        newn->LinkCount=0;
        newn->RefernceCount=0;
        newn->FileType=0;
        newn->FileSize=0;

            newn->Buffer=NULL;
            newn->next=NULL;

            newn->InodeNumber=i;

            if(temp==NULL)
            {
                head=newn;
                temp=head;
            }
            else
            {
                temp->next=newn;
                temp=temp->next;
            }
            i++;
    }
    printf("DILB created succesfully\n");
}

void InitialiseSuperBlock()
{
    int i=0;
    while(i<MAXINCODE)
    {
        UFDTArr[i].ptrfiletable=NULL;
        i++;
    }

    SUPERBLOCKobj.TotalInodes=MAXINCODE;
    SUPERBLOCKobj.FreeInode=MAXINCODE;
}

int CreateFile(char *name,int permission)
{
    int i=3;
    PINODE temp=head;

    if((name==NULL) || (permission ==0) || (permission >3))
    return -1;

    if(SUPERBLOCKobj.FreeInode==0)
        return -2;
    
    (SUPERBLOCKobj.FreeInode)--;

    if(Get_Inode(name)!=NULL)
        return -3;
    
    while(temp!=NULL)
    {
        if(temp->FileType==0)
            break;
        temp=temp->next;
    }

    while(i<50)
    {
        if(UFDTArr[i].ptrfiletable==NULL)
        break;
        i++;
    }
    
UFDTArr[i].ptrfiletable=(PFILETABLE)malloc(sizeof(FILETABLE));

UFDTArr[i].ptrfiletable->count=1;
UFDTArr[i].ptrfiletable->mode=permission;
UFDTArr[i].ptrfiletable->readoffset=0;
UFDTArr[i].ptrfiletable->writeoffset=0;

UFDTArr[i].ptrfiletable->ptrinode=temp;
}


int rm_file()
{
    return 0;
}

int Read_File()
{
    return 0;
}

int WriteFile()
{
    return 0;
}

int OpenFile()
{
    return 0;
}

void CloseFileByName()
{

}

int CloseFileByName()
{
    return 0;
}

void CloseAllFile()
{

}

int LseekFile()
{

}

void ls_File()
{

}

int fstat_file()
{
    return 0;
}

int stat_file()
{
    return 0;
}

int truncate_File()
{

}

void ls_file()
{
    int i=0;
    PINODE temp=head;

    if(SUPERBLOCKobj.FreeInode==MAXINCODE)
    {
        printf("Error:There are no files\n");
        return;
    }

    printf("\nFile Name\tInode Number\tFile Size \tLink count\n");
    printf("-----------------------------------------------------\n");
    while(temp!=NULL)
    {
        if(temp->FileType !=0)
        {
            printf("%s\t\t%d\t\t%d\n",temp->Filename,temp->InodeNumber,temp->FileActualSize,temp->LinkCount);
        }
        temp=temp->next;
    }
    printf("--------------------------------------------------------------\n");
}

int main()
{
    char *ptr=NULL;
    int ret=0,fd=0,count=0;
    char command[4][80],str[80],arr[1024];

    InitialiseSuperBlock();
    CreateDILB();

    while(1)
    {
        fflush(stdin);
        strcpy(str,"");

        printf("\nAkshata VFS: >");

        fgets(str,80,stdin);//scanf("%[^'\n]s",str);

        count=sscanf(str,"%s %s %s %s ",command[0],command[1],command[2],command[3]);

        if(count==1)
        {
            if(strcmp(command[0],"ls")==0)
            {
                ls_file();
            }
            else if(strcmp(command[0],"closeall")==0)
            {
                CloseAllFile();
                printf("All files closed Succesfully\n");
                continue;
            }
            else if(strcmp(command[0],"clear")==0)
            {
                system("cls");
                continue;
            }
            else if(strcmp(command[0],"help"==0))
            {
                DisplayHelp();
                continue;
            }
            else if(strcmp(command[0],"exit")==0)
            {
                printf("Termintating the Akshata Virtual file System\n");
                break;
            }
            else
            {
                printf("\nERROR:Command not Found!!!\n");
                continue;
            }
        }
            else if(count==2)
            {
                if(strcmp(command[0],"stat")==0)
                {
                    ret=stat_file(command[1]);
                    if(ret==-1)
                        printf("ERROR:Incorrect parameters\n");
                    if(ret==-2)
                        printf("ERROR:There is no such file\n");
                        continue;
                }
                else if(strcmp(command[0],"fstat")==0)
                {
                    ret=fstat_file(atoi(command[1]));
                    if(ret==-1)
                        printf("ERROR:Incorrect parameters\n");
                    if(ret==-2)
                        printf("ERROR:There is no such file\n");
                        continue;
                }
                else if(strcmp(command[0],"close")==0)
                {
                    ret=CloseFileByName(command[1]);
                    if(ret==-1)
                        printf("ERROR:There is no such file\n");
                        continue;
                }
                else if(strcmp(command[0],"rm")==0)
                {
                    ret=rm_file(command[1]);
                    if(ret==-1)
                        printf("ERROR:There is no such file\n");
                        continue;
                }
                else if(strcmp(command[0],"man")==0)
                {
                    man(command[1]);  
                }
                else if(strcmp(command[0],"write")==0)
                {
                    fd=GetFDFromName(command[1]);
                    if(ret==-1)
                    {
                        printf("ERROR:There is no such file\n");
                        continue;
                    }
                    printf("Enter the data:\n");
                    scanf("%[^\n]",arr);

                    ret=strlen(arr);
                    if(ret==0)
                    {
                        printf("ERROR:Incorrrect parameter\n");
                        continue;
                    }    
                    ret=WriteFile(fd,arr,ret);
                    if(ret==-1)
                        printf("ERROR:Permission denied\n");
                    if(ret==-2)
                        printf("ERROR:There is no suffiecient memory to write\n");
                    if(ret==-3)
                        printf("ERROR:It is not regular file\n");
                }
                else if(strcmp(command[0],"truncate")==0)\
                {
                    ret=truncate_File(command[1]);
                    if(ret==-1)
                    printf("ERROR:Incroorect parameter\n");
                }
                else
                {
                    printf("\nERROR:Command not found !!!\n");
                    continue;
                }
            }
            else if(count==3)
            {
                if(strcmp(command[0],"create")==0)
                {
                    ret=CreateFile(command[1],atoi(command[2]));
                    if(ret>=0)
                        printf("File is successfully created withe file descriptor:%d\n",ret);
                    if(ret==-1)
                        printf("ERROR:Incorrect paramters\n");
                    if(ret==-2)
                        printf("ERROR:There is no inodes\n");
                    if(ret==-3)
                        printf("ERROR:File already exits\n");
                    if(ret==-4)
                        printf("ERROR:Memory allocation failure\n");
                    continue;
                }
                else if(strcmp(command[0],"open")==0)
                {
                    ret=CreateFile(command[1],atoi(command[2]));
                    if(ret>=0)
                        printf("File is successfully created with file descriptor:%d\n",ret);
                    if(ret>=-1)
                        printf("ERROR:Incrorect paramters\n");
                    if(ret>=-2)
                        printf("ERROR:File not present\n");
                    if(ret>=-3)
                        printf("ERROR:Permisssion denied\n");
                    continue;
                }
                else if(strcmp(command[0],"read")==0)
                {
                    fd=GetFDFromName(command[1]);
                    if(fd==-1)
                    {
                        printf("ERROR:Incccoredct paramter\n");
                        continue;
                    }
                    ptr=(char*)malloc(sizeof(atoi(command[2]))+1);
                    if(ptr==NULL)
                    {
                        printf("ERROR:Memeory allocation is failure\n");
                        continue;
                    }
                    ret=ReadFile(fd,ptr,atoi(command[2]));
                    if(ret==-1)
                        printf("ERROR:File not exisiting\n");
                    if(ret==-2)
                        printf("ERROR:Permission denied\n");
                    if(ret==-3)
                        printf("ERROR:Reached at end of file\n");
                    if(ret==-4)
                        printf("ERROR:It is not regular file\n");
                    if(ret>0)
                        printf("ERROR:File empty\n");
                    if(ret>0)
                    {
                        write(2,ptr,ret);
                    }
                    continue;
                }
                else
                {
                    printf("\nERROR:command not found!!!\n");
                    continue;
                }
            }
            else if(count==4)
            {
                if(strcmp(command[0],"lseek")==0)
                {
                fd=GetFDFromName(command[1]);
                if(fd==-1)
                {
                    printf("ERROR:incorrect parameter\n");
                    continue;
                }
                ret=LseekFile(fd,atoi(command[2]),atoi(command[3]));
                if(ret==-1)
                {
                    printf("ERROR:Unable to perform lssek\n");
                }
            }
            else 
            {
                printf("\nERROR:Command not found !!!\n");
                continue;
            }
    }
        else
        {
            printf("\nERROR :COmmand not Found !!!\n");
            continue;
        }
    }
    return 0;
}



































































