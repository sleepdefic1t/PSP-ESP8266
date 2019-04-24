
#include <AUnit.h>
#include <vector>

#include "psp_crypto.h"

namespace {

// PrivateKeyTestBytes
// PrivateKey Hex: 'd8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712'
// 32 bytes
std::vector<uint8_t> PrivateKeyTestBytes = {
    216, 131, 156,  36,  50, 191, 208, 166,
    126, 241,  10, 128,  75, 169, 145, 234,
    187, 161, 159,  21,  74,  61, 112, 121,
     23, 104,  29,  69, 130,  42,  87,  18
};

// PublicKeyTestBytes
// PublicKey Hex: '034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192'
// uncompressed: '044151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192fa29ce00fb8ed92f34f68c0018f12d5b68c1609080abaf331746d6d86c6039e5'
// 33 bytes
std::vector<uint8_t> PublicKeyTestBytes = {
    3,
    65,  81, 163, 236,  70, 181, 103,  10,
    104,  43,  10,  99,  57,  79, 134, 53,
    135, 209, 188, 151,  72,  59,  27, 108,
    112, 235,  88, 231, 240, 174, 209, 146
};

// MessageTestBytes
// Test Message: "Hello World"
// 11 bytes
std::vector<uint8_t> MessageTestBytes = {
    72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100
};

// MessageHashTestBytes
// SHA256 of "Hello World": a591a6d40bf420404a011733cfb7b190d62c65bf0bcda32b57b277d9ad9f146e
// 32 bytes
std::vector<uint8_t> MessageHashTestBytes = {
    165, 145, 166, 212,  11, 244,  32,  64,
     74,   1,  23,  51, 207, 183, 177, 144,
    214,  44, 101, 191,  11, 205, 163,  43,
     87, 178, 119, 217, 173, 159,  20, 110
};

// Expected DER Signature of 'PrivateKeyTestBytes' and 'MessageHashTestBytes':
// 304402200fb4adddd1f1d652b544ea6ab62828a0a65b712ed447e2538db0caebfa68929e02205ecb2e1c63b29879c2ecf1255db506d671c8b3fa6017f67cfd1bf07e6edd1cc8
//
// RValueTestBytes
// Expected unencoded R-Value: 0fb4adddd1f1d652b544ea6ab62828a0a65b712ed447e2538db0caebfa68929e
// 32 bytes
std::vector<uint8_t> RValueTestBytes = {
     15, 180, 173, 221, 209, 241, 214,  82,
    181,  68, 234, 106, 182,  40,  40, 160,
    166,  91, 113,  46, 212,  71, 226,  83,
    141, 176, 202, 235, 250, 104, 146, 158
};
//
// SValueTestBytes
// Expected unencoded S-Value: 0fb4adddd1f1d652b544ea6ab62828a0a65b712ed447e2538db0caebfa68929e
// 32 bytes
std::vector<uint8_t> SValueTestBytes = {
     94, 203,  46,  28,  99, 178, 152, 121,
    194, 236, 241,  37,  93, 181,   6, 214,
    113, 200, 179, 250,  96,  23, 246, 124,
    253,  27, 240, 126, 110, 221,  28, 200
};

};   // namespace

test(crypto_crypto_is_valid_publickey) {
    bool publicKeyIsValid = Ark::Platform::Crypto::IsValidPublicKey(&PublicKeyTestBytes[0]);
    assertTrue(publicKeyIsValid);
}

test(crypto_crypto_privatekey_to_publickey) {
    std::vector<uint8_t> pubkeyBuffer(33);
    Ark::Platform::Crypto::PrivateKeyToPublicKey(&PrivateKeyTestBytes[0], &pubkeyBuffer[0]);

    bool publicKeysMatch = PublicKeyTestBytes == pubkeyBuffer;
    assertTrue(publicKeysMatch);
}

test(crypto_crypto_sign) {
    std::vector<uint8_t> r(32), s(32);
    Ark::Platform::Crypto::Sign(
            &MessageHashTestBytes[0],
            &PrivateKeyTestBytes[0],
            &r[0], &s[0]);

    bool rValuesMatch = RValueTestBytes == r;
    bool sValuesMatch = SValueTestBytes == s;
    assertTrue(rValuesMatch);
    assertTrue(sValuesMatch);
}

test(crypto_crypto_verify) {
    bool isVerifiable = Ark::Platform::Crypto::Verify(
            &PublicKeyTestBytes[0],
            &MessageHashTestBytes[0],
            &RValueTestBytes[0],
            &SValueTestBytes[0]);
    assertTrue(isVerifiable);
}

test(crypto_encoding_base58_encode) {

    // Base58ToBeEncodedTestBytes
    // 21 bytes
    std::vector<uint8_t> Base58ToBeEncodedTestBytes = {
        30, 9, 149, 117, 2, 7, 236, 175, 12, 207, 37, 28, 18, 101, 185, 42, 216, 79, 85, 54, 98
    };

    // Base58EncodedTestStr
    // 34 characters
    const char *Base58EncodedTestStr = "D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib";

    // encodedStrBuffer
    // 34 bytes + 1-byte (null terminator)
    char encodedStrBuffer[34 + 1];

    Ark::Platform::Encoding::Base58::encode(&Base58ToBeEncodedTestBytes[0], encodedStrBuffer);

    bool encodedStringsMatch = (strcmp(Base58EncodedTestStr, &encodedStrBuffer[0]) == 0);
    assertTrue(encodedStringsMatch);
}

