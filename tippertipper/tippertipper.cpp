#include "tippertipper.hpp"

// --------------------------------------------------------------------------------------------------------------------
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
	// Although this is checked in "eosio.token::transfer" action, but fund_token_symbol check is pending. 
	// So, in addition the entire asset check is done using static func defined in "dciico.hpp" file.
	// check quantity is valid for all conditions as 'asset'
	// check_quantity(quantity, fund_token_symbol, native_token_symbol);

	// if there is no alphabets, just the telegram_id in memo.
	if(count_alpha(memo) == 0) {
		auto tg_id = str_to_uint64t(memo);		// capture the telegram_id by converting from string to uint64_t

		// instantiate the `account` table
		account_index account_table(get_self(), get_self().value);
		auto account_it = account_table.find(tg_id);

		// update (emplace/modify) the deposit_qty
		if(account_it == account_table.end()) {
			account_table.emplace(get_self(), [&](auto& row) {
				row.owner = tg_id;
				row.balances = map<extended_symbol, uint64_t>{
					make_pair(extended_symbol(quantity.symbol, get_first_receiver()), quantity.amount)
				};
			});
		} 
		else {
			account_table.modify(account_it, get_self(), [&](auto& row) {
				// ----------------------------------------------------------------------------
				// code snippet for modifying the value in place of creatify_map() func
				// ----------------------------------------------------------------------------
				bool arithmetic_op = 1;		// 1 for add balance
				auto s_it = std::find_if(row.balances.begin(), row.balances.end(), 
									[&](auto& ms) {return ms.first.get_symbol() == quantity.symbol;});
				
				if(s_it != row.balances.end()) {		// key found
					if (arithmetic_op == 1) {
						s_it->second += quantity.amount;
						// check(false, "values: " + std::to_string(s_it->second) + ", " + std::to_string(s_it->second + qty.amount));					// for testing
					}
					else if (arithmetic_op == 0)
						s_it->second -= quantity.amount;
				}
				else {						// key NOT found
					// check(false, "Key NOT found");					// for testing
					row.balances.insert( make_pair(extended_symbol(quantity.symbol, get_first_receiver()), quantity.amount) );
				}
				// ----------------------------------------------------------------------------
				// ----------------------------------------------------------------------------
			});
		}

	}

}

// --------------------------------------------------------------------------------------------------------------------
void tippertipper::withdraw( /*const name& contract_ac,*/
							 uint64_t from_id,
							 const string& from_username,
							 const name& to_ac,
							 const asset& quantity,
							 const string& memo )
{
	require_auth(get_self());

	// check(is_account(contract_ac), "contract account is not valid.");

	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must withdraw positive quantity" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

	// instantiate the `account` table
	account_index account_table(get_self(), get_self().value);
	auto frm_account_it = account_table.find(from_id);

	check(frm_account_it != account_table.end(), "there is no account available for the given from_id.");

	// compare the amount present in balances map's value
	compare_amount_in_map( frm_account_it->balances, quantity );

    // transfer quantity from tippertipper contract to to_ac
    action(
		permission_level{get_self(), "active"_n},
		capture_contract_in_map(frm_account_it->balances, quantity),
		"transfer"_n,
		std::make_tuple(get_self(), to_ac, quantity, memo)
	).send();

	// update (substract) the balances' value in from_id accounts table
	account_table.modify(frm_account_it, get_self(), [&](auto& row) {
		// ----------------------------------------------------------------------------
		// code snippet for modifying the value in place of creatify_map() func
		// ----------------------------------------------------------------------------
		bool arithmetic_op = 0;		// 0 for sub balance
		auto s_it = std::find_if(row.balances.begin(), row.balances.end(), 
							[&](auto& ms) {return ms.first.get_symbol() == quantity.symbol;});
		
		if(s_it != row.balances.end()) {		// key found
			if (arithmetic_op == 1) {
				s_it->second += quantity.amount;
				// check(false, "values: " + std::to_string(s_it->second) + ", " + std::to_string(s_it->second + qty.amount));					// for testing
			}
			else if (arithmetic_op == 0)
				s_it->second -= quantity.amount;
		}
		else {						// key NOT found
			// check(false, "Key NOT found");					// for testing
			row.balances.insert( make_pair(extended_symbol(quantity.symbol, get_first_receiver()), quantity.amount) );
		}
		// ----------------------------------------------------------------------------
		// ----------------------------------------------------------------------------
	});

	// erase the from_id in accounts table due to zero balances' value
	// this is to save the contract's RAM space
	auto s_it = std::find_if(frm_account_it->balances.begin(), frm_account_it->balances.end(), 
							[&](auto& ms) {return ms.first.get_symbol() == quantity.symbol;});
		
	if(s_it != frm_account_it->balances.end()) {		// key found
		if (s_it->second == 0)			// if val == 0
			account_table.erase(frm_account_it);
	}	
}

