#include "tippertipper.hpp"

void tippertipper::deposit( const name& from_ac, 
							const name& contract_ac, 
							const asset& quantity,
							const string& memo ) 
{
	// check for conditions if either or both of these are true for `from_ac` & `to`
	// M-1: here, the msg will be printed on the console, provided print --to-console is activated on the used nodeos url
	if(contract_ac != get_self() ||  from_ac == get_self()) {		// atleast one of the condition is true
		print("Either money is not sent to the contract or contract itself is the buyer.");
		return;
	}
	
/*	// M-2: here, the contract ac can't be payer for RAM, CPU, NET then it will throw the error as eosio.token::transfer notifies this ACTION
	check( (contract_ac == get_self()) && (from_ac != get_self()), "Either money is not sent to the contract or contract itself is the sender.");
*/
	// Although this is checked in "dcitoken::transfer" action, but fund_token_symbol check is pending. 
	// So, in addition the entire asset check is done using static func defined in "dciico.hpp" file.
	// check quantity is valid for all conditions as 'asset'
	check_quantity(quantity, fund_token_symbol);

	// instantiate the `fund` table
	fund_index fund_table(get_self(), from_ac.value);
	auto fund_it = fund_table.find(fund_token_symbol.raw());

	// update (add/modify) the deposit_qty
	if(fund_it == fund_table.end()) {
		fund_table.emplace(get_self(), [&](auto& row) {
			row.balance = quantity;
		});
	} else {
		fund_table.modify(fund_it, get_self(), [&](auto& row) {
			row.balance += quantity;
		});
	}
}

void tippertipper::tip( const name& fund_owner_ac,
						const name& tip_to_ac,
						const asset& quantity,
						const string& memo )
{
	require_auth(get_self());

	check(fund_owner_ac != tip_to_ac, "fund owner & tip_to_ac can\'t be same.");
	
	check_quantity(quantity, fund_token_symbol);

    check( memo.size() <= 256, "memo has more than 256 bytes" );

	// instantiate the `fund` table
	fund_index fund_table(get_self(), fund_owner_ac.value);
	auto fund_it = fund_table.find(fund_token_symbol.raw());

	check(fund_it != fund_table.end(), "there is no fund available for the given fund_owner_ac.");
	check(fund_it->balance.amount >= quantity.amount, "Insufficient balance in owner\'s fund.");

	action(
		permission_level{get_self(), "active"_n},
		"eosio.token"_n,
		"transfer"_n,
		std::make_tuple(get_self(), tip_to_ac, quantity, memo)
	).send();

	// update the fund balance after successful token transfer to tip_to_ac
	fund_table.modify(fund_it, get_self(), [&](auto& row) {
		row.balance -= quantity;
	});

	if(fund_it->balance.amount == 0) {
		fund_table.erase(fund_it);
	}
}