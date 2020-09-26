CS666 Advanced Distributed Systems 
Lab Assignment: Implementing a Distributed File System
Due: 11:59pm on 30 September, 2020

Objective: 
    The overall goal of this project is to build a distributed file system using SUN RPC (Remote Procedure Call) or Java RMI (Remote Method Inovocation) for distributed communication between the client and the server. The file server will serve as a directory and file contents of the underlying Unix file system. The Kernel will not need to be modified in this project. 

Group:
    Leyu Lin(jack) REDID:817372914
    Parth Thummar RedID:824679473

Tasks: 
    Leyu Lin(jack): Worked on Sequential Access File Operations and Random File Implementations. 
    Parth Thummar: Worked on Directory Operations and Filtering Mechanism.  

Files:
    DFS_client.c - client file
    DFS_server.c - server file

    DFS.x - protocls rpcgen split into server and client

    READMME.md - readme for project breakdown
    
    makefile - script to run project
    Readme_demo.docx - project demo is broken down to display each function implmented and supported with screenshots. 

    get_test_long.txt - test file destination for 'get' function 
    get_test.txt - test file destination for 'get' function 
    test_long.txt - test file with a large size (1600+ bytes)to test 'get' function
    test.txt - test file with a small size (20+ bytes) to test 'get' function

Function Usage: 
    DFS_client.c
        main() <int main (int argc, char *argv[]) at DFS_client.c:419>:
            printf()
            exit()
            dfsrog_1() <void dfsrog_1 (char *host) at DFS_client.c:6>:
                clnt_create()
                clnt_pcreateerror()
                exit()
                printf()
                scanf()
                strtok()
                strcmp()
                getdir_1()
                clnt_perror()
                fprintf()
                changedir_1()
                openlist_1()
                nextlist_1()
                strcat()
                closelist_1()
                strchr()
                fopen()
                fseek()
                ftell()
                fclose()
                openfiletowrite_1()
                fread()
                nextwrite_1()
                closefile_1()
                openfiletoread_1()
                nextread_1()
                fwrite()
                atoi()
                randomread_1()
                filecount_1()  

    DFS_server.c
        changedir_1_svc() <bool_t *changedir_1_svc (char **argp, struct svc_req rqstp) at DFS_server.c:29>:
            chdir()
        closefile_1_svc() <bool_t *closefile_1_svc (void *argp, struct svc_req rqstp) at DFS_server.c:641>:
            fclose()
        closelist_1_svc() <bool_t *closelist_1_svc (void *argp, struct svc_req rqstp) at DFS_server.c:608>:
            closedir()
        filecount_1_svc() <int *filecount_1_svc (char **argp, struct svc_req rqstp) at DFS_server.c:41>:
            opendir()
            strcmp()
            readdir()
            strchr()
            strtok()
            atoi()
            stat()
            localtime()
            closedir()
        getdir_1_svc() <char **getdir_1_svc (void *argp, struct svc_req rqstp) at DFS_server.c:17>:
            getcwd()
        nextlist_1_svc() <direntry *nextlist_1_svc (char **argp, struct svc_req rqstp) at DFS_server.c:332>:
            strcmp()
            readdir()
            strcpy()
            stat()
            strftime()
            localtime()
            memset()
            strchr()
            strtok()
            atoi()
        nextread_1_svc() <file_block *nextread_1_svc (void *argp, struct svc_req rqstp) at DFS_server.c:658>:
            fseek()
            ftell()
            fread()
        nextwrite_1_svc() <bool_t *nextwrite_1_svc (file_block *argp, struct svc_req rqstp) at DFS_server.c:625>:
            fwrite()
        openfiletoread_1_svc() <bool_t *openfiletoread_1_svc (char **argp, struct svc_req rqstp) at DFS_server.c:650>:
            fopen()
        openfiletowrite_1_svc() <bool_t *openfiletowrite_1_svc (char **argp, struct svc_req rqstp) at DFS_server.c:617>:
            fopen()
        openlist_1_svc() <bool_t *openlist_1_svc (char **argp, struct svc_req rqstp) at DFS_server.c:323>:
            opendir()
        randomread_1_svc() <file_block *randomread_1_svc (file_block *argp, struct svc_req rqstp) at DFS_server.c:682>:
            fseek()
            ftell()
            memset()
            fread()  