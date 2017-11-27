#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <pthread.h>

#define PER_BIT_LEN 4096
#define SERV_PORT 5000
#define HEADER  (sizeof(long long) + sizeof(int))

#define SET_BIT(arr, n)   (arr[n / 8] |= (1 << (n % 8)))
#define CLEAR_BIT(arr, n) (arr[n / 8] &= ~(1 << (n % 8)))
#define CHECK_BIT(arr, n) (arr[n / 8] & (1 << (n % 8)))

struct th_arg{
	char *disk;
	char *bmap;
	char *host;
	int bsize;
};

int finish = 0;
pthread_mutex_t map_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t finish_lock = PTHREAD_MUTEX_INITIALIZER;

int transfer(int sockfd, int fd, char *map, int size, long long *total)
{
	int  read_count, send_count;
	long long pos, i;
	char buf[HEADER + PER_BIT_LEN];

	*total = 0;
	
	for(i = 0;i < size;i++)
	{
		pthread_mutex_lock(&map_lock);
		if( CHECK_BIT(map, i) )
		{
			CLEAR_BIT(map, i);
			pthread_mutex_unlock(&map_lock);

			pos = i * PER_BIT_LEN;
			if( lseek(fd, pos, SEEK_SET) < 0)
			{
				printf("transfer():%lld,lseek error!\n",pos);
				return -1;
			}

			*(long long *)buf = pos;
			read_count = read(fd, buf + HEADER , PER_BIT_LEN);
			if(read_count < 0)
			{
				printf("transfer():read error!\n");
				return -1;
			}
			*(int *)(buf + sizeof(long long)) = read_count;

			send_count = send(sockfd, buf, sizeof(buf), 0);
			if(send_count < 0)
			{
				printf("transfer():send error!\n");
				return -1;
			}
			*total += send_count;
		}
		else
			pthread_mutex_unlock(&map_lock);
	}
	return 0;
}

void * do_precopy(void * th_arg)
{
	struct th_arg * arg;
	int sockfd,fd;
	struct hostent *host;
	struct sockaddr_in serv_addr;
	int iter;

	char buf[HEADER + PER_BIT_LEN];
	long long last_send, total_send, this_send;
	time_t start, end, shang, ehang;

	start = time(NULL);
	arg = (struct th_arg *)th_arg;
	
	if( (host = gethostbyname(arg->host)) == NULL)
	{
		printf("get hostbyname error!\n");
		return NULL;
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket error!\n");
		return NULL;
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	bcopy((char *) host->h_addr_list[0],
			(char *)&serv_addr.sin_addr.s_addr, host->h_length);

	if( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr) ) == -1 )
	{
		printf("connect error!\n");
		return NULL;
	}
	if( (fd = open(arg->disk, O_RDONLY)) < 0 )
	{
		printf("do_precopy():open disk error!\n");
		return NULL;
	}

	last_send = 0;
	total_send = 0;
	iter = 0;

	while(1)
	{
		this_send = 0;
		iter++;
		if(transfer(sockfd, fd, arg->bmap, arg->bsize, &this_send) < 0)
		{
			printf("transfer error!\n");
			return NULL;
		}
	//	printf("%d iter complete\n",iter);
		total_send += this_send;

		if( 2 * this_send < last_send )
		{
			iter++;
			shang = time(NULL);
			if(transfer(sockfd, fd, arg->bmap, arg->bsize, &this_send) < 0)
			{
				printf("transfer error!\n");
				exit(-1);
			}
			ehang = time(NULL);
			total_send += this_send;

			/*send -1 to inform server  to stop*/
			*(long long *)buf = -1;
			if( send(sockfd, buf, sizeof(buf), 0) < 0)
			{
				printf("send complete flag error!\n");
				return NULL;
			}

			pthread_mutex_lock(&finish_lock);
			finish = 1;
			pthread_mutex_unlock(&finish_lock);

			printf("migration complete!\n");
			printf("total send %lld\n",total_send);
			end = time(NULL);
			printf("total time %lf\n",difftime(end, start));
			printf("hang time %lf\n",difftime(ehang, shang));
			printf("iter times %d\n",iter);
			close(fd);
			close(sockfd);

			/* simply call exit() is also ok */
			pthread_exit(NULL);

		}
		last_send = this_send;
	}
}

int main(int argc,char *argv[])
{
	int fd, i;
	int map_size;
	long long disk_len;
	int len,pos;
	struct stat stat_buf;
	char *bit_map;
	struct th_arg arg;
	pthread_t precopy;

	if( argc < 3 )
	{
		printf("too few arguments\n");
		printf("useage:a.out disk_name host_name\n");
		return 0;
	}
	if( (fd = open(argv[1],O_RDONLY)) < 0 )
	{
		printf("open %s error\n",argv[1]);
		return -1;
	}
	if( fstat(fd,&stat_buf) < 0 )
	{
		printf("get disk infomation failure\n");
		return -1;
	}
	close(fd);

	disk_len = stat_buf.st_size;
	printf("disk length:%lld\n",disk_len);

	/*the size of bitmap by char*/
	map_size = (disk_len / PER_BIT_LEN + 1) / 8 + 1;
	bit_map = (char *)malloc(sizeof(char) * map_size);
	if( bit_map == NULL )
	{
		printf("malloc memory for bit_map failure!\n");
		return -1;
	}
	/*firstly transfer the whole disk*/
	memset(bit_map,0xff,map_size);
	arg.bmap = bit_map;
	arg.disk = argv[1];
	arg.host = argv[2];

	/*get the exact bit number*/
	i = disk_len / PER_BIT_LEN; 
	arg.bsize = disk_len % PER_BIT_LEN ? i + 1 : i;
	printf("exact bit number:%d\n",arg.bsize);
	

	finish = 0;

	if( pthread_create(&precopy,NULL,do_precopy,&arg) < 0 )
	{
		printf("create precopy thread failure!\n");
		return -1;
	}

	int first_bit,last_bit;
	while(1)
	{
		pthread_mutex_lock(&finish_lock);
		if(finish == 1)
		{
			pthread_mutex_unlock(&finish_lock);
			break;
		}
		else
			pthread_mutex_unlock(&finish_lock);

		srand( (unsigned int)time(NULL) );
		len = rand() % disk_len;
		srand((unsigned int)time(NULL));
		pos = rand() % disk_len;

		first_bit = pos / PER_BIT_LEN;
		last_bit = (pos + len - 1) / PER_BIT_LEN;
		while( first_bit <= last_bit )
		{
			pthread_mutex_lock(&map_lock);
			SET_BIT(bit_map, first_bit);
			pthread_mutex_unlock(&map_lock);
			first_bit++;
		}
		sleep(10);
	}
	pthread_mutex_lock(&map_lock);
	free(bit_map);
	pthread_mutex_unlock(&map_lock);
	return 0;
}
