# dacservice

This contract will relay payments sent to it using a configurable time delay, by default it is 23 hours.

## Memo format

The memo sent with the transfer will dictate where it is sent.  The memo should be in the format account:New Memo

If the memo format is incorrect then the transfer will fail

## Compiling

`eosio-cpp -DTRANSFER_DELAY=60 -o dacservice.wasm dacservice.cpp`

