# Alandr file storage system

### Dependencies

- A custom openSSL wrapper called [SSLWrap](https://github.com/AlbinAndreasson/SSLWrap)
- Crypto++
- libfmt
- pugixml

### Algorithms

- TLS/SSL for communication (negotiated at run-time)
- AES128 with GCM for file encryption and authentication
- SHA256 for the Merkle tree

These were chosen because I've heard they are standard and 
I'm not knowledgeable enough about cryptography to choose myself.

### Running

Unfortunately somethings are hardcoded right now:

Next to the client and server binaries the needs to be a SSL-prepared directory named
"cert" with the key- and certificate-files. 
They need to be named cert.pem and key.pem and can't have passwords.

Next to the client binary a config file needs to exist; a template exists as 
[`exampleconfig.xml`](exampleconfig.xml) and the program should run find with just it

If you send SIGINT to the server it will quit when the client disconnects and 
store the data, just closing will result in data loss

### Commands

Reference guide to the client commands:

- ls - List files on server
- ll - List local files
- ul - Upload file, follow by the file name
- dl - Download, followed by the servers id
- rm - Remove, followed by the server id
- q - quit
