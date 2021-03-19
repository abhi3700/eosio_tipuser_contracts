#pragma once
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
// #include <eosio/crypto.hpp>
#include <string>

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


CONTRACT tippertipper : public contract
{
private:
	const symbol fund_token_symbol;
	const symbol native_token_symbol_1;

public:
	using contract::contract;

	tippertipper(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds),
				fund_token_symbol("EOS", 4) 
				native_token_symbol_1("EOS", 4) 
				{}

	/**
	 * @brief - deposit money to the contract ac
	 * @details - deposit money to the contract ac with memo including telegram user_id e.g. 452435325
	 * 			- only chain token i.e. EOSIO token e.g. "EOS", "TLOS"
	 * @param from - user account
	 * @param contract_ac - contract ac
	 * @param quantity - in EOS
	 * @param memo - purpose which should include telegram user_id
	 */
	[[eosio::on_notify("eosio.token::transfer")]]
	void deposit( const name& from, 
					const name& contract_ac, 
					const asset& quantity,
					const string& memo );

	/**
	 * @brief - tip money to a person
	 * @details - tip money to a person
	 * 
	 * @param fund_owner - user's fund
	 * @param tip_to - tip to user
	 * @param quantity - qty
	 * @param memo - memo
	 */
	ACTION tip( const name& fund_owner_ac,
				const name& tip_to_ac,
				const asset& quantity,
				const string& memo );


/*	ACTION withdraw( const name& user,
					 const name& quantity,
					 const string& memo );

*/	
	
	static void check_quantity( const asset& quantity, const symbol& token1_sym, const symbol& token2_sym ) {
		check(quantity.is_valid(), "invalid quantity");
		check(quantity.amount > 0, "must withdraw positive quantity");
		check( (quantity.symbol == token1_sym) || (quantity.symbol == token2_sym) , "symbol precision mismatch. Also, could be bcoz of sending unaccepted tokens to this contract.");
	}
	// -----------------------------------------------------------------------------------------------------------------------
	TABLE fund
	{
		asset balance;			// in EOSIO token i.e. chain token e.g. "EOS", "TLOS"

		auto primary_key() const { return balance.symbol.raw(); }
	};

	using fund_index = multi_index<"fund"_n, fund>;

};