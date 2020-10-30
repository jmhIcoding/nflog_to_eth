# 项目介绍
	本项目支持将从Linux NFLOG网卡抓取的NFLOG 帧转换为常见的以太网帧。 
# 项目地址：
https://github.com/jmhIcoding/nflog_to_eth.git

# 项目编译方式
## VS2013

打开vsrc目录,里面有个vsrc.sln项目文件,用VS2013打开这个项目文件，然后编译就可以。

## Linux

```shell
sudo apt-get install libpcap-dev
git clone https://github.com/jmhIcoding/nflog_to_eth.git
cd nflog_to_eth
make
```

就可以看到一个编译好的可执行文件nflog_to_eth.

# 使用方法

目前项目已经在预编译好Win10 x64和 Ubuntu 18.04 x64平台下的可执行文件,可执行文件在bin目录.

bin目录下有一个名为nflog.pcap的测试文件。

nflog_to_eth接受2个参数: 

- src_pcapname : 源NFLOG格式的pcap文件
- dst_pcapname : 目的文件地址

执行如下指令：

```
cd bin/
nflog_to_eth nflog.pcap eth.pcap
``` 

即可看到转换得到的eth.pcap文件。