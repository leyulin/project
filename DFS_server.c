/**
 * @author: Leyu,Lin (jack)
 * REDID: 817372914
 *
 * @author: Parth Thummar
 * REDID: 824679473
 * 
 * All parts of project were attempted and implemented. 
 * Please see Readme_demo.docx for reference/screenshots. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <rpc/rpc.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "DFS.h"

/**gets and returns directory address*/
char **
getdir_1_svc(void *argp, struct svc_req *rqstp)
{
	static char *getdir_res;

	if ((getdir_res = getcwd(NULL, 0)) == NULL)
	{
		getdir_res = NULL;
	}
	return &getdir_res;
}

/**change directory and return bool_t*/
bool_t *
changedir_1_svc(char **argp, struct svc_req *rqstp)
{
	static bool_t cd_res;

	cd_res = (chdir(*argp) == -1) ? 0 : 1;

	return &cd_res;
}
DIR *pdir;
FILE *fp;
int bl_cnt = 0;


/**file count flie with condition if given*/
/*prase string to find out what condition given*/
/*only count files if speficed type=d then count directories*/
int *filecount_1_svc(char **argp, struct svc_req *rqstp)
{
	static int counts;
	int tmp = 0;
	char op[2] = " ";
	struct dirent *dr;
	struct stat attrs;
	char d[2] = "d";
	char h[2] = "h";
	char m[2] = "m";
	char s[2] = "s";
	char *words[5];
	char *val;
	int int_val;
	DIR *tdir;

	tdir = opendir(".");
	if (!strcmp(*argp, "NONE"))
	{
		while ((dr = readdir(tdir)) != NULL)
		{
			++tmp;
		}
	}
	else
	{
		/**parse string to get operators and operans*/
		if (strchr(*argp, '>') != NULL)
		{
			int cnt = 0;
			char op[2] = ">";
			char *token = strtok(*argp, op);
			while (token != NULL)
			{
				words[cnt] = token;
				token = strtok(NULL, op);
				cnt++;
			}
			words[2] = words[1];
			words[1] = ">";
		}
		else if (strchr(*argp, '<') != NULL)
		{
			int cnt = 0;
			char op[2] = "<";
			char *token = strtok(*argp, op);
			while (token != NULL)
			{
				words[cnt] = token;
				token = strtok(NULL, op);
				cnt++;
			}
			words[2] = words[1];
			words[1] = "<";
		}
		else if (strchr(*argp, '=') != NULL)
		{
			int cnt = 0;
			char op[2] = "=";
			char *token = strtok(*argp, op);
			while (token != NULL)
			{
				words[cnt] = token;
				token = strtok(NULL, op);
				cnt++;
			}
			words[2] = words[1];
			words[1] = "=";
		}
		else
		{
			counts = -9;
			return &counts;
		}

		if (!strcmp(words[0], "type"))
		{
			while ((dr = readdir(tdir)) != NULL)
			{
				if (!strcmp(words[2], "d"))
				{
					if ((int)dr->d_type == 4)
					{
						++tmp;
					}
				}
				else if (!strcmp(words[2], "r"))
				{
					if ((int)dr->d_type == 8)
					{
						++tmp;
					}
				}
			}
		}
		else if (!strcmp(words[0], "size"))
		{
			int f_size = atoi(words[2]);
			while ((dr = readdir(tdir)) != NULL)
			{
				if ((int)dr->d_type == 8)
				{
					stat(dr->d_name, &attrs);
					if (!strcmp(words[1], ">"))
					{
						if (attrs.st_size > f_size)
						{
							++tmp;
						}
					}
					else if (!strcmp(words[1], "<"))
					{
						if (attrs.st_size < f_size)
						{
							++tmp;
						}
					}
					else if (!strcmp(words[1], "="))
					{
						if (attrs.st_size == f_size)
						{
							++tmp;
						}
					}
				}
			}
		}
		else if (!strcmp(words[0], "age"))
		{
			if (strchr(words[2], 'd') != NULL)
			{
				val = strtok(words[2], d);
				int_val = atoi(val);
				words[3] = "d";
			}
			else if (strchr(words[2], 'h') != NULL)
			{
				val = strtok(words[2], h);
				int_val = atoi(val);
				words[3] = "h";
			}
			else if (strchr(words[2], 'm') != NULL)
			{
				val = strtok(words[2], m);
				int_val = atoi(val);
				words[3] = "m";
			}
			else if (strchr(words[2], 's') != NULL)
			{
				val = strtok(words[2], s);
				int_val = atoi(val);
				words[3] = "s";
			}
			else
			{
				counts = -8;
				return &counts;
			}

			while ((dr = readdir(tdir)) != NULL)
			{
				stat(dr->d_name, &attrs);
				time_t time = attrs.st_mtime;
				struct tm *l_time;
				l_time = localtime(&time);
				int sec = l_time->tm_sec;
				int min = l_time->tm_min;
				int hour = l_time->tm_hour;
				int day = l_time->tm_mday;
				if ((int)dr->d_type == 8)
				{
					if (!strcmp(words[3], "d"))
					{
						if (!strcmp(words[1], ">"))
						{
							if (day > int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "<"))
						{
							if (day < int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "="))
						{
							if (day == int_val)
							{
								++tmp;
							}
						}
					}
					else if (!strcmp(words[3], "h"))
					{
						if (!strcmp(words[1], ">"))
						{
							if (hour > int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "<"))
						{
							if (hour < int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "="))
						{
							if (hour == int_val)
							{
								++tmp;
							}
						}
					}
					else if (!strcmp(words[3], "m"))
					{
						if (!strcmp(words[1], ">"))
						{
							if (min > int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "<"))
						{
							if (min < int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "="))
						{
							if (min == int_val)
							{
								++tmp;
							}
						}
					}
					else if (!strcmp(words[3], "s"))
					{
						if (!strcmp(words[1], ">"))
						{
							if (sec > int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "<"))
						{
							if (sec < int_val)
							{
								++tmp;
							}
						}
						else if (!strcmp(words[1], "="))
						{
							if (sec == int_val)
							{
								++tmp;
							}
						}
					}
				}
			}
		}
		else
		{
			/**command not found or invalid*/
			counts = -10;
		}
	}
	closedir(tdir);
	counts = tmp;
	return &counts;
}


