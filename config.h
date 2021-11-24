#ifndef __DEF_H__
#define __DEF_H__

#include "qglobal.h"

#define APP_VERSION                                 "0.2.9"

#define NETWORKS                                    {"testnet", "mainnet"/*, "dev"*/}

#ifdef Q_OS_OSX
#define USE_WINDOWS_SCALING                         true
#elif defined(Q_OS_WIN32)
#define USE_WINDOWS_SCALING                         true
#else
#define USE_WINDOWS_SCALING                         true
#endif

#define TITLEBAR_HEIGHT                             0
#define WINDOW_WIDTH                                1350
#define WINDOW_HEIGHT                               960
#ifdef Q_OS_OSX
#define DEFAULT_SCALE                               0.8
#elif defined(Q_OS_WIN32)
#define DEFAULT_SCALE                               0.8
#else
#define DEFAULT_SCALE                               1.0
#endif
#define SCALE_LIST                                  {"0.5x", "0.6x", "0.7x", "0.8x", "0.9x", "1.0x", "1.1x", "1.2x", "1.3x", "1.4x", "1.5x", "1.6x", "1.7x", "1.8x", "1.9x"}
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

#define DEFAULT_NETWORK                             NETWORK_TESTNET

#define LYRA_TOKEN_PAGE_IMPLEMENTED                 0
#define SWAP_PAGE_IMPLEMENTED                       0
#define TRANSITIONS_PAGE_IMPLEMENTED                1
#define SEARCH_PAGE_IMPLEMENTED                     1

#define HEADER_HEIGHT                               112
#define LEFT_MENU_WIDTH                             228

#define USER_HOME                                   QDir::homePath()
#define WALLET_PATH                                 ".lyra"

#if (DEFAULT_NETWORK == NETWORK_MAINNET)
#define DEFAULT_NODE_LIST                           RPC_TESTNET_IP_LIST
#else
#define DEFAULT_NODE_LIST                           RPC_MAINNET_IP_LIST
#endif
/* RPC nodes DNS/IP */
/* Those nodes are selected randomly thru elimination from the full list, if all nodes are tried the list is refreshed and the cicle continue */
#define RPC_TESTNET_IP_LIST                         {{"wss://seed.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed2.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.testnet.lyra.live:443/api/v1/socket", true}}

#define RPC_MAINNET_IP_LIST                         {{"wss://161.97.166.188:5504/api/v1/socket", true}, /*Iulian Gheorghiu node*/ \
                                                    {"wss://173.212.228.110:5504/api/v1/socket", true}, /*Iulian Gheorghiu node*/ \
                                                    {"wss://seed1.mainnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed2.mainnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.mainnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.mainnet.lyra.live:443/api/v1/socket", true}}

#define RPC_DEV_IP_LIST                             {{"wss://81.196.64.78:4504/api/v1/socket", true},  /*Iulian Gheorghiu node*/\
                                                    {"wss://seed.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed2.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.testnet.lyra.live:443/api/v1/socket", true}, \
                                                    {"wss://seed3.testnet.lyra.live:443/api/v1/socket", true}}

#define RPC_CONNECT_TIMEOUT                         1000 // mS
#define RPC_MESSAGE_RESPONSE_TIMEOUT                10000 // mS

#define EXCHANGES_PRICE_BTC_PAIR_LIST               {{"finexbox", "https://xapi.finexbox.com/v1/ticker?market=lyr_btc"}}
#define EXCHANGES_PRICE_USD_PAIR_LIST               {{"latoken", "https://api.latoken.com/v2/ticker"}}

/* ICO location definition */
#define ICO_WINDOW_SMALL                            ":/resource/ico/256color.ico"

#ifdef Q_OS_OSX
#define FONT_REDUCTION  0.7
#else
#define FONT_REDUCTION  1
#endif




#endif // DEF_H
