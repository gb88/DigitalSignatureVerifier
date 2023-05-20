/*
  DigitalSignatureVerification Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: gb88 (https://github.com/gb88)

  Alice generate the private key:
    - openssl genrsa -out priv_key.pem 2048

  and the corresponding public key:
    - openssl rsa -in priv_key_2048.pem -pubout > rsa_key_2048.pub

  sign the SHA256 hash with the private key:
    - openssl dgst -sign priv_key_2048.pem -keyform PEM -sha256 -out textSigned.sign -binary Alice.txt

  create the array with the signature:
    - xxd -i textSigned.sign > textSigned.h

  The code will output:
  The text: Hello Bob!
  was signed by Alice

*/

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <DigitalSignatureVerifier.h>
#include "textSigned.h"

HashSHA256 Hash256;
RSADigitalSignatureVerifier RSAVerifier;

#if defined(ESP32)
const char pub_key[] = "-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAso/F7lALGgdK8FhvLzM+\n3+L+o7zvpMCjJubWtBO44106fISLa8FE9kRsgkFxD2Av0oFqJC+Ts8ryXF+z9SKY\nj+3+HUv1dmMtvkI7aVeH6ahytXOazhs2DXaYDSXBreaud0/q+r+NBQAzen0IGxDY\n6+Z5XiGDiIUgPTj4Bnrlush+2XcK9kt+LPOeUbxmETj3KsR4FTW6eWWEyeouLoWt\nBt5jh6VcD5VqFsNOpN4DPN7ocjnPAgvX3a9quEekx2Ex4/E7ITz/N7xntU3fKhcm\noNbGvgaNZZCDDZieaQJLZ+kviGZ2IhMDAqEm8tMUPbTiCDSC31awMXBDB/cEebt8\nxQIDAQAB\n-----END PUBLIC KEY-----";
#else
const char pub_key[] PROGMEM = R"EOF(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAso/F7lALGgdK8FhvLzM+
3+L+o7zvpMCjJubWtBO44106fISLa8FE9kRsgkFxD2Av0oFqJC+Ts8ryXF+z9SKY
j+3+HUv1dmMtvkI7aVeH6ahytXOazhs2DXaYDSXBreaud0/q+r+NBQAzen0IGxDY
6+Z5XiGDiIUgPTj4Bnrlush+2XcK9kt+LPOeUbxmETj3KsR4FTW6eWWEyeouLoWt
Bt5jh6VcD5VqFsNOpN4DPN7ocjnPAgvX3a9quEekx2Ex4/E7ITz/N7xntU3fKhcm
oNbGvgaNZZCDDZieaQJLZ+kviGZ2IhMDAqEm8tMUPbTiCDSC31awMXBDB/cEebt8
xQIDAQAB
-----END PUBLIC KEY-----
)EOF";
#endif

const char AliceTxt[] = "Hello Bob!";

void setup(void) {
  Serial.begin(115200);

  // Set the public key for signature verification
  #if defined(ESP32)
  RSAVerifier.setKey(pub_key, strlen(pub_key));
  #elif defined(ESP8266)
  RSAVerifier.setKey(pub_key, strlen_P(pub_key));
  #endif

  Hash256.begin();
  Hash256.add(AliceTxt, strlen(AliceTxt));
  Hash256.end();

  Serial.print("The text: ");
  Serial.println(AliceTxt);

  // Verify the signature
  if (RSAVerifier.verify(&Hash256, textSigned_sign, RSAVerifier.getSigLen())) {
    Serial.println("was NOT signed by Alice");
  } else {
    Serial.println("was signed by Alice");
  }
}

void loop(void) {
  // Empty loop
}
