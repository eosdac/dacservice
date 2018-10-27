#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/transaction.hpp>

#include <boost/algorithm/string.hpp>
#include <string>

#include "dacservice.hpp"

using namespace eosio;
using namespace std;


void dacservice::transfer(name from,
                          name to,
                          asset quantity,
                          string memo) {

    if (to != _self){
        return;
    }

    eosio_assert(from == eosio::string_to_name(REQUIRE_FROM), "Can only receive tokens from a known account");

    size_t pos = memo.find(":");

    string act_name_str;
    string final_memo;

    if (pos == string::npos){
        act_name_str = memo;
    }
    else {
        act_name_str = memo.substr(0, pos);
        final_memo = memo.substr(pos+1);
    }


    account_name to_act = string_to_name(act_name_str.c_str());
    eosio_assert(is_account(to_act), "The account name supplied is not valid");

    transaction deferredTrans{};

    deferredTrans.actions.emplace_back(
        action({_self, N(xfer)},
               N(eosio.token), N(transfer),
               std::make_tuple(_self, to_act, quantity, final_memo)
        )
    );

    deferredTrans.delay_sec = TRANSFER_DELAY;
    uint128_t sender_id = (uint128_t(to_act) << 64) | now();
    deferredTrans.send(sender_id, _self);
}

#define EOSIO_ABI_EX(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      if( action == N(onerror)) { \
         /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
         eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
      } \
      auto self = receiver; \
      if( (code == self  && action != N(transfer)) || (code == N(eosio.token) && action == N(transfer)) ) { \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}

EOSIO_ABI_EX(dacservice,
             (transfer)
)
