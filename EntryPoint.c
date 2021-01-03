#include"MyHeader.h"
extern FILE *stdin;

int main(int argc, char const *argv[])
{
    char cPWD[MAX_FILE_SiZE] = {0};
    if(getcwd(cPWD,MAX_FILE_SiZE-1)==NULL)
    {
        printf("Error: Unable to get current working directory...!");
        return -1;
    }
    char cmd[4][80],str[80],arr[MAX_FILE_SiZE];
    int iRet=0,fd=0,count=0;
    char *ptr=NULL;
    PINODE temp,temp2;
    InitializeSuperBlock();
    CreateDILB();
    while(1)
    {
        str[0]='\0';
        fflush(stdin);
        printf("%s$ ",cPWD);
        fgets(str,80,stdin);
        count = sscanf(str,"%s %s %s %s",cmd[0],cmd[1],cmd[2],cmd[3]);

        if(count==0)
        {
            continue;
        }
        else if(count==1)
        {
            if(strcasecmp(cmd[0],"exit")==0)
            {
                temp=head;
                while(temp!=NULL)
                {
                    if(temp->Buffer!=NULL)
                    {
                        free(temp->Buffer);
                    }
                    temp2=temp->Next;
                    free(temp);
                    temp=temp2;
                }
                iRet=0;
                while((iRet<MAX_INODES))
                {
                    if(UFDTarr[iRet].pFileTable!=NULL)
                    {
                        free(UFDTarr[iRet].pFileTable);
                        UFDTarr[iRet].pFileTable=NULL;
                    }
                    iRet++;
                }
                //fptr=NULL;
                break;
            }
            else if(strcasecmp(cmd[0],"ls")==0)
            {
                ls_File();
            }
            else if(strcasecmp(cmd[0],"clear")==0)
            {
                system("clear");
            }
            else if(strcasecmp(cmd[0],"help")==0)
            {
                DisplayHelp();
            }
            else if(strcasecmp(cmd[0],"closeall")==0)
            {
                CloseAllFiles();
            }
            else if(strcasecmp(cmd[0],"backup")==0)
            {
                iRet=GetBackup();
                if(iRet>=0)
                {
                    printf("Success: Got backup of %d files.\n",iRet);
                }
                else if(iRet==ERR_FileIsNotOpened)
                {
                    printf("Error: No file is created!\n");
                }
                else if(iRet==ERR_UnableLocateFile)
                {
                    printf("Error: Unable to locate backup file!\n");
                }
                else if(iRet==ERR_FailedToWriteBackupData)
                {
                    printf("Error: Failed to write backup file!\n");
                }
            }
            else if(strcasecmp(cmd[0],"restore")==0)
            {
                iRet=Restore();
                if(iRet>=0)
                {
                    printf("Success:Restored %d files.\n",iRet);
                }
                else if(iRet==ERR_FailedToReadBackupData)
                {
                    printf("ERROR: Failed to read backup data!\n");
                }
                else if(iRet==ERR_EndOfFile)
                {
                    printf("ERROR: Unnecessary end of file!\n");
                }
            }
            else
            {
                printf("Error: Command not found\n");
            }
            continue;
        }
        else if(count==2)
        {
            if(strcasecmp(cmd[0],"close")==0)
            {
                iRet=CloseFileByName(cmd[1]);
                if((iRet==ERR_IndexOutOfUFDTarr))
                {
                    printf("Error: File is not created or opened!\n");
                }
                else if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect file name!\n");
                }
                else if(iRet<=0)
                {
                    printf("Success: File closed successflly!\n");
                }
            }
            else if(strcasecmp(cmd[0],"write")==0)
            {
                iRet=GetFdByMode(cmd[1],WRITE);
                if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect input!\n");
                }
                else if(iRet==ERR_IndexOutOfUFDTarr)
                {
                    printf("Error: File is not opened or created!\n");
                }
                else if(iRet>=0)
                {
                    printf("File found with file descriptor %d.\n",iRet);
                    printf("Enter the data:-");
                    fgets(arr,MAX_FILE_SiZE,stdin);
                    iRet=WriteFile(iRet,arr);
                    if(iRet==ERR_IncorrectParameters)
                    {
                        printf("Error: Incorrect input!\n");
                    }
                    else if(iRet==ERR_AccessDenied)
                    {
                        printf("Error: Access denied!\n");
                    }
                    else if(iRet==ERR_NoMemory)
                    {
                        printf("Error: Memory full!\n");
                    }
                    else if(iRet==ERR_InvalidFileType)
                    {
                        printf("Error: File is not regular!\n");
                    }
                    else if(iRet>=0)
                    {
                        printf("Success: Written %d bytes in %s.\n",iRet,cmd[1]);
                    }
                }
            }
            else if(strcasecmp(cmd[0],"ls")==0)
            {
                if(strcasecmp(cmd[1],"-o")==0)
                {
                    ls_OpenFile();
                    continue;
                }
                else
                {
                    printf("Error: Command not found!\n");
                    continue;
                }
            }
            else if(strcasecmp(cmd[0],"man")==0)
            {
                Man(cmd[1]);
                continue;
            }
            else if(strcasecmp(cmd[0],"stat")==0)
            {
                iRet=stat(cmd[1]);
                if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: File not found!\n");
                    continue;
                }
            }
            else if(strcasecmp(cmd[0],"fstat")==0)
            {
                iRet=fstat(atoi(cmd[1]));
                if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: No file found!\n");
                    continue;
                }
            }
            else if(strcasecmp(cmd[0],"truncate")==0)
            {
                iRet=TruncateFile(cmd[1]);
                if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect parameters!\n");
                }
                else if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: File is not opened or created!\n");
                }
                else if(iRet>=0)
                {
                    printf("Success: File data has been successfully erased!\n");
                }
            }
            else if(strcasecmp(cmd[0],"rm")==0)
            {
                iRet=rmFile(cmd[1]);
                if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect parameters!\n");
                }
                else if(iRet==ERR_NoInodes)
                {
                    printf("Error: File not found!\n");
                }
                else if(iRet==ERR_FileAreadyExits)
                {
                    printf("Error: File is opened!\n");
                }
                else if(iRet>0)
                {
                    printf("Success: File has successfully deleted!\n");
                }
            }
            else
            {
                printf("Error: Command not found!\n");
                continue;
            }
        }
        else if(count==3)
        {
            if(strcasecmp(cmd[0],"create")==0)
            {
                iRet=CreateFile(cmd[1],atoi(cmd[2]));
                if(iRet>=0)
                {
                    printf("File has been successfully created with file descriptor %d\n",iRet);
                }
                else if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect input!\n");
                }
                else if(iRet==ERR_FileAreadyExits)
                {
                    printf("Error: File already exists!\n");
                }
                else if(iRet==ERR_NoInodes)
                {
                    printf("Error: No iNodes available!\n");
                }
                else if(iRet==ERR_MemAllocationFailure)
                {
                    printf("Error: Failed to allocate memory!\n");
                }
            }
            else if(strcasecmp(cmd[0],"open")==0)
            {
                iRet=OpenFile(cmd[1],atoi(cmd[2]));
                if(iRet>=0)
                {
                    printf("Success: File is successfully opened %d mode with file desrciptor %d.\n",atoi(cmd[2]),iRet);
                }
                else if(iRet==ERR_AccessDenied)
                {
                    printf("Error: Could not open file in specified mode. Permission denied!\n");
                }
                else if(iRet==ERR_FileAlreadyOpened)
                {
                    printf("Error: File is already opend in specified mode!\n");
                }
                else if(iRet==ERR_IndexOutOfUFDTarr)
                {
                    printf("Error: No space to open files!\n");
                }
                else if(iRet==ERR_NoInodes)
                {
                    printf("Error: File not found!\n");
                }
                else if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect parameters!\n");
                }
            }
            else if(strcasecmp(cmd[0],"write")==0)
            {
                if(strcasecmp(cmd[1],"-fd")==0)
                {
                    int iValue = atoi(cmd[2]);
                    char cTemp[3] = {0};
                    sprintf(cTemp,"%d",iValue);
                    if(iValue <0 || iValue >= MAX_INODES || (strcasecmp(cTemp,cmd[2])!=0))
                    {
                        printf("Error: Invalid Fd!\n");
                    }
                    else
                    {
                        printf("Enter the data:-");
                        fgets(arr,MAX_FILE_SiZE,stdin);
                        iRet=WriteFile(iValue,arr);
                        if(iRet==ERR_IncorrectParameters)
                        {
                            printf("Error: Incorrect input!\n");
                        }
                        else if(iRet==ERR_AccessDenied)
                        {
                            printf("Error: Access denied!\n");
                        }
                        else if(iRet==ERR_NoMemory)
                        {
                            printf("Error: Memory full!\n");
                        }
                        else if(iRet==ERR_InvalidFileType)
                        {
                            printf("Error: File is not regular!\n");
                        }
                        else if(iRet>=0)
                        {
                            printf("Success: Written %d bytes to file descriptor %d.\n",iRet,iValue);
                        }
                    }                    

                }
                else
                {
                    printf("Error: Command not found!\n Try 'help'\n");
                }
                
            }
            else if(strcasecmp(cmd[0],"read")==0)
            {
                iRet=GetFdByMode(cmd[1],READ);
                if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect input!\n");
                }
                else if(iRet==ERR_IndexOutOfUFDTarr)
                {
                    printf("Error: File is not opened or created!\n");
                }
                else if(iRet>=0)
                {
                    printf("File found with file descriptor %d.\n",iRet);
                    ptr= (char *) malloc(sizeof(atoi(cmd[2]))+1);
                    if(ptr==NULL)
                    {
                        printf("Error: Insufficient memeory!\n");
                        continue;
                    } 
                    iRet=ReadFile(iRet,ptr,atoi(cmd[2]));
                    if(iRet==ERR_IncorrectParameters)
                    {
                        printf("Error: Incorrect parameters!\n");
                    }
                    else if(iRet==ERR_AccessDenied)
                    {
                        printf("Error: Access Denied!\n");
                    }
                    else if(iRet==ERR_NoDataFurther)
                    {
                        printf("Error: Offset at end!\n");
                    }
                    else if(iRet==ERR_InvalidFileType)
                    {
                        printf("Error: File is not regular!\n");
                    }
                    else if(iRet>0)
                    {
                        printf("Read values:%s\n",ptr);
                    }
                    free(ptr);
                }
            }
            else
            {
                printf("Error: Command not found!\n");
            }
            continue;
        }
        else if(count==4)
        {
            if(strcasecmp(cmd[0],"lseek")==0)
            {
                fd=GetFdByName(cmd[1]);
                if(fd==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect parameters!\n");
                }
                iRet=lSeek(fd,atoi(cmd[2]),atoi(cmd[3]));
                if(iRet==ERR_IncorrectParameters)
                {
                    printf("Error: Incorrect parameters!\n");
                }
            }
            else if(strcasecmp(cmd[0],"read")==0)
            {
                if(strcasecmp(cmd[1],"-fd")==0)
                {
                    int iValue = atoi(cmd[2]);
                    if(iValue <0 || iValue >= MAX_INODES )
                    {
                        printf("Error: Invalid Fd!\n");
                    }
                    else
                    {
                        printf("File found with file descriptor %d.\n",iValue);
                        ptr= (char *) malloc(sizeof(atoi(cmd[3]))+1);
                        if(ptr==NULL)
                        {
                            printf("Error: Insufficient memeory!\n");
                            continue;
                        } 
                        iRet=ReadFile(iValue,ptr,atoi(cmd[3]));
                        if(iRet==ERR_IncorrectParameters)
                        {
                            printf("Error: Incorrect parameters!\n");
                        }
                        else if(iRet==ERR_AccessDenied)
                        {
                            printf("Error: Access Denied!\n");
                        }
                        else if(iRet==ERR_NoDataFurther)
                        {
                            printf("Error: Offset at end!\n");
                        }
                        else if(iRet==ERR_InvalidFileType)
                        {
                            printf("Error: File is not regular!\n");
                        }
                        else if(iRet>0)
                        {
                            printf("Read values:%s\n",ptr);
                        }
                        free(ptr);
                    }
                }
                else
                {
                    printf("Error: Command not found!\n Try 'help'\n");
                }
            }
            else
            {
                printf("Error: Command not found!\n");    
            }
        }
        else
        {
            printf("Error: Command not found!\n");
        }
    
    }
    printf("Exiting application...!\n");
    return 0;
}
