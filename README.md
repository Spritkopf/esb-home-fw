# esb-home-fw
Firmware modules for DIY smart home system 'esb-home'

This repository is meant to be included as submodule in the firmware projects that use the esb-home protocol. 

# usage
Add this repository as submodule to your firmware project. Add the submodule folder to the include paths of the build system (Makefile, CMake etc)

## ESB Protocol
The module esb_protocol implements a bidirectional binary communication protocl based on the Enhanced Shockburst Capabilities of the NRF52 SoCs.
A ESB protocol message has the following format:

```
            |----HEADER----|------- PAYLOAD----------------|
 * Bytes:   |  0   |   1   |           N                   |
 * Value:   | CMD  | ERROR | 
 ```
 The same message format is used for commands and answers.
 The Header is mandatory for every message, payload is optional. 

 * `CMD` - Command byte. Each ESB command has a unique command identifier
 * `ERROR` - Error byte. Used for replies to signal execution results. Only relevant for replies, not evaluated for commands
 * `PAYLOAD` - Data payload. Maximum number of bytes is 30