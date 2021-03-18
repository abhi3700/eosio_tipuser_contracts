# TipUser contract
## Brief
* It is a tipuser contract where:
	- [ ] tip user based on requester's request from fund balance in contract account. 

## About
* contract name - `tippertipper`
* contract's account name - `tippertipper`
* action
	- `deposit`
* table
	- `fund`

## Compile
```console
$ eosio-cpp tippertipper.cpp -o tippertipper.wasm
Warning, empty ricardian clause file
Warning, empty ricardian clause file
```

## Deploy
* deploy contract
```console
$ cleost set contract tippertipper ./
Reading WASM from /mnt/f/Coding/github_repos/eosio_tipuser_contracts/tippertipper/tippertipper.wasm...
Publishing contract...
executed transaction: dac607782007ca349e248cc5e75ca29721133ff4ae361e85415db156f207bd90  3488 bytes  496 us
#         eosio <= eosio::setcode               {"account":"tippertipper","vmtype":0,"vmversion":0,"code":"0061736d01000000018a011760000060027f7f006...
#         eosio <= eosio::setabi                {"account":"tippertipper","abi":"0e656f73696f3a3a6162692f312e3100010466756e6400010762616c616e6365056...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* Adding eosio.code to permissions (for inline actions)
```console
$ cleost set account permission tippertipper active --add-code
executed transaction: 1e3e160957fac0094230942b2b248c6ffa74ca400d757a291d2d09cba56b10e4  184 bytes  145 us
#         eosio <= eosio::updateauth            {"account":"tippertipper","permission":"active","parent":"owner","auth":{"threshold":1,"keys":[{"key...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```

## Testing
### Action - `deposit` (Payable action)
* `tipuser11111` transfer some quantity to contract account
	- check for any tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
	- show the eosio.token balance of `tipuser11111`
```console
$ cleost get table eosio.token tipuser11111 accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "100.0000 EOS"
      },
      "payer": "junglefaucet"
    },{
      "data": {
        "balance": "100.0000 JUNGLE"
      },
      "payer": "junglefaucet"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
	- transfer
```console
$ cleost push action eosio.token transfer '["tipuser11111", "tippertipper", "10.0000 EOS", "transfer for fund"]' -p tipuser11111@active
executed transaction: 37e1969218c20e5f2b7733b3bc715949cbc0998e7448377c5e0938a2715df255  144 bytes  277 us
#   eosio.token <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"transfer for fund"}
#  tipuser11111 <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"transfer for fund"}
#  tippertipper <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"transfer for fund"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
	- Now, check for any tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "balance": "10.0000 EOS"
      },
      "payer": "tippertipper"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```

### Extra

## TODO

## Troubleshooting
