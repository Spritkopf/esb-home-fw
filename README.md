# esb-home-fw
Firmware modules for DIY smart home system 'esb-home'

This repository is meant to be included as submodule in the firmware projects that use the esb-home protocol. 

# Usage
Add this repository as submodule to your firmware project. Add the submodule folder to the CMake build of your project
with `add_subdirectory`.
The project exposes the following CMake targets:
- `common` - Implementation of the base communication layer: ESB driver, protocol, and command handler
- `binary-sensor` - Application module implementing a "binary sensor"

## ESB Protocol
The module esb_protocol (common/protocol) implements a bidirectional binary communication protocol based on the Enhanced Shockburst Capabilities of the NRF52 SoCs.
An ESB protocol message has the following format:

```
            |----HEADER----------------|---------PAYLOAD----------------|
 * Bytes:   |  0   |   1   | 2 3 4 5 6 | 7          ...               31|
 * Value:   | CMD  | ERROR |   PIPE    |          DATA                  |
 ```
 The same message format is used for commands and answers.
 The Header is mandatory for every message, payload is optional. 

 * `CMD` - Command byte. Each ESB command has a unique command identifier
 * `ERROR` - Error byte. Used for replies to signal execution results. Only relevant for replies, not evaluated for commands
 * `PIPE` - Pipeline address of the source of the message. Used for direct replies, or identification at the central
 * `PAYLOAD` - Data payload. Maximum number of bytes is 30

## Applications
Application modules (like binary-sensor) utilize the ESB protocol and command handler. Each application
module implements its own command table to interact with a central device.

## Example

Pseudo code example on how to use the binary sensor module

```C
    // 5 byte Pipeline address of the device
    uint8_t esb_listener_address[5] = {0x55,0x55,0x55,0x55,1};

    // 5 byte Pipeline address of the central (used for notifications)
    uint8_t esb_central_address[5] = {0x20,0x21,0x22,0x23,1}; 

    esb_protocol_init(esb_listener_address);

    binary_sensor_init(esb_listener_address);
    binary_sensor_set_central_address(g_app_config.esb_central_addr);

    while(1){
        
        // we assume our application uses channel 0, and an interrupt is in place
        // that sets `binary_input_changed` to 1 when a event happens
        if(binary_input_changed == 1){
            binary_input_changed = 0
            binary_sensor_set_channel(0, CHAN_VAL_TRUE);  // sets internal state of the channel
            binary_sensor_publish(); // publish new state to the central
        }

        // process incoming commands and send replies / notifications
        esb_protocol_process();
    }
```