// --------------------------------------------------------------------------------------------------------------------
void tippertipper::tip( /*const name& contract_ac,*/
						uint64_t from_id,
						uint64_t to_id,
						const string& from_username,
						const string& to_username,
						const asset& quantity,
						const string& memo )
{
	require_auth(get_self());
	// check(is_account(contract_ac), "contract account is not valid.");

	check(from_id != to_id, "from_id & to_id can\'t be same.");
	check(from_username != to_username, "from_username & to_username can\'t be same.");
	
	check( quantity.is_valid(), "invalid quantity" );
	check( quantity.amount > 0, "must withdraw positive quantity" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

	// instantiate the `account` table
	account_index account_table(get_self(), get_self().value);
	auto frm_account_it = account_table.find(from_id);
	auto to_account_it = account_table.find(to_id);

	check(frm_account_it != account_table.end(), "there is no account available for the given from_id.");

	// compare the amount present in balances map's value
	compare_amount_in_map( frm_account_it->balances, quantity );

	// -------------------------------------------------------------------------
	// update (substract) the balances' value in from_id accounts table
	account_table.modify(frm_account_it, get_self(), [&](auto& row) {
		// ----------------------------------------------------------------------------
		// code snippet for modifying the value in place of creatify_map() func
		// ----------------------------------------------------------------------------
		bool arithmetic_op = 0;		// 0 for sub balance
		auto s_it = std::find_if(row.balances.begin(), row.balances.end(), 
							[&](auto& ms) {return ms.first.get_symbol() == quantity.symbol;});
		
		if(s_it != row.balances.end()) {		// key found
			if (arithmetic_op == 1) {
				s_it->second += quantity.amount;
				// check(false, "values: " + std::to_string(s_it->second) + ", " + std::to_string(s_it->second + qty.amount));					// for testing
			}
			else if (arithmetic_op == 0)
				s_it->second -= quantity.amount;
		}
		else {						// key NOT found
			// check(false, "Key NOT found");					// for testing
			row.balances.insert( make_pair(extended_symbol(quantity.symbol, get_first_receiver()), quantity.amount) );
		}
		// ----------------------------------------------------------------------------
		// ----------------------------------------------------------------------------
	});

	// -------------------------------------------------------------------------
	// update (add) the balances' value in to_id accounts table
	if(to_account_it == account_table.end()) {						// table for to_ac doesn't exist
		account_table.emplace(get_self(), [&](auto& row) {
			row.owner = to_id;
			row.balances = map<extended_symbol, uint64_t>{
				make_pair(extended_symbol(quantity.symbol, capture_contract_in_map( frm_account_it->balances, quantity )), quantity.amount)
			};
		});
	} else {														// table for to_ac exist
		account_table.modify(to_account_it, get_self(), [&](auto& row) {
			// ----------------------------------------------------------------------------
			// code snippet for modifying the value in place of creatify_map() func
			// ----------------------------------------------------------------------------
			bool arithmetic_op = 1;		// 1 for add balance
			auto s_it = std::find_if(row.balances.begin(), row.balances.end(), 
								[&](auto& ms) {return ms.first.get_symbol() == quantity.symbol;});
			
			if(s_it != row.balances.end()) {		// key found
				if (arithmetic_op == 1) {
					s_it->second += quantity.amount;
					// check(false, "values: " + std::to_string(s_it->second) + ", " + std::to_string(s_it->second + qty.amount));					// for testing
				}
				else if (arithmetic_op == 0)
					s_it->second -= quantity.amount;
			}
			else {						// key NOT found
				// check(false, "Key NOT found");					// for testing
				row.balances.insert( make_pair(extended_symbol(quantity.symbol, get_first_receiver()), quantity.amount) );
			}
			// ----------------------------------------------------------------------------
			// ----------------------------------------------------------------------------

		});
	}

	// -------------------------------------------------------------------------
	// erase the from_id in accounts table due to zero balances' value
	// this is to save the contract's RAM space
	auto s_it = std::find_if(frm_account_it->balances.begin(), frm_account_it->balances.end(), 
							[&](auto& ms) {return ms.first.get_symbol() == quantity.symbol;});
		
	if(s_it != frm_account_it->balances.end()) {		// key found
		if (s_it->second == 0)			// if val == 0
			account_table.erase(frm_account_it);
	}	
}