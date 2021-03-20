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
Warning, action <tip> does not have a ricardian contract
Warning, action <testdelfund> does not have a ricardian contract
Warning, action <tip> does not have a ricardian contract
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
### Action - `deposit` (on_notify action)
#### EOS
* `tipuser11111` transfer some quantity (in EOS) to contract account
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
	- show the eosio.token balance (EOS) of `tipuser11111`
```console
$ cleost get table eosio.token tipuser11111 accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "98.0000 EOS"
      },
      "payer": "tipuser11111"
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
executed transaction: 168dd0c86f20405c66e0f32f164dc60f38d7e9c42e0cd2762dc14b51db87f915  144 bytes  239 us
#   eosio.token <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"transfer for fund"}
#  tipuser11111 <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"transfer for fund"}
#  tippertipper <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"transfer for fund"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "token_id": 1397703940,
        "token_contract_ac": "eosio.token",
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
  - show the eosio.token (EOS) balance of `tipuser11111`
```console
$ cleost get table eosio.token tipuser11111 accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "88.0000 EOS"
      },
      "payer": "tipuser11111"
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

#### FUTBOL
* `tipuser11111` transfer some quantity (in FUTBOL) to contract account
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "token_id": 1397703940,
        "token_contract_ac": "eosio.token",
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
  - show the FUTBOL token balance of `tipuser11111`
