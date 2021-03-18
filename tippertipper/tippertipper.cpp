#include "tippertipper.hpp"

void tippertipper::deposit( const name& from, 
			  const name& contract_ac, 
			  const asset& quantity,
			  const string& memo ) 
{
	// check for conditions if either or both of these are true for `from` & `to`
/*	if(contract_ac != get_self() ||  from == get_self()) {		// atleast one of the condition is true
		print("Either money is not sent to the contract or contract itself is the buyer.");
		return;
	}
*/	check( (contract_ac == get_self()) && (from != get_self()), "Either money is not sent to the contract or contract itself is the sender.");

	// Although this is checked in "dcitoken::transfer" action, but fund_token_symbol check is pending. 
	// So, in addition the entire asset check is done using static func defined in "dciico.hpp" file.
	// check quantity is valid for all conditions as 'asset'
	check_quantity(quantity, quantity.symbol);

	// instantiate the `fund` table
	fund_index fund_table(get_self(), from.value);
	auto fund_it = fund_table.find(quantity.symbol.raw());

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