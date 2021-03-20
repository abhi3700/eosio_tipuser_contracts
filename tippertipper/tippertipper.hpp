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
	const symbol native_token_symbol;

public:
	using contract::contract;

	tippertipper(name receiver, name code, datastream<const char*> ds) : 
				contract(receiver, code, ds),
				fund_token_symbol("EOS", 4),
				native_token_symbol("FUTBOL", 4)
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

	ACTION testdelfund( const name& fund_owner_ac, uint64_t token_id ) {
		require_auth(get_self());

		// instantiate the `fund` table
		fund_index fund_table(get_self(), fund_owner_ac.value);
		auto fund_it = fund_table.find(token_id);

		check(fund_it != fund_table.end(), "there is no fund available for the given fund_owner_ac.");

		fund_table.erase(fund_it);
	}
	
	// static void check_quantity( const asset& quantity, const symbol& token1_sym, const symbol& token2_sym ) {
	// 	check(quantity.is_valid(), "invalid quantity");
	// 	check(quantity.amount > 0, "must withdraw positive quantity");
	// 	check( (quantity.symbol == token1_sym) || (quantity.symbol == token2_sym) , "symbol precision mismatch. Also, could be bcoz of sending unaccepted tokens to this contract.");
	// }
	// -----------------------------------------------------------------------------------------------------------------------
	TABLE fund
	{
		uint64_t token_id;		// created using entire - symbol & precision, encoded as integer.
		name token_contract_ac;	// e.g "eosio.token", "tokenfutbol1"
		asset balance;			// i.e. EOSIO token e.g. "EOS", "TOE", "FUTBOL" created on a chain

		auto primary_key() const { return token_id; }
		uint64_t by_tokenac() const { return token_contract_ac.value; }
	};

	using fund_index = multi_index<"fund"_n, fund,
						indexed_by< "bytokenac"_n, const_mem_fun<fund, uint64_t, &fund::by_tokenac>>
						>;
};