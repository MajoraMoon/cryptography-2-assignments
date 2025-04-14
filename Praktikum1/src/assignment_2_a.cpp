#include "task.h"

/**
 *
 * C.2 DSA-Signaturen:
 *
 * (a) Implementieren Sie die DSA-Schlüsselgenerierung (vgl. Folie 33
 * der Vorlesung). Geben Sie alle verwendeten DSA-Schlüsselparameter
 * aus. Hinweis: Verwenden Sie den Algorithmus aus Appendix A.2.1 von
 * https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.186-4.
 * pdf für die Bestimmung eines Generators.
 *
 */

// Schlüsselgenerator nach FIPS 186-4 (also in der Vorlesung ist der Generator
// alpha so wie ich es verstanden habe)
Botan::BigInt generate_alpha(Botan::BigInt &p, Botan::BigInt &q,
                             Botan::RandomNumberGenerator &rng) {

  Botan::BigInt e = (p - 1) / q;

  Botan::BigInt h, g;

  do {
    // Botan inkludiert die unter UND obergrenze, deshalb muss hier 2 anstatt 1
    // stehen, weil h nicht gleich größer/kleiner 1 ist , sondern echt
    // größer/kleiner 1
    h = Botan::BigInt::random_integer(rng, 2, p - 2);

    g = Botan::power_mod(h, e, p);

  } while (g == 1);

  return g;
}

void execute_assignment_2_a() {

  Botan::AutoSeeded_RNG rng;

  // Primzahlen generieren nach FIPS 186-4
  const size_t p_bits = 3072;
  const size_t q_bits = 256;

  // generiere q
  Botan::BigInt q = Botan::random_prime(rng, q_bits);

  // generiere p

  Botan::BigInt p;
  for (;;) {
    // random zahl c wird genommen
    Botan::BigInt c = Botan::BigInt::random_integer(
        rng, 1, Botan::BigInt::power_of_2(p_bits - q_bits));
    // p - 1 = c * q also ist p = c * q + 1
    p = c * q + 1;

    if (p.bits() == p_bits && Botan::is_prime(p, rng)) {
      break;
    }
  }

  Botan::BigInt alpha = generate_alpha(p, q, rng);

  Botan::BigInt a = Botan::BigInt::random_integer(rng, 1, q - 1);
  Botan::BigInt beta = Botan::power_mod(alpha, a, p);
  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\nAufgabe 2 a)" << "\n";
  std::cout << "DSA-Schluesselparameter:\n"
            << "p: " << p.to_hex_string() << "\n\n"
            << "q: " << q.to_hex_string() << "\n\n"
            << "α: " << alpha.to_hex_string() << "\n\n"
            << "Privater Schluessel a: " << a.to_hex_string() << "\n\n"
            << "Oeffentlicher Schluessel : " << beta.to_hex_string() << "\n";
}