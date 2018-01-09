## an bash shell emulator
this is a simple bash program that run background processes, redirect file i/o,
and launch any program from the terminal. 

```
:ls
sic.c  makefilep3testscript  sigcont smallsh    smallsh.h  smallsh.s
basic.o  mytestresultsREADME      sigcont.c  smallsh.c  smallsh.o
smallsh.tar.bz2
:ls > junk
:wc < junk
15  15 146
:test -f badfile
:status
exit value 1
:sleep 100 &
background pid is: 5662
:pkill sleep
background pid 5662 is done: terminated by signal 15
:

```

## OTP -- one time pad encryption
It is a demonstration of using OTP to encrypt file using:
* otp_dec_d (daemon)
* otp_dec 
* otp_enc_d (daemon)
* otp_enc

To start daemons:
```
otp_enc_d $encport &
Otp_dec_d $decport &
```

Generate the random key and encrypt using the encryption client:
```
keygen 70000 > key70000
otp_enc plaintext1 key70000 $encport > ciphertext1
```

To decrypt and get the original file and check:
```
otp_dec ciphertext1 key70000 $decport > plaintext1_a
cmp plaintext1 plaintext1_a
```


