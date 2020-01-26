# TinyIRC

TinyIRC is an IRC-based chat application written in C.

## Server

### Building

To build the server, issue the following command from the root of the repository:
```bash
$> make server
```

### Running

To run the server, issue the following command from the root of the repository:
```bash
$> make run
```

If the server isn't built `make run` is going to take care of that.

### Commands
Currently-supported server commands are:
- PASS
- NICK
- PRIVMSG
- QUIT
- HELP

The **HELP** command is an extension that sends a list of available commands to client.
