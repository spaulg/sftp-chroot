# SFTP Chroot

Restricts users to their home directory when making SFTP connections. SSH access is revoked.

## Usage
To root a user to their home directory, simply change their shell to the sftp-chroot binary,
using the command:

```bash
usermod -s /bin/sftp-chroot <user>
```

This will restrict all command line shell access and ensure that when they make an SFTP 
connection, their file access will be restricted to their home directory with a path of /.

## Installation

Binaries are not distributed. You'll need to compile the binaries from source. This is done
using the normal autotools packages.

To compile:

```bash

./configure
make
```

Then with administration privileges, run:

```bash
make install

```

## How It Works

The /bin/sftp-chroot binary is installed as a user shell. It acts as a mediator between
the sftp-server process and the client connection. Each end of the connection has a
stdin and stdout (2 of each, making 4 streams in total). Each stdout stream is appropriately 
parsed by the sftp-chroot binary for packets, which are modified and rewritten back to
the input stream of the other end of the connection.

Outgoing packets from the server to the client have their directory paths adjusted to remove
the home directory, whilst incoming packets are adjusted to add the users home directory.

This forces all communications to be restricted to the confines of the users home directory,
effectively rooting them as chroot would.

The process works because the sftp-chroot shell is above the encryption of SSH but below the 
SFTP server.

## License

Copyright 2013 Simon Paulger <spaulger@codezen.co.uk>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