bool_t *
openlist_1_svc(char **argp, struct svc_req *rqstp)
{
	static bool_t op_res;
	pdir = opendir(*argp);
	op_res = (pdir == NULL) ? 0 : 1;
	return &op_res;
}

/*read file return the name and tyep*/
/*if ls -l return the size, date, name and type*/
/*if condition given, parse string to find condition*/
/*return only the name of file /direcctory satified condition*/
direntry *
nextlist_1_svc(char **argp, struct svc_req *rqstp)
{
	static direntry dir;
	struct dirent *dr;
	struct stat attrs;
	char *val;
	int int_val;
	char op[2] = " ";
	char d[2] = "d";
	char h[2] = "h";
	char m[2] = "m";
	char s[2] = "s";
	char *words[5];

	if (!strcmp(*argp, "NONE"))
	{
		if ((dr = readdir(pdir)) == NULL)
		{
			dir.flag = -1;
			return &dir;
		}
		strcpy(dir.entryname, dr->d_name);
		dir.flag = (int)dr->d_type;
		stat(dir.entryname, &attrs);
		strftime(dir.date, 255, "%Y-%m-%d %H:%M:%S", localtime(&attrs.st_mtime));
		dir.size = attrs.st_size;
	}
	else
	{
		memset(dir.entryname, 0, MAXLEN);
		if ((dr = readdir(pdir)) == NULL)
		{
			dir.flag = -1;
			return &dir;
		}

		/**parse string to get operators and operans*/
		if (strchr(*argp, '>') != NULL)
		{
			int cnt = 0;
			char op[2] = ">";
			char *token = strtok(*argp, op);
			while (token != NULL)
			{
				words[cnt] = token;
				token = strtok(NULL, op);
				cnt++;
			}
			words[2] = words[1];
			words[1] = ">";
		}
		else if (strchr(*argp, '<') != NULL)
		{
			int cnt = 0;
			char op[2] = "<";
			char *token = strtok(*argp, op);
			while (token != NULL)
			{
				words[cnt] = token;
				token = strtok(NULL, op);
				cnt++;
			}
			words[2] = words[1];
			words[1] = "<";
		}
		else if (strchr(*argp, '=') != NULL)
		{
			int cnt = 0;
			char op[2] = "=";
			char *token = strtok(*argp, op);
			while (token != NULL)
			{
				words[cnt] = token;
				token = strtok(NULL, op);
				cnt++;
			}
			words[2] = words[1];
			words[1] = "=";
		}
		else
		{
			dir.flag = -10;
		}

		if (!strcmp(words[0], "type"))
		{

			if (!strcmp(words[2], "d"))
			{
				if ((int)dr->d_type == 4)
				{
					strcpy(dir.entryname, dr->d_name);
				}
			}
			else if (!strcmp(words[2], "r"))
			{
				if ((int)dr->d_type == 8)
				{
					strcpy(dir.entryname, dr->d_name);
				}
			}
		}
		else if (!strcmp(words[0], "size"))
		{
			int f_size = atoi(words[2]);
			stat(dr->d_name, &attrs);
			if (!strcmp(words[1], ">"))
			{
				if (attrs.st_size > f_size)
				{
					strcpy(dir.entryname, dr->d_name);
				}
			}
			else if (!strcmp(words[1], "<"))
			{
				if (attrs.st_size < f_size)
				{
					strcpy(dir.entryname, dr->d_name);
				}
			}
			else if (!strcmp(words[1], "="))
			{
				if (attrs.st_size == f_size)
				{
					strcpy(dir.entryname, dr->d_name);
				}
			}
		}
		else if (!strcmp(words[0], "age"))
		{
			if (strchr(words[2], 'd') != NULL)
			{
				val = strtok(words[2], d);
				int_val = atoi(val);
				words[3] = "d";
			}
			else if (strchr(words[2], 'h') != NULL)
			{
				val = strtok(words[2], h);
				int_val = atoi(val);
				words[3] = "h";
			}
			else if (strchr(words[2], 'm') != NULL)
			{
				val = strtok(words[2], m);
				int_val = atoi(val);
				words[3] = "m";
			}
			else if (strchr(words[2], 's') != NULL)
			{
				val = strtok(words[2], s);
				int_val = atoi(val);
				words[3] = "s";
			}
			else
			{
				dir.flag = -10;
			}

			stat(dr->d_name, &attrs);
			time_t time = attrs.st_mtime;
			struct tm *l_time;
			l_time = localtime(&time);
			int sec = l_time->tm_sec;
			int min = l_time->tm_min;
			int hour = l_time->tm_hour;
			int day = l_time->tm_mday;

			if (!strcmp(words[3], "d"))
			{
				if (!strcmp(words[1], ">"))
				{
					if (day > int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "<"))
				{
					if (day < int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "="))
				{
					if (day == int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
			}
			else if (!strcmp(words[3], "h"))
			{
				if (!strcmp(words[1], ">"))
				{
					if (hour > int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "<"))
				{
					if (hour < int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "="))
				{
					if (hour == int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
			}
			else if (!strcmp(words[3], "m"))
			{
				if (!strcmp(words[1], ">"))
				{
					if (min > int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "<"))
				{
					if (min < int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "="))
				{
					if (min == int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
			}
			else if (!strcmp(words[3], "s"))
			{
				if (!strcmp(words[1], ">"))
				{
					if (sec > int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "<"))
				{
					if (sec < int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
				else if (!strcmp(words[1], "="))
				{
					if (sec == int_val)
					{
						strcpy(dir.entryname, dr->d_name);
					}
				}
			}
		}
		else
		{
			dir.flag = -10;
		}

		dir.flag = 0;
	}
	return &dir;
}

bool_t *
closelist_1_svc(void *argp, struct svc_req *rqstp)
{
	static bool_t close_d_res;

	close_d_res = (closedir(pdir) == 0) ? 1 : 0;
	return &close_d_res;
}

bool_t *
openfiletowrite_1_svc(char **argp, struct svc_req *rqstp)
{
	static bool_t file_w_open;
	file_w_open = ((fp = fopen(*argp, "w")) == NULL) ? 0 : 1;
	return &file_w_open;
}

bool_t *
nextwrite_1_svc(file_block *argp, struct svc_req *rqstp)
{
	static bool_t w_result;
	int num_blcok = argp->num_block;
	if (argp->size < 512)
	{
		w_result = (fwrite(argp->data, 1, argp->size, fp) == argp->size) ? 1 : 0;
	}
	else
	{
		w_result = (fwrite(argp->data, 1, 510, fp) == (510)) ? 1 : 0;
	}
	return &w_result;
}

bool_t *
closefile_1_svc(void *argp, struct svc_req *rqstp)
{
	static bool_t fd_close;
	fd_close = (fclose(fp) == 0) ? 1 : 0;
	bl_cnt = 0;
	return &fd_close;
}

bool_t *
openfiletoread_1_svc(char **argp, struct svc_req *rqstp)
{
	static bool_t file_r_open;
	file_r_open = ((fp = fopen(*argp, "r")) == NULL) ? 0 : 1;
	return &file_r_open;
}

file_block *
nextread_1_svc(void *argp, struct svc_req *rqstp)
{
	static file_block fb;
	if (bl_cnt == 0)
	{
		fseek(fp, 0, SEEK_END);
		fb.size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
	}

	if (fb.size < 512)
	{
		fread(fb.data, fb.size, 1, fp);
	}
	else
	{
		fread(fb.data, 510, 1, fp);
	}
	bl_cnt++;
	fb.num_block = (int)(fb.size / 510);
	return &fb;
}

/*randomm read from give postion to size sepecified*/
/*the size can beyond actual size will just return actual size*/
file_block *
randomread_1_svc(file_block *argp, struct svc_req *rqstp)
{
	static file_block rand_fb;
	int rd_bytes;
	fseek(fp, 0, SEEK_END);
	rand_fb.size = ftell(fp);
	rand_fb.rr_bytes = argp->size;
	rd_bytes = argp->size;

	/**can't read beyond the actual size*/
	if (argp->offset + rd_bytes > rand_fb.size)
	{
		rand_fb.flag = -1;
		rd_bytes = rand_fb.size - argp->offset;
		rand_fb.rr_bytes = rd_bytes;
	}
	else
	{
		rand_fb.flag = 0;
	}
	fseek(fp, argp->offset, SEEK_SET);
	memset(rand_fb.data, 0, MAXSIZE);
	fread(rand_fb.data, rd_bytes, 1, fp);

	return &rand_fb;
}
