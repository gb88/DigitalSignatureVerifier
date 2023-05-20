# DigitalSignatureVerifier
Library for verifying digital signatures using RSA-2048 and SHA-256 on ESP32 and ESP8266

# How to install
- Download the Library as ZIP files
- Launch the Arduino IDE on your computer.
- Import the Library: Go to "Sketch" in the menu bar and navigate to "Include Library" > "Add .ZIP Library...". This will open a file dialog.
- Locate the Library ZIP File: In the file dialog, navigate to the location where you downloaded the library ZIP file and select it. Click the "Open" button to import the library.

# Examples
The example signatureVerification demonstrates the process of verifying a digital signature using a public key and the SHA256 hash algorithm. It provides instructions for key generation, signing a file, and creating the necessary signature array for inclusion in an Arduino sketch.
By following these instructions, Bob will be able to verify if a message is indeed coming from Alice by using her public key. This ensures the authenticity and integrity of the message, as only Alice's private key can produce a valid signature that can be verified using her public key.\
Alice generate the private key:
- openssl genrsa -out priv_key.pem 2048
and the corresponding public key:
- openssl rsa -in priv_key_2048.pem -pubout > rsa_key_2048.pub
sign the SHA256 hash with the private key:
- openssl dgst -sign priv_key_2048.pem -keyform PEM -sha256 -out textSigned.sign -binary Alice.txt
create the array with the signature:
- xxd -i textSigned.sign > textSigned.h
