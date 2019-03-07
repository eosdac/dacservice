# dacservice

This contract will relay payments sent to it using a configurable time delay, by default it is 1 hour.

## Memo format

The memo sent with the transfer will dictate where it is sent.  The memo should be in the format account:New Memo

If the memo format is incorrect then the transfer will fail.

The contract will only accept transfers from the account specified at compile time.

## Compiling

`eosio-cpp -DTRANSFER_DELAY=60 -DREQUIREFROM=fromaccount -o dacservice.wasm dacservice.cpp`

