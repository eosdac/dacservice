#include <eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

#define _STRINGIZE(x) #x
#define STRINGIZE(x) _STRINGIZE(x)

#ifndef TRANSFER_DELAY
#define TRANSFER_DELAY 60*60*1
#endif

#ifdef REQUIREFROM
#define REQUIRE_FROM STRINGIZE(REQUIREFROM)
#endif

#ifndef REQUIRE_FROM
#define REQUIRE_FROM "eosdacthedac"
#endif

class dacservice : public contract {

public:

    dacservice( name s, name code, datastream<const char*> ds ) : contract(s,code,ds){}

    ~dacservice() {}

/**
 * Forwarding contract
 *
 * @param from The account to observe as the source of funds for a transfer
 * @param to The account to observe as the destination of funds for a transfer
 * @param quantity
 * @param memo The memo should be in the format account:Final Memo.  This account will check that the forwarding account is valid and then forward the transfer with a deferred transaction
 */
    void transfer(name from,
                  name to,
                  asset quantity,
                  string memo);


};
