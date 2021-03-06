#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define PER_SEC 512

struct _block{
	long long start;
	long long size;
}blk[10240];

struct _part{
	char dev[100];
	int s_sec,e_sec;
	char ftype[100];	//only ftype =="Linux" then fullcopy
	int offset;
	int pe_start;	// assigned if ftype == "Linux LVM",or 0 
}pt[100];

struct _lv{
	char lv_name[100];
	char lv_path[100];
	char pv_name[100];
	int s_lv,s_pv;
	int seg_size;
};

struct _lv lv[100];

int bsize,bcount,freeb;
int ex_size;
//int buf_size;
FILE *f_src,*f_dsc;

int  inline do_copy(long long size,long long offset)
{
	long long count,sum;
	char *buf;
	int unit;
	unit = 4<<20;
	buf = (char *)malloc(sizeof(char)*unit);
	fseeko(f_src,offset,SEEK_CUR);
	fseeko(f_dsc,offset,SEEK_CUR);
	sum = 0;
	while(!feof(f_src) && sum < size - unit)
	{
		count = fread(buf,sizeof(char),unit,f_src);
		fwrite(buf,sizeof(char),unit,f_dsc);
		sum += count;
	}
	unit = size - sum;
	fread(buf,sizeof(char),unit,f_src);
	fwrite(buf,sizeof(char),unit,f_dsc);
	free(buf);
	return 0;
}

struct _part* find_dev(char *dev,int part_number)
{
	int i;
	i = 0;
	while(i < part_number)
		if(!strcmp(dev,pt[i].dev))
			return &pt[i];
		else
			i++;
	return NULL;
}

int full_copy(struct _part *pt)
{
	long long start,size;
	start = pt->s_sec * PER_SEC;
//	end = (pt->s_sec + pt->offset - 1) * PER_SEC;
	size = (long long)(pt->offset - 1) * PER_SEC;
	fseeko(f_src,start,SEEK_SET);
	fseeko(f_dsc,start,SEEK_SET);
	do_copy(size,0);
	return 0;
}

void copy_ext(int index,int offset,int last)
{   
	char buf[bsize];
	int i;
	long long start,size;
	//copy useful data
	fseeko(f_src,offset,SEEK_SET);
	fseeko(f_dsc,offset,SEEK_SET);
	size = blk[0].size * bsize;
	do_copy(size,0);
	for(i = 1;i < index;i++)
	{
//		start = offset + blk[i].start * bsize;
//		end = offset + blk[i].end * bsize;
		offset = (long long)(blk[i].start - blk[i-1].start - blk[i-1].size) * bsize;
		size = blk[i].size * bsize;
		do_copy(size,offset);
	}
	//copy the last sector
	fseeko(f_src,last,SEEK_SET);
	fseeko(f_dsc,last,SEEK_SET);
	fread(buf,sizeof(char),PER_SEC,f_src);
	fwrite(buf,sizeof(char),PER_SEC,f_dsc);
}


int get_used(char *dev)
{
	char buf[4096],cmd[100];
	FILE *fp;
	char *p;
	int i,status;
	int num,index;
	int start,end;
	sprintf(cmd,"sudo dumpe2fs %s > file.dat",dev);
	status = system(cmd);

	if(status != 0)
		return -1;
	if((fp=fopen("file.dat","r")) == NULL)
	{
		printf("get_used():open tmp file failure\n");
		return -1;
	}
	while(fgets(buf,4096,fp) != NULL)
		if((p = strstr(buf,"Block count:")))
		{
			p += 12;
			sscanf(p,"%d",&bcount);
			break;
		}
	while(fgets(buf,4096,fp) != NULL)
		if((p = strstr(buf,"Free blocks:")))
		{
			p += 12;
			sscanf(p,"%d",&freeb);
			break;
		}
	while(fgets(buf,4096,fp) != NULL)
		if((p = strstr(buf,"Block size:")))
		{
			p += 12;
			sscanf(p,"%d",&bsize);
			break;
		}
	index = 0;
	start = 0;
	end = -1;
	i = 0;
	while(fgets(buf,4096,fp) != NULL)
	{
		i++;
		p = buf;
		while(*p == ' ')
			p++;
		if(( !strncmp(p,"Free blocks: ",13)) && *(p+13) != '\n')
		{
			p += 13;
			num = 0;
	 		while(*p !='\0')
			{
				if(*p >= '0' && *p <= '9')
					num = num * 10 + (*p - '0');
				else if(*p == '-')
				{
	 				end = num;
					num = 0;
				}
				else if(*p == ',' || *p == '\n')
				{
					if(end == -1 && start < num) // case 3-5,7,9,11-13...
					{
						blk[index].start = start;
						blk[index++].size = num - start;
						
					}
					else if(start < end) // case 3-5,6-7...
					{
						blk[index].start = start;
						blk[index++].size =  end- start;
					}
					start = num + 1;
					end = -1;
					num = 0;
				}
				p++;
			}
		}
	}
	if(start != bcount)
	{
		blk[index].start = start;
		blk[index++].size = bcount - start;
    }
	fclose(fp);
	return index;
}

