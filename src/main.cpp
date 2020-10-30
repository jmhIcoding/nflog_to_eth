#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <pcap-stdinc.h>
#endif

#include <pcap.h>
#include <protocol.h>
#include <vector>
#include <set>
#include <map>
#include <string.h>

#define DLT_NFLOG 239
#define DLT_ETH 1
#define SRC_MAC {0x11,0x11,0x11,0x11,0x11,0x11}		//源MAC
#define DST_MAC {0x22,0x22,0x22,0x22,0x11,0x11}		//目的MAC

const char src_mac[6] = SRC_MAC;
const char dst_mac[6] = DST_MAC;

void display(unsigned char * pkt_data, int len, int nextline=16)
{
	for (int i = 0; i < len;)
	{
		printf("%.2X ", pkt_data[i]);
		i += 1;
		if (i % nextline == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}

int nflog2eth(char *pcapname, char *dstfile="out.pcap")
{
	pcap_t * rdpcap, *_wtpcap;//打开pcap的指针
	pcap_dumper_t * wtpcap_dump; 	//写pcap的指针
	char errBUF[4096] = { 0 };
	rdpcap = pcap_open_offline(pcapname,errBUF);
	if (rdpcap == NULL)
	{
		printf("Error when open pcap file. error is :%s\n", errBUF);
		return -1;
	}

	//检测是否为NFLOG链路

	if (pcap_datalink(rdpcap) != DLT_NFLOG)
	{
		printf("Warning: pcapfile [%s] is not nflog file.\n", pcapname);
		printf("Warning: pcapfile datalinktype is %d\n", pcap_datalink(rdpcap));
		return 0;
	}

	_wtpcap = pcap_open_dead(DLT_ETH, 65535);//第一个参数是 linktype,以太网的linktype是1;

	wtpcap_dump = pcap_dump_open(_wtpcap, dstfile);
	
	//检查是否打开成功
	if (_wtpcap == NULL || wtpcap_dump == NULL)
	{
		printf("Error when create dst file:%s\n", dstfile);
		return -2;
	}

	int nb_transfer = 0;
	while (1)
	{
		pcap_pkthdr pktheader;
		const u_char *pktdata = pcap_next(rdpcap, &pktheader);
		if (pktdata != NULL)
			//pcap存在pcapket未处理
		{
			//printf("Packet :%d\n", nb_transfer);
			display((unsigned char *)pktdata, pktheader.len);

			pcap_pkthdr new_pkthdr = pktheader;
			u_char new_data[1600] = { 0};
			int offset = 4;//nflog有4个字节的头
			int last_offset = offset;

			if (pktdata[0] != 0x02)
				//非IPv4协议不转换
			{
				continue;
			}
			//if (nb_transfer == 3)
			//{
			//	__asm{
			//		int 3;
			//	}
			//}
			while ((pktdata[last_offset + 3] * 256 + pktdata[last_offset + 2]) != 0x0009)
			{
				last_offset = offset;
				offset = offset + pktdata[offset] + pktdata[offset + 1] * 256;
				if ((offset - last_offset) == 0)
				{
					while (pktdata[offset]==0)
					{
						offset += 1;
					}
				}
				//printf("offset:%d, length:%d\n", offset, offset - last_offset);
			}

			if ((pktdata[last_offset+3] * 256 + pktdata[last_offset+2]) != 0x0009)
			{
				printf("NFLOG Format error.");
				continue;
			}

			last_offset += 4;//除去最后一层有4个字节是NFLOG的，PAYLOAD数据，剩下才是最终的IPv4载荷
			

			//修改长度
			new_pkthdr.caplen = pktheader.len - last_offset;
			new_pkthdr.len = pktheader.len - last_offset;
			
			//构造以太网头
			memcpy(new_data, src_mac, 6);
			memcpy(new_data + 6, dst_mac, 6);
			//填充上层协议头,只填充IPv4协议。
			new_data[12] = 0x08;
			new_data[13] = 0x00;

			//拷贝后续packet内容
			memcpy(new_data + 14, pktdata + last_offset, new_pkthdr.len);

			display((unsigned char *)pktdata + last_offset, new_pkthdr.len);

			//把修改的以太网帧写入
			new_pkthdr.caplen += 6+6+2;
			new_pkthdr.len += 6+6+2;
			pcap_dump((u_char*) wtpcap_dump, &new_pkthdr, new_data);

			nb_transfer++;
			if (nb_transfer % 10 == 0)
			{
				pcap_dump_flush(wtpcap_dump);
			}

		}
		else
		{
			pcap_dump_flush(wtpcap_dump);
			break;
		}
	}
	if (wtpcap_dump)
	{
		pcap_dump_close(wtpcap_dump);
	}
	if (rdpcap)
	{
		pcap_close(rdpcap);
	}
	if (_wtpcap)
	{
		pcap_close(_wtpcap);
	}

	return nb_transfer;
}
int main(int argc,char *argv[])
{
	//nflog2eth("eth.pcap");
	//system("pause");
	//return 0;

	if (argc != 3)
	{
		printf("[usage]:nflog2eth.exe src_pcapname dst_pcapname \n");
		exit(-1);
	}
	
	char * pcapname = argv[1];
	char * dstpcapname = argv[2];
	if (nflog2eth(pcapname,dstpcapname) <0 )
	{
			printf("Error!!!!%s\n", pcapname);
	}

	//system("pause");
	return 0;
}