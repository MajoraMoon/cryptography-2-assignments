#include "task.h"

using namespace Botan;

void execute_task_1_b() {
  AutoSeeded_RNG rng;

  // 1. RSA-Schlüssel erzeugen
  RSA_PrivateKey private_key(rng, 2048);
  RSA_PublicKey public_key = private_key;

  BigInt n = public_key.get_n();
  BigInt e = public_key.get_e();
  BigInt d = private_key.get_d();

  // 2. Nachricht m
  BigInt m("42");

  // 3. Zufällige invertierbare Zahl r (mit gcd(r, n) = 1)
  BigInt r;
  do {
    r = BigInt::random_integer(rng, 2, n - 1);
  } while (gcd(r, n) != 1);

  // 4. m' = r^e * m mod n
  BigInt m_prime = power_mod(r, e, n) * m % n;

  // 5. Signatur vom Orakel (eigentlich m'^d mod n)
  BigInt s_prime = power_mod(m_prime, d, n);

  // 6. Berechne s = r^-1 * s' mod n
  BigInt r_inv = inverse_mod(r, n);
  BigInt s = r_inv * s_prime % n;

  // 7. Verifikation: s^e mod n == m?
  BigInt verified = power_mod(s, e, n);

  std::cout << "Original message:   " << m << std::endl;
  std::cout << "Forged signature:   " << s << std::endl;
  std::cout << "Verification result: " << verified << std::endl;

  if (verified == m) {
    std::cout << "✅ Signature successfully forged and verified!" << std::endl;
  } else {
    std::cout << "❌ Verification failed!" << std::endl;
  }
}