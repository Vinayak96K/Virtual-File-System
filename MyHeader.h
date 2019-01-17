#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<dirent.h>

#define REGULAR 1
#define SPECIAL 2

#define READ 1
#define WRITE 2

#define START 0
#define CURRENT 1
#define END 2

#define TRUE 1
#define FALSE 0

#define MAX_INODES 50
#define MAX_FILE_SiZE 1024
#define MAX_FILE_NAME_SIZE 50

#define ERR_IncorrectParameters -1
#define ERR_NoInodes -2
#define ERR_NoMemory -2
#define ERR_NoDataFurther -2
#define ERR_InvalidFileType -3
#define ERR_FileAlreadyOpened -3
#define ERR_FileAreadyExits -3
#define ERR_UnableLocateFile -3
#define ERR_MemAllocationFailure -4
#define ERR_AccessDenied -4
#define ERR_FailedToWriteBackupData -4
#define ERR_FailedToReadBackupData -4
#define ERR_FileIsNotOpened -5
#define ERR_IndexOutOfUFDTarr -5
#define ERR_EndOfFile -5

#define BackupDir "./Backup"
#define BACKUPFILE "./BackupFile"
#define PWD "/vinayak/Projects/C/VirtualFileSystem$ "

typedef unsigned int BOOL;

#pragma pack(1)
typedef struct SuperBlock
{
    int Total_Blocks;
    int Free_Blocks;
}SUPERBLOCK;

#pragma pack(1)
typedef struct iNode
{
    char FileName[MAX_FILE_NAME_SIZE];
    int iNodeNumber;
    int Permission;
    char *Buffer;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int LinkCount;
    int Count;
    BOOL bckup;
    struct iNode * Next;
}INODE,* PINODE;

#pragma pack(1)
typedef struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    int RefCount;
    PINODE inode;
}FILETABLE,* PFILETABLE,** PPINODE;

#pragma pack(1)
typedef struct UserFileDescriptorTable
{
    PFILETABLE pFileTable;
}UFDT;

SUPERBLOCK sObj;
PINODE head;
UFDT UFDTarr[MAX_INODES];

//////////////////////////////////////////////To be defined in Command helper////////////////////////////////////////////////////
void ls_File();
void Man(char *);
void ls_OpenFile();
void DisplayHelp();
void CloseAllFiles();
int stat(char *);
int fstat(int);
int rmFile(char *);
int GetBackup();
int Restore();
int lSeek(int,int,int);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////To be defined in File helper//////////////////////////////////////////////////////
void InitializeSuperBlock();
void CreateDILB();
int CreateFile(char *,int);
int CloseFileByName(char *);
int OpenFile(char *,int);
int WriteFile(int,char *,char *);
int TruncateFile(char *);
int ReadFile(int, char *,int);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////To be defined in Function helper///////////////////////////////////////////////////
PINODE GetINodeByName(char *);
int GetFdByName(char *);
int GetFdByMode(char *,int);
int StrCpy(int *,char *,char *);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////