
## autoconf

command for arm64 macos

```bash
curl -o curl.tar.gz https://curl.se/download/curl-8.6.0.tar.gz

LDFLAGS="-L/opt/homebrew/Cellar/libpsl/0.21.5/lib" CPPFLAGS="-I/opt/homebrew/Cellar/libpsl/0.21.5/include" ./configure --disable-ftp --disable-ftps --disable-gopher --disable-gophers --disable-imap --disable-imaps --disable-pop3 --disable-pop3s --disable-rtsp --disable-scp --disable-sftp --disable-smb --disable-smbs --disable-smtp --disable-smtps --disable-tftp --disable-ldap --disable-ldaps --disable-dict --disable-file --disable-mqtt --enable-http --enable-telnet --with-ssl

make
```

output:
```bash
❯ ./src/curl --version
curl 8.6.0 (aarch64-apple-darwin23.0.0) libcurl/8.6.0 OpenSSL/3.2.1 zlib/1.2.12 libpsl/0.21.5
Release-Date: 2024-01-31
Protocols: http https ipfs ipns telnet
Features: alt-svc AsynchDNS HSTS HTTPS-proxy IPv6 Largefile libz NTLM PSL SSL threadsafe TLS-SRP UnixSockets
```

## cmake

```bash
mkdir build && cd build
cmake -DCURL_DISABLE_DICT=ON \
      -DCURL_DISABLE_FILE=ON \
      -DCURL_DISABLE_FTP=ON \
      -DCURL_DISABLE_GOPHER=ON \
      -DCURL_DISABLE_IMAP=ON \
      -DCURL_DISABLE_LDAP=ON \
      -DCURL_DISABLE_LDAPS=ON \
      -DCURL_DISABLE_MQTT=ON \
      -DCURL_DISABLE_POP3=ON \
      -DCURL_DISABLE_RTSP=ON \
      -DCURL_DISABLE_SMB=ON \
      -DCURL_DISABLE_SMTP=ON \
      -DCURL_DISABLE_TFTP=ON \
      ..

make
```
output:
```bash
❯ ./src/curl --version
curl 8.6.0 (aarch64-apple-darwin23.0.0) libcurl/8.6.0 OpenSSL/3.2.1 zlib/1.2.12 libpsl/0.21.5
Release-Date: 2024-01-31
Protocols: http https ipfs ipns telnet
Features: alt-svc AsynchDNS HSTS HTTPS-proxy IPv6 Largefile libz NTLM PSL SSL threadsafe TLS-SRP UnixSockets
```

