#include "DigitalSignatureVerifier.h"

#if defined(ESP32)
HashSHA256::HashSHA256()
{
}
void HashSHA256::begin()
{
	mbedtls_sha256_free( &_sha256_ctx );
	mbedtls_sha256_init( &_sha256_ctx );
	mbedtls_sha256_starts( &_sha256_ctx, 0 );
}

void HashSHA256::add(const void *data, uint32_t len)
{
	mbedtls_sha256_update( &_sha256_ctx, (const unsigned char *)data, len );
}

void HashSHA256::end()
{
	mbedtls_sha256_finish( &_sha256_ctx, _sha256 );
}

int HashSHA256::len()
{
	return sizeof(_sha256);
}

const void * HashSHA256::hash()
{
	return (const void *) _sha256;
}

const unsigned char * HashSHA256::oid()
{
	return NULL;
}

unsigned char HashSHA256::type()
{
	return MBEDTLS_MD_SHA256;
}
#endif

RSADigitalSignatureVerifier::RSADigitalSignatureVerifier()
{
	signature = NULL;
#if defined(ESP8266)
	_PubKey = NULL;
	_Verifier = NULL;
#endif
}
bool RSADigitalSignatureVerifier::setKey(const char * key, size_t len)
{
	_key = key;
	_len = len;
	if(len < 128)
		return true;
	if(signature != NULL)
		free(signature);
	signature = (unsigned char*)malloc(_len);
#if defined(ESP32)
	mbedtls_pk_free(&_pk_context);
	mbedtls_pk_init(&_pk_context);
	if (mbedtls_pk_parse_public_key( &_pk_context, (const unsigned char*)key, len + 1) != 0 )
	{
		return true;
	}
	return false;
#elif defined(ESP8266)
	if(_PubKey!=NULL)
		free(_PubKey);
	if(_Verifier!=NULL)
		free(_Verifier);
	_PubKey = new BearSSL::PublicKey((const char*)key);
	_Verifier = new BearSSL::SigningVerifier(_PubKey);
	return false;
#endif
}

size_t RSADigitalSignatureVerifier::getSigLen(void)
{
#if defined(ESP32)
	return mbedtls_pk_get_len(&_pk_context);
#elif defined(ESP8266)
	return _Verifier->length();
#endif
}


bool RSADigitalSignatureVerifier::verify(UpdaterHashClass * hash, const unsigned char * signature, size_t sig_len)
{
#if defined(ESP32)
	if(mbedtls_pk_verify( &_pk_context, (mbedtls_md_type_t)hash->type(), (const unsigned char*)hash->hash(), hash->len(), signature, sig_len))
	{
		Serial.println("Signature Error");
		return true;
	}
	return false;
#elif defined(ESP8266)	
	if (!_Verifier->verify(hash, signature, sig_len)) 
		return true;
	return false;
#endif
}

 