```console
$ cleost get table tokenfutbol1 tipuser11111 accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "100.0000 FUTBOL"
      },
      "payer": "tipuser11111"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
  - transfer
```console
$ cleost push action tokenfutbol1 transfer '["tipuser11111", "tippertipper", "10.0000 FUTBOL", "deposit for fund"]' -p tipuser11111@active
executed transaction: 0c4c058cf212668b95e725f460160acf06137b3667c1fbc459c0f6b2d92deacc  144 bytes  180 us
#  tokenfutbol1 <= tokenfutbol1::transfer       {"from":"tipuser11111","to":"tippertipper","quantity":"1.0000 FUTBOL","memo":"deposit for fund"}
#  tipuser11111 <= tokenfutbol1::transfer       {"from":"tipuser11111","to":"tippertipper","quantity":"1.0000 FUTBOL","memo":"deposit for fund"}
#  tippertipper <= tokenfutbol1::transfer       {"from":"tipuser11111","to":"tippertipper","quantity":"1.0000 FUTBOL","memo":"deposit for fund"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "token_id": 1397703940,
        "token_contract_ac": "eosio.token",
        "balance": "10.0000 EOS"
      },
      "payer": "tippertipper"
    },{
      "data": {
        "token_id": "21479244531320324",
        "token_contract_ac": "tokenfutbol1",
        "balance": "10.0000 FUTBOL"
      },
      "payer": "tippertipper"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
  - show the FUTBOL token balance of `tipuser11111`
```console
$ cleost get table tokenfutbol1 tipuser11111 accounts --show-payer
{
  "rows": [{
      "data": {
        "balance": "90.0000 FUTBOL"
      },
      "payer": "tipuser11111"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```

### Action - `tip`
#### EOS
* `tipuser11111` tips "1 EOS" to `tipuser11112`
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "token_id": 1397703940,
        "token_contract_ac": "eosio.token",
        "balance": "10.0000 EOS"
      },
      "payer": "tippertipper"
    },{
      "data": {
        "token_id": "21479244531320324",
        "token_contract_ac": "tokenfutbol1",
        "balance": "9.9000 FUTBOL"
      },
      "payer": "tippertipper"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
  - tip
```console
$ cleost push action tippertipper tip '["tipuser11111", "tipuser11112", "1.0000 EOS", "tip for enjoy"]' -p tippertipper@active
executed transaction: 38453fec4b8233b239692a86e3b1877619a80a9024d08c22b49e2547f8a6f2ad  144 bytes  190 us
#  tippertipper <= tippertipper::tip            {"fund_owner_ac":"tipuser11111","tip_to_ac":"tipuser11112","quantity":"1.0000 EOS","memo":"tip for e...
#   eosio.token <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11112","quantity":"1.0000 EOS","memo":"tip for enjoy"}
#  tippertipper <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11112","quantity":"1.0000 EOS","memo":"tip for enjoy"}
>> Either money is not sent to the contract or contract itself is the buyer.
#  tipuser11112 <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11112","quantity":"1.0000 EOS","memo":"tip for enjoy"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
``` 
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "token_id": 1397703940,
        "token_contract_ac": "eosio.token",
        "balance": "9.0000 EOS"
      },
      "payer": "tippertipper"
    },{
      "data": {
        "token_id": "21479244531320324",
        "token_contract_ac": "tokenfutbol1",
        "balance": "9.9000 FUTBOL"
      },
      "payer": "tippertipper"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```

#### FUTBOL
* `tipuser11111` tips "0.1 FUTBOL" to `tipuser11112`
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "token_id": 1397703940,
        "token_contract_ac": "eosio.token",
        "balance": "10.0000 EOS"
      },
      "payer": "tippertipper"
    },{
      "data": {
        "token_id": "21479244531320324",
        "token_contract_ac": "tokenfutbol1",
        "balance": "10.0000 FUTBOL"
      },
      "payer": "tippertipper"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
  - tip
```console
$ cleost push action tippertipper tip '["tipuser11111", "tipuser11112", "0.1000 FUTBOL", "tip for enjoy"]' -p tippertipper@active
executed transaction: 0ea36ddad5614ac420e418b021f3f8033381a8c943bcd494e634f323b11111e5  144 bytes  175 us
#  tippertipper <= tippertipper::tip            {"fund_owner_ac":"tipuser11111","tip_to_ac":"tipuser11112","quantity":"0.1000 FUTBOL","memo":"tip fo...
#  tokenfutbol1 <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11112","quantity":"0.1000 FUTBOL","memo":"tip for enjoy"}
#  tippertipper <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11112","quantity":"0.1000 FUTBOL","memo":"tip for enjoy"}
>> Either money is not sent to the contract or contract itself is the buyer.
#  tipuser11112 <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11112","quantity":"0.1000 FUTBOL","memo":"tip for enjoy"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
``` 
  - show the tippertipper fund balance of `tipuser11111`
```console
$ cleost get table tippertipper tipuser11111 fund --show-payer
{
  "rows": [{
      "data": {
        "token_id": 1397703940,
        "token_contract_ac": "eosio.token",
        "balance": "10.0000 EOS"
      },
      "payer": "tippertipper"
    },{
      "data": {
        "token_id": "21479244531320324",
        "token_contract_ac": "tokenfutbol1",
        "balance": "9.9000 FUTBOL"
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
* del fund table
```console
$ cleost push action tippertipper testdelfund '["tipuser11111", "1397703940"]' -p tippertipper@active
executed transaction: abd1ed6b56273b387f2f101e63ae7f50766566b3b948f97d8bb0c5b91667057d  112 bytes  250 us
#  tippertipper <= tippertipper::testdelfund    {"fund_owner_ac":"tipuser11111","token_id":1397703940}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* transfer EOS token back
```console
$ cleost push action eosio.token transfer '["tippertipper", "tipuser11111", "9.0000 EOS", "return fund"]' -p tippertipper@active
executed transaction: 58e02ed6b81837208dbb9900ec5d279747ee1bb232355d4235985eec98612599  136 bytes  169 us
#   eosio.token <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11111","quantity":"9.0000 EOS","memo":"return fund"}
#  tippertipper <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11111","quantity":"9.0000 EOS","memo":"return fund"}
>> Either money is not sent to the contract or contract itself is the buyer.
#  tipuser11111 <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11111","quantity":"9.0000 EOS","memo":"return fund"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* transfer FUTBOL token back
```console
$ cleost push action tokenfutbol1 transfer '["tippertipper", "tipuser11111", "9.9000 FUTBOL", "return fund"]' -p tippertipper@active
executed transaction: 89a821e0193e65fee6bd6e03c845292a877edd3358e9156898dd29af9a32c615  136 bytes  145 us
#  tokenfutbol1 <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11111","quantity":"9.9000 FUTBOL","memo":"return fund"}
#  tippertipper <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11111","quantity":"9.9000 FUTBOL","memo":"return fund"}
>> Either money is not sent to the contract or contract itself is the buyer.
#  tipuser11111 <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11111","quantity":"9.9000 FUTBOL","memo":"return fund"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```


## TODO

## Troubleshooting