int get_ptable(char *dev)
{
	char cmd[1024],buf[1024],tmp[100];
	char *p;
	FILE *fp,*fq;
	int j,num;
	char unit;
	float pe_start;
	sprintf(cmd,"sudo cfdisk -P s %s > part.dat",dev);
	system(cmd);
	if((fp = fopen("part.dat","r")) == NULL)
	{
		printf("get_ptable():open tmp file failure\n");
		return -1;
	}
	while(fgets(buf,1024,fp) )
		if(*buf == '-')
			break;
	j = 0;
	while(fgets(buf,1024,fp))
	{
		p = buf;
		while(*p ==' ')
			p++;
		if(isdigit(*p))
		{
			num = 0;
			while(*p != ' ')
				num = num *10 + (*p++ - '0');
			sprintf(pt[j].dev,"/dev/mapper/loop0p%d",num);
			while(!isdigit(*p))
				p++;
		}
		else
		{
			pt[j].dev[0] = '\0'; 
			while(!isdigit(*p))
				p++;
		}
		
		sscanf(p,"%d%*[^0-9]%d%*[^0-9]%d",&pt[j].s_sec,&pt[j].e_sec,&pt[j].offset);
		while(!isalpha(*p))
			p++;
		sscanf(p,"%s",tmp);
		strcpy(pt[j].ftype,tmp);
		if(!strcmp(tmp,"Linux"))
		{
			p += 6;
			sscanf(p,"%s",tmp);
			if(!strcmp(tmp,"LVM"))
			{
				strcpy(pt[j].ftype,"Linux LVM");
				sprintf(cmd,"pvs --noheading -o pe_start %s > pe_start.dat",pt[j].dev);
				system(cmd);
				if(!(fq = fopen("pe_start.dat","r")))
				{
					printf("do_depart():open pe_start.dat failure!\n");
					return -1;
				}
				fscanf(fq,"%f",&pe_start);
				fscanf(fq,"%c",&unit);
				if(unit == 'm' || unit == 'M')
					pt[j].pe_start = ((int)pe_start<<20) / PER_SEC;
				else if(unit == 'k' || unit == 'K')
					pt[j].pe_start = ((int)pe_start<<10)  / PER_SEC;
				fclose(fq);
			}
			else
				pt[j].pe_start = 0;
		}
		j++;
	}
	fclose(fp);
	return j;
}

int get_lvtable(char *vg)
{
	char cmd[200],buf[1024];
	char *p;
	FILE *fp;
	int i,j;
	sprintf(cmd,"sudo pvs --noheading --aligned --separator %% -o vg_name,lv_name,lv_path,seg_start_pe,pvseg_size,pv_name,pvseg_start -O lv_name,seg_start > lv_table.dat");
	system(cmd);
	if(!(fp = fopen("lv_table.dat","r")))
	{
		printf("get_lvtable():open lv_table failure\n");
		return -1;
	}
	j = 0;
	while(fgets(buf,1024,fp))
	{
		p = buf;
		i = 0;
		for(p = strtok(p,"% \t");p;p = strtok(NULL,"% \t"))
		{ 
			i++;
			if(i == 1)
			{
				if(!strcmp(p,vg))
					continue;
				else
					break;
			}
			else if(i == 2)
				strcpy(lv[j].lv_name,p);
			else if(i == 3)
				strcpy(lv[j].lv_path,p);
			else if(i == 4)
				lv[j].s_lv = atoi(p);
			else if(i == 5)
				lv[j].seg_size = atoi(p);
			else if(i == 6)
				strcpy(lv[j].pv_name,p);
			else if(i == 7)
				lv[j++].s_pv = atoi(p);
		}
	}
	fclose(fp);
	return j;
}

