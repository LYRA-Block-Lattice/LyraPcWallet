## LYRA PC official wallet

This wallet was developed in close collaboration with:

 **Joseph Fowler**: LYRA Block lattice cofounder https://lyra.live.
 **@Zabusher**: Marketing expert and artist.

 This application is developed to manage individual accounts under a single encrypted wallet.

 The wallet file is encrypted with aes256 encryption method using a sha256 of the wallet password assuring a very strong wallet protection.

 ### Description:

The wallet is composed of different pages:

* Dashboard.

* My Wallet:
  * Send.
  * Received.
* SWAP ( in development ).
* Transitions.
* Settings.

#### Dashboard:
Dashboard page is composed of two sections:
* General info like total supply, locked tokens, circulating supply and total block count.
* The second section display a graph of the selected account value at current conversion rate, recent transactions in the selected account and a table of assets in the selected account.

#### My Wallet:
Here in the header will display:
* The account balance in LYR.
* If there is unreceived balance with a synchronize* button.
* The date when the account was created.
* The number of completed transactions.

*In the middle:*
* Will display the account ID, if the account ID is clicked the account is copied to clipboard.
* STAKE button will open a new window where you can create new staking accounts and stake/unstake tokens, will allow to stake portions of account balance to multiple profiting accounts or stake entire balance to one profiting account.

In the footer will display:
* A graph with the account balance in LYR.
* A graph with the account balance in chosen value ( BTC or USD ).

#### My Wallet - Send:
Here in the header will display:
* The account balance in LYR.
* If there is unreceived balance with a synchronize* button.
* Total lyra send last week.
* Last send transaction value and date.

*In the middle*:
* Will display the account ID, if the account ID is clicked the account is copied to clipboard.
* Destination account where the user will type or paste the destination account ID.
* A dropdown box where user can choose the token that want to send if have multiple tokens in the account.
* The amount of selected tokens want to send.
* A conversion rate from selected token to BTC or USD.
* Wallet password for confirmation.
* A note that is stored in the wallet under selected account, the note is not send to the node.

*In the footer:*
Is a table that display the recent send transactions.

#### My Wallet - Receive:
Here in the header will display:
* The account balance in LYR.
* If there is unreceived balance with a synchronize* button.
* Total lyra send last week.
* Last received transaction value and date.

*In the middle*:
* Will display the account ID, if the account ID is clicked the account is copied to clipboard.
* The address from where you request funds.
* Requested token.
* Amount of funds requested.
* A conversion from requested value of selected token to BTC or USD.
* A QR generator for to easily receive funds.

*In the footer:*
Is a table that display the recent received transactions.

#### Transitions:
Will display all transactions occurred in the selected account along with a filter find a specific transaction more easily.

#### Settings:
* A Language dropdown box where the user can choose the desired wallet language.
* A style dropdown where user can select the wallet appearance ( currently only light mode is available ).
* A network selection dropdown where the user can select the network that want to operate ( testnet/mainnet).
* Default alternative value dropdown is the default conversion that is used by the wallet to display account value (BTC/USD).
* Window scale dropdown where user can chose the wallet window magnification for better visualisation on different display resolutions.

*User manager settings:*
* User can backup the wallet.
* User can edit the username/password of the wallet.
  * There is a unified window in order to change the wallet name and the wallet password; if the user enter only the new username and the current wallet password, the wallet will be renamed, if the user will enter only the current passwords and a new password the wallet will change the password of the wallet, if user enter both username and a new password along with the current password the wallet name and password will be changed.

*Wallet settings:*
Here is a list of accounts in the current wallet where user can create profiting accounts/add/edit/delete selected accounts.
* PROFITING button will open a new window where you can create profiting accounts specific functions ( Node/Oracle/Merchant/Yield ).
Clicking on an account address will copy it to the clipboard if you need it.

#### Any change in the wallet/accounts will result in an immediately wallet backup.
#### Any send/synchronize or other changes in the wallet will trigger a wallet backup, in these cases there is a service function that check for those changes once every five minutes and backup it if there are any changes.
#### If there are pending backups and the wallet application is closed the wallet will be saved before the application is closed.

# Instalation:

### Windows:

Both versions of Windows are supported 32bit and 64bit, Microsoft decided to begin to broke network libraries for Windows 64bit, if you see that the wallet is offline please use the 32bit Wallet.

Decompress the archive and double click on the wallet executable.

If someone still use Windows 7 x64 or is reported that MSVCR100.dll is missing, download and install Microsoft Visual C++ 2010 Redistributable from Microsoft site.

### Linux:

Ubuntu 16 and UP and derivates 64bit versions only.

Decompress the archive and  double click on "LyraWalletInstall.sh" file, or in Ubuntu case open a terminal and navigate to the wallet directory and type: ./LyraWalletInstall.sh, will install all dependencies, on Linux Mint all dependencies are satisfied out of the box by the OS, but on other platforms the "libxcb" and "libxkb" libs are not present, so this file will install them.

The same file will copy the wallet files in the ~/LiraWallet directory and install a shortcut on the desktop.

On some OS'es when you first double click the launcher will popup a message "Untrusted application launcher", click to "Trust and Launch".

### Mac OS X:

Minimum supported version is 10.12.

Decompress the archive and double click on the APP file.

### On all platforms:

The wallet will take some time to appear on the desktop ~5 seconds, this is due to components initializations.
