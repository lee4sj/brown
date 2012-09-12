IP over DNS README
Matt Carroll (mcarroll)
Seungji Lee (sl136)

The client and server C programs are distributed with their own Makefiles, so they are
each compiled separately with make.
Similarly, client and server have separate bash scripts to create the tunnel, bring up
the interface, and modify the routing table.

Steps:

1. "make" in client and server directory.
2. Client: "./tunup.sh tun0 matt 10.0.0.1 10.0.0.2"
3. Server: "./server_tuntup tun0"
4. Server "./server_tuntap tun0" (bash script)
5. Client: "./testtun tun0"
6. Either: "squid" (this should be running already anyway)

After this, the tunnel and proxy is up and running.


Routes are configured with this on the client side:

"ip addr add <clientip> peer <serverip> dev <devicename>"

where devicename is the virtual interface name.

And on the server side:

Routes are configured with

ifconfig <devname> up mtu 120 10.0.0.2 pointopoint 10.0.0.1
route del 10.0.0.1
route add -host  10.0.0.1 gw 10.0.0.2 <devname>

where <devname> is the device name.

We set the routes using the initial configuration scripts.  
The data is encoded in base 62 with the encoding/decoding functions in base64.c.

The protocol considers this to be an empty "keep-alive" message:

xxx.d.dtun.co.cc

where xxx is any 3 base62 characters.

The protocol considers a data-containing message as having the same final 16 characters
as the empty message, but with any number of 0-63 character dot separated chunks before it.
