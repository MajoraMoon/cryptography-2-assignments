#include "task.h"

/**
 *
 * C.1 Universelle Faelschung von RSA-Signaturen:
 *
 *
 * (b) Implementieren Sie die universelle F¨alschung von RSA Signaturen
 * (vgl. Folie 19 der Vorlesung). W¨ahlen Sie hierf¨ur eine beliebige
 * Nachricht. Nehmen Sie bei der universellen F¨alschung sowohl
 * die Rolle des Angreifers als auch die des Signierers (Orakel) ein.
 * Pruefen Sie mit dem RSA-Verifikationsalgorithmus, dass die so
 * berechneten RSA-Signaturen korrekt verifiziert werden.
 */

/**
 *
 * We take the role of the attacker and the role of the oracle.
 *
 * The public key consists  out of (n, e) and the private key consists out of
 * (p,q,d).
 *
 * Universal Forgery: An attacker can produce a valid signature s for a chosen
 * message m, without knowing the private key directly.
 *
 * The attacker choses a random number r, calcualtes m and gets the signature s'
 * for m' from the oracle. Then the attacker can calculate the signatur s for m
 * out of s'.
 */

using namespace Botan;

void execute_assignment_1_b() {

  AutoSeeded_RNG rng;

  // key generation
  RSA_PrivateKey private_key(rng, 3000);

  // important data preperation for calcuations
  // public parameters
  BigInt n = private_key.get_n(); // -> here its n = p * q
  BigInt e = private_key.get_e();
  // private parameter
  BigInt d = private_key.get_d();

  // Choosing a message
  BigInt m("98765432123456789");

  // choose a big integer r with r^e mod n ≠ 1
  BigInt r;
  BigInt r_e_mod_n;

  do {
    r = BigInt::random_integer(rng, 2, n - 1);

    r_e_mod_n = power_mod(r, e, n);
  } while (r_e_mod_n == 1);

  // define  m' as: m' = (r^e * m) mod n
  BigInt m_prime = (r_e_mod_n * m) % n;

  /**
   * the oracle (which knows the private key) signs m', so it calculates
   * s' = (m')^d mod n
   */
  BigInt s_prime = power_mod(m_prime, d, n);

  // The attacker now calculates: s = r⁻¹ · s' mod n.
  // Because: s' = (r^e · m)^d = r^(e·d) · m^d mod n
  BigInt r_inv = inverse_mod(r, n);
  BigInt s = (r_inv * s_prime) % n;

  // verification:  m ≡ s^e mod n
  BigInt verified = power_mod(s, e, n);

  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\nAufgabe 1 b)"
            << "\n";

  std::cout << "öffentliche Komponente n: " << n << "\n";
  std::cout << "öffentliche Komponente e: " << e << "\n\n";

  std::cout << "Nachricht m: " << m << "\n\n";
  std::cout << "Signatur s' (m'^d mod n): " << s_prime << "\n\n";
  std::cout << "Berechnete gefälschte Signatur s (r^-1 * s' mod n): " << s
            << "\n\n";
  std::cout << "Verifikation: s^e mod n = " << verified << "\n\n";

  if (verified == m) {
    std::cout << "Verifikation erfolgreich. Die "
                 "Gefälschte Signatur ist valide\n";
  } else {
    std::cout << "Verifikation gescheitert.\n";
  }
}