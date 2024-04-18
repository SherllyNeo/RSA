# RSA
A useless RSA implimentation in C

Please do not use for anything. 


# example usage:

## encryption
rsa -e -t "example plaintext" -k "./keys/pubKey.pub" 

OUTPUT:
encrytping: example plaintext 
from ./keys/pubKey.pub found key: 5 and modulo 1934701 
[+] done 
 804669,1010439,1137219,1497197,225423,1161374,804669,664515,225423,1161374,1137219,1527829,648876,302520,804669,1010439,302520


## decryption
rsa --decrypt --text "804669,1010439,1137219,1497197,225423,1161374,804669,664515,225423,1161374,1137219,1527829,648876,302520,804669,1010439,302520" --key_file_path "./keys/privKey.priv"

OUTPUT:
decrypting 804669,1010439,1137219,1497197,225423,1161374,804669,664515,225423,1161374,1137219,1527829,648876,302520,804669,1010439,302520
from ./keys/privKey.priv found key: 386381 and modulo 1934701
[+] done
 example plaintext