test(crypto_encoding_base58_to_bytes) {
    // Base58ToBeEncodedTestBytes
    // 21 bytes
    std::vector<uint8_t> Base58ToBeEncodedTestBytes = {
        30, 9, 149, 117, 2, 7, 236, 175, 12, 207, 37, 28, 18, 101, 185, 42, 216, 79, 85, 54, 98
    };

    // Base58EncodedTestStr
    // 34 characters
    const char *Base58EncodedTestStr = "D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib";

    // unencodedBytesBuffer
    // 21 bytes
    std::vector<uint8_t> unencodedBytesBuffer(21);

    Ark::Platform::Encoding::Base58::toBytes(&Base58EncodedTestStr[0], &unencodedBytesBuffer[0]);

    bool unencodedBytesMatch = Base58ToBeEncodedTestBytes == unencodedBytesBuffer;
    assertTrue(unencodedBytesMatch);
}

test(crypto_encoding_base58_from_pubkey_hash) {

    // 20 bytes
    std::vector<uint8_t> Ripemd160HashTestBytes = {
        9, 149, 117,   2,   7,
      236, 175,  12, 207,  37,
       28,  18, 101, 185,  42,
       216, 79,  85,  54,  98
    };

    const uint8_t networkVersion = 0x1E;
    const char *Base58EncodedTestStr = "D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib";


    char addressBuffer[35];
    Ark::Platform::Encoding::Base58::fromPubkeyHash(
        &Ripemd160HashTestBytes[0],
        networkVersion,
        &addressBuffer[0]);

    bool encodedStringsMatch = (strcmp(Base58EncodedTestStr, addressBuffer) == 0);
    assertTrue(encodedStringsMatch);
}

test(crypto_encoding_base58_to_pubkey_hash) {
  // 20 bytes
  std::vector<uint8_t> Ripemd160HashTestBytes = {
    9, 149, 117, 2, 7, 236, 175, 12, 207, 37, 28, 18, 101, 185, 42, 216, 79, 85, 54, 98
  };
  const uint8_t networkVersion = 0x1E;

  const char *Base58EncodedTestStr = "D61mfSggzbvQgTUe6JhYKH2doHaqJ3Dyib";

  uint8_t outVersion = 0;

  // unencodedBytesBuffer
  // 20 bytes
  std::vector<uint8_t> unencodedBytesBuffer(20);

  Ark::Platform::Encoding::Base58::toPubkeyHash(Base58EncodedTestStr, &unencodedBytesBuffer[0], &outVersion);

  bool hashBytesMatch = Ripemd160HashTestBytes == unencodedBytesBuffer;
  assertTrue(hashBytesMatch);
  assertEqual(outVersion, networkVersion);
}

test(crypto_encoding_base58_to_privatekey) {
  const char *WIFTestStr = "SGq4xLgZKCGxs7bjmwnBrWcT4C1ADFEermj846KC97FSv1WFD1dA";

  uint8_t wifByte = 0xaa;
  std::vector<uint8_t> privateKeyBytesBuffer(32);

  Ark::Platform::Encoding::Base58::toPrivateKey(WIFTestStr, &wifByte, &privateKeyBytesBuffer[0]);

  bool privateKeyBytesMatch = PrivateKeyTestBytes == privateKeyBytesBuffer;
  assertTrue(privateKeyBytesMatch);
}

test(crypto_encoding_base58_from_privatekey) {
  const char *WIFTestStr = "SGq4xLgZKCGxs7bjmwnBrWcT4C1ADFEermj846KC97FSv1WFD1dA";

  const char *PrivateKeyTestStr = "d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712";

  uint8_t wifByte = 0xaa;

  // wifStrBuffer
  // 52 bytes + 1-byte (null terminator)
  char wifStrBuffer[52 + 1];

  Ark::Platform::Encoding::Base58::fromPrivateKey(PrivateKeyTestStr, wifByte, wifStrBuffer);

  bool wifStringsMatch = (strcmp(WIFTestStr, wifStrBuffer) == 0);
  assertTrue(wifStringsMatch);
}

test(crypto_hashing_ripemd160_to_bytes) {
  std::vector<uint8_t> Ripemd160HashTestBytes = {
    9, 149, 117, 2, 7, 236, 175, 12, 207, 37, 28, 18, 101, 185, 42, 216, 79, 85, 54, 98
  };

  std::vector<uint8_t> hashBytesBuffer(20);

  Ark::Platform::Hashing::RIPEMD160::toBytes(&PublicKeyTestBytes[0], PublicKeyTestBytes.size(), &hashBytesBuffer[0]);

  bool hashBytesMatch = Ripemd160HashTestBytes == hashBytesBuffer;
  assertTrue(hashBytesMatch);
}

test(crypto_hashing_sha256_get) {
  std::vector<uint8_t> hashBuffer(32);
  Ark::Platform::Hashing::SHA256::get(&MessageTestBytes[0], MessageTestBytes.size(), &hashBuffer[0]);

  bool hashesMatch = MessageHashTestBytes == hashBuffer;
  assertTrue(hashesMatch);
}
