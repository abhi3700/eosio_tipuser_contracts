# TipUser contract
## Brief
* It is a tipuser contract where:
	- [x] tip user based on requester's request from accounts balance in contract account. 

## About
* contract name - `tippertipper`
* contract's account name - `tippertipper`
* action
	- `deposit`
* table
	- `accounts`

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
  - show the tippertipper accounts balance of `tipuser11111`
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
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
        "balance": "94.9899 EOS"
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
$ cleost push action eosio.token transfer '["tipuser11111", "tippertipper", "10.0000 EOS", "145624324"]' -p tipuser11111@active
executed transaction: 097f9a8498af29d0ca87952cac2ffa78b295ab480aa9a95aec3614a8162c8176  136 bytes  164 us
#   eosio.token <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"145624324"}
#  tipuser11111 <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"145624324"}
#  tippertipper <= eosio.token::transfer        {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 EOS","memo":"145624324"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - show the tippertipper accounts balance of `tipuser11111`
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
{
  "rows": [{
      "data": {
        "owner": 145624324,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 100000
          }
        ]
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
        "balance": "84.9899 EOS"
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
  - show the tippertipper accounts balance of `tipuser11111` by telegram_id
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
{
  "rows": [{
      "data": {
        "owner": 145624324,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 100000
          }
        ]
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
        "balance": "99.8000 FUTBOL"
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
$ cleost push action tokenfutbol1 transfer '["tipuser11111", "tippertipper", "10.0000 FUTBOL", "145624324"]' -p tipuser11111@active
executed transaction: 11212f037dff1c2f4af5c02b1e4d2e88ed6171dea62ff5f62711ab840ea5c3b4  136 bytes  208 us
#  tokenfutbol1 <= tokenfutbol1::transfer       {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 FUTBOL","memo":"145624324"}
#  tipuser11111 <= tokenfutbol1::transfer       {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 FUTBOL","memo":"145624324"}
#  tippertipper <= tokenfutbol1::transfer       {"from":"tipuser11111","to":"tippertipper","quantity":"10.0000 FUTBOL","memo":"145624324"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
  - show the tippertipper accounts balance of `tipuser11111` by telegram_id
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
{
  "rows": [{
      "data": {
        "owner": 145624324,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 100000
          },{
            "key": {
              "sym": "4,FUTBOL",
              "contract": "tokenfutbol1"
            },
            "value": 100000
          }
        ]
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
        "balance": "89.8000 FUTBOL"
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
* `tipuser11111` tips "1 EOS" to `tipuser11112` by just using telegram_id
  - show the tippertipper accounts balance of `tipuser11111` by telegram_id
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
{
  "rows": [{
      "data": {
        "owner": 145624324,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 100000
          },{
            "key": {
              "sym": "4,FUTBOL",
              "contract": "tokenfutbol1"
            },
            "value": 100000
          }
        ]
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
$ cleost push action tippertipper tip '["145624324", "768743431", "ali67", "ali68", "1.0000 EOS", "tip for enjoy"]' -p tippertipper@active
executed transaction: e76752df9d9f1d11028d67a3a0bb43cc8025f3b7ae0836a510151fdf1385b50e  152 bytes  178 us
#  tippertipper <= tippertipper::tip            {"from_id":145624324,"to_id":768743431,"from_username":"ali67","to_username":"ali68","quantity":"1.0...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
``` 
  - show the tippertipper accounts balance of `tipuser11111` by telegram_id
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
{
  "rows": [{
      "data": {
        "owner": 145624324,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 90000
          },{
            "key": {
              "sym": "4,FUTBOL",
              "contract": "tokenfutbol1"
            },
            "value": 100000
          }
        ]
      },
      "payer": "tippertipper"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
  - show the tippertipper accounts balance of `tipuser11112` by telegram_id
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 768743431 --upper 768743431
{
  "rows": [{
      "data": {
        "owner": 768743431,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 10000
          }
        ]
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
  - show the tippertipper accounts balance of `tipuser11111`
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
{
  "rows": [{
      "data": {
        "owner": 145624324,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 90000
          },{
            "key": {
              "sym": "4,FUTBOL",
              "contract": "tokenfutbol1"
            },
            "value": 100000
          }
        ]
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
$ cleost push action tippertipper tip '["145624324", "768743431", "ali67", "ali68", "1.0000 FUTBOL", "tip for enjoy"]' -p tippertipper@active
executed transaction: 513f60ed461420058edc235dd8c6abeb72a3326cdd1df94b4b671f7877e82f79  152 bytes  139 us
#  tippertipper <= tippertipper::tip            {"from_id":145624324,"to_id":768743431,"from_username":"ali67","to_username":"ali68","quantity":"1.0...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
``` 
  - show the tippertipper accounts balance of `tipuser11111` by telegram_id
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 145624324 --upper 145624324
{
  "rows": [{
      "data": {
        "owner": 145624324,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 90000
          },{
            "key": {
              "sym": "4,FUTBOL",
              "contract": "tokenfutbol1"
            },
            "value": 90000
          }
        ]
      },
      "payer": "tippertipper"
    }
  ],
  "more": false,
  "next_key": "",
  "next_key_bytes": ""
}
```
  - show the tippertipper accounts balance of `tipuser11112` by telegram_id
```console
$ cleost get table tippertipper tippertipper accounts --show-payer --lower 768743431 --upper 768743431
{
  "rows": [{
      "data": {
        "owner": 768743431,
        "balances": [{
            "key": {
              "sym": "4,EOS",
              "contract": "eosio.token"
            },
            "value": 10000
          },{
            "key": {
              "sym": "4,FUTBOL",
              "contract": "tippertipper"
            },
            "value": 10000
          }
        ]
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
* del accounts table
```console
$ cleost push action tippertipper testdlacbyid '["145624324"]' -p tippertipper@active
executed transaction: 90ac9c2b382f2eb744ae040808497ba64412b7ca880ca19c34f12fe14903dc11  104 bytes  224 us
#  tippertipper <= tippertipper::testdlacbyid   {"tg_id":145624324}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* transfer EOS token back
```console
$ cleost push action eosio.token transfer '["tippertipper", "tipuser11111", "9.0000 EOS", "return accounts"]' -p tippertipper@active
executed transaction: 63830728e6cf5f5557e255bd9f4dd106cc1aa52fbc4f686cb8038befddcd5a40  144 bytes  143 us
#   eosio.token <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11111","quantity":"9.0000 EOS","memo":"return accounts"}
#  tippertipper <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11111","quantity":"9.0000 EOS","memo":"return accounts"}
>> Either money is not sent to the contract or contract itself is the buyer.
#  tipuser11111 <= eosio.token::transfer        {"from":"tippertipper","to":"tipuser11111","quantity":"9.0000 EOS","memo":"return accounts"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* transfer FUTBOL token back
```console
$ cleost push action tokenfutbol1 transfer '["tippertipper", "tipuser11111", "9.9000 FUTBOL", "return accounts"]' -p tippertipper@active
executed transaction: 89a821e0193e65fee6bd6e03c845292a877edd3358e9156898dd29af9a32c615  136 bytes  145 us
#  tokenfutbol1 <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11111","quantity":"9.9000 FUTBOL","memo":"return accounts"}
#  tippertipper <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11111","quantity":"9.9000 FUTBOL","memo":"return accounts"}
>> Either money is not sent to the contract or contract itself is the buyer.
#  tipuser11111 <= tokenfutbol1::transfer       {"from":"tippertipper","to":"tipuser11111","quantity":"9.9000 FUTBOL","memo":"return accounts"}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
* test by add/sub - 1/0 token to existing tg_id
```console
$ cleost push action tippertipper testmdqtyusr '["145624324", "2.0000 EOS", "1"]' -p tippert
ipper@active
executed transaction: 45f6fe0feb3b2b364fc4a3202267aad17429a67519e348ee55170832ed0e730e  120 bytes  253 us
#  tippertipper <= tippertipper::testmdqtyusr   {"from_id":145624324,"quantity":"2.0000 EOS","arithmetic_op":1}
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```


## TODO

## Troubleshooting
