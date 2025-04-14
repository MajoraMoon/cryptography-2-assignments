#include "task.h"

/**
 *
 * C.2 DSA-Signaturen:
 *
 * (c) Erstellen Sie zwei DSA-Signaturen unter Verwendung derselben
 * Zufallszahl r (vgl. Übungsblatt 1, Aufgabe T.3). Berechnen
 * Sie den privaten DSA-Schlüssel ausgehend von diesen zwei DSA-
 * Signaturen.
 *
 */

/**
 *
 * Schlüsselgenerator nach FIPS 186-4 (also in der Vorlesung ist der Generator
 * alpha so wie ich es verstanden habe)
 * genrate_alphaa mit drei a, weil sonst der Linker meckert und es ist unnötig
 * dafür extra eine datei bzw Klasse zu kreieren
 *
 */

Botan::BigInt generate_alphaaa(Botan::BigInt &p, Botan::BigInt &q,
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

void execute_assignemt_2_c() {

  Botan::AutoSeeded_RNG rng;

  // Primzahlen generieren nach FIPS 186-4
  const size_t p_bits = 3072;
  const size_t q_bits = 256;

  // generiere q
  Botan::BigInt q = Botan::random_prime(rng, q_bits);

  // generiere p
  Botan::BigInt p;
  // Dieser For loop ist äquivalent zu "while(true)", also ein infinity for loop
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

  Botan::BigInt alpha = generate_alphaaa(p, q, rng);

  Botan::BigInt a = Botan::BigInt::random_integer(rng, 1, q - 1);
  Botan::BigInt beta = Botan::power_mod(alpha, a, p);

  // zwei verschiedene Nachrichten
  std::string msg1 = "Nachricht 1 mit gleichem k";
  std::string msg2 = "Nachricht 2 mit gleichem k";

  auto hash_message = [](const std::string &msg, Botan::BigInt q) {
    std::unique_ptr<Botan::HashFunction> hash(
        Botan::HashFunction::create("SHA-256"));
    hash->update(reinterpret_cast<const uint8_t *>(msg.data()), msg.size());
    return Botan::BigInt::decode(hash->final()) % q;
  };

  Botan::BigInt H1 = hash_message(msg1, q);
  Botan::BigInt H2 = hash_message(msg2, q);

  // Gemeinsames k für beide Signaturen
  Botan::BigInt k = Botan::BigInt::random_integer(rng, 1, q - 1);
  Botan::BigInt gamma = Botan::power_mod(alpha, k, p) % q;
  Botan::BigInt k_inv = Botan::inverse_mod(k, q);

  // Beide Signaturen berechnen
  Botan::BigInt delta1 = ((H1 + a * gamma) * k_inv) % q;
  Botan::BigInt delta2 = ((H2 + a * gamma) * k_inv) % q;

  // Privaten Schlüssel aus Signaturen rekonstruieren
  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\nAufgabe 2 c)" << "\n";

  Botan::BigInt delta_diff = (delta1 - delta2) % q;
  Botan::BigInt H_diff = (H1 - H2) % q;

  Botan::BigInt delta_diff_inv = Botan::inverse_mod(delta_diff, q);
  Botan::BigInt k_recovered = (H_diff * delta_diff_inv) % q;

  Botan::BigInt gamma_inv = Botan::inverse_mod(gamma, q);
  Botan::BigInt a_recovered = ((delta1 * k_recovered - H1) % q) * gamma_inv % q;

  // Ausgabe der Ergebnisse
  std::cout << "Originaler privater Schluessel:\n" << a.to_hex_string() << "\n";
  std::cout << "Berechneter privater Schluessel:\n"
            << a_recovered.to_hex_string() << "\n";

  if (a_recovered == a) {
    std::cout << "Schluessel korrekt rekonstruiert!\n";
  } else {
    std::cout << "Falsche Rekonstruktion!\n";
  }
}
