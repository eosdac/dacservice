# dacservice

This contract will relay payments sent to it using a configurable time delay, by default it is 1 hour.

## Memo format

The memo sent with the transfer will dictate where it is sent.  The memo should be in the format account:New Memo

If the memo format is incorrect then the transfer will fail.

The contract will only accept transfers from the account specified at compile time.

## Compiling

`eosio-cpp -DTRANSFER_DELAY=60 -DREQUIREFROM=fromaccount -o dacservice.wasm dacservice.cpp`

## Adding code permission

The active permission of the service account must have code permissions set.

`cleos set account permission --add-code youraccount active youraccount owner`

## Sending tokens

The contract will then forward any payments sent to it by the trusted account

`cleos transfer fromaccount serviceaccount "10 EOS" "toaccount:This is the memo"`

The contract will forward 10 EOS (minus the service fee) to `toaccount` with the memo "This is the memo"
