# 测试包含libevent 进行编译

目的：
1.  使用config进行编译，并导入

官网： https://www.openssl.org/

You can run `Configure LIST` to see a list of available platforms.
```
$ ./Configure LIST
BC-32
BS2000-OSD
```

举个栗子：
```
./config no-shared no-idea no-mdc2 no-rc5 --prefix=${CMAKE_BINARY_DIR}
# 注意大写
./Configure no-asm no-shared linux-elf -fPIC --prefix=${CMAKE_BINARY_DIR}
```

官网的编译方式[在这里](https://wiki.openssl.org/index.php/Compilation_and_Installation)

| Option                                                       | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| --prefix=XXX                                                 | See [PREFIX and OPENSSLDIR](https://wiki.openssl.org/index.php/Compilation_and_Installation#PREFIX_and_OPENSSLDIR) in the next section (below). |
| --openssldir=XXX                                             | See [PREFIX and OPENSSLDIR](https://wiki.openssl.org/index.php/Compilation_and_Installation#PREFIX_and_OPENSSLDIR) in the next section (below). |
| -d                                                           | Debug build of the library. Optimizations are disabled (no `-O3` or similar) and `libefence` is used (`apt-get install electric-fence` or `yum install electric-fence`). TODO: Any other features? |
| shared                                                       | Build a shared object in addition to the static archive. You probably need a [RPATH](https://wiki.openssl.org/index.php/Compilation_and_Installation#Using_RPATHs) when enabling `shared` to ensure `openssl` uses the correct `libssl` and `libcrypto` after installation. |
| enable-ec_nistp_64_gcc_128                                   | Use on little endian platforms when GCC supports `__uint128_t`. ECDH is about 2 to 4 times faster. Not enabled by default because `Configure` can't determine it. Enable it if your compiler defines `__SIZEOF_INT128__`, the CPU is little endian and it tolerates unaligned data access. |
| enable-capieng                                               | Enables the Microsoft CAPI engine on Windows platforms. Used to access the Windows Certificate Store. Also see [Using Windows certificate store through OpenSSL](http://openssl.6102.n7.nabble.com/Using-Windows-certificate-store-through-OpenSSL-td46788.html) on the OpenSSL developer list. |
| no-ssl2                                                      | Disables SSLv2. `OPENSSL_NO_SSL2` will be defined in the OpenSSL headers. |
| no-ssl3                                                      | Disables SSLv3. `OPENSSL_NO_SSL3` will be defined in the OpenSSL headers. |
| no-comp                                                      | Disables compression independent of `zlib`. `OPENSSL_NO_COMP` will be defined in the OpenSSL headers. |
| no-idea                                                      | Disables IDEA algorithm. Unlike RC5 and MDC2, IDEA is enabled by default |
| no-asm                                                       | Disables assembly language routines (and uses C routines)    |
| no-dtls                                                      | Disables DTLS in OpenSSL 1.1.0 and above                     |
| no-dtls1                                                     | Disables DTLS in OpenSSL 1.0.2 and below                     |
| no-shared                                                    | Disables shared objects (only a static library is created)   |
| no-hw                                                        | Disables hardware support (useful on mobile devices)         |
| no-engine                                                    | Disables hardware support (useful on mobile devices)         |
| no-threads                                                   | Disables threading support.                                  |
| no-dso                                                       | Disables the OpenSSL DSO API (the library offers a shared object abstraction layer). If you disable DSO, then you must disable Engines also |
| no-err                                                       | Removes all error function names and error reason text to reduce footprint |
| no-npn/no-nextprotoneg                                       | Disables Next Protocol Negotiation (NPN). Use `no-nextprotoneg` for 1.1.0 and above; and `no-npn` otherwise |
| no-psk                                                       | Disables Preshared Key (PSK). PSK provides mutual authentication independent of trusted authorities, but its rarely offered or used |
| no-srp                                                       | Disables Secure Remote Password (SRP). SRP provides mutual authentication independent of trusted authorities, but its rarely offered or used |
| no-ec2m                                                      | Used when configuring FIPS Capable Library with a FIPS Object Module that only includes prime curves. That is, use this switch if you use `openssl-fips-ecp-2.0.5`. |
| no-weak-ssl-ciphers                                          | Disables RC4. Available in OpenSSL 1.1.0 and above.          |
| -DXXX                                                        | Defines XXX. For example, `-DOPENSSL_NO_HEARTBEATS`.         |
| -DPEDANTIC                                                   | Defines PEDANTIC. The library will avoid some undefined behavior, like casting an unaligned byte array to a different pointer type. This define should be used if building OpenSSL with undefined behavior sanitizer (`-fsanitize=undefined`). |
| -DOPENSSL_USE_IPV6=0                                         | Disables IPv6. Useful if OpenSSL encounters incorrect or inconsistent platform headers and mistakenly enables IPv6. Must be passed to `Configure` manually. |
| -L*something*, -l*something*, -K*something*, -Wl,*something* | Linker options, will become part of LDFLAGS.                 |
| -*anythingelse*, +*anythingelse*                             | Compiler options, will become part of CFLAGS.                |


```
```
```
```
