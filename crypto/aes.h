#ifndef AES_H
#define AES_H

#include <QByteArray>
#include <QString>

typedef struct {
    uint8_t key[32];
    uint8_t enckey[32];
    uint8_t deckey[32];
} aes256_context;

void aes256_init(aes256_context *, uint8_t * /* key */);
void aes256_done(aes256_context *);
void aes256_encrypt_ecb(aes256_context *, uint8_t * /* plaintext */);
void aes256_decrypt_ecb(aes256_context *, uint8_t * /* cipertext */);

class aes
{
public:
    static QByteArray aesEncrypt(QByteArray data, QString password);
    static QByteArray aesDecrypt(QByteArray data, QString password);
private:

};

#endif // AES_H
