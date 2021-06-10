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
using eosio::extended_symbol;
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
	 * @details - deposit money to the contract ac with memo - telegram user_id e.g. 452435325
	 * 			- accepts any token i.e. EOSIO token e.g. "EOS", "TOE", "FUTBOL" created on a chain
	 * @param from - user account
	 * @param contract_ac - contract ac
	 * @param quantity - in eosio tokens - EOS, TOE, etc.
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
	 * @param from_id - from telegram_id
	 * @param from_username - from telegram username
	 * @param to_ac - to eosio account
	 * @param quantity - qty
	 * @param memo - memo
	 */
	ACTION withdraw( uint64_t from_id,
					 const string& from_username,
					 const name& to_ac,
					 const asset& quantity,
					 const string& memo 
					);

	
	/**
	 * @brief - tip money to a person via just telegram_id
	 * @details - no eosio account needed for both from & to users
	 * 
	 * @param from_id - from telegram_id
	 * @param to_id - to telegram_id
	 * @param from_username - from telegram username
	 * @param to_username - to telegram username
	 * @param quantity - qty
	 * @param memo - memo
	 */
	ACTION tip( uint64_t from_id,
				uint64_t to_id,
				const string& from_username,
				const string& to_username,
				const asset& quantity,
				const string& memo );


	ACTION testdlacbyid( uint64_t tg_id ) {
		require_auth(get_self());

		// instantiate the `account` table
		account_index account_table(get_self(), get_self().value);
		auto account_it = account_table.find(tg_id);

		check(account_it != account_table.end(), "there is no account available for the parsed tg_id.");

		account_table.erase(account_it);
	}

	ACTION testmdqtyusr(uint64_t from_id, const asset& quantity, bool arithmetic_op) {
		require_auth(get_self());

		// instantiate the `account` table
		account_index account_table(get_self(), get_self().value);
		auto frm_account_it = account_table.find(from_id);

		check(frm_account_it != account_table.end(), "there is no account available for the given from_id.");
		
		// update (substract) the balances' value in from_id accounts table
		account_table.modify(frm_account_it, get_self(), [&](auto& row) {

			// NOTE: below is not used as map was not allowed to be used as const like this: `const map<extended_symbol, uint64_t>& m`
			creatify_map(row.balances, quantity, arithmetic_op, "captract"_n);		// arithmetic_op for add/sub - (1/0) balance
			
/*			// ----------------------------------------------------------------------------
			// code snippet for modifying the value in place of creatify_map() func
			// ----------------------------------------------------------------------------
			// bool arithmetic_op = 1;		// 1 for add balance
			auto s_it = std::find_if(row.balances.begin(), row.balances.end(), 
								[&](auto& ms) {return ms.first.get_symbol() == quantity.symbol;});
			
			if(s_it != row.balances.end()) {		// key found
				if (arithmetic_op == 1)
					s_it->second += quantity.amount;
				else if (arithmetic_op == 0)
					s_it->second -= quantity.amount;
			}
			else {						// key NOT found
				row.balances.insert( make_pair(extended_symbol(quantity.symbol, get_first_receiver()), quantity.amount) );
			}
			// ----------------------------------------------------------------------------
			// ----------------------------------------------------------------------------
*/
		});
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
				{ "key": { "symbol": "4,SOV", "contract": "sovmintofeos" }, "value": 30000 }, 
				{ "key": { "symbol": "4,FROG", "contract": "frogfrogcoin" }, "value": 3500000 }, 
				{ "key": { "symbol": "4,PEOS", "contract": "thepeostoken" }, "value": 100000 }, 
				{ "key": { "symbol": "4,KROWN", "contract": "krowndactokn" }, "value": 7169 } 
			]
			
			Here, quantity amount is 30000/10^4 = 3 i.e. asset is "3.0000 SOV"
		*/
		map<extended_symbol, uint64_t> balances; // map with extended_symbol, uint64_t

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
		// char* end;
		uint64_t num = strtoull(s.c_str(), NULL, 10);
	
		//----------------------------------------------
		// M-2
		// uint64_t num = lexical_cast<uint64_t>(s);

		return num;
	}

	/*	
		Here, 2 cases are covered in which the balances map is modified when the row with (owner, balances) exist: 
			- case-1: if the row exists & key is found. i.e. the parsed quantity symbol is found
				- add/sub quantity amount is done by an arithmetic_op (0/1) => (-/+) 
			- case-2: if the row exists & key is NOT found. i.e. the parsed quantity symbol is NOT found 
	*/	
	inline void creatify_map( map<extended_symbol, uint64_t>& m, const asset& qty, 
								bool arithmetic_op, 			// add/sub balance from existing quantity
								const name& token_contract_name ) {
		auto s_it = std::find_if(m.begin(), m.end(), 
							[&](auto& ms) {return ms.first.get_symbol() == qty.symbol;});
		
		if(s_it != m.end()) {		// key found
			if (arithmetic_op == 1)
				s_it->second += qty.amount;
			else if (arithmetic_op == 0)
				s_it->second -= qty.amount;
		}
		else {						// key NOT found
			if (token_contract_name == ""_n)
				m.insert( make_pair(extended_symbol(qty.symbol, get_first_receiver()), qty.amount) );
			else
				m.insert( make_pair(extended_symbol(qty.symbol, capture_contract_in_map(m, qty)), qty.amount) );
		}
	}


	/*	
		Here, 2 cases are covered in which the balances map is checked for <>= than the given quantity, when the row with (owner, balances) exist: 
			- case-1: if the row exists & key is found. i.e. the parsed quantity symbol is found
				- check for >= , else throw message
			- case-2: if the row exists & key is NOT found. i.e. the parsed quantity symbol is NOT found 
				- throw message saying that there is no balances available
	*/	
	inline void compare_amount_in_map( map<extended_symbol, uint64_t> m, const asset& qty ) {
		auto s_it = std::find_if(m.begin(), m.end(), 
							[&](auto& ms) {return ms.first.get_symbol() == qty.symbol;});
		
		if(s_it != m.end()) {		// key found
			check( s_it->second >= qty.amount, "Insufficient balance in from\'s account." );
		}
		else {						// key NOT found
			check( false, "there is no balances available corresponding to the parsed quantity symbol for the given from_id." );
		}
	}

	/*	
		Here, capture the token contract name in which the balances map exists in the row with (owner, balances) exist: 
			- case-1: if the row exists & key is found. i.e. the parsed quantity symbol is found
				- capture the contract ac name from the key
	*/	
	inline name capture_contract_in_map( map<extended_symbol, uint64_t> m, const asset& qty ) {
		name token_contract_ac = ""_n;

		auto s_it = std::find_if(m.begin(), m.end(), 
							[&](auto& ms) {return ms.first.get_symbol() == qty.symbol;});
		
		if(s_it != m.end()) {		// key found
			token_contract_ac = s_it->first.get_contract();
		}
		else {
			check(false, "there is no contract account found with this quantity");
		}

		return token_contract_ac;
	}

};