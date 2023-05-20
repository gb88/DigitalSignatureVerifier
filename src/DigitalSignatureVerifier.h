#ifndef DigitalSignatureVerifier_h
#define DigitalSignatureVerifier_h

#include "Arduino.h"
#include "stdlib_noniso.h"

#if defined(ESP32)
	#define HARDWARE "ESP32"
	#include "mbedtls/pk.h"
	#include "mbedtls/sha256.h"
#elif defined(ESP8266)
    #define HARDWARE "ESP8266"	
	#include "bearssl/bearssl_hash.h"
	#include <BearSSLHelpers.h>
#endif


#if defined(ESP32)
class UpdaterHashClass {
  public:
    virtual void begin();
    virtual void add(const void *data, uint32_t len);
    virtual void end();
    virtual int len() ;
    virtual const void *hash();
    virtual const unsigned char *oid();
	virtual unsigned char type();
};

// Updater SHA256 hash and signature verification
class HashSHA256 : public UpdaterHashClass {
  public:
	HashSHA256();
    void begin();
    void add(const void *data, uint32_t len);
    void end();
    int len();
    const void *hash();
    const unsigned char *oid();
	unsigned char type();
  private:
    mbedtls_sha256_context _sha256_ctx;
    unsigned char _sha256[32];
};
#endif


class DigitalSignatureVerifier {
public:
	virtual bool setKey(const char * key, size_t len);
	virtual size_t getSigLen(void);
    virtual bool verify(UpdaterHashClass * hash, const unsigned char * signature, size_t sig_len);
	unsigned char * signature;
protected:
	const char * _key;
	size_t _len;
};


class RSADigitalSignatureVerifier : public DigitalSignatureVerifier {
public:
    RSADigitalSignatureVerifier();
	bool setKey(const char * key, size_t len);
	size_t getSigLen(void);
    bool verify(UpdaterHashClass * hash, const unsigned char * signature, size_t sig_len);
private:
#if defined(ESP32)
	mbedtls_pk_context _pk_context;
#elif defined(ESP8266)
	BearSSL::PublicKey * _PubKey;
	BearSSL::SigningVerifier * _Verifier;
#endif
};

#endif