int copy_lv(int lv_number,int part_number)
{ 
	int i,j,index;
	int k;
	struct _part *pt;
	long long start,pt_offset;
//	long long end;
	long long offset,size;
//	long long limit;

	for(i = 0;i < lv_number;i++)
	{
		index = get_used(lv[i].lv_path);
		pt = find_dev(lv[i].pv_name,part_number);
		if(!pt)
		{
			printf("copy_lv():find device failure!\n");
			return -1;
		}

		start = (long long)pt->s_sec * PER_SEC;
//		end = (long long)(pt->s_sec + pt->offset + pt->pe_start) * PER_SEC;
		size = (long long)(pt->offset + pt->pe_start) * PER_SEC;
		fseeko(f_src,start,SEEK_SET);
		fseeko(f_dsc,start,SEEK_SET);
	    do_copy(size,0);	

		pt_offset = (long long)(pt->s_sec + pt->offset + pt->pe_start) * PER_SEC + (long long)lv[i].s_pv *ex_size;
		fseeko(f_src,pt_offset,SEEK_SET);
		fseeko(f_dsc,pt_offset,SEEK_SET);
		if(index < 0)
		{
//			start = pt_offset;
//			end = pt_offset + (long long)lv[i].seg_size * (long long)ex_size;
			size = (long long)lv[i].seg_size * (long long)ex_size;
			do_copy(size,0);
		}
		else
		{
//			start = pt_offset + (long long)blk[0].start * bsize;
//			end = start + (long long)(blk[0].end - blk[0].start) * bsize;
			size = blk[0].size * bsize;
			do_copy(size,0);
			for(j = 1;j < index;j++)
	   		{
				offset = (long long)(blk[j].start - blk[j-1].start - blk[j-1].size ) * bsize;
				size = blk[j].size * bsize;
//				start = pt_offset + offset ;
//		 		end = start + (long long)(blk[j].end - blk[j].start) * bsize;
//				limit = start + (8<<20);
//				limit = start + buf_size;

				if(size < (4<<20))
				{
					for(k = j+1;k < index;k++)
						if((blk[k].start - blk[j].start + blk[k].size) * bsize > (4<<20))
							break;
					if(k-1 > j)
					{
						size = (blk[k-1].start - blk[j].start + blk[k].size) * bsize;
						j = k-1;
					}
				}
				do_copy(bsize,offset);
			}
		}
	}
	return 0;
}


int main(int argc,char *argv[])
{
	int part_number,lv_number;
	int i,index;
	long long last;
	int flag;
	char cmd[100],ch;
	char buf[PER_SEC];
	float extent;
	FILE *fp;
//	struct stat  st;
	if(argc != 4)
	{
		printf("useage:a.out device file\n");
		return 0;
	}
	part_number = get_ptable(argv[1]);
	printf("part-number:%d\n",part_number);

	if((f_src=fopen(argv[2],"r")) == NULL)
	{
		printf("main()%d:open file  failure\n",__LINE__);
		return -1;
	}
	if((f_dsc=fopen("new.img","w")) == NULL)
	{
		printf("main()%d:open file failure\n",__LINE__);
		return -1;
	}
//	stat("new.img",&st);
//	buf_size = st.st_blksize;

	flag = 1;
	for(i = 0;i < part_number;i++)
	{
		if(!strcmp(pt[i].ftype,"Free"))
				full_copy(&pt[i]);
		else if(!strcmp(pt[i].ftype,"Linux")) 
		{
			if(pt[i].offset)
				full_copy(&pt[i]);
			index = get_used(pt[i].dev);
			copy_ext(index,(pt[i].s_sec + pt[i].offset) * PER_SEC,pt[i].e_sec*PER_SEC);
		}
		else if(flag && !strcmp(pt[i].ftype,"Linux LVM"))
		{
			flag = 0;
			sprintf(cmd,"vgs --noheading -o vg_extent_size %s >extent_size.dat",argv[3]);
			system(cmd);
			if(!(fp = fopen("extent_size.dat","r")))
			{
				printf("main():open extent_size.dat failure\n");
				return -1;
			}
			fscanf(fp,"%f%c",&extent,&ch);
			if(ch == 'm' || ch == 'M')
				ex_size = (int)extent << 20;
			else if(ch == 'k' || ch == 'K')
				ex_size =(int)extent << 10;
			fclose(fp);
			lv_number = get_lvtable(argv[3]);
			copy_lv(lv_number,part_number);
		}
	}

	last =(long long) pt[part_number-1].e_sec * PER_SEC;
	fseeko(f_src,last,SEEK_SET);
	fseeko(f_dsc,last,SEEK_SET);
	fread(buf,sizeof(char),PER_SEC,f_src);
	fwrite(buf,sizeof(char),PER_SEC,f_dsc);

	fclose(f_src);
	fclose(f_dsc);
	return 0;
}
