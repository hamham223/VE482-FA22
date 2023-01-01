#include "list.h"
#include "sort.h"


#define CHAR 1
#define INT sizeof(int)
#define DOUBLE sizeof(double)
#define MAX_LENGTH 2048
/* elem: list of elments; card: cardinal of the set; type: data type (CHAR INT or DOUBLE) */

const char _INC_[4] = "inc";
const char _DEC_[4] = "dec";
const char _RAND_[5] = "rand";
const char * _namePtr_[3] = {_RAND_,_INC_,_DEC_};
const char _INT_[4] = "int";
const char _DOUBLE_[7] = "double";
#ifdef DEBUG
const char _CHAR_[6] = "char";
#else
const char _CHAR_[6] = "char*";
#endif
const char * _typePtr_[3] = {_CHAR_,_INT_,_DOUBLE_};


int getSize(int type){
    if (type == 1 ) return INT;
    if (type == 2 ) return DOUBLE;
    return CHAR;
}

int getOrd(char * token){
    char _token_[4]="";
    strncpy(_token_,token,4);
    if (strcmp(_token_,_INC_)==0) return 1;
    if (strcmp(_token_,_DEC_)==0) return 2;
    return 0;
}

int getType(char * token){
    char * pos = strchr(token,'_');
    if (*(pos+1)=='i') return 1;
    if (*(pos+1)=='d') return 2;
    return 0;
}

void * getVal(char * data, int type){
    if (type==0) return data;
    if (type==1) {
        int * val = malloc(INT);
        *val = atoi(data); free(data);
        return val;
    } else {
        double * val = malloc(DOUBLE);
        *val = atof(data); free(data);
        return val;
    }
    return NULL;
}

void printMess(char * f1, char * f2) {
    printf("reading %s\n",f1);
    puts("sorting elements");
    printf("writing %s",f2);
}


int main(int argc, char * argv[]){
    
    srand((unsigned)time(NULL));

    if (argc==1) {
        printf("No option.!\n");
        return 0;
    }

    int ord = getOrd(argv[2]);
    FILE * fp = fopen(argv[1],"r");
    

    //if (fp) printf("Input file %s opened successfully\n",argv[1]);
    //else printf("Input file not open!\n");
    fflush(stdout);
    int type = getType(argv[1]);
    
    node_t * head = NULL;
    char line[MAX_LENGTH];
    char * pos = NULL;
    while (fgets(line,MAX_LENGTH,fp)){
        line[strlen(line)-1]='\0';
        pos=strchr(line,'='); *pos='\0';
        unsigned int i = (unsigned int)strlen(line)+1;
        char * name = malloc(i); strncpy(name,line,i);
        char * data = malloc(192); strncpy(data,pos+1,192);
        void * val = getVal(data,type); // data is freed
        head = insert_node(head,val,name);
        *pos = '=';
    }
    
    //printf("Read in list successfully!\n");
    //fflush(stdout);
    head = mergesort(head,type,ord);

    char outFilename[20]="";
    sprintf(outFilename,"%s_%s.txt",(_namePtr_[ord]),(_typePtr_[type]));
    FILE * fout = fopen(outFilename,"w");
    //if (fout) printf("Output file %s opened successfully\n",argv[1]);
    //else printf("Output file not open!\n");

    printMess(argv[1],outFilename);
    fprintList(head,type,fout);
    
    delete_list(head);
    fclose(fp); fclose(fout);
    return 0;
}
