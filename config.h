#ifndef __DEF_H__
#define __DEF_H__

#include "qglobal.h"

#define APP_VERSION                                 "0.1.3"

#define TITLEBAR_HEIGHT                             30
#define WINDOW_WIDTH                                1350
#define WINDOW_HEIGHT                               960
#define DEFAULT_SCALE                               1.0
#define DEFAULT_THEME                               "light"
#ifdef Q_OS_OSX
#define DEFAULT_FONT_SIZE                           17
#elif defined(Q_OS_WIN32)
#define DEFAULT_FONT_SIZE                           14
#else
#define DEFAULT_FONT_SIZE                           13
#endif
#define DEFAULT_LANGUAGE                            "en"
#define DEFAULT_WALLET_EXTENSION                    "lyr"

#define LYRA_TOKEN_PAGE_IMPLEMENTED                 0
#define SWAP_PAGE_IMPLEMENTED                       0
#define TRANSITIONS_PAGE_IMPLEMENTED                1
#define SEARCH_PAGE_IMPLEMENTED                     0

#define HEADER_HEIGHT                               112
#define LEFT_MENU_WIDTH                             228

#define USER_HOME                                   QDir::homePath()
#define WALLET_PATH                                 ".lyra"

#define DEFAULT_NETWORK                             NETWORK_TESTNET
#define DEFAULT_NODE_LIST                           RPC_TESTNET_IP_LIST
/* RPC nodes DNS/IP */
/* Those nodes are selected randomly thru elimination from the full list, if all nodes are tried the list is refreshed and the cicle continue */
#define RPC_TESTNET_IP_LIST                         {{"wss://81.196.64.78:4504/api/v1/socket", true}, \
                                                    {"wss://seed.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed2.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.testnet.lyra.live:443/api/v1/socket", true}}

#define RPC_MAINNET_IP_LIST                         {{"wss://161.97.166.188:5504/api/v1/socket", true}, /*Iulian Gheorghiu node*/ \
                                                    {"wss://173.212.228.110:5504/api/v1/socket", true}, /*Iulian Gheorghiu node*/ \
                                                    {"wss://seed1.mainnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed2.mainnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.mainnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.mainnet.lyra.live:443/api/v1/socket", true}}
#define RPC_CONNECT_TIMEOUT                         500
#define RPC_MESSAGE_RESPONSE_TIMEOUT                5000

#define EXCHANGES_PRICE_BTC_PAIR_LIST               {{"finexbox", "https://xapi.finexbox.com/v1/ticker?market=lyr_btc"}}
#define EXCHANGES_PRICE_USD_PAIR_LIST               {{"latoken", "https://api.latoken.com/v2/ticker"}}

/* ICO location definition */
#define ICO_WINDOW_SMALL                            ":/resource/ico/256color.ico"

/* FILE */
#define WALLET_FILE_NAME                            "pig1"

#endif // DEF_H
