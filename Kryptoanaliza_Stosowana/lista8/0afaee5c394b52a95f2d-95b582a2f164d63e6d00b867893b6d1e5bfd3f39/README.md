Overview
-----

This example illustrates that with a server configuration where SSL is enabled
globally and SSLVerifyClient is only required for a particular route, golang
crypto/tls Conn will trigger a no renegotiation alert when transitioning into
the route requiring a client certificate.
(http://golang.org/src/crypto/tls/conn.go#L655).

This is known issue per https://github.com/golang/go/issues/5742

The pcap file demonstrates that the Client provides the "renegotiation_info"
extension in the ClientHello.  There is a screenshot of the ClientHello
included as well.

According to RFC 5746 (https://tools.ietf.org/html/rfc5746#section-3.6):

```
* The server MUST check if the "renegotiation_info" extension is included in the
  ClientHello.  If the extension is present, set secure_renegotiation flag to
  TRUE.  The server MUST then verify that the length of the
  "renegotiated_connection" field is zero, and if it is not, MUST abort the
  handshake.

* If neither the TLS_EMPTY_RENEGOTIATION_INFO_SCSV SCSV nor the
  "renegotiation_info" extension was included, set the secure_renegotiation flag
  to FALSE.  In this case, some servers may want to terminate the handshake
  instead of continuing; see Section 4.3 for discussion.
```

It appears this golang client is implementing the first bullet, but since
secure renegotiation is not actually supported in the client, the potential for
a confusing situation exists: the handshake is initiated, but sometime later
the session aborts at the time of renegotiation.

Instead, I propose that the golang client implement the second bullet (i.e. not
providing a renegotiation_info extension) where it makes it clear to the server
that the client will not support secure renegotation.  Not providing the
extension runs the risk that some servers will reject the connection, but I
feel it is better to hit the issue early and directly rather than having a
hidden problem that can surface much later.

Alternatively, the problem could be addressed by actually implementing secure
renegotiation.  I understand that secure renegotiation is not supported
currently due to the "triple handshake attack" which is a valid concern.
However, ECDHE cipher suites are not vulnerable to this particular attack [1].
Perhaps it would be possible for golang to optionally support secure
renegotiation and when doing so only provide ECDHE suites in the ClientHello
(until the protocol vulnerability is addressed by IETF).

[1] Mentioned in "Bulletproof SSL and TLS" by Ivan Ristic and briefly in
http://blog.cryptographyengineering.com/2014/04/attack-of-week-triple-handshakes-3shake.html
in the "So What's the Problem Here?" section: "It does not seem to work on
ECDHE".

Usage
-----

	$> make run

Tail of output
-----

```
&{200 OK 200 HTTP/1.1 1 1 map[Last-Modified:[Thu, 15 Jan 2015 19:30:15 GMT] Etag:["6-50cb5e2aa9fc0"] Accept-Ranges:[bytes] Content-Length:[6] Content-Type:[text/html; charset=UTF-8] Date:[Thu, 15 Jan 2015 19:48:06 GMT] Server:[Apache/2.4.10 (Fedora) OpenSSL/1.0.1e-fips]] 0xc2080962c0 6 [] false map[] 0xc20802a820 0xc208004ba0}
[Thu Jan 15 19:48:06.686639 2015] [ssl:debug] [pid 13] ssl_engine_kernel.c(243): [client 127.0.0.1:56715] AH02034: Subsequent (No.2) HTTPS request received for child 0 (server localhost:443)
[Thu Jan 15 19:48:06.686725 2015] [ssl:debug] [pid 13] ssl_engine_kernel.c(591): [client 127.0.0.1:56715] AH02255: Changed client verification type will force renegotiation
[Thu Jan 15 19:48:06.686737 2015] [ssl:info] [pid 13] [client 127.0.0.1:56715] AH02221: Requesting connection re-negotiation
[Thu Jan 15 19:48:06.686753 2015] [ssl:debug] [pid 13] ssl_engine_kernel.c(791): [client 127.0.0.1:56715] AH02260: Performing full renegotiation: complete handshake protocol (client does support secure renegotiation)
[Thu Jan 15 19:48:06.686830 2015] [ssl:info] [pid 13] [client 127.0.0.1:56715] AH02226: Awaiting re-negotiation handshake
2015/01/15 19:48:06 FATAL: Get https://localhost:443/secret: local error: no renegotiation
[Thu Jan 15 19:48:06.688010 2015] [ssl:error] [pid 13] [client 127.0.0.1:56715] AH02261: Re-negotiation handshake failed: Not accepted by client!?
[Thu Jan 15 19:48:06.688122 2015] [ssl:info] [pid 13] [client 127.0.0.1:56715] AH02006: SSL handshake stopped: connection was closed
[Thu Jan 15 19:48:06.688143 2015] [ssl:info] [pid 13] [client 127.0.0.1:56715] AH01998: Connection closed to child 0 with abortive shutdown (server localhost:443)
```

# Debugging SSL with Wireshark
Wireshark can dissect SSL and show you the HTTP underneath.

Here's how:

1. Install wireshark and add yourself to the wireshark group so you don't have to run it as root all the time.

   ```
   $ sudo yum install wireshark-gnome
   $ sudo usermod -a -G wireshark `whoami`
   ```

   Log in to the new group (so you don't have to log out and back in again).

   ```
   $ newgrp wireshark
   ```
   
   Make sure you're in the group.

   ```
   $ groups
   ... wireshark ...
   ```

1. Run Wireshark.
1. Go to Edit -> Preferences.  Click Protocols.  Go to SSL.  Enter a value for
   SSL debug file.[^1]  (I use /tmp/ssl.debug).  Click RSA keys list.
   Click new and add the following entry for your localhost:

   ```
   IP address: 127.0.0.1
   Port: 9001
   Protocol: http
   Key File: /path/to/server.key
   ```
1. Go to Capture -> Options.  Select the interface you want to listen on.  This
   step is very important and it took me a long time to figure this out.  Do
   not listen on the pseudo-interface "any".  If you listen to "any",
   sometimes you'll get a bunch of TCP retransmissions.
   These retransmission packets will break the SSL dissector.[^2] [^3]
   Instead check the box for the appropriate interface. "em1"
   if you're getting packets from another machine, and "lo" if the
   packets are coming from localhost.
   (You can check all three of these options if you want and Wireshark will
   listen to all three interfaces.  I haven't seen the retransmission problem
   when doing this.)
1. Double click on interface to add a filter.  In the Capture Filter box enter
   "port 9001" to filter calls to those hitting the Candlepin default port.
   Filters are very powerful and you can do a lot of fancy stuff with them.
   Learn more at http://wiki.wireshark.org/CaptureFilters
1. Exit the interface settings by clicking OK and then click Start on the
   Capture Options dialog.
1. You are now sniffing packets
1. Make a SSL connection and you'll start seeing packets populate the
   window.  You can enter "http" in the Filter box if you just want to see the
   HTTP requests or "ssl" if you want to see the SSL stuff too.

#### Footnotes
[^1]: Used to be required because of <https://bugs.wireshark.org/bugzilla/show_bug.cgi?id=6033> but now it's just good practice.
[^2]: <http://www.wireshark.org/lists/wireshark-dev/200805/msg00067.html>
[^3]: <http://www.wireshark.org/lists/wireshark-dev/201202/msg00071.html>
