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
