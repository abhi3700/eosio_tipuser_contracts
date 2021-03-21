#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
// #include <eosio/crypto.hpp>
#include <string>
#include <vector>
#include <map>
#include <cstdlib> 		// M-1: for strtoull
// #include <boost/lexical_cast.hpp>		// M-2: for string to uint64_t (Using Boost lib)		compile using `$ eosio-cpp tippertipper.cpp -I /mnt/f/Coding/github_repos -o tippertipper.wasm`


using eosio::contract;
using eosio::print;
using eosio::name;
using eosio::multi_index;
using eosio::const_mem_fun;
using eosio::indexed_by;
using eosio::asset;
using eosio::check;
using eosio::permission_level;
using eosio::datastream;
// using eosio::current_time_point;
using eosio::action;
using eosio::same_payer;
using eosio::symbol;
// using eosio::require_recipient;
// using eosio::checksum256;
// using eosio::action_wrapper;

using std::string;
using std::vector;
using std::map;
using std::make_pair;
// using boost::lexical_cast;

CONTRACT tippertipper : public contract
{
private:
	// const symbol fund_token_symbol;
	// const symbol native_token_symbol;

public:
	using contract::contract;

	tippertipper(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds)
				// fund_token_symbol("EOS", 4),
				// native_token_symbol("FUTBOL", 4)
				{}

	/**
	 * @brief - deposit money to the contract ac
	 * @details - deposit money to the contract ac with memo including telegram user_id e.g. 452435325
	 * 			- accepts any token i.e. EOSIO token e.g. "EOS", "TOE", "FUTBOL" created on a chain
	 * @param from - user account
	 * @param contract_ac - contract ac
	 * @param quantity - in EOS
	 * @param memo - purpose which should include telegram user_id
	 */
	[[eosio::on_notify("*::transfer")]]
	void deposit( const name& from_ac, 
					const name& contract_ac, 
					const asset& quantity,
					const string& memo );


	/**
	 * @brief - withdraw amount
	 * @details - withdraw amount from_id to to_ac
	 * 
	 * @param contract_ac - contract account name
	 * @param from_id - from telegram_id
	 * @param from_username - from telegram username
	 * @param to_ac - to eosio account
	 * @param quantity - qty
	 * @param memo - memo
	 */
	ACTION withdraw( /*const name& contract_ac,*/
					 uint64_t from_id,
					 const string& from_username,
					 const name& to_ac,
					 const asset& quantity,
					 const string& memo );

	
	/**
	 * @brief - tip money to a person via just telegram_id
	 * @details - no eosio account needed for both from & to users
	 * 
	 * @param contract_ac - contract account name
	 * @param from_id - from telegram_id
	 * @param to_id - to telegram_id
	 * @param from_username - from telegram username
	 * @param to_username - to telegram username
	 * @param quantity - qty
	 * @param memo - memo
	 */
	ACTION tip( /*const name& contract_ac,*/
				uint64_t from_id,
				uint64_t to_id,
				const string& from_username,
				const string& to_username,
				const asset& quantity,
				const string& memo );


	ACTION testdelfund( uint64_t owner_id ) {
		require_auth(get_self());

		// instantiate the `account` table
		account_index account_table(get_self(), get_self().value);
		auto account_it = account_table.find(owner_id);

		check(account_it != account_table.end(), "there is no account available for the parsed owner_id.");

		account_table.erase(account_it);
	}
	
	// static void check_quantity( const asset& quantity, const symbol& token1_sym, const symbol& token2_sym ) {
	// 	check(quantity.is_valid(), "invalid quantity");
	// 	check(quantity.amount > 0, "must withdraw positive quantity");
	// 	check( (quantity.symbol == token1_sym) || (quantity.symbol == token2_sym) , "symbol precision mismatch. Also, could be bcoz of sending unaccepted tokens to this contract.");
	// }
	// -----------------------------------------------------------------------------------------------------------------------
	TABLE account
	{
		uint64_t owner;		// telegram_id, e.g. 452435325.

		/*
			[
			 {("symbol_name", "EOS"), ("symbol_precision", "4"), ("contract", "eosio.token"), ("value", "90000")},
			 {("symbol_name", "FUTBOL"), ("symbol_precision", "4"), ("contract", "tokenfutbol1"), ("value", "110000")}
			]

			Here, amount in 1st element of vector is 90000/10^4 = 9 i.e. asset is "9.0000 EOS"
		*/
		vector<map<string, string>> balances; // vector of maps e.g. 

		auto primary_key() const { return owner; }
	};

	using account_index = multi_index<"accounts"_n, account>;


	// -----------------------------------------------------------------------------------------------------------------------
	// UTILITY functions
	// -----------------------------------------------------------------------------------------------------------------------
	// count no. of alphabets in a string
	inline uint64_t count_alpha( const string& str ) {
		uint64_t count = 0;		// no. of alphabet chars

	    for (int i=0; i<=str.size(); ++i) {
	        if (isalpha(str[i]))
	            ++count;
	    }

	    return count;
	}

	// string to uint64_t
	inline uint64_t str_to_uint64t(const string& s) {
		// M-1
		char* end;
		uint64_t num = strtoull(s.c_str(), &end, 10);
	
		//----------------------------------------------
		// M-2
		// uint64_t num = lexical_cast<uint64_t>(s);

		return num;
	}

	// // find index of balances vector where contract key's value is contract (in string)
	// inline uint64_t find_idx_balances( vector<map<string, string>> balances, const string& contract_str ) {
	// 	uint64_t search_index = -1;

	// 	for (int i = 0; i < balances.size(); ++i)
	// 	{
	// 		if(balances[i]["contract"] == contract_str) {
	// 			search_index = i;
	// 			break;
	// 		}
	// 	}

	// 	return search_index;
	// }

	// find index of balances vector where contract key's value is symbol name & precision (in string)
	inline uint64_t find_idx_balances( vector<map<string, string>> balances, const asset& quantity ) {
		uint64_t search_index = -1;

		for (int i = 0; i < balances.size(); ++i)
		{
			if( symbol(balances[i]["symbol_name"], /*uint8_t*/stoi(balances[i]["symbol_precision"])) == quantity.symbol ) {
				search_index = i;
				break;
			}
		}

		return search_index;
	}


};