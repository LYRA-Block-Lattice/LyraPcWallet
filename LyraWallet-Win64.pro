QT       += core gui network charts websockets
TARGET = ../../LyraWallet-Win64/lyra-pc-wallet


#win32{
#    LIBS += -LD:\Qt\Tools\OpenSSL\Win_x64\lib -llibcrypto
#    LIBS += -LD:\Qt\Tools\OpenSSL\Win_x64\lib -llibssl
#    LIBS += -L:/resource/lib -llibssl_static
#    INCLUDEPATH += D:\Qt\Tools\OpenSSL\Win_x64\lib
#}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    api/nebula.cpp \
    crypto/aes.cpp \
    crypto/base58Encoding.cpp \
    crypto/ecc.cpp \
    crypto/signatures.cpp \
    extensions/qrcodegen.cpp \
    keyrevealer.cpp \
    language/translate.cpp \
    main.cpp \
    mainwindow.cpp \
    network/rpc.cpp \
    network/rpcapi.cpp \
    network/tokenpairing.cpp \
    network/websocket/sslwebsocket.cpp \
    network/websocket/websocket.cpp \
    pages/pc/login/login.cpp \
    pages/pc/main/dashboard/dashboardpage.cpp \
    pages/pc/main/dashboard/dashmain.cpp \
    pages/pc/main/info.cpp \
    pages/pc/main/mainboard.cpp \
    pages/pc/main/search/searchwindow.cpp \
    pages/pc/main/settings/createprofitaccount.cpp \
    pages/pc/main/settings/settingsaddaccount.cpp \
    pages/pc/main/settings/settingsdeleteaccount.cpp \
    pages/pc/main/settings/settingseditaccount.cpp \
    pages/pc/main/settings/settingsedituser.cpp \
    pages/pc/main/settings/settingsshowprivkey.cpp \
    pages/pc/main/settings/settingswindow.cpp \
    pages/pc/main/transitions/transitionswindow.cpp \
    pages/pc/main/wallet/stake.cpp \
    pages/pc/main/wallet/walletmain.cpp \
    pages/pc/main/wallet/walletpage.cpp \
    pages/pc/main/wallet/walletreceive.cpp \
    pages/pc/main/wallet/walletsend.cpp \
    pages/pc/main/wallet/walletshowdetails.cpp \
    pages/pc/populate.cpp \
    pages/pc/textformating.cpp \
    storage/walletfile.cpp \
    wallet/check.cpp \
    wallet/events.cpp \
    wallet/rpc/pool.cpp \
    wallet/rpc/profiting.cpp \
    wallet/rpc/sign.cpp \
    wallet/rpc/swap.cpp \
    wallet/rpc/wallet.cpp \
    wallet/rpc/walletbalance.cpp \
    wallet/rpc/wallethistory.cpp \
    wallet/tickedupdates.cpp

HEADERS += \
    api/nebula.h \
    config.h \
    configlyra.h \
    crypto/aes.h \
    crypto/base58Encoding.h \
    crypto/ecc.h \
    crypto/signatures.h \
    extensions/qrcodegen.h \
    keyrevealer.h \
    language/translate.h \
    mainwindow.h \
    network/rpc.h \
    network/rpcapi.h \
    network/tokenpairing.h \
    network/websocket/sslwebsocket.h \
    network/websocket/websocket.h \
    pages/pc/login/login.h \
    pages/pc/main/dashboard/dashboardpage.h \
    pages/pc/main/dashboard/dashmain.h \
    pages/pc/main/def.h \
    pages/pc/main/info.h \
    pages/pc/main/mainboard.h \
    pages/pc/main/search/searchwindow.h \
    pages/pc/main/settings/createprofitaccount.h \
    pages/pc/main/settings/settingsaddaccount.h \
    pages/pc/main/settings/settingsdeleteaccount.h \
    pages/pc/main/settings/settingseditaccount.h \
    pages/pc/main/settings/settingsedituser.h \
    pages/pc/main/settings/settingsshowprivkey.h \
    pages/pc/main/settings/settingswindow.h \
    pages/pc/main/transitions/transitionswindow.h \
    pages/pc/main/wallet/stake.h \
    pages/pc/main/wallet/walletmain.h \
    pages/pc/main/wallet/walletpage.h \
    pages/pc/main/wallet/walletreceive.h \
    pages/pc/main/wallet/walletsend.h \
    pages/pc/main/wallet/walletshowdetails.h \
    pages/pc/populate.h \
    pages/pc/textformating.h \
    storage/walletfile.h \
    wallet/check.h \
    wallet/events.h \
    wallet/rpc/pool.h \
    wallet/rpc/profiting.h \
    wallet/rpc/sign.h \
    wallet/rpc/swap.h \
    wallet/rpc/wallet.h \
    wallet/rpc/walletbalance.h \
    wallet/rpc/wallethistory.h \
    wallet/tickedupdates.h

FORMS += \
    keyrevealer.ui \
    mainwindow.ui \
    pages/pc/main/info.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    lyra-gui-wallet.qrc

RC_ICONS = resource/ico/256color.ico

DISTFILES +=

