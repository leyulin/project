/**
 * @author: Leyu Lin(jack)
 * REDID: 817372914
 *
 * @author: Parth Thummar
 * REDID: 824679473
 * 
 * All parts of project were attempted and implemented. 
 * Please see Readme_demo.docx for reference/screenshots. 
 *
 */

/**maxlengthofadirectory/names*/
const MAXLEN = 255;
/**maxsizeoffiledata*/
const MAXSIZE = 512;

/**opaque pointer*/
typedef struct file_block *fp_block;

/**directory entry structure*/
struct direntry{
    int flag;
    char entryname[MAXLEN];
    int size;
    char date[MAXLEN];
};

/**file block structure*/
struct file_block {
    long int size;
    char filename[MAXLEN];
    unsigned char data[MAXSIZE];
    int num_block;
    int offset;
    int flag;
    int rr_bytes;
};


program DFSROG {
    version DFSOPERATIONS {
         /*Directory Operations*/
        string getdir(void) = 1;
        bool changedir(string) = 2;
        int filecount(string) = 3;
        bool openlist(string) = 4;
        direntry nextlist(string) = 5;
        bool closelist(void) =6;

        /*Sequential Access File Operations*/
        bool openfiletowrite(string) = 7;
        bool nextwrite(file_block) = 8;
        bool closefile(void) = 9;

        bool openfiletoread(string) = 10;
        file_block nextread(void) = 11; 

        /*Random Access File Operations*/
        file_block randomread(file_block) = 12;
    } = 1;


} = 6;
