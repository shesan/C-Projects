#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MASK 0x00ff
#define OFFSET_SIZE 8
#define BUFFER_SIZE 10
#define TLB_SIZE 16
#define FRAME_NUM_MAX 256
#define FRAME_SIZE 256
#define PAGE_NUM_MAX 256
#define PAGE_SIZE 256

int op_count = 0;
int frame_count = 0;
int fault = 0;
int TLB_hit = 0;
int TLB_count = 0;

int i;
int j;
int input;

FILE *address_txt;
FILE *backing_store;
FILE *fl;

typedef struct 
{
  int page_num;
  int frame_num;
} page_frame_struct;



signed char physical_mem[FRAME_NUM_MAX][FRAME_SIZE];
page_frame_struct page_tbl[PAGE_NUM_MAX];
page_frame_struct TLB[TLB_SIZE];
  
char address[BUFFER_SIZE];
int logical_address;
signed char read_val;


int TBL_check(int page_num)
{
  for(i = 0; i < TLB_SIZE; i++)
  {
    if(TLB[i].page_num == page_num)
      return TLB[i].frame_num;
  }
  return -1;
}


void TLB_add(int page_num, int frame_num)
{
  TLB[TLB_count].page_num = page_num;
  TLB[TLB_count].frame_num = frame_num;
  TLB_count = (TLB_count + 1) % TLB_SIZE;
}


int page_tbl_check(int page_num)
{

  for(i = 0; i < PAGE_NUM_MAX; i++)
  {
    if(page_tbl[i].page_num == page_num)
      return i;
  }
  
  return -1;
}


int page_tbl_add(int page_num, int frame_num)
{
  
  for(i = PAGE_NUM_MAX - 1; i > 0; i--)
    page_tbl[i] = page_tbl[i - 1];
  
  page_tbl[0].page_num = page_num;
  page_tbl[0].frame_num = frame_count;    
 
  return frame_count;
}

void page_tbl_alter(int index)
{
   
  page_frame_struct front = page_tbl[index];
  
  for(i = index; i > 0; i--)
    page_tbl[i] = page_tbl[i - 1];
  
  page_tbl[0] = front;
}

int main(int argc, char *argv[])
{
  // /.c [Size] [Backing_Store] [Address.txt]
    if (argc != 4){
      printf("/mmu [Size] [Backing_Store] [Address.txt]");
      return -1;
    }
    else {
      input = atoi(argv[1]);
      backing_store = fopen(argv[2], "rb");
      address_txt = fopen(argv[3], "r");
      fl = fopen("output256.csv", "w+");
    }

  //initialize
  for(i = 0; i < FRAME_NUM_MAX; i++)
  {
    for(j = 0; j < FRAME_SIZE; j++)
      physical_mem[i][j] = 0;
  }
 
  for(i = 0; i < PAGE_NUM_MAX; i++)
  {
    page_tbl[i].page_num = -1;
    page_tbl[i].frame_num = -1;
  } 
 
  for(i = 0; i < TLB_SIZE; i++)
  {
    TLB[i].page_num = -1;
    TLB[i].frame_num = -1;
  }





  while (fgets(address, BUFFER_SIZE, address_txt) != 0) 
  {
    op_count++;
    logical_address = atoi(address);

    int page_num = logical_address >> OFFSET_SIZE;
    page_num = page_num & MASK;
    int offset = logical_address & MASK;

    int frame_num = TBL_check(page_num);

    if(frame_num == -1)
    {
      int index = page_tbl_check(page_num);

      if(index == -1)
      {
        frame_num = page_tbl_add(page_num, frame_num);
        frame_count = (frame_count + 1) % FRAME_NUM_MAX;
        
        fault++;

        if (fseek(backing_store, page_num * PAGE_SIZE, SEEK_SET) != 0 || fread(physical_mem[frame_num], sizeof(signed char), FRAME_SIZE, backing_store) == 0)
        {
          return -1;
        }        
      }
      else
      {
        frame_num = page_tbl[index].frame_num;
        page_tbl_alter(index);
      }


      TLB_add(page_num, frame_num); 
    }

    else
    {
      TLB_hit++;
    }
    
    int physical_address = frame_num << 8;
    physical_address = physical_address | offset;
    read_val = physical_mem[frame_num][offset];  
 
    //print
    fprintf(fl,"%d,%d,%d\n", logical_address, physical_address, read_val);
  }

  fprintf(fl, "Page Faults Rate, %.2f%%,\n", (double)fault * 100/ (double)op_count);
  fprintf(fl, "TLB Hits Rate, %.2f%%,", (double)TLB_hit *100 / (double)op_count);

  fclose(fl);
  fclose(address_txt);
  fclose(backing_store);

  return 0;
}

