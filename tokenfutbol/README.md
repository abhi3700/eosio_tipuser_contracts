# Token contract
## Brief
* It is a token contract which is to 
	- [x] create tokens with issuer & max_supply
	- [x] issue tokens to issuer only
	- [x] retire tokens from supply
	- [x] transfer tokens from one account to another
	- [x] open account with even zero balance
	- [x] close account with zero balance

## About
* contract name - `tokenfutbol`
* contract's account name - `tokenfutbol1`
* action
	- `create`
	- `issue`
	- `retire`
	- `transfer`
	- `open`
	- `close`
* table
	- `stats`
	- `accounts`

## Compile
```console
$ eosio-cpp tokenfutbol.cpp -o tokenfutbol.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
Warning, action <create> does not have a ricardian contract
Warning, action <issue> does not have a ricardian contract
Warning, action <retire> does not have a ricardian contract
Warning, action <transfer> does not have a ricardian contract
Warning, action <open> does not have a ricardian contract
Warning, action <close> does not have a ricardian contract
Warning, action <create> does not have a ricardian contract
Warning, action <issue> does not have a ricardian contract
Warning, action <retire> does not have a ricardian contract
Warning, action <transfer> does not have a ricardian contract
Warning, action <open> does not have a ricardian contract
Warning, action <close> does not have a ricardian contract
```

## Deploy
* deploy contract
```console
$ cleost set contract tokenfutbol1 ./
Reading WASM from /mnt/f/Coding/github_repos/eosio_tipuser_contracts/tokenfutbol/tokenfutbol.wasm...
Publishing contract...
executed transaction: b1550e780d0a65feea3aa2ee3b74ac949afa6487f420e3d9b64e5959e5f1fc02  6984 bytes  883 us
#         eosio <= eosio::setcode               {"account":"tokenfutbol1","vmtype":0,"vmversion":0,"code":"0061736d0100000001a0011b60000060017e00600...
#         eosio <= eosio::setabi                {"account":"tokenfutbol1","abi":"0e656f73696f3a3a6162692f312e310008076163636f756e7400010762616c616e6...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* Adding eosio.code to permissions (for inline actions)
```console
$ cleost set account permission tokenfutbol1 active --add-code
executed transaction: 6d80cc6213aa1e3520ebb835c4e157fc2831ede9141b3c5274f73a22a6ed1e0e  184 bytes  148 us
#         eosio <= eosio::updateauth            {"account":"tokenfutbol1","permission":"active","parent":"owner","auth":{"threshold":1,"keys":[{"key...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Testing
### Testnet
#### Action - `create`
* create the token with max_supply - 100 M FUTBOL tokens.
```console
$ cleost push action tokenfutbol1 create '["tipjaguakicr", "100000000.0000 FUTBOL"]' -p tokenfutbol1@active
executed transaction: 982c4e4eb104b3c3183ddf86cbcb83654022ac8113161755a1a70f7d972fed67  120 bytes  132 us
#  tokenfutbol1 <= tokenfutbol1::create         {"issuer":"tipjaguakicr","maximum_supply":"100000000.0000 FUTBOL"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### Action - `issue`
* issue 10 M to issuer - `tipjaguakicr`
```console
$ cleost push action tokenfutbol1 issue '["tipjaguakicr", "10000000.0000 FUTBOL", "issue 10 M tokens"]' -p tipjaguakicr@active
executed transaction: 5e65d1824145bfeb984fdceeb1de11768b10eb648ae00f080de4e4dadf839f61  136 bytes  168 us
#  tokenfutbol1 <= tokenfutbol1::issue          {"to":"tipjaguakicr","quantity":"10000000.0000 FUTBOL","memo":"issue 10 M tokens"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

#### Action - `transfer`
* issuer - `tipjaguakicr` transfer some tokens to a user - `tipuser11111`
```console
$ cleost push action tokenfutbol1 transfer '["tipjaguakicr", "tipuser11111", "100.0000 DCI", "transfer DCI tokens"]' -p tipjaguakicr@active
executed transaction: e4334c81c4f2894c8de88c8e7561ebcb6e7be4705156e1e71a0d0483cb58686d  160 bytes  214 us
#  tokenfutbol1 <= tokenfutbol1::transfer       {"from":"tipjaguakicr","to":"dci111111ico","quantity":"100000.0000 DCI","memo":"transfer DCI tokens ...
#  tipjaguakicr <= tokenfutbol1::transfer       {"from":"tipjaguakicr","to":"dci111111ico","quantity":"100000.0000 DCI","memo":"transfer DCI tokens ...
#  dci111111ico <= tokenfutbol1::transfer       {"from":"tipjaguakicr","to":"dci111111ico","quantity":"100000.0000 DCI","memo":"transfer DCI tokens ...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```