# Project Introduction
This project supports conversion of NFLOG frames captured by Linux NFLOG network card into common Ethernet frames.
# project address:
https://github.com/jmhIcoding/nflog_to_eth.git

# Environmental requirements:

Windows: Need to install VC++ 2013 runtime library

Linux: g++, libpcap-dev need to be installed

# Project compilation method

## Source code installation
### VS2013

 In the vsrc directory, there is a vsrc.sln project file, open this project file with VS2013, and then compile it.

### Linux

```shell
sudo apt-get install libpcap-dev
git clone https://github.com/jmhIcoding/nflog_to_eth.git
cd nflog_to_eth
make
```

You can see a compiled reconstruction file nflog_to_eth.

## Pre-compiled installation
At present, the project has been pre-compiled to insert files under Win10 x64 and Ubuntu 18.04 x64 platforms, and the replacement files are in the bin directory.

# Instructions

nflog_to_eth accepts 2 parameters:

- src_pcapname: source pcap file in NFLOG format
- dst_pcapname: destination file address


There is a test file called nflog.pcap in the bin directory: the data packets inside are encapsulated by NFLOG.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201030181404791.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ptaDE5OTY=,size_16,color_FFFFFF,t_70#pic_center)

For example, execute the following command:

```shell
cd bin/
nflog_to_eth nflog.pcap eth.pcap
```

You can see the converted eth.pcap file.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20201030181535159.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ptaDE5OTY=,size_16,color_FFFFFF,t_70#pic_center)


# other
The source MAC address and destination MAC in the current Ethernet frame are hard-coded in the `src/main.cpp` file:

```cpp
#define SRC_MAC {0x11,0x11,0x11,0x11,0x11,0x11} //Source MAC
#define DST_MAC {0x22,0x22,0x22,0x22,0x11,0x11} //Destination MAC
```

If you need source and destination MAC, you can modify the definition of these two macros yourself, and then recompile the program.
