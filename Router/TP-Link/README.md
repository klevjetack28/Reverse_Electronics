# ./busybox-mips netstat -tulnp
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

