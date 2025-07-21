## Setup

For my setup to extract the firmware I needed the USB to TTL for serial communication and to connect four pins the to UART connection. On my board I did not have any pins pre connected to connect my UART communication too so I had so solder my own connections. I taped everything down with some electrical tape to secure it all. Once I had my serial connection I used minicom to connect to the router once powered on. Later in the project another peice of hardware I used was an Ethernet cable. 

## Extracting Firmware

On the router I chose extracting the firmware was a very easy task. Once the router fully booted you were given root access to the router without any login credentials. This meant that I could find out what filesystem the router used. Once I know the filesystem I can download busybox which is a collection of the most common Unix tools. I did this with an Ethernet connection from my laptop to the router in the /tmp directory. After installing the package I could run all the important Unix commands and compress the entire filesystem into a binary to transfer to my computer.

## Reverse Engineering

I know have the entire filesystem on my main PC where I can start to find which processes I can reverse engineer. The first step is to run binwalk and extract the root filesystem into my case squashfs. This is where you’ll have access to all of the processes that can potentially run on the router. I started by running (full command) to find any focuses that had open ports and were listening. These are vulnerable to WAN connection and could have vulnerabilities that I could access from anywhere. In the video I was watching they mentioned that the tmpServer is common for vulnerabilities and it was also listed with ports open so I decided to try it first. I ran find through the filesystem and found the directory the process is located in and loaded it into ghidra. 

### Trace 

So far in my process I started with the entry function and noticed it has three main parts where it forks and the child process is what creates sockets and listens while the parent process listen on the ubus. I am choosing to ignore the parent process for now because the child process has lots of open sockets that I want to try and abuse. 

## Future Improvements 

In the future I want to solder proper connections to the board I just didn’t buy any beforehand. This would give much more reliable connections so I wouldn’t need to retake to sure them or power on and off the board cause it stopped reading properly. 

./busybox-mips netstat -tulnp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 0.0.0.0:33344           0.0.0.0:*               LISTEN      -
tcp        0      0 127.0.0.1:20002         0.0.0.0:*               LISTEN      3751/tmpServer
tcp        0      0 0.0.0.0:20005           0.0.0.0:*               LISTEN      -
tcp        0      0 0.0.0.0:80              0.0.0.0:*               LISTEN      3796/uhttpd
tcp        0      0 0.0.0.0:53              0.0.0.0:*               LISTEN      3070/dnsmasq
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      3794/dropbear
tcp        0      0 0.0.0.0:443             0.0.0.0:*               LISTEN      3796/uhttpd
tcp        0      0 :::53                   :::*                    LISTEN      3070/dnsmasq
tcp        0      0 :::22                   :::*                    LISTEN      3794/dropbear
udp        0      0 0.0.0.0:20002           0.0.0.0:*                           3751/tmpServer
udp        0      0 0.0.0.0:47142           0.0.0.0:*                           4514/conn-indicator
udp        0      0 0.0.0.0:53              0.0.0.0:*                           3070/dnsmasq
udp        0      0 0.0.0.0:37434           0.0.0.0:*                           7153/ntpd
udp        0      0 0.0.0.0:67              0.0.0.0:*                           3070/dnsmasq
udp        0      0 0.0.0.0:60543           0.0.0.0:*                           -
udp        0      0 :::53                   :::*                                3070/dnsmasq

