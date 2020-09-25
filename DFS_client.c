#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DFS.h"

void dfsrog_1(char *host)
{
	CLIENT *clnt;

	char line[255];
	char *word;
	char *words[255];
	char **getdir_result;
	char *nextlist_1_arg;
	char *closelist_1_arg;
	char *filecount_1_arg;
	char *nextread_1_arg;
	char *remote_filename;
	char *local_filename;
	char *condition_arg;
	char *closefile_1_arg;

	const char s[2] = " ";

	direntry dirtry;
	direntry *dir_next;
	file_block *fblock;
	file_block cl_fblock;
	file_block *rd_fblock;

	int *num_files;
	int num_block = 0;
	int prompt_number = 1;
	long int total_bytes;
	long int left_write_bytes;
	long int write_bytes = 0;
	long int read_bytes = 0;

	FILE *clnt_fp;

	bool_t *cd_result;
	bool_t *dir_open;
	bool_t *dir_close;
	bool_t *file_open;
	bool_t *file_close;
	bool_t *file_read;
	bool_t *file_write;

	clnt = clnt_create(host, DFSROG, DFSOPERATIONS, "tcp");
	if (clnt == NULL)
	{
		clnt_pcreateerror(host);
		exit(1);
	}

	/*prompt for user*/
	void prompt()
	{
		/*format specifier escape sequence %% is %*/
		printf("%%%d%% ", prompt_number);
	}

	for (;;)
	{
		int cnt = 0;
		prompt();
		scanf(" %[^\n]", line);

		/* get the first word */
		word = strtok(line, s);

		/* walk through line */
		while (word != NULL)
		{
			words[cnt] = word;
			word = strtok(NULL, s);
			cnt++;
		}
		words[cnt] = NULL;

		/*terminate when enter done*/
		if ((strcmp(words[0], "done")) == 0)
			break;
		else
		{
			++prompt_number;
		}

		if (strcmp(words[0], "getdir") == 0 && cnt < 2)
		{
			getdir_result = getdir_1(words[0], clnt);
			if (getdir_result == (char **)NULL)
			{
				clnt_perror(clnt, "getdir failed");
			}
			printf("getdir succeeded with %s\n", *getdir_result);
		}
		else if (strcmp(words[0], "cd") == 0 && cnt < 3)
		{
			if (cnt < 2)
			{
				fprintf(stderr, "directory name is missing\n");
			}
			else
			{
				cd_result = changedir_1(&words[1], clnt);
				if (*cd_result == 0)
				{
					printf("cd failed\n");
				}
				else
				{
					printf("cd succeeded\n");
				}
			}
		}
		else if (strcmp(words[0], "ls") == 0 && cnt < 4)
		{
			nextlist_1_arg = "NONE";
			if (words[1] != NULL && (strcmp(words[1], "-l") == 0))
			{
				dirtry.flag = 0;
				words[1] = (words[2] != NULL)? words[2] : ".";
				dir_open = openlist_1(&words[1], clnt);
				if (*dir_open == 0)
				{
					clnt_perror(clnt, "open directory failed");
				}

				dir_next = nextlist_1(&nextlist_1_arg, clnt);
				while (dir_next->flag != -1)
				{
					/*dictory */
					if (dir_next->flag == 4)
					{
						strcat(dir_next->entryname, "/");
						printf("%-16s%-12d%-24s%-12s\n", dir_next->entryname, dir_next->size, dir_next->date, "UTC");
					}
					else
					{
						printf("%-16s%-12d%-24s%-12s\n", dir_next->entryname, dir_next->size, dir_next->date, "UTC");
					}
					dir_next = nextlist_1(&nextlist_1_arg, clnt);
				}
				dir_close = closelist_1(&closelist_1_arg, clnt);
				if (*dir_close = 0)
				{
					clnt_perror(clnt, "error occur when close dirctory ls failed");
				}
			}
			else if (words[1] != NULL && (strchr(words[1], '>') != NULL || strchr(words[1], '<') != NULL || strchr(words[1], '=') != NULL))
			{
				if (words[2] == NULL)
				{
					words[2] = ".";
				}
				dirtry.flag = 0;
				dir_open = openlist_1(&words[2], clnt);
				if (*dir_open == 0)
				{
					clnt_perror(clnt, "open directory failed");
				}
				dir_next = nextlist_1(&words[1], clnt);
				if (dir_next->flag == -10)
				{
					clnt_perror(clnt, "invalid command or typo");
				}
				else
				{
					while (dir_next->flag != -1)
					{
						char *tmp = dir_next->entryname;
						if (tmp[0] != '\0')
						{
							printf("%s\n", dir_next->entryname);
						}
						dir_next = nextlist_1(&words[1], clnt);
					}
				}
				dir_close = closelist_1(&closelist_1_arg, clnt);
				if (*dir_close = 0)
				{
					clnt_perror(clnt, "error occur when close dirctory ls failed");
				}
			}
			else if(words[1] != NULL && cnt <3)
			{
				if (words[1] == NULL)
				{
					words[1] = ".";
				}
				dirtry.flag = 0;
				dir_open = openlist_1(&words[1], clnt);
				if (*dir_open == 0)
				{
					clnt_perror(clnt, "open directory failed");
				}
				dir_next = nextlist_1(&nextlist_1_arg, clnt);
				while (dir_next->flag != -1)
				{
					/*dictory */
					if (dir_next->flag == 4)
					{
						printf("%s/\n", dir_next->entryname);
					}
					else
					{
						printf("%s\n", dir_next->entryname);
					}
					dir_next = nextlist_1(&nextlist_1_arg, clnt);
				}
				dir_close = closelist_1(&closelist_1_arg, clnt);
				if (*dir_close = 0)
				{
					clnt_perror(clnt, "error occur when close dirctory ls failed");
				}
			}
			else
			{
				fprintf(stderr, "invalid command or typo\n");
			}
		}
		else if (strcmp(words[0], "put") == 0 && cnt < 4)
		{
			if (words[1] != NULL)
			{
				local_filename = words[1];
				clnt_fp = fopen(local_filename, "r");
				if (clnt_fp == NULL)
				{
					clnt_perror(clnt, "local file not found\n");
				}
				else
				{
					//get total bytes of client file
					fseek(clnt_fp, 0, SEEK_END);
					total_bytes = ftell(clnt_fp);
					cl_fblock.size = total_bytes;
					left_write_bytes = total_bytes;
					fclose(clnt_fp);

					remote_filename = (words[2] != NULL) ? words[2] : words[1];
					file_open = openfiletowrite_1(&remote_filename, clnt);
					if (*file_open == 0)
					{
						clnt_perror(clnt, "open file failed\n");
					}
					else
					{
						//iniltize block number
						cl_fblock.num_block = 0;
						while (write_bytes < total_bytes)
						{
							if (left_write_bytes < 512)
							{
								clnt_fp = fopen(local_filename, "r");
								if (num_block != 0)
								{
									fseek(clnt_fp, num_block * 510, SEEK_CUR);
								}
								fread(cl_fblock.data, left_write_bytes, 1, clnt_fp);
								fclose(clnt_fp);
								cl_fblock.size = left_write_bytes;
								file_write = nextwrite_1(&cl_fblock, clnt);
								if (*file_write == 0)
								{
									fprintf(stderr, "write failed\n");
								}
								write_bytes = write_bytes + left_write_bytes;
							}
							else
							{
								// leave 2 bytes space in case
								write_bytes = write_bytes + 510;
								clnt_fp = fopen(local_filename, "r");

								fseek(clnt_fp, num_block * 510, SEEK_CUR);

								fread(cl_fblock.data, 510, 1, clnt_fp);
								fclose(clnt_fp);
								++num_block;
								file_write = nextwrite_1(&cl_fblock, clnt);
								if (*file_write == 0)
								{
									fprintf(stderr, "write failed\n");
								}
								cl_fblock.num_block = num_block;
								left_write_bytes = left_write_bytes - 510;
								cl_fblock.size = left_write_bytes;
							}
						}
						file_close = closefile_1(&closefile_1_arg, clnt);
						if (*file_close == 0)
						{
							fprintf(stderr, "close file failed\n");
						}
						printf("put succeeded transferring %ld bytes\n", total_bytes);
					}
				}
			}
			else
			{
				fprintf(stderr, "miss file name\n");
			}
		}
		else if (strcmp(words[0], "get") == 0 && cnt < 4)
		{
			if (words[1] == NULL)
			{
				fprintf(stderr, "miss file name\n");
			}
			else
			{
				local_filename = (words[2] != NULL) ? words[2] : words[1];
				clnt_fp = fopen(local_filename, "r");
				if (clnt_fp == NULL)
				{
					fprintf(stderr, "%s not found\n", local_filename);
				}
				else
				{
					fclose(clnt_fp);
					remote_filename = words[1];
					file_open = openfiletoread_1(&remote_filename, clnt);
					if (*file_open == 0)
					{
						clnt_perror(clnt, "read file failed\n");
					}
					else
					{
						rd_fblock = nextread_1(&nextread_1_arg, clnt);
						num_block = rd_fblock->num_block;
						read_bytes = rd_fblock->size;
						clnt_fp = fopen(local_filename, "w+");
						int i;
						for (i = 0; i < num_block; i++)
						{
							fwrite(rd_fblock->data, 1, 510, clnt_fp);
							rd_fblock = nextread_1(&nextread_1_arg, clnt);
						}
						fwrite(rd_fblock->data, 1, read_bytes - num_block * 510, clnt_fp);
						fclose(clnt_fp);
						file_close = closefile_1(&closefile_1_arg, clnt);
						if (*file_close == 0)
						{
							fprintf(stderr, "close file failed\n");
							exit(1);
						}
						printf("get succeeded transferring %ld bytes\n", read_bytes);
					}
				}
			}
		}
		else if (strcmp(words[0], "randomread") == 0 && cnt < 5)
		{
			if (words[2] == NULL || words[3] == NULL || words[1] == NULL)
			{
				fprintf(stderr, "num of bytes missing or start off poistion missing or file name missing\n");
			}
			else if (atoi(words[2]) > 512 || atoi(words[3]) > 512)
			{
				fprintf(stderr, "maxium is 512 bytes\n");
			}
			else
			{
				remote_filename = words[1];
				file_open = openfiletoread_1(&remote_filename, clnt);
				if (*file_open == 0)
				{
					clnt_perror(clnt, "read file failed\n");
				}
				else
				{
					cl_fblock.offset = atoi(words[2]);
					cl_fblock.size = atoi(words[3]);
					rd_fblock = randomread_1(&cl_fblock, clnt);
					file_close = closefile_1(&closefile_1_arg, clnt);
					if (*file_close == 0)
					{
						fprintf(stderr, "close file failed\n");
						exit(1);
					}
					if (rd_fblock->flag == -1)
					{
						printf("you reach the file size limit %ld bytes only transferring %d bytes\n", rd_fblock->size, rd_fblock->rr_bytes);
					}
					else
					{
						printf("randomread succeeded transferring %d bytes\n", rd_fblock->rr_bytes);
					}
					printf("%s\n", rd_fblock->data);
				}
			}
		}
		else if (strcmp(words[0], "filecount") == 0)
		{
			condition_arg = (cnt == 1) ? "NONE" : words[1];
			num_files = filecount_1(&condition_arg, clnt);
			if (num_files == (int *)NULL)
			{
				clnt_perror(clnt, "filecount failed");
			}
			if (*num_files >= 0)
			{
				printf("filecount succeeded with count of %d\n", *num_files);
			}
			else
			{
				fprintf(stderr, "commmand invalid or operator is missing\n");
			}
		}
		else
		{
			fprintf(stderr, "No such command implemented or checking typo\n");
		}
	}
}

int main(int argc, char *argv[])
{
	char *host;

	if (argc < 2)
	{
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];
	dfsrog_1(host);
	exit(0);
}
