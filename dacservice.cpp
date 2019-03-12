#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/transaction.hpp>

//#include <boost/algorithm/string.hpp>
#include <string>

#include "dacservice.hpp"

using namespace eosio;
using namespace std;


void dacservice::transfer(name from,
                          name to,
                          asset quantity,
                          string memo) {

    // Transfer is outgoing, or it is a transfer from system account (unstake, sell ram etc)
    // Don't reject or modify the transfer
    if (to != _self || from == "eosio"_n || from == "eosio.ram"_n || from == "eosio.stake"_n){
        return;
    }

    // Check if memo starts with ! and allow this transfer with no forward
    size_t first_char = memo.find("%");
    if (first_char == 0){
        return;
    }

    eosio_assert(from == name(REQUIRE_FROM), "Can only receive tokens from a known account");

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

    asset new_quantity = (quantity * 100) / 105;

    name to_act = name(act_name_str.c_str());
    eosio_assert(is_account(to_act), "The account name supplied is not valid");

    transaction deferredTrans{};

    deferredTrans.actions.emplace_back(
        action({_self, "active"_n},
               "eosio.token"_n, "transfer"_n,
               std::make_tuple(_self, to_act, new_quantity, final_memo)
        )
    );

    deferredTrans.delay_sec = TRANSFER_DELAY;
    uint128_t sender_id = (uint128_t(to_act.value) << 64) | now();
    deferredTrans.send(sender_id, _self);
}


#define EOSIO_ABI_EX(TYPE, MEMBERS) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      if( action == "onerror"_n.value) { \
         /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
         eosio_assert(code == "eosio"_n.value, "onerror action's are only valid from the \"eosio\" system account"); \
      } \
      auto self = receiver; \
      if( (code == self  && action != "transfer"_n.value) || (code == "eosio.token"_n.value && action == "transfer"_n.value) ) { \
         switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
}

EOSIO_ABI_EX(dacservice,
             (transfer)